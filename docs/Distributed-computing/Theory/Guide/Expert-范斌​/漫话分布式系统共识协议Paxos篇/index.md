# zhihu [漫话分布式系统共识协议: Paxos篇](https://zhuanlan.zhihu.com/p/35737689)

可能和很多人的印象相反, Paxos其实是一个异常简洁而精巧的算法. 解读一遍Paxos算法其实只需要5分钟. 本文将集中在经典的basic Paxos上, 而不会涉及其各种变种(实在也太多了).

## 前言

本文是“漫话分布式系统共识协议”这个系列的第二篇. 前一篇"[2PC/3PC篇](https://zhuanlan.zhihu.com/p/35298019)"介绍了分布式共识算法中最早的2PC和3PC两位老前辈. 