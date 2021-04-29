# 关于本章

1、Concurrent computing有着非常广泛的application，因此它的design pattern很多都是和具体的application密切相关的，所以我将部分内容分散到了具体的application中

2、由于event-driven model非常强大，能够描述非常多的问题，包括本章描述的一些design pattern也是可以归入到event-driven model的，为了区分，本章所描述的patter，主要是intra-process，即在同一个process内；这些pattern中，也会涉及到message，当谈及message的时候，它们的passing都是不经过network的，大多数都是在thread之间进行传输





## wikipedia [Concurrency pattern](http://en.wiki.sxisa.org/wiki/Concurrency_pattern)







实现event-driven model的一些pattern。

| Pattern                                                      | 说明 |
| ------------------------------------------------------------ | ---- |
|                                                              |      |
| [Observer pattern](https://en.wikipedia.org/wiki/Observer_pattern) |      |
|                                                              |      |
| [Actor model](https://en.wikipedia.org/wiki/Actor_model)     |      |
| [Reactor pattern](https://en.wikipedia.org/wiki/Reactor_pattern) |      |
| [Proactor pattern](https://en.wikipedia.org/wiki/Proactor_pattern) |      |
|                                                              |      |



[Message queue](https://en.wikipedia.org/wiki/Message_queue)

[Messaging pattern](https://en.wikipedia.org/wiki/Messaging_pattern)

[Publish–subscribe pattern](https://en.wikipedia.org/wiki/Publish%E2%80%93subscribe_pattern)

一个典型的例子就是redis的pub/sub，在这种情况下，pub需要注册回调函数，用于指定当收到信息时，需要执行的动作。这非常类似于signal handler。