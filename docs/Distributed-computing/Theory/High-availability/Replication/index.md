# Replication 

有备无患。

## wikipedia [Replication (computing)](https://en.wikipedia.org/wiki/Replication_(computing))

**Replication** in [computing](https://en.wikipedia.org/wiki/Computing) involves sharing information so as to ensure **consistency** between redundant resources, such as [software](https://en.wikipedia.org/wiki/Software) or [hardware](https://en.wikipedia.org/wiki/Computer_hardware) components, to improve reliability, [fault-tolerance](https://en.wikipedia.org/wiki/Fault-tolerance), or accessibility.



## wikipedia [Redundancy (engineering)](https://en.wikipedia.org/wiki/Redundancy_(engineering))



## 两种方式

### master slave

这种方式比较广泛的，比如: 

1、Redis sentinel HA就是采用的这种方式

### 成组

其实这也是一种replication，在下面的文章中，介绍了这种用法:

1、Redis doc [Partitioning: how to split data among multiple Redis instances](https://redis.io/topics/partitioning)

> It is *not* a **single point of failure** since you can start multiple proxies and instruct your clients to connect to the first that accepts the connection.

显然，是通过成组来避免**single point of failure** 。



