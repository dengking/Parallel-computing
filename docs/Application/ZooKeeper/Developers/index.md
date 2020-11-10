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

> NOTE: 这是version技术，参见`Application\Data-system\Version`

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

### ZooKeeper Stat Structure



## [ZooKeeper Sessions](https://zookeeper.apache.org/doc/r3.6.2/zookeeperProgrammers.html#ch_zkSessions)





## [ZooKeeper Watches](https://zookeeper.apache.org/doc/r3.6.2/zookeeperProgrammers.html#ch_zkWatches)





## [ZooKeeper access control using ACLs](https://zookeeper.apache.org/doc/r3.6.2/zookeeperProgrammers.html#sc_ZooKeeperAccessControl)





## [Pluggable ZooKeeper authentication](https://zookeeper.apache.org/doc/r3.6.2/zookeeperProgrammers.html#sc_ZooKeeperPluggableAuthentication)





## [Consistency Guarantees](https://zookeeper.apache.org/doc/r3.6.2/zookeeperProgrammers.html#ch_zkGuarantees)



## [Bindings](https://zookeeper.apache.org/doc/r3.6.2/zookeeperProgrammers.html#ch_bindings)





## [Building Blocks: A Guide to ZooKeeper Operations](https://zookeeper.apache.org/doc/r3.6.2/zookeeperProgrammers.html#ch_guideToZkOperations)