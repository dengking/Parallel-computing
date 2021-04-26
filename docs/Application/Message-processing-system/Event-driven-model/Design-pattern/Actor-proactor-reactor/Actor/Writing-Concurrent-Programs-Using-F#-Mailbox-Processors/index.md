# [Writing Concurrent Programs Using F# Mailbox Processors](https://www.codemag.com/article/1707051)

> NOTE: 
>
> 1、这篇文章关于actor model的解释是非常好的，正是这个原因，才收录了这篇文章
>
> 2、`F#` Mailbox Processors 其实就是actor model中的actor

There are many approaches to concurrency, but in this issue, I’d like to talk about the built-in way that `F#` can handle **message-based concurrency**: using `F#` mailbox processors.

## The Actor Model

To understand `F#` mailbox processors better, let’s look a little closer at the theory they’re based on: the actor model, which is also a prominent part of the **Erlang programming language**. There are a few key parts of the theory that can help you know when and why you should be using mailbox processors.

The Actor Model is a model of concurrent programming that uses **actors** as the base unit. These actors are lightweight constructs that contain a queue, and can receive and process messages. 

> NOTE: 
>
> 1、到处都可以看到message queue
>
> 2、既然使用了message queue，那么它肯定是 asynchronous

When I say lightweight, I mean lightweight—they’re not like threads. I can easily spin up 100,000 on my aging laptop without a hitch. An actor’s behavior is very limited. They can only:

1、Create more actors

2、Determine what to do with the next incoming message

3、Send messages to another actor

There are also a few rules for the actor model itself:

1、Actors can be created dynamically.

2、The only interaction between actors is through direct asynchronous message passing.

3、Messages must include the actor’s address.

4、There’s no restriction on message arrival order.

A **mailbox processor** is based on a single actor. However, in general, you’ll want to create several actors as part of a system. Even one of the creators of the actor model, Carl Hewitt, has famously said, "One actor is no actor. They come in systems." 

### Erlang actor analogy 

> NOTE: 
>
> 1、下面的类比是非常好的

I mentioned Erlang above as the first, and most faithful, representation of the actor model, because this gives you a good idea how to think about a whole system of actors. Erlang was developed for telephony applications, and you can think about a telephone as a single actor that can send messages (or make calls) to another telephone, as long as you have an address (telephone number). This analogy gives you a good working idea of how to build systems of actors, or mailbox processors.

### Actor, mailbox processor, and agent 

> NOTE: 
>
> 1、三者其实是相同的含义。

Here’s one more note before I jump into code. The original model is called the actor model and uses actors. F#’s implementation of actors is referred to as mailbox processors, as I’ve mentioned. Actors and mailbox processors are also occasionally referred to as agents. For most uses, these terms are interchangeable.

> For most purposes, the terms actor, mailbox processor, and agent are interchangeable.

> 