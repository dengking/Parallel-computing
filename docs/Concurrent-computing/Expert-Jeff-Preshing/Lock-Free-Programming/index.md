# 关于本章

本章记录阅读 lock free 主题的文章。

## 文章总结

作者的写作思路: 先抛出问题，然后概述技术图景，然后自底向上地描述各种技术；

在 preshing [Memory Reordering Caught in the Act](https://preshing.com/20120515/memory-reordering-caught-in-the-act/) 中，抛出问题；

在 preshing [An Introduction to Lock-Free Programming](https://preshing.com/20120612/an-introduction-to-lock-free-programming/) 综述 lock-free programming的技术；

在下面两篇文章中描述各种可能的memory reordering，底层的、控制memory ordering的技术:

1、 compile-time: preshing [Memory Ordering at Compile Time](https://preshing.com/20120625/memory-ordering-at-compile-time/)

2、runtime: preshing [Memory Barriers Are Like Source Control Operations](https://preshing.com/20120710/memory-barriers-are-like-source-control-operations/)

在 preshing [Weak vs. Strong Memory Models](https://preshing.com/20120930/weak-vs-strong-memory-models/) 中，讨论了hardware memory model，它决定了runtime memory ordering。



### preshing [Memory Reordering Caught in the Act](https://preshing.com/20120515/memory-reordering-caught-in-the-act/)

结合具体的例子来说明 memory reordering的存在，显然memory reordering是编写lock-free concurrent program的挑战、

阅读完这篇文章，有如下疑问:

1、memory barrier 和 full memory barrier

2、memory barrier 和 acquire-release semantic

### preshing [An Introduction to Lock-Free Programming](https://preshing.com/20120612/an-introduction-to-lock-free-programming/)

介绍了lock-free的含义

介绍了lock-free programming的技术总览

### preshing [Memory Ordering at Compile Time](https://preshing.com/20120625/memory-ordering-at-compile-time/)

介绍了compiler barrier，以及memory barrier 和 compiler barrier的关系。

#### Memory barrier 和 compiler barrier的关系

memory barrier 会产生 compiler barrier的效果，它们是 Implied Compiler Barriers。

关于这一点，在 preshing [Memory Barriers Are Like Source Control Operations](https://preshing.com/20120710/memory-barriers-are-like-source-control-operations/) 中也有总结: 

> any operation which acts as a memory barrier also prevents [compiler reordering](http://preshing.com/20120625/memory-ordering-at-compile-time).

### preshing [Memory Barriers Are Like Source Control Operations](https://preshing.com/20120710/memory-barriers-are-like-source-control-operations/)

介绍各种memory barrier，我们使用memory barrier的目的是: "You can enforce correct memory ordering on the processor by issuing any instruction which acts as a **memory barrier**. "

在高级programming language，比如C++中，各种memory semantic其实最终都是依赖于这些memory barrier instruction来实现的，关于这一点，在:

1、在 preshing [Acquire and Release Semantics](https://preshing.com/20120913/acquire-and-release-semantics/) 中，有这样的介绍: 

> it’s not hard to see that acquire and release semantics can be achieved using simple combinations of the memory barrier types I [described at length in my previous post](http://preshing.com/20120710/memory-barriers-are-like-source-control-operations). 



#### Memory barrier and memory reordering

这篇文章所总结的四种memory barrier分别对应了四种memory reordering。



## Memory reordering的基本原则

compiler、CPU进行memory reordering的基本原则

## Memory model of CPU/hardware memory model

这是作者的系列文章都会涉及到的一个问题。主要是在文章 preshing [Weak vs. Strong Memory Models](https://preshing.com/20120930/weak-vs-strong-memory-models/) 中进行了讨论。



## 控制memory ordering

### 控制runtime memory ordering

这涉及了CPU的运行机制

### 控制compile-time memory ordering

这涉及了compiler、programming language specialization，尤其是expression evaluation的问题

下面是一些draft

#### draft function call acts as a compiler barrier

在阅读[Memory Reordering Caught in the Act](https://preshing.com/20120515/memory-reordering-caught-in-the-act/)的例子的source code的时候，发现了下面的内容:

```C++
class MersenneTwister
{
    unsigned int m_buffer[MT_LEN];
    int m_index;

public:
    MersenneTwister(unsigned int seed);
    // Declare noinline so that the function call acts as a compiler barrier:
    unsigned int integer() __attribute__((noinline));
};
```

显然，上述compiler barrier是compile-time对memory ordering进行控制。

下面是Google: function call acts as a compiler barrier

stackoverflow [c++ atomic: would function call act as memory barrier?](https://stackoverflow.com/questions/40641904/c-atomic-would-function-call-act-as-memory-barrier)

preshing [Memory Ordering at Compile Time](https://preshing.com/20120625/memory-ordering-at-compile-time/)



## Fence VS atomic variable

在 preshing [Acquire and Release Semantics](https://preshing.com/20120913/acquire-and-release-semantics/) # Without Fences in Portable C++11 中，对这个话题进行了讨论。