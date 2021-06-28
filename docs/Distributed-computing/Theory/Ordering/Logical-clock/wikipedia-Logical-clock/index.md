# wikipedia [Logical clock](https://en.wikipedia.org/wiki/Logical_clock)

A **logical clock** is a mechanism for capturing chronological and causal relationships in a [distributed system](https://en.wanweibaike.com/wiki-Distributed_system). Distributed systems may have no physically synchronous global clock, so a logical clock allows global ordering on events from different processes in such systems. The first implementation, the [Lamport timestamps](https://en.wanweibaike.com/wiki-Lamport_timestamps), was proposed by [Leslie Lamport](https://en.wanweibaike.com/wiki-Leslie_Lamport) in 1978 ([Turing Award](https://en.wanweibaike.com/wiki-Turing_Award) in 2013).

> NOTE: 
>
> 一、
>
> chronological relationship 时间关系
>
> causal relationship 因果关系
>
> 二、"Distributed systems may have no physically synchronous global clock"
>
> 这是分布式系统的挑战

## Algorithms

Some noteworthy logical clock algorithms are:

1、[Lamport timestamps](https://en.wanweibaike.com/wiki-Lamport_timestamps), which are monotonically increasing software counters.

> NOTE: 
>
> 非常简单，但是具有重要的意义

2、[Vector clocks](https://en.wanweibaike.com/wiki-Vector_clock), that allow for partial ordering of events in a distributed system.

3、[Version vectors](https://en.wanweibaike.com/wiki-Version_vector), order replicas, according to updates, in an [optimistic replicated system](https://en.wanweibaike.com/wiki-Optimistic_replication).

4、[Matrix clocks](https://en.wanweibaike.com/wiki-Matrix_clock), an extension of vector clocks that also contains information about other processes' views of the system.

> NOTE:
>
> 从名称就可以看出是vector clock的升级版
