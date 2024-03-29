# csdn [用太极拳讲分布式理论，真舒服！](https://blog.csdn.net/jackson0714/article/details/112002319?spm=1001.2014.3001.5501)

> NOTE: 
>
> 一、虽然原文使用了很多类比，其实核心思想是非常简单的:
>
> CP 刚
>
> AP 柔
>
> 二、tradeoff
>
> 三、原文还介绍了2PC、3PC



## 一、太极的两面

> NOTE: 
>
> 简而言之:
>
> CAP
>
> 1、CP 刚
>
> 2、AP 柔

CAP 理论是对分布式系统的特性做了一个高度的抽象，变成了三大指标：

1、一致性（Consistency）

2、可用性（Availability）

3、分区容错性（Partition Tolerance）



### 一致性

分布式中的一致性，我们可以理解为客户端的每次`读操作`，不管访问的是哪个几点，要么读到的都是同一份最新写入的数据，要么读取失败。这就很刚了，不能说这种`刚`不好，在很多场景中，也确实需要保证高度的一致性。

为了帮助大家理解一致性，我举个`倚天屠龙记`的故事：**六大派围攻光明顶**。



### 1.1 理解分布式中的 CAP

#### `一致性` 刚

`一致性`强调的是数据正确，每次读取节点中的数据都是最新写入的数据。这个我称作`刚`。

但是我们生产的集群环境下如果发生分区故障时（节点失联，节点无法响应，节点无法写入数据），客户端查询节点时，我们不能返回错误信息给客户端。比如说业务集群中的一些关键系统，如注册中心，不能因为某个节点失联了，就不响应最新的数据。那么相关的业务也获取不到正确的注册信息而导致系统瘫痪。

> NOTE: 
>
> 如果按照 CP，则在这种情况下是会"**返回错误信息给客户端**"的

#### `可用性` 柔

`可用性`就派上用场了，牺牲数据准确性，每个节点使用本地数据来响应客户端的请求。另外当节点不可用时，可以使用**快速失败策略**，至少不能让服务长时间不能响应可用性强调的是服务可用，不保证数据正确。这个我称作`柔`。

#### `分区容错性`

`分区容错性`的含义就是节点间出现任意数量的消息丢失或高延迟的时候，系统仍然在继续工作。分布式系统告诉客户端，我的内部不论出现什么样的数据同步问题，我会一直运行。强调的是集群堆分区故障的容错能力。

### 1.2 CAP 三角

那么这三个指标又有什么关系呢？这个就是我们经常听到的 `CAP` 理论。`C` 代表一致性（Consistency），`A` 代表可用性（Availability）、`P` 代表分区容错性（Partition Tolerance）。

对于分布式系统，CAP 三个指标只能选择其中两个。

#### **CA**

> NOTE: 
>
> 其实这种情况压根就不是分布式

保证一致性和可用性。当分布式系统正常运行时（大部分时候所处的状态），这个时候不需要 P，那么 C 和 A 能够同时保证。只有在发生分区故障时，才需要 P，这个时候就只能在 C 和 A 之间做出选择。`典型应用`：单机版部署的 MySQL。

#### **CP**

保证数据的一致性和分区容错性，比如配置信息，必须保证每个节点存的都是最新的，正确的数据。比如 Raft 的强一致性系统，会导致无法执行读操作和写操作。`典型应用`：Etcd、Consul、Hbase。

#### **AP**

保证分布式系统的可用性和分区容错性。用户访问系统，都能得到相应数据，不会出现响应错误，但是可能会读到旧的数据。`典型应用`：Cassandra 和 DynamoDB。

## 二、太极的刚

> NOTE: 
>
> 选择consistency

### 2.1 ACID 的刚

> NOTE: 
>
> 一、所谓"ACID的刚"，指的是它强调consistency
>
> 二、读者肯定会思考CAP、ACID的关系，在"五、总结"中，作者给出了非常好的总结:
>
> "ACID 是传统数据库的设计理念，追求强一致性。四个指标：原子性、一致性、隔离性、持久性。是 CAP 中 CP 的延伸。"

最开始知道 ACID 是研究 SQL 数据库的时候，`原子性`（Atomicity）、`一致性`（Consistency）、`隔离性`（Isolation）、`持久性`（Durability）。

这四个属性是针对`事务`而言的，而事务就是为单个工作单元而执行的一系列操作。如查询、修改数据、修改数据定义。

事务不仅仅只用在数据库上，还可以用在业务系统中，比如发券后扣减库存，这种业务场景可以定义为一个事务。单机场景我们可以通过加锁、时间序列等机制来保证单个节点上的 ACID 特性，但无法保证节点间操作的 ACID 特性。

