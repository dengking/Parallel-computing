# Out of order execution and memory reordering

由于Out of order execution(主要是memory reordering)也会对导致shared data线程不安全，关于此的例子有:

1、参见 "aristeia-C++and-the-Perils-of-Double-Checked-Locking" 章节

其中对此进行了非常好的介绍

关于Out of order execution(主要是memory ordering)，参见下面章节:

1、`Concurrent-computing\Expert-Jeff-Preshing\Lock-Free-Programming`

他的这个系列的文章非常好，可以作为guideline。

2、工程hardware的如下章节:

a、`CPU\Execution-of-instruction`

b、`CPU-memory-access\Memory-ordering`



## Order of write to shared data may be different among different threads

这是在阅读一篇文章时，其中提出的一个观点，我觉得这个观点比较好，在下面文章中，描述了相关的例子:

1、参见 "aristeia-C++and-the-Perils-of-Double-Checked-Locking" 章节

2、queue.acm [You Don’t Know Jack about Shared Variables or Memory Models](https://queue.acm.org/detail.cfm?id=2088916)