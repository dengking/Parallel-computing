# CAP VS ACID

## Consistency

CAP  和 ACID 中都有"consistency"，但是两者的含义是完全不同的:

### ACID 的 "consistency"

在 cnblogs [数据库事务的四大特性以及事务的隔离级别](https://www.cnblogs.com/fjdingsd/p/5273008.html) 中，结合例子对"ACID 的 "consistency""进行了非常好的描述:

> 一致性是指事务必须使数据库从一个一致性状态变换到另一个一致性状态，也就是说一个事务执行之前和执行之后都必须处于一致性状态。
>
> 拿转账来说，假设用户A和用户B两者的钱加起来一共是5000，那么不管A和B之间如何转账，转几次账，事务结束后两个用户的钱相加起来应该还得是5000，这就是事务的一致性。



### CAP 的 "consistency"

在 wikipedia [CAP theorem](https://en.wikipedia.org/wiki/CAP_theorem) 中的解释如下:

> 1、*Consistency*: Every read receives the most recent write or an error
>
> > NOTE: 
> >
> > 即不会丢失write，如果丢失了write，则可能导致inconsistency，其实这个consistency的定义非常类似于[6.3.1. Strict Consistency](https://www.e-reading.club/chapter.php/143358/217/Tanenbaum_-_Distributed_operating_systems.html) 
>
> 