# Actor model 参与者模型

The **actor model** in [computer science](https://en.wikipedia.org/wiki/Computer_science) is a [mathematical model](https://en.wikipedia.org/wiki/Mathematical_model) of [concurrent computation](https://en.wikipedia.org/wiki/Concurrent_computation) that treats "**actors**" as the *universal primitives* of concurrent computation(其将“actors”视为并发计算的通用原语). In response to a [message](https://en.wikipedia.org/wiki/Message_(computing)) that it receives, an actor can: 

- make local decisions
- create more actors
- send more messages
- determine how to respond to the next message received.

**Actors** may modify their own [private state](https://en.wikipedia.org/wiki/Private_state), but can only affect each other through messages (avoiding the need for any [locks](https://en.wikipedia.org/wiki/Lock_(computer_science))).

The actor model originated in 1973.[[1\]](https://en.wikipedia.org/wiki/Actor_model#cite_note-1) It has been used both as a framework for a [theoretical understanding](https://en.wikipedia.org/wiki/Actor_model_theory)(理论上理解) of [computation](https://en.wikipedia.org/wiki/Concurrency_(computer_science)) and as the theoretical basis for several [practical implementations](https://en.wikipedia.org/wiki/Actor_model_implementation)(实际实现) of [concurrent systems](https://en.wikipedia.org/wiki/Concurrent_systems). The relationship of the model to other work is discussed in [Actor model and process calculi](https://en.wikipedia.org/wiki/Actor_model_and_process_calculi).



## Fundamental concepts

The **actor model** adopts the philosophy(哲学) that *everything is an actor*. This is similar to the *everything is an object* philosophy used by some [object-oriented programming languages](https://en.wikipedia.org/wiki/Object-oriented_programming).

An actor is a **computational entity** that, in response to a message it receives, can concurrently:

- send a finite number of messages to other actors;
- create a finite number of new actors;
- designate the behavior to be used for the next message it receives.

There is no assumed sequence to the above actions and they could be carried out in parallel.

Decoupling the sender from communications sent was a fundamental advance of the **Actor model** enabling [asynchronous communication](https://en.wikipedia.org/wiki/Asynchronous_communication) and control structures as patterns of [passing messages](https://en.wikipedia.org/wiki/Message_passing).[[8\]](https://en.wikipedia.org/wiki/Actor_model#cite_note-8)

Recipients of messages are identified by address, sometimes called "mailing address". Thus an actor can only communicate with actors whose addresses it has. It can obtain those from a message it receives, or if the address is for an actor it has itself created.

The **actor model** is characterized by inherent(固有的，内在的，天生的) concurrency of computation within(内部) and among(之间) actors, dynamic creation of actors, inclusion of actor addresses in messages, and interaction only through direct asynchronous [message passing](https://en.wikipedia.org/wiki/Message_passing) with no restriction on message arrival order.

actor模型的特征在于actor内部和之间的计算的内在并发性，actor的动态创建，在消息中包含actor地址，以及仅通过直接异步消息传递的交互，而不限制消息到达顺序。	

## Formal systems

Over the years, several different **formal systems** have been developed which permit reasoning(推理) about systems in the actor model. These include:

- [Operational semantics](https://en.wikipedia.org/wiki/Operational_semantics)[[3\]](https://en.wikipedia.org/wiki/Actor_model#cite_note-greif1975-3)[[9\]](https://en.wikipedia.org/wiki/Actor_model#cite_note-agha1993-9)
- Laws for Actor systems[[4\]](https://en.wikipedia.org/wiki/Actor_model#cite_note-baker1977-4)
- [Denotational semantics](https://en.wikipedia.org/wiki/Denotational_semantics)(指定语义)[[2\]](https://en.wikipedia.org/wiki/Actor_model#cite_note-clinger1981-2)[[10\]](https://en.wikipedia.org/wiki/Actor_model#cite_note-hewitt2006-10)
- Transition semantics(过渡语义)[[6\]](https://en.wikipedia.org/wiki/Actor_model#cite_note-agha1986-6)

There are also formalisms(形式主义) that are not fully faithful to the actor model in that they do not formalize the guaranteed delivery of messages including the following(还有一些形式主义并不完全忠实于actor model ，因为它们没有形式化保证的消息传递，包括以下内容) (See [Attempts to relate Actor semantics to algebra and linear logic](https://en.wikipedia.org/wiki/Actor_model_later_history#Attempts_to_relate_Actor_semantics_to_algebra_and_linear_logic)):

- Several different Actor algebras[[11\]](https://en.wikipedia.org/wiki/Actor_model#cite_note-gaspari1997-11)[[12\]](https://en.wikipedia.org/wiki/Actor_model#cite_note-gaspari1999-12)[[13\]](https://en.wikipedia.org/wiki/Actor_model#cite_note-agha2004-13)
- [Linear logic](https://en.wikipedia.org/wiki/Linear_logic)[[14\]](https://en.wikipedia.org/wiki/Actor_model#cite_note-darlington1994-14)

## Applications

The Actor model can be used as a framework for modeling, understanding, and reasoning about a wide range of [concurrent systems](https://en.wikipedia.org/wiki/Concurrent_systems). For example:

- [Electronic mail](https://en.wikipedia.org/wiki/Electronic_mail) (e-mail) can be modeled as an Actor system. Accounts are modeled as Actors and [email addresses](https://en.wikipedia.org/wiki/E-mail_address) as Actor addresses.
- [Web Services](https://en.wikipedia.org/wiki/Web_Services) can be modeled with [SOAP](https://en.wikipedia.org/wiki/SOAP) endpoints modeled as Actor addresses.
- Objects with [locks](https://en.wikipedia.org/wiki/Lock_(computer_science)) (*e.g.*, as in [Java](https://en.wikipedia.org/wiki/Java_(programming_language)) and [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language))) can be modeled as a **Serializer**, provided that their implementations are such that messages can continually arrive (perhaps by being stored in an internal [queue](https://en.wikipedia.org/wiki/Queue_(abstract_data_type))). A serializer is an important kind of Actor defined by the property that it is continually available to the arrival of new messages; every message sent to a serializer is guaranteed to arrive.
- Testing and Test Control Notation ([TTCN](https://en.wikipedia.org/wiki/TTCN)), both TTCN-2 and [TTCN-3](https://en.wikipedia.org/wiki/TTCN-3), follows actor model rather closely. In TTCN, Actor is a test component: either parallel test component (PTC) or main test component (MTC). Test components can send and receive messages to and from remote partners (peer test components or test system interface), the latter being identified by its address. Each test component has a behaviour tree bound to it; test components run in parallel and can be dynamically created by parent test components. Built-in language constructs allow the definition of actions to be taken when an expected message is received from the internal message queue, like sending a message to another peer entity or creating new test components.

总结：上面这段我的理解和文中的意思不同



## Message-passing semantics

### Unbounded nondeterminism controversy 无限的不确定性争议



### Direct communication and asynchrony 直接通信和异步

Messages in the actor model are not necessarily buffered(不一定是缓冲的). This was a sharp break with previous approaches to models of concurrent computation(这与以前的并发计算模型方法有着明显的突破). The lack of buffering caused a great deal of misunderstanding at the time of the development of the **actor model** and is still a controversial issue. Some researchers argued that the messages are buffered in the "ether" or the "environment". Also, messages in the actor model are simply sent (like [packets](https://en.wikipedia.org/wiki/Packet_(information_technology)) in [IP](https://en.wikipedia.org/wiki/Internet_Protocol)); there is no requirement for a **synchronous handshake**(同步握手) with the recipient.

### Actor creation plus addresses in messages means variable topology 在消息中创建Actor以及地址意味着变量拓扑

### Inherently concurrent 

### Actor libraries and frameworks

[CAF_C++ Actor Framework](http://actor-framework.org/)

