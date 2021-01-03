# Multiple model

本文使用一个统一的模型来对对parallel computing、concurrent computing、distributed computing进行描述，这个模型命名为“**multiple model**”。

## What is multiple model?

在有了parallel computing、concurrent computing、distributed computing的一些理论知识后，我们会发现它们涉及到了非常多类似的概念（问题）：

- race condition
- atomic
- communication
- ......

这是因为parallel computing、concurrent computing、distributed computing都涉及“**多个**”element或entity（这些element并发或并行地运行中），它们可能同时操作shared data，所以它们都会涉及与**多个**相关的问题，我们将使用“**multiple model**”来对它们进行统一的描述。



### Concurrency/parallel element/unit

本节总结在各种情况下，可以concurrency/parallel执行/发生的element，其中部分参考了wikipedia [Parallel computing](https://en.wikipedia.org/wiki/Parallel_computing)中就总结了可能的element：

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

1、DBMS 

table

2、...

> NOTE: 后续进行补充

### Operation on shared data

对shared data的操作可以概括为如下两种:

1、read

2、write

因此，大多数concurrency control都是基于read、write来做的，比如:

1、参见下面的: "Operation conflict on shared data"

2、atomic instruction，尤其是read-modify-write系列instruction，参见`Concurrent-computing\Concurrency-control\Non-blocking\Atomic-instruction`章节

3、read-write lock

4、consistent model也是基于read、write而建立的，其中所谓的看见，其实是read

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

#### Operation conflict on shared data

在DBMS中，对此有着较好的总结，我们可以借用其中的理论:

|           | read        | write    |
| --------- | ----------- | -------- |
| **read**  | no conflict | conflict |
| **write** | conflict    | conflict |

参见: 

1、[Read–write conflict](http://en.wiki.sxisa.org/wiki/Read%E2%80%93write_conflict)

2、[Write–read conflict](http://en.wiki.sxisa.org/wiki/Write%E2%80%93read_conflict)

3、[Write–write conflict](http://en.wiki.sxisa.org/wiki/Write%E2%80%93write_conflict)



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

### Serialization

> NOTE: 本节标题的含义是"串行"，串行的英语是serial，显然，串行和并行是相反的。

参见文章：维基百科[Linearizability](https://en.wikipedia.org/wiki/Linearizability)。

实现Serialization的方式有：

- atomic操作
- 加锁机制

[What is the difference between linearizability and serializability?](https://stackoverflow.com/questions/4179587/what-is-the-difference-between-linearizability-and-serializability)

维基百科[Mutual exclusion](https://en.wikipedia.org/wiki/Mutual_exclusion)



## Communication

其实就是entity之间的communication。



