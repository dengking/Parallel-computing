# preshing [Acquire and Release Semantics](https://preshing.com/20120913/acquire-and-release-semantics/)

Generally speaking, in [lock-free programming](http://preshing.com/20120612/an-introduction-to-lock-free-programming), there are two ways in which threads can manipulate shared memory: 

1、They can compete with each other for a resource, or 

2、they can pass information co-operatively from one thread to another. 

> NOTE:
>
> 1、对应的是lock-based
>
> 2、对应的是lock-free

**Acquire and release semantics** are crucial(至关重要的) for the latter: reliable passing of information between threads. 

> NOTE: 
>
> 1、上面这段话的意思是: 使用**Acquire and release semantics** 对于实现thread之间pass information co-operatively to access shared memory至关重要，也就是: 我们使用**Acquire and release semantics** 来实现lock free

In fact, I would venture(冒进的) to guess that incorrect or missing acquire and release semantics is the `#1` type of lock-free programming error.

In this post, I’ll demonstrate various ways to achieve **acquire and release semantics** in C++. I’ll touch upon the C++11 atomic library standard in an introductory way, so you don’t already need to know it. And to be clear from the start, the information here pertains(适合、属于) to lock-free programming *without* [sequential consistency](http://preshing.com/20120612/an-introduction-to-lock-free-programming#sequential-consistency). We’re dealing directly with memory ordering in a multicore or multiprocessor environment.

Unfortunately, the terms *acquire and release semantics* appear to be in even worse shape than the term *lock-free*, in that the more you scour the web, the more seemingly contradictory definitions you’ll find. Bruce Dawson offers a couple of good definitions (credited to Herb Sutter) about halfway through [this white paper](http://msdn.microsoft.com/en-us/library/windows/desktop/ee418650.aspx). I’d like to offer a couple of definitions of my own, staying close to the principles behind C++11 atomics:

## Definition 

> NOTE: 原文并没有本节的标题。
>
> 1、本节描述Acquire semantics、Release semantics的含义
>
> 2、Acquire semantics、Release semantics是目标、要求，后面Implementation章节描述如何来实现它们

### Acquire semantics

![img](https://preshing.com/images/read-acquire.png)

**Acquire semantics** is a property that can only apply to operations that **read** from shared memory, whether they are [read-modify-write](http://preshing.com/20120612/an-introduction-to-lock-free-programming#atomic-rmw) operations or plain loads. The operation is then considered a **read-acquire**. **Acquire semantics** prevent memory reordering of the **read-acquire** with any read or write operation that **follows** it in program order.

> NOTE: 将 **Acquire semantics** 应用于一个read operation，则就是: read-acquire；read-acquire对memory reordering有如下要求:
>
> "**Acquire semantics** prevent memory reordering of the **read-acquire** with any read or write operation that **follows** it in program order."

### Release semantics

![img](https://preshing.com/images/write-release.png)

**Release semantics** is a property that can only apply to operations that **write** to shared memory, whether they are read-modify-write operations or plain stores. The operation is then considered a **write-release**. **Release semantics** prevent memory reordering of the **write-release** with any read or write operation that **precedes** it in program order.

## Implementation

> NOTE: 本节描述如何来实现Acquire semantics、Release semantics

Once you digest the above definitions, it’s not hard to see that acquire and release semantics can be achieved using simple combinations of the memory barrier types I [described at length in my previous post](http://preshing.com/20120710/memory-barriers-are-like-source-control-operations). The barriers must (somehow) be placed *after* the **read-acquire operation**, but *before* the **write-release**. *[Update: Please note that these barriers are technically more strict than what’s required for acquire and release semantics on a single memory operation, but they do achieve the desired effect.]*

> NOTE: 下面的例子非常形象地说明了上面这一段的内容；

![img](https://preshing.com/images/acq-rel-barriers.png)