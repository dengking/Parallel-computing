# Multiple model

本文使用一个统一的模型来对对parallel computing、concurrent computing、distributed computing进行描述，这个模型命名为“**multiple model**”。

## What is multiple model?

在有了parallel computing、concurrent computing、distributed computing的一些理论知识后，我们会发现它们涉及到了非常多类似的概念（问题）：

- race condition
- atomic
- communication
- ......

这是因为parallel computing、concurrent computing、distributed computing都涉及“**多个**”element或entity（这些element并发或并行地运行中），它们可能同时操作shared data，所以它们都会涉及与**多个**(multiple)相关的问题，我们将使用“**multiple model**”来对它们进行统一的描述。



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

显然对于不同的concurrency/parallel element/unit，它们的shared data是可能不同的，下面是一些简单的例子:

1、DBMS  table

2、file

3、memory

4、......

> NOTE: 后续进行补充

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



