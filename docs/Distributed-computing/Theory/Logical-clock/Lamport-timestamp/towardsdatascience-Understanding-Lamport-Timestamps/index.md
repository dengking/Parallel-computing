# towardsdatascience [Understanding Lamport Timestamps with Python’s multiprocessing library](https://towardsdatascience.com/understanding-lamport-timestamps-with-pythons-multiprocessing-library-12a6427881c6)

Everyone who has been working with distributed systems or logs from such a systems, has directly or indirectly encountered **Lamport Timestamps**. **Lamport Timestamps** are used to (partially) **order** events in a distributed system. The algorithm is based on **causal ordering** of events and is the foundation of more advanced clocks such as **Vector Clocks** and **Interval Tree Clocks (ITC)**.

In this article we will first briefly introduce the concept of **logical clocks**, explain why ordering of events in distributed systems is needed and discuss some alternatives. Then we’ll go over the algorithm of **Lamport Timestamps** and work an example with three processes. Next, we’ll implement this example in easy-to-understand code using Python’s *multiprocessing* library. To top it all off, we’ll transform our code into an implementation with **Vector Clocks**.

# Logical clocks

To understand why **logical clocks** are needed, it is important to understand what a distributed system is. A **distributed system** is a system whose components (here called **processes**) are located on different networked computers, which then coordinate their actions by passing **messages** to one other.

One of the main properties of a distributed system is that it **lacks a global clock**. All the processes have their own local clock, but due to [clock skew](https://en.wikipedia.org/wiki/Clock_skew) and [clock drift](https://en.wikipedia.org/wiki/Clock_drift) they have no direct way to know if their clock is in check with the local clocks of the other processes in the system, this problem is sometimes referred to as the **problem of clock synchronization**.

Solutions to this problem consist of using a central time server ([Cristian’s Algorithm](https://en.wikipedia.org/wiki/Cristian's_algorithm)) or a mechanism called a **logical clock**. The problem with a central time server is that its error depends on the round-trip time of the message from process to time server and back.

**Logical clocks** are based on capturing chronological（按照时间发生的） and **causal relationships** of processes and ordering events based on these **relationships**. The first implementation, the **Lamport timestamps**, was proposed by **Leslie Lamport** in 1978 and still forms the foundation of almost all **logical clocks**.



# Lamport Timestamps algorithm

A **Lamport logical clock** is an incrementing counter maintained in each process. Conceptually, this **logical clock** can be thought of as a **clock** that only has meaning in relation to **messages** moving between processes. When a process receives a message, it resynchronizes its **logical clock** with that sender (causality（因果关系）).

> NOTE : 每类message有一个自己的一个独立的clock，不同类的message有各自独立的clock，显然不同类的message之间是无需进行compare的，所以lamport timestamp是partial ordering；这在这一节的最后一段进行了介绍；

> NOTE : distributed system中的各个节点之间是使用message进行通信的（当节点观察到某个event发生的时候，它通过发送message来将这个event告知给distributed system中的其他的节点；当distributed system中有多个节点都观察到该event的方式，则它们都会通过message来将这个event告知给distributed system中的其他的node，显然，在这种情况下，多个发送message的节点之间是存在着某种竞争的；在很多场景中，就有这样的一个需求：需要确定到底是哪个节点先发送的message，哪个节点后发送的message，以便修改解决这种竞争），一个非常重要的需求是：需要确定两个同类的message发生的先后次序，即到底是哪个message先发生的；

The algorithm of Lamport Timestamps can be captured in a few rules:

- All the process counters start with value 0.
- A process increments its counter for each event (internal event, message sending, message receiving) in that process.
- When a process sends a message, it includes its (incremented) counter value with the message.
- On receiving a message, the counter of the recipient is updated to the greater of its current counter and the timestamp in the received message, and then incremented by one.



Looking at these rules, we can see the algorithm will create a minimum overhead, since the counter consists of just one integer value and the messaging piggybacks on inter-process messages.

One of the shortcomings of Lamport Timestamps is rooted in the fact that they only partially order events (as opposed to total order). **Partial order** indicates that not every pair of events need be comparable. If two events can’t be compared, we call these events **concurrent**. The problem with Lamport Timestamps is that they can’t tell if events are concurrent or not. This problem is solved by **Vector Clocks**.



# Vector Clocks

As mentioned before, Lamport timestamp have one big shortcoming: **they can’t tell you when two events are concurrent**. Going back to our example, by just checking the timestamps, we could conclude that event 3 in process 1 has happend before event 8 in process 3, but this isn’t necessarily true.

