# 关于本章

本章标题"version-based concurrency control"的含义是"基于时间来进行并发控制"。将multiple version concurrency control收录进来的原因是: version也是表示的时间，它以简单的数字来表示时间含义。



## Version represents time

本节标题的含义是version表示了time。



## Application

version在data system中有着广泛的应用。

### MVCC

multiple version concurrency control，参见`Concurrent-computing\Concurrency-control\Concurrency-control\Time-based-concurrency-control`章节。

### Schema evaluation

protobuf、thrift都使用version来实现schema evaluation，关于此，可以参见:

1) `Designing Data-IntensiveApplications#CHAPTER 4 Encoding and Evolution`

### Zookeeper

[Nodes and ephemeral nodes](https://zookeeper.apache.org/doc/r3.6.2/zookeeperOver.html#Nodes+and+ephemeral+nodes)

[ZooKeeper Programmer's Guide#ZNodes](https://zookeeper.apache.org/doc/r3.6.2/zookeeperProgrammers.html#sc_zkDataModel_znodes)

## Why time can be used in concurrency control?

1、"time"是strict ordering的

2、可以使用timestamp来作为version ID

## Version-based concurrency control is optimistic

根据 `Optimistic-and-pessimistic` 章节的内容可知，version-based concurrency control is optimistic。

