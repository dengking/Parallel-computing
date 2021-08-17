# CAP



## wikipedia [CAP theorem](https://en.wikipedia.org/wiki/CAP_theorem) 

In [theoretical computer science](https://en.wikipedia.org/wiki/Theoretical_computer_science), the **CAP theorem**, also named **Brewer's theorem** after computer scientist [Eric Brewer](https://en.wikipedia.org/wiki/Eric_Brewer_(scientist)), states that it is impossible for a [distributed data store](https://en.wikipedia.org/wiki/Distributed_data_store) to simultaneously provide more than two out of the following three guarantees:[[1\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-Gilbert_Lynch-1)[[2\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-2)[[3\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-3)

1、*Consistency*: Every read receives the most recent write or an error

> NOTE: 
>
> 即不会丢失write，如果丢失了write，则可能导致inconsistency，其实这个consistency的定义非常类似于[6.3.1. Strict Consistency](https://www.e-reading.club/chapter.php/143358/217/Tanenbaum_-_Distributed_operating_systems.html) 

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

> NOTE: 
>
> 一、关于CAP、ACID、BASE，参见如下文章:
>
> 1、csdn [用太极拳讲分布式理论，真舒服！](https://blog.csdn.net/jackson0714/article/details/112002319?spm=1001.2014.3001.5501)
>
> 

The [PACELC theorem](https://en.wikipedia.org/wiki/PACELC_theorem) builds on CAP by stating that even in the absence of partitioning, another trade-off between **latency** and **consistency** occurs.



### See also

- [PACELC theorem](https://en.wikipedia.org/wiki/PACELC_theorem)
- [Consistency model](https://en.wikipedia.org/wiki/Consistency_model)
- [Fallacies of distributed computing](https://en.wikipedia.org/wiki/Fallacies_of_distributed_computing)
- [Paxos (computer science)](https://en.wikipedia.org/wiki/Paxos_(computer_science))
- [Project management triangle](https://en.wikipedia.org/wiki/Project_management_triangle)
- [Raft (computer science)](https://en.wikipedia.org/wiki/Raft_(computer_science))
- [Trilemma](https://en.wikipedia.org/wiki/Trilemma)



## stackoverflow [CAP theorem - Availability and Partition Tolerance](https://stackoverflow.com/questions/12346326/cap-theorem-availability-and-partition-tolerance)

While I try to understand the "Availability" (A) and "Partition tolerance" (P) in CAP, I found it difficult to understand the explanations from various articles.

I get a feeling that A and P can go together (I know this is not the case, and that's why I fail to understand!).

Explaining in simple terms, what are A and P and the difference between them?

***COMMENTS*** : 

here is an article which explains CAP in plain english [ksat.me/a-plain-english-introduction-to-cap-theorem](http://ksat.me/a-plain-english-introduction-to-cap-theorem/) – [Tushar Saha](https://stackoverflow.com/users/4557946/tushar-saha) [Jun 25 at 6:14](https://stackoverflow.com/questions/12346326/cap-theorem-availability-and-partition-tolerance#comment100053761_12346326)



### [A](https://stackoverflow.com/a/12347673)

Consistency means that data is the same across the cluster, so you can read or write from/to any node and get the same data.

Availability means the ability to access the cluster even if a node in the cluster goes down.

Partition tolerance means that the cluster continues to function even if there is a "partition" (communication break) between two nodes (both nodes are up, but can't communicate).

In order to get both availability and partition tolerance, you have to give up consistency. Consider if you have two nodes, X and Y, in a master-master setup. Now, there is a break between network communication between X and Y, so they can't sync updates. At this point you can either:

A) Allow the nodes to get out of sync (giving up consistency), or

B) Consider the cluster to be "down" (giving up availability)

All the combinations available are:

1、**CA** - data is consistent between all nodes - as long as all nodes are online - and you can read/write from any node and be sure that the data is the same, but if you ever develop a partition between nodes, the data will be out of sync (and won't re-sync once the partition is resolved).

> NOTE: 
>
> 实际中，并不存在

2、**CP** - data is consistent between all nodes, and maintains partition tolerance (preventing data desync) by becoming unavailable when a node goes down.

3、**AP** - nodes remain online even if they can't communicate with each other and will resync data once the partition is resolved, but you aren't guaranteed that all nodes will have the same data (either during or after the partition)

You should note that [CA systems don't practically exist](http://codahale.com/you-cant-sacrifice-partition-tolerance/) (even if some systems claim to be so).



## CAP的直观理解

"鱼和熊掌不可兼得":

1) 之前在开发一个application的时候，涉及到在performance和reliability中进行抉择: 

如果实时(同步)地将记录写入到文件，则performance较差，reliability较好，这就是牺牲performance来获取reliability。

如果非实时(异步)地将记录写入到文件，则performance较好，reliability较差，这就是牺牲reliability来获取performance。



