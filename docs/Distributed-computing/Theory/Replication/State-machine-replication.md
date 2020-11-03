# wikipedia [State machine replication](https://en.wikipedia.org/wiki/State_machine_replication)

状态机复制

In [computer science](https://en.wikipedia.org/wiki/Computer_science), **state machine replication** or **state machine approach** is a general method for implementing a **fault-tolerant service** by replicating servers and coordinating client interactions with server replicas. The approach also provides a framework for understanding and designing replication management protocols.[[1\]](https://en.wikipedia.org/wiki/State_machine_replication#cite_note-schneider-1)

SUMMARY : 使用replicated state machine的目的是构建fault-tolerant service，如果以redis来看的话，fault-tolerant的意思是即使cluster中有一个node出现了问题，系统能够进行failover。恢复的正常的状态。
***SUMMARY*** : 要想完整地理解state machine replication的含义，其实可以阅读一下如下文章：

- [Raft (computer science)](https://en.wikipedia.org/wiki/Raft_(computer_science))
- [The Raft Consensus Algorithm](https://raft.github.io/)

## Problem definition

### Distributed services

Distributed software is often structured in terms of clients and services. Each service comprises one or more servers and exports operations that clients invoke by making requests. Although using a single, **centralized server** is the simplest way to implement a service, the resulting service can only be as **fault tolerant** as the processor executing that server. If this level of **fault tolerance** is unacceptable, then multiple servers that fail independently must be used. Usually, replicas of a single server are executed on separate processors of a distributed system, and protocols are used to coordinate client interactions with these replicas. The physical and electrical isolation of processors in a distributed system ensures that server failures are independent, as required.





### State machine

*Main article:* [Finite-state machine](https://en.wikipedia.org/wiki/Finite-state_machine)

For the subsequent discussion a *State Machine* will be defined as the following tuple of values [[2\]](https://en.wikipedia.org/wiki/State_machine_replication#cite_note-reliable-2) (See also [Mealy machine](https://en.wikipedia.org/wiki/Mealy_machine) and [Moore Machine](https://en.wikipedia.org/wiki/Moore_Machine)):

- A set of *States*
- A set of *Inputs*
- A set of *Outputs*
- A transition function (Input × State → State)
- An output function (Input × State → Output)
- A distinguished State called Start.

A State Machine begins at the State labeled Start. Each Input received is passed through the **transition** and output function to produce a new State and an Output. The State is held stable until a new Input is received, while the Output is communicated to the appropriate receiver.

This discussion requires a State Machine to be **deterministic**: multiple copies of the same State Machine begin in the Start state, and receiving the same Inputs in the same order will arrive at the same State having generated the same Outputs.

State Machines can implement any algorithm when driven by an appropriate Input stream, including Turing-complete algorithms (see [Turing machine](https://en.wikipedia.org/wiki/Turing_machine)). Typically, systems based on **State Machine Replication** voluntarily restrict their implementations to use [finite-state machines](https://en.wikipedia.org/wiki/Finite-state_machine) to simplify error recovery.

### Fault Tolerance

**Determinism** is an ideal characteristic for providing fault-tolerance. Intuitively（直观地）, if multiple copies of a system exist, a fault in one would be noticeable as a **difference** in the State or Output from the others.

A little deduction shows the minimum number of copies needed for **fault-tolerance** is three; one which has a fault, and two others to whom we compare State and Output. Two copies are not enough as there is no way to tell which copy is the faulty one.

Further deduction shows a three-copy system can support at most one failure (after which it must repair or replace the faulty copy). If more than one of the copies were to fail, all three States and Outputs might differ, and there would be no way to choose which is the correct one.

In general, a system which supports F failures must have 2F+1 copies (also called replicas).[[3\]](https://en.wikipedia.org/wiki/State_machine_replication#cite_note-lowerbounds-3) The extra copies are used as evidence to decide which of the copies are correct and which are faulty. Special cases can improve these bounds.[[4\]](https://en.wikipedia.org/wiki/State_machine_replication#cite_note-cheap-4)

All of this deduction pre-supposes that replicas are experiencing only random independent faults such as memory errors or hard-drive crash. Failures caused by replicas which attempt to lie, deceive, or collude can also be handled by the State Machine Approach, with isolated changes.

所有这些推断都假设副本只经历随机的独立故障，比如内存错误或硬盘崩溃。
由试图说谎、欺骗或串通的副本引起的失败也可以由状态机方法处理，并进行独立的更改。

Failed replicas are not required to stop; they may continue operating, including generating spurious or incorrect Outputs.

#### Special Case: Fail-Stop

Theoretically, if a failed replica is guaranteed to stop without generating outputs, only F+1 replicas are required, and clients may accept the first output generated by the system. No existing systems achieve this limit, but it is often used when analyzing systems built on top of a **fault-tolerant layer** (Since the fault-tolerant layer provides fail-stop semantics to all layers above it).

#### Special Case: Byzantine Failure

Faults where a replica sends different values in different directions (for instance, the correct Output to some of its fellow replicas and incorrect Outputs to others) are called [Byzantine Failures](https://en.wikipedia.org/wiki/Byzantine_fault_tolerance).[[5\]](https://en.wikipedia.org/wiki/State_machine_replication#cite_note-byzantine-5) Byzantine failures may be random, spurious faults, or malicious, intelligent attacks. 2F+1 replicas, with non-cryptographic hashes suffices to survive all non-malicious Byzantine failures (with high probability). Malicious attacks require cryptographic primitives to achieve 2F+1 (using message signatures), or non-cryptographic techniques can be applied but the number of replicas must be increased to 3F+1.[[5\]](https://en.wikipedia.org/wiki/State_machine_replication#cite_note-byzantine-5)

## The State Machine Approach

The preceding intuitive discussion implies a simple technique for implementing a fault-tolerant service in terms of a State Machine:

1. Place copies of the State Machine on multiple, independent servers.
2. Receive client requests, interpreted as Inputs to the State Machine.
3. Choose an ordering for the Inputs.
4. Execute Inputs in the chosen order on each server.
5. Respond to clients with the Output from the State Machine.
6. Monitor replicas for differences in State or Output.

The remainder of this article develops the details of this technique.

- Step 1 and 2 are outside the scope of this article.
- Step 3 is the critical operation, see [Ordering Inputs](https://en.wikipedia.org/wiki/State_machine_replication#Ordering_Inputs).
- Step 4 is covered by the [State Machine Definition](https://en.wikipedia.org/wiki/State_machine_replication#State_Machine_Definition).
- Step 5, see [Ordering Outputs](https://en.wikipedia.org/wiki/State_machine_replication#Sending_Outputs).
- Step 6, see [Auditing and Failure Detection](https://en.wikipedia.org/wiki/State_machine_replication#Auditing_and_Failure_Detection).

The appendix contains discussion on typical extensions used in real-world systems such as [Logging](https://en.wikipedia.org/wiki/State_machine_replication#Logging), [Checkpoints](https://en.wikipedia.org/wiki/State_machine_replication#Checkpoints), [Reconfiguration](https://en.wikipedia.org/wiki/State_machine_replication#Reconfiguration), and [State Transfer](https://en.wikipedia.org/wiki/State_machine_replication#State_Transfer).

