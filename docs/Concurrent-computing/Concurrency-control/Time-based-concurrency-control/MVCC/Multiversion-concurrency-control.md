# Multiversion concurrency control

## wikipedia [Multiversion concurrency control](https://en.wikipedia.org/wiki/Multiversion_concurrency_control)

**Multiversion concurrency control** (**MCC** or **MVCC**), is a [concurrency control](https://en.wikipedia.org/wiki/Concurrency_control) method commonly used by [database management systems](https://en.wikipedia.org/wiki/Database_management_system) to provide concurrent access to the database and in programming languages to implement [transactional memory](https://en.wikipedia.org/wiki/Transactional_memory).[[1\]](https://en.wikipedia.org/wiki/Multiversion_concurrency_control#cite_note-1)



### Description

Without concurrency control, if someone is reading from a database at the same time as someone else is writing to it, it is possible that the reader will see a half-written or [inconsistent](https://en.wikipedia.org/wiki/Consistency_(database_systems)) piece of data. 

> NOTE: dirty read



## TODO

shiroyasha [Multi Version Concurrency Control](http://shiroyasha.io/multiversion-concurrency-control.html)



### MCVV + pointer to data  

读加锁，允许dirty read；写创建一个新的version，然后加锁，更新pointer；使用**reference count**来决定何时将旧数据删除。

造成copy的成本。