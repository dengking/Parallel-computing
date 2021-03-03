

# Lock-free VS lock-based

## Performance

在下面文章中，对此进行了讨论。

### 1、stackoverflow [What is lock-free multithreaded programming?](https://stackoverflow.com/questions/14011849/what-is-lock-free-multithreaded-programming) # [A](https://stackoverflow.com/a/14011948) 

It is worth noting that in general lock-free programming trades(交换) throughput and mean latency throughput for **predictable latency**. That is, a lock-free program will usually get less done than a corresponding locking program if there is not too much contention (since atomic operations are slow and affect a lot of the rest of the system), but it guarantees to never produce unpredictably large latencies.

> NOTE: 上面这段话的表明意思是: "值得注意的是，一般来说，无锁编程用吞吐量和平均延迟吞吐量交换可预测延迟。也就是说，如果没有太多的争用(因为原子操作很慢，并且会影响系统的其他部分)，那么无锁程序通常比相应的锁定程序完成的工作要少，但是它保证不会产生不可预测的大延迟。"
>
> 1、lock-free programming 以 throughout 和 mean latency throughout 来换取 predictable latency。
>
> 2、lock-free 不一定比 lock-base 要慢: 当没有太多的thread contention的时候，lock-based的性能可能更佳。

### 2、arangodb [Comparison: Lockless programming with atomics in C++ 11 vs. mutex and RW-locks](https://www.arangodb.com/2015/02/comparing-atomic-mutex-rwlocks/)

#### Methods tested

Here is a list of the implemented algorithms with explanations:

- `Read`: reads the current value and increases control counter
- `Write`: reads the old value, increases it and writes it back
- `Set`: writes a loop counter into the protected variable

0: Unlocked

1: Mutexes

2: Writelock / Writelock

3: Writelock / Readlock

4: Atomic Read & Write

5: Atomic Read “consume”, Atomic Set “release” for setting

6: Atomic Read “acquire”, Atomic Set “release” for setting

7: Atomic Read “consume”, Atomic Set “cst – consistent” for setting

8: Atomic Read “acquire”, Atomic Set “cst – consistent” for setting

9: Atomic Read “consume”, Atomic Set “relaxed” for setting

10: Atomic Read “acquire”, Atomic Set “relaxed” for setting

11: Atomic Read “Acquire”, Atomic exchange weak for writing

12: Atomic Read “consume”, Atomic exchange weak for writing

#### Test Results

Heres a table of the time in s the tests took to execute:

| TESTCASE / OS | WINDOWS 8.1 | GRML LINUX X64 | MAC OS X  | LINUX ARM | LINUX X64 |
| :------------ | :---------- | :------------- | :-------- | :-------- | :-------- |
| #0            | 0.033       | 0.0300119      | 0.02503   | 0.18299   | 0.02895   |
| #1            | 479.878     | 5.7003600      | 118.47100 | 21.29970  | 4.47721   |
| #2            | 1.45997     | 4.5296900      | 142.61700 | 23.29240  | 4.72051   |
| #3            | 4.70791     | 6.3525200      | 7.65026   | 23.82040  | 7.87677   |
| #4            | 0.056999    | 0.0454769      | 0.03771   | 0.94990   | 0.035302  |
| #5            | 0.033999    | 0.0263720      | 0.02774   | 0.58076   | 0.017803  |
| #6            | 0.032999    | 0.0286388      | 0.02785   | 0.58125   | 0.017604  |
| #7            | 0.060998    | 0.0528450      | 0.03783   | 0.57926   | 0.033422  |
| #8            | 0.060999    | 0.0536420      | 0.03807   | 0.57851   | 0.033546  |
| #9            | 0.032999    | 0.0299869      | 0.02606   | 0.55443   | 0.017258  |
| #10           | 0.033999    | 0.0235679      | 0.02593   | 0.54985   | 0.017839  |
| #11           | 0.058999    | 0.0534279      | 0.06688   | 0.56929   | 0.030724  |
| #12           | 0.059998    | 0.0676858      | 0.07563   | 0.57466   | 0.036788  |



### 3、lock-based add VS non-lock atomic add

使用atomic add和用lock来保护两者都能够实现正确的操作（顺序、原子、互斥），为什么前者比后者性能更好？

因为: lock-based存在lock contention，因此存在system call(system call的成本是比较高的)、存在blocking、等等，而atomic add则没有前面描述的内容。

### 4、infoq [Lock-free Programming in C++ with Herb Sutter](https://www.infoq.com/news/2014/10/cpp-lock-free-programming/)

其中进行了较好的总结。

## Cooperative VS contention 

lock显然是contention 

lock-free是cooperative 

下面是关于这个topic的素材: 



1、preshing [Acquire and Release Semantics](https://preshing.com/20120913/acquire-and-release-semantics/)

Generally speaking, in [lock-free programming](http://preshing.com/20120612/an-introduction-to-lock-free-programming), there are two ways in which threads can manipulate shared memory: They can compete with each other for a resource, or they can pass information co-operatively from one thread to another. Acquire and release semantics are crucial for the latter: reliable passing of information between threads. 

2、工程programming-language的`Memory-model-and-atomic-library\Design`章节



