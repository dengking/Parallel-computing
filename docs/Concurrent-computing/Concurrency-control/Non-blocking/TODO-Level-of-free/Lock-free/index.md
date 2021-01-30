# Lock free

## What is lock free?

### preshing [An Introduction to Lock-Free Programming](https://preshing.com/20120612/an-introduction-to-lock-free-programming/)

这篇文章中，也对lock-free进行了介绍，参见相关章节；

### 1024cores [Home](http://www.1024cores.net/home)‎ > ‎[Lockfree Algorithms](http://www.1024cores.net/home/lock-free-algorithms)‎ > ‎[Introduction](http://www.1024cores.net/home/lock-free-algorithms/introduction)

这篇文章中，也对lock-free进行了介绍，参见相关章节；

### stackoverflow [What is lock-free multithreaded programming?](https://stackoverflow.com/questions/14011849/what-is-lock-free-multithreaded-programming)

I have seen people/articles/SO posts who say they have designed their own "lock-free" container for multithreaded usage. Assuming they haven't used a performance-hitting modulus trick (i.e. each thread can only insert based upon some modulo) how can data structures be multi-threaded but also lock-free???

This question is intended towards C and C++.

[A](https://stackoverflow.com/a/14011948)

> NOTE: 结合具体的例子，是比较容易理解的

The key in lock-free programming is to use hardware-intrinsic *atomic* operations.

As a matter of fact, even locks themselves must use those atomic operations!

But the difference between locked and lock-free programming is that a lock-free program can never be stalled(停滞) entirely by any single thread. By contrast, if in a locking program one thread acquires a lock and then gets suspended indefinitely(无期限的), the entire program is blocked and cannot make progress. By contrast, a lock-free program can make progress even if individual threads are suspended indefinitely.

Here's a simple example: A concurrent counter increment. We present two versions which are both "thread-safe", i.e. which can be called multiple times concurrently. First the locked version:

```c++
int counter = 0;
std::mutex counter_mutex;

void increment_with_lock()
{
    std::lock_guard<std::mutex> _(counter_mutex);
    ++counter;
}
```

Now the lock-free version:

```c++
std::atomic<int> counter(0);

void increment_lockfree()
{
    ++counter;
}
```

Now imagine hundreds of thread all call the `increment_*` function concurrently. In the locked version, *no thread can make progress* until the lock-holding thread unlocks the mutex. By contrast, in the lock-free version, *all threads can make progress*. If a thread is held up, it just won't do its share of the work, but everyone else gets to get on with their work.

**Lock-free VS locked**

It is worth noting that in general lock-free programming trades(交换) throughput and mean latency throughput for **predictable latency**. That is, a lock-free program will usually get less done than a corresponding locking program if there is not too much contention (since atomic operations are slow and affect a lot of the rest of the system), but it guarantees to never produce unpredictably large latencies.







## Lock-free VS lock-based

在 stackoverflow [What is lock-free multithreaded programming?](https://stackoverflow.com/questions/14011849/what-is-lock-free-multithreaded-programming) # [A](https://stackoverflow.com/a/14011948) 中，对这个话题进行了探讨: 

It is worth noting that in general lock-free programming trades(交换) throughput and mean latency throughput for **predictable latency**. That is, a lock-free program will usually get less done than a corresponding locking program if there is not too much contention (since atomic operations are slow and affect a lot of the rest of the system), but it guarantees to never produce unpredictably large latencies.

> NOTE: 上面这段话的表明意思是: "值得注意的是，一般来说，无锁编程用吞吐量和平均延迟吞吐量交换可预测延迟。也就是说，如果没有太多的争用(因为原子操作很慢，并且会影响系统的其他部分)，那么无锁程序通常比相应的锁定程序完成的工作要少，但是它保证不会产生不可预测的大延迟。"
>
> 1、lock-free programming 以 throughout 和 mean latency throughout 来换取 predictable latency。
>
> 2、lock-free 不一定比 lock-base 要慢: 当没有太多的thread contention的时候，lock-based的性能可能更佳。

## TODO

[rigtorp](https://github.com/rigtorp)/**[awesome-lockfree](https://github.com/rigtorp/awesome-lockfree)**



https://github.com/search?o=desc&q=lockfree&s=stars&type=Repositories

