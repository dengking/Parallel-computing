# [Weak consistency](https://en.wikipedia.org/wiki/Weak_consistency)

The name **weak consistency** may be used in two senses. In the first sense, strict and more popular, the **weak consistency** is one of the [consistency models](https://en.wikipedia.org/wiki/Consistency_model) used in the domain of the [concurrent programming](https://en.wikipedia.org/wiki/Concurrent_programming) (e.g. in [distributed shared memory](https://en.wikipedia.org/wiki/Distributed_shared_memory), [distributed transactions](https://en.wikipedia.org/wiki/Distributed_transactions) etc.).

The protocol is said to support **weak consistency** if:

1. All accesses to **synchronization variables** are seen by all processes (or nodes, processors) in the same order (sequentially) - these are synchronization operations. Accesses to critical sections are seen sequentially.
2. All other accesses may be seen in different order on different processes (or nodes, processors).
3. The set of both read and write operations in between different synchronization operations is the same in each process.

Therefore, there can be no access to a synchronization variable if there are pending write operations. And there can not be any new read/write operation started if system is performing any synchronization operation.

In the second, more general, sense **weak consistency** may be applied to any consistency model weaker than [sequential consistency](https://en.wikipedia.org/wiki/Sequential_consistency).

A stricter condition is [strong consistency](https://en.wikipedia.org/wiki/Strong_consistency), where parallel processes can observe **only one consistent state**.

