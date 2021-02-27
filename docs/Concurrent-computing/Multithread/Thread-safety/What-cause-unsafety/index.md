# 关于本章



## What cause unsafety？

哪些原因导致shared data线程不安全呢？本节对这个topic进行总结，知道这些原因是实现thread safety的前提；

### Race condition

这在 "Race"  章节中进行了讨论；

### Out of order execution(主要是memory ordering)

这在 "Out-of-order-execution-and-memory-reordering" 章节进行了讨论。



## How to solve?

需要对shared data进行concurrency control；显然各种concurrency control中，都需要解决前面描述的cause，典型的例子就是C++ atomic library，参见下面"Implementation: C++ atomic library"段。



## Implementation: C++ atomic library

参见工程programming language的 `C++\Guide\Memory-model` 章节。