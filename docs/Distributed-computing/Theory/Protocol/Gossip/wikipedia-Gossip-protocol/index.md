
# wikipedia [Gossip protocol](https://en.wikipedia.org/wiki/Gossip_protocol)

> NOTE: 
>
> gossip的本意是闲话，八卦

A **gossip protocol**[[1\]](https://en.wikipedia.org/wiki/Gossip_protocol#cite_note-1) is a procedure or process of computer peer-to-peer communication that is based on the way [epidemics](https://en.wikipedia.org/wiki/Epidemic) (传染病)spread. Some [distributed systems](https://en.wikipedia.org/wiki/Distributed_computing) use **peer-to-peer gossip** to ensure that data is routed to all members of an ad-hoc network. Some ad-hoc networks have no **central registry** and the only way to spread common data is to rely on each **member** to pass it along to their **neighbors**.

The term **epidemic protocol** is sometimes used as a synonym for a gossip protocol, as gossip spreads information in a manner similar to the spread of a [virus](https://en.wikipedia.org/wiki/Virus) in a biological community.

## Gossip communication

The concept of *gossip communication* can be illustrated by the analogy of office workers spreading rumors. Let's say each hour the office workers congregate around the water cooler. Each employee pairs off with another, chosen at random, and shares the latest gossip. At the start of the day, Alice starts a new rumor: she comments to Bob that she believes that Charlie dyes his mustache. At the next meeting, Bob tells Dave, while Alice repeats the idea to Eve. After each water cooler rendezvous, the number of individuals who have heard the rumor roughly doubles (though this doesn't account for gossiping twice to the same person; perhaps Alice tries to tell the story to Frank, only to find that Frank already heard it from Dave). Computer systems typically implement this type of protocol with a form of random "peer selection": with a given frequency, each machine picks another machine at random and shares any hot rumors.

> NOTE: 
>
> 可以通过办公室工作人员传播谣言的类比来说明*gossip communication*的概念。让我们说办公室工作人员每小时聚集在水冷却器周围。每个员工与另一个员工配对，随机选择，并分享最新的八卦。在一天开始时，爱丽丝开始了一个新的谣言：她对鲍勃说她认为查理染了他的胡子。在下次会议上，鲍勃告诉戴夫，而爱丽丝则重复了夏娃的想法。在每个水冷却器会合之后，听到谣言的人数大致翻了一番（虽然这并没有说明两次对同一个人说闲话;也许爱丽丝试图把这个故事讲给弗兰克，但却发现弗兰克已经听过了来自戴夫）。计算机系统通常以随机“对等选择”的形式实现这种类型的协议：在给定频率下，每台机器随机选择另一台机器并分享任何热门谣言。

The weakness of gossip is that quality of service, i.e. complete and timely dissemination, is predicated on the requirement that each member does not discriminate and ensures prompt and dependable transmission of the data to every member of their own peer network. In a real office gossip scenario, not everyone is privy to the gossip that is being spread. Gossip, versus broadcast, is discriminatory and often participants are left out of vital or important communications. As such, the comparison to 'office gossip' is not as good as the comparison to the spread of an epidemic. Nevertheless, the technique of peer-to-peer communication is sometimes referred to as 'gossip'.

gossip 的弱点在于服务质量，即完整和及时的传播，是基于每个成员不区分并确保将数据迅速和可靠地传输到他们自己的对等网络的每个成员的要求。在一个真正的办公室八卦场景中，并非每个人都知道正在传播的八卦。八卦与广播相比具有歧视性，通常参与者被排除在重要或重要的通信之外。因此，与“办公室八卦”的比较并不像与流行病传播的比较那么好。然而，点对点通信技术有时被称为“gossip”。

## Many variants and styles

There are probably hundreds of variants of specific Gossip-like protocols because each use-scenario is likely to be customized to the organization's specific needs.

For example, a gossip protocol might employ some of these ideas:

- The core of the protocol involves periodic, pairwise, inter-process interactions.
- The information exchanged during these interactions is of bounded size.
- When [agents](https://en.wikipedia.org/wiki/Agent-based_model#In_computer_science_and_artificial_intelligence) interact, the state of at least one agent changes to reflect the state of the other.
- Reliable communication is not assumed.
- The frequency of the interactions is low compared to typical message latencies so that the protocol costs are negligible.
- There is some form of randomness in the peer selection. Peers might be selected from the full set of nodes or from a smaller set of [neighbors](https://en.wikipedia.org/wiki/Neighbourhood_(graph_theory)).
- Due to the replication there is an implicit [redundancy](https://en.wikipedia.org/wiki/Redundancy_(engineering)) of the delivered information.