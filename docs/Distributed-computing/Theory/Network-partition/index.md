# Network partition



## wikipedia [Network partition](https://en.wikipedia.org/wiki/Network_partition)

A **network partition** refers to network decomposition（分解） into relatively independent [subnets](https://en.wikipedia.org/wiki/Subnetwork) for their separate optimization as well as network split due to the **failure of network devices**. In both cases the **partition-tolerant** behavior of subnets is expected. This means that even after the network is partitioned into multiple sub-systems, it still works correctly.

> NOTE: 
>
> 网络划分是指将网络分解为相对独立的子网，进行各自的优化，以及由于网络设备故障导致的网络分割。在这两种情况下，子网的分区容忍行为都是预期的。这意味着即使将网络划分为多个子系统，它仍然可以正常工作。

For example, in a network with multiple subnets where nodes A and B are located in one subnet and nodes C and D are in another, a partition（分裂） occurs if the **network switch device** between the two subnets fails. In that case nodes A and B can no longer communicate with nodes C and D, but all nodes A-D work the same as before.



### Network Partition for Optimization

To decompose an [NP-hard](https://en.wikipedia.org/wiki/NP-hardness) network optimization task into subtasks, the network can be decomposed into relatively independent subnets. In order to partition the network, it is useful to visualize it as a weighted complete graph, where each vertex corresponds to a network element, and each edge has a weight equal to the rank of the correlation between each pair of corresponding elements. Then the most irrelevant interactions between elements of network are discarded. Based on the remaining connections, the network is then further split into relatively independent subnets.[[1\]](https://en.wikipedia.org/wiki/Network_partition#cite_note-Network_partition-1) Wherein different allocations of optimized elements predispose alternative splits of the network (Fig. 1). In the case of a large network, the optimization of each subnet can then be performed independently on different computer clusters.

### As a CAP trade-off

> NOTE: 
>
> 当network partition发生的时候，选择consistency还是availability。

The [CAP Theorem](https://en.wikipedia.org/wiki/CAP_Theorem) is based on three trade-offs: [Consistency](https://en.wikipedia.org/wiki/Consistency_(database_systems)), [Availability](https://en.wikipedia.org/wiki/Availability), and **Partition tolerance**. Partition tolerance, in this context, means the ability of a data processing system to continue processing data even if a **network partition** causes communication errors between subsystems.[[2\]](https://en.wikipedia.org/wiki/Network_partition#cite_note-2)

