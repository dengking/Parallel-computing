# Consensus 

"consensus "即"共识"，它是distributed computing中的典型问题。目前采用的基本思想是(在Designing Data-Intensive Applications ):

> 事实是由多数决定的，使用投票的方式



## wikipedia [Consensus (computer science)](https://en.wikipedia.org/wiki/Consensus_(computer_science))

> NOTE： Consensus的中文意思是共识

A fundamental problem in [distributed computing](https://en.wikipedia.org/wiki/Distributed_computing) and [multi-agent systems](https://en.wikipedia.org/wiki/Multi-agent_system) is to achieve overall system reliability in the presence of a number of faulty processes. This often requires processes to agree on some data value that is needed during computation. Examples of applications of **consensus** include whether to commit a transaction to a database, agreeing on the identity of a [leader](https://en.wikipedia.org/wiki/Leader_election), [state machine replication](https://en.wikipedia.org/wiki/State_machine_replication), and [atomic broadcasts](https://en.wikipedia.org/wiki/Atomic_broadcast). The real world applications include [clock synchronization](https://en.wikipedia.org/wiki/Clock_synchronization), [PageRank](https://en.wikipedia.org/wiki/PageRank), opinion formation(意见形成), [smart power grids](https://en.wikipedia.org/wiki/Smart_grid), [state estimation](https://en.wikipedia.org/wiki/State_estimation), [control of UAVs](https://en.wikipedia.org/wiki/Unmanned_aerial_vehicle) (and multiple robots/agents in general), [load balancing](https://en.wikipedia.org/wiki/Load_balancing_(computing)), [blockchain](https://en.wikipedia.org/wiki/Blockchain) and others.

> NOTE : 在redis中，Consensus 所指就是cluster中的所有的node对cluster state（哪些节点是master，哪些节点是slave）的agreement；显然，达成agreement的最简单的方式就是vote；

### Problem description

The consensus problem requires agreement among a number of processes (or agents) for a single data value. Some of the processes (agents) may fail or be unreliable in other ways, so consensus protocols must be [fault tolerant](https://en.wikipedia.org/wiki/Fault_tolerant) or resilient（弹性的，可快速恢复的）. The processes must somehow put forth their candidate values, communicate with one another, and agree on a single consensus value（发起一轮投票）.

The consensus problem is a fundamental problem in control of multi-agent systems. One approach to generating consensus is for all processes (agents) to agree on a **majority value**. In this context, a majority requires at least one more than half of available **votes** (where each process is given a vote). However, one or more faulty processes may skew（扭曲，歪斜） the resultant outcome such that consensus may not be reached or reached incorrectly.

> 然而,一个或多个有缺陷的过程可能会扭曲结果,即达成共识的结果可能无法达到或达到错误。

Protocols that solve consensus problems are designed to deal with limited numbers of faulty [processes](https://en.wikipedia.org/wiki/Process_(computing)). These protocols must satisfy a number of requirements to be useful. For instance, a trivial protocol could have all processes output binary value 1. This is not useful and thus the requirement is modified such that the output must somehow depend on the input. That is, the output value of a consensus protocol must be the input value of some process. Another requirement is that a process may decide upon and output a value only once and this decision is irrevocable. A process is called correct in an execution if it does not experience a failure. A consensus protocol tolerating halting failures must satisfy the following properties.

