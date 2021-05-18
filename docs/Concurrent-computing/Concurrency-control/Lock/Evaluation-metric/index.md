# Evaluation metrics for lock

锁的评估指标。



## wikipedia [Test-and-set](https://en.wikipedia.org/wiki/Test-and-set) # [Performance evaluation of test-and-set locks](https://en.wikipedia.org/wiki/Test-and-set#Performance_evaluation_of_test-and-set_locks)

The four major evaluation metrics for locks in general are :

1、uncontended lock-acquisition latency

> NOTE: 
>
> 1、在没有contention的情况下，获得所的延时

bus traffic, 

> NOTE: 
>
> 一、最最典型的是TAS spin lock->TTAS spin lock->MCS Lock、CLH Lock 
>
> 它们的bus traffic都在降低
>
> 二、关于bus traffic，参见
>
> 1、csdn [聊聊高并发（五）理解缓存一致性协议以及对并发编程的影响](https://blog.csdn.net/iter_zc/article/details/40342695)
>
> 三、bus traffic是影响"scalability"的重要因素

fairness, and 

> NOTE: 
>
> 1、公平性，一般使用FIFO ordering
>
> "tag-fairness-公平性-starvation-free-fair to lock acquisition"

storage.[[7\]](https://en.wikipedia.org/wiki/Test-and-set#cite_note-Solihin-7)

> NOTE:
>
> 

## 补充的评判指标

一、user space VS kernel space



二、block VS non blocking



## Scalability

"scalability"是在spin lock的优化论文中提的非常多的一个内容，它和如下内容有关:

1、bus traffic