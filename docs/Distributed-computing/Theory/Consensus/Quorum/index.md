# Quorum 

## Multiple model



一、通过**投票**来达成共识。

二、既然有multiple entity，那么由它们来进行vote从而进行决策是自然而然的想法了。

少数服从多数



## wikipedia [Quorum (distributed computing)](https://en.wikipedia.org/wiki/Quorum_(distributed_computing))

A **quorum** is the minimum number of **votes** that a **distributed transaction** has to obtain in order to be allowed to perform an operation in a [distributed system](https://en.wikipedia.org/wiki/Distributed_system). A **quorum**-based technique is implemented to enforce **consistent operation** in a distributed system.



### Quorum-based techniques in distributed database systems

Quorum-based voting can be used as a [replica](https://en.wikipedia.org/wiki/Replication_(computer_science)#Database_replication) control method,[[1\]](https://en.wikipedia.org/wiki/Quorum_(distributed_computing)#cite_note-ozsu-1) as well as a commit method to ensure [transaction](https://en.wikipedia.org/wiki/Database_transaction) [atomicity](https://en.wikipedia.org/wiki/Atomicity_(database_systems)) in the presence of [network partitioning](https://en.wikipedia.org/wiki/Network_partitioning).[[1\]](https://en.wikipedia.org/wiki/Quorum_(distributed_computing)#cite_note-ozsu-1)

#### Quorum-based voting in commit protocols

In a distributed database system, a transaction could be executing its operations at multiple sites. Since **atomicity** requires every **distributed transaction** to be **atomic**, the transaction must have the same fate ([commit](https://en.wikipedia.org/wiki/Commit_(data_management)) or [abort](https://en.wikipedia.org/wiki/Rollback_(data_management))) at every site. In case of network partitioning, sites are partitioned and the partitions may not be able to communicate with each other. This is where a **quorum-based** technique comes in. **The fundamental idea is that a transaction is executed if the majority of sites vote to execute it**.

Every site in the system is assigned a vote $V_i$. Let us assume that the total number of votes in the system is $V$ and the abort and commit quorums are $V_a$ and $V_c$, respectively. Then the following rules must be obeyed in the implementation of the commit protocol:

1. $V_a + V_c > V$, where $0 < V_c, V_a$ $ \leq $ $V$.
2. Before a transaction commits, it must obtain a commit quorum $V_c$.
   The total of at least one site that is prepared to commit and zero or more sites waiting $ \geq $ $V_c$.[[2\]](https://en.wikipedia.org/wiki/Quorum_(distributed_computing)#cite_note-2)
3. Before a transaction aborts, it must obtain an abort quorum $V_a$
   The total of zero or more sites that are prepared to abort or any sites waiting $ \geq $ Va.

The first rule ensures that a transaction cannot be committed and aborted at the same time. The next two rules indicate the votes that a transaction has to obtain before it can terminate one way or the other.



#### Quorum-based voting for replica control

In replicated databases, a data object has copies present at several sites. To ensure [serializability](https://en.wikipedia.org/wiki/Serializability), no two transactions should be allowed to read or write a data item concurrently. In case of replicated databases, a quorum-based replica control protocol can be used to ensure that no two copies of a data item are read or written by two transactions concurrently.

The quorum-based voting for replica control is due to [Gifford, 1979].[[3\]](https://en.wikipedia.org/wiki/Quorum_(distributed_computing)#cite_note-3) Each copy of a replicated data item is assigned a vote. Each operation then has to obtain a *read quorum* (Vr) or a *write quorum* (Vw) to read or write a data item, respectively. If a given data item has a total of V votes, the quorums have to obey the following rules:

1. Vr + Vw > V
2. Vw > V/2

The first rule ensures that a data item is not read and written by two transactions concurrently. Additionally, it ensures that a read quorum contains at least one site with the newest version of the data item. The second rule ensures that two write operations from two transactions cannot occur concurrently on the same data item. The two rules ensure that one-copy serializability is maintained.



### See also

- [CAP theorem](https://en.wikipedia.org/wiki/CAP_theorem)
- [Database transaction](https://en.wikipedia.org/wiki/Database_transaction)
- [Replication (computer science)](https://en.wikipedia.org/wiki/Replication_(computer_science))
- [Atomicity (database systems)](https://en.wikipedia.org/wiki/Atomicity_(database_systems))



## Case study

### Case: ZooKeeper 

在 [ZooKeeper Getting Started Guide](https://zookeeper.apache.org/doc/r3.6.2/zookeeperStarted.html) 中的"Running Replicated ZooKeeper"章节进行了介绍:

> But in production, you should run ZooKeeper in replicated mode. A replicated group of servers in the same application is called a ***quorum***, and in replicated mode, all servers in the **quorum** have copies of the same configuration file.