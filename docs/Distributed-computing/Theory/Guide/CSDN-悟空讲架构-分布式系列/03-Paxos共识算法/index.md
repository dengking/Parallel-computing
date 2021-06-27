# csdn [诸葛亮 VS 庞统，拿下 Paxos 共识算法](https://blog.csdn.net/jackson0714/article/details/112593110?spm=1001.2014.3001.5501)

从这篇开始，将会讲解分布式的八大协议/算法。本篇主要讲解 Paxos 共识算法。

本文主要内容如下：

![本文主要内容](https://img-blog.csdnimg.cn/img_convert/b93741e8e6c624bdd0fe7274ca65a149.png)

## Paxos 算法

`Paxos` 是分布式算法中的老大哥，可以说 Paxos 是分布式共识的代名词。最常用的分布式共识算法都是基于它改进。比如 Raft 算法（后面也会介绍）。所以学习分布式算法必须先学习 Paxos 算法。

Paxos 算法主要包含两个部分：

1、**Basic Paxos 算法**：多个节点之间如何就某个值达成共识。（这个值我们称作`提案 Value`）

2、**Multi-Paxos 算法**：执行多个 Basic Paxos 实例，就一系列值达成共识。

Basic Paxos 算法是 Multi-Paxos 思想的核心，Multi 的意思就是多次，也就是说多执行几次 Basic Paxos 算法。所以 Basic Paxos 算法是重中之重。

## 三国中的 Paxos

三国中刘备集团，有两大军师：诸葛亮和庞统，都是非常厉害的人物，当他们有不同作战计划给多名武将时，如何达成一致？

### 角色

Paxos 中有三种角色：提议者、接受者、学习者。

