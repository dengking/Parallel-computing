# Futures and promises



## dist-prog-book [Futures and Promises](http://dist-prog-book.com/chapter/2/futures.html)

> NOTE: 这篇文章收录在`Distributed-computing\Book-Programming-Models-for-Distributed-Computing\chapter-2`章节。

## wikipedia [Futures and promises](https://en.wikipedia.org/wiki/Futures_and_promises)

In [computer science](https://en.wikipedia.org/wiki/Computer_science), **future**, **promise**, **delay**, and **deferred** refer to constructs used for [synchronizing](https://en.wikipedia.org/wiki/Synchronization_(computer_science)) program [execution](https://en.wikipedia.org/wiki/Execution_(computing)) in some [concurrent programming languages](https://en.wikipedia.org/wiki/Concurrent_programming_language). They describe an object that acts as a **proxy** for a result that is initially unknown, usually because the [computation](https://en.wikipedia.org/wiki/Computation) of its value is yet incomplete.

> NOTE: 上面所提及的proxy正是[Proxy pattern](https://en.wikipedia.org/wiki/Proxy_pattern)的用法--Virtual Proxy

The term *promise* was proposed in 1976 by [Daniel P. Friedman](https://en.wikipedia.org/wiki/Daniel_P._Friedman) and David Wise,[[1\]](https://en.wikipedia.org/wiki/Futures_and_promises#cite_note-1) and Peter Hibbard called it *eventual*.[[2\]](https://en.wikipedia.org/wiki/Futures_and_promises#cite_note-2) A somewhat similar concept *future* was introduced in 1977 in a paper by [Henry Baker](https://en.wikipedia.org/wiki/Henry_Baker_(computer_scientist)) and [Carl Hewitt](https://en.wikipedia.org/wiki/Carl_Hewitt).[[3\]](https://en.wikipedia.org/wiki/Futures_and_promises#cite_note-3)

The terms *future*, *promise*, *delay*, and *deferred* are often used interchangeably（互换）, although some differences in usage between *future* and *promise* are treated below. Specifically, when usage is distinguished, a **future** is a *read-only* placeholder view of a variable, while a **promise** is a writable, [single assignment](https://en.wikipedia.org/wiki/Single_assignment) container which sets the value of the future（promise用于设置future的值）.[[4\]](https://en.wikipedia.org/wiki/Futures_and_promises#cite_note-4) Notably, a future may be defined without specifying which specific **promise** will set its value, and different possible **promises** may set the value of a given **future**, though this can be done only once for a given **future**. In other cases a future and a promise are created together and associated with each other: the **future** is the value, the **promise** is the function that sets the value – essentially the return value (future) of an asynchronous function (promise). Setting the value of a future is also called *resolving*, *fulfilling*, or *binding* it.

> NOTE: 这段话对future和promise的解释非常到位。

### Applications

Futures and promises originated in [functional programming](https://en.wikipedia.org/wiki/Functional_programming) and related paradigms (such as [logic programming](https://en.wikipedia.org/wiki/Logic_programming)) to decouple a value (a future) from how it was computed (a promise), allowing the computation to be done more flexibly, notably by parallelizing it. Later, it found use in [distributed computing](https://en.wikipedia.org/wiki/Distributed_computing), in reducing the latency from communication round trips. Later still, it gained more use by allowing writing asynchronous programs in [direct style](https://en.wikipedia.org/wiki/Direct_style), rather than in [continuation-passing style](https://en.wikipedia.org/wiki/Continuation-passing_style).

### Implicit vs. explicit

Use of futures may be *implicit* (any use of the future automatically obtains its value, as if it were an ordinary [reference](https://en.wikipedia.org/wiki/Reference_(programming))) or *explicit* (the user must call a function to obtain the value, such as the `get` method of `java.util.concurrent.Future` or `java.util.concurrent.CompletableFuture` in [Java](https://en.wikipedia.org/wiki/Java_(programming_language))). Obtaining the value of an explicit future can be called *stinging* or *forcing*. Explicit futures can be implemented as a library, whereas implicit futures are usually implemented as part of the language.

The original Baker and Hewitt paper described **implicit futures**, which are naturally supported in the [actor model](https://en.wikipedia.org/wiki/Actor_model) of computation and pure [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming) languages like [Smalltalk](https://en.wikipedia.org/wiki/Smalltalk). The Friedman and Wise paper described only **explicit futures**(弗里德曼和怀斯论文仅描述了**explicit futures**), probably reflecting the difficulty of efficiently implementing **implicit futures** on stock hardware. The difficulty is that stock hardware does not deal with futures for primitive data types like integers. For example, an add instruction does not know how to deal with `3 + future factorial(100000)`. In pure actor or object languages this problem can be solved by sending `future factorial(100000)` the message `+[3]`, which asks the future to add `3` to itself and return the result. Note that the message passing approach works regardless of when `factorial(100000)` finishes computation and that no stinging/forcing is needed.

### Promise pipelining

> NOTE: 此处的pipeline的含义是将多个promise连接起来。

The use of futures can dramatically reduce [latency](https://en.wikipedia.org/wiki/Latency_(engineering)) in [distributed systems](https://en.wikipedia.org/wiki/Distributed_computing). For instance, futures enable *promise pipelining*,[[5\]](https://en.wikipedia.org/wiki/Futures_and_promises#cite_note-5)[[6\]](https://en.wikipedia.org/wiki/Futures_and_promises#cite_note-6) as implemented in the languages [E](https://en.wikipedia.org/wiki/E_(programming_language)) and [Joule](https://en.wikipedia.org/wiki/Joule_(programming_language)), which was also called *call-stream*[[7\]](https://en.wikipedia.org/wiki/Futures_and_promises#cite_note-SIGPLAN_pp._260-7) in the language [Argus](https://en.wikipedia.org/wiki/Argus_(programming_language)).

总结：上面这段中的stream的含义是流，其实就是一个串。

Consider an expression involving conventional [remote procedure calls](https://en.wikipedia.org/wiki/Remote_procedure_call), such as:

```
 t3 := ( x.a() ).c( y.b() )
```

which could be expanded to

```E
 t1 := x.a();
 t2 := y.b();
 t3 := t1.c(t2);
```

Each statement needs a message to be sent and a reply received before the next statement can proceed. Suppose, for example, that `x`, `y`, `t1`, and `t2` are all located on the same remote machine. In this case, two complete network round-trips to that machine must take place before the third statement can begin to execute. The third statement will then cause yet another round-trip to the same remote machine.

Using futures, the above expression could be written

```E
 t3 := (x <- a()) <- c(y <- b())
```

which could be expanded to

```E
 t1 := x <- a();
 t2 := y <- b();
 t3 := t1 <- c(t2);
```

The syntax used here is that of the language E, where `x <- a()` means to send the message `a()` asynchronously to `x`. All three variables are immediately assigned futures for their results, and execution proceeds to subsequent statements. Later attempts to resolve the value of `t3` may cause a **delay**（思考：此处的delay如何来理解）; however, **pipelining** can reduce the number of round-trips needed(pipelining能够减少round-trip). If, as in the prior example, `x`, `y`, `t1`, and `t2` are all located on the same remote machine, a pipelined implementation can compute `t3` with one round-trip instead of three. Because all three messages are destined for objects which are on the same remote machine, only one request need be sent and only one response need be received containing the result. Note also that the send `t1 <- c(t2)` would not block even if `t1` and `t2` were on different machines to each other, or to `x` or `y`.

> NOTE: 上面这段话对比了是否使用future所带来的优势。

**Promise pipelining** should be distinguished from **parallel asynchronous message passing**. In a system supporting parallel message passing but not pipelining, the message sends `x <- a()` and `y <- b()` in the above example could proceed in parallel, but the send of `t1 <- c(t2)` would have to wait until both `t1` and `t2` had been received, even when `x`, `y`, `t1`, and `t2` are on the same remote machine. The relative latency advantage of pipelining becomes even greater in more complicated situations involving many messages.

Promise pipelining also should not be confused with [pipelined message processing](https://en.wikipedia.org/wiki/Actor_model#No_requirement_on_order_of_message_arrival) in actor systems, where it is possible for an actor to specify and begin executing a behaviour for the next message before having completed processing of the current message.

### Read-only views

In some programming languages such as [Oz](https://en.wikipedia.org/wiki/Oz_(programming_language)), [E](https://en.wikipedia.org/wiki/E_(programming_language)), and [AmbientTalk](https://en.wikipedia.org/wiki/AmbientTalk), it is possible to obtain a *read-only view* of a future, which allows reading its value when resolved, but does not permit resolving it:

> NOTE: 只有当promise已经完成resolve的情况下，才允许read它的value，否则不允许去resolve它。

- In [C++11](https://en.wikipedia.org/wiki/C%2B%2B11) a `std::future` provides a read-only view. The value is set directly by using a `std::promise`, or set to the result of a function call using `std::packaged_task` or `std::async`.



## Futures and promises originated in [functional programming](https://en.wikipedia.org/wiki/Functional_programming) 

关于functional programming，参见工程programming-language的`Theory\Programming-paradigm\Functional-programming`章节。