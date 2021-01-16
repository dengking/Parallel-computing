# [ZooKeeper Programmer's Guide](https://zookeeper.apache.org/doc/r3.6.2/zookeeperProgrammers.html)

## [Introduction](https://zookeeper.apache.org/doc/r3.6.2/zookeeperProgrammers.html#_introduction)

It does not contain source code, but it does assume a familiarity with the problems associated with distributed computing. 

> NOTE: 关于"the problems associated with distributed computing"，参见`Distributed-computing\Theory\The-Trouble-with-Distributed-Systems`。

## [The ZooKeeper Data Model](https://zookeeper.apache.org/doc/r3.6.2/zookeeperProgrammers.html#ch_zkDataModel)

ZooKeeper has a **hierarchal** namespace, much like a distributed file system. 

> NOTE: 关于hierarchal ，参见工程discrete的`Relation-structure-computation\Model\Hierarchy-relation-model`章节；上述**hierarchal** namespace，其实就是一棵树。

### ZNodes

Every node in a ZooKeeper tree is referred to as a *znode*. 

Znodes maintain a **stat** structure that includes: 

1) version numbers for data changes, acl changes

2) timestamps

The version number, together with the timestamp, allows ZooKeeper to validate the cache and to coordinate updates. Each time a znode's data changes, the version number increases.

> NOTE: 这是version技术，参见`Application\Data-system\Version`，在distributed system中有着非常重要的应用，其实它背后的思想有:
>
> 1、make it computational-ordering
>
> 2、lamport clock

For instance, whenever a client retrieves data, it also receives the **version** of the data. And when a client performs an update or a delete, it must supply the **version** of the data of the znode it is changing. If the version it supplies doesn't match the actual version of the data, the update will fail. 

> NOTE: ZK将version暴露给了用户

Znodes are the main entity that a programmer access. They have several characteristics that are worth mentioning here.

#### Watches

Clients can set watches on znodes. Changes to that znode trigger the watch and then clear the watch. When a watch triggers, ZooKeeper sends the client a notification. More information about watches can be found in the section [ZooKeeper Watches](https://zookeeper.apache.org/doc/r3.6.2/zookeeperProgrammers.html#ch_zkWatches).

> NOTE: redis也有类似的功能

#### Data Access

The data stored at each znode in a namespace is read and written **atomically**. 

> NOTE: atomicity是一个非常重要的特性

##### Coordination data

ZooKeeper was not designed to be a general database or large object store. Instead, it manages **coordination data**. This data can come in the form of **configuration**, **status information**, **rendezvous**, etc.  A common property of the various forms of **coordination data** is that they are relatively small: measured in kilobytes. 

If large data storage is needed, the usual pattern of dealing with such data is to store it on a **bulk storage system**, such as NFS or HDFS, and store pointers to the storage locations in ZooKeeper.

#### Ephemeral Nodes

ZooKeeper also has the notion of ephemeral nodes. These znodes exists as long as the session that created the znode is active. When the session ends the znode is deleted. 

#### Sequence Nodes -- Unique Naming

### Time in ZooKeeper

ZooKeeper tracks time multiple ways:

1、**Zxid** Every change to the ZooKeeper state receives a stamp in the form of a *zxid* (ZooKeeper Transaction Id). This exposes the **total ordering** of all changes to ZooKeeper. Each change will have a unique zxid and if zxid1 is smaller than zxid2 then zxid1 **happened before** zxid2.

> NOTE: " **total ordering** "体现了ZK的ordering
>
> "happened-before"是distributed system中，非常重要的一种relation，它是由Leslie Lamport 提出的，参见:
>
> 1、wikipedia [Happened-before](https://en.wikipedia.org/wiki/Happened-before)
>
> 2、https://amturing.acm.org/p558-lamport.pdf

2、**Version numbers** Every change to a node will cause an increase to one of the version numbers of that node. The three version numbers are version (number of changes to the data of a znode), cversion (number of changes to the children of a znode), and aversion (number of changes to the ACL of a znode).

3、**Ticks** When using multi-server ZooKeeper, servers use ticks to define timing of events such as status uploads, session timeouts, connection timeouts between peers, etc. The tick time is only indirectly exposed through the minimum session timeout (2 times the tick time); if a client requests a session timeout less than the minimum session timeout, the server will tell the client that the session timeout is actually the minimum session timeout.

4、**Real time** ZooKeeper doesn't use real time, or clock time, at all except to put timestamps into the stat structure on znode creation and znode modification.

### ZooKeeper Stat Structure



## [ZooKeeper Sessions](https://zookeeper.apache.org/doc/r3.6.2/zookeeperProgrammers.html#ch_zkSessions)

A ZooKeeper client establishes a session with the ZooKeeper service by creating a handle to the service using a language binding.

![State transitions](https://zookeeper.apache.org/doc/r3.6.2/images/state_dia.jpg)

> NOTE: 典型的finite state machine



## [ZooKeeper Watches](https://zookeeper.apache.org/doc/r3.6.2/zookeeperProgrammers.html#ch_zkWatches)

All of the read operations in ZooKeeper - **getData()**, **getChildren()**, and **exists()** - have the option of setting a watch as a side effect. Here is ZooKeeper's definition of a watch: a watch event is one-time trigger, sent to the client that set the watch, which occurs when the data for which the watch was set changes. There are three key points to consider in this definition of a watch:

> NOTE: one-time trigger让我想到了[epoll Triggering modes](https://en.wikipedia.org/wiki/Epoll)，显然它类似于  [edge-triggered](https://en.wikipedia.org/wiki/Interrupt#Edge-triggered) 



## [ZooKeeper access control using ACLs](https://zookeeper.apache.org/doc/r3.6.2/zookeeperProgrammers.html#sc_ZooKeeperAccessControl)





## [Pluggable ZooKeeper authentication](https://zookeeper.apache.org/doc/r3.6.2/zookeeperProgrammers.html#sc_ZooKeeperPluggableAuthentication)





## [Consistency Guarantees](https://zookeeper.apache.org/doc/r3.6.2/zookeeperProgrammers.html#ch_zkGuarantees)



## [Bindings](https://zookeeper.apache.org/doc/r3.6.2/zookeeperProgrammers.html#ch_bindings)





## [Building Blocks: A Guide to ZooKeeper Operations](https://zookeeper.apache.org/doc/r3.6.2/zookeeperProgrammers.html#ch_guideToZkOperations)