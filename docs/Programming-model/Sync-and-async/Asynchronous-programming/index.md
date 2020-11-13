# Asynchronous programming

今天在阅读jQuery的ajax的代码的时候，看到了其中有非常多的**回调函数**等，显然这是**异步编程**的思想，于是我想，有必要总结Asynchronous  programming的相关知识了:



## Examples

[Ajax](https://infogalactic.com/info/Ajax_(programming))



## Programming model

### Fluent API



## Asynchronous programming and multithreading



https://stackoverflow.com/questions/34680985/what-is-the-difference-between-asynchronous-programming-and-multithreading

https://codewala.net/2015/07/29/concurrency-vs-multi-threading-vs-asynchronous-programming-explained/asynchronous 

## Asynchronous programming patterns

asynchronous programming paradigm

asynchronous programming javascript

https://eloquentjavascript.net/11_async.html

## Asynchronous programming python

https://realpython.com/async-io-python/

https://dbader.org/blog/understanding-asynchronous-programming-in-python

https://stackoverflow.com/questions/3221314/asynchronous-programming-in-python

https://xph.us/2009/12/10/asynchronous-programming-in-python.html

https://stackoverflow.com/questions/2625493/asynchronous-vs-non-blocking

https://stackoverflow.com/questions/7931537/whats-the-difference-between-asynchronous-non-blocking-event-base-architectu/9489547#9489547



[Now that Python has coroutines (e.g. async/await), does multi-threading still have a use?](https://www.reddit.com/r/Python/comments/5all7u/now_that_python_has_coroutines_eg_asyncawait_does/)([self.Python](https://www.reddit.com/r/Python/))

这篇文章提及了IO bound和CPU bound，这对于选取concurrency方式非常重要；

## Asynchronous method invocation

https://en.wikipedia.org/wiki/Asynchronous_method_invocation

## Unix signal

 在APUE的10.2 Signal Concepts中有下面这样的一段话：

Signals are classic examples of **asynchronous events**. They occur at what appear to be random times to the process. The process can’t simply test a variable (such as errno) to see whether a signal has occurred; instead, the process has to tell the kernel ‘‘if and when this signal occurs, do the following.’’We can tell the kernel to do one of three things when a signal occurs. We call this the disposition of the signal, or the action associated with a signal.

Unix的signal的asynchronous event的处理是通过注册signal handler来实现的；注册的signal handler的接口有



## Draft



asynchronous的思想是: 对于耗时操作，不等待它执行完成，让它在**main loop/flow **之外独立运行而不阻塞**main loop**，这样main loop就可以在它执行的这段时间内去处理其他的事情，待耗时操作完成的时候，它再**通知**main loop它的完成，然后main loop再来处理它的结果。

当进行**asynchronous programming**的时候，往往需要表达"当 某个**event** 发生时，执行 某个 **callback**"，这个**callback**往往是用户注册的自定义函数，这种是可以使用fluent API来进行描述的，它其实非常类似于builder pattern，即由用户来设置**callback**，最最典型的就是jQuery，现代很多programming language都是支持这种paradigm的。