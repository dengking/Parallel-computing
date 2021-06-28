# wikipedia [Paxos (computer science)](https://en.wikipedia.org/wiki/Paxos_(computer_science))

**Paxos** is a family of protocols for solving [consensus](https://en.wikipedia.org/wiki/Consensus_(computer_science)) in a network of unreliable processors (that is, processors that may fail). Consensus is the process of agreeing on one result among a group of participants. This problem becomes difficult when the participants or their communication medium may experience failures.[[1\]](https://en.wikipedia.org/wiki/Paxos_(computer_science)#cite_note-agree-1)

**Consensus protocols** are the basis for the [state machine replication](https://en.wikipedia.org/wiki/State_machine_replication) approach to distributed computing, as suggested by [Leslie Lamport](https://en.wikipedia.org/wiki/Leslie_Lamport)[[2\]](https://en.wikipedia.org/wiki/Paxos_(computer_science)#cite_note-clocks-2) and surveyed by [Fred Schneider](https://en.wikipedia.org/wiki/Fred_B._Schneider).[[3\]](https://en.wikipedia.org/wiki/Paxos_(computer_science)#cite_note-schneider-3) **State machine replication** is a technique for converting an algorithm into a fault-tolerant, distributed implementation. Ad-hoc techniques may leave important cases of failures unresolved. The principled approach proposed by Lamport et al. ensures all cases are handled safely.

> NOTE: 
>
> 正如Leslie Lamport[2]和Fred Schneider所建议的，协商一致协议是分布式计算的状态机复制方法的基础。状态机复制是一种将算法转换为容错分布式实现的技术。特别的技术可能会使重要的失败案例得不到解决。Lamport等人提出的原则方法确保了所有案件都得到安全处理。

The Paxos protocol was first published in 1989 and named after a fictional legislative consensus system used on the [Paxos](https://en.wikipedia.org/wiki/Paxi) island in Greece.[[4\]](https://en.wikipedia.org/wiki/Paxos_(computer_science)#cite_note-Lamport-4) It was later published as a journal article in 1998.[[5\]](https://en.wikipedia.org/wiki/Paxos_(computer_science)#cite_note-paxos-5)

The Paxos family of protocols includes a spectrum of trade-offs between the number of processors, number of message delays before learning the agreed value, the activity level of individual participants, number of messages sent, and types of failures. Although no deterministic fault-tolerant consensus protocol can guarantee progress in an asynchronous network (a result proved in a paper by [Fischer](https://en.wikipedia.org/wiki/Michael_J._Fischer), [Lynch](https://en.wikipedia.org/wiki/Nancy_Lynch) and [Paterson](https://en.wikipedia.org/wiki/Mike_Paterson)[[6\]](https://en.wikipedia.org/wiki/Paxos_(computer_science)#cite_note-flp-6)), Paxos guarantees safety (consistency), and the conditions that could prevent it from making progress are difficult to provoke.

> NOTE: 
>
> Paxos协议家族包括处理器数量、学习商定值之前的消息延迟数量、单个参与者的活动级别、发送的消息数量和故障类型之间的一系列权衡。
> 虽然没有确定的容错一致性协议可以保证异步网络中的进程(Fischer、Lynch和Paterson在一篇论文中证明了这一结果)，但是Paxos保证了安全性(一致性)，并且很难激发能够阻止它取得进展的条件。

Paxos is usually used where **durability** is required (for example, to replicate a file or a database), in which the amount of durable state could be large. The protocol attempts to make progress even during periods when some bounded number of replicas are unresponsive. There is also a mechanism to drop a permanently failed replica or to add a new replica.

> NOTE: 
>
> Paxos通常用于需要持久性的地方(例如，复制文件或数据库)，其中持久性状态的数量可能很大。即使在某些有限数量的副本没有响应时，协议也试图取得进展。还有一种机制可以删除永久失败的副本或添加新副本。