# NRW quorum consistency

## 参考文章

### 1、zhihu [关于分布式系统中的NWR quorum consistency](https://zhuanlan.zhihu.com/p/138806042)

>  NOTE: 
>
> 一、我是在这篇文章中，首次发现NRW quorum consistency的
>
> 二、其实这篇文章，汇总了处理replication lag的两种方法:
>
> 1、NRW 
>
> 2、read-your-own-writes

在这片文章里面[https://www.allthingsdistributed.com/2008/12/eventually_consistent.html](https://link.zhihu.com/?target=https%3A//www.allthingsdistributed.com/2008/12/eventually_consistent.html)

**作者Werner Vogels给了这个NWR的定义**

> N = the number of nodes that store replicas of the data
>
> W = the number of replicas that need to acknowledge the receipt of the update before the update completes
>
> R = the number of replicas that are contacted when a data object is accessed through a read operation

N 存储了数据副本的节点

W 在更新完成之前需要更新的副本数量

R 在读操作中需要联系的副本数量

并且 给出了结论

> If W+R > N, then the write set and the read set always overlap and one can guarantee strong consistency.

如果W+R >N ， 那么写的这个节点集合和读的节点集合肯定会重合（小学时候学习的抽屉原理），肯定能保证强一致。



对上面这个场景稍稍改一下，变成一个我们司空见惯的一个场景。这个DB NODE A是一个master节点，DB NODE B是一个slave节点。一个通用的做法是我们只write master，通过master-slave之间的异步同步机制将数据copy到slave上，然后我们再read slave，根据我们上面的分析来看确实是不能保证read的数据一定是最新的，很可能的原因就是master节点在同步这个data数据到slave的时候有了时延，这个问题在学术界中也是有研究的，叫Problems with Replication Lag

为了保证强一致，保证我们读的数据是最新的，那我们写master DB之后再读 master DB不就完了吗？ 对，没错，是可以这么干

write的时候写master DB ， read的时候读 master DB ，这个时候是可以保证强一致的。其实这个是有一个理论依据，叫read-your-own-writes.就是我只读我写过的那个节点，这个也是解决Replication Lag的一个方法。

> NOTE: 
>
> 使用 read-your-own-writes 来解决 Replication Lag问题

### 2、zhihu [NWR模型下的一致性问题](https://zhuanlan.zhihu.com/p/99358806)

> NOTE: 
>
> 没有深入阅读，其中结合Dynamo来进行分析的



### 3、[太上老君的炼丹炉之分布式 Quorum NWR](https://blog.csdn.net/jackson0714/article/details/114338016?spm=1001.2014.3001.5501)

> NOTE: 
>
> 这篇文章，基本上讲清楚了 Quorum NWR，已经收录了



### 4、geeksforgeeks [Quorum Consistency in Cassandra](https://www.geeksforgeeks.org/quorum-consistency-in-cassandra/)

## 抽屉原理

可以使用抽屉原理来验证NRW quorum的正确性。

