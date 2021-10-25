# Async and sync

今天在阅读jQuery的ajax的代码的时候，看到了其中有非常多的**回调函数**等，显然这是**异步编程**的思想，于是我想，有必要总结Asynchronous  programming的相关知识了。

## Async

"async"即"异步"。asynchronous的思想是: 对于耗时操作，不等待它执行完成，让它在**main loop/flow **之外独立运行而不阻塞**main loop**，这样main loop就可以在它执行的这段时间内去处理其他的事情，待耗时操作完成的时候(是一个**event**)，它再**通知**(notify)main loop它的完成，然后**main loop**再来处理它的结果。

当进行**asynchronous programming**的时候，往往需要表达"当 某个**event** 发生时，执行 某个 **callback**"，这个**callback**往往是用户注册的自定义函数，这种是可以使用fluent API来进行描述的，它其实非常类似于builder pattern，即由用户来设置**callback**，最最典型的就是jQuery，现代很多programming language都是支持这种paradigm的。

### Wikipedia [Asynchrony (computer programming)](https://infogalactic.com/info/Asynchrony_(computer_programming))

> https://en.wikipedia.org/wiki/Asynchrony_(computer_programming)asynchronous 

**Asynchrony**, in [computer programming](https://infogalactic.com/info/Computer_programming), refers to the occurrence of events independently of the main [program flow](https://infogalactic.com/info/Control_flow) and ways to deal with such events. These may be "outside" events such as the arrival of [signals](https://infogalactic.com/info/Unix_signal), or actions instigated(发起的) by a program that take place [concurrently](https://infogalactic.com/info/Concurrent_computing) with program execution, without the program *blocking* to wait for results.[[1\]](https://infogalactic.com/info/Asynchrony_(computer_programming)#cite_note-davies-1) [Asynchronous input/output](https://infogalactic.com/info/Asynchronous_I/O) is an example of the latter cause(原因) of **asynchrony**, and lets programs issue commands to storage or network devices that service these requests while the [processor](https://infogalactic.com/info/Central_processing_unit) continues executing the program. Doing so provides a degree of [parallelism](https://infogalactic.com/info/Parallel_computing).[[1\]](https://infogalactic.com/info/Asynchrony_(computer_programming)#cite_note-davies-1)

> NOTE: **Asynchrony**的本质含义: 某个**event**的出现是独立于 **main program flow** 的，这个event可能是inside的，也可能是outside的，下面是例子: 
>
> 1) outside: signal是 asynchronous event，并且它是典型的 outside event，它的出现是独立于main program flow的。
>
> 2) inside: "[Asynchronous input/output](https://infogalactic.com/info/Asynchronous_I/O) is an example of the latter cause(原因) of **asynchrony**"的意思是: asynchronous IO引发**asynchrony**的另外一个原因，因为IO完成的event是 独立于 main program flow的。
>
> 从control theory的角度来看，asynchronous event是不是由当前program控制的，它是由外部系统控制的，由外部系统进行通知。
>
> 



A common way for dealing with **asynchrony** in a [programming interface](https://infogalactic.com/info/Application_programming_interface) is to provide [subroutines](https://infogalactic.com/info/Subroutine) (methods, functions) that return to their caller an **object**, sometimes called a [future or promise](https://infogalactic.com/info/Futures_and_promises), that represents the **ongoing events**. Such an object will then typically come with a **synchronizing operation** that blocks until the operation is completed. Some programming languages, such as [Cilk](https://infogalactic.com/info/Cilk), have special syntax for expressing an asynchronous procedure call.[[2\]](https://infogalactic.com/info/Asynchrony_(computer_programming)#cite_note-spp-2)

> NOTE: 上面这段话对future/promise的介绍是非常好的，从asynchrony的角度非常容易理解引入"future or promise"的目的。



### Examples of asynchrony 



1) Ajax

> "[Ajax](https://infogalactic.com/info/Ajax_(programming))", short for "asynchronous [JavaScript](https://infogalactic.com/info/JavaScript) and [XML](https://infogalactic.com/info/XML)")[[3\]](https://infogalactic.com/info/Asynchrony_(computer_programming)#cite_note-3)[[4\]](https://infogalactic.com/info/Asynchrony_(computer_programming)#cite_note-4)[[5\]](https://infogalactic.com/info/Asynchrony_(computer_programming)#cite_note-garrett-5) is a set of [web development](https://infogalactic.com/info/Web_development) techniques utilizing many web technologies used on the [client-side](https://infogalactic.com/info/Client-side) to create asynchronous I/O [Web applications](https://infogalactic.com/info/Web_application).

2) Asynchronous method dispatch

> [Asynchronous method dispatch](https://infogalactic.com/info/Asynchronous_method_dispatch) (AMD), a data communication method used when there is a need for the server side to handle a large number of long lasting client requests.[[6\]](https://infogalactic.com/info/Asynchrony_(computer_programming)#cite_note-6)

3) Asynchronous method invocation

https://en.wikipedia.org/wiki/Asynchronous_method_invocation

4) Interrupt

今天在阅读 Wikipedia的[context switch](https://en.wikipedia.org/wiki/Context_switch) 的时候，其中提及了interrupt，显然，interrupt是一种典型的asynchronous方式。

5) Unix signal

参见工程Linux-OS的`Programming\IO\Blocking-and-non-blocking-VS-sync-and-async`章节。

6) Asynchronous IO

参见工程Linux-OS的`Programming\IO\IO-model`章节。



## Async to sync

本段的意思是: 将asynchronous API转换为synchronous API，因为在一些情况下，基于asynchronous + callback的模式是不方便使用的，

### Examples

下面是我遇到的一些将asynchronous operation转换为synchronize operation的例子: 

1、amust API，使用promise-future、condition variable



2、[tgockel](https://github.com/tgockel)/**[zookeeper-cpp](https://github.com/tgockel/zookeeper-cpp)**

> ### Synchronous API
>
> The C library offers both a synchronous and an asynchronous API. This library offers only an asynchronous version. If you prefer a synchronous API, call `get()` on the returned `future` to block until you receive the response.



3、[yyzybb537](https://github.com/yyzybb537)/**[libgo](https://github.com/yyzybb537/libgo)**

> Provide golang's General powerful protocol, write code based on coroutine, can write simple code in a synchronous manner, while achieving asynchronous performance.



### Blocking with timeout

显然，在没有收到响应之前，需要将caller阻塞，因此需要考虑的一个问题是: blocking with timeout，因为有可能一直无法收到响应，因此需要设置一个超时时间。



## Sync to async

1、典型的实现方式是asynchronous method invocation + message queue(mailbox) +  thread pool

例子包括:

a、libuv filesystem operation，参见 http://docs.libuv.org/en/v1.x/design.html