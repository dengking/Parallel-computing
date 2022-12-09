# Linearizability

"linearizability"的中文含义是 "线性一致性"。

## stackoverflow [What is "Linearizability"?](https://stackoverflow.com/questions/9762101/what-is-linearizability)

Can anyone out there help me understand what **Linearizability** is? I need an explanation that is simple and easy to understand. I'm reading *The Art of Multiprocessor Programming* by Maruice Herilihy and Nir Shavit and am trying to understand Chapter 3 about Concurrent Objects.

I understand that a method is Linearizable if it has a point where it seems to "take effect" instantaneously(瞬时的) from the point of view of the other threads. That makes sense, but it is also said that Linearizability is actually a property of the **execution history**. What does it mean for an **execution history** to be Linearizable, why do I care, and how does it relate to a method or object being Linearizable?

Thank you!

### [A](https://stackoverflow.com/a/50653833/10173843)

A picture is worth 1000 words.

[![enter image description here](https://i.stack.imgur.com/gGYMq.png)](https://i.stack.imgur.com/gGYMq.png)

The first `SELECT` statement reads the value of 50, while the second `SELECT` reads the value of 10 since in between the two read operations a write operation was executed.

Linearizability means that modifications happen instantaneously, and once a **registry value** is written, any subsequent read operation will find the very same value as long as the registry will not undergo any modification.

> NOTE: 
>
> 一、翻译如下:
>
> "线性化意味着修改立即发生，一旦写入注册表值，只要注册表不进行任何修改，任何后续的读操作都会找到相同的值。"

#### What happens if you don't have Linearizability?

[![enter image description here](https://i.stack.imgur.com/WYcNo.png)](https://i.stack.imgur.com/WYcNo.png)

This time, we don’t have a single **registry** or a single source of truth. Our system uses asynchronous database replication, and we have a Primary node that takes both reads and writes and a Follower node used for read operations only.

Because replication happens asynchronously, there’s a **lag**(滞后) between the Primary node row modification and the time when the Follower applies the same change.

One database connection changes the account balance from 50 to 10 and commits the transaction. Right after, a second transaction reads from the Follower node, but since replication did not apply the balance modification, the value of 50 is read.

Therefore, this system is not linearizable since changes don’t appear to happen instantaneously. In order to make this system linearizable, we need to use **synchronous replication**, and the Primary node UPDATE operation will not complete until the Follower node also applies the same modification.

### [A](https://stackoverflow.com/a/48183351/10173843)



## stackexchange [What is a linearization point?](https://cs.stackexchange.com/questions/7132/what-is-a-linearization-point)

With respect concurrent programming, what is a **linearization point**?

They seem to occur at a **compare-and-swap** instruction apparently. The best definition I could find is [here](http://en.wikipedia.org/wiki/Atomicity_(programming)#Linearization_points).

> All function calls have a **linearization point** at some instant between their invocation and their response.

Okay that's fine, they occur somewhere within a function call, but what are they?

> All functions appear to occur instantly at their **linearization point**, behaving as specified by the sequential definition.

Occur instantly at their LP's??? I don't understand this.

> NOTE: 
>
> "LP"是**linearization point**。

I also read through [this](http://www.eecs.qmul.ac.uk/~ohearn/Workshops/Concurrency09/slides/vafeiadis.pdf) which attempts to prove LP's. I am having trouble finding any solid definitions. Could anyone help?

### [A](https://cs.stackexchange.com/a/7133)

Sounds like you are reading The Art of Multiprocessor Programming.

> "All function calls have a linearization point at some instant between their invocation and their response"
>
> > Okay that's fine, they occur somewhere within a function call, but what are they?

Side effects of the functions. http://en.wikipedia.org/wiki/Side_effect_(computer_science):

> A function or expression is said to have a side effect if, in addition to returning a value, it also modifies some state or has an observable interaction with calling functions or the outside world.

In C/C++ side effects are basically writing to memory that is not on the function stack.

> "All functions appear to occur instantly at their linearization point, behaving as specified by the sequential definition"
>
> > Occur instantly at their LP's??? I don't understand this.

It means that memory writes become visible to other CPUs in the system at that point. Imagine a function of 100 instructions. Instruction 90 is CAS which is a **linearization point**. Once the instruction has completed the effects of preceding memory writes become visible to other CPUs instantly ([release semantics](http://www.niallryan.com/node/138)). This happens some time after the function has been called but before it returned (on instruction 90 of 100).

Note, that memory writes may not always be visible to other CPUs in the system if **linearization points** are not employed.

> NOTE: 
>
> 其实就是同步点

## wikipedia [Linearizability](https://en.wikipedia.org/wiki/Linearizability)

In [concurrent programming](https://en.wikipedia.org/wiki/Concurrent_programming), an operation (or set of operations) is **linearizable** if it consists of an ordered list of invocation and **response events** ([callbacks](https://en.wikipedia.org/wiki/Callbacks)), that may be extended by adding **response events** such that:

1、The extended list can be re-expressed as a **sequential history** (is [serializable](https://en.wikipedia.org/wiki/Serializability)), and

2、That **sequential history** is a subset of the original unextended list.

Informally, this means that the unmodified list of events is linearizable [if and only if](https://en.wikipedia.org/wiki/If_and_only_if) its invocations were [serializable](https://en.wikipedia.org/wiki/Serializability), but some of the responses of the serial schedule have yet to return.[[1\]](https://en.wikipedia.org/wiki/Linearizability#cite_note-:0-1)

> NOTE: 没有完全理解；直观理解是: response event list 是由invocation list决定，并且它们的order是匹配的。

In a concurrent system, processes can access a **shared object** at the same time. Because multiple processes are accessing a single object, there may arise a situation in which while one process is accessing the object, another process changes its contents. This example demonstrates the need for linearizability. In a linearizable system although operations overlap on a **shared object**, each operation appears to take place instantaneously(瞬间，其实就是atomic). 

> NOTE: 上面这段话总结得非常好的。
>
> 使用multiple model来进行理解: 
>
> 1、一旦有一个进行了write，则必须进行 concurrency control。
>
> 2、上述shared object，其实就是shared data

Linearizability is a **strong correctness condition**, which constrains what outputs are possible when an object is accessed by multiple processes concurrently. It is a **safety property** which ensures that operations do not complete in an unexpected or unpredictable manner. If a system is linearizable it allows a programmer to reason about the system.[[2\]](https://en.wikipedia.org/wiki/Linearizability#cite_note-2)

> NOTE: Linearizability 其实就是 make it  computational 中的 **ordering**，它能够保证 **correct/ideal/expected result**，它是programmer理解system的方式。



> ***TRANSLATION*** : 在并发系统中，进程可以同时访问共享对象。由于多个进程正在访问单个对象，因此可能会出现这样的情况：当一个进程正在访问该对象时，另一个进程会更改其内容。此示例演示了线性化的必要性。在可线性化系统中，尽管操作在共享对象上重叠，但每个操作似乎都是即时进行的。线性化是一种强正确性条件，它限制了当多个进程同时访问对象时可能的输出。它是一种安全属性，可确保操作不会以意外或不可预测的方式完成。如果系统是可线性化的，则允许程序员推理系统。





![img](https://upload.wikimedia.org/wikipedia/commons/thumb/c/c3/Linearlizable_Process.svg/220px-Linearlizable_Process.svg.png)





> In grey a linear sub-history, processes beginning in b do not have a linearizable history because b0 or b1 may complete in either order before b2 occurs.





### History of linearizability

> NOTE: 这段话涉及了linearizability和atomic之间的关系；从后文可以看出，两者是不同的概念

Linearizability was first introduced as a [consistency model](https://en.wikipedia.org/wiki/Consistency_model) by [Herlihy](https://en.wikipedia.org/wiki/Maurice_Herlihy) and [Wing](https://en.wikipedia.org/wiki/Jeannette_Wing) in 1987. It encompassed(包含) more restrictive definitions of **atomic**, such as "an atomic operation is one which cannot be (or is not) interrupted by concurrent operations", which are usually vague(模糊的) about when an operation is considered to begin and end.

An atomic object can be understood immediately and completely from its sequential definition, as a set of operations run in parallel which always appear to occur one after the other; no **inconsistencies** may emerge. Specifically, linearizability guarantees that the [invariants](https://en.wikipedia.org/wiki/Invariant_(computer_science)) of a system are *observed* and *preserved* by all operations: if all operations individually preserve an invariant, the system as a whole will.

> NOTE: 上面这段话，简而言之:
>
> atomic -> linearizability
>
> linearizability !-> atomic 
>
> 

### Definition of linearizability

> NOTE: 未读

### Primitive atomic instructions

> NOTE:这部分内容 放到了工程hardware的atomic-instruction章节中

Processors have [instructions](http://en.wiki.sxisa.org/wiki/Instruction_(computer_science)) that can be used to implement [locking](http://en.wiki.sxisa.org/wiki/Lock_(computer_science)) and [lock-free and wait-free algorithms](http://en.wiki.sxisa.org/wiki/Lock-free_and_wait-free_algorithms). 

### High-level atomic operations

> NOTE: 前面的"Primitive atomic instructions"，其实是非常底层的，学习难度是较大的，因此，需要创建合理的abstraction，或者提供high-level interface。

#### Lock based

> NOTE: 其实就是基于lock来实现

The easiest way to achieve linearizability is running groups of primitive operations in a [critical section](http://en.wiki.sxisa.org/wiki/Critical_section). Strictly, independent operations can then be carefully permitted to overlap their critical sections, provided this does not violate linearizability. Such an approach must balance the cost of large numbers of [locks](http://en.wiki.sxisa.org/wiki/Lock_(computer_science)) against the benefits of increased parallelism.

#### Native atomic primitives based

> NOTE: 其实就是基于Native atomic primitives 来实现

Another approach, favoured by researchers (but not yet widely used in the software industry), is to design a linearizable object using the native atomic primitives provided by the hardware. This has the potential to maximise available parallelism and minimise synchronisation costs, but requires mathematical proofs which show that the objects behave correctly.

#### Transactional memory based

> NOTE: 基于transactional memory的实现

A promising hybrid of these two is to provide a [transactional memory](http://en.wiki.sxisa.org/wiki/Transactional_memory) abstraction. As with critical sections, the user marks sequential code that must be run in isolation from other threads. The implementation then ensures the code executes atomically. This style of abstraction is common when interacting with databases; for instance, when using the [Spring Framework](http://en.wiki.sxisa.org/wiki/Spring_Framework), annotating a method with `@Transactional` will ensure all enclosed database interactions occur in a single [database transaction](http://en.wiki.sxisa.org/wiki/Database_transaction). Transactional memory goes a step further, ensuring that all memory interactions occur atomically. As with database transactions, issues arise regarding composition of transactions, especially database and in-memory transactions.

#### All-or-nothing interface

> NOTE: 其实，我觉得这是更加宽泛的transaction概念

A common theme when designing linearizable objects is to provide an **all-or-nothing interface**: either an operation succeeds completely, or it fails and does nothing. ([ACID](http://en.wiki.sxisa.org/wiki/ACID) databases refer to this principle as [atomicity](http://en.wiki.sxisa.org/wiki/Atomicity_(database_systems)).) If the operation fails (usually due to concurrent operations), the user must retry, usually performing a different operation. For example:

1、[Compare-and-swap](http://en.wiki.sxisa.org/wiki/Compare-and-swap) writes a new value into a location only if the latter's contents matches a supplied old value. This is commonly used in a read-modify-CAS sequence: the user reads the location, computes a new value to write, and writes it with a CAS (compare-and-swap); if the value changes concurrently, the CAS will fail and the user tries again.

2、[Load-link/store-conditional](http://en.wiki.sxisa.org/wiki/Load-link/store-conditional) encodes this pattern more directly: the user reads the location with load-link, computes a new value to write, and writes it with store-conditional; if the value has changed concurrently, the SC (store-conditional) will fail and the user tries again.

3、In a [database transaction](http://en.wiki.sxisa.org/wiki/Database_transaction), if the transaction cannot be completed due to a concurrent operation (e.g. in a [deadlock](http://en.wiki.sxisa.org/wiki/Deadlock)), the transaction will be aborted and the user must try again.



### Examples of linearizability

> NOTE: 这段介绍了使用

> 

## TODO

在下面文章中提及了atomicity内容:

1) [Using Redis as a Time Series Database: Why and How](https://www.infoq.com/articles/redis-time-series/)



## draft



### Serialization

> NOTE: 本节标题的含义是"串行"，串行的英语是serial，显然，串行和并行是相反的。

参见文章：维基百科[Linearizability](https://en.wikipedia.org/wiki/Linearizability)。

实现Serialization的方式有：

- atomic操作
- 加锁机制

[What is the difference between linearizability and serializability?](https://stackoverflow.com/questions/4179587/what-is-the-difference-between-linearizability-and-serializability)

维基百科[Mutual exclusion](https://en.wikipedia.org/wiki/Mutual_exclusion)

