# wikipedia [Strong consistency](https://en.wikipedia.org/wiki/Strong_consistency)

**Strong consistency** is one of the [consistency models](https://en.wikipedia.org/wiki/Consistency_model) used in the domain of the [concurrent programming](https://en.wikipedia.org/wiki/Concurrent_programming) (e.g., in [distributed shared memory](https://en.wikipedia.org/wiki/Distributed_shared_memory), [distributed transactions](https://en.wikipedia.org/wiki/Distributed_transactions)).

The protocol is said to support strong consistency if:

1. All accesses are seen by all parallel processes (or nodes, processors, etc.) in the same order (sequentially)

Therefore, **only one consistent state** can be observed, as opposed to [weak consistency](https://en.wikipedia.org/wiki/Weak_consistency), where different parallel processes (or nodes, etc.) can perceive（感知） variables in different states.

