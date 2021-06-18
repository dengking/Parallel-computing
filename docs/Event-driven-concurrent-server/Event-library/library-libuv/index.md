# Welcome to the libuv documentation[¶](http://docs.libuv.org/en/v1.x/#welcome-to-the-libuv-documentation)

## Overview[¶](http://docs.libuv.org/en/v1.x/#overview)

`libuv` is a multi-platform support library with a focus on asynchronous I/O. It was primarily developed for use by [Node.js](http://nodejs.org/), but it’s also used by [Luvit](http://luvit.io/), [Julia](http://julialang.org/), [pyuv](https://github.com/saghul/pyuv), and [others](https://github.com/libuv/libuv/wiki/Projects-that-use-libuv).

> NOTE: 
>
> 1、需要注意的是，上述"asynchronous IO"，并不是在工程`Linux-OS` 的 `Programming\IO\IO-model` 章节中的asynchronous IO，从下面的描述来看，libuv 也是使用的 IO-multiplexing，从准确的IO-multiplexing的实现来看，它是采用的non-blocking IO，关于此，在 Design overview[¶](http://docs.libuv.org/en/v1.x/design.html#design-overview) 中也进行了说明:
>
> > The **event loop** follows the rather usual **single threaded asynchronous I/O approach**: all (network) I/O is performed on **non-blocking sockets** which are **polled** using the best mechanism available on the given platform

## Features[¶](http://docs.libuv.org/en/v1.x/#features)

1、Full-featured **event loop** backed by epoll, kqueue, IOCP, event ports.

> NOTE: 
>
> 1、I have seen the event loop in many places, including [libevent](https://libevent.org/), [celery](http://docs.celeryproject.org/en/latest/userguide/extending.html#worker)

2、Asynchronous TCP and UDP sockets

> NOTE:
>
> 1、更加准确地来说，是non-blocking IO
>
> 2、这是network IO

3、Asynchronous DNS resolution

4、Asynchronous file and file system operations

> NOTE:
>
> 1、如何实现的？
>
> 2、这是file-IO

5、File system events

6、ANSI escape code controlled TTY

7、IPC with socket sharing, using Unix domain sockets or named pipes (Windows)

8、Child processes

> NOTE: 
>
> 1、process功能

9、Thread pool

10、Signal handling

11、High resolution clock

> NOTE:
>
> 关于它的实现，参见 :
>
> 1、Design overview[¶](http://docs.libuv.org/en/v1.x/design.html#design-overview) # The I/O loop

12、Threading and synchronization primitives

> NOTE:
>
> 1、从上面的描述可以看出，libuv对很多功能进行了非常好的 封装



## stackoverflow [Single Threaded Event Loop vs Multi Threaded Non Blocking Worker in Node.JS](https://stackoverflow.com/questions/21485920/single-threaded-event-loop-vs-multi-threaded-non-blocking-worker-in-node-js)

Node.JS biggest advantage is it's non blocking nature. It's single threaded, so it doesn't need to spawn a new thread for each new incoming connection.

Behind the **event-loop** (which is in fact **single threaded**), there is a "Non blocking Worker". This thing is not single threaded anymore, so (as far as I understood) it can spawn a new thread for each task.

Maybe I misunderstood something, but where exactly is the advantage. If there are to many tasks to handle, wouldn't be the Non Blocking Working turn into a Blocking Worker?

Thanks Christian

### [A](https://stackoverflow.com/a/21488067)

[You need to read about libuv](http://nikhilm.github.io/uvbook/), the "magic" behind node's non-blocking I/O.

The important thing to take away from the libuv book is that libuv uses the host OS's [asynchronous](http://en.wikipedia.org/wiki/Epoll)[I/O](http://en.wikipedia.org/wiki/Kqueue) [facilities](http://en.wikipedia.org/wiki/IOCP); **it does not simply create a new thread** for every connection.

libuv tells the OS that it would like to know about any changes (connection state, data received, etc) that happen on **a particular set of sockets**. It is then up to the OS to deal with managing the connections. The OS itself may create one or more threads to accomplish that, but that's not our concern. (And it certainly won't create a thread for every connection.)

For other types of operations like calls to C libraries that may be computationally expensive (ie crypto), libuv provides a *thread pool* on which those operations may run. Since it is a thread pool, again you don't have to worry about thread count growing without bound. When the pool is fully busy, operations are queued.

So yes, JavaScript runs on a **single thread**. Yes, node (via libuv) spawns many threads in the background to do work so that it need not block the JavaScript thread. However, the **thread count** is always controlled, and I/O generally doesn't even get its own node-allocated thread because that's handled by the OS.



### [A](https://stackoverflow.com/a/21486005)

Alright, let's break this down a bit. Single threaded applications have advantages: you can never get deadlocks or race conditions. These issues stem from simultaneous memory access in multi-threaded systems. If two threads access the same piece of information weird things can happen.

So why does JavaScript have Workers? If you need do some heavy processing you're going to block the event loop, you could try to split up the workload by generating timer events but that's tedious. A Worker allows you to spawn a thread under one condition: **no shared memory access**. This solves the issue of heavy processing in a single threaded environment while avoiding the pitfalls of multi-threaded environments (deadlocks, race-conditions).

And as @dandavis said, if you have a multi-core CPU (which everyone does these days) the Worker threads can be offloaded to the other cores.

You have to appreciate that, although JavaScript is single threaded, the environment around it is still very much multi-threaded. Reading a file is non-blocking in Node.JS but there is likely a thread to support it in the OS.

------

As a minor addendum I would say that Node.JS's biggest advantage is that it allows you to write JavaScript on the server, which allows you to **share code** between the client and the server. The fact that it's non-blocking is nice but threads already solve that. The non-blocking IO stems from the single threaded-ness. It's very inconvenient to have a single thread with blocking IO.

