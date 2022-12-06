# 关于本工程

[Parallel computing](https://en.wikipedia.org/wiki/Parallel_computing)、[Distributed computing](https://en.wikipedia.org/wiki/Concurrent_computing)、[Concurrent computing](https://en.wikipedia.org/wiki/Concurrent_computing) 这三个computing是软件工程师经常碰到的，它们紧密关联又各有不同，正如在wikipedia [Distributed computing](https://en.wikipedia.org/wiki/Distributed_computing) 的 [Parallel and distributed computing](https://en.wikipedia.org/wiki/Distributed_computing#Parallel_and_distributed_computing) 章节所述：

> The terms "[concurrent computing](https://en.wikipedia.org/wiki/Concurrent_computing)", "[parallel computing](https://en.wikipedia.org/wiki/Parallel_computing)", and "distributed computing" have a lot of overlap(重叠), and no clear distinction exists between them. The same system may be characterized both as "parallel" and "distributed"; the processors in a typical distributed system run concurrently in parallel. Parallel computing may be seen as a particular tightly coupled form of distributed computing, and distributed computing may be seen as a loosely coupled form of parallel computing.

所以，有必要对它们进行研究。本工程按照wikipedia [Parallel computing](https://en.wikipedia.org/wiki/Parallel_computing) 的思路进行组织，它将 [Distributed computing](https://en.wikipedia.org/wiki/Concurrent_computing)、[Concurrent computing](https://en.wikipedia.org/wiki/Concurrent_computing) 都归入 [Parallel computing](https://en.wikipedia.org/wiki/Parallel_computing) 的范轴，本工程对相关理论知识进行梳理。



## 时代背景、发展趋势 以及 why need parallel computing

关于当前的时代背景、发展趋势，在下面章节中进行了描述:

1、工程`hardware`的`Modern-CPU\Tendency-toward-parallel-computing`章节

2、`Book-Designing-Data-Intensive-Applications`

下面是截取自 `Book-Designing-Data-Intensive-Applications`:

> 4、CPU clock speeds are barely increasing, but multi-core processors are standard, and networks are getting faster. This means parallelism is only going to increase.

> NOTE: 
>
> 一、CPU clock speed几乎不增加，multi-core processors被广泛采用，网络越来越快，这些都意味着“parallelism”将会增加。



### Why need parallel computing ?

充分发挥computation power，克服越来越多的挑战。

