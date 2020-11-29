# Optimistic and pessimistic

## wikipedia [Optimistic concurrency control](https://en.wikipedia.org/wiki/Optimistic_concurrency_control)

**Optimistic concurrency control** (**OCC**) is a [concurrency control](https://en.wikipedia.org/wiki/Concurrency_control) method applied to transactional systems such as [relational database management systems](https://en.wikipedia.org/wiki/Relational_database_management_systems) and [software transactional memory](https://en.wikipedia.org/wiki/Software_transactional_memory). OCC assumes that multiple transactions can frequently complete without interfering(干涉) with each other. While running, **transactions** use data resources without acquiring **locks** on those resources. Before committing, each transaction verifies that no other transaction has modified the data it has read. If the check reveals conflicting modifications, the committing transaction rolls back and can be restarted.[[1\]](https://en.wikipedia.org/wiki/Optimistic_concurrency_control#cite_note-1) Optimistic concurrency control was first proposed by [H. T. Kung](https://en.wikipedia.org/wiki/H._T._Kung) and John T. Robinson.[[2\]](https://en.wikipedia.org/wiki/Optimistic_concurrency_control#cite_note-KungRobinson1981-2)

> NOTE: 通过上面描述的策略可以看出，显然相比于lock，optimistic concurrency control允许更大的concurrency。





# optimistic lock

https://stackoverflow.com/questions/129329/optimistic-vs-pessimistic-locking

https://en.wikipedia.org/wiki/Optimistic_concurrency_control

# redis [Transactions](https://redis.io/topics/transactions)

# [Optimistic replication](https://en.wikipedia.org/wiki/Optimistic_replication)

# Optimistic and pessimistic synchronization in distributed computing

https://link.springer.com/chapter/10.1007/BFb0024159