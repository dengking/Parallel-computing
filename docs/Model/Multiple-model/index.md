# Multiple model

本文使用一个统一的模型来对对parallel computing、concurrent computing、distributed computing进行描述，这个模型命名为“**multiple model**”。

## What is multiple model?

在有了parallel computing、concurrent computing、distributed computing的一些理论知识后，我们会发现它们涉及到了非常多类似的概念（问题）：

- race condition
- atomic
- communication
- consistency
- ......

这是因为parallel computing、concurrent computing、distributed computing都涉及“**多个**”element或entity（这些element并发或并行地运行中），它们可能同时操作shared data(shared memory)，所以它们都会涉及与**多个**(multiple)相关的问题，我们将使用“**multiple model**”来对它们进行统一的描述。



### Concurrency/parallel element/unit

本节总结在各种情况下，可以concurrent/parallel执行/发生的element，其中部分参考了wikipedia [Parallel computing](https://en.wikipedia.org/wiki/Parallel_computing)中总结的可能的element：

1、machine/server

主要是distributed computing领域。

2、[Process](https://en.wikipedia.org/wiki/Process_(computing))

主要是multiple process。

3、[Thread](https://en.wikipedia.org/wiki/Thread_(computing))

主要是multiple thread。

4、[Fiber](https://en.wikipedia.org/wiki/Fiber_(computer_science))

主要是coroutine。

5、[Instruction window](https://en.wikipedia.org/wiki/Instruction_window) 

> NOTE: 维基百科这里使用的是[Instruction window](https://en.wikipedia.org/wiki/Instruction_window)，我有些不懂，我觉得使用 [instruction](https://en.wikipedia.org/wiki/Instruction_(computer_science)) 会更加准确。

6、network connection

主要是concurrent server。

7、transaction

主要是DBMS中。

8、......

### Shared data

参见 `Shared-data` 章节。

### Operation on shared data

对shared data的操作可以概括为如下两种:

1、read

2、write

关于read、write，在`./Read-and-write`中，会进行专门总结。

### Relation:  many-to-one

Many: multiple entity

One: shared data



### Problems

**多个**比**单个**的复杂度要高很多，在**multiple model**中存在着一系列问题：

- 多个element之间的race
- 多个element之间如何进行communication（消息传递，进行沟通）
- 多个element之间如何进行coordination（协作、协调）
- 多个entity之间如何达成consensus（共识）
- ......

下面是一些用于说明problem的词语:

1、memory corruption 来源: microsoft [Lockless Programming Considerations for Xbox 360 and Microsoft Windows](https://docs.microsoft.com/en-us/windows/win32/dxtecharts/lockless-programming?redirectedfrom=MSDN)

### From single machine to multiple machine

> NOTE: 参见《One-to-many》

#### Single machine

单个process：它的element就是thread

多个process：它的element就是process

#### Multiple machine

多个process：它的element就是process



## Nature

从上到下，问题越来越复杂，在每一层级，解决上述问题的技术手段都是不同的，本质上它们所解决的都是相同类型的问题，这是我们建立其multiple model的价值所在。

当然，随着越来越复杂，每一种也都会涉及到简单问题中不存在的问题。

## Concurrency control

在multiple model中，进行Concurrency control是非常重要的。



## Communication

其实就是entity之间的communication。



## Consistency

关于此的非常好的论述:

### zhuanlan.zhihu [高并发编程--多处理器编程中的一致性问题(上)](https://zhuanlan.zhihu.com/p/48157076)

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