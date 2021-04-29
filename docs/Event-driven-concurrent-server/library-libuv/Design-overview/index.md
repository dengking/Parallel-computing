# Design overview[¶](http://docs.libuv.org/en/v1.x/design.html#design-overview)

`libuv` is cross-platform support library which was originally written for `NodeJS`. It’s designed around the **event-driven asynchronous I/O model**.

The library provides much more than a simple abstraction over different **I/O polling mechanisms**: ‘handles’ and ‘streams’ provide a high level abstraction for sockets and other entities; cross-platform file I/O and threading functionality is also provided, amongst other things.

Here is a diagram illustrating the different parts that compose `libuv` and what subsystem they relate to:

[![_images/architecture.png](http://docs.libuv.org/en/v1.x/_images/architecture.png)](http://docs.libuv.org/en/v1.x/_images/architecture.png)







## The I/O loop

The **I/O (or event) loop** is the central part of `libuv`. It establishes the content for all I/O operations, and it’s meant to be tied to a **single thread**. One can run multiple event loops as long as each runs in a different **thread**. The `libuv` event loop (or any other API involving the loop or handles, for that matter) **is not thread-safe** except where stated otherwise(除非另有说明，否则libuv事件循环（或涉及循环或句柄的任何其他API）都不是线程安全的).



The **event loop** follows the rather usual **single threaded asynchronous I/O approach**: all (network) I/O is performed on **non-blocking sockets** which are **polled** using the best mechanism available on the given platform: `epoll` on Linux, `kqueue` on OSX and other BSDs, event ports on SunOS and `IOCP` on Windows. As part of a loop iteration the loop will block waiting for **I/O activity** on sockets which have been added to the **poller** and callbacks will be fired indicating **socket conditions** (readable, writable hangup) so **handles** can read, write or perform the desired I/O operation.

> NOTE:
>
> 1、最后一段话说明: callback是在IO loop thread中执行的，对应的是下面的图中的"call pending callback"

In order to better understand how the **event loop** operates, the following diagram illustrates all stages of a loop iteration:

![_images/loop_iteration.png](http://docs.libuv.org/en/v1.x/_images/loop_iteration.png)



1、The loop concept of ‘now’ is updated. The **event loop** caches the **current time** at the start of the event loop tick in order to reduce the number of time-related system calls.

> NOTE:
>
> 1、这样做的目的是什么？是为了实现 "High resolution clock"？

2、If the loop is ***alive*** an iteration is started, otherwise the loop will exit immediately. So, when is a loop considered to be *alive*? If a loop has active and ref’d handles, active requests or closing handles it’s considered to be *alive*.

NOTE: ref’d 参考的（referenced）

3、**Due timers** are run. All active timers scheduled for a time before the loop’s concept of *now* get their callbacks called.

> NOTE:due timer 可以理解为“到期计数器”；第二句话的主干是：All active timers get their callbacks called.

3、**Pending callbacks** are called. All I/O callbacks are called right after polling for I/O, for the most part. There are cases, however, in which calling such a callback is deferred for the next loop iteration. If the previous iteration deferred any I/O callback it will be run at this point.

4、**Idle handle callbacks** are called. Despite the unfortunate name, idle handles are run on every loop iteration, if they are active.

5、**Prepare handle callbacks** are called. Prepare handles get their callbacks called right before the loop will block for I/O.

6、Poll timeout is calculated. Before blocking for I/O the loop calculates for how long it should block. These are the rules when calculating the timeout:

> NOTE:
>
> 1、典型的" block with timeout-system call with timeout"

- If the loop was run with the `UV_RUN_NOWAIT` flag, the timeout is 0.
- If the loop is going to be stopped ([`uv_stop()`](http://docs.libuv.org/en/v1.x/loop.html#c.uv_stop) was called), the timeout is 0.
- If there are no active handles or requests, the timeout is 0.
- If there are any idle handles active, the timeout is 0.
- If there are any handles pending to be closed, the timeout is 0.
- If none of the above cases matches, the timeout of the closest timer is taken, or if there are no active timers, infinity.





### Mock asynchronous file I/O 

libuv uses a thread pool to make asynchronous file I/O operations possible, but network I/O is **always** performed in a single thread, each loop’s thread.

> NOTE: 
>
> 1、下面会进行说明

### Note

While the polling mechanism is different, libuv makes the execution model consistent across Unix systems and Windows.

## File I/O

> NOTE: 
>
> 1、这是典型的asynchronous method invocation(sync to async)，它的实现方式是: thread pool + queue

Unlike network I/O, there are no platform-specific file I/O primitives libuv could rely on, so the current approach is to run blocking file I/O operations in a thread pool.

For a thorough explanation of the cross-platform file I/O landscape, checkout [this post](http://blog.libtorrent.org/2012/10/asynchronous-disk-io/).

libuv currently uses a global thread pool on which all loops can queue work. 3 types of operations are currently run on this pool:

> - File system operations
> - DNS functions (getaddrinfo and getnameinfo)
> - User specified code via [`uv_queue_work()`](http://docs.libuv.org/en/v1.x/threadpool.html#c.uv_queue_work)

Warning

See the [Thread pool work scheduling](http://docs.libuv.org/en/v1.x/threadpool.html#threadpool) section for more details, but keep in mind the thread pool size is quite limited.