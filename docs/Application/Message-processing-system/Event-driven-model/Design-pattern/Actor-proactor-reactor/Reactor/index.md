# Reactor pattern

1、wikipedia [Reactor pattern](https://en.wikipedia.org/wiki/Reactor_pattern) 对 Reactor pattern的描述是非常正式的， 初次阅读是难以理解的

2、Linux IO multiplexing就是用于实现Reactor pattern的

3、使用Reactor pattern的例子:

a、zhihu [如何深刻理解reactor和proactor？](https://www.zhihu.com/question/26943938) # [A](https://www.zhihu.com/question/26943938/answer/35034068)

b、zhihu [I/O多路复用技术（multiplexing）是什么？](https://www.zhihu.com/question/28594409) # [A](https://www.zhihu.com/question/28594409/answer/52763082)

典型的例子是就是Redis ae，它的实现比较简单，结合它的实现来理解会非常快。

## wikipedia [Reactor pattern](https://en.wikipedia.org/wiki/Reactor_pattern)

The reactor [design pattern](https://en.wikipedia.org/wiki/Design_pattern_(computer_science)) is an [event handling](https://en.wikipedia.org/wiki/Event_handling) pattern for handling **service requests** delivered [concurrently](https://en.wikipedia.org/wiki/Concurrency_(computer_science)) to a **service handler** by one or more inputs. The **service handler** then [demultiplexes](https://en.wikipedia.org/wiki/Demultiplex) the incoming requests and dispatches them **synchronously** to the associated request handlers.[[1\]](https://en.wikipedia.org/wiki/Reactor_pattern#cite_note-1)

> NOTE: 
>
> 1、如果使用中文来进行理解的话，reactor就可以翻译为反应器，它是对发生的event进行react，即反映；由于这篇文章并没有列举示例，所以我在第一次阅读的时候并没有完全理解，今天在阅读《redis设计与实现》的第12章事件的时候，其中对此的解释是非常到位的，所以要想完全理解reactor pattern，需要结合这本书的内容一起来进行理解。
>
> 2、上文中的**synchronously**非常重要，它是reactor和[Proactor pattern](https://en.wikipedia.org/wiki/Proactor_pattern)之间的差异所在；
>
> 3、: 参见[Redis 设计与实现](http://redisbook.com/)，其中chapter 12文件事件[¶](http://redisbook.com/preview/event/file_event.html#id1)对reactor pattern进行了介绍；



### Structure

#### Resources

Any **resource** that can provide input to or consume output from the system.

>  NOTE: resource可以是socket，file等；

#### Synchronous Event Demultiplexer

Uses an [event loop](https://en.wikipedia.org/wiki/Event_loop) to block on all resources. The `demultiplexer` sends the resource to the dispatcher when it is possible to start a synchronous operation on a resource without blocking (*Example:* a synchronous call to `read()` will block if there is no data to read. The `demultiplexer` uses `select()` on the resource, which blocks until the resource is available for reading. In this case, a synchronous call to `read()` won't block, and the `demultiplexer` can send the resource to the dispatcher.)

> NOTE: 
>
> 1、上面这一段中的`Demultiplexer`其实是比较容易让人混淆的，它的实现往往是使用[I/O multiplex](I/O multiplex)技术，如[select](http://man7.org/linux/man-pages/man2/select.2.html)等。
>
> 2、其实上面这段话是非常任意让人混淆的，结合[Redis 设计与实现](http://redisbook.com/)，其中chapter 12文件事件[¶](http://redisbook.com/preview/event/file_event.html#id1)对reactor pattern的介绍，已经redis的实现是非常任意的；

#### Dispatcher

Handles registering and unregistering of **request handlers**. Dispatches resources from the demultiplexer to the associated request handler.

#### Request Handler

An application defined **request handler** and its associated resource.



### Properties

All reactor systems are single-threaded by definition, but can exist in a [multithreaded](https://en.wikipedia.org/wiki/Thread_(computer_science)) environment.

> NOTE: redis默认情况下就是single-thread



### Benefits

The **reactor pattern** completely separates application-specific code from the reactor implementation, which means that application components can be divided into modular, reusable parts.

### Limitations

The **reactor pattern** can be more difficult to [debug](https://en.wikipedia.org/wiki/Debugging)[[2\]](https://en.wikipedia.org/wiki/Reactor_pattern#cite_note-2) than a procedural pattern due to the inverted flow of control. Also, by only calling request handlers **synchronously**, the **reactor pattern** limits maximum concurrency, especially on [symmetric multiprocessing](https://en.wikipedia.org/wiki/Symmetric_multiprocessing) hardware. The scalability（可扩展性） of the reactor pattern is limited not only by calling request handlers synchronously, but also by the `demultiplexer`.[[3\]](https://en.wikipedia.org/wiki/Reactor_pattern#cite_note-3)



## TODO

https://www.zhihu.com/question/26943938

https://www.codeproject.com/Articles/33011/Proactor-Pattern

http://didawiki.cli.di.unipi.it/lib/exe/fetch.php/magistraleinformatica/tdp/tpd_reactor_proactor.pdf

https://www.boost.org/doc/libs/1_47_0/doc/html/boost_asio/overview/core/async.html