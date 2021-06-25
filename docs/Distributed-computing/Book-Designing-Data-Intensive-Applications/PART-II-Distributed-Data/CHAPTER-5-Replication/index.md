# CHAPTER 5 Replication

There are several reasons why you might want to replicate data:

- To keep data geographically close to your users (and thus reduce latency)

- To allow the system to continue working even if some of its parts have failed (and thus increase availability)

- To scale out the number of machines that can serve read queries (and thus increase read throughput)

  > NOTE: 读写分离

All of the difficulty in replication lies in handling changes to replicated data, and that’s what this chapter is about. We will discuss three popular algorithms for replicating *changes* between nodes: *single-leader*, *multi-leader*, and *leaderless replication*. Almost all distributed databases use one of these three approaches. They all have various pros and cons, which we will examine in detail.

There are many trade-offs to consider with replication: for example, whether to use **synchronous** or **asynchronous** replication, and how to handle failed replicas. Those are often configuration options in databases, and although the details vary by database, the general principles are similar across many different implementations.

In “Problems with Replication Lag” on page 161 we will get more precise about eventual consistency and discuss things like the read-your-writes and monotonic reads guarantees.

