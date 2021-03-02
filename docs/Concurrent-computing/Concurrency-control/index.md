# Concurrency control

Concurrency control无处不在。

## Concurrency control的目的

最最基本的目的: 保证shared data的correction；

其他的系统，可能有着其他的目的；



## wikipedia [Concurrency control](https://en.wikipedia.org/wiki/Concurrency_control)

In information technology and computer science, especially in the fields of computer programming, operating systems, multiprocessors, and databases, **concurrency control** ensures that correct results for concurrent operations are generated, while getting those results as quickly as possible.

> NOTE: concurrency control存在于computer science的各个领域:
>
> 1) programming language
>
> 2) operating system
>
> 3) multiprocessor
>
> 4) database
>
> 上面这段话的最后一句总结了concurrency control的目标:
>
> 1) 保证correct
>
> 2) 保证performance



Computer systems, both software and hardware, consist of modules, or components. Each component is designed to operate correctly, i.e., to obey or to meet certain **consistency rules**. When components that operate concurrently interact by **messaging** or by **sharing accessed data** (in memory or storage), a certain component's **consistency** may be violated by another component. The general area of concurrency control provides rules, methods, design methodologies, and theories to maintain the consistency of components operating concurrently while interacting, and thus the **consistency** and **correctness** of the whole system. 

> NOTE: 上面这段话的前面部分其实就是multiple model(参见`Model\Multiple-model`章节)。

Introducing concurrency control into a system means applying operation constraints which typically result in some performance reduction. Operation consistency and correctness should be achieved with as good as possible efficiency, without reducing performance below reasonable levels. Concurrency control can require significant additional complexity and overhead in a concurrent algorithm compared to the simpler sequential algorithm.

Concurrency control can require significant additional complexity and overhead in a [concurrent algorithm](https://en.wikipedia.org/wiki/Concurrent_algorithm) compared to the simpler [sequential algorithm](https://en.wikipedia.org/wiki/Sequential_algorithm).

> NOTE: concurrent algorithm VS sequential algorithm

### Concurrency control in databases

> NOTE: 这部分内容放到了工程DB中



## wikipedia [Distributed concurrency control](https://en.wikipedia.org/wiki/Distributed_concurrency_control)





## wikipedia [Category:Concurrency control](https://en.wikipedia.org/wiki/Category:Concurrency_control)



## wikipedia [Category:Concurrency control algorithms](https://en.wikipedia.org/wiki/Category:Concurrency_control_algorithms)



## 从multiple model来看待concurrency control

由于对shared data的operation只有read、write两大类，因此，

一、大多数concurrency control都是基于read、write来做的

比如:

1、atomic instruction，尤其是read-modify-write系列instruction，参见`Concurrent-computing\Concurrency-control\Non-blocking\Atomic-instruction`章节

2、read-write lock

二、可以基于read and write来进行optimize

在`./How-to-choose-technique`中进行了总结。

## How to optimize？

> NOTE: 显然，需要遵循optimization principle

在`./How to choose technique`中进行了总结。



## TODO



[Test and test-and-set](https://en.wikipedia.org/wiki/Test_and_test-and-set)

[Consistency in Distributed Systems](https://www.microsoft.com/en-us/research/wp-content/uploads/2016/06/printversion.pdf)

