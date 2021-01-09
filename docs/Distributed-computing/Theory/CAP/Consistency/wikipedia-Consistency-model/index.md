# wikipedia [Consistency model](https://en.wikipedia.org/wiki/Consistency_model)




In [computer science](https://en.wikipedia.org/wiki/Computer_science), **consistency models** are used in [distributed systems](https://en.wikipedia.org/wiki/Distributed_computing) like [distributed shared memory](https://en.wikipedia.org/wiki/Distributed_shared_memory) systems or distributed data stores (such as a [filesystems](https://en.wikipedia.org/wiki/Filesystem), [databases](https://en.wikipedia.org/wiki/Database), [optimistic replication](https://en.wikipedia.org/wiki/Optimistic_replication) systems or [web caching](https://en.wikipedia.org/wiki/Web_caching)). The system is said to support a given model if operations on **memory** follow specific **rules**. The **data consistency model** specifies a **contract** between programmer and system, wherein the system guarantees that if the programmer follows the rules, memory will be [consistent](https://en.wikipedia.org/wiki/Consistent) and the results of reading, writing, or updating memory will be **predictable**（说白了其实就是数据不会丢失，不会出现错误）. This is different from **coherence**, which occurs in systems that are [cached](https://en.wikipedia.org/wiki/Cache_coherence) or cache-less, and is consistency of data with respect to all processors. Coherence deals with maintaining a global order in which writes to a single location or single variable are seen by all processors. **Consistency** deals with the **ordering of operations** to multiple locations with respect to all processors.

> NOTE:  Coherence处理维护全局顺序，其中所有处理器都可以看到对单个位置或单个变量的写入。 一致性处理与所有处理器相关的多个位置的操作顺序。

[High level languages](https://en.wikipedia.org/wiki/High_level_language), such as [C++](https://en.wikipedia.org/wiki/C%2B%2B) and [Java](https://en.wikipedia.org/wiki/Java_(programming_language)), partially maintain the contract by translating memory operations into low-level operations in a way that preserves [memory semantics](https://en.wikipedia.org/wiki/Memory_semantics_(computing)). To hold to the contract, compilers may reorder some memory instructions, and library calls such as `pthread_mutex_lock()` encapsulate required synchronization.[[1\]](https://en.wikipedia.org/wiki/Consistency_model#cite_note-1)

Verifying [sequential consistency](https://en.wikipedia.org/wiki/Sequential_consistency) through [model checking](https://en.wikipedia.org/wiki/Model_checking) is [undecidable](https://en.wikipedia.org/wiki/Undecidable_problem) in general, even for finite-state [cache coherence](https://en.wikipedia.org/wiki/Cache_coherence) protocols.[[2\]](https://en.wikipedia.org/wiki/Consistency_model#cite_note-2)

Consistency models define rules for the apparent order and visibility of updates, and are on a [continuum](https://en.wikipedia.org/wiki/Linear_continuum) with tradeoffs.[[3\]](https://en.wikipedia.org/wiki/Consistency_model#cite_note-Rules-3)



## Example



Assume that the following case occurs:[[3\]](https://en.wikipedia.org/wiki/Consistency_model#cite_note-Rules-3)

- The row X is replicated on nodes M and N
- The client A writes row X to node M
- After a period of time t, client B reads row X from node N

The consistency model has to determine whether client B sees the write from client A or not.



## Types

There are two methods to define and categorize **consistency models**; issue and view.

- Issue

  Issue method describes the restrictions(限制) that define how a process can issue operations.

  ***SUMMARY*** : 定义进程如何发出操作的限制

- View

  View method which defines the order of operations visible to processes.

  ***SUMMARY*** : 定义进程可见的操作顺序

For example, a consistency model can define that a process is not allowed to issue an operation until all previously issued operations are completed. Different consistency models enforce different **conditions**. One consistency model can be considered **stronger** than another if it requires all conditions of that model and more. In other words, a model with fewer constraints is considered a weaker consistency model.

These models define how the hardware needs to be laid out and at high-level, how the programmer must code. The chosen model also affects how the compiler can re-order instructions. Generally, if control dependencies between instructions and if writes to same location are **ordered**, then the compiler can reorder as required. However, with the models described below, some may allow writes before loads to be reordered while some may not.

### Strict consistency

Strict consistency is the strongest consistency model. Under this model, a write to a variable by any processor needs to be seen instantaneously by all processors.

The strict model diagram and non-strict model diagrams describe the **time constraint** – instantaneous. It can be better understood as though a **global clock** is present in which every write should be reflected in all **processor caches** by the end of that **clock period**. The next operation must happen only in the next clock period.

***SUMMARY*** : 严格的模型图和非严格的模型图描述了时间约束 - 瞬时。 可以更好地理解，好像存在全局时钟，其中每个写入应该在该时钟周期结束时反映在所有处理器高速缓存中。 下一个操作必须仅在下一个时钟周期内发生。

This is the most rigid model and is impossible to implement (with current technology). In this model, the programmer’s expected result will be received every time. It is deterministic. A distributed system with many nodes will take some time to copy information written to one node to all the other nodes responsible for replicating that information. That time can't be zero (without quantum entanglement and quantum computing 量子纠缠和量子计算) because it takes time for **information** to propagate through **space**, and there is a limit to how fast information can travel through space: the [speed of light](https://en.wikipedia.org/wiki/Speed_of_light). Therefore, strict consistency is impossible (with current production level technology). The best one can do is design a system where the time-to-replicate approaches the theoretical minimum.



### Sequential consistency

The [sequential consistency](https://en.wikipedia.org/wiki/Sequential_consistency) model was proposed by Lamport(1979). It is a weaker memory model than strict consistency model. A write to a variable does not have to be seen instantaneously, however, writes to variables by different processors have to be seen in the **same order** by all processors. As defined by Lamport(1979),[[4\]](https://en.wikipedia.org/wiki/Consistency_model#cite_note-Lamport-4)sequential consistency is met if "the result of any execution is the same as if the operations of all the processors were executed in some **sequential order**, and the operations of each individual processor appear in this sequence in the order specified by its program."

***SUMMARY*** : 序列一致性模型由Lamport（1979）提出。 它是一种比严格一致性模型更弱的内存模型。 不必立即看到对变量的写入，但是，所有处理器必须以相同的顺序看到不同处理器对变量的写入。 正如Lamport（1979）所定义的那样，如果“任何执行的结果与所有处理器的操作以某种顺序执行相同，并且每个处理器的操作出现在此中，则满足顺序一致性[4]。 序列按其程序指定的顺序排列。“

Program order within each processor and sequential ordering of operations between processors should be maintained. In order to preserve **sequential order of execution** between processors, all operations must appear to execute instantaneously or atomically with respect to every other processor( 为了保持处理器之间的连续执行顺序，所有操作必须看起来相对于每个其他处理器即时或原子地执行). These operations need only "appear" to be completed because it is physically impossible to send information instantaneously. For instance, once a bus line is posted with information, it is guaranteed that all processors will see the information at the same instant. Thus, passing the information to the bus line completes the execution with respect to all processors and has appeared to have been executed. Cache-less architectures or cached architectures with interconnect networks that are not instantaneous can contain a slow path between processors and memories. These slow paths can result in sequential inconsistency, because some memories receive the broadcast data faster than others.

应保持每个处理器内的程序顺序和处理器之间的操作顺序排序。 为了保持处理器之间的连续执行顺序，所有操作必须看起来相对于每个其他处理器即时或原子地执行。 这些操作只需要“显示”即可完成，因为物理上不可能立即发送信息。 例如，一旦发布了包含信息的总线，就可以保证所有处理器都能在同一时刻看到这些信息。 因此，将信息传递到总线完成了对所有处理器的执行，并且似乎已经执行。 具有非瞬时互连网络的无缓存体系结构或缓存体系结构可能包含处理器和存储器之间的慢速路径。 这些慢速路径可能导致顺序不一致，因为某些存储器比其他存储器更快地接收广播数据。

**Sequential consistency** can produce non-deterministic results. This is because the sequence of sequential operations between processors can be different during different runs of the program. All memory operations need to happen in the program order.

[**Linearizability**](https://en.wikipedia.org/wiki/Linearizability) (also known as **atomic consistency**) can be defined as **sequential consistency** with the real-time constraint.

### Causal consistency

[Causal consistency](https://en.wikipedia.org/wiki/Causal_consistency) is a weakening model of sequential consistency by categorizing events into those causally related and those that are not. It defines that only write operations that are causally related, need to be seen in the same order by all processes.

This model relaxes sequential consistency on concurrent writes by a processor and on writes that are not causally related. Two writes can become causally related if one write to a variable is dependent on a previous write to any variable if the processor doing the second write has just read the first write. The two writes could have been done by the same processor or by different processors.

As in sequential consistency, reads do not need to reflect changes instantaneously, however, they need to reflect all changes to a variable sequentially.

| Sequence |    P1    |    P2    |
| :------: | :------: | :------: |
|    1     | W1(*x*)3 |          |
|    2     |          |  W2(x)5  |
|    3     |          | R1(*x*)3 |

W1 is not causally related to W2. R1 would be *sequentially inconsistent* but is *causally consistent*.[*clarification needed*][[5\]](https://en.wikipedia.org/wiki/Consistency_model#cite_note-:0-5)

| Sequence |  P1   |  P2   |  P3   |  P4   |
| :------: | :---: | :---: | :---: | :---: |
|    1     | W(x)1 | R(x)1 | R(x)1 | R(x)1 |
|    2     |       | W(x)3 |       |       |
|    3     | W(x)2 |       | R(x)3 | R(x)2 |
|    4     |       |       | R(x)2 | R(x)3 |

W(x)1 and W(x)2 are causally related due to the read made by P2 to x before W(x)2.[[5\]](https://en.wikipedia.org/wiki/Consistency_model#cite_note-:0-5)



## [Consistency and replication](https://en.wikipedia.org/wiki/Consistency_model#Consistency_and_replication)

> NOTE:这就是寻常所说的 "主备的一致性"，这是很多的data system采用的方法，redis就是采用的这种方式。

