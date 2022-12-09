# Consistency model

本章讨论consistency model。

## Consistency model for multiple system

1、sequential model是lamport为multiple core system所提出的



## wikipedia [Consistency model](https://en.wikipedia.org/wiki/Consistency_model)



In [computer science](https://en.wikipedia.org/wiki/Computer_science), **consistency models** are used in [distributed systems](https://en.wikipedia.org/wiki/Distributed_computing) like [distributed shared memory](https://en.wikipedia.org/wiki/Distributed_shared_memory) systems or distributed data stores (such as a [filesystems](https://en.wikipedia.org/wiki/Filesystem), [databases](https://en.wikipedia.org/wiki/Database), [optimistic replication](https://en.wikipedia.org/wiki/Optimistic_replication) systems or [web caching](https://en.wikipedia.org/wiki/Web_caching)). 

The system is said to support a given model if operations on **memory** follow specific **rules**. The **data consistency model** specifies a **contract** between programmer and system, wherein the system guarantees that if the programmer follows the rules, memory will be [consistent](https://en.wikipedia.org/wiki/Consistent) and the results of reading, writing, or updating memory will be **predictable**. 

> NOTE: 
>
> 一、上述**predictable**，让我想起了"make it computational"。
>
> **predictable**说白了其实就是数据不会丢失，不会出现错误。

This is different from **coherence**, which occurs in systems that are [cached](https://en.wikipedia.org/wiki/Cache_coherence) or cache-less, and is consistency of data with respect to all processors. 

Coherence deals with maintaining a **global order** in which writes to a single location or single variable are seen by all processors. **Consistency** deals with the **ordering of operations** to multiple locations with respect to all processors.

> NOTE:  
>
> 一、coherence，在工程hardware中进行了描述；
>
> 二、上述consistency，可以使用consistency model abstract machine来描述；

[High level languages](https://en.wikipedia.org/wiki/High_level_language), such as [C++](https://en.wikipedia.org/wiki/C%2B%2B) and [Java](https://en.wikipedia.org/wiki/Java_(programming_language)), partially maintain the contract by translating memory operations into low-level operations in a way that preserves [memory semantics](https://en.wikipedia.org/wiki/Memory_semantics_(computing)). To hold to the contract, compilers may reorder some memory instructions, and library calls such as `pthread_mutex_lock()` encapsulate required synchronization.[[1\]](https://en.wikipedia.org/wiki/Consistency_model#cite_note-1)

> NOTE: 
>
> 一、programming language的memory model。

Verifying [sequential consistency](https://en.wikipedia.org/wiki/Sequential_consistency) through [model checking](https://en.wikipedia.org/wiki/Model_checking) is [undecidable](https://en.wikipedia.org/wiki/Undecidable_problem) in general, even for finite-state [cache coherence](https://en.wikipedia.org/wiki/Cache_coherence) protocols.[[2\]](https://en.wikipedia.org/wiki/Consistency_model#cite_note-2)

Consistency models define rules for the apparent order and visibility of updates, and are on a [continuum](https://en.wikipedia.org/wiki/Linear_continuum) with tradeoffs.[[3\]](https://en.wikipedia.org/wiki/Consistency_model#cite_note-Rules-3)



### Example

> NOTE: 原文这一段给出的例子是容易理解的

### Types

There are two methods to define and categorize **consistency models**; issue and view.

1、Issue

Issue method describes the restrictions(限制) that define how a process can issue operations.

> NOTE: 
>
> 一、翻译如下: 
>
> 定义进程如何发出操作的限制

2、View

View method which defines the order of operations visible to processes.

> NOTE: 
>
> 一、翻译如下:
>
> 定义进程可见的操作顺序，这是一个非常重要的概念，如何来理解"the order of operations visible to processes"？
>
> 

For example, a consistency model can define that a process is not allowed to issue an operation until all previously issued operations are completed. Different consistency models enforce different **conditions**. One consistency model can be considered **stronger** than another if it requires all conditions of that model and more. In other words, a model with fewer constraints is considered a weaker consistency model.

> NOTE: 
>
> 一、level and tradeoff

These models define how the hardware needs to be laid out and at high-level, how the programmer must code. The chosen model also affects how the compiler can re-order instructions. Generally, if **control dependencies** between instructions and if writes to same location are **ordered**, then the compiler can reorder as required. However, with the models described below, some may allow writes before loads to be reordered while some may not.

#### Strict consistency

Strict consistency is the strongest consistency model. Under this model, a write to a variable by any processor needs to be seen **instantaneously** by all processors.

> NOTE: 上面描述了strict consistency的constrain

The strict model diagram and non-strict model diagrams describe the **time constraint** – **instantaneous**. It can be better understood as though a **global clock** is present in which every write should be reflected in all **processor caches** by the end of that **clock period**. The next operation must happen only in the next clock period.



This is the most rigid model and is impossible to implement (with current technology). 

> NOTE: 需要注意: 当前的技术水平，strict consistency是无法实现的，为什么? 在下面这段话中回答了这个问题: the [speed of light](https://en.wikipedia.org/wiki/Speed_of_light). 
>
> 那人类的突破它的目标在哪呢？在下面这段话中回答了这个问题: 
>
> **quantum entanglement and quantum computing** 

In this model, the programmer’s expected result will be received every time. It is deterministic. A distributed system with many nodes will take some time to copy information written to one node to all the other nodes responsible for replicating that information. That time can't be zero (without **quantum entanglement and quantum computing** 量子纠缠和量子计算) because it takes time for **information** to propagate through **space**, and there is a limit to how fast information can travel through space: the [speed of light](https://en.wikipedia.org/wiki/Speed_of_light). Therefore, strict consistency is impossible (with current production level technology). The best one can do is design a system where the time-to-replicate approaches the theoretical minimum.



#### Sequential consistency

The [sequential consistency](https://en.wikipedia.org/wiki/Sequential_consistency) model was proposed by Lamport(1979). It is a weaker memory model than strict consistency model. A write to a variable does not have to be seen instantaneously, however, **writes to variables by different processors have to be seen in the same order by all processors**. As defined by Lamport(1979),[[4\]](https://en.wikipedia.org/wiki/Consistency_model#cite_note-Lamport-4)sequential consistency is met if "the result of any execution is the same **as if** the operations of all the processors were executed in some **sequential order**, and the operations of each individual processor appear in this sequence in the order specified by its program."

> NOTE: 
>
> 一、如何来理解Lamport(1979) 关于 sequential order的定义呢？参见 "introduction" 章节；
>
> 上面这段话中的"writes to variables by different processors have to be seen in the same order by all processors"要如何进行理解呢？在 csdn [强一致性、顺序一致性、弱一致性和共识](https://blog.csdn.net/chao2016/article/details/81149674) 中给出了例子，可以结合其中的例子来进行理解。

Program order within each processor and **sequential ordering of operations between processors** should be maintained. In order to preserve **sequential order of execution between processors**, all operations must appear to execute instantaneously or atomically with respect to every other processor( 为了保持处理器之间的连续执行顺序，所有操作必须看起来相对于每个其他处理器即时或原子地执行). These operations need only "appear" to be completed because it is physically impossible to send information instantaneously. For instance, once a bus line is posted with information, it is guaranteed that all processors will see the information at the same instant. Thus, passing the information to the bus line completes the execution with respect to all processors and has appeared to have been executed. Cache-less architectures or cached architectures with interconnect networks that are not instantaneous can contain a slow path between processors and memories. These slow paths can result in sequential inconsistency, because some memories receive the broadcast data faster than others.

**Sequential consistency** can produce non-deterministic results. This is because the sequence of sequential operations between processors can be different during different runs of the program. All memory operations need to happen in the program order.

> NOTE: 上面的这些内容，使用 "能够使用C++11 `std::atomic` 的 sequential consistency来进行模拟" 后，是非常容易理解的。

[**Linearizability**](https://en.wikipedia.org/wiki/Linearizability) (also known as **atomic consistency**) can be defined as **sequential consistency** with the real-time constraint.

#### Causal consistency

> NOTE: 因果一致性

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



### [Consistency and replication](https://en.wikipedia.org/wiki/Consistency_model#Consistency_and_replication)

> NOTE:这就是寻常所说的 "主备的一致性"，这是很多的data system采用的方法，redis就是采用的这种方式。



## TODO

csdn [Zookeeper专题——3、分布式一致性，几种实现的优缺点](https://blog.csdn.net/Happy_wu/article/details/80197356)

jianshu [取代 ZooKeeper！高并发下的分布式一致性开源组件 StateSynchronizer](https://www.jianshu.com/p/2691ef1c2b35)

mwhittaker [Replicated Data Consistency Explained Through Baseball](https://mwhittaker.github.io/consistency_in_distributed_systems/1_baseball.html)