那么分布式系统下该如何解决事务问题呢？这也是面试中经常遇到的题。分布式事务协议大家一定听过，比如`二阶段提交协议`和 `TCC 协议`，下面我还是用`六大派围攻光明顶`故事来讲解二阶段协议。

> NOTE: 
>
> "TCC协议" 是指3PC，即three-phase commit protocol

### 2.2 围攻光明顶

> NOTE: 
>
> 结合具体例子来说明2PL

`峨眉派`想汇集`少林派`、`武当派`、`昆仑派`明天一起进攻`光明顶`。如果有一方不同意进攻，或者进攻时机不一致，则需要取消整个行动计划。少林派、武当派、昆仑派进攻光明顶这一组行动可以看成是`一个分布式事务`，**要么全部执行、要么全部不执行**。

> NOTE: 
>
> 一、需要所有节点都同意，而不是少数服从多数，因此需要有一个"Propose"阶段，即提议阶段，在提议阶段，可以看出是否所有的节点都同意，如果不是，则通知所有的节点取消；如果是，则通知所有的节点commit；
>
> 二、它的强一致体现在: 所有的node都需要保持一致: "如果有一方不同意进攻，或者进攻时机不一致，则需要取消整个行动计划"

如下图所示：

![img](https://img-blog.csdnimg.cn/img_convert/3d66c142bfea2d40af0d53f34fbfebbd.png)

### 2.3 二阶段提交协议

在二阶段提交协议中，灭绝师太先给少林派发送进攻的消息，少林派作为**协调者**的身份，由少林派联系武当派和昆仑派是进攻还是撤退。

> NOTE: 
>
> 一、有"协调者"，显然就存在single point failure的风险
>
> 二、"协调者"即"coordinator"
>
> 三、这一段关于2PL的介绍，没有范斌 zhihu [漫话分布式系统共识协议: 2PC/3PC篇](https://zhuanlan.zhihu.com/p/35298019) 的好

二阶段就是说有两个阶段，`1.提交请求阶段`（投票阶段），`2.提交执行阶段（完成阶段）`。

#### **阶段一：提交请求阶段(Propose)：**

1、**第一步**：少林派作为协调者分别给武当派和昆仑派发送消息：**“明天进攻光明顶，可行？”**

2、**第二步**：少林派、武当派、昆仑派分别评估明天是否能进攻光明顶，如果能，就预留时间并锁定，不再安排其他的进攻事项。

> NOTE: 
>
> 此处会锁定资源

3、**第三步**：少林派得到全部的回复结果，包括少林派自己的评估结果。最后三方的结果都是`可行`。

如下图所示：

![mark](https://img-blog.csdnimg.cn/img_convert/50570ca9ccaefb810f7373fc15c278c8.png)

> NOTE: 
>
> 上述图中展示的执行顺序:
>
> 第一步: 1.1、1.2
>
> 第二步: 2.1、2.2、2.3
>
> 第三步: 3.2、3.3
>
> 可以看出，作者的画图还是非常认真的，和前面的步骤是对应的

#### **阶段二：提交执行阶段(Commit)：**

1、**第一步**：少林派统计自己、昆仑派和武当派的消息，都是`可以进攻`，所以可以执行分布式事务，进攻光明顶。

> NOTE: 
>
> 需要注意的是: 可能不是都同意，本节所讨论的是执行的情况；

2、**第二步**：少林派通知昆仑派和武当派进攻光明顶。

3、**第三步**：少林派、昆仑派、武当派召集手下弟子，进攻光明顶（执行事务）。

4、**第四步**：昆仑派、武当派将`是否已发起进攻`告诉少林派。

5、**第五步**：少林派汇总自己、昆仑派、武当派的进攻结果给灭绝师太。这样灭绝师太看到的就是统一的作战计划。

![mark](https://img-blog.csdnimg.cn/img_convert/ff578060ff07713f9037a7495b2b1418.png)

> NOTE: 
>
> 第一步:  1
>
> 第二部:  2.1、2.2
>
> 第三步: 3.1、3.2、3.3
>
> 第四步: 4.1、4.2
>
> 第五步: 5

#### 注意：

1、可以将灭绝师太当做客户端。少林派、武当派、昆仑派当做分布式系统的三个节点。少林派作为协调者。

2、将评估是否能进攻光明顶以及预留时间可以理解为需要操作的对象和对象状态，是否已经准备好了，能否提交新的操作。

3、发送消息、飞鸽传书可以理解为网络消息。

4、第一个阶段中，每个参与者投票表决事务是放弃还是提交，一旦投票要求提交事务，那么就不允许放弃事务。

5、第二个阶段中，每个参与者执行最终统一的决定，提交事务或者放弃事务。这个就是 ACID 的原子性。

> NOTE: 
>
> 这就是transaction

6、第一个阶段中，需要预留资源，预留期间，其他人不能操作这个资源。

### 2.4 二阶段协议带来的问题

ACID 特性是 CAP 中一致性的`边界`，可以称作最强的一致性，如果分布式系统中实现了一致性，必然会影响到`可用性`。如果一个节点失败，这个分布式事务的执行都是失败的。

绝大数场景中，对一致性要求没那么高，并不需要保证强一致性，短暂的不一致也能接收，最后能保证数据是正确的就OK。也就是说我们可以用`最终一致性`方案来保证数据的一致性。

另外要提到的就是 `TCC` 协议（三阶段提交协议），他是针对二阶段提交中的：协调者故障，参与者长期锁定资源的`痛点`而出的协议。引入了询问阶段和超时机制，减少资源被长时间锁定。但是需要更多的消息进行协商，增加了系统负载和响应延迟，所以三阶段提交协议很少被使用。

> NOTE: 
>
> 相比于Paxos协议，2PC是比较脆弱的

## 三、太极的柔

### 3.1 BASE 的柔

讲了太极的刚，下面来讲太极的柔。谈到分布式事务的柔，一定会提到 `BASE` 理论，俗称`柔性事务`。

BASE 理论是 CAP 理论中 AP 的扩展。大部分互联网分布式系统都强调可用性，都会考虑引入 BASE 支持。这个理论非常非常重要，我要告诉你的是，掌握了这个理论，设计出符合自己业务的分布式架构也会变得容易很多，而不是摸不着头脑。

**BASE 的核心**：

1、基本可用 `BA`（Basically Available）

> NOTE: 
>
> 需要注意，不是百分之百的可用

2、软状态 `S`（Soft state）

> NOTE: 
>
> 如何理解？

3、最终一致性 `E`（Eventually consistent）

那为什么叫它柔性事务？其实它和 ACID 是相对的，不需要保证**强一致性**。



### 3.3 基本可用

> NOTE: 
>
> Redis cluster就是BASE的

怎么理解基本可用？重点是在这个基本，这个理论并没有告诉我们怎么定义基本，这是一个模糊的概念。其实就是要`柔`到什么程度。

在分布式系统中，我们可以把基本可用理解为保证核心功能可用，允许损失部分功能的可用性。`基本可用`可以用四种方案来实现。

1、**流量削峰**：比如多个秒杀场次，某东的 8 点秒杀场，12 点的秒杀场。

> NOTE: 
>
> 参见:
>
> jianshu [高并发架构系列：什么是流量削峰？如何解决秒杀业务的削峰场景](https://www.jianshu.com/p/6746140bbb76)
>
> zhihu [秒杀之流量削峰](https://zhuanlan.zhihu.com/p/47114618)

2、**延迟响应**：比如双 11 期间某商城创建的订单，会提示客户订单正在创建中，可能需要等个十几秒。

3、**体验降级**：比如某次比赛活动，有大量用户进活动页查看图片，这个时候，大量图片因为网络超时而无法显示，这个时候就可以考虑替换原有图片，返回清晰度没有那么高或图片比较小的图片。

4、**过载保护**：比如我们常用的消息队列占满了，可以考虑丢弃后来的请求，或清除队列中的一些请求，保护系统不过载，但这都需要结合自身的业务场景来设计。

### 3.4 最终一致性

**最终一致性**：系统中的所有的数据副本在经过一段时间的同步后，最终能够达到一个一致的状态。最终可以理解为一个短暂的延迟。

最终一致性在非常多的互联网业务中采用。但是跟钱打交道或金融系统会采用强一致性或事务。

前面提到了 ACID 的`强一致性`，而`最终一致性`和它是什么关系？

强一致性其实也是最终一致性的一种。那最终一致性怎么理解？强一致性可以看作不存在延迟的一致性。如果无法容忍延迟就用强一致性，否则就用最终一致性。

## 五、总结

1、太极拳分为阴和阳两方面，就如 CAP 中的 C 和 A。

2、ACID 是传统数据库的设计理念，追求强一致性。四个指标：原子性、一致性、隔离性、持久性。是 CAP 中 CP 的延伸。

> NOTE: 
>
> 总结得非常好

3、BASE 理论是 CAP 中一致性和可用性权衡的结果。是 CAP 中的 AP 的延伸。注重可用性和性能优先，根据业务的场景特点，实现弹性的基本可用，然后实现数据的最终一致性。

4、BASE 理论在很大程度上，解决了事务性系统在性能、容错、可用性等方面的痛点。

5、BASE 理论在 NoSQL 中应用广泛，是 NoSQL 系统设计的事实上的理论支撑。

> NOTE: 
>
> Redis就是典型的BASE
