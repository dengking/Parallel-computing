# [Linearizability](https://en.wikipedia.org/wiki/Linearizability)

In [concurrent programming](https://en.wikipedia.org/wiki/Concurrent_programming), an operation (or set of operations) is **linearizable** if it consists of an ordered list of invocation and **response events** ([callbacks](https://en.wikipedia.org/wiki/Callbacks)), that may be extended by adding **response events** such that:

1. The extended list can be re-expressed as a **sequential history** (is [serializable](https://en.wikipedia.org/wiki/Serializability)), and
2. That **sequential history** is a subset of the original unextended list.

Informally, this means that the unmodified list of events is linearizable [if and only if](https://en.wikipedia.org/wiki/If_and_only_if) its invocations were [serializable](https://en.wikipedia.org/wiki/Serializability), but some of the responses of the serial schedule have yet to return.[[1\]](https://en.wikipedia.org/wiki/Linearizability#cite_note-:0-1)

In a concurrent system, processes can access a shared object at the same time. Because multiple processes are accessing a single object, there may arise a situation in which while one process is accessing the object, another process changes its contents. This example demonstrates the need for linearizability. In a linearizable system although operations overlap on a shared object, each operation appears to take place instantaneously. Linearizability is a strong correctness condition, which constrains what outputs are possible when an object is accessed by multiple processes concurrently. It is a safety property which ensures that operations do not complete in an unexpected or unpredictable manner. If a system is linearizable it allows a programmer to reason about the system.[[2\]](https://en.wikipedia.org/wiki/Linearizability#cite_note-2)

***TRANSLATION*** : 在并发系统中，进程可以同时访问共享对象。由于多个进程正在访问单个对象，因此可能会出现这样的情况：当一个进程正在访问该对象时，另一个进程会更改其内容。此示例演示了线性化的必要性。在可线性化系统中，尽管操作在共享对象上重叠，但每个操作似乎都是即时进行的。线性化是一种强正确性条件，它限制了当多个进程同时访问对象时可能的输出。它是一种安全属性，可确保操作不会以意外或不可预测的方式完成。如果系统是可线性化的，则允许程序员推理系统。





![img](https://upload.wikimedia.org/wikipedia/commons/thumb/c/c3/Linearlizable_Process.svg/220px-Linearlizable_Process.svg.png)





In grey a linear sub-history, processes beginning in b do not have a linearizable history because b0 or b1 may complete in either order before b2 occurs.





## History of linearizability

Linearizability was first introduced as a [consistency model](https://en.wikipedia.org/wiki/Consistency_model) by [Herlihy](https://en.wikipedia.org/wiki/Maurice_Herlihy) and [Wing](https://en.wikipedia.org/wiki/Jeannette_Wing) in 1987. It encompassed(包含) more restrictive definitions of atomic, such as "an atomic operation is one which cannot be (or is not) interrupted by concurrent operations", which are usually vague(模糊的) about when an operation is considered to begin and end.

An atomic object can be understood immediately and completely from its sequential definition, as a set of operations run in parallel which always appear to occur one after the other; no inconsistencies may emerge. Specifically, linearizability guarantees that the [invariants](https://en.wikipedia.org/wiki/Invariant_(computer_science)) of a system are *observed* and *preserved* by all operations: if all operations individually preserve an invariant, the system as a whole will.

