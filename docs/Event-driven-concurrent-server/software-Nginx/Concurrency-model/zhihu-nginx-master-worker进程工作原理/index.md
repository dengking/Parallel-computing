# zhihu [nginx master-worker进程工作原理](https://zhuanlan.zhihu.com/p/96757160)

nginx的master-worker进程模型是其能够高性能的处理用户请求的原因之一，而且这里的每个worker进程都只会启动一个线程来处理用户请求。通常我们会将worker进程的数量设置得与我们的CPU数量一致，nginx也会将每个进程与每个CPU进行绑定。通过这种方式，可以充分利用操作系统多核的特性，并且能够最大限度的减少线程之间的切换而导致的资源损耗。本文首先会对nginx的master-worker进程模型进行讲解，然后会从源码的角度对nginx的master-worker进程模型的实现原理进行讲解。

## 1. 工作原理

在nginx启动过程中，主进程就是master进程，该进程在启动各个worker进程之后，就会进入一个无限循环中，以处理客户端发送过来的控制指令；而worker进程则会进入一个循环中，从而不断接收客户端的连接请求以及处理请求。如下是master-worker进程模型的一个原理示意图：



![img](https://pic3.zhimg.com/80/v2-8cee813470a8b7ee7bf76d3cdf8ccb92_720w.jpg)



从图中我们可以看出nginx工作的一般性原理：

- master进程通过接收客户端的请求，比如`-s reload`、`-s stop`等，解析这些命令之后，通过进程间通信，将相应的指令发送到各个worker进程，从而实现对worker进程的控制；
- 每个worker进程都会竞争同一个共享锁，只有竞争到共享锁的进程才能够处理客户端请求；
- 当客户端请求发送过来之后，worker进程会处理该请求的事件，如果是accept事件，则会将其添加到accept队列中，如果是read或者write事件，则会将其添加到read-write队列中；
- 在将事件添加到相应的队列中之后，在持有共享锁的情况下，nginx会处理完accept队列中的客户端连接请求，而对于read或者write事件，则会在释放锁之后直接从read-write队列中取出事件来处理。