# Actor model

下面是解决Actor model非常好的文章:

1、codemag [Writing Concurrent Programs Using F# Mailbox Processors](https://www.codemag.com/article/1707051)

2、brianstorti [The actor model in 10 minutes](https://www.brianstorti.com/the-actor-model/)

3、TODO rocketeer [Concurrency in Erlang & Scala: The Actor Model](https://rocketeer.be/articles/concurrency-in-erlang-scala/)

## 使用了Actor model的

### Erlang programming language and actor model

关于 Erlang programming language and actor model，在下面文章中进行描述: 

1、codemag [Writing Concurrent Programs Using F# Mailbox Processors](https://www.codemag.com/article/1707051)

2、brianstorti [The actor model in 10 minutes](https://www.brianstorti.com/the-actor-model/)

3、TODO rocketeer [Concurrency in Erlang & Scala: The Actor Model](https://rocketeer.be/articles/concurrency-in-erlang-scala/)

### Python celery

> Instantiation[¶](http://docs.celeryproject.org/en/latest/userguide/tasks.html#instantiation)
>
> A task is **not** instantiated for every request, but is registered in the **task registry** as a **global** instance.
>
> This means that the `__init__` constructor will only be called once per **process**, and that the **task class** is semantically closer to an **Actor**.

## wikipedia [Actor model](http://en.wikipedia.org/wiki/Actor_model) 

> NOTE: 参与者模型

The **actor model** in [computer science](https://en.wikipedia.org/wiki/Computer_science) is a [mathematical model](https://en.wikipedia.org/wiki/Mathematical_model) of [concurrent computation](https://en.wikipedia.org/wiki/Concurrent_computation) that treats "**actors**" as the *universal primitives* of concurrent computation(其将“actors”视为并发计算的通用原语). In response to a [message](https://en.wikipedia.org/wiki/Message_(computing)) that it receives, an actor can: 

1、make local decisions

2、create more actors

3、send more messages

4、determine how to respond to the next message received.

**Actors** may modify their own [private state](https://en.wikipedia.org/wiki/Private_state), but can only affect each other through messages (avoiding the need for any [locks](https://en.wikipedia.org/wiki/Lock_(computer_science))).

The actor model originated in 1973.[[1\]](https://en.wikipedia.org/wiki/Actor_model#cite_note-1) It has been used both as a framework for a [theoretical understanding](https://en.wikipedia.org/wiki/Actor_model_theory)(理论上理解) of [computation](https://en.wikipedia.org/wiki/Concurrency_(computer_science)) and as the theoretical basis for several [practical implementations](https://en.wikipedia.org/wiki/Actor_model_implementation)(实际实现) of [concurrent systems](https://en.wikipedia.org/wiki/Concurrent_systems). The relationship of the model to other work is discussed in [Actor model and process calculi](https://en.wikipedia.org/wiki/Actor_model_and_process_calculi).



## Fundamental concepts

The **actor model** adopts the philosophy(哲学) that *everything is an actor*. This is similar to the *everything is an object* philosophy used by some [object-oriented programming languages](https://en.wikipedia.org/wiki/Object-oriented_programming).

> NOTE:
>
> 1、让我想到了Unix "everything is a file philosophy"

An actor is a **computational entity** that, in response to a message it receives, can concurrently:

- send a finite number of messages to other actors;
- create a finite number of new actors;
- designate the behavior to be used for the next message it receives.

There is no assumed sequence to the above actions and they could be carried out in parallel.

Decoupling the sender from communications sent was a fundamental advance of the **Actor model** enabling [asynchronous communication](https://en.wikipedia.org/wiki/Asynchronous_communication) and control structures as patterns of [passing messages](https://en.wikipedia.org/wiki/Message_passing).[[8\]](https://en.wikipedia.org/wiki/Actor_model#cite_note-8)

Recipients(接收) of messages are identified by address, sometimes called "mailing address". Thus an actor can only communicate with actors whose addresses it has. It can obtain those from a message it receives, or if the address is for an actor it has itself created.

The **actor model** is characterized by inherent(固有的，内在的，天生的) concurrency of computation within(内部) and among(之间) actors, dynamic creation of actors, inclusion of actor addresses in messages, and interaction only through direct asynchronous [message passing](https://en.wikipedia.org/wiki/Message_passing) with no restriction on message arrival order.

> actor模型的特征在于actor内部和之间的计算的内在并发性，actor的动态创建，在消息中包含actor地址，以及仅通过直接异步消息传递的交互，而不限制消息到达顺序。	



