# Proactor and Reactor

每个模式中都有一个`Initiator`

reactor的同步是指什么？

Proactor的异步是指什么？启动一个操作，等操作完成后，在执行回调函数



两者都涉及了call back



## zhihu [如何深刻理解reactor和proactor？](https://www.zhihu.com/question/26943938)





### [A](https://www.zhihu.com/question/26943938/answer/68773398)



1、标准定义

两种I/O多路复用模式：Reactor和Proactor



一般地,I/O多路复用机制都依赖于一个事件**多路分离器(Event Demultiplexer)**。**分离器对象**可将来自事件源的I/O事件分离出来，并分发到对应的**read/write事件处理器(Event Handler)**。开发人员预先注册需要处理的事件及其事件处理器（或回调函数）；事件分离器负责将请求事件传递给事件处理器。



**两个与事件分离器有关的模式是Reactor和Proactor。Reactor模式采用同步IO，而Proactor采用异步IO。**



在**Reactor**中，**事件分离器**负责等待文件描述符或socket为读写操作准备就绪，然后将就绪事件传递给对应的处理器，最后由**处理器**负责完成实际的读写工作。

**而在Proactor模式中，**处理器--或者兼任处理器的**事件分离器**，只负责发起异步读写操作。IO操作本身由**操作系统**来完成。传递给操作系统的参数需要包括用户定义的**数据缓冲区地址**和**数据大小**，操作系统才能从中得到写出操作所需数据，或写入从socket读到的数据。**事件分离器**捕获**IO操作完成事件**，然后将事件传递给对应处理器。比如，在windows上，处理器发起一个异步IO操作，再由事件分离器等待IOCompletion事件。典型的异步模式实现，都建立在操作系统支持异步API的基础之上，我们将这种实现称为“系统级”异步或“真”异步，因为应用程序完全依赖操作系统执行真正的IO工作。


举个例子，将有助于理解Reactor与Proactor二者的差异，以读操作为例（类操作类似）。
**在Reactor中实现读：**

\- 注册读就绪事件和相应的事件处理器
\- 事件分离器等待事件
\- 事件到来，激活分离器，分离器调用事件对应的处理器。
\- 事件处理器完成实际的读操作，处理读到的数据，注册新的事件，然后返还控制权。
**在Proactor中实现读：**

\- **处理器**发起异步读操作（注意：操作系统必须支持异步IO）。在这种情况下，处理器无视IO就绪事件，它关注的是完成事件。
\- 事件分离器等待操作完成事件
\- 在分离器等待过程中，操作系统利用并行的内核线程执行实际的读操作，并将结果数据存入用户自定义缓冲区，最后通知事件分离器读操作完成。
\- 事件分离器呼唤**处理器**。
\- 事件处理器处理用户自定义缓冲区中的数据，然后启动一个新的异步操作，并将控制权返回事件分离器。

可以看出，两个模式的相同点，都是对某个IO事件的事件通知(即告诉某个模块，这个IO操作可以进行或已经完成)。在结构上，两者也有相同点：demultiplexor负责提交IO操作(异步)、查询设备是否可操作(同步)，然后当条件满足时，就回调handler；不同点在于，异步情况下(Proactor)，当回调handler时，表示IO操作已经完成；同步情况下(Reactor)，回调handler时，表示IO设备可以进行某个操作(can read or can write)。

2、通俗理解

使用Proactor框架和Reactor框架都可以极大的简化网络应用的开发，但它们的重点却不同。

Reactor框架中用户定义的操作是在实际操作之前调用的。比如你定义了操作是要向一个SOCKET写数据，那么当该SOCKET可以接收数据的时候，你的操作就会被调用；**而Proactor框架中用户定义的操作是在实际操作之后调用的。比如你定义了一个操作要显示从SOCKET中读入的数据，那么当读操作完成以后，你的操作才会被调用。**

**Proactor和Reactor都是并发编程中的设计模式。**在我看来，他们都是用于派发/分离IO操作事件的。这里所谓的IO事件也就是诸如read/write的IO操作。"派发/分离"就是将单独的IO事件通知到上层模块。两个模式不同的地方在于，**Proactor用于异步IO，而Reactor用于同步IO。**

部分参考自

```text
http://www.cnblogs.com/dawen/archive/2011/05/18/2050358.html
```

### [A](https://www.zhihu.com/question/26943938/answer/35007092)

reactor：能收了你跟俺说一声。
proactor: 你给我收十个字节，收好了跟俺说一声。



### [A](https://www.zhihu.com/question/26943938/answer/35034068)

Reactor: 
libevent/libev/libuv/ZeroMQ/Event Library in Redis

Proactor
IOCP/Boost.Asio

linux下还是Reactor把, 没有os支持, Proactor玩不转.

