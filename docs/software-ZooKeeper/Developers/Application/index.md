# 关于本章

本章讨论ZK的application。

## zookeeper [ZooKeeper Recipes and Solutions](https://zookeeper.apache.org/doc/r3.6.2/recipes.html)

> NOTE: 原文描述的各种solution在distributed computing中是典型的技术。

In this article, you'll find guidelines for using ZooKeeper to implement higher order functions. All of them are conventions implemented at the **client** and do not require special support from ZooKeeper. Hopefully the community will capture these conventions in client-side libraries to ease their use and to encourage standardization.

One of the most interesting things about ZooKeeper is that even though ZooKeeper uses *asynchronous* notifications, you can use it to build *synchronous* consistency primitives, such as queues and locks. As you will see, this is possible because ZooKeeper imposes an overall order on updates, and has mechanisms to expose this ordering.

> NOTE: ZK将它的order暴露给了user

### Out of the Box Applications: Name Service, Configuration, Group Membership

> NOTE: 开箱即用的功能

Name service and configuration are two of the primary applications of ZooKeeper. These two functions are provided directly by the ZooKeeper API.

Another function directly provided by ZooKeeper is *group membership*. The group is represented by a node. Members of the group create ephemeral nodes under the group node. Nodes of the members that fail abnormally will be removed automatically when ZooKeeper detects the failure.

### Barriers

### Queues

### Locks

### Two-phased Commit

### Leader Election

参见`./Lead-election`



## zhuanlan.zhihu [为什么需要 Zookeeper](https://zhuanlan.zhihu.com/p/69114539)