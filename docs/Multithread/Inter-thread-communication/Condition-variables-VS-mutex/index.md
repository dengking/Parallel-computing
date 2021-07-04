# condition variables VS mutex


## stackoverflow [Advantages of using condition variables over mutex](https://stackoverflow.com/questions/4742196/advantages-of-using-condition-variables-over-mutex)

I was wondering what is the performance benefit of using **condition variables** over **mutex locks** in pthreads.

What I found is : "Without condition variables, the programmer would need to have threads continually polling (possibly in a critical section), to check if the condition is met. This can be very resource consuming since the thread would be continuously busy in this activity. A condition variable is a way to achieve the same goal without polling." (<https://computing.llnl.gov/tutorials/pthreads>)

***Summary***:作者在上面这一段所描述的是事实，因为condition variable是一种inter-thread communication，它允许由programmer来决定何时，基于何种条件，来通知（唤醒）被阻塞的线程。而mutex则并不具备此功能。


But it also seems that **mutex calls** are **blocking** (unlike spin-locks). Hence if a thread (`T1`) fails to get a lock because some other thread (`T2`) has the lock, `T1` is put to sleep by the OS, and is woken up only when `T2 `releases the lock and the OS gives `T1` the lock. The thread `T1` does not really **poll** to get the lock. From this description, it seems that there is no performance benefit of using condition variables. In either case, there is no polling involved. The OS anyway provides the benefit that the condition-variable paradigm can provide.



***SUMMARY*** : 关于上面这段中的**blocking**，参见APUE 14.2 Nonblocking I/O，其中nonblocking I/O的例子非常好，并且，那个例子也结合了**polling** ，和本问题正好相对应，本问题是：blocking + polling模式。

Can you please explain what actually happens.



### [A](https://stackoverflow.com/a/4742236)

A **condition variable** allows a thread to be signaled when something of interest to that thread occurs.

***SUMMARY*** : 上面这段话描述了condition variable的semantic。

By itself, a mutex doesn't do this.

If you just need mutual exclusion, then condition variables don't do anything for you. However, if you need to know when something happens, then condition variables can help.

For example, if you have a queue of items to work on, you'll have a mutex to ensure the queue's internals are consistent when accessed by the various producer and consumer threads. However, when the queue is empty, how will a consumer thread know when something is in there for it to work on? Without something like a condition variable it would need to **poll** the queue, taking and releasing the mutex on each poll (otherwise a producer thread could never put something on the queue).

Using a condition variable lets the consumer find that when the queue is empty it can just wait on the condition variable indicating that the queue has had something put into it. No polling - that thread does nothing until a producer puts something in the queue, then signals the condition that the queue has a new item.

***SUMMARY*** : 如果consumer thread的处理能力比producer thread的强，那么此时consumer thread就无需再mutex上阻塞，那么此时它就会一直进行poll了，显然这是对提问者问题中没有考虑的一种情况的补出，显然在这种 情况下，condition variable是明显优于mutex的；总的来说，condition variable是不同于mutex的一种机制；

***comments*** ： 

Right. A **mutex** only allows you to wait until the lock is available; a **condition variable** allows you to wait until some **application-defined condition** has changed. – [caf](https://stackoverflow.com/users/134633/caf) [Jan 20 '11 at 5:09](https://stackoverflow.com/questions/4742196/advantages-of-using-condition-variables-over-mutex#comment5244481_4742236)

I don't see why you can't drop the condition variable and just use the mutex in some simple cases. Make acquiring the mutex the "work is available" condition. Then have the producer thread acquire the mutex and the worker thread try to acquire it. When work is available, the producer unlocks the mutex. Then the OS will wake the worker with the mutex acquired and the worker will run to completion and then sleep (deadlock itself) by trying to re-acquire it's own mutex. Once the producer has more work, it can unlock the mutex. Care may need to be taken to check that the mutex is locked first. – [Eloff](https://stackoverflow.com/users/152580/eloff) [Apr 6 '13 at 20:34](https://stackoverflow.com/questions/4742196/advantages-of-using-condition-variables-over-mutex#comment22565687_4742236)



### [A](https://stackoverflow.com/a/4743033)

If you are looking for performance, then start reading about "non blocking / non locking" thread synchronization algorithms. They are based upon **atomic operations**, which `gcc` is kind enough to provide. Lookup `gcc` atomic operations. Our tests showed we could increment a global value with multiple threads using **atomic operation** magnitudes faster than locking with a mutex. [Here is some sample code that shows how to add items to and from a linked list from multiple threads at the same time without locking.](https://stackoverflow.com/questions/4600208/is-memory-allocation-in-linux-non-blocking/4650200#4650200)

For sleeping and waking threads, **signals** are much faster than **conditions**. You use `pthread_kill` to send the signal, and `sigwait` to sleep the thread. We tested this too with the same kind of performance benefits. [Here is some example code.](https://stackoverflow.com/questions/4016789/sleeping-in-a-thread-c-posix-threads/4676069#4676069)

***COMMENTS*** :

Thanks a lot for pointing out the area of non-locking synchronization! I am looking into it now!

Nice tip about using signals instead of condition variables. – [Eloff](https://stackoverflow.com/users/152580/eloff) [Apr 6 '13 at 18:16](https://stackoverflow.com/questions/4742196/advantages-of-using-condition-variables-over-mutex#comment22563237_4743033)

### [A](https://stackoverflow.com/a/4742791)

You're looking for too much overlap in two separate but related things: a mutex and a condition variable.

A common implementation approach for a mutex is to use a flag and a queue. The flag indicates whether the mutex is held by anyone (a single-count semaphore would work too), and the queue tracks which threads are in line waiting to acquire the mutex exclusively.

A condition variable is then implemented as another queue bolted onto that mutex. Threads that got in line to wait to acquire the mutex can—usually once they have acquired it—volunteer to get out of the front of the line and get into the condition queue instead. At this point, you have two separate set of waiters:

- Those waiting to acquire the mutex exclusively
- Those waiting for the condition variable to be signaled

When a thread holding the mutex exclusively signals the condition variable, for which we'll assume for now that it's a *singular signal* (unleashing no more than one waiting thread) and not a *broadcast*(unleashing all the waiting threads), the first thread in the condition variable queue gets shunted back over into the front (usually) of the mutex queue. Once the thread currently holding the mutex—usually the thread that signaled the condition variable—relinquishes the mutex, the next thread in the mutex queue can acquire it. That next thread in line will have been the one that was at the head of the condition variable queue.

There are many complicated details that come into play, but this sketch should give you a feel for the structures and operations in play.


## mutex and condition variable

condition variable是一种inter-thread communication的方式，而mutex并不具备此功能。mutex仅仅是一种互斥。