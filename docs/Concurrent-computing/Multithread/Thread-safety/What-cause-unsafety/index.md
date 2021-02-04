# 关于本章



## What cause unsafety？

哪些原因导致shared data线程不安全呢？本节对这个topic进行总结，知道这些原因是实现thread safety的前提；

### Race condition

这在 "Race"  章节中进行了讨论；

### Out of order execution(主要是memory ordering)

由于Out of order execution(主要是memory reordering)也会对导致shared data线程不安全。

关于Out of order execution(主要是memory ordering)，参见下面章节:

1、`Concurrent-computing\Expert-Jeff-Preshing\Lock-Free-Programming`

2、工程hardware的如下章节:

a、`CPU\Execution-of-instruction`

b、`CPU-memory-access\Memory-ordering`



## Design of C++ atomic library

前面总结了导致非线程安全的两个原因，这让我想到了C++ atomic library，显然C++ atomic library提供了对这两个cause的控制:

1、`std::atomic`: 避免了 Race condition

2、`std::memory_ordering`: 对memory ordering进行控制

更多详细的信息，参见工程programming language的 `C++\Guide\Memory-model\Atomic-operations-library\Design-and-implementation` 章节。

## How to solve?

需要对shared data进行concurrency control；