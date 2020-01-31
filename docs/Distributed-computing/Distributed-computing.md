# [Distributed computing](https://en.wikipedia.org/wiki/Distributed_computing)

**Distributed computing** is a field of [computer science](https://en.wikipedia.org/wiki/Computer_science) that studies distributed systems. A *distributed system* is a system whose components are located on different [networked computers](https://en.wikipedia.org/wiki/Computer_network), which communicate and coordinate their actions by [passing messages](https://en.wikipedia.org/wiki/Message_passing) to one another. The components interact with one another in order to achieve a common goal. 

Three significant characteristics of distributed systems are: 

- concurrency of components
- [lack of a global clock](https://en.wikipedia.org/wiki/Clock_synchronization)
- independent failure of components.

Examples of distributed systems vary from [SOA-based systems](https://en.wikipedia.org/wiki/Service-oriented_architecture) to [massively multiplayer online games](https://en.wikipedia.org/wiki/Massively_multiplayer_online_game) to [peer-to-peer applications](https://en.wikipedia.org/wiki/Peer-to-peer).

A [computer program](https://en.wikipedia.org/wiki/Computer_program) that runs within a distributed system is called a **distributed program** (and distributed programming is the process of writing such programs). There are many different types of implementations for the message passing mechanism, including pure HTTP, [RPC-like](https://en.wikipedia.org/wiki/Remote_procedure_call) connectors and [message queues](https://en.wikipedia.org/wiki/Message-oriented_middleware). 

*Distributed computing* also refers to the use of distributed systems to solve computational problems. In *distributed computing*, a problem is divided into many tasks, each of which is solved by one or more computers, which communicate with each other via message passing. 

> NOTE: 上面这段的解释非常类似于[parallel computing](https://en.wikipedia.org/wiki/Parallel_computing)。



## [Parallel and distributed computing](https://en.wikipedia.org/wiki/Distributed_computing#Parallel_and_distributed_computing)

Distributed systems are groups of **networked computers** which share a common goal for their work. The terms "[concurrent computing](https://en.wikipedia.org/wiki/Concurrent_computing)", "[parallel computing](https://en.wikipedia.org/wiki/Parallel_computing)", and "distributed computing" have a lot of overlap, and no clear distinction exists between them. The same system may be characterized both as "parallel" and "distributed"; the processors in a typical distributed system run concurrently in parallel. **Parallel computing** may be seen as a particular tightly coupled form of **distributed computing**, and **distributed computing** may be seen as a loosely coupled form of **parallel computing**. Nevertheless, it is possible to roughly classify concurrent systems as "parallel" or "distributed" using the following criteria:

- In parallel computing, all processors may have access to a [shared memory](https://en.wikipedia.org/wiki/Shared_memory_architecture) to exchange information between processors. 
- In distributed computing, each processor has its own private memory ([distributed memory](https://en.wikipedia.org/wiki/Distributed_memory)). Information is exchanged by passing messages between the processors. 



![img](https://upload.wikimedia.org/wikipedia/commons/thumb/c/c6/Distributed-parallel.svg/300px-Distributed-parallel.svg.png)

(a), (b): a distributed system.
(c): a parallel system.

The figure on the right illustrates the difference between distributed and parallel systems. Figure (a) is a schematic view of a typical distributed system; the system is represented as a network topology in which each node is a computer and each line connecting the nodes is a communication link. Figure (b) shows the same distributed system in more detail: each computer has its own local memory, and information can be exchanged only by passing messages from one node to another by using the available communication links. Figure (c) shows a parallel system in which each processor has a direct access to a shared memory.

The situation is further complicated by the traditional uses of the terms parallel and distributed *algorithm* that do not quite match the above definitions of parallel and distributed *systems* (see [below](https://en.wikipedia.org/wiki/Distributed_computing#Theoretical_foundations) for more detailed discussion). Nevertheless, as a rule of thumb, high-performance parallel computation in a shared-memory multiprocessor uses parallel algorithms while the coordination of a large-scale distributed system uses distributed algorithms. 

## [Architectures](https://en.wikipedia.org/wiki/Distributed_computing#Architectures)

Various hardware and software architectures are used for distributed computing. At a lower level, it is necessary to interconnect multiple CPUs with some sort of network, regardless of whether that network is printed onto a circuit board or made up of loosely coupled devices and cables. At a higher level, it is necessary to interconnect [processes](https://en.wikipedia.org/wiki/Process_(computing)) running on those CPUs with some sort of [communication system](https://en.wikipedia.org/wiki/Communication_system). 

Distributed programming typically falls into one of several basic architectures: [client–server](https://en.wikipedia.org/wiki/Client–server_model), [three-tier](https://en.wikipedia.org/wiki/Three-tier_(computing)), [*n*-tier](https://en.wikipedia.org/wiki/Multitier_architecture), or [peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer); or categories: [loose coupling](https://en.wikipedia.org/wiki/Loose_coupling), or [tight coupling](https://en.wikipedia.org/wiki/Computer_cluster). 

- [Client–server](https://en.wikipedia.org/wiki/Client–server_model): architectures where smart clients contact the server for data then format and display it to the users. Input at the client is committed back to the server when it represents a permanent change.
- [Three-tier](https://en.wikipedia.org/wiki/Three-tier_(computing)): architectures that move the client intelligence to a middle tier so that [stateless](https://en.wikipedia.org/wiki/Stateless_protocol) clients can be used. This simplifies application deployment. Most web applications are three-tier.
- [*n*-tier](https://en.wikipedia.org/wiki/Multitier_architecture): architectures that refer typically to web applications which further forward their requests to other enterprise services. This type of application is the one most responsible for the success of [application servers](https://en.wikipedia.org/wiki/Application_server).
- [Peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer): architectures where there are no special machines that provide a service or manage the network resources. Instead all responsibilities are uniformly divided among all machines, known as peers. Peers can serve both as clients and as servers. Examples of this architecture include [BitTorrent](https://en.wikipedia.org/wiki/BitTorrent) and the [bitcoin network](https://en.wikipedia.org/wiki/Bitcoin_network).

Another basic aspect of distributed computing architecture is the method of communicating and coordinating work among concurrent processes. Through various message passing protocols, processes may communicate directly with one another, typically in a [master/slave](https://en.wikipedia.org/wiki/Master-slave_(technology)) relationship. Alternatively, a ["database-centric" architecture](https://en.wikipedia.org/wiki/Database-centric_architecture) can enable distributed computing to be done without any form of direct [inter-process communication](https://en.wikipedia.org/wiki/Inter-process_communication), by utilizing a shared [database](https://en.wikipedia.org/wiki/Database). Database-centric architecture in particular provides relational processing analytics in a schematic architecture allowing for live environment relay. This enables distributed computing functions both within and beyond the parameters of a networked database. 