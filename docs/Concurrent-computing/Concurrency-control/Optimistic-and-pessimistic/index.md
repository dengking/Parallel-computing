# Optimistic and pessimistic

一、tradeoff

1、需要根据data contention的频率、量来选择到底是使用optimistic还是pessimistic

二、之前介绍的各种concurrency control technique，都是可以根据optimistic and pessimistic进行分类的



## 入门文章

### zhihu [【BAT面试题系列】面试官：你了解乐观锁和悲观锁吗？](https://zhuanlan.zhihu.com/p/74372722) 

参见 `zhihu-你了解乐观锁和悲观锁吗` 章节。



### stackoverflow [Optimistic vs. Pessimistic locking](https://stackoverflow.com/questions/129329/optimistic-vs-pessimistic-locking)

> NOTE: 各个回答是从DB的角度来谈的

## wikipedia [Optimistic concurrency control](https://en.wikipedia.org/wiki/Optimistic_concurrency_control) 

**Optimistic concurrency control** (**OCC**) is a [concurrency control](https://en.wikipedia.org/wiki/Concurrency_control) method applied to transactional systems such as [relational database management systems](https://en.wikipedia.org/wiki/Relational_database_management_systems) and [software transactional memory](https://en.wikipedia.org/wiki/Software_transactional_memory). OCC assumes that multiple transactions can frequently complete without interfering(干涉) with each other. While running, **transactions** use data resources without acquiring **locks** on those resources. Before committing, each transaction verifies that no other transaction has modified the data it has read. If the check reveals conflicting modifications, the committing transaction rolls back and can be restarted.[[1\]](https://en.wikipedia.org/wiki/Optimistic_concurrency_control#cite_note-1) Optimistic concurrency control was first proposed by [H. T. Kung](https://en.wikipedia.org/wiki/H._T._Kung) and John T. Robinson.[[2\]](https://en.wikipedia.org/wiki/Optimistic_concurrency_control#cite_note-KungRobinson1981-2)

> NOTE: 
>
> 1、通过上面描述的策略可以看出，显然相比于lock，optimistic concurrency control允许更大的concurrency。
>
> 2、以transaction的方式来理解optimistic concurrency control，这能够帮助我们的理解

OCC is generally used in environments with low [data contention](https://en.wikipedia.org/wiki/Block_contention)(数据竞争). When conflicts are rare, **transactions** can complete without the expense of managing locks and without having transactions wait for other transactions' locks to clear, leading to higher throughput than other **concurrency control** methods. However, if **contention** for data resources is frequent, the cost of repeatedly restarting transactions hurts performance significantly; it is commonly thought[*[who?](https://en.wikipedia.org/wiki/Wikipedia:Manual_of_Style/Words_to_watch#Unsupported_attributions)*] that other [concurrency control](https://en.wikipedia.org/wiki/Concurrency_control) methods have better performance under these conditions.[*[citation needed](https://en.wikipedia.org/wiki/Wikipedia:Citation_needed)*] However, **locking-based** ("pessimistic") methods also can deliver poor performance because locking can drastically limit effective concurrency even when deadlocks are avoided.

> NOTE: lock-based method是pessimistic的。

### Examples

> NOTE: 下来列举了一些我知道的

1、[Redis](https://en.wikipedia.org/wiki/Redis) provides OCC through WATCH command.[[16\]](https://en.wikipedia.org/wiki/Optimistic_concurrency_control#cite_note-16)



2、[MySQL](https://en.wikipedia.org/wiki/MySQL) implements OCC in Group Replication configuration.[*[citation needed](https://en.wikipedia.org/wiki/Wikipedia:Citation_needed)*]



## wikipedia [Transactional memory#Motivation](https://en.wikipedia.org/wiki/Transactional_memory#Motivation)

In concurrent programming, synchronization is required when parallel threads attempt to access a shared resource. Low level thread synchronization constructs such as locks are pessimistic and prohibit threads that are outside a [critical section](https://en.wikipedia.org/wiki/Critical_section) from making any changes. The process of applying and releasing locks often functions as additional overhead in workloads with little conflict among threads. Transactional memory provides [optimistic concurrency control](https://en.wikipedia.org/wiki/Optimistic_concurrency_control) by allowing threads to run in parallel with minimal interference.[[2\]](https://en.wikipedia.org/wiki/Transactional_memory#cite_note-:0-2) The goal of transactional memory systems is to transparently support regions of code marked as transactions by enforcing [atomicity](https://en.wikipedia.org/wiki/Atomicity_(database_systems)), [consistency](https://en.wikipedia.org/wiki/Consistency_(database_systems)) and [isolation](https://en.wikipedia.org/wiki/Isolation_(database_systems)).



## Optimistic concurrency control的实现方式

主要参考: 

1、zhuanlan [【BAT面试题系列】面试官：你了解乐观锁和悲观锁吗？](https://zhuanlan.zhihu.com/p/74372722)



下面是我的总结: 

两种实现方式都可以从transaction的角度来进行思考: 

CAS 通过 value comparison 来判断 "状态 是否发生了改变"、是否有其他transaction发生，如果没有改变、没有其他transaction，则commit；否则rollback。

MVCC 通过 version ID 来判断 "状态 是否发生了改变"、是否有其他transaction，如果没有改变、没有其他transaction，则commit；否则rollback。





## Optimistic and pessimistic synchronization in distributed computing

https://link.springer.com/chapter/10.1007/BFb0024159



## See also

### redis [Transactions](https://redis.io/topics/transactions)



### wikipedia [Optimistic replication](https://en.wikipedia.org/wiki/Optimistic_replication)



## TODO

https://en.wikipedia.org/wiki/Server_Message_Block#Opportunistic_locking



https://docs.jboss.org/jbossas/docs/Server_Configuration_Guide/4/html/TransactionJTA_Overview-Pessimistic_and_optimistic_locking.html

### dennyzhang [Explain: Pessimistic And Optimistic Locking](https://architect.dennyzhang.com/explain-locks/)