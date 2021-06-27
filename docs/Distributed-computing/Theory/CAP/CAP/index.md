# CAP



## wikipedia [CAP theorem](https://en.wikipedia.org/wiki/CAP_theorem)

In [theoretical computer science](https://en.wikipedia.org/wiki/Theoretical_computer_science), the **CAP theorem**, also named **Brewer's theorem** after computer scientist [Eric Brewer](https://en.wikipedia.org/wiki/Eric_Brewer_(scientist)), states that it is impossible for a [distributed data store](https://en.wikipedia.org/wiki/Distributed_data_store) to simultaneously provide more than two out of the following three guarantees:[[1\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-Gilbert_Lynch-1)[[2\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-2)[[3\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-3)

1、*Consistency*: Every read receives the most recent write or an error

> NOTE: 即不会丢失write，如果丢失了write，则可能导致inconsistency，其实这个consistency的定义非常类似于[6.3.1. Strict Consistency](https://www.e-reading.club/chapter.php/143358/217/Tanenbaum_-_Distributed_operating_systems.html) 

2、*[Availability](https://en.wikipedia.org/wiki/Availability)*: Every request receives a (non-error) response – without the guarantee that it contains the most recent write

3、*[Partition tolerance](https://en.wikipedia.org/wiki/Network_partitioning)*: The system continues to operate despite an arbitrary number of messages being dropped (or delayed) by the network between nodes

When a [network partition failure](https://en.wikipedia.org/wiki/Network_partition) happens should we decide to

1、Cancel the operation and thus decrease the availability but ensure consistency.

> NOTE: 
>
> CP

2、Proceed with the operation and thus provide availability but risk inconsistency.

> NOTE:
>
> AP

In particular, the CAP theorem implies that in the presence of a **network partition**, one has to choose between **consistency** and **availability**. Note that **consistency** as defined in the **CAP theorem** is quite different from the consistency guaranteed in [ACID](https://en.wikipedia.org/wiki/ACID) [database transactions](https://en.wikipedia.org/wiki/Database_transaction)[[4\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-4).

### Explanation

No distributed system is safe from network failures, thus [network partitioning](https://en.wikipedia.org/wiki/Network_partitioning) generally has to be tolerated（容错的）. In the presence of a partition（分裂）, one is then left with two options: consistency or [availability](https://en.wikipedia.org/wiki/Availability). 

1、When choosing consistency over availability, the system will return an error or a time-out if particular information cannot be guaranteed to be up to date due to network partitioning. 

2、When choosing availability over consistency, the system will always process the query and try to return the most recent available version of the information, even if it cannot guarantee it is up to date due to **network partitioning**.

In the absence of **network failure** – that is, when the **distributed system** is running normally – both availability and consistency can be satisfied.

CAP is frequently misunderstood as if one has to choose to abandon one of the three guarantees at all times. In fact, the choice is really between consistency and availability only when a **network partition** or **failure** happens; at all other times, no trade-off has to be made.[[5\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-5)[[6\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-6)

Database systems designed with traditional [ACID](https://en.wikipedia.org/wiki/ACID) guarantees in mind such as [RDBMS](https://en.wikipedia.org/wiki/Relational_database_management_system) choose **consistency** over **availability**, whereas systems designed around the [BASE](https://en.wikipedia.org/wiki/Eventual_consistency) philosophy, common in the [NoSQL](https://en.wikipedia.org/wiki/NoSQL) movement for example, choose availability over consistency.[[7\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-Brewer2012-7)

The [PACELC theorem](https://en.wikipedia.org/wiki/PACELC_theorem) builds on CAP by stating that even in the absence of partitioning, another trade-off between **latency** and **consistency** occurs.

### History

According to [University of California, Berkeley](https://en.wikipedia.org/wiki/University_of_California,_Berkeley) computer scientist [Eric Brewer](https://en.wikipedia.org/wiki/Eric_Brewer_(scientist)), the theorem first appeared in autumn 1998.[[7\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-Brewer2012-7) It was published as the CAP principle in 1999[[8\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-Brewer1999-8) and presented as a [conjecture](https://en.wikipedia.org/wiki/Conjecture) by Brewer at the 2000 [Symposium on Principles of Distributed Computing](https://en.wikipedia.org/wiki/Symposium_on_Principles_of_Distributed_Computing) (PODC).[[9\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-Brewer2000-9) In 2002, [Seth Gilbert](https://en.wikipedia.org/w/index.php?title=Seth_Gilbert&action=edit&redlink=1) and [Nancy Lynch](https://en.wikipedia.org/wiki/Nancy_Lynch) of [MIT](https://en.wikipedia.org/wiki/MIT) published a formal proof of Brewer's conjecture（推想）, rendering it a [theorem](https://en.wikipedia.org/wiki/Theorem).[[1\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-Gilbert_Lynch-1)

In 2012, Brewer clarified some of his positions, including why the often-used "two out of three" concept can be misleading or misapplied, and the different definition of consistency used in CAP relative to the one used in [ACID](https://en.wikipedia.org/wiki/ACID).[[7\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-Brewer2012-7)

A similar theorem stating the trade-off between consistency and availability in distributed systems was published by Birman and Friedman in 1996.[[10\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-10) The result of Birman and Friedman restricted this lower bound to non-commuting operations.

### See also

- [PACELC theorem](https://en.wikipedia.org/wiki/PACELC_theorem)
- [Consistency model](https://en.wikipedia.org/wiki/Consistency_model)
- [Fallacies of distributed computing](https://en.wikipedia.org/wiki/Fallacies_of_distributed_computing)
- [Paxos (computer science)](https://en.wikipedia.org/wiki/Paxos_(computer_science))
- [Project management triangle](https://en.wikipedia.org/wiki/Project_management_triangle)
- [Raft (computer science)](https://en.wikipedia.org/wiki/Raft_(computer_science))
- [Trilemma](https://en.wikipedia.org/wiki/Trilemma)



## CAP的直观理解

"鱼和熊掌不可兼得":

1) 之前在开发一个application的时候，涉及到在performance和reliability中进行抉择: 

如果实时(同步)地将记录写入到文件，则performance较差，reliability较好，这就是牺牲performance来获取reliability。

如果非实时(异步)地将记录写入到文件，则performance较好，reliability较差，这就是牺牲reliability来获取performance。



