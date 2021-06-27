# Fault tolerance



## wikipedia [Fault tolerance](https://en.wikipedia.org/wiki/Fault_tolerance)

**Fault tolerance** is the property that enables a [system](https://en.wikipedia.org/wiki/System) to continue operating properly in the event of the failure of (or one or more faults within) some of its components. If its operating quality decreases at all, the decrease is proportional to the severity of the failure, as compared to a naively designed system, in which even a small failure can cause total breakdown. Fault tolerance is particularly sought after in [high-availability](https://en.wikipedia.org/wiki/High-availability) or [life-critical systems](https://en.wikipedia.org/wiki/Life-critical_system). The ability of maintaining functionality when portions of a system break down is referred to as **graceful degradation**.[[1\]](https://en.wikipedia.org/wiki/Fault_tolerance#cite_note-1)

> NOTE: 
>
> 容错是使系统在某些组件发生故障（或其中的一个或多个故障）时能够继续正常运行的属性。 如果系统的运行质量完全下降，下降的程度与故障的严重程度成比例，相比之下，在天真设计的系统中，即使是很小的故障也可能导致整体故障。 在高可用性或对生命至关重要的系统中，尤其需要容错功能。 当系统的一部分发生故障时保持功能的能力称为正常降级。

A **fault-tolerant design** enables a system to continue its intended operation, possibly at a reduced level, rather than failing completely, when some part of the system [fails](https://en.wikipedia.org/wiki/Failure).[[2\]](https://en.wikipedia.org/wiki/Fault_tolerance#cite_note-2) The term is most commonly used to describe [computer systems](https://en.wikipedia.org/wiki/Computer_system) designed to continue more or less fully operational with, perhaps, a reduction in [throughput](https://en.wikipedia.org/wiki/Throughput) or an increase in [response time](https://en.wikipedia.org/wiki/Response_time_(technology)) in the event of some partial failure. That is, the system as a whole is not stopped due to problems either in the [hardware](https://en.wikipedia.org/wiki/Computer_hardware) or the [software](https://en.wikipedia.org/wiki/Software). An example in another field is a motor vehicle designed so it will continue to be drivable if one of the tires（轮胎） is punctured（刺破）, or a structure that is able to retain its integrity in the presence of damage due to causes such as [fatigue](https://en.wikipedia.org/wiki/Fatigue_(material)), [corrosion](https://en.wikipedia.org/wiki/Corrosion), manufacturing flaws, or impact.

> NOTE:
>
> 容错设计使系统可以在系统的某些部分发生故障时继续以预期的方式运行（可能会降低级别），而不是完全故障。[2] 该术语最常用于描述设计为或多或少地完全正常运行的计算机系统，在某些局部故障的情况下，吞吐量可能会降低或响应时间会增加。 即，由于硬件或软件的问题，整个系统不会停止。 另一个领域的例子是设计成在轮胎中的一个被刺破的情况下仍可继续驾驶的机动车辆，或在由于疲劳，腐蚀，制造等原因而造成损坏的情况下仍能保持其完整性的结构。 缺陷或影响。

Within the scope of an *individual* system, fault tolerance can be achieved by anticipating exceptional conditions and building the system to cope with them, and, in general, aiming for [self-stabilization](https://en.wikipedia.org/wiki/Self-stabilization) so that the system converges towards an error-free state. However, if the consequences of a system failure are catastrophic, or the cost of making it sufficiently reliable is very high, a better solution may be to use some form of duplication. In any case, if the consequence of a system failure is so catastrophic, the system must be able to use reversion to fall back to a safe mode. This is similar to roll-back recovery but can be a human action if humans are present in the loop.

> NOTE:
>
> 在单个系统的范围内，可以通过预料到异常情况并构建系统以应对这些情况来实现容错能力，并且一般而言，其目标是实现自我稳定，从而使系统收敛于无错误状态。 但是，如果系统故障的后果是灾难性的，或者使其足够可靠的成本非常高，则更好的解决方案可能是使用某种形式的复制。 无论如何，如果系统故障的后果是灾难性的，则系统必须能够使用还原来退回到安全模式。 这类似于回滚恢复，但如果循环中存在人员，则可能是人为的行为。



## Example

[Distributed locks with Redis](https://redis.io/topics/distlock)

[Redis Cluster Specification](https://redis.io/topics/cluster-spec)

[Redis: replication, part 1 – an overview. Replication vs Sharding. Sentinel vs Cluster. Redis topology.](https://rtfm.co.ua/en/redis-replication-part-1-overview-replication-vs-sharding-sentinel-vs-cluster-redis-topology/)



## TODO

wikipedia [Fault-tolerant computer system](https://en.wikipedia.org/wiki/Fault-tolerant_computer_system)

searchdisasterrecovery [fault-tolerant](https://searchdisasterrecovery.techtarget.com/definition/fault-tolerant)