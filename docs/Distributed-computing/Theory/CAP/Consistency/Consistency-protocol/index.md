# 一致性协议



## csdn [分布式一致性协议](https://blog.csdn.net/demon7552003/article/details/86657767)

为了解决分布式系统的一致性问题，在长期的研究探索过程中，业内涌现出了一大批经典的一致性协议和算法，其中比较著名的有二阶段提交协议（2PC），三阶段提交协议（3PC）和 Paxos 算法。

Google 2009年 在 [Transaction Across DataCenter](https://snarfed.org/transactions_across_datacenters_io.html) 的分享中，对一致性协议在业内的实践做了一简单的总结，如下图所示，这是 CAP 理论在工业界应用的实践经验。

[![CAP 理论在工业界的实践](http://matt33.com/images/distribute/cap-sumarry.png)](http://matt33.com/images/distribute/cap-sumarry.png)

其中，第一行表头代表了分布式系统中通用的一致性方案，包括冷备、Master/Slave、Master/Master、两阶段提交以及基于 Paxos 算法的解决方案，第一列表头代表了分布式系统大家所关心的各项指标，包括一致性、事务支持程度、数据延迟、系统吞吐量、数据丢失可能性、故障自动恢复方式。