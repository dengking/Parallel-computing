# The challenge of Distributed Systems

本文对Distributed Systems的challenge、trouble进行总结，它们其实反映的是分布式系统的特征，只有把握了它们才能够更好地理解分布式系统的各种技术。

## 素材

在下面文章中，对challenge of Distributed Systems进行了总结。

### Book-Designing-Data-Intensive-Applications

在这本书的`PART-II-Distributed-Data\CHAPTER-8-The-Trouble-with-Distributed-Systems`中进行了非常好的总结，参见对应的章节。

### [ZooKeeper](https://zookeeper.apache.org/doc/r3.6.2/zookeeperOver.html)

> ZooKeeper: A Distributed Coordination Service for Distributed Applications

通过它的实现我们是可以看出distributed computing中的各种trouble的。



### embedded [Distributed Software Design: Challenges and Solutions](https://www.embedded.com/distributed-software-design-challenges-and-solutions/)

> NOTE: 
>
> 总结的还可以

### martinfowler [Patterns of Distributed Systems](https://martinfowler.com/articles/patterns-of-distributed-systems/)

其中也进行了探讨。

### wikipedia [Distributed computing](https://en.wikipedia.org/wiki/Distributed_computing)

2、[lack of a global clock](https://en.wikipedia.org/wiki/Clock_synchronization)

> NOTE: 
>
> 这是分布式计算的挑战

3、independent failure of components.

> NOTE: 
>
> 1、简而言之，去中心化的

### [Byzantine fault](https://en.wikipedia.org/wiki/Byzantine_fault)

它是非常能够体现分布式系统的特性、调整、困难的，参见: `Byzantine-Fault-Tolerance` 章节。



## [Lack of a global clock](https://en.wikipedia.org/wiki/Clock_synchronization) and ordering

ordering 是computational 的前提，而分布式系统"[Lack of a global clock](https://en.wikipedia.org/wiki/Clock_synchronization) "，那这要如何解决呢？这在 `Distributed-computing\Theory\Ordering` 章节进行了讨论。
