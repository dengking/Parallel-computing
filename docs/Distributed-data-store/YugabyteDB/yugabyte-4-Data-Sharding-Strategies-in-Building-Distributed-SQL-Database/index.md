# yugabyte [Four Data Sharding Strategies We Analyzed in Building a Distributed SQL Database](https://blog.yugabyte.com/four-data-sharding-strategies-we-analyzed-in-building-a-distributed-sql-database)

> NOTE: 
>
> 对各种策略总结地比较好

A distributed SQL database needs to automatically partition the data in a table and distribute it across nodes. This is known as **data sharding** and it can be achieved through different strategies, each with its own tradeoffs. In this post, we will examine various data sharding strategies for a distributed SQL database, analyze the tradeoffs, explain the rationale for which of these strategies YugabyteDB supports and what we picked as the default **sharding strategy**.

## Lessons from building sharded data systems

Data sharding helps in scalability and geo-distribution by horizontally partitioning data. A SQL table is decomposed into multiple sets of rows according to a specific sharding strategy. Each of these sets of rows is called a shard. These shards are distributed across multiple server nodes (containers, VMs, bare-metal) in a **shared-nothing architecture**. This ensures that the shards do not get bottlenecked by the compute, storage and networking resources available at a single node. High availability is achieved by replicating each shard across multiple nodes. However, the application interacts with a SQL table as one logical unit and remains agnostic(不知) to the physical placement of the shards. In this section, we will outline the pros, cons and our practical learnings from the sharding strategies adopted by these databases.

### Memcached and Redis – Algorithmic Sharding

**Distributed caches** have had to distribute data across multiple nodes for a while. A commonly used technique is **algorithmic sharding**, where each key consistently maps to the same node. This is achieved by computing a numeric hash value out of the key and computing a modulo of that hash using the total number of nodes to compute which node owns the key.



![](./image2.png)

> *Part of the image from source: [How Sharding Works](https://medium.com/@jeeyoungk/how-sharding-works-b4dec46b3f6)*

**Pros**
In algorithmic sharding, the client can determine a given partition’s database without any help.

**Cons**
When a new node is added or removed, the ownership of almost all keys would be affected, resulting in a massive redistribution of all the data across nodes of the cluster. While this is not a correctness issue in a distributed cache (because cache misses will repopulate the data), it can have a huge performance impact since the entire cache will have to be warmed again.

**Analysis**
Adding and removing nodes is fundamental to a distributed database, and these operations need to be efficient.

This makes this type of sharding a poor option and is not implemented in YugabyteDB.

**References:**

1、Sharding data across a memcache tier from “[Distributed Caching with Memcached”](https://www.linuxjournal.com/article/7451)

2、Algorithmic sharding from “[How Sharding Works”](https://medium.com/@jeeyoungk/how-sharding-works-b4dec46b3f6)
