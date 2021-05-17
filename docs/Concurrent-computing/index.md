# Concurrent computing



## 我的学习之路

一、APUE

从中学习基本的pthread API，基本的thread concurrency control，基本的inter-thread communication、event notification等，主要是C、pthread。



二、C++ thread library、asynchronous programming library



三、一些design pattern

1、参见 `Event-driven-concurrent-server\Design-pattern`

2、thread pool



四、学习lock free

1、C++ memory model、atomic library

2、`CPU-cache-memory`、cache coherence、cache optimization

3、custom spinning lock

4、参见 `Concurrent-computing\Concurrency-control\Non-blocking` 章节



五、Expert-Herb-Sutter的Effective-Concurrency-Serial

前面、系统性的、概括、居高临下、非常好，可以作为系统性的学习concurrency的guide。

后续很多内容，可能是基于这个系列中的内容展开、组织。

## 主要topic

下面总结了学习concurrent programming时，涉及的一些topic。

### Concurrency control

在 wikipedia [Concurrent computing#Coordinating access to shared resources](https://en.wikipedia.org/wiki/Concurrent_computing#Coordinating_access_to_shared_resources) 中说明了: 为什么在concurrent computing中需要concurrency control:

> The main challenge in designing concurrent programs is [concurrency control](https://en.wikipedia.org/wiki/Concurrency_control): ensuring the correct sequencing of the interactions or communications between different computational executions, and coordinating access to resources that are shared among executions.

### Unit

“concurrent computing”即并发，一个我们平时经常听到的词语。在谈及concurrency的时候，我们要思考：

1、并发的entity/unit是什么？



### `Concurrent-data-structure`

并发的数据结构。



## wikipedia [Concurrent computing](https://en.wikipedia.org/wiki/Concurrent_computing#Concurrent_programming_languages)

**Concurrent computing** is a form of [computing](https://en.wikipedia.org/wiki/Computing) in which several [computations](https://en.wikipedia.org/wiki/Computation) are executed *[concurrently](https://en.wikipedia.org/wiki/Concurrency_(computer_science))*—during overlapping time periods—instead of *sequentially*, with one completing before the next starts.

> NOTE: 
>
> 一、并行 VS 串行
