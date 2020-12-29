# Two-phase commit protocol



## wikipedia [Two-phase commit protocol](https://en.wikipedia.org/wiki/Two-phase_commit_protocol)

## csdn [两阶段提交协议（two phase commit protocol，2PC）](https://blog.csdn.net/shenlan211314/article/details/7283948)



## Example

### sqlite

由于要基于sqlite开发一个基于virtual table的extension，在阅读sqlite [The Virtual Table Mechanism Of SQLite#2.16. The xSync Method](https://sqlite.org/vtab.html#the_xsync_method)时，其中有这样的描述:

> This method signals the start of a two-phase commit on a virtual table. 

显然，这是为了支持transaction的，这让我想起来之前在学习Redis的时候，其中也有描述two-phase commit protocol，所以有必要对Two-phase commit 进行总结。



#### sqlite implementation of Two-phase commit 

如下重要方法: 

xBegin 

xSync 

xCommit 

xRollback 

参见: sqlite [The Virtual Table Mechanism Of SQLite#2.16. The xSync Method](https://sqlite.org/vtab.html#the_xsync_method)

### C++

#### [Generic: Change the Way You Write Exception-Safe Code — Forever](https://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758)

`AddFriend` now has two distinct parts: the *activity* phase, in which the operations occur, and the *commitment* phase, which doesn't throw — it only stops the undo from happening.