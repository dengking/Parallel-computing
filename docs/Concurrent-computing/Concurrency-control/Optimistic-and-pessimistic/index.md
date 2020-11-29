# Optimistic and pessimistic

## wikipedia [Optimistic concurrency control](https://en.wikipedia.org/wiki/Optimistic_concurrency_control)

**Optimistic concurrency control** (**OCC**) is a [concurrency control](https://en.wikipedia.org/wiki/Concurrency_control) method applied to transactional systems such as [relational database management systems](https://en.wikipedia.org/wiki/Relational_database_management_systems) and [software transactional memory](https://en.wikipedia.org/wiki/Software_transactional_memory). OCC assumes that multiple transactions can frequently complete without interfering(干涉) with each other. While running, **transactions** use data resources without acquiring **locks** on those resources. Before committing, each transaction verifies that no other transaction has modified the data it has read. If the check reveals conflicting modifications, the committing transaction rolls back and can be restarted.[[1\]](https://en.wikipedia.org/wiki/Optimistic_concurrency_control#cite_note-1) Optimistic concurrency control was first proposed by [H. T. Kung](https://en.wikipedia.org/wiki/H._T._Kung) and John T. Robinson.[[2\]](https://en.wikipedia.org/wiki/Optimistic_concurrency_control#cite_note-KungRobinson1981-2)

> NOTE: 通过上面描述的策略可以看出，显然相比于lock，optimistic concurrency control允许更大的concurrency。

OCC is generally used in environments with low [data contention](https://en.wikipedia.org/wiki/Block_contention)(数据竞争). When conflicts are rare, **transactions** can complete without the expense of managing locks and without having transactions wait for other transactions' locks to clear, leading to higher throughput than other **concurrency control** methods. However, if **contention** for data resources is frequent, the cost of repeatedly restarting transactions hurts performance significantly; it is commonly thought[*[who?](https://en.wikipedia.org/wiki/Wikipedia:Manual_of_Style/Words_to_watch#Unsupported_attributions)*] that other [concurrency control](https://en.wikipedia.org/wiki/Concurrency_control) methods have better performance under these conditions.[*[citation needed](https://en.wikipedia.org/wiki/Wikipedia:Citation_needed)*] However, **locking-based** ("pessimistic") methods also can deliver poor performance because locking can drastically limit effective concurrency even when deadlocks are avoided.

> NOTE: lock-based method是pessimistic的。

### Examples

> NOTE: 下来列举了一些我知道的

- [Redis](https://en.wikipedia.org/wiki/Redis) provides OCC through WATCH command.[[16\]](https://en.wikipedia.org/wiki/Optimistic_concurrency_control#cite_note-16)
- [MySQL](https://en.wikipedia.org/wiki/MySQL) implements OCC in Group Replication configuration.[*[citation needed](https://en.wikipedia.org/wiki/Wikipedia:Citation_needed)*]



## wikipedia [Transactional memory#Motivation](https://en.wikipedia.org/wiki/Transactional_memory#Motivation)

In concurrent programming, synchronization is required when parallel threads attempt to access a shared resource. Low level thread synchronization constructs such as locks are pessimistic and prohibit threads that are outside a [critical section](https://en.wikipedia.org/wiki/Critical_section) from making any changes. The process of applying and releasing locks often functions as additional overhead in workloads with little conflict among threads. Transactional memory provides [optimistic concurrency control](https://en.wikipedia.org/wiki/Optimistic_concurrency_control) by allowing threads to run in parallel with minimal interference.[[2\]](https://en.wikipedia.org/wiki/Transactional_memory#cite_note-:0-2) The goal of transactional memory systems is to transparently support regions of code marked as transactions by enforcing [atomicity](https://en.wikipedia.org/wiki/Atomicity_(database_systems)), [consistency](https://en.wikipedia.org/wiki/Consistency_(database_systems)) and [isolation](https://en.wikipedia.org/wiki/Isolation_(database_systems)).



## optimistic lock

https://stackoverflow.com/questions/129329/optimistic-vs-pessimistic-locking

https://en.wikipedia.org/wiki/Optimistic_concurrency_control

## redis [Transactions](https://redis.io/topics/transactions)

## wikipedia [Optimistic replication](https://en.wikipedia.org/wiki/Optimistic_replication)



## Optimistic and pessimistic synchronization in distributed computing

https://link.springer.com/chapter/10.1007/BFb0024159