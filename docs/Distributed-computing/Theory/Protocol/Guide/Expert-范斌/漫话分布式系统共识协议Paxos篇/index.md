# zhihu [漫话分布式系统共识协议: Paxos篇](https://zhuanlan.zhihu.com/p/35737689) 

可能和很多人的印象相反, Paxos其实是一个异常简洁而精巧的算法. 解读一遍Paxos算法其实只需要5分钟. 本文将集中在经典的basic Paxos上, 而不会涉及其各种变种(实在也太多了).

## 前言

本文是“漫话分布式系统共识协议”这个系列的第二篇. 前一篇"[2PC/3PC篇](https://zhuanlan.zhihu.com/p/35298019)"介绍了分布式共识算法中最早的2PC和3PC两位老前辈. 

再比因为Lamport稳重带皮的操作, 导致大家口口相传Paxos理解和实现起来有多困难多复杂, 导致出现了Raft这种改良版等等.

> NOTE: 
>
> paxos和raft之间的关系

**敲黑板**: Paxos其实是一个异常简洁而精巧的算法. 解读一遍Paxos算法其实只需要5分钟. 真正复杂的地方在于想清楚Paxos算法在各种failure情形下如何依然"正确"的工作. 只有明白了这一层, 才算练成了Paxos的心法, 才能真正欣赏Paxos算法的精妙设计, 赞叹Lamport的天才思维. 在我看来, Paxos算法(连同Lamport的其他如BFT, Vector Clock等成就)是上个世纪八十/九十年代的经典分布式系统研究中最纯粹最优美, 也是整栋大厦底座最坚实的那一部分.

> NOTE: 
>
> 一、Leslie-Lamport
>
> 参见 `Expert-Leslie-Lamport` 章节
>
> 二、BFT即Byzantine-Fault-Tolerance

