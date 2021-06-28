# zhihu [漫话分布式系统共识协议: 2PC/3PC篇](https://zhuanlan.zhihu.com/p/35298019)

> NOTE: 
>
> 作者将2PC、3PC视为共识协议

## 2PC (两阶段提交)协议

### 2PC的原理

顾名思义, 2PC协议有两个阶段:Propose和Commit. **在无failure情况下**的2PC协议流程的画风是这样的:

![img](https://pic1.zhimg.com/80/v2-e2f7149a81d9ad3aa46589e25503d688_1440w.jpg)

> 图1: 2PC, coordinator提议通过, voter{1,2,3}达成新的共识

如果有至少一个voter (比如voter3)在Propose阶段投了反对票, 那么propose通过失败. coordinator就会在Commit(or abort)阶段跟所有voter说, 放弃这个propose.

![img](https://pic4.zhimg.com/80/v2-d40abfa365ed84e84e264ba13900f64b_1440w.jpg)

> 图2: 2PC, coordinator提议没有通过, voter{1,2,3}保持旧有的共识

### 2PC的缺陷

2PC的缺点在于不能处理fail-stop形式的节点failure. 比如下图这种情况. 假设coordinator和voter3都在Commit这个阶段crash了, 而voter1和voter2没有收到commit消息. 这时候voter1和voter2就陷入了一个困境. 因为他们并不能判断现在是两个场景中的哪一种: 

(1)上轮全票通过然后voter3第一个收到了commit的消息并在commit操作之后crash了, 

(2)上轮voter3反对所以干脆没有通过.

![img](https://pic3.zhimg.com/80/v2-a9e4ef8b9082ffdf76bc426e61ba3ed2_1440w.jpg)

> 图3: 2PC, coordinator和voter3 crash, voter{1,2}无法判断当前状态而卡死

2PC在这种fail-stop情况下会失败是因为voter在得知Propose Phase结果后就直接commit了, 而并没有在commit之前告知其他voter自己已收到Propose Phase的结果. 从而导致在coordinator和一个voter双双掉线的情况下, 其余voter不但无法复原Propose Phase的结果, 也无法知道掉线的voter是否打算甚至已经commit. 为了解决这一问题, 3PC了解一下.

## 3PC (三阶段提交)协议

### 3PC的原理

简单的说来, 3PC就是把2PC的Commit阶段拆成了PreCommit和Commit两个阶段. 通过进入增加的这一个PreCommit阶段, voter可以得到Propose阶段的投票结果, 但不会commit; 而通过进入Commit阶段, voter可以盘出其他每个voter也都打算commit了, 从而可以放心的commit.

**换言之, 3PC在2PC的Commit阶段里增加了一个barrier**(即相当于告诉其他所有voter, 我收到了Propose的结果啦). 在这个barrier之前coordinator掉线的话, 其他voter可以得出结论不是每个voter都收到Propose Phase的结果, 从而放弃或选出新的coordinator; 在这个barrier之后coordinator掉线的话, 每个voter会放心的commit, 因为他们知道其他voter也都做同样的计划.

![img](https://pic2.zhimg.com/80/v2-28c17c86e689007015a4853f0d0c4a89_1440w.jpg)

> 图4: 3PC, coordinator提议通过, voter{1,2,3}达成新的共识

### 3PC的缺陷

3PC可以有效的处理fail-stop的模式, 但不能处理网络划分(network partition)的情况---节点互相不能通信. 假设在PreCommit阶段所有节点被一分为二, 收到preCommit消息的voter在一边, 而没有收到这个消息的在另外一边. 在这种情况下, 两边就可能会选出新的coordinator而做出不同的决定.

![img](https://pic4.zhimg.com/80/v2-7a18adc27a2bd7b5f5926dd999bc7bb3_1440w.jpg)

> 图5: 3PC, network partition, voter{1,2,3}失去共识

除了网络划分以外, 3PC也不能处理fail-recover的错误情况. 简单说来当coordinator收到preCommit的确认前crash, 于是其他某一个voter接替了原coordinator的任务而开始组织所有voter commit. 而与此同时原coordinator重启后又回到了网络中, 开始继续之前的回合---发送abort给各位voter因为它并没有收到preCommit. 此时有可能会出现原coordinator和继任的coordinator给不同节点发送相矛盾的commit和abort指令, 从而出现个节点的状态分歧.

这种情况等价于一个更真实或者更负责的网络环境假设: 异步网络. 在这种假设下, 网络传输时间可能任意长. 为了解决这种情况, 那就得请出下一篇的主角: Paxos

## 总结

1、2PC使用两个roundtrip来达成新的共识或维持旧有的共识. 其局限性在于不能保证有节点永久性崩溃(fail-stop)的情况下算法能向前推进;

2、3PC扩展了2PC, 使用三个roundtrip达成共识. 其局限性在于不能保证在节点暂时性崩溃(fail-recover), 或是有网络划分的情况下, 共识依旧成立.

## 推荐阅读

- [Consensus Protocols: Two-Phase Commit - Paper Trail](https://link.zhihu.com/?target=http%3A//the-paper-trail.org/blog/consensus-protocols-two-phase-commit/)
- [Consensus Protocols: Three-phase Commit - Paper Trail](https://link.zhihu.com/?target=http%3A//the-paper-trail.org/blog/consensus-protocols-three-phase-commit/)
- [In distributed systems, what is a simple explanation of the Paxos algorithm?](https://link.zhihu.com/?target=https%3A//www.quora.com/In-distributed-systems-what-is-a-simple-explanation-of-the-Paxos-algorithm)