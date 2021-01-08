# Producer–consumer problem



## wikipedia [Producer–consumer problem](https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem)



### Without semaphores or monitors

The producer–consumer problem, particularly in the case of a single producer and single consumer, strongly relates to implementing a [FIFO](https://en.wikipedia.org/wiki/FIFO_(computing_and_electronics)) or a [channel](https://en.wikipedia.org/wiki/Channel_(programming)). The producer–consumer pattern can provide highly efficient data communication without relying on semaphores, mutexes, or monitors *for data transfer*. The use of those primitives can be expansive in terms of performance, in comparison to basic read/write atomic operation. Channels and FIFOs are popular just because they avoid the need for end-to-end atomic synchronization. A basic example coded in C is shown below. 

> NOTE: lock-free

1、Atomic [read-modify-write](https://en.wikipedia.org/wiki/Read-modify-write) access to shared variables is avoided, as each of the two `Count` variables is updated only by a single thread. Also, these variables support an unbounded number of increment operations; the relation remains correct when their values wrap around on an **integer overflow**.

2、This example does not put threads to sleep, which may be acceptable depending on the system context. The `schedulerYield()` is inserted as an attempt to improve performance, and may be omitted. Thread libraries typically require semaphores or condition variables to control the sleep/wakeup of threads. In a multi-processor environment, thread sleep/wakeup would occur much less frequently than passing of data tokens, so avoiding atomic operations on data passing is beneficial.



## cornell cs3110 [Lecture 18: Concurrency—Producer/Consumer Pattern and Thread Pools](https://www.cs.cornell.edu/courses/cs3110/2010fa/lectures/lec18.html)

## Implementation

### C++

[A C++ Producer-Consumer Concurrency Template Library](https://www.drdobbs.com/a-c-producer-consumer-concurrency-templa/184401751?pgno=2)



### Eiffel

https://www.eiffel.org/doc/solutions/Producer-consumer



### Java

https://www.geeksforgeeks.org/producer-consumer-solution-using-threads-java/