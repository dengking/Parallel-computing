# 关于本工程

[Parallel computing](https://en.wikipedia.org/wiki/Parallel_computing)、[Distributed computing](https://en.wikipedia.org/wiki/Concurrent_computing)、[Concurrent computing](https://en.wikipedia.org/wiki/Concurrent_computing)这三个computing是软件工程师经常碰到的，它们紧密关联又各有不同，正如在[Distributed computing](https://en.wikipedia.org/wiki/Distributed_computing)的[
Parallel and distributed computing](https://en.wikipedia.org/wiki/Distributed_computing#Parallel_and_distributed_computing)章节所述：

> The terms "[concurrent computing](https://en.wikipedia.org/wiki/Concurrent_computing)", "[parallel computing](https://en.wikipedia.org/wiki/Parallel_computing)", and "distributed computing" have a lot of overlap, and no clear distinction exists between them. The same system may be characterized both as "parallel" and "distributed"; the processors in a typical distributed system run concurrently in parallel. Parallel computing may be seen as a particular tightly coupled form of distributed computing, and distributed computing may be seen as a loosely coupled form of parallel computing.

所以，有必要对它们进行研究。本repository按照维基百科的[Parallel computing](https://en.wikipedia.org/wiki/Parallel_computing)进行组织，它将[Distributed computing](https://en.wikipedia.org/wiki/Concurrent_computing)、[Concurrent computing](https://en.wikipedia.org/wiki/Concurrent_computing)都归入[Parallel computing](https://en.wikipedia.org/wiki/Parallel_computing)的范轴，本repository对相关理论知识进行梳理。

## Unit of parallel computing

“Unit of parallel computing”即并行的单位（参见文章Unit），类似于在维基百科[Parallel computing](https://en.wikipedia.org/wiki/Parallel_computing)所述的[Types of parallelism](https://en.wikipedia.org/wiki/Parallel_computing#Types_of_parallelism)、或者说是：并发的级别。unit是一个更加抽象/概括的概念，能够让我们正在更加高的角度来理解和分析在parallel computing中的各种问题，能够让我们清楚地看到在不同层级的parallel computing中，都会面临的问题，比如无论是multiple process、multiple thread都涉及通信问题。在后面我们有时候也会使用“entity”来表示，后面我们将使用unit/entity的概念来描述在各种parallel computing中的各种问题，如：

- parallel的entity/unit是什么？
- 这些entity之间如何进行通信？这在[Inter-entity-communication](./Inter-entity-communication.md)中进行了总结。
- 这些entity之间如何进行synchroniz？这在[Synchronization](./Synchronization/Synchronization.md)中进行了总结。
- 有哪些通用的model？这在[Model](./Model/Model.md)中进行了总结。
- 这些entity之间如何达成共识？