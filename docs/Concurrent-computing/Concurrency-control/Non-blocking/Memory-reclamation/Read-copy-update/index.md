# Read-copy-update

简称: RCU。

一、multiple reader thread、single writer thread

> NOTE: 关于这一点，参见:
>
> 1、lwn.net [What is RCU, Fundamentally?](https://lwn.net/Articles/262464/)
>
> 2、geeksforgeeks [Read-Copy Update (RCU)](https://www.geeksforgeeks.org/read-copy-update-rcu/)

二、由于它只有single writer，因此它相对于MCVV就简单地很多:

1、不需要考虑MVCC中，multiple version copy的问题

2、可以认为，只有一份copy

三、memory reclamation

RCU中，需要考虑memory reclamation的问题

## wikipedia [Read-copy-update](https://en.wikipedia.org/wiki/Read-copy-update)

In [computer science](https://en.wikipedia.org/wiki/Computer_science), **read-copy-update** (**RCU**) is a [synchronization](https://en.wikipedia.org/wiki/Synchronization_(computer_science)) mechanism that avoids the use of [lock](https://en.wikipedia.org/wiki/Lock_(computer_science)) primitives while multiple [threads](https://en.wikipedia.org/wiki/Thread_(computing)) concurrently read and update elements that are linked through [pointers](https://en.wikipedia.org/wiki/Pointer_(computer_programming)) and that belong to shared [data structures](https://en.wikipedia.org/wiki/Data_structure) (e.g., [linked lists](https://en.wikipedia.org/wiki/Linked_lists), [trees](https://en.wikipedia.org/wiki/Tree_(data_structure)), [hash tables](https://en.wikipedia.org/wiki/Hash_table)).[[1\]](https://en.wikipedia.org/wiki/Read-copy-update#cite_note-:0-1)

> NOTE: 
>
> 可以使用multiple-model来进行理解: 
>
> multiple: threads
>
> shared data: linked structure
>
> read:  read
>
> update: write
>
> 
>
> 

Whenever a thread is inserting or deleting elements of data structures in [shared memory](https://en.wikipedia.org/wiki/Shared_memory), all readers are guaranteed to see and traverse either the older or the new structure, therefore avoiding inconsistencies (e.g., dereferencing *null pointers).*[[1\]](https://en.wikipedia.org/wiki/Read-copy-update#cite_note-:0-1)



It is used when performance of reads is crucial and is an example of [space–time tradeoff](https://en.wikipedia.org/wiki/Space–time_tradeoff), enabling fast operations at the cost of more space. This makes all readers proceed as if there were no [synchronization](https://en.wikipedia.org/wiki/Synchronization_(computer_science)) involved, hence they will be fast, but also making updates more difficult.

> NOTE:  [space–time tradeoff](https://en.wikipedia.org/wiki/Space–time_tradeoff): 以空间换时间

### Name and overview

The name comes from the way that RCU is used to update a linked structure **in place**. A thread wishing to do this uses the following steps:

1、create a new structure,

> NOTE: 
>
> 此处的"new structure"，如果以linked list为例的话，则它表示的是linked list的node

2、copy the data from the old structure into the new one, and save a [pointer](https://en.wikipedia.org/wiki/Pointer_(computer_programming)) to the old structure,

> NOTE: 
>
> 在new node中，有一个pointer to the old structure

3、modify the new, copied, structure

> NOTE: 
>
> update是发生于new node

4、update the global pointer to refer to the new structure, and then

> NOTE: 
>
> 这是指更新linked list

5、sleep until the operating system kernel determines that there are no readers left using the old structure, for example, in the Linux kernel, by using `synchronize_rcu()`.

> NOTE: 
>
> 可以使用reference counting

When the thread that made the copy is awakened by the kernel, it can safely deallocate the old structure.

So the structure is *read* concurrently with a thread *copying* in order to do an *update*, hence the name "read-copy update". The abbreviation "RCU" was one of many contributions by the Linux community. Other names for similar techniques include *passive serialization* and *MP defer* by [VM/XA](https://en.wikipedia.org/wiki/VM_(Operating_system)) programmers and *generations* by [K42](https://en.wikipedia.org/wiki/K42) and [Tornado](https://web.archive.org/web/20140102191807/http://www.eecg.toronto.edu/~tornado/) programmers.

> NOTE: 
>
> RCU在执行update的时候，并不阻塞read

### Uses

By early 2008, there were almost 2,000 uses of the RCU API within the Linux kernel[[5\]](https://en.wikipedia.org/wiki/Read-copy-update#cite_note-7) including the networking protocol stacks[[6\]](https://en.wikipedia.org/wiki/Read-copy-update#cite_note-8) and the memory-management system.[[7\]](https://en.wikipedia.org/wiki/Read-copy-update#cite_note-9) As of March 2014, there were more than 9,000 uses.[[8\]](https://en.wikipedia.org/wiki/Read-copy-update#cite_note-10) Since 2006, researchers have applied RCU and similar techniques to a number of problems, including management of metadata used in dynamic analysis,[[9\]](https://en.wikipedia.org/wiki/Read-copy-update#cite_note-11) managing the lifetime of clustered objects,[[10\]](https://en.wikipedia.org/wiki/Read-copy-update#cite_note-12) managing object lifetime in the [K42](https://en.wikipedia.org/wiki/K42) research operating system,[[11\]](https://en.wikipedia.org/wiki/Read-copy-update#cite_note-13)[[12\]](https://en.wikipedia.org/wiki/Read-copy-update#cite_note-14) and optimizing [software transactional memory](https://en.wikipedia.org/wiki/Software_transactional_memory) implementations.[[13\]](https://en.wikipedia.org/wiki/Read-copy-update#cite_note-15)[[14\]](https://en.wikipedia.org/wiki/Read-copy-update#cite_note-16) [Dragonfly BSD](https://en.wikipedia.org/wiki/Dragonfly_BSD) uses a technique similar to RCU that most closely resembles Linux's Sleepable RCU (SRCU) implementation.

> NOTE: 
>
> 在下面的TODO章节中，给出的链接，都是Linux OS kernel的。

### Advantages and disadvantages

> NOTE: 这段总结非常好

The ability to wait until all readers are done allows RCU readers to use much **lighter-weight synchronization**—in some cases, absolutely no synchronization at all. In contrast, in more conventional lock-based schemes, readers must use heavy-weight synchronization in order to prevent an updater from deleting the data structure out from under them. The reason is that lock-based updaters typically update data in place, and must therefore exclude readers. In contrast, RCU-based updaters typically take advantage of the fact that **writes to single aligned pointers are atomic on modern CPUs, allowing atomic insertion, removal, and replacement of data in a linked structure without disrupting readers**. Concurrent RCU readers can then continue accessing the old versions, and can dispense with(豁免，免除) the **atomic read-modify-write instructions**, memory barriers, and cache misses that are so expensive on modern [SMP](https://en.wikipedia.org/wiki/Symmetric_multiprocessing) computer systems, even in absence of lock contention.[[15\]](https://en.wikipedia.org/wiki/Read-copy-update#cite_note-17)[[16\]](https://en.wikipedia.org/wiki/Read-copy-update#cite_note-18) The lightweight nature of RCU's read-side primitives provides additional advantages beyond excellent performance, scalability, and real-time response. For example, they provide immunity(免疫、豁免) to most deadlock and livelock conditions.[[note 3\]](https://en.wikipedia.org/wiki/Read-copy-update#cite_note-19)

> NOTE: 
>
> 上述 **writes to single aligned pointers are atomic on modern CPUs, allowing atomic insertion, removal, and replacement of data in a linked structure without disrupting readers** 是非常重要的，需要注意的是，其中 pointer前面是有一个定语: "aligned"，关于此的解释参见工程hardware的`CPU\Memory-access\Memory-alignment`章节。
>
> 关于  **atomic read-modify-write instructions**，参见 `Concurrent-computing\Concurrency-control\Non-blocking\Atomic-instruction\Read–modify–write` 章节。

Of course, RCU also has disadvantages. For example, RCU is a specialized technique that works best in situations with mostly reads and few updates, but is often less applicable to update-only workloads. For another example, although the fact that RCU readers and updaters may execute concurrently is what enables the lightweight nature of RCU's read-side primitives, some algorithms may not be amenable to read/update concurrency.

> NOTE: 需要从read-write的角度对concurrency control technique的选择继续总结。

Despite well over a decade of experience with RCU, the exact extent of its applicability is still a research topic.



## lwn.net [What is RCU, Fundamentally?](https://lwn.net/Articles/262464/)

Read-copy update (RCU) is a synchronization mechanism that was added to the Linux kernel in October of 2002. RCU achieves scalability improvements by allowing reads to occur concurrently with updates. In contrast with conventional locking primitives that ensure **mutual exclusion** among concurrent threads regardless of whether they be readers or updaters, or with **reader-writer locks** that allow concurrent reads but not in the presence of updates, RCU supports concurrency between a **single updater** and **multiple readers**. 

RCU ensures that reads are coherent(连贯) by maintaining multiple versions of objects and ensuring that they are not freed up until all pre-existing read-side critical sections complete. 

> NOTE: 此处的"coherent"的用法，让我连接了这个此的含义

RCU defines and uses efficient and scalable mechanisms for publishing and reading new versions of an object, and also for deferring the collection of old versions. 

> NOTE: memory reclamation，上述"deferring"，让我想到了在"cs.toronto-Comparative-Performance-of-Memory-Reclamation-Strategies"中，也使用了这个词语。

These mechanisms distribute the work among read and update paths in such a way as to make read paths extremely fast. In some cases (non-preemptable kernels), RCU's read-side primitives have zero overhead.



## RCU VS MVCC

一、zhihu [Linux中的RCU机制[一] - 原理与使用方法](https://zhuanlan.zhihu.com/p/89439043)

> RCU是有唯一意思的，尤其是Linux内核的角度下；而Fedor Pikus说的是，理解“RCU”不能简单地从名字出发。确实，版本号也可以达到类似的“读写并行进行”的效果；但RCU和版本号的区别在于，RCU有特别的内存回收方式，也就是文中描述的——**写者线程**等待**读者线程**退出后者的critical section，再进行**内存回收**，这是RCU最关键的地方。因为它最早是用在Linux内核，而内核知道读者线程的进度而几乎不需要额外的簿记信息，因此能够获得很好的性能。

二、RCU与MVCC的类似之处:

1、都是  [space–time tradeoff](https://en.wikipedia.org/wiki/Space–time_tradeoff)

2、参见 "Advantages and disadvantages"，其中点到了 version

3、在原文的 "See also" 章节中，给出了 [Multiversion concurrency control](http://en.wiki.sxisa.org/wiki/Multiversion_concurrency_control) 链接



## Use case



一、wikipedia [Atomicity (database systems)](https://en.wikipedia.org/wiki/Atomicity_(database_systems))

Typically, systems implement Atomicity by providing some mechanism to indicate which transactions have started and which finished; or by keeping a copy of the data before any changes occurred ([read-copy-update](https://en.wikipedia.org/wiki/Read-copy-update)).



二、kernel [RCU concepts](https://www.kernel.org/doc/html/latest/RCU/index.html) # Using RCU to Protect Read-Mostly Arrays

Although RCU is more commonly used to protect linked lists, it can also be used to protect arrays. Three situations are as follows:

1. [Hash Tables](https://www.kernel.org/doc/html/latest/RCU/arrayRCU.html#hash-tables)
2. [Static Arrays](https://www.kernel.org/doc/html/latest/RCU/arrayRCU.html#static-arrays)
3. [Resizable Arrays](https://www.kernel.org/doc/html/latest/RCU/arrayRCU.html#resizable-arrays)

Each of these three situations involves an RCU-protected pointer to an array that is separately indexed. 



## TODO

3、mit [Read-Copy Update in a Garbage Collected Environment](https://math.mit.edu/research/highschool/primes/materials/2016/conf/10-1%20Sheth-Welling-Sheth.pdf)

4、geeksforgeeks [Read-Copy Update (RCU)](https://www.geeksforgeeks.org/read-copy-update-rcu/)

> NOTE: 其中给出了实现样例代码