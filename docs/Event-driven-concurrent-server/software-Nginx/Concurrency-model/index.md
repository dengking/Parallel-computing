# Nginx concurrency model



## csdn [详解nginx的master进程和worker进程](https://blog.csdn.net/yrx420909/article/details/104513013/)

> NOTE: 
>
> 这篇文章比较精简，不涉及实现细节



## csdn [Nginx资料之Master与Worker基础概念](https://blog.csdn.net/sky6even/article/details/81409800)

master进程主要用来管理worker进程，包含：接收来自外界的信号，向各worker进程发送信号，监控worker进程的运行状态，当worker进程退出后(异常情况下)，会自动重新启动新的worker进程。

　　worker进程则处理基本的网络事件。多个worker进程之间是对等的，他们同等竞争来自客户端的请求，各进程互相之间是独立的。一个请求，只可能在一个worker进程中处理，一个worker进程，不可能处理其它进程的请求。worker进程的个数是可以设置的，一般我们会设置与机器cpu核数一致。

### nginx的进程模型：

![这里写图片描述](https://img-blog.csdn.net/20180804123742622?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NreTZldmVu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### worker进程处理请求：

worker进程之间是平等的，每个进程，处理请求的机会也是一样的。当我们提供80端口的http服务时，一个连接请求过来，每个进程都有可能处理这个连接，怎么做到的呢？首先，每个worker进程都是从master进程fork过来，在master进程里面，先建立好需要listen的socket（listenfd）之后，然后再fork出多个worker进程。所有worker进程的listenfd会在新连接到来时变得可读，为保证只有一个进程处理该连接，所有worker进程在注册listenfd读事件前抢`accept_mutex`，抢到互斥锁的那个进程注册listenfd读事件，在读事件里调用accept接受该连接。当一个worker进程在accept这个连接之后，就开始读取请求，解析请求，处理请求，产生数据后，再返回给客户端，最后才断开连接，这样一个完整的请求就是这样的了。我们可以看到，一个请求，完全由worker进程来处理，而且只在一个worker进程中处理。

> NOTE: 
>
> 上述并没有说到使用`accept_mutex`的关键，关键是: "mitigate缓和缓解thundering herd惊群效应Nginx-serialize串行accept-accept_mutex"

### nginx进程模型的好处：

> NOTE: 
>
> "multithread多线程-VS-multiprocess多进程"

1、对于每个worker进程来说，独立的进程，不需要加锁，所以省掉了锁带来的开销，同时在编程以及问题查找时，也会方便很多。

2、采用独立的进程，可以让互相之间不会影响，一个进程退出后，其它进程还在工作，服务不会中断，master进程则很快启动新的worker进程。当然，worker进程的异常退出，肯定是程序有bug了，异常退出，会导致当前worker上的所有请求失败，不过不会影响到所有请求，所以降低了风险。





## stackoverflow [what is worker_processes and worker_connections in Nginx?](https://stackoverflow.com/questions/23386986/what-is-worker-processes-and-worker-connections-in-nginx)

