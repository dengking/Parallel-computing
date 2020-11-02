# [ZooKeeper: A Distributed Coordination Service for Distributed Applications](https://zookeeper.apache.org/doc/current/zookeeperOver.html)

## Design Goals

### ZooKeeper is simple.

ZooKeeper allows distributed processes to coordinate with each other through a shared hierarchical namespace which is organized similarly to a standard file system.

> NOTE:  这是ZooKeeper的simple特性的内涵: 使用容易理解的data model来实现distributed processes的coordination。data model在后面的章节会进行描述。

The **reliability** aspects keep it from being a single point of failure. 

The **strict ordering** means that sophisticated synchronization primitives can be implemented at the client.

> NOTE: 由client来实现synchronization？



### ZooKeeper is replicated

![ZooKeeper Service](https://zookeeper.apache.org/doc/current/images/zkservice.jpg)

The servers that make up the ZooKeeper service must all know about each other.

> NOTE: 从上图可以看出，每个slave server只和leader server进行连接，而slave server之间并没有connection，那这并不是上面这段话中所描述的"know about each other"

### ZooKeeper is ordered

ZooKeeper stamps(它的含义非常类似于timestamp) each update with a number that reflects the order of all ZooKeeper transactions. Subsequent operations can use the order to implement higher-level abstractions, such as synchronization primitives.

> NOTE: 这种技术叫什么名称？
>
> 上述update具体是指什么？

### ZooKeeper is fast

It is especially fast in "read-dominant" workloads.

> NOTE: 为什么是"read"？

## Data model and the hierarchical namespace

### ZooKeeper's Hierarchical Namespace

![ZooKeeper's Hierarchical Namespace](https://zookeeper.apache.org/doc/current/images/zknamespace.jpg)



## Nodes and ephemeral(短暂的) nodes

(ZooKeeper was designed to store **coordination data**: status information, configuration, location information, etc., so the data stored at each node is usually small, in the byte to kilobyte range.) 

> NOTE: 上述**coordination data**，可以是consensus

We use the term *znode* to make it clear that we are talking about ZooKeeper data nodes.

**Znodes** maintain a **stat structure** that includes **version numbers** for data changes, ACL changes, and timestamps, to allow **cache validations** and **coordinated updates**. Each time a znode's data changes, the version number increases. For instance, whenever a client retrieves data it also receives the version of the data.

> NOTE: **stat structure**的含义"统计结构"。

The data stored at each znode in a namespace is read and written **atomically**. Reads get all the data bytes associated with a znode and a write replaces all the data. Each node has an Access Control List (ACL) that restricts who can do what.

ZooKeeper also has the notion of ephemeral nodes. These znodes exists as long as the session that created the znode is active. When the session ends the znode is deleted.

## Conditional updates and watches

ZooKeeper supports the concept of *watches*. Clients can set a watch on a znode.



## Guarantees



## Simple API

One of the design goals of ZooKeeper is providing a very simple programming interface. As a result, it supports only these operations:

- *create* : creates a node at a location in the tree
- *delete* : deletes a node
- *exists* : tests if a node exists at a location
- *get data* : reads the data from a node
- *set data* : writes data to a node
- *get children* : retrieves a list of children of a node
- *sync* : waits for data to be propagated

> NOTE: 就是对tree的操作



