# Multiversion concurrency control

## 总结

MVCC:

1、允许multiple reader、multiple writer

2、如果multiple writer同时write，那么就导致同时存在multiple version copy，那MVCC中，如何实现将multiple version copy进行合并/commit的呢？这比较复杂，在 wikipedia [Multiversion concurrency control](https://en.wikipedia.org/wiki/Multiversion_concurrency_control) # [Implementation](https://en.wikipedia.org/wiki/Multiversion_concurrency_control#Implementation) 章节中进行了简单说明

3、MVCC可以作为实现Concurrently-readable Data Structure的参考

4、MVCC中，是否存在dirty read？即没有读到最新的数据？每个reader都读到当前最新时刻的数据，如果同时有writer，writer的修改发生在copy中，不认为是最新的数据，因此可以认为它不存在dirty read

5、不是in-place write，而是write to copy

6、MVCC也有类似于 lock-free data structure memory reclamation 的问题

7、  [space–time tradeoff](https://en.wikipedia.org/wiki/Space–time_tradeoff)

## wikipedia [Multiversion concurrency control](https://en.wikipedia.org/wiki/Multiversion_concurrency_control)

**Multiversion concurrency control** (**MCC** or **MVCC**), is a [concurrency control](https://en.wikipedia.org/wiki/Concurrency_control) method commonly used by [database management systems](https://en.wikipedia.org/wiki/Database_management_system) to provide concurrent access to the database and in programming languages to implement [transactional memory](https://en.wikipedia.org/wiki/Transactional_memory).[[1\]](https://en.wikipedia.org/wiki/Multiversion_concurrency_control#cite_note-1)



### Description

> NOTE: 本质原因还是multiple model中对shared data的read and write问题

Without concurrency control, if someone is reading from a database at the same time as someone else is writing to it, it is possible that the reader will see a half-written or [inconsistent](https://en.wikipedia.org/wiki/Consistency_(database_systems)) piece of data. 

> NOTE: dirty read

For instance, when making a wire transfer between two bank accounts if a reader reads the balance at the bank when the money has been withdrawn from the original account and before it was deposited in the destination account, it would seem that money has disappeared from the bank. 

[Isolation](https://en.wikipedia.org/wiki/ACID#Isolation) is the property that provides guarantees in the concurrent accesses to data. Isolation is implemented by means of a [concurrency control](https://en.wikipedia.org/wiki/Concurrency_control) protocol. The simplest way is to make all readers wait until the writer is done, which is known as a read-write [lock](https://en.wikipedia.org/wiki/Lock_(database)). Locks are known to create contention especially between long read transactions and update transactions. 

> NOTE: lock contention，参见Lock章节

MVCC aims at solving the problem by keeping multiple copies of each data item. In this way, each user connected to the database sees a *snapshot* of the database at a particular instant in time. Any changes made by a writer will not be seen by other users of the database until the changes have been completed (or, in database terms: until the [transaction](https://en.wikipedia.org/wiki/Database_transaction) has been committed.)

> NOTE: 思考: 如何有multiple writer，那如何决定呢？

MVCC introduces the challenge of how to remove versions that become obsolete and will never be read.

> NOTE: 非常类似于 lock-less data structure memory reclamation 的问题

### Implementation

MVCC uses [timestamps](https://en.wikipedia.org/wiki/Timestamp) (**TS**), and *incrementing transaction IDs*, to achieve *transactional consistency*. MVCC ensures a transaction (**T**) never has to wait to *Read* a database object (**P**) by maintaining several versions of the object. Each version of object **P** has both a *Read Timestamp* (**RTS**) and a *Write Timestamp* (**WTS**) which lets a particular transaction **Ti** read the most recent version of the object which precedes the transaction's *Read Timestamp* **RTS**(**Ti**).

> NOTE: 显然MVCC是time-based的

## TODO

shiroyasha [Multi Version Concurrency Control](http://shiroyasha.io/multiversion-concurrency-control.html)


