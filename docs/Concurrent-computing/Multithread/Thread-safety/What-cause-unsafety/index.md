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

前面总结了导致非线程安全的两个原因，这让我想到了C++ atomic library，显然C++ atomic library提供了对这两个cause的控制:

1、`std::atomic`: 避免了 Race condition

2、`std::memory_ordering`: 对memory ordering进行控制

> 多个thread同时对原子变量进行操作，原子变量能够保证原子性，即没有中间状态，使用原子变量进行lockless programming还需要克服的一个问题是：memory reordering，因此，还需要对memory order进行控制。

更多详细的信息，参见工程programming language的 `C++\Guide\Memory-model\Atomic-operations-library\Design-and-implementation` 章节。