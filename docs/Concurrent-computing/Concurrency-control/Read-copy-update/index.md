# Read-copy-update



## wikipedia [Read-copy-update](https://en.wikipedia.org/wiki/Read-copy-update)

In computer [operating systems](https://infogalactic.com/info/Operating_system), **read-copy-update** (**RCU**) is a [synchronization](https://infogalactic.com/info/Synchronization_(computer_science)) mechanism implementing a kind of [mutual exclusion](https://infogalactic.com/info/Mutual_exclusion)[[note 1\]](https://infogalactic.com/info/Read-copy-update#cite_note-1) that can sometimes be used as an alternative to a [readers-writer lock](https://infogalactic.com/info/Readers-writer_lock). It allows extremely low overhead, [wait-free](https://infogalactic.com/info/Non-blocking_synchronization) reads. However, RCU updates can be expensive, as they must leave the old versions of the data structure in place to accommodate pre-existing readers. These old versions are reclaimed after all pre-existing readers finish their accesses.



## TODO

1) lwn.net [What is RCU, Fundamentally?](https://lwn.net/Articles/262464/)

2) kernel [RCU concepts](https://www.kernel.org/doc/html/latest/RCU/index.html)