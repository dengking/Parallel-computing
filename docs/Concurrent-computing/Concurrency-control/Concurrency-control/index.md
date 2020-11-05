# Concurrency control

Concurrency control无处不在。

## wikipedia [Concurrency control](https://en.wikipedia.org/wiki/Concurrency_control)

In information technology and computer science, especially in the fields of computer programming, operating systems, multiprocessors, and databases, **concurrency control** ensures that correct results for concurrent operations are generated, while getting those results as quickly as possible.

> NOTE: concurrency control存在于computer science的各个领域:
>
> 1) programming language
>
> 2) operating system
>
> 3) multiprocessor
>
> 4) database
>
> 上面这段话的最后一句总结了concurrency control的目标:
>
> 1) 保证correct
>
> 2) 保证performance



Computer systems, both software and hardware, consist of modules, or components. Each component is
designed to operate correctly, i.e., to obey or to meet certain consistency rules. When components that operate
concurrently interact by messaging or by sharing accessed data (in memory or storage), a certain component's
consistency may be violated by another component. The general area of concurrency control provides rules,
methods, design methodologies, and theories to maintain the consistency of components operating
concurrently while interacting, and thus the consistency and correctness of the whole system. Introducing
concurrency control into a system means applying operation constraints which typically result in some
performance reduction. Operation consistency and correctness should be achieved with as good as possible
efficiency, without reducing performance below reasonable levels. Concurrency control can require significant
additional complexity and overhead in a concurrent algorithm compared to the simpler sequential algorithm.

Concurrency control can require significant additional complexity and overhead in a [concurrent algorithm](https://en.wikipedia.org/wiki/Concurrent_algorithm) compared to the simpler [sequential algorithm](https://en.wikipedia.org/wiki/Sequential_algorithm).

> NOTE: concurrent algorithm VS sequential algorithm

### Concurrency control in databases

#### Concurrency control mechanisms

##### Categories

- [Optimistic](https://en.wikipedia.org/wiki/Optimistic_concurrency_control)
- Pessimistic 
- Semi-optimistic 

##### Methods

- Locking (e.g., **[Two-phase locking](https://en.wikipedia.org/wiki/Two-phase_locking)** - 2PL)
- Serialization [graph checking](https://en.wikipedia.org/wiki/Serializability#Testing_conflict_serializability)
- [Timestamp ordering](https://en.wikipedia.org/wiki/Timestamp-based_concurrency_control) (TO)
- [Commitment ordering](https://en.wikipedia.org/wiki/Commitment_ordering) 



## wikipedia [Distributed concurrency control](https://en.wikipedia.org/wiki/Distributed_concurrency_control)



### Concurrency control in operating systems

## wikipedia [Category:Concurrency control](https://en.wikipedia.org/wiki/Category:Concurrency_control)



## wikipedia [Category:Concurrency control algorithms](https://en.wikipedia.org/wiki/Category:Concurrency_control_algorithms)





## TODO





[Double-checked locking](https://en.wikipedia.org/wiki/Double-checked_locking)

[Test and test-and-set](https://en.wikipedia.org/wiki/Test_and_test-and-set)

[Consistency in Distributed Systems](https://www.microsoft.com/en-us/research/wp-content/uploads/2016/06/printversion.pdf)