插点题外话: 我第一次认真接触和学习Paxos是在CMU时TA分布式系统([15-440, Fall 2012: Distributed Systems](https://link.zhihu.com/?target=http%3A//www.cs.cmu.edu/~dga/15-440/F12/)). 

## Paxos算法描述

> NOTE: 
>
> 一、paper [Paxos Made Simple](https://lamport.azurewebsites.net/pubs/paxos-simple.pdf)
>
> > We let the three roles in the consensus algorithm be performed by three classes of agents: proposers, acceptors, and learners.
>
> 二、看了一下，下面的内容是使用 paper [Paxos Made Simple](https://lamport.azurewebsites.net/pubs/paxos-simple.pdf) 中的术语描述的

考虑一个简化了的Paxos系统: 只有leader和acceptor两种角色.

### 1、Prepare阶段

> NOTE: 
>
> 一、paper [Paxos Made Simple](https://lamport.azurewebsites.net/pubs/paxos-simple.pdf)
>
> > The algorithm chooses a leader, which plays the roles of the distinguished proposer and the distinguished learner.
>
> 二、Prepare阶段就是从proposer中选择一个作为leader，即leader election

#### (1a) 

leader的节点给所有其他acceptor节点发送消息"proposal(n)"---n是该节点为这个提议选择的一个数字, 姑且理解为一个方案编号. 并期待该提议获得所有节点中的简单多数(Paxos的Quorum)的许可.

#### (1b)

每一个接受到proposal的acceptor节点: 

一、如果这是它接受到的第一个proposal, 回答"promise". 代表该节点**许诺**将会保持承认该proposal发送方为leader, 除非收到其他优先级更高的proposal; 

> NOTE: 
>
> 回答promise

二、如果已经有接收到并accepted(注: 这是下一阶段可能会发生的动作)其他的proposal(n',v')--n'是该proposal的方案号而v'是提议的共识:

1、如果 n < n', 之前accept的提议有更高优先级, 对新接受的提议回答"reject", 以兑现之前的许诺.

> NOTE: 
>
> 回答reject

2、如果 n > n', 回答"promise"的并同时附上旧的提议,proposal(n', v'). 这样在认可新的leader身份的同时, 也告诉了新的leader过去的被简单多数认可过的提议

> NOTE: 
>
> 回答promise



> NOTE: 
>
> 需要注意，上面仅仅列举了两种情况，其实不止两种情况，还有第三种情况，在 csdn [诸葛亮 VS 庞统，拿下 Paxos 共识算法](https://blog.csdn.net/jackson0714/article/details/112593110?spm=1001.2014.3001.5501) 中，就列举了这种情况: 
>
> 三、如果已经有接收到并promise其他的proposal(n',v')
>
> 1、如果 n < n'，回答reject
>
> 2、如果 n > n'，回答promise

#### (1c) 

一、如果proposer的提议受到了简单多数的"reject", 竞争leader宣告失败, 可以放弃这一提议; 

二、如果接受到了简单多数的"promise", 则该proposer成为leader, 它需要从收到的promise里附带的之前accepted的提议中选取方案号(n值)最高的对应的共识; 如果历史上没有被accept过的提议, leader可以自己选取一个共识v.

> NOTE: 
>
> 需要注意，它需要首先处理"之前accepted的提议中选取方案号(n值)最高的对应的共识"

### 2、Accept阶段

#### (2a)

leader会对所有acceptor发送"accept-request(n,v)", 请求所有acceptor接受编号为n的共识v的提议

#### (2b) 

每一个接收到该提议的acceptor节点: 如果没有接受过编号比n更高的提议, 则返回"accept"表示接受这一共识提议; 否则返回"reject"

> NOTE: 
>
> 回答accept
>
> 回答reject

#### (2c) 

如果简单多数的acceptor返回了"accept", 则共识达成; 否则共识失败, 重启Paxos协议.



请注意几点以帮助理解协议:

1、第一阶段竞争的并不是共识本身, 而是在争取坐实leader身份获得简单多数的认可

2、方案编号n本身并不是共识, 而是提议的一个优先级, 在多个节点竞争leader身份时可以区分优先顺序. 共识本身(v)会在下一阶段leader身份确认后由leader添加进提议;

> NOTE: 
>
> 显然"方案编号n"是为了处理多个节点同时竞争leader身份而添加的

3、虽然这一轮上只会有一个leader获得简单多数的认可产生, 但可能有多个"糊涂"节点认为自己应该做leader, 见后面的分析;

### Paxos对2PC和3PC的改进

在我看来, Paxos对2PC和3PC有几点重要的改进.

**第一，分离共识的提议者proposer以及帮助提议最终通过的leader这两个角色.** 

Paxos里, 即使一个leader身份被批准, 它也需要尊重历史上其他被同意过的提议. 换言之leader本身只是一个服务性的角色, 未必有机会自己提出共识. 

> NOTE: 
>
> 这是一个非常深刻的认识

回忆一下上一篇介绍的2PC和3PC这两个协议当中, coordinator不仅负责提出最后的共识协议, 同时也负责服务所有节点保证它的共识被通过. 而正是因为Paxos中把coordinator的职责解耦合成了proposer和leader, 使得整个算法更加robust.就算前任leader宕机了, 后面新产生leader也可以继承前任的"遗志"来完成一个Paxos协议.

**第二，对简单多数的巧妙应用.** 

第一阶段里选举leader要求的简单多数保证了选举出来的leader一定不会错过之前被accept过的提议---所以就算那个提议最初的proposer挂了, 也会至少被一个acceptor发给新的leader来继承. 而第二阶段里要求的达成共识的简单多数保证了有多个"自以为是"的leader出现时(比如一个leader掉线, 新leader选出, 旧leader重新上线), 一定只会有一个最后通过. 看过一个精彩的评论, 说Paxos其实就是连续运用两次"抽屉原理", 其实非常准确.

> NOTE: 
>
> 一、关于"Paxos其实就是连续运用两次"抽屉原理""，在下面的文章中有非常好的介绍:
>
> 1、cnblogs [分布式一致性算法——Paxos原理分析](https://www.cnblogs.com/limejuiceOwO/p/14881623.html)
>
> > 我们称上面的算法为“多数同意算法”。为什么这个算法能够确保达成共识——换个说法，能够保证最多只有一个节点有权进行广播呢？用反证法很容易证明这一点。假设有两个提案者都收到了一半以上的接受回复，那么根据**抽屉原理**，回复这两个节点的接受者里面，必定会至少有一个节点同时回复了这两个提案者。这就产生了明显的矛盾——说好的只回复一个提案者呢？到此，我们已经保证了这个算法的正确性，任务似乎已经完成了。
>
> 

## Paxos与2PC/3PC的关系

### Paxos如何克服2PC的问题

2PC的问题在于不能处理最简单的fail-stop错误模式.

1、2PC中coordinator是唯一而固定的, 如果coordinator宕机, 那么就会有情形导致coordinator之前propose的提议的投票结果丢失. 就算启动新的后备coordinator, 没有机制可以学习以前的投票结果.

2、Paxos因为分离了提议和leader, 从算法上保证总可以选举出后备leader并接替前任leader的工作.

### Paxos如何克服3PC的问题

3PC改进了2PC的fail-stop的问题, 但是不能处理fail-recover类型的错误.

1、3PC发生的问题在于当有多个"自认的leader"出现时, 并不能有效的解决coordinator之间的竞争---谁是真正的coordinator.

2、而Paxos通过Quorum的运用, 保证了多了个leader之间可以互相发现.

## Paxos的局限性

就像2PC以及3PC一样, Paxos也有其局限性.

1 活锁问题.

Paxos理论上存在一个不能终结协议的活锁竞争问题. 比如一个proposer提交的提议因为编号过低被拒绝时, 此proposer可能重启Paxos而提高编号重新提交. 如果同时有两个proposer都发现自己的方案编号过低, 从而轮流提出更高编号的proposal而导致对方被拒, 可能会导致死循环(或活锁).

> NOTE: 
>
> 不理解

2 恶意节点.

目前为止2PC, 3PC, Paxos均是假设所有节点都遵守协议的规定. 当存在恶意的, 可能发送任何导致协议停止或者出错的消息的节点存在时, 就需要有更强的共识算法在"守法节点"间达成共识. Lamport 的BFT(拜占庭将军问题)了解一下.

