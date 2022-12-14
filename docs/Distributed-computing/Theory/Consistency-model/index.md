# Consistency model

本章讨论consistency model。

## 如何掌握consistency model?



### Consistency model for multiple system

"consistency"的含义是"一致"，multiple的各个entity之间保持consistency，在computer science中，主要是data 保持consistency，对data/memory的operation包括

1、read

2、write

Consistency model能够对multiple model中的shared data进行描述，multiple model能够描述非常多的。

下面是一些例子:

一、zhuanlan.zhihu [高并发编程--多处理器编程中的一致性问题(上)](https://zhuanlan.zhihu.com/p/48157076)

在分布式存储系统中，为了提高**可靠性**，通常会引入多个副本，多个副本需要向用户表现出一致的内容。这很自然的让人想到如何确保多副本之间的一致性。为此也有了paxos和raft等保证多副本之间一致性的协议。当我们在一个多处理器机器上编程时我们通常会忽略在多处理器环境下的一致性问题，因为系统已经为我们做好了基本的**一致性**保证，当存在一致性问题的时候上层编程语言也提供了具备一致性语意的接口，只是我们在编程中并没有意识到这些**接口**与**一致性**的关系。无论是分布式存储还是多处理器编程都有一个共同点，就是会涉及共享对象的操作。

> NOTE: 
>
> 1、"可靠性"，其实就是HA
>
> "多个副本"，其实就是master-slave
>
> "一致的内容"，其实就是consistency
>
> "共享对象"其实就是multiple model中的shared data
>
> 2、上述其实可以使用multiple model-shared data来进行描述

一旦出现共享，就会出现正确性的问题，那么如何定义在并发中操作共享对象的正确性，这就是一致性协议的任务了。

本文主要针对多处理器系统中的一致性问题进行了一些总结，对于分布式中的一致性问题会在后面文章中总结。

多处理器中的一致性问题源于并发，源于共享。对于共享内存并发操作下的正确性保证是硬件设计者需要提供给上层开发人员最重要的保证。对于上层开发人员来说，系统内部的一致性是透明的，但是去了解系统内部一致性的设计及原理有利于我们更能够面向机器编程，写出正确的代码。



### Multiple model

Multiple model中的entity可以为:

| model                 | entity         | shared data |
| --------------------- | -------------- | ----------- |
| multicore             | processor core |             |
| multithread           | thread         |             |
| multiprocess          | process        |             |
| distributed computing | node           |             |



因此下面的描述的内容，有的时候并不针对上述情况中的一种，而是描述的一种通用的原理。



### 理想 与 现实

**理想**: 各个entity之间的**通信延时**为0，或者说，它们之间不存在延时，这样就不存在**同步延时**，这样任何一个entity对自己的copy of shared data的修改能够立即同步到其他的entity中，各个entity的**copy of shared data**是不存在差异的，即各个entity所看到的**shared data**是相同的。其实这就是**strict consistency**。



**现实**: 各个entity之间存在着**通信延时**(**delay**)，一旦entity对自己的**copy of shared data**做出了write，在完成和其他的entity的**synchronization**之前(**同步延时**)，各个entity的**copy of shared data**是存在着差异的，即各个entity所看到的**shared data**是不同的。

> NOTE: 
>
> 一、replication lag
>
> lag的意思是延迟



## Consistency model abstract machine

为了便于理解各种consistency model，本节提出一个consistency model abstract machine，它其实就是programmer站在全局的视角来看待并运行整个multiple model，从而得到输出，它的简单定义如下:

1、所有的entity都独立地运行在这台abstract machine上

> NOTE: 可以认为每个entity有一个自己的processor

2、shared data

> NOTE: 可以认为是memory

3、每个entity都有自己的copy of shared data

> NOTE: 可以认为每个entity都全量的cache shared data

4、每个entity需要将自己对shared data的更改同步到shared data中

5、**全局时钟**、abstract machine的时钟，其实就是墙上时钟，programmer以这个时钟为参考，来看abstract machine上的各个entity，其实就是从programmer的角度来看各个entity的活动。这位programmer提供了观察整个系统的视角。

### 模拟strict consistency model

**Strict consistency model**是最最理想的情况，同时它也是非常容易理解的。

在strict consistency中，由于:

1、**所有的write都立即生效**(在实际中，可能无法达成)

2、entity的copy of shared data能够一直保持相同

因此**strict consistency model**可以这样模拟: 

所有的**entity**都在同一个processor上执行，使用同一个shared data，这个processor没有cache，所有的write立即生效。





## Consistency model is a contract

它是一种contract，遵循一定的contract，则可以实现预期的效果；

参见:

1、wikipedia [Consistency model](https://en.wikipedia.org/wiki/Consistency_model)

二、sequential model是lamport为multiple core system所提出的



### Level and tradeoff

> NOTE: 有哪些constrain、相较于上一层，放松了哪些constrain。

每种consistency model都有constrain，从而决定了consistency model的strong、weak

1、strict consistency是最最理想的、容易理解情况，可以以它来作为对比

2、在现实应用中，strict consistency的实现是比较困难的，并且有的application不需要strict consistency，因此可以放宽constrain，使用weak consistency model

3、programmer需要进行tradeoff



## consistency vs coherence

参见: 

1、wikipedia [Consistency model](https://en.wikipedia.org/wiki/Consistency_model)



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
> 一、consistency vs coherence
>
> coherence，在工程hardware中进行了描述；
>
> 上述consistency，可以使用consistency model abstract machine来描述；

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

> NOTE: 
>
> 上面描述了strict consistency的constrain

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

