# [High-availability cluster](https://en.wikipedia.org/wiki/High-availability_cluster)



**High-availability clusters** (also known as **HA clusters** or **fail-over clusters**) are groups of [computers](https://en.wikipedia.org/wiki/Computer) that support [server](https://en.wikipedia.org/wiki/Server_(computing)) [applications](https://en.wikipedia.org/wiki/Application_software) that can be reliably utilized with [a minimum amount of down-time](https://en.wikipedia.org/wiki/High_availability)（高可用性群集（也称为HA群集或故障转移群集）是支持服务器应用程序的计算机组，可以在最少的停机时间内可靠地利用服务器应用程序）. They operate by using [high availability software](https://en.wikipedia.org/wiki/High_availability_software) to harness（利用） [redundant](https://en.wikipedia.org/wiki/Redundancy_(engineering)) computers in groups or [clusters](https://en.wikipedia.org/wiki/Computer_cluster) that provide continued service when system components fail. Without clustering, if a server running a particular application crashes, the application will be unavailable until the crashed server is fixed. HA clustering remedies（解决） this situation by detecting hardware/software faults, and immediately restarting the application on another system without requiring administrative intervention, a process known as [failover](https://en.wikipedia.org/wiki/Failover). As part of this process, clustering software may configure the node before starting the application on it. For example, appropriate file systems may need to be imported and mounted, network hardware may have to be configured, and some supporting applications may need to be running as well.[[1\]](https://en.wikipedia.org/wiki/High-availability_cluster#cite_note-1)



HA clusters are often used for critical [databases](https://en.wikipedia.org/wiki/Database_management_system), file sharing on a network, business applications, and customer services such as [electronic commerce](https://en.wikipedia.org/wiki/Electronic_commerce)[websites](https://en.wikipedia.org/wiki/Websites).

HA cluster implementations attempt to build redundancy into a cluster to eliminate single points of failure, including multiple network connections and data storage which is redundantly connected via [storage area networks](https://en.wikipedia.org/wiki/Storage_area_network).

**HA clusters** usually use a [heartbeat](https://en.wikipedia.org/wiki/Heartbeat_(computing)) private network connection which is used to monitor the health and status of each node in the cluster. One subtle but serious condition all clustering software must be able to handle is [split-brain](https://en.wikipedia.org/wiki/Split-brain_(computing)), which occurs when all of the private links go down simultaneously, but the cluster nodes are still running. If that happens, each node in the cluster may mistakenly decide that every other node has gone down and attempt to start services that other nodes are still running. Having duplicate instances of services may cause data corruption on the shared storage.

HA群集通常使用心跳专用网络连接，该连接用于监视群集中每个节点的运行状况和状态。 所有群集软件必须能够处理的一个微妙但严峻的情况是裂脑，这是在所有专用链接同时中断但群集节点仍在运行时发生的。 如果发生这种情况，群集中的每个节点可能会错误地确定每个其他节点都已关闭，并尝试启动其他节点仍在运行的服务。 具有重复的服务实例可能会导致共享存储上的数据损坏。

HA clusters often also use [quorum](https://en.wikipedia.org/wiki/Quorum_(distributed_computing)) witness storage (local or cloud) to avoid this scenario. A witness device cannot be shared between two halves of a split cluster, so in the event that all cluster members cannot communicate with each other (e.g., failed heartbeat), if a member cannot access the witness, it cannot become active.

高可用性群集通常还使用仲裁见证存储（本地或云）来避免这种情况。 见证设备无法在拆分后的集群的两半之间共享，因此，如果所有集群成员都无法相互通信（例如，心跳失败），则如果成员无法访问见证，则它也不能处于活动状态。



***SUMMARY*** : 所谓的HA，其实是指系统能够monitor集群中节点的状态，并且在发现节点异常的情况下，系统能够自动进行 [failover](https://en.wikipedia.org/wiki/Failover)，而无需人为的介入；正如在[High availability versus fault tolerance](https://www.ibm.com/support/knowledgecenter/en/SSPHQG_7.2/concept/ha_concepts_fault.html)中所介绍的：highly available environment has a minimal service interruption

## Application design requirements

Not every application can run in a high-availability cluster environment, and the necessary design decisions need to be made early in the software design phase. In order to run in a **high-availability cluster** environment, an application must satisfy at least the following technical requirements, the last two of which are critical to its reliable function in a cluster, and are the most difficult to satisfy fully:

- There must be a relatively easy way to start, stop, force-stop, and check the status of the application. In practical terms, this means the application must have a command line interface or scripts to control the application, including support for multiple instances of the application.

- The application must be able to use **shared storage** ([NAS](https://en.wikipedia.org/wiki/Network-attached_storage)/[SAN](https://en.wikipedia.org/wiki/Storage_Area_Network)).

- Most importantly, the application must store as much of its **state** on non-volatile shared storage as possible. Equally important is the ability to restart on another node at the last state before failure using **the saved state** from the shared storage.

- The application must not corrupt data if it crashes, or restarts from the saved state.

- A number of these constraints can be minimized through the use of virtual server environments, wherein the hypervisor itself is cluster-aware and provides seamless migration of virtual machines (including running memory state) between physical hosts -- see

   

  Microsoft Server 2012 and 2016 Failover Clusters.

  - A key difference between this approach and running cluster-aware applications is that the latter can deal with server application crashes and support live "rolling" software upgrades while maintaining client access to the service (e.g. database), by having one instance provide service while another is being upgraded or repaired. This requires the cluster instances to communicate, flush caches and coordinate file access during hand-off.



## Node configurations

The most common size for an HA cluster is a two-node cluster, since that is the minimum required to provide redundancy, but many clusters consist of many more, sometimes dozens of nodes.

The attached diagram is a good overview of a classic HA cluster, with the caveat that it does not make any mention of quorum/witness functionality (see above).

Such configurations can sometimes be categorized into one of the following models:

- Active/active — Traffic intended for the failed node is either passed onto an existing node or load balanced across the remaining nodes. This is usually only possible when the nodes use a homogeneous software configuration.
- Active/passive — Provides a fully redundant instance of each node, which is only brought online when its associated primary node fails.[[2\]](https://en.wikipedia.org/wiki/High-availability_cluster#cite_note-2) This configuration typically requires the most extra hardware.
- N+1 — Provides a single extra node that is brought online to take over the role of the node that has failed. In the case of heterogeneous software configuration on each primary node, the extra node must be universally capable of assuming any of the roles of the primary nodes it is responsible for. This normally refers to clusters that have multiple services running simultaneously; in the single service case, this degenerates to active/passive.
- N+M — In cases where a single cluster is managing many services, having only one dedicated failover node might not offer sufficient redundancy. In such cases, more than one (M) standby servers are included and available. The number of standby servers is a tradeoff between cost and reliability requirements.
- N-to-1 — Allows the failover standby node to become the active one temporarily, until the original node can be restored or brought back online, at which point the services or instances must be failed-back to it in order to restore high availability.
- N-to-N — A combination of active/active and N+M clusters, N to N clusters redistribute the services, instances or connections from the failed node among the remaining active nodes, thus eliminating (as with active/active) the need for a 'standby' node, but introducing a need for extra capacity on all active nodes.

The terms *logical host* or *cluster logical host* is used to describe the [network address](https://en.wikipedia.org/wiki/Network_address) that is used to access services provided by the cluster. This logical host identity is not tied to a single cluster node. It is actually a network address/hostname that is linked with the service(s) provided by the cluster. If a cluster node with a running database goes down, the database will be restarted on another cluster node.

## Node reliability

HA clusters usually use all available techniques to make the individual systems and shared infrastructure as reliable as possible. These include:

- [Disk mirroring](https://en.wikipedia.org/wiki/Disk_mirroring) (or Redundant Arrays of Independent Disks --RAID) so that failure of internal disks does not result in system crashes. The [Distributed Replicated Block Device](https://en.wikipedia.org/wiki/Distributed_Replicated_Block_Device) is one example.
- [Redundant](https://en.wikipedia.org/wiki/Redundancy_(engineering)) [network](https://en.wikipedia.org/wiki/Computer_network) connections so that single cable, switch, or network interface failures do not result in network outages.
- Redundant [storage area network](https://en.wikipedia.org/wiki/Storage_area_network) (SAN) connections so that single cable, switch, or interface failures do not lead to loss of connectivity to the storage (this would violate [shared nothing architecture](https://en.wikipedia.org/wiki/Shared_nothing_architecture)).
- Redundant [electrical power](https://en.wikipedia.org/wiki/Electric_power) inputs on different circuits, usually both or all protected by [uninterruptible power supply](https://en.wikipedia.org/wiki/Uninterruptible_power_supply) units, and redundant [power supply](https://en.wikipedia.org/wiki/Power_supply) units, so that single power feed, cable, UPS, or power supply failures do not lead to loss of power to the system.

These features help minimize the chances that the clustering failover between systems will be required. In such a failover, the service provided is unavailable for at least a little while, so measures to avoid failover are preferred.

## Failover strategies

Systems that handle failures in distributed computing have different strategies to cure a failure. For instance, the [Apache Cassandra](https://en.wikipedia.org/wiki/Apache_Cassandra) [API](https://en.wikipedia.org/wiki/API) [Hector](https://en.wikipedia.org/wiki/Hector_(API)) defines three ways to configure a failover:

- **Fail Fast**, scripted as "FAIL_FAST", means that the attempt to cure the failure fails if the first node cannot be reached.
- **On Fail, Try One - Next Available**, scripted as "ON_FAIL_TRY_ONE_NEXT_AVAILABLE", means that the system tries one host, the most accessible or available, before giving up.
- **On Fail, Try All**, scripted as "ON_FAIL_TRY_ALL_AVAILABLE", means that the system tries all existing, available nodes before giving up.



## Case study

### Case: Linux-HA

> https://en.wanweibaike.com/wiki-Heartbeat%20(program)