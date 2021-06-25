# paper [Time, Clocks, and the Ordering of Events in a Distributed System](https://lamport.azurewebsites.net/pubs/time-clocks.pdf)

Leslie Lamport Massachusetts Computer Associates, Inc. 



The concept of one **event** happening before another in a distributed system is examined, and is shown to define a **partial ordering of the events**. A **distributed algorithm** is given for synchronizing a system of **logical clocks** which can be used to totally order the events. The use of the total ordering is illustrated with a method for solving synchronization problems. The algorithm is then specialized for synchronizing physical clocks, and a bound is derived on how far out of synchrony the clocks can become.

> NOTE: 
>
> 一、翻译: "研究了分布式系统中一个事件先于另一个事件发生的概念，并显示了该事件以定义事件的部分排序。 给出了用于同步逻辑时钟系统的分布式算法，该逻辑时钟可用于对事件进行整体排序。 通过解决同步问题的方法说明了总排序的使用。 然后，该算法专门用于同步物理时钟，并得出时钟可以变得多不同步的界限。"

> NOTE : 上述**logical clocks**，在[raft](https://www.usenix.org/system/files/conference/atc14/atc14-paper-ongaro.pdf)算法中是使用term来实现的；

**Key Words and Phrases**: distributed systems, computer networks, clock synchronization, multiprocess systems

CR Categories: 4.32, 5.29 



## Introduction

The concept of time is fundamental to our way of thinking. It is derived from the more basic concept of the **order** in which events occur. We say that something happened at 3:15 if it occurred after our clock read 3:15 and before it read 3:16. The concept of the temporal ordering of events pervades（遍布） our thinking about systems. For example, in an airline reservation system we specify that a request for a reservation should be granted if it is made before the flight is filled. However, we will see that this concept must be carefully reexamined when considering events in a distributed system. 

A distributed system consists of a collection of distinct processes which are spatially separated, and which communicate with one another by exchanging messages. A network of interconnected computers, such as the ARPA net, is a distributed system. A single computer can also be viewed as a distributed system in which the central control unit, the memory units, and the input-output channels are separate processes. A system is distributed if the message transmission delay is not negligible compared to the time between events in a single process.

We will concern ourselves primarily with systems of spatially separated computers. However, many of our remarks will apply more generally. In particular, a multiprocessing system on a single computer involves problems similar to those of a distributed system because of the unpredictable order in which certain events can occur.

In a distributed system, it is sometimes impossible to say that one of two events occurred first. The relation "happened before" is therefore only a partial ordering of the events in the system. We have found that problems often arise because people are not fully aware of this fact and its implications.

In this paper, we discuss the partial ordering defined by the "happened before" relation, and give a distributed algorithm for extending it to a consistent total ordering of all the events. This algorithm can provide a useful mechanism for implementing a distributed system. We illustrate its use with a simple method for solving synchronization problems. Unexpected, anomalous behavior can occur if the ordering obtained by this algorithm differs from that perceived by the user. This can be avoided by introducing real, physical clocks. We describe a simple method for synchronizing these clocks, and derive an upper bound on how far out of synchrony they can drift. 