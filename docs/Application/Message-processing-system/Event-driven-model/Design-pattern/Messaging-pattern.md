# wikipedia [Messaging pattern](https://en.wikipedia.org/wiki/Messaging_pattern)

In [software architecture](https://en.wikipedia.org/wiki/Software_architecture), a **messaging pattern** is a network-oriented [architectural pattern](https://en.wikipedia.org/wiki/Architectural_pattern) which describes how two different parts of a [message passing](https://en.wikipedia.org/wiki/Message_passing) system connect and communicate with each other.

> NOTE : 上面这段话对message的定义是局限于network的，其实我觉得我们需要从更加宽泛的角度来认识message，而不是局限在network；在我们的programming中，其实很多时候都涉及到了通过message来通知，传递，有的时候，它们并不是network间，可能就是在process内（如condition variable（参见youdao notebook的《`cppreference-std-condition_variable.md`》），或者[message  queue](https://en.wikipedia.org/wiki/Message_queue)）；所以对message的认知要拓宽；在youdao notebook的《`event-and-message-passing-summary.md`》中对message进行了综述；其实本文则从另外一个角度来进行阐述：message的pattern；的确，仅仅认知message是完全不能够将其应用起来解决实际问题的，还需要根据问题采取合适的架构，这就涉及message pattern了；所以认知到messaging pattern对于理解一些应用是非常重要的；

In [telecommunications](https://en.wikipedia.org/wiki/Telecommunications), a **message exchange pattern** (**MEP**) describes the [pattern](https://en.wikipedia.org/wiki/Pattern) of [messages](https://en.wikipedia.org/wiki/Message_passing) required by a [communications protocol](https://en.wikipedia.org/wiki/Communications_protocol) to establish or use a [communication channel](https://en.wikipedia.org/wiki/Communication_channel). There are two major message exchange patterns — a *request–response* pattern, and a *one-way*（单程的） pattern. For example, [HTTP](https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol) is a *request–response* pattern protocol, and [UDP](https://en.wikipedia.org/wiki/User_Datagram_Protocol) is a *one-way* pattern.[[1\]](https://en.wikipedia.org/wiki/Messaging_pattern#cite_note-1)

> NOTE: 本文所描述的messaging pattern是建立在network protocol的基础上而构建的更加高级的功能，需要将它和比较底层的network protocol区分开来；

## ØMQ

The [ØMQ](https://en.wikipedia.org/wiki/%C3%98MQ) message queueing library provides so-called *sockets* (a kind of generalization over the traditional [IP](https://en.wikipedia.org/wiki/Internet_socket) and [Unix sockets](https://en.wikipedia.org/wiki/Unix_domain_socket)) which require indicating a **messaging pattern** to be used, and are optimized for each pattern. The basic ØMQ patterns are:[[4\]](https://en.wikipedia.org/wiki/Messaging_pattern#cite_note-4)

1、**Request–reply** connects a set of clients to a set of services. This is a [remote procedure call](https://en.wikipedia.org/wiki/Remote_procedure_call) and task distribution pattern.[*clarification needed*]

2、**Publish–subscribe** connects a set of publishers to a set of subscribers. This is a data distribution pattern.[*clarification needed*]

3、**Push–pull** connects nodes in a [fan-out](https://en.wikipedia.org/wiki/Fan-out_(software)) / fan-in pattern that can have multiple steps, and loops. This is a parallel task distribution and collection pattern.[*clarification needed*]

> NOTE: 并没有搞懂

4、**Exclusive pair** connects two sockets in an exclusive pair. This is a low-level pattern for specific, advanced use cases.

> NOTE: 并没有搞懂

Each pattern defines a particular **network topology**. **Request-reply** defines so-called "service bus", **publish-subscribe** defines "data distribution tree", **push-pull** defines "parallelised pipeline". All the patterns are deliberately designed in such a way as to be infinitely scalable and thus usable on Internet scale.[[5\]](https://en.wikipedia.org/wiki/Messaging_pattern#cite_note-5)

> NOTE: 最后一段话非常好；