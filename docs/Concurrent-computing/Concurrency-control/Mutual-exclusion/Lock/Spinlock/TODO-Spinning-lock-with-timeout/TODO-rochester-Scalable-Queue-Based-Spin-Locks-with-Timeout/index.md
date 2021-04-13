# paper [Scalable Queue-Based Spin Locks with Timeout](https://www.cs.rochester.edu/u/scott/papers/2001_PPoPP_Timeout.pdf)

## ABSTRACT

**Queue-based spin locks** allow programs with busy-wait synchronization to scale to very large multiprocessors, without fear of starvation or performance-destroying contention. So called ***try locks***, traditionally based on non-scalable test-and-set locks, allow a process to abandon its attempt to acquire a lock after a given amount of time. The process can then pursue(继续) an alternative code path, or yield the processor to some other process.

> NOTE: 
>
> 1、**Queue-based spin locks** 具备 **scalability**、"fairness公平-starvation-free"
>
> 2、So called ***try locks***, traditionally based on test-and-set locks 不具备 **scalability**、不具备"fairness公平-starvation-free"，但是它可以实现bounded waiting(timeout)





We demonstrate that it is possible to obtain both **scalability** and bounded waiting, using variants of the queue-based locks of Craig, Landin, and Hagersten, and of Mellor-Crummey and Scott. A process that decides to stop waiting for one of these new locks can “link itself out of line” atomically. Single-processor experiments reveal performance penalties of 50–100% for the CLH and MCS try locks in comparison to their standard versions; this marginal cost decreases with larger numbers of processors.

> NOTE: 
>
> 1、这篇论文提出了综合  **scalability**、"fairness公平-starvation-free"、bounded waiting 的 spinning lock方案

### Comparison、实验结果

We have also compared our queue-based locks to a traditional test-and-test-and-set-lock with exponential backoff and timeout. At modest (non-zero) levels of contention, the queued locks sacrifice cache locality for fairness, resulting in a worst-case 3X performance penalty. At high levels of contention, however, they display a 1.5–2X performance advantage, with significantly more regular timings and significantly higher rates of acquisition prior to timeout.

> NOTE: 
>
> 1、tradeoff: sacrifice **cache locality** for **fairness**

## 1、INTRODUCTION

Spin locks are widely used for mutual exclusion on **shared memory multiprocessors**. Traditional test-and set-based spin locks are vulnerable(易受……的攻击) to **memory and interconnect contention**, and do not scale well to large machines. Queue-based spin locks avoid contention by arranging for every waiting process to spin on a separate, local flag in memory.

> NOTE:
>
> 1、关于 "**memory and interconnect contention**"，参见 csdn [聊聊高并发（五）理解缓存一致性协议以及对并发编程的影响](https://blog.csdn.net/iter_zc/article/details/40342695)

### Lock contention and scalability

Several researchers (ourselves among them) have conjectured(推测；猜想；假设) that any program that routinely encounters high levels of **lock contention** is unlikely to scale well to large machines(所谓large machine，是指具备更多core的). Conversely, in any program that scales well, the overhead of spin locks may not be a serious concern. The problem with this argument is that infrequent pathological(病态) events can be a serious problem to users. An improvement in worst case performance from, say, 2 to 10% efficiency may be enormously(极其的) valuable, even if typical efficiency is more like 80%. With the increasing popularity of medium-scale (20–100 processor) server-class machines, we believe that scalable spin locks will prove increasingly important. 

> NOTE: 
>
> 一、翻译如下:
>
> "这种观点的问题在于，偶尔发生的病态事件对用户来说可能是一个严重的问题。
> 在最坏的情况下，从2到10%的效率提高可能是非常有价值的，即使典型的效率更接近80%。
> 随着中等规模(20-100个处理器)服务器级机器的日益普及，我们相信可伸缩自旋锁将变得越来越重要。"
>
> 理解上面这段话，需要一点统计学的基础:
>
> 1、"typical efficiency is more like 80%"其实类似于统计学中的"众数"、"平均数"的概念
>
> 2、"worst case performance"其实就是一些极端值
>
> 

In a traditional test and set spin lock, processes are mutually anonymous. In a queue-based lock, they are not: each waiting process is linked to its predecessor and successor in the queue. In previous work [6], members of our group considered the possibility that a spinning process might be preempted, thereby unnecessarily delaying processes farther down the queue. In the current paper we consider the possibility that a spinning process may “become impatient” and wish to leave the queue before acquiring the lock. 

> NOTE: 
>
> 一、翻译如下:
>
> "在传统的test和set spin lock中，进程是相互匿名的。
> 在基于队列的锁中，它们不是:每个等待进程都链接到队列中的前任和后继进程。
> 在之前的工作[6]中，我们小组的成员考虑了纺丝过程可能是
> 抢占，因此不必要地延迟队列后面的进程。
> 在本文中，我们考虑了一种可能性，即一个正在旋转的进程可能会“失去耐心”，并希望在获得锁之前离开队列。"

### Timeout-capable spin locks、try-locks

> NOTE: 
>
> 1、这一节关于lock with timeout的重要性

Timeout-capable spin locks, sometimes referred to as “try-locks”, are important for several reasons:

1、A process in a soft real-time application may need to bound the time it spends waiting for a lock. If the timeout expires, the process can choose to announce an error or to pursue(继续) an alternative code path that does not require the lock.

2、Absent special OS support, a user-level process in a multiprogrammed environment may be preempted while holding a lock. Timeout allows other processes waiting for the lock to give up, yield the processor, and try again when rescheduled.

> NOTE: 
>
> 1、已经获得lock的process被挂起了，那么等待lock的process会因为timeout而放弃

3、In a parallel database system, timeout provides a viable strategy for deadlock recovery. A process that waits “too long” for a lock can assume that deadlock has occurred, abort the current transaction, and retry.

We are aware of commercially significant signal processing applications that use timeout for reason (1), and parallel database servers that use timeout for reasons (2) and (3). In the latter case, timeout may be the deciding factor in making spin locks usable in user-level code.



In contrast to the problem of preemption among spinning processes, timeout does not require interaction between the scheduler and the synchronization algorithm. It is complicated, however, by the fact that a timed-out process may attempt to acquire the lock again, possibly many times, before the point at which it would have acquired the lock if it had stayed in line. The possibility of multiple aborted acquisition attempts suggests that a timed-out process must remove itself from the queue entirely, leaving nothing behind; otherwise we would be unable to bound the space or time requirements of the algorithm.

We have developed so-called “try lock” (timeout capable) versions of our MCS queue-based lock [8] and of the CLH queue-based lock of Craig [2] and Landin and Hagersten [7]. After presenting additional background information in section 2, we describe our new locks in section 3. Both new
locks employ swap and compare and swap instructions, and can be implemented on any shared-memory machine, with or without cache coherence, that provides these operations or their equivalent. In section 4 we present performance results obtained on a 56-processor Sun Wildfire machine. We
conclude with a summary of recommendations in section 5.

## 2、BACKGROUND

### Spinning and blocking

> NOTE:  
>
> 1、这段关于"Spinning and blocking"的总结是非常好的

Programmers face several choices when synchronizing processes on a shared-memory multiprocessor. The most basic choice is between spinning (busy-waiting), in which processes actively poll for a desired condition, and blocking, in which processes yield the processor in expectation that they will be made runnable again when the desired condition holds. Spinning is the method of choice when the expected wait time is small, or when there is no other productive use for the processor.

### Spinning、busy-wait

The most basic busy-wait mechanism is a test-and-set lock, in which a process desiring entry to a critical section repeatedly attempts to change a “locked” flag from false to true, using an atomic hardware primitive. Unfortunately, test-and-set locks lead to increasing amounts of contention for memory and bus or interconnect bandwidth as the number of competing processors grows. This contention can be reduced somewhat by polling with ordinary read operations, rather than atomic test and set operations; polls are then satisfied from local caches during critical sections, with a burst of refill traffic whenever the lock is released. The cost of the burst can be further reduced using Ethernet-style exponential backoff [8]. Figure 1 shows code for a test-and-test-and-set (TATAS) lock with exponential backoff.

> NOTE: 
>
> 一、后半段的翻译如下:
>
> "通过普通读操作的轮询，而不是原子测试和设置操作，可以多少减少这种争用;然后，在关键部分，本地缓存满足轮询，当锁被释放时，会有爆发性的重新填充流量。使用以太网风格的指数回退[8]可以进一步降低突发的成本。图1显示了带有指数后退的测试-测试-设置(TATAS)锁的代码。"
>
> 二、read并不会导致cache coherence flood、high interconnect contention
>
> 三、"polls are then satisfied from local caches during critical sections"指的是:
>
> ```C++
> 			if (*L)
> 				continue; // spin with reads
> ```
>
> 此时thread执行的是read，因此只需要和自己的cache交互即可，这就是"polls are then satisfied from local caches during critical sections"的含义
>
> 四、"with a burst of refill traffic whenever the lock is released"指的是: 当lock被release，则所有其他的process会尝试进行写入，则这就导致了"burst of refill"，环节它的方法是"Ethernet-style exponential backoff"

```C++
typedef unsigned long bool;
typedef volatile bool tatas_lock;
void tatas_acquire(tatas_lock *L)
{
	if (tas(L))
	{
		int b = BACKOFF_BASE;
		do
		{
			for (i = b; i; i--)
				; // delay
			b = min(b * BACKOFF_FACTOR, BACKOFF_CAP);
			if (*L)
				continue; // spin with reads
		} while (tas(L));
	}
}
void tatas_release(tatas_lock *L)
{
	*L = 0;
}

```

Figure 1: Test-and-test and set (TATAS) lock with exponential backoff. Parameters `BACKOFF BASE`, `BACKOFF FACTOR`, and `BACKOFF CAP` must be tuned by
trial and error for each individual machine architecture.



### 2.1 Queue-based locks

Even with exponential backoff, test-and-test and set locks still induce significant contention, leading to irregular timings and compromised memory performance on large machines. 

> NOTE: 
>
> 1、关于"irregular timings and compromised memory performance on large machines"的介绍，参见csdn [聊聊高并发（六）实现几种自旋锁（一）](https://blog.csdn.net/ITer_ZC/article/details/40373881)
>
> > 缺点是在锁高争用的情况下，线程很难一次就获取锁，CAS的操作会大大增加。
>
> 

Queue-based spin locks eliminate these problems by arranging for every competing process to spin on a different memory location. To ensure scalability, each location must be local to the spinning process, either by virtue of hardware cache coherence or as a result of explicit local allocation on a non-cache-coherent machine.

> NOTE: 
>
> 一、为了降低contention，r让每个competing process spin on a different memory location，为了保证scalability，每个location必须local to the spinning process，那如何实现呢?
>
> 1、对于支持cache coherence的hardware，这是天然满足的
>
> 2、对于non-cache-coherent machine，通过 explicit local allocation 来实现
>
> 总的来说，思路是: 通过增加locality来降低contention，来提高 scalability
>
> 



Several researchers, working independently and roughly concurrently, developed queue-based spin locks in the late 1980s.

Anderson [1] and Graunke and Thakkar [3] embed their queues in per-lock arrays, requiring space per lock proportional to the maximum number of processes that may compete for access concurrently. Anderson’s lock uses atomic `fetch and increment` instructions to assign slots in the array to waiting processes; Graunke and Thakkar’s lock uses `swap` instead. 

#### MCS lock

Our MCS lock, co-designed with John Mellortypedef Crummey [8], employs a linked list with pointers from each process to its successor; it relies on `swap and compare` and `swap`, and has the advantage of requiring total space linear in the number of locks and the number of competing processes. (The lock can be re-written to use only `swap`, but at the expense of FIFO ordering: in the swap-only version of mcs release there is a timing window that may allow
newly arriving processes to jump ahead of processes already in the queue.) The MCS lock is naturally suited to local-only spinning on both cache-coherent and non-cache coherent machines. The Anderson and Graunke/Thakkar locks must be modified to employ an extra level of indirection
on non-cache-coherent machines.

#### CLH lock

The CLH lock, developed about three years later by Craig [2] and, independently, Landin and Hagersten [7], also employs a linked list, but with pointers from each process to its predecessor. The CLH lock relies on atomic swap, and may outperform the MCS lock on cache-coherent machines. Like the Anderson and Graunke/Thakkar locks, it requires an extra level of indirection to avoid spinning on remote locations on a non-cache-coherent machine [2]. Code for the MCS and CLH locks appears in Figures 2, 3, and 4.

### 2.2 Atomic primitives



## pseudocode [Scalable Queue-Based Spin Locks with Timeout](https://www.cs.rochester.edu/research/synchronization/pseudocode/timeout.html)

