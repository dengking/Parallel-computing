# Atomic commit



## wikipedia [Atomic commit](https://en.wikipedia.org/wiki/Atomic_commit)

In the field of [computer science](https://en.wikipedia.org/wiki/Computer_science), an **atomic [commit](https://en.wikipedia.org/wiki/Commit_(data_management))** is an operation that applies a set of distinct changes as a single operation. If the changes are applied then the **atomic commit** is said to have succeeded. If there is a failure before the **atomic commit** can be completed then all of the changes completed in the **atomic commit** are reversed. 

This ensures that the system is always left in a consistent state. The other key property of isolation comes from their nature as [atomic](https://en.wikipedia.org/wiki/Atomicity_(database_systems)) operations. Isolation ensures that only one atomic commit is processed at a time. 

The most common uses of atomic commits are in [database systems](https://en.wikipedia.org/wiki/Database_systems) and [version control systems](https://en.wikipedia.org/wiki/Version_control).

The problem with atomic commits is that they require coordination between multiple systems.[[1\]](https://en.wikipedia.org/wiki/Atomic_commit#cite_note-1) As computer networks are unreliable services this means no algorithm can coordinate with all systems as proven in the [Two Generals Problem](https://en.wikipedia.org/wiki/Two_Generals'_Problem). As databases become more and more distributed this coordination will increase the difficulty of making truly atomic commits.[[2\]](https://en.wikipedia.org/wiki/Atomic_commit#cite_note-2)

### Database systems

Atomic commits in database systems fulfil two of the key properties of [ACID](https://en.wikipedia.org/wiki/ACID),[[4\]](https://en.wikipedia.org/wiki/Atomic_commit#cite_note-4) [atomicity](https://en.wikipedia.org/wiki/Atomicity_(database_systems)) and [consistency](https://en.wikipedia.org/wiki/Consistency_(database_systems)). Consistency is only achieved if each change in the atomic commit is consistent.