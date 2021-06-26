# Architecture of distributed computing

distributed computing是一个非常宽泛的概念，非常多的架构都可以归入到distributed computing的范轴中。本章梳理这些架构，需要注意的是，架构之间没有优劣之分，只有谁比谁更加适合于具体的问题。



## Client-server VS peer-to-peer

[Peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer) 和 [Client–server](https://en.wikipedia.org/wiki/Client%E2%80%93server_model) 这两种Architectures是 [Distributed computing application](https://en.wikipedia.org/wiki/Distributed_computing) 最最常见的两种architecture，有必要对这两种architecture进行对比。

在 wikipedia [Peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer) 中，一直拿他和 [Client–server](https://en.wikipedia.org/wiki/Client%E2%80%93server_model) 进行对比，同样 [Client–server](https://en.wikipedia.org/wiki/Client%E2%80%93server_model) 也专门总结了它和 [Peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer) 之间的差异。下面梳理一下设计到两者对比的内容的章节：

1、[Peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer)

在"[Peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer) # [Resilient and scalable computer networks](https://en.wikipedia.org/wiki/Peer-to-peer#Resilient_and_scalable_computer_networks)"小节中从 [single point of failure](https://en.wikipedia.org/wiki/Reliability_engineering) 中对比了两者。

2、[Client–server model](https://en.wikipedia.org/wiki/Client%E2%80%93server_model)

在 [Client–server model](https://en.wikipedia.org/wiki/Client%E2%80%93server_model) # [Comparison with peer-to-peer architecture](https://en.wikipedia.org/wiki/Client%E2%80%93server_model)中对两者进行了对比。这一段的内容给我的一个重要提示是：[Peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer)是 [decentralized system](https://en.wikipedia.org/wiki/Decentralized_system)，但是[Client–server architecture](https://en.wikipedia.org/wiki/Client%E2%80%93server_model)不是centralized system。



除此之外，在一些应用中，也能够看到对这两种架构的比较，比如：

1、git

在Progit book的[1.1 Getting Started - About Version Control](https://git-scm.com/book/en/v2/Getting-Started-About-Version-Control)中就对比了[Distributed version control](https://en.wikipedia.org/wiki/Distributed_version_control)和[Centralized version control](https://en.wikipedia.org/wiki/Version_control)。



## TODO P2P  VS shared nothing architecture

我觉得两种的一个共性是: decentralized。



## Centralized VS Decentralized

搜罗了一下，维基百科中关于这两者的文章有如下：

- wikipedia [Decentralized computing](https://en.wikipedia.org/wiki/Decentralized_computing)
- wikipedia [Centralized computing](https://en.wikipedia.org/wiki/Centralized_computing)
- wikipedia [Decentralised system](https://en.wikipedia.org/wiki/Decentralised_system)

上述文章中，[Decentralised system](https://en.wikipedia.org/wiki/Decentralised_system)是最最通俗易懂的，前两篇质量一般。