# zhihu [漫话分布式系统共识协议: Paxos篇](https://zhuanlan.zhihu.com/p/35737689)

可能和很多人的印象相反, Paxos其实是一个异常简洁而精巧的算法. 解读一遍Paxos算法其实只需要5分钟. 本文将集中在经典的basic Paxos上, 而不会涉及其各种变种(实在也太多了).

## 前言

本文是“漫话分布式系统共识协议”这个系列的第二篇. 前一篇"[2PC/3PC篇](https://zhuanlan.zhihu.com/p/35298019)"介绍了分布式共识算法中最早的2PC和3PC两位老前辈. 

再比因为Lamport稳重带皮的操作, 导致大家口口相传Paxos理解和实现起来有多困难多复杂, 导致出现了Raft这种改良版等等.

> NOTE: 
>
> paxos和raft之间的关系

**敲黑板**: Paxos其实是一个异常简洁而精巧的算法. 解读一遍Paxos算法其实只需要5分钟. 真正复杂的地方在于想清楚Paxos算法在各种failure情形下如何依然"正确"的工作. 只有明白了这一层, 才算练成了Paxos的心法, 才能真正欣赏Paxos算法的精妙设计, 赞叹Lamport的天才思维. 在我看来, Paxos算法(连同Lamport的其他如BFT, Vector Clock等成就)是上个世纪八十/九十年代的经典分布式系统研究中最纯粹最优美, 也是整栋大厦底座最坚实的那一部分.

> NOTE: 
>
> 一、Leslie-Lamport
>
> 参见 `Expert-Leslie-Lamport` 章节
>
> 二、BFT即Byzantine-Fault-Tolerance

插点题外话: 我第一次认真接触和学习Paxos是在CMU时TA分布式系统([15-440, Fall 2012: Distributed Systems](https://link.zhihu.com/?target=http%3A//www.cs.cmu.edu/~dga/15-440/F12/)). 

## Paxos算法描述

考虑一个简化了的Paxos系统: 只有leader和acceptor两种角色.

