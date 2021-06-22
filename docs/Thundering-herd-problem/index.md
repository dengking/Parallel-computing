# Thundering herd problem

一、"Thundering Herd" 的意思是 "惊群效应"。

二、它的本质还是many to one:

当many wait on one，则一旦条件满足，如果notify all，就会导致"Thundering Herd"；

解决的方法: notify one、serialize(串行) 



## wanweibaike [Thundering herd problem](https://en.wanweibaike.com/wiki-Thundering%20herd%20problem)

In [computer science](https://en.wanweibaike.com/wiki-Computer_science), the **thundering herd problem** occurs when a large number of processes or threads waiting for an event are awoken when that event occurs, but only one process is able to handle the event. After all the processes wake up, they will start to handle the event, but only one will win. All processes will compete for resources, possibly freezing the computer, until the herd is calmed down again.[[1\]](https://en.wanweibaike.com/wiki-Thundering herd problem#cite_note-1)[[2\]](https://en.wanweibaike.com/wiki-Thundering herd problem#cite_note-2)



### Mitigation

> NOTE: 
>
> 总的来说，环节的方法就是notify one、serialize

The Linux-kernel will serialize(串行) responses for requests to a single file descriptor, so only one thread (process) is woken up.[[3\]](https://en.wanweibaike.com/wiki-Thundering herd problem#cite_note-3)

> NOTE: 
>
> 在 uwsgi [Serializing accept(), AKA Thundering Herd, AKA the Zeeg Problem](https://uwsgi-docs.readthedocs.io/en/latest/articles/SerializingAccept.html) 中，给出了实现方案

Similarly in Microsoft Windows, [I/O completion ports](https://en.wanweibaike.com/wiki-Input/output_completion_port) can mitigate the thundering herd problem, as they can be configured such that only one of the threads waiting on the completion port is woken up when an event occurs.[[4\]](https://en.wanweibaike.com/wiki-Thundering herd problem#cite_note-4)

## 各个领域中应付thundering herd总结

### Concurrent server

| server       | 参考文章                                                     |      |
| ------------ | ------------------------------------------------------------ | ---- |
| Nginx        | `Thundering-herd-in-Nginx`                                   |      |
| uwsgi        | uwsgi-docs [Serializing accept(), AKA Thundering Herd, AKA the Zeeg Problem](https://uwsgi-docs.readthedocs.io/en/latest/articles/SerializingAccept.html) |      |
| Apache httpd | [accept Serialization - Multiple Sockets](https://httpd.apache.org/docs/2.4/misc/perf-tuning.html)、[accept Serialization - Single Socket](https://httpd.apache.org/docs/2.4/misc/perf-tuning.html) |      |



### Distributed computing

|           | 文章                                                         |      |
| --------- | ------------------------------------------------------------ | ---- |
| zookeeper | zookeeper [A Guide to Creating Higher-level Constructs with ZooKeeper#Leader Election](https://zookeeper.apache.org/doc/r3.6.2/recipes.html#sc_leaderElection) |      |
|           | `Thundering-Herd-in-distributed-computing`                   |      |

### Linux OS

参见 `Thundering-Herd-in-Linux` 章节

### Multithread

参见 `Concurrent-computing\Concurrency-control\Lock\Spinlock` 章节。
