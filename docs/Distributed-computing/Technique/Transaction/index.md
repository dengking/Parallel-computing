# Distributed transaction

## wikipedia [Distributed transaction](https://en.wikipedia.org/wiki/Distributed_transaction)



## 文章: tianshouzhi [1.0 分布式事务概述](http://www.tianshouzhi.com/api/tutorials/distributed_transaction)



## ucsd [Lecture 8: Transactions, ACID, 2PC, 2PL, Serializability](https://cseweb.ucsd.edu/classes/sp16/cse291-e/applications/ln/lecture8.html)



## stackoverflow [What is a “distributed transaction”?](https://stackoverflow.com/questions/4217270/what-is-a-distributed-transaction)

[A](https://stackoverflow.com/a/4217349)

Usually, transactions occur on one database server:

```SQL
BEGIN TRANSACTION
SELECT something FROM myTable
UPDATE something IN myTable
COMMIT
```

A *distributed transaction* involves multiple servers:

```SQL
BEGIN TRANSACTION
UPDATE amount = amount - 100 IN bankAccounts WHERE accountNr = 1
UPDATE amount = amount + 100 IN someRemoteDatabaseAtSomeOtherBank.bankAccounts WHERE accountNr = 2
COMMIT
```

The difficulty comes from the fact that the servers must *communicate* to ensure that transactional properties such as *atomicity* are satisfied on both servers: If the transaction succeeds, the values must be updated on both servers. If the transaction fails, the transaction must be rollbacked on both servers. It must never happen that the values are updated on one server but not updated on the other.



## cnblogs [分布式事务，两阶段提交协议，三阶段提交协议](https://www.cnblogs.com/balfish/p/8658691.html)