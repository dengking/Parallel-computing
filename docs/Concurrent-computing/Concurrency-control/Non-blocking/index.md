# 关于本章

本章总结实现non-blocking concurrency control的一些技术。



## wikipedia [Non-lock concurrency control](http://en.wiki.sxisa.org/wiki/Non-lock_concurrency_control)

> NOTE: 原文主要介绍的是DBMS中的non-lock concurrency control，其实这些技术不仅仅局限于DBMS，它们还可以用于其他的system中

1、[Optimistic concurrency control](http://en.wiki.sxisa.org/wiki/Optimistic_concurrency_control)

2、[Timestamp-based concurrency control](http://en.wiki.sxisa.org/wiki/Timestamp-based_concurrency_control)

3、[Multiversion concurrency control](http://en.wiki.sxisa.org/wiki/Multiversion_concurrency_control)



## wikipedia [Non-blocking algorithm](https://infogalactic.com/info/Non-blocking_algorithm)

In [computer science](https://infogalactic.com/info/Computer_science), an [algorithm](https://infogalactic.com/info/Algorithm) is called **non-blocking** if failure or [suspension](https://infogalactic.com/info/Scheduling_(computing)) of any [thread](https://infogalactic.com/info/Thread_(computing)) cannot cause failure or suspension of another thread;[[1\]](https://infogalactic.com/info/Non-blocking_algorithm#cite_note-1) for some operations, these algorithms provide a useful alternative to traditional [blocking implementations](https://infogalactic.com/info/Lock_(computer_science)). 

> NOTE: 表明意思: 如果任何线程的失败或暂停不会导致另一个线程的失败或暂停，则称为**非阻塞算法**。
>
> 没有理解它的含义。

A non-blocking algorithm is 

1) **lock-free** if there is guaranteed system-wide [progress](https://infogalactic.com/info/Resource_starvation), and 

2) **wait-free** if there is also guaranteed per-thread progress.

3、obstruction-free

"Non-blocking" was used as a synonym for "lock-free" in the literature until the introduction of obstruction-freedom in 2003.[[2\]](http://en.wiki.sxisa.org/wiki/Non-blocking_algorithm#cite_note-obs-free-2)

> NOTE: 没有理解它们的含义

#### Motivation

> NOTE: *Main article:* [Disadvantages of locks](https://infogalactic.com/info/Lock_(computer_science)#Disadvantages) 
>
> non-blocking algorithm是为了解决lock的弊端



#### Implementation

> NOTE: 下面描述了几种实现方式
>

With few exceptions, non-blocking algorithms use [atomic](https://infogalactic.com/info/Linearizability) [read-modify-write](https://infogalactic.com/info/Read-modify-write) primitives that the hardware must provide, the most notable of which is [compare and swap (CAS)](https://infogalactic.com/info/Compare-and-swap). [Critical sections](https://infogalactic.com/info/Critical_section) are almost always implemented using standard interfaces over these primitives (in the general case, critical sections will be blocking, even when implemented with these primitives). 

> NOTE: atomic read-modify-write在`./Atomic-operation`章节进行了描述。

In the 1990s all non-blocking algorithms had to be written "natively" with the underlying primitives to achieve acceptable performance. 

> NOTE: 在application level，直接使用这些atomic primitive。

However, the emerging field of [software transactional memory](https://infogalactic.com/info/Software_transactional_memory) promises standard abstractions for writing efficient non-blocking code.[[3\]](https://infogalactic.com/info/Non-blocking_algorithm#cite_note-lightweight-transactions-3)[[4\]](https://infogalactic.com/info/Non-blocking_algorithm#cite_note-composable-memory-transactions-4)

> NOTE: transactional memory

Additionally, some non-blocking data structures are weak enough to be implemented without special atomic primitives. These exceptions include:

1、a single-reader single-writer [ring buffer](http://en.wiki.sxisa.org/wiki/Circular_buffer) [FIFO](http://en.wiki.sxisa.org/wiki/FIFO_(computing_and_electronics)), with a size which evenly divides the overflow of one of the available unsigned integer types, can unconditionally be [implemented safely](http://en.wiki.sxisa.org/wiki/Producer–consumer_problem#Without_semaphores_or_monitors) using only a memory barrier

> NOTE: 上述  [ring buffer](http://en.wiki.sxisa.org/wiki/Circular_buffer) [FIFO](http://en.wiki.sxisa.org/wiki/FIFO_(computing_and_electronics)) ，其实就是一个环形队列

2、[Read-copy-update](http://en.wiki.sxisa.org/wiki/Read-copy-update) with a single writer and any number of readers. (The readers are wait-free; the writer is usually lock-free, until it needs to reclaim memory).

3、[Read-copy-update](http://en.wiki.sxisa.org/wiki/Read-copy-update) with multiple writers and any number of readers. (The readers are wait-free; multiple writers generally serialize with a lock and are not obstruction-free).