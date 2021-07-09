# 案例

下面是我遇到的，非常好的concurrent server，它们都非常典型，并且有的提供了多种concurrency方式。



## 并发模型

在下面文章中，对此进行了讨论:

一、kegel [The C10K problem](http://www.kegel.com/c10k.html)

二、lwn [The SO_REUSEPORT socket option](https://lwn.net/Articles/542629/) 

三、zhihu [如何深刻理解reactor和proactor？](https://www.zhihu.com/question/26943938) # [小林coding的回答](https://www.zhihu.com/question/26943938/answer/1856426252)

非常好的文章，将可能的并发模型总结得非常好。

大多数都是使用:

1、reactor、handler、acceptor

2、master worker

multiple thread

multiple process



## [Nginx](http://nginx.org/en/docs/beginners_guide.html)

nginx has one master process and several worker processes. The main purpose of the master process is to read and evaluate configuration, and maintain worker processes. Worker processes do actual processing of requests. nginx employs event-based model and OS-dependent mechanisms to efficiently distribute requests among worker processes. The number of worker processes is defined in the configuration file and may be fixed for a given configuration or automatically adjusted to the number of available CPU cores (see [worker_processes](http://nginx.org/en/docs/ngx_core_module.html#worker_processes)).

### 并发模型

多 Reactor 多进程方案



## [Redis](https://redis.io/)

### 并发模型

读取解析命令-多线程

执行命令-单线程

返回响应-多线程

## [Memcached](https://memcached.org/)

### 并发模型

多 Reactor 多线程方案

## inetd

TODO





## Python

### [Gunicorn](http://docs.gunicorn.org/en/latest/design.html)

Gunicorn is based on the pre-fork worker model. This means that there is a central master process that manages a set of worker processes. The master never knows anything about individual clients. All requests and responses are handled completely by worker processes.



### [Celery](http://docs.celeryproject.org/en/latest/userguide/workers.html)





### [Tornado](http://www.tornadoweb.org/)









