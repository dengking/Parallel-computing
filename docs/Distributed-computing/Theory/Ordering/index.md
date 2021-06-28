# Ordering in distributed system

一、关于order和computation之间的关系，参见工程discrete的`Relation-structure-computation\Make-it-computational`章节。

二、在distributed system中，ordering是一个非常重要的主题。

检索 "ordering in distributed system"，最先弹出的往往是: Lamport clock，这部分内容放到了`Distributed-computing\Theory\Logical-clock`章节中。



## Distributed system的disordering本质

Distributed system是典型multiple entity model，即它是由多个不同的、独立的entity构成，这些entity的execution是独立的、随机的，这就是"distributed system的disordering本质"；但是从系统的整体而言，如果要实现computation、确定性，就需要设法消除这种disordering，显然就需要加入一定的control。control机制的目的是消除disordering，实现ordering。



## Logical clock

参见 `Logical-clock` 章节。



## Ordering and synchronization 

本节讨论ordering和synchronization之间的关系:

1) synchronization的目的是保持ordering 

上述"Distributed system的disordering本质"章节的内容论证了这个观点。

2) ordering能够用于实现synchronization

上述"Ordering in zookeeper"章节的内容论证了这个观点。

## Ordering and consistency

需要讨论ordering 和 consistency之间的关系，在下面文章中对此进行了描述:

cornell [Distributed Systems: Ordering and Consistency](https://www.cs.cornell.edu/courses/cs6410/2018fa/slides/14-lamport-clocks.pdf)



## Example

### Ordering in TensorFlow 

在阅读TensorFlow whitepaper的时候，其中有这样的描述:

> Our implementation also sometimes inserts **control dependencies** to enforce **orderings** between otherwise independent operations as a way of, for example, controlling the peak memory usage.

### Ordering in zookeeper

在 [ZooKeeper: A Distributed Coordination Service for Distributed Applications](https://zookeeper.apache.org/doc/current/zookeeperOver.html) 中很多关于ordering的描述:

> The ZooKeeper implementation puts a premium(保证) on high performance, highly available, strictly **ordered** access. 



> The strict **ordering** means that sophisticated synchronization primitives can be implemented at the client.



> **ZooKeeper is ordered.** ZooKeeper stamps(贴上时间戳) each update with a number that reflects the order of all ZooKeeper transactions. Subsequent operations can use the order to implement higher-level abstractions, such as synchronization primitives.



从上面的描述可以看出，"ordering能够用于实现synchronization"。

> Sequential Consistency - Updates from a client will be applied in the order that they were sent.