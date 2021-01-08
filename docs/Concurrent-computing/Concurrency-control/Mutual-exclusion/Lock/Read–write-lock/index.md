# Readers–writer lock



## wikipedia [Readers–writer lock](https://en.wikipedia.org/wiki/Readers%E2%80%93writer_lock)

In [computer science](https://en.wikipedia.org/wiki/Computer_science), a **readers–writer** (**single-writer** lock,[[1\]](https://en.wikipedia.org/wiki/Readers–writer_lock#cite_note-Hamilton-1) a **multi-reader** lock,[[2\]](https://en.wikipedia.org/wiki/Readers–writer_lock#cite_note-2) a **push lock**,[[3\]](https://en.wikipedia.org/wiki/Readers–writer_lock#cite_note-3) or an **MRSW lock**) is a [synchronization](https://en.wikipedia.org/wiki/Synchronization_(computer_science)) primitive that solves one of the [readers–writers problems](https://en.wikipedia.org/wiki/Readers–writers_problem). An RW lock allows [concurrent](https://en.wikipedia.org/wiki/Concurrency_control) access for read-only operations, while write operations require exclusive access. This means that multiple threads can read the data in parallel but an exclusive [lock](https://en.wikipedia.org/wiki/Lock_(computer_science)) is needed for writing or modifying data. When a writer is writing the data, all other writers or readers will be blocked until the writer is finished writing. A common use might be to control access to a data structure in memory that cannot be updated [atomically](https://en.wikipedia.org/wiki/Atomicity_(programming)) and is invalid (and should not be read by another thread) until the update is complete.

### Priority policies

> NOTE: 可以根据read and write来选择priority。