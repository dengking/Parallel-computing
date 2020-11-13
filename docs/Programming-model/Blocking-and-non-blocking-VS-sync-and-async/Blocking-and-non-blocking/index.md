# Blocking and non-blocking



## Block的含义

"block"的原意是: "阻塞"，是指当前的流程被暂停了，它被暂停的原因有:

1) 等待耗时的操作完成

比如执行了一个非常耗时的操作。

2) 等待条件满足

比如等待lock。

## Blocking

### wikipedia [Blocking (computing)](https://en.wikipedia.org/wiki/Blocking_(computing))

In [computing](https://en.wikipedia.org/wiki/Computing), a *process* is an instance of a computer program that is being executed. A process always exists in exactly one *process state*. A process that is **blocked** is one that is waiting for some [event](https://en.wikipedia.org/wiki/Event_(computing)), such as a [resource](https://en.wikipedia.org/wiki/Resource_(computer_science)) becoming available or the completion of an [I/O](https://en.wikipedia.org/wiki/I/O) operation.[[1\]](https://en.wikipedia.org/wiki/Blocking_(computing)#cite_note-1)

> NOTE: 在I/O system call中，就有non blocking I/O

In a [multitasking](https://en.wikipedia.org/wiki/Computer_multitasking) computer system, individual [tasks](https://en.wikipedia.org/wiki/Task_(computing)), or [threads of execution](https://en.wikipedia.org/wiki/Thread_of_execution), must share the resources of the system. Shared resources include: the CPU, network and network interfaces, memory and disk.

When one task is using a resource, it is generally not possible, or desirable, for another task to access it. The techniques of [mutual exclusion](https://en.wikipedia.org/wiki/Mutual_exclusion) are used to prevent this concurrent use. When the other task is blocked, it is unable to execute until the first task has finished using the shared resource.

[Programming languages](https://en.wikipedia.org/wiki/Programming_language) and [scheduling algorithms](https://en.wikipedia.org/wiki/Scheduling_algorithm) are designed to minimize the over-all(全面的，整体的) effect blocking. A process that blocks may prevent local **work-tasks** from progressing. In this case "blocking" often is seen as not wanted.[[2\]](https://en.wikipedia.org/wiki/Blocking_(computing)#cite_note-2) However, such **work-tasks** may instead have been assigned to independent processes, where halting one has no or little effect on the others, since scheduling will continue. An example is "blocking on a [channel](https://en.wikipedia.org/wiki/Channel_(programming))" where passively(变动地) waiting for the other part (no polling or spin loop) is part of the semantics of channels.[[3\]](https://en.wikipedia.org/wiki/Blocking_(computing)#cite_note-3) Correctly engineered any of these may be used to implement reactive systems.

[Deadlock](https://en.wikipedia.org/wiki/Deadlock) means that processes pathologically wait for each other in a circle. As such it is not directly associated with blocking.

Once the event occurs for which the process is waiting ("is blocked on"), the process is advanced from blocked state to an imminent one, such as [runnable](https://en.wikipedia.org/wiki/Process_state#Ready).

#### See also

- [Non-blocking algorithm](https://en.wikipedia.org/wiki/Non-blocking_algorithm)
- [Non-blocking synchronization](https://en.wikipedia.org/wiki/Non-blocking_synchronization)
- [Concurrent computing](https://en.wikipedia.org/wiki/Concurrent_computing)
- [Race condition](https://en.wikipedia.org/wiki/Race_condition#Computing)
- [Deadlock](https://en.wikipedia.org/wiki/Deadlock)
- [Scheduling (computing)](https://en.wikipedia.org/wiki/Scheduling_(computing))
- [Data dependency](https://en.wikipedia.org/wiki/Data_dependency)



### 补充内容

1) Node.js

- About Node.js

- Node.js-Overview of Blocking vs Non-Blocking

2) Unix-The TTY demystified

3) redis-An introduction to Redis data types and abstractions

其中Blocking operations on lists章节的观点非常具有参考意义

- wikipedia-Asynchronous-IO
- wikipedia-Process-state

### Blocking and timeout

在很多应用中，一直将application block是非常危险的，因为它可能导致系统不可用；所以为block设置一个timeout是非常重要的；





### Process的blocked状态

OS中，process有blocked状态，显然是符合前面的描述的。



## Non-blocking



### Wikipedia [Non-blocking algorithm](https://infogalactic.com/info/Non-blocking_algorithm)





### Wikipedia [Asynchronous I/O](https://infogalactic.com/info/Asynchronous_I/O)