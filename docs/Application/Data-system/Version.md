# Version

本文总结version在data system中的application。

## Version represents time

本节标题的含义是version表示了time。



## Application

### MVCC

multiple version concurrency control，参见`Concurrent-computing\Concurrency-control\Concurrency-control\Time-based-concurrency-control`章节。

### Schema evaluation

protobuf、thrift都使用version来实现schema evaluation，关于此，可以参见:

1) `Designing Data-IntensiveApplications#CHAPTER 4 Encoding and Evolution`

### Zookeeper

[Nodes and ephemeral nodes](https://zookeeper.apache.org/doc/r3.6.2/zookeeperOver.html#Nodes+and+ephemeral+nodes)

[ZooKeeper Programmer's Guide#ZNodes](https://zookeeper.apache.org/doc/r3.6.2/zookeeperProgrammers.html#sc_zkDataModel_znodes)