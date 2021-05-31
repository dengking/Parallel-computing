# Proactor and reactor

## Reactor VS Proactor 

参考文章:

一、zhihu [如何深刻理解reactor和proactor？](https://www.zhihu.com/question/26943938)

下面是我的总结:

相同点: 

1、两种都是基于IO multiplexing

不同点:

IO model: 

reactor sync同步IO-proactor async异步IO

reactor非阻塞同步IO-proactor非阻塞异步IO

reactor通知notify IO就绪状态-proactor通知notify IO完成状态



## stackoverflow [What is the difference between event driven model and reactor pattern? ](https://stackoverflow.com/questions/9138294/what-is-the-difference-between-event-driven-model-and-reactor-pattern)

From the wikipedia [Reactor Pattern](http://en.wikipedia.org/wiki/Reactor_pattern) article:

> The reactor design pattern is an event handling pattern for handling service requests delivered concurrently to a service handler by one or more inputs.

It named a few examples, e.g. `nodejs`, `twisted`, `eventmachine`

But what I understand that above is popular event driven framework, so make them also a reactor pattern framework?

How to differentiate between these two? Or they are the same?

### [A](https://stackoverflow.com/a/9143390)

The **reactor pattern** is more specific than "event driven programming". It is a specific implementation technique used when doing **event driven programming**. However, the term is not used with much accuracy in typical conversation, so you should be careful about using it and expecting your audience to understand you, and you should be careful in how you interpret the term when you encounter its use.

One way to look at the reactor pattern is to consider it closely related to the idea of "non-blocking" operations. The reactor sends out notifications when certain operations can be completed without blocking. For example, `select(2)` can be used to implement the **reactor pattern** for reading from and writing to sockets using the standard BSD socket APIs (`recv(2)`, `send(2)`, etc). `select` will tell you when you can receive bytes from a socket instantly - because the bytes are present in the kernel receiver buffer for that socket, for example.

Another pattern you might want to consider while thinking about these ideas is the ***proactor* pattern**. In contrast to the **reactor pattern**, the **proactor pattern** has operations start regardless of whether they can finish immediately or not, has them performed *asynchronously*, and then arranges to deliver notification about their **completion**.

The Windows I/O Completion Ports (IOCP) API is one example where the *proactor* pattern can be seen. When performing a send on a socket with IOCP, the send operation is started regardless of whether there is any room in the kernel send buffer for that socket. The send operation continues (in another thread, perhaps a thread in the kernel) while the `WSASend` call completes immediately. When the send *actually* completes (meaning only that the bytes being sent have been copied into the kernel send buffer for that socket), a callback function supplied to the `WSASend` call is invoked (in a new thread in the application).

This approach of starting operations and then being notified when they are complete is central to the idea of *asynchronous* operations. Compare it to *non-blocking* operations where you wait until an operation can complete immediately before attempting to perform it.

Either approach can be used for event driven programming. Using the reactor pattern, a program waits for the *event* of (for example) a socket being readable and then reads from it. Using the proactor pattern, the program instead waits for the *event* of a socket read completing.

Strictly speaking, Twisted misuses the term *reactor*. The Twisted reactor which is based on `select(2)` (`twisted.internet.selectreactor`) is implemented using non-blocking I/O, which is very reactor-like. However, the interface it exposes to application code is *asynchronous*, making it more proactor-like. Twisted also has a reactor based on IOCP. This reactor exposes the same asynchronous application-facing API **and** uses the proactor-like IOCP APIs. This hybrid approach, varying from platform to platform in its details, makes neither the term "reactor" nor "proactor" particularly accurate, but since the API exposed by `twisted.internet.reactor` is basically entirely asynchronous instead of non-blocking, *proactor* would probably have been a better choice of name.



### [A](https://stackoverflow.com/a/38971145)

I think that this separation "non-blocking" and "asynchronous" is wrong, as the main implication of "asynchronous" is "non-blocking". Reactor pattern is about asynchronous (so non-blocking) calls, but synchronous (blocking) processing of those calls. Proactor is about asynchronous (non-blocking) calls and asynchronous (non-blocking) processing of those calls.