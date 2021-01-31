# Non-blocking VS blocking





## Lock-free VS lock-based

在 stackoverflow [What is lock-free multithreaded programming?](https://stackoverflow.com/questions/14011849/what-is-lock-free-multithreaded-programming) # [A](https://stackoverflow.com/a/14011948) 中，对这个话题进行了探讨: 

It is worth noting that in general lock-free programming trades(交换) throughput and mean latency throughput for **predictable latency**. That is, a lock-free program will usually get less done than a corresponding locking program if there is not too much contention (since atomic operations are slow and affect a lot of the rest of the system), but it guarantees to never produce unpredictably large latencies.

> NOTE: 上面这段话的表明意思是: "值得注意的是，一般来说，无锁编程用吞吐量和平均延迟吞吐量交换可预测延迟。也就是说，如果没有太多的争用(因为原子操作很慢，并且会影响系统的其他部分)，那么无锁程序通常比相应的锁定程序完成的工作要少，但是它保证不会产生不可预测的大延迟。"
>
> 1、lock-free programming 以 throughout 和 mean latency throughout 来换取 predictable latency。
>
> 2、lock-free 不一定比 lock-base 要慢: 当没有太多的thread contention的时候，lock-based的性能可能更佳。



## Cooperative VS contention 

lock显然是contention 

lock-free是cooperative 

下面是draft: 

### draft

preshing [Acquire and Release Semantics](https://preshing.com/20120913/acquire-and-release-semantics/)

Generally speaking, in [lock-free programming](http://preshing.com/20120612/an-introduction-to-lock-free-programming), there are two ways in which threads can manipulate shared memory: They can compete with each other for a resource, or they can pass information co-operatively from one thread to another. Acquire and release semantics are crucial for the latter: reliable passing of information between threads. 