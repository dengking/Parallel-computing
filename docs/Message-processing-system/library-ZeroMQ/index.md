# ZeroMQ



## [ZeroMQ](https://zeromq.org/)

ZeroMQ (also known as ØMQ, 0MQ, or zmq) looks like an embeddable networking library but acts like a concurrency framework. It gives you sockets that carry atomic messages across various transports like in-process, inter-process, TCP, and multicast. You can connect sockets N-to-N with patterns like fan-out, pub-sub, task distribution, and request-reply. It's fast enough to be the fabric for clustered products. Its asynchronous I/O model gives you scalable multicore applications, built as asynchronous message-processing tasks. It has a score of language APIs and runs on most operating systems.

> NOTE: 
>
> 一、上面这段话基本上总结了ZeroMQ的核心特性:
>
> 二、它的socket是非常抽象的，它的设计遵循了abstraction原则
>
> 1、它允许socket进行灵活的连接: "You can connect sockets N-to-N"
>
> 2、它提供了message passing的各种模式: "fan-out, pub-sub, task distribution, and request-reply"
>
> 3、它隐藏了各种可能的transport细节，在这些transport之上建立起了consistent API，使得programmer能够"program to an abstraction and polymorphism"；其实它的这种抽象思路是非常容易理解的，因为:
>
> a、"Everything is a file" abstraction，ZeroMQ的socket其实就相当于前面所说的"file"，因此，对于每个ZeroMQ的socket，都能够实现read、write基本操作
>
> 总的来说: 通过上述这种设计思路，ZeroMQ给予了programmer灵活的message passing 功能
>
> 三、它的价值: 上述提及的这些，对于实现各种基于message的application都是具有非常重要得多意义的
>
> 四、ZeroMQ体现了abstraction的机制： simplify

## github [ libzmq](https://github.com/zeromq/libzmq)

