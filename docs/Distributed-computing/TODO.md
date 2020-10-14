# TODO



## Two-phase commit 

由于要基于sqlite开发一个基于virtual table的extension，在阅读sqlite [The Virtual Table Mechanism Of SQLite#2.16. The xSync Method](https://sqlite.org/vtab.html#the_xsync_method)时，其中有这样的描述:

> This method signals the start of a two-phase commit on a virtual table. 

显然，这是为了支持transaction的，这让我想起来之前在学习Redis的时候，其中也有描述two-phase commit protocol，所以有必要对Two-phase commit 进行总结。



### sqlite implementation of Two-phase commit 

如下重要方法: 

xBegin 

xSync 

xCommit 

xRollback 

参见: sqlite [The Virtual Table Mechanism Of SQLite#2.16. The xSync Method](https://sqlite.org/vtab.html#the_xsync_method)



### 文章: tianshouzhi [1.0 分布式事务概述](http://www.tianshouzhi.com/api/tutorials/distributed_transaction)



### wikipedia [Two-phase_commit_protocol](https://en.wikipedia.org/wiki/Two-phase_commit_protocol) 