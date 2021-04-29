# Implementation



## troglobit [Threads vs Event Loop, Again](http://troglobit.com/2017/09/19/threads-vs-event-loop-again-.../) 

I still get asked this, from time to time. Maybe it’s because I only use **event loops**, maybe it’s because I’ve written [libuEv](https://github.com/troglobit/libuev), or maybe people still don’t understand:

> Why an event loop, why not use threads?

So here’s my response, once more.

With the advent of light-weight processes (threads) programmers these days have a [golden hammer](http://c2.com/cgi/wiki?GoldenHammer) they often swing without consideration. Event loops and non-blocking I/O is often a far easier approach, as well as less error prone.

The purpose of many applications is, with a little logic sprinkled on top, to act on network packets entering an interface, timeouts expiring, mouse clicks, or other types of events. Such applications are often very well suited to use an **event loop**.

> NOTE: 适合event loop的情形

Applications that need to churn massively **parallel algorithms** are more suitable for running multiple (independent) threads on several CPU cores. However, threaded applications must deal with the side effects of concurrency, like race conditions, deadlocks, live locks, etc. Writing error free threaded applications is hard, debugging them can be even harder.

> NOTE : 适合multiple threads的情形

Sometimes the combination of multiple threads *and* an event loop per thread can be the best approach, but each application of course needs to be broken down individually to find the most optimal approach. Do keep in mind, however, that not all systems your application will run on have multiple CPU cores – some small embedded systems still use a single CPU core, even though they run Linux, with multiple threads a program may actually run slower! Always profile your program, and if possible, test it on different architectures.

> NOTE: combine event loop and multiple threads


## 20190710

现在想想，其实这个问题更应该是reactor和multithread之间的异与同