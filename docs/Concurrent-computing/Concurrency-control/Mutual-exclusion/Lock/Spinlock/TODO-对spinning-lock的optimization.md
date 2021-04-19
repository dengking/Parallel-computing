# 对spinning lock的optimization

1、我在思考"spinning lock with timeout"的时候，通过Google "spinning lock with timeout"发现的这个topic

2、后来发现了很多的资源，我觉得应该从逐步优化的角度来组织内容

## 综述的、梳理性的、导读性的教程

### cs.tau [Multiprocessor Programming](http://www.cs.tau.ac.il/~shanir/multiprocessor-synch-2003/) # **Lecture 7:** Spin Locks and Contention Management (last updated Sunday, May 11, 2003**)**

其中内容比较不错，梳理了:

TASLock ->TTASLock->TTASLock with exponential delay->Queue-based Locks:

a、An Array Based Queue Lock(A-lock)

b、The CLH queue lock(Mellor-Crummey and Scott provided the popular list based
MCS Queue lock)

c、The MCS queue lock(CLH lock of Craig and Landin and Hagersten)

> 7.10 Chapter Notes
>
> The TTASLock is due to Kruskal and Rudolph and Snir. Exponential backoff is a well known technique used in Ethernet routing. Anderson was one of the first to empirically study contention in shared memory multiprocessors, introducing the A-lock, the first queue lock which was array based. Many of the graphs in this document are idealizations of his actual empirical results on the Sequent Symmetry. Mellor-Crummey and Scott provided the popular list based MCS Queue lock, later improved upon by the CLH lock of Craig and Landin and Hagersten. The NUMA machine graphs we show are idealizations of the experiments of Mellor-Crummey and Scott on the BBN butterfly machine. The
> modern machine graphs are due to Scott.

总结地比较好。

并且其中还提供了C++ source code。

### Expert-iter_zc

参见相应章节。

### rochester [Algorithms for Scalable Synchronization on Shared-Memory Multiprocessors](https://www.cs.rochester.edu/research/synchronization/pseudocode/ss.html)

1、梳理得非常好

2、参见相关章节



### disco.ethz [Locking Part 2, Chapter 11](https://disco.ethz.ch/courses/hs15/distsys/lecture/chapter11.pdf)



## Optimization: TASLock ->TTASLock

1、spinning lock的劣势、对spinning lock的第一次优化

2、TASLock ->TTASLock

参考内容:

1、csdn iter_zc [聊聊高并发（六）实现几种自旋锁（一）](https://blog.csdn.net/iter_zc/article/details/40373881)

### TAS vs TTAS

stackoverflow [Better explanations of TAS Vs. TTAS in the context of “The Art Of Multiprocessor Programming” Herlihy & Shavits](https://stackoverflow.com/questions/26322991/better-explanations-of-tas-vs-ttas-in-the-context-of-the-art-of-multiprocessor)



## Optimization: TTASLock with exponential delay



## Optimization: Queue-based Locks

wikipedia [Array_Based_Queuing_Locks](https://en.wikipedia.org/wiki/Array_Based_Queuing_Locks)

## Optimization: Queue-based Locks with timeout

### rochester [Scalable Queue-Based Spin Locks with Timeout](https://www.cs.rochester.edu/research/synchronization/pseudocode/timeout.html)

这是非常好的资源:

一、它总结了前人的工作

二、它添加上了timeout，这又是一次优化，并且还给出了代码

1、CLH queue-based spin lock with timeout

2、MCS queue-based spin lock with timeout



2、https://patents.google.com/patent/US6965961B1/en

谷歌的专利

## Implementation

### tick lock

https://github.com/search?l=C&q=ticket+lock&type=Repositories



### [jason741852](https://github.com/jason741852)/**[c_locks](https://github.com/jason741852/c_locks)**

Implementations of spin lock, exponential backoff lock and queue lock

### [luapvu](https://github.com/luapvu)/**[lock-algorithms](https://github.com/luapvu/lock-algorithms)**

其中有benchmark。

### [geidav](https://github.com/geidav)/**[spinlocks-bench](https://github.com/geidav/spinlocks-bench)**



### [shines77](https://github.com/shines77)/**[RingQueue](https://github.com/shines77/RingQueue)**

## 0xax.gitbooks [Synchronization primitives in the Linux kernel. Part 2.](https://0xax.gitbooks.io/linux-insides/content/SyncPrim/linux-sync-2.html)

### Queued Spinlocks

### Introduction to queued spinlocks