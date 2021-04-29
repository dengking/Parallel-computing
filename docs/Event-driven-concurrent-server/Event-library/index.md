# Event library

非常多的成熟的框架，进行了非常抽象、封装，最终user只需要指定event和event handler之间的映射关系即可。比如flask、libevent、libuv。

web framework都可以看做是event-driven model的实现。



## Python

[Eventlet](http://eventlet.net/)

[gevent](https://blog.gevent.org/)



## C、C++

1、都是基于IO-multiplexing

2、都是asynchronous、callback based notifications of IO



有非常多的基于event loop模型的software

1、[redis](https://redis.io/)



2、[libevent](http://libevent.org/)



3、[libuv](https://libuv.org/)

libuv is a multi-platform support library with a focus on asynchronous I/O.



## JavaScript

1、[node.js](https://nodejs.org/)

2、[JavaScript](https://developer.mozilla.org/en-US/docs/Web/JavaScript/EventLoop)



## Java



### [Netty](https://netty.io/index.html) 

Netty is *an asynchronous event-driven network application framework* for rapid development of maintainable high performance protocol servers & clients.



