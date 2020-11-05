# Multiple model

本文使用一个统一的模型来对对parallel computing、concurrent computing、distributed computing进行描述，这个模型命名为“**multiple model**”。

## What is multiple model?

在有了parallel computing、concurrent computing、distributed computing的一些理论知识后，我们会发现它们涉及到了非常多类似的概念（问题）：

- race condition
- atomic
- communication
- ......

这是因为parallel computing、concurrent computing、distributed computing都涉及“**多个**”element或entity（这些element并发或并行地运行中），所以它们都会涉及与**多个**相关的问题，我们将使用“**multiple model**”来对它们进行统一的描述。

### Element

在维基百科[Parallel computing](https://en.wikipedia.org/wiki/Parallel_computing)中就总结了可能的element：

1) [Process](https://en.wikipedia.org/wiki/Process_(computing))

2) [Thread](https://en.wikipedia.org/wiki/Thread_(computing))

3) [Fiber](https://en.wikipedia.org/wiki/Fiber_(computer_science))

4) [Instruction window](https://en.wikipedia.org/wiki/Instruction_window) 

> NOTE: 维基百科这里使用的是[Instruction window](https://en.wikipedia.org/wiki/Instruction_window)，我有些不懂，我觉得使用 [instruction](https://en.wikipedia.org/wiki/Instruction_(computer_science)) 会更加准确。

## Problems

**多个**比**单个**的复杂度要高很多，在**multiple model**中存在着一系列问题：

- 多个element之间的race

- 多个element之间如何进行message passing（消息传递，进行沟通）

- 多个element之间如何进行coordination（协作、协调）

- 多个entity之间如何达成consensus（共识）

- ......

## From single machine to multiple machine



#### Single machine

单个process：它的element就是thread

多个process：它的element就是process

#### Multiple machine

多个process：它的element就是process



从上到下，问题越来越复杂，在每一层级，解决上述问题的技术手段都是不同的，本质上它们所解决的都是相同类型的问题。

当然，随着越来越复杂，每一种也都会涉及到简单问题中不存在的问题。





## Serialization

> NOTE: 本节标题的含义是"串行"，串行的英语是serial，显然，串行和并行是相反的。

参见文章：维基百科[Linearizability](https://en.wikipedia.org/wiki/Linearizability)。

实现Serialization的方式有：

- atomic操作
- 加锁机制

[What is the difference between linearizability and serializability?](https://stackoverflow.com/questions/4179587/what-is-the-difference-between-linearizability-and-serializability)

维基百科[Mutual exclusion](https://en.wikipedia.org/wiki/Mutual_exclusion)

### Atomic

instruction level： instruction就是atomic的

在application层，也有atomic的概念，也可以实现atomic



transaction： application层的atomic

维基百科[Transaction](https://en.wikipedia.org/wiki/Transaction)

维基百科[Atomicity (database systems)](https://en.wikipedia.org/wiki/Atomicity_(database_systems))

维基百科[Transaction processing](https://en.wikipedia.org/wiki/Transaction_processing)

在各种application层，也提供了实现atomic的Primitive 

cppreference [Atomic operations library](https://en.cppreference.com/w/cpp/atomic)





这些都是理论的概念，也就是它们都是一种简化的模型，其实现实中的一个大型的software system往往既涉及了这三种模型的问题。



## Message passing

其实就是entity之间的communication。



