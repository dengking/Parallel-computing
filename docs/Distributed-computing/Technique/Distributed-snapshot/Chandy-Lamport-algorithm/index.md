# Chandy-Lamport algorithm

参考文章: 

1、jianshu [Chandy-Lamport分布式快照算法小记](https://www.jianshu.com/p/06fff1ffe0a7)

2、zhihu [Chandy-Lamport算法核心解读](https://zhuanlan.zhihu.com/p/113612578)

3、paper [Distributed Snapshots: Determining Global States of Distributed Systems](https://www.microsoft.com/en-us/research/uploads/prod/2016/12/Determining-Global-States-of-a-Distributed-System.pdf) 



## Lack of global clock

jianshu [Chandy-Lamport分布式快照算法小记](https://www.jianshu.com/p/06fff1ffe0a7)

> 各个进程的时钟也很有可能不同步，故不能在一瞬间同时捕获所有进程和链路的状态。所以必须要曲线救国，通过**每个进程记录的与自己相关的状态合并出全局状态**，这也是Chandy-Lamport算法的核心思想所在。



## jianshu [Chandy-Lamport分布式快照算法小记](https://www.jianshu.com/p/06fff1ffe0a7)

> NOTE: 
>
> 一、在 Chandy-Lamport 中，蕴含: 
>
> 1、gossip
>
> 2、event->因果



### Distributed Snapshot

所谓**分布式快照**，实际上就是特定时间点记录下来的分布式系统的**全局状态**（**global state**）。**分布式快照**的主要用途有**故障恢复**（即**检查点**）、死锁检测、垃圾收集等。

#### Formal analysis

> NOTE: 
>
> 一、下面描述的分析方法是典型的formal analysis，是值得借鉴学习的

为了方便理解，我们仍然沿用原论文中的方法，将**分布式系统**抽象为一张**有向图**：顶点称为进程（process），边称为链路（channel）。下图就示出包含3个进程和4个链路的分布式系统。

![img](https://upload-images.jianshu.io/upload_images/195230-9cf4136b2be438ec.png?imageMogr2/auto-orient/strip|imageView2/2/w/722/format/webp)

那么，**全局状态就要包含所有进程的状态以及所有链路的状态**。由于进程之间在通过链路不停地交换数据，所以以下动作都可能造成**全局状态**的改变：

1、进程p收到或发出一条消息M；

2、链路c承载了到达或离开进程p的一条消息M。

论文中将使分布式系统状态发生变化的因素叫做事件（event），并给出了它的形式化定义，即e=<p, s, s', M, c>。p、M、c的定义上面已经提到了，而s和s'分别是进程p在事件e发生之前及发生之后的状态。

> NOTE: 
>
> 因果分析



#### Chandy-Lamport算法的核心思想

可见，进程对自己的状态是有感知的，而链路本身只负责传递消息，它们的状态不容易记录。并且我们**无法让时间静止**，各个进程的时钟也很有可能不同步，故不能在一瞬间同时捕获所有进程和链路的状态。所以必须要曲线救国，通过**每个进程记录的与自己相关的状态合并出全局状态**，这也是Chandy-Lamport算法的核心思想所在。

### The Chandy-Lamport Algorithm

Chandy-Lamport算法基于如下前提：在每对进程$p_i$、$p_j$ 之间都存在两条单向的链路$c_{ij}$ 和 $c_{ji}$，即对于$p_i$ 来讲，$c_{ij}$ 是出边，$c_{ji}$ 是入边。链路的网络可靠，缓存无限大，并且先进先出，即链路上的消息会不重不漏地按序到达。

算法要达到如下的终极目标：

1、最终产生的**快照**必须保证一致性；

> NOTE: 
>
> 此处的一致性如何理解？

2、快照过程不能影响系统正常运行，更不能stop the world。

为了保证成功取得全局快照，Chandy-Lamport算法分为3个阶段，即**初始化快照**、**扩散快照**与**完成快照**，并且借助一种与正常消息不同的特殊消息作为**标记**，英文称为**marker**。这3个阶段并没有直接体现在论文中，而是出自[普林斯顿大学相关课程的PPT](https://links.jianshu.com/go?to=https%3A%2F%2Fwww.cs.princeton.edu%2Fcourses%2Farchive%2Ffall16%2Fcos418%2Fdocs%2FP8-chandy-lamport.pdf)，但算法实际流程则是严格一致的。下面叙述3个阶段的流程。



#### Initiating a Snapshot

> NOTE: 
>
> 一、发起snapshot的process是否固定？

假设进程 $p_i$ 发起快照：

1、$p_i$ 记录自己的状态；

2、$p_i$ 通过所有出边链路 $c_{ij}$向其 他进程 $p_j$ 发送marker消息；

3、$p_i$ 通过所有入边链路 $c_{ji}$ 开始监听所有流向自己的消息。



#### Propagating a Snapshot

对于任意一个进程$p_j$（包含发起快照的那个进程），考虑它的所有入边链路$c_{kj}$。当在$c_{kj}$ 上收到了marker消息时，有两种情况。

一、如果 $p_j$ 还没有记录自己的状态——

1、$p_j$ 记录自己的状态，并将$c_{kj}$​ 标记为空；

> NOTE: 
>
> 一、"标记为空"是什么含义？

2、$p_j$​ 通过所有出边链路向其他进程发送marker消息；

> NOTE: 
>
> 一、有些gossip的意味

3、通过所有入边链路开始监听所有流向自己的消息。

二、如果 $p_{j}$ 已经记录过自己的状态——

记下入边链路上监听到的消息，直到收到marker消息为止。



#### Terminating a Snapshot

若所有进程都成功地：

1、收到了marker消息；

2、记录下了自己的状态；

3、记录下了入边链路的状态（就是链路上的消息）

则快照成功，算法流程结束。然后就可以将所有这些状态传输到一个稳定的分布式存储中心，全局快照就产生了。



#### Understanding the Algorithm

全局快照的难点在于，因为系统不能停止，每个进程向下游发送的消息是源源不断的，所以必须得有个东西来划分“当前的消息”与“将来的消息”，让它们不会混淆，而**marker消息**就是这个界限。对进程 $p_j$​​ 的入边链路 $c_{kj}$​​ 而言，如果收到的消息序列是[a, b, c, marker, d, e, f]，那么就说明 `a/b/c` 三条消息属于当前快照，而 `d/e/f` 三条消息属于下一个快照。

另外，因为每个进程都记录自己收到的消息，所以进程最终都能持有入边链路的状态。为什么要用接收进程来记录？用两句大白话来解释：

1、只有发送了数据，下游才会（按序）接收到数据；

2、发送者无法知道数据在下游的接收情况。

**单令牌系统**

如果不这样设计，而采用简单的单令牌系统的话，就会有问题。仍然借用论文中给出的一个例子：

![img](https://upload-images.jianshu.io/upload_images/195230-560e5880a098f70b.png?imageMogr2/auto-orient/strip|imageView2/2/w/721/format/webp)

进程p和q以令牌（token）的传送来标定状态，s1表示持有令牌，s0表示未持有令牌。假设在“token in C”全局状态下保存快照，而q的时间戳比p早，那么就会造成两个进程都处于s0状态，而q认为令牌不在链路C中，p也认为令牌不在链路C'中，因此快照里令牌就会丢失，造成不一致。



## TODO

geeksforgeeks [Chandy–Lamport’s global state recording algorithm](https://www.geeksforgeeks.org/chandy-lamports-global-state-recording-algorithm/)

Cambridge University Press [Chapter 4: Global State and Snapshot Recording Algorithms](https://www.cs.uic.edu/~ajayk/Chapter4.pdf)

