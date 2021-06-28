# Time and ordering in distributed system

这个领域的开拓者是: [Leslie Lamport](https://en.wanweibaike.com/wiki-Leslie_Lamport#Distributed_Systems):

> "Time, Clocks, and the Ordering of Events in a Distributed System",[[6\]](https://en.wanweibaike.com/wiki-Leslie_Lamport#cite_note-timeclocks-6) which received the [PODC Influential Paper Award](https://en.wanweibaike.com/wiki-Dijkstra_Prize) in 2000,[[12\]](https://en.wanweibaike.com/wiki-Leslie_Lamport#cite_note-12)

## Make it computational-ordering

一、关于order和computation之间的关系，参见工程discrete的`Relation-structure-computation\Make-it-computational`章节。

二、在分布式系统中，尤其需要处理time、ordering的问题

### 比较好的文章

在下面文章，对Distributed Computing中，time、ordering问题进行了比较好的探讨:

1、cockroachlabs [Living Without Atomic Clocks](https://www.cockroachlabs.com/blog/living-without-atomic-clocks/)

其中讨论了"The Importance of Time in Distributed Systems"。

2、uci [Distributed Computing Concepts - Global Time and State in Distributed Systems](https://www.ics.uci.edu/~cs237/lecture2020/GlobalTimeState.pdf)



### Ordering and synchronization 

本节讨论ordering和synchronization之间的关系:

1) synchronization的目的是保持ordering 

上述"Distributed system的disordering本质"章节的内容论证了这个观点。

2) ordering能够用于实现synchronization

上述"Ordering in zookeeper"章节的内容论证了这个观点。

### Ordering and consistency

需要讨论ordering 和 consistency之间的关系，在下面文章中对此进行了描述:

cornell [Distributed Systems: Ordering and Consistency](https://www.cs.cornell.edu/courses/cs6410/2018fa/slides/14-lamport-clocks.pdf)



## Distributed system的disordering本质

一、Distributed system是典型multiple entity model，即它是由多个不同的、独立的entity构成，这些entity的execution是独立的、随机的，这就是"distributed system的disordering本质"；但是从系统的整体而言，如果要实现computation、确定性，就需要设法消除这种disordering，显然就需要加入一定的control。control机制的目的是消除disordering，实现ordering。

二、通过 wikipedia [Distributed computing](https://en.wikipedia.org/wiki/Distributed_computing) 、`Distributed-system-challenge`  中的内容，我们已经知道，分布式系统的一个非常大的调整是: 

[lack of a global clock](https://en.wikipedia.org/wiki/Clock_synchronization)

因此，在分布式系统中就无法依赖于physical clock来进行ordering；

## 如何克服这些问题

### 使用logical clock

参见 `Logical-clock` 章节。

### 实现global clock

Google spanner:  "GPS和原子钟"，cockroachlabs [Living Without Atomic Clocks](https://www.cockroachlabs.com/blog/living-without-atomic-clocks/) 中，总结如下:

> One of the most surprising and inspired facets of Spanner is its use of atomic clocks and GPS clocks to give participating nodes *really* accurate wall time synchronization. The designers of Spanner call this [“TrueTime”](https://cloud.google.com/spanner/docs/true-time-external-consistency), and it provides a tight bound on clock offset between any two nodes in the system. This lets them do pretty nifty things!

参见`Google-spanner`章节。





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