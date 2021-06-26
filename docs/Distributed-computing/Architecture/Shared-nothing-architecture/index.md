# Shared-nothing architecture



## wanweibaike [Shared-nothing architecture](https://en.wanweibaike.com/wiki-Shared-nothing%20architecture)

A **shared-nothing architecture** (**SN**) is a [distributed computing](https://en.wanweibaike.com/wiki-Distributed_computing) architecture in which each update request is satisfied by a single node (processor/memory/storage unit). The intent is to eliminate contention among nodes. Nodes do not share (independently access) the same memory or storage. One alternative architecture is shared everything, in which requests are satisfied by arbitrary combinations of nodes. This may introduce contention, as multiple nodes may seek to update the same data at the same time.

> NOTE: 
>
> 两种完全的极端:
>
> 1、shared-nothing architecture 
>
> no contention
>
> 2、shared-everything architecture
>
> 

SN eliminates [single points of failure](https://en.wanweibaike.com/wiki-Single_point_of_failure), allowing the overall system to continue operating despite failures in individual nodes and allowing individual nodes to upgrade without a system-wide shutdown.

> NOTE: 
>
> 一、SN architecture的优势:
>
> 1、eliminates [single points of failure](https://en.wanweibaike.com/wiki-Single_point_of_failure)
>
> 2、能够独立升级
>
> 3、scalability
>
> 这是下面这一段提出的

A SN system can scale simply by adding nodes, since no central resource bottlenecks the system.[[2\]](https://en.wanweibaike.com/wiki-Shared-nothing architecture#cite_note-2) In databases, another term for SN is *[sharding](https://en.wanweibaike.com/wiki-Sharding)*. A SN system typically partitions its data among many nodes. A refinement is to replicate commonly used but infrequently modified data across many nodes, allowing more requests to be resolved on a single node.

> NOTE: 
>
> 关于 *[sharding](https://en.wanweibaike.com/wiki-Sharding)*，参见 `Distributed-data-store\Data-Sharding-Strategy` 章节

## TODO

colinbreck [Shared-Nothing Architectures for Server Replication and Synchronization](https://blog.colinbreck.com/shared-nothing-architectures-for-server-replication-and-synchronization/)

