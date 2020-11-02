# Apache ZooKeeper

ZooKeeper的关键字是"coordination"，即"协调"。

## 官网 [Apache ZooKeeper](http://zookeeper.apache.org/)



## [ZooKeeper: A Distributed Coordination Service for Distributed Applications](https://zookeeper.apache.org/doc/current/zookeeperOver.html)







## wikipedia [Apache ZooKeeper#Use cases](https://en.wikipedia.org/wiki/Apache_ZooKeeper#Use_cases)

Typical use cases for ZooKeeper are:

1) [Naming service](https://en.wikipedia.org/wiki/Name_service)



2) [Configuration management](https://en.wikipedia.org/wiki/Configuration_management)



3) [Data Synchronization](https://en.wikipedia.org/wiki/Synchronization_(computer_science))



4) [Leader election](https://en.wikipedia.org/wiki/Leader_election)



5) [Message queue](https://en.wikipedia.org/wiki/Message_queue)



6) [Notification system](https://en.wikipedia.org/wiki/Notification_system)



## Doc

[ZooKeeper: A Distributed Coordination Service for Distributed Applications](http://zookeeper.apache.org/doc/r3.6.0/zookeeperOver.html#ch_DesignOverview)





## Q&A

zookeeper主备切换。

一个zookeeper process需要侦听三个端口？
|||
| -------------------------- | ------------------------------------------------------------ |
| `server.id=host:port1:port2` | 其中id为一个数字，表示zk进程的id，这个id也是dataDir目录下myid文件的内容。   host是该zk进程所在机器的IP地址，port1表示follower和leader交换消息所使用的端口，port2表示选举leader所使用的端口。 |
| `dataDir`                  | 其配置的含义跟单机模式下的含义类似，不同的是集群模式下还有一个myid文件。myid文件的内容只有一行，且内容只能为1 - 255之间的数字，这个数字亦即上面介绍server.id中的id，表示zk进程的id。 |
| `Clientport`               | 是指外部客户端连接的端口；                                   |



leader 和 follower 的角色如何进行指定？

与redis不同的是，zookeeper中，leader是由选举产生的，而不是由用户直接进行指定。	

