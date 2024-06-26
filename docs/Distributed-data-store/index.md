# Distributed data store

感觉distributed data store/partition一定程度上是源自于**分而治之思想**。



## wikipedia [Distributed data store](https://en.wikipedia.org/wiki/Distributed_data_store)

A **distributed data store** is a [computer network](https://en.wikipedia.org/wiki/Computer_network) where information is stored on more than one [node](https://en.wikipedia.org/wiki/Node_(networking)), often in a [replicated](https://en.wikipedia.org/wiki/Replication_(computing)) fashion.[[1\]](https://en.wikipedia.org/wiki/Distributed_data_store#cite_note-1) It is usually specifically used to refer to either a [distributed database](https://en.wikipedia.org/wiki/Distributed_database) where users store information on a *number of nodes*, or a [computer network](https://en.wikipedia.org/wiki/Computer_network) in which users store information on a *number of peer network nodes*.

### Distributed databases

[Distributed databases](https://en.wikipedia.org/wiki/Distributed_database) are usually [non-relational databases](https://en.wikipedia.org/wiki/Non-relational_database) that enable a quick access to data over a large number of nodes. Some distributed databases expose rich query abilities while others are limited to a [key-value store](https://en.wikipedia.org/wiki/Key-value_store) semantics. Examples of limited distributed databases are [Google](https://en.wikipedia.org/wiki/Google)'s [Bigtable](https://en.wikipedia.org/wiki/Bigtable), which is much more than a [distributed file system](https://en.wikipedia.org/wiki/Distributed_file_system) or a [peer-to-peer network](https://en.wikipedia.org/wiki/Peer-to-peer_network),[[2\]](https://en.wikipedia.org/wiki/Distributed_data_store#cite_note-2) [Amazon](https://en.wikipedia.org/wiki/Amazon.com)'s [Dynamo](https://en.wikipedia.org/wiki/Dynamo_(storage_system))[[3\]](https://en.wikipedia.org/wiki/Distributed_data_store#cite_note-3) and [Windows Azure Storage](https://en.wikipedia.org/wiki/Azure_Services_Platform).[[4\]](https://en.wikipedia.org/wiki/Distributed_data_store#cite_note-4)

As the ability of arbitrary querying is not as important as the [availability](https://en.wikipedia.org/wiki/Availability), designers of distributed data stores have increased the latter at an expense of **consistency**. But the high-speed read/write access results in reduced consistency, as it is not possible to have both [consistency](https://en.wikipedia.org/wiki/Consistency_(database_systems)), availability, and **partition tolerance of the network**, as it has been proven by the [CAP theorem](https://en.wikipedia.org/wiki/CAP_theorem).



## Application

### 分库分表

zhuanlan [MySQL：互联网公司常用分库分表方案汇总！](https://zhuanlan.zhihu.com/p/137368446)

内容较好

csdn [彻底搞清分库分表（垂直分库，垂直分表，水平分库，水平分表）](https://blog.csdn.net/weixin_44062339/article/details/100491744)

meituan [大众点评订单系统分库分表实践](https://tech.meituan.com/2016/11/18/dianping-order-db-sharding.html)

### Redis 

redis [Partitioning: how to split data among multiple Redis instances.](https://redis.io/topics/partitioning)

redis [Redis cluster tutorial](https://redis.io/topics/cluster-tutorial)

### Distributed hash table

参见 `Distributed-hash-table` 章节。
