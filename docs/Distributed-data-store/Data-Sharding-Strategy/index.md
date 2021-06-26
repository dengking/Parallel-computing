# Data sharding strategy

本章讨论在distributed data store中涉及的data sharding strategy。

## wanweibaike [Partition (database)](https://en.wanweibaike.com/wiki-Partition%20(database))

A **partition** is a division of a logical [database](https://en.wanweibaike.com/wiki-Database) or its constituent elements into distinct independent parts. Database partitioning is normally done for manageability, [performance](https://en.wanweibaike.com/wiki-Optimization_(computer_science)) or [availability](https://en.wanweibaike.com/wiki-Availability)[[1\]](https://en.wanweibaike.com/wiki-Partition (database)#cite_note-:0-1) reasons, or for [load balancing](https://en.wanweibaike.com/wiki-Load_balancing_(computing)).

### Partitioning criteria

They take a *partitioning key* and assign a partition based on certain criteria. Some common criteria include:

**Range partitioning**

**Round-robin partitioning**

### Partitioning methods

**Horizontal partitioning** 

> NOTE: 
>
> 水平

**Vertical partitioning** 

> NOTE: 
>
> 垂直

## wanweibaike [Shard (database architecture)](https://en.wanweibaike.com/wiki-Sharding) 

A **database shard**, or simply a **shard**, is a [horizontal partition](https://en.wanweibaike.com/wiki-Partition_(database)) of data in a [database](https://en.wanweibaike.com/wiki-DBMS) or [search engine](https://en.wanweibaike.com/wiki-Search_engine). Each shard is held on a separate [database server](https://en.wanweibaike.com/wiki-Database_server) instance, to spread load.

### Notable implementations

> NOTE: 
>
> 可以看到，主流的都是采用的shared，即horizontal partition

#### Google [Spanner](https://en.wanweibaike.com/wiki-Spanner_(database))

[Spanner](https://en.wanweibaike.com/wiki-Spanner_(database)), Google's global-scale distributed database, shards across multiple [Paxos](https://en.wanweibaike.com/wiki-Paxos_(computer_science)) state machines to scale to "millions of machines across hundreds of data centers and trillions of database rows".[[19\]](https://en.wanweibaike.com/wiki-Sharding#cite_note-Spanner-20)





## TODO

gitbook [Systems Design Glossary](https://aaronice.gitbook.io/system-design/distributed-systems)

