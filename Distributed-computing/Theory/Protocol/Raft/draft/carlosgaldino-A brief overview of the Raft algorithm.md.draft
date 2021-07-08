# carlosgaldino [A brief overview of the Raft algorithm](http://blog.carlosgaldino.com/a-brief-overview-of-the-raft-algorithm.html)

Raft is a **consensus algorithm** for managing a replicated log. It is used to achieve an agreement between multiple entities allowing them to serve as a coherent group that can tolerate failures of some of its members. For example, you can have several Key/Value servers and want them to have the same values so in case some of them fail the system still operates correctly since the data is the same. Using a consensus algorithm can help in this situation where for each operation, the servers agree on what is/should be the correct value.

In the following sections you will learn more about **consensus algorithms** and how the Raft algorithm works.

## Consensus algorithms

As mentioned above you might want to use a consensus algorithm when you have multiple servers providing some kind of service, where the servers are viewed as a single group and data is replicated between them so if some of them fail the group can still provide the data that was previously stored.

**Consensus algorithms** must satisfy some properties in order to be useful[1](http://blog.carlosgaldino.com/a-brief-overview-of-the-raft-algorithm.html#fn1). They are the following:

- **Termination**: All correct processes decide on some value.
- **Validity**: If all processes propose the same value `v`, then all correct processes decide `v`.
- **Integrity**: Every correct process decide on at most one value, and if it decides some value `v`, then `v` must been proposed by some process.
- **Agreement**: Every correct process must agree on the same value.

## The Raft consensus algorithm

The algorithm centers around the role of the *leader*. One process is elected and then has the responsibility for managing the **replicated log**. Management means accepting log entries from clients, replicating them to other processes, and telling processes that is safe to apply log entries to their **state machines**.

What Raft does is to break the consensus problem into three subproblems:

- **Leader election**: a new leader is always chosen when an existing leader dies.
- **Log replication**: the leader has to accept log entries from the clients and replicate them to other servers and thus force them to agree with its own logs.
- **Safety**: if a particular log entry is applied by some server into its state machine then a different log with the same log index cannot be applied by the other servers.

