# Sequential consistency



## wikipedia [Sequential consistency](https://en.wikipedia.org/wiki/Sequential_consistency)

> 顺序一致性
>
> sequential order 先后顺序，相继顺序

**Sequential consistency** is one of the [consistency models](https://en.wikipedia.org/wiki/Consistency_model) used in the domain of [concurrent computing](https://en.wikipedia.org/wiki/Concurrent_computing) (e.g. in [distributed shared memory](https://en.wikipedia.org/wiki/Distributed_shared_memory), [distributed transactions](https://en.wikipedia.org/wiki/Distributed_transaction), etc.).

It was first defined as the property that requires that

"... the result of any execution is the same as if the operations of all the processors were executed in some sequential order, and the operations of each individual processor appear in this sequence in the order specified by its program."[[1\]](https://en.wikipedia.org/wiki/Sequential_consistency#cite_note-1)

To understand this statement, it is essential to understand one key property of **sequential consistency**: execution order of program in the same processor (or thread) is the same as the **program order**, while（尽管） execution order of program between processors (or threads) is undefined. In an example like this:

```
 processor 1:     <-- A1 run --> <-- B1 run -->        <-- C1 run -->
 processor 2:       <-- A2 run --> <-- B2 run -->
 Time --------------------------------------------------------------------->
```

execution order between A1, B1 and C1 is preserved, that is, A1 runs before B1, and B1 before C1. The same for A2 and B2. But, as execution order between processors is undefined, B2 might run before or after C1 (B2 might physically run before C1, but the effect of B2 might be seen after that of C1, which is the same as "B2 run after C1")

Conceptually, there is single **global memory** and a "switch" that connects an arbitrary processor to memory at any time step. Each **processor** issues memory operations in **program order** and the switch provides the global serialization among all memory operations[[2\]](https://en.wikipedia.org/wiki/Sequential_consistency#cite_note-2)

[![Rsz selection 055.png](https://upload.wikimedia.org/wikipedia/commons/thumb/5/56/Rsz_selection_055.png/120px-Rsz_selection_055.png)](https://en.wikipedia.org/wiki/File:Rsz_selection_055.png)

The sequential consistency is weaker than [strict consistency](https://en.wikipedia.org/wiki/Strict_consistency), which requires a read from a location to return the value of the last write to that location; strict consistency demands that operations be seen in the order in which they were actually issued.

### See also

- [Concurrent data structure](https://en.wikipedia.org/wiki/Concurrent_data_structure)
- [Linearizability](https://en.wikipedia.org/wiki/Linearizability)
- [Serializability](https://en.wikipedia.org/wiki/Serializability)



### `book-Distributed-operating-systems`

在这本书的`6-Distributed-Shared-Memory\6.3.2-Sequential-Consistency`中对sequential

参见`Theory\book-Distributed-operating-systems\6-Distributed-Shared-Memory\6.3.2-Sequential-Consistency`章节。

## Case study

### Case: zookeeper

zookeeper承诺Sequential Consistency，在 [ZooKeeper overview#Guarantees](https://zookeeper.apache.org/doc/r3.6.2/zookeeperOver.html#Guarantees) 中对此进行了说明:

> **Sequential Consistency** - Updates from a **client** will be applied in the order that they were sent.



### Case: C++ memory model

参见工程programming-language的`C++\Language-reference\Basic-concept\Abstract-machine\Memory-model`章节。