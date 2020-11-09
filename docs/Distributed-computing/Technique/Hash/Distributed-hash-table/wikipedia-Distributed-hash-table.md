[TOC]



# [Distributed hash table](https://en.wikipedia.org/wiki/Distributed_hash_table)

> NOTE : redis cluster可以看做是[Distributed hash table](https://en.wikipedia.org/wiki/Distributed_hash_table)；

A **distributed hash table** (**DHT**) is a class of a decentralized [distributed system](https://en.wikipedia.org/wiki/Distributed_computing) that provides a **lookup service** similar to a [hash table](https://en.wikipedia.org/wiki/Hash_table): (*key*, *value*) pairs are stored in a DHT, and any participating [node](https://en.wikipedia.org/wiki/Node_(networking)) can efficiently retrieve the value associated with a given key. *Keys* are unique identifiers which map to particular *values*, which in turn can be anything from addresses, to [documents](https://en.wikipedia.org/wiki/Electronic_document), to arbitrary [data](https://en.wikipedia.org/wiki/Data_(computing)).[[1\]](https://en.wikipedia.org/wiki/Distributed_hash_table#cite_note-StoicaEtAl2001-1) **Responsibility for maintaining the mapping from keys to values is distributed among the nodes, in such a way that a change in the set of participants causes a minimal amount of disruption.** This allows a DHT to [scale](https://en.wikipedia.org/wiki/Scale_(computing)) to extremely large numbers of nodes and to handle continual node arrivals, departures, and failures.

DHTs form an infrastructure that can be used to build more complex services, such as [anycast](https://en.wikipedia.org/wiki/Anycast), cooperative [Web caching](https://en.wikipedia.org/wiki/Web_cache), [distributed file systems](https://en.wikipedia.org/wiki/Distributed_file_system), [domain name services](https://en.wikipedia.org/wiki/Domain_name_system), [instant messaging](https://en.wikipedia.org/wiki/Instant_messaging), [multicast](https://en.wikipedia.org/wiki/Multicast), and also [peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer) [file sharing](https://en.wikipedia.org/wiki/File_sharing) and [content distribution](https://en.wikipedia.org/wiki/Content_distribution) systems. Notable distributed networks that use DHTs include [BitTorrent](https://en.wikipedia.org/wiki/BitTorrent_(protocol))'s distributed tracker, the [Coral Content Distribution Network](https://en.wikipedia.org/wiki/Coral_Content_Distribution_Network), the [Kad network](https://en.wikipedia.org/wiki/Kad_network), the [Storm botnet](https://en.wikipedia.org/wiki/Storm_botnet), the [Tox instant messenger](https://en.wikipedia.org/wiki/Tox_(protocol)), [Freenet](https://en.wikipedia.org/wiki/Freenet), the [YaCy](https://en.wikipedia.org/wiki/YaCy) search engine, and the [InterPlanetary File System](https://en.wikipedia.org/wiki/InterPlanetary_File_System).



[![img](https://upload.wikimedia.org/wikipedia/commons/thumb/9/98/DHT_en.svg/500px-DHT_en.svg.png)](https://en.wikipedia.org/wiki/File:DHT_en.svg)

Distributed hash tables



## History

DHT research was originally motivated, in part, by [peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer) systems such as [Freenet](https://en.wikipedia.org/wiki/Freenet), [gnutella](https://en.wikipedia.org/wiki/Gnutella), [BitTorrent](https://en.wikipedia.org/wiki/BitTorrent_(software)) and [Napster](https://en.wikipedia.org/wiki/Napster), which took advantage of resources distributed across the Internet to provide a single useful application. In particular, they took advantage of increased [bandwidth](https://en.wikipedia.org/wiki/Bandwidth_(computing)) and [hard disk](https://en.wikipedia.org/wiki/Hard_disk) capacity to provide a file-sharing service.[[2\]](https://en.wikipedia.org/wiki/Distributed_hash_table#cite_note-2)

DHT研究最初的动机部分是由Freenet，gnutella，BitTorrent和Napster等对等系统推动的，这些系统利用分布在Internet上的资源来提供单一有用的应用程序。特别是，他们利用增加的带宽和硬盘容量来提供文件共享服务。

These systems differed in how they located the data offered by their peers. Napster, the first large-scale P2P content delivery system, required a **central index server**: each node, upon joining, would send a list of locally held files to the server, which would perform searches and refer the queries to the nodes that held the results. This central component left the system vulnerable to attacks and lawsuits.

这些系统在定位同行提供的数据方面存在差异。 Napster是第一个大规模P2P内容传送系统，需要一个中央索引服务器：每个节点在加入时，会向服务器发送一个本地保存文件列表，这将执行搜索并将查询引用到持有结果。这一核心组件使系统容易受到攻击和诉讼



Gnutella and similar networks moved to a flooding query model – in essence, each search would result in a message being broadcast to every other machine in the network. While avoiding a [single point of failure](https://en.wikipedia.org/wiki/Single_point_of_failure), this method was significantly less efficient than Napster. Later versions of Gnutella clients moved to a dynamic querying model which vastly improved efficiency.[[3\]](https://en.wikipedia.org/wiki/Distributed_hash_table#cite_note-3)

Gnutella和类似的网络转移到洪泛查询模型 - 实质上，每次搜索都会导致消息被广播到网络中的每台其他机器。虽然避免了单点故障，但这种方法的效率明显低于Napster。更高版本的Gnutella客户转向动态查询模型，大大提高了效率。

[Freenet](https://en.wikipedia.org/wiki/Freenet) is fully distributed, but employs a heuristic [key-based routing](https://en.wikipedia.org/wiki/Key-based_routing) in which each file is associated with a key, and files with similar keys tend to cluster on a similar set of nodes. Queries are likely to be routed through the network to such a cluster without needing to visit many peers.[[4\]](https://en.wikipedia.org/wiki/Distributed_hash_table#cite_note-4) However, Freenet does not guarantee that data will be found.

Freenet是完全分布式的，但采用基于密钥的启发式路由，其中每个文件与密钥相关联，具有相似密钥的文件倾向于在类似的节点集上集群。查询很可能通过网络路由到这样的集群，而无需访问许多对等体。[4]但是，Freenet不保证会找到数据。



**Distributed hash table**s use a more structured key-based routing in order to attain both the **decentralization** of Freenet and gnutella, and the efficiency and guaranteed results of Napster. One drawback is that, like Freenet, DHTs only directly support **exact-match search**, rather than **keyword search**, although Freenet's routing algorithm can be generalized to any key type where a closeness operation can be defined.[[5\]](https://en.wikipedia.org/wiki/Distributed_hash_table#cite_note-5)

分布式哈希表使用更加结构化的基于密钥的路由，以实现Freenet和gnutella的分散化，以及Napster的效率和保证结果。 一个缺点是，像Freenet一样，DHT只直接支持精确匹配搜索，而不是关键字搜索，尽管Freenet的路由算法可以推广到任何可以定义接近度操作的密钥类型。



In 2001, four systems—[CAN](https://en.wikipedia.org/wiki/Content_addressable_network),[[6\]](https://en.wikipedia.org/wiki/Distributed_hash_table#cite_note-Ratnasamy01-6) [Chord](https://en.wikipedia.org/wiki/Chord_(peer-to-peer)),[[7\]](https://en.wikipedia.org/wiki/Distributed_hash_table#cite_note-7) [Pastry](https://en.wikipedia.org/wiki/Pastry_(DHT)), and [Tapestry](https://en.wikipedia.org/wiki/Tapestry_(DHT))—ignited（激起） DHTs as a popular research topic. A project called the Infrastructure for Resilient Internet Systems (Iris) was funded by a $12 million grant from the US [National Science Foundation](https://en.wikipedia.org/wiki/National_Science_Foundation) in 2002.[[8\]](https://en.wikipedia.org/wiki/Distributed_hash_table#cite_note-8) Researchers included [Sylvia Ratnasamy](https://en.wikipedia.org/wiki/Sylvia_Ratnasamy), [Ion Stoica](https://en.wikipedia.org/wiki/Ion_Stoica), [Hari Balakrishnan](https://en.wikipedia.org/wiki/Hari_Balakrishnan) and [Scott Shenker](https://en.wikipedia.org/wiki/Scott_Shenker).[[9\]](https://en.wikipedia.org/wiki/Distributed_hash_table#cite_note-9) Outside academia, DHT technology has been adopted as a component of [BitTorrent](https://en.wikipedia.org/wiki/BitTorrent_(protocol)) and in the [Coral Content Distribution Network](https://en.wikipedia.org/wiki/Coral_Content_Distribution_Network).



2001年，四个系统-CAN，[6] Chord，[7] Pastry和Tapestry-点燃了DHT作为一个热门的研究课题。 一个名为“弹性互联网系统基础设施”（Iris）的项目由2002年美国国家科学基金会拨款1200万美元资助。[8] 研究人员包括Sylvia Ratnasamy，Ion Stoica，Hari Balakrishnan和Scott Shenker。[9] 在学术界之外，DHT技术已被采纳为BitTorrent和Coral内容分发网络的一个组成部分。



## Properties



DHTs characteristically emphasize the following properties:

- [Autonomy（自治的） and decentralization](https://en.wikipedia.org/wiki/Decentralized_computing): the nodes collectively form the system without any **central coordination**.
- [Fault tolerance](https://en.wikipedia.org/wiki/Fault_tolerance): the system should be reliable (in some sense) even with nodes continuously joining, leaving, and failing.
- [Scalability](https://en.wikipedia.org/wiki/Scale_(computing)): the system should function efficiently even with thousands or millions of nodes.

***SUMMARY*** : redis cluster也具有上述的property

A key technique used to achieve these goals is that any one node needs to coordinate with only a few other nodes in the system – most commonly, [O](https://en.wikipedia.org/wiki/Big_O_notation)(log *n*) of the $ n $ participants (see below) – so that only a limited amount of work needs to be done for each change in membership.

Some DHT designs seek to be [secure](https://en.wikipedia.org/wiki/Secure_communication) against malicious（恶意的） participants[[10\]](https://en.wikipedia.org/wiki/Distributed_hash_table#cite_note-10) and to allow participants to remain [anonymous](https://en.wikipedia.org/wiki/Anonymity), though this is less common than in many other [peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer) (especially [file sharing](https://en.wikipedia.org/wiki/File_sharing)) systems; see [anonymous P2P](https://en.wikipedia.org/wiki/Anonymous_P2P).

Finally, DHTs must deal with more traditional distributed systems issues such as [load balancing](https://en.wikipedia.org/wiki/Load_balancing_(computing)), [data integrity](https://en.wikipedia.org/wiki/Data_integrity), and performance (in particular, ensuring that operations such as routing and data storage or retrieval complete quickly).



## Structure



The structure of a DHT can be decomposed into several main components.[[11\]](https://en.wikipedia.org/wiki/Distributed_hash_table#cite_note-11)[[12\]](https://en.wikipedia.org/wiki/Distributed_hash_table#cite_note-12) 

- The foundation is an abstract **keyspace**, such as the set of 160-bit [strings](https://en.wikipedia.org/wiki/String_(computer_science)). 
- A **keyspace partitioning** scheme splits ownership of this **keyspace** among the participating nodes. 
- An **overlay network** then connects the nodes, allowing them to find the owner of any **given key** in the **keyspace**.



Once these components are in place, a typical use of the DHT for **storage** and **retrieval** might proceed as follows. Suppose the keyspace is the set of 160-bit strings. To index a file with given *filename* and *data* in the DHT, the [SHA-1](https://en.wikipedia.org/wiki/SHA-1) hash of *filename* is generated, producing a 160-bit key *k*, and a message *put*(*k, data*) is sent to any node participating in the DHT. The message is forwarded from node to node through the **overlay network** until it reaches the single node **responsible** for key *k* as specified by the **keyspace partitioning**. That node then stores the key and the data. Any other client can then retrieve the contents of the file by again hashing *filename* to produce *k* and asking any **DHT node** to find the data associated with *k* with a message *get*(*k*). The message will again be routed through the overlay to the node responsible for *k*, which will reply with the stored *data*.

***SUMMARY*** : 上述是使用 [SHA-1](https://en.wikipedia.org/wiki/SHA-1) hash 来获得abstract keyspace；使用 [Consistent hashing](https://en.wikipedia.org/wiki/Consistent_hashing)、 [Rendezvous hashing](https://en.wikipedia.org/wiki/Rendezvous_hashing)来进行keyspace partition；

***SUMMARY*** : **redis cluster**是否存在上述forward的过程？



The **keyspace partitioning** and **overlay network** components are described below with the goal of capturing the principal ideas common to most DHTs; many designs differ in the details.



### Keyspace partitioning



Most DHTs use some variant of [consistent hashing](https://en.wikipedia.org/wiki/Consistent_hashing) or [rendezvous hashing](https://en.wikipedia.org/wiki/Rendezvous_hashing) to map keys to nodes. The two algorithms appear to have been devised independently and simultaneously to solve the distributed hash table problem.

Both **consistent hashing** and **rendezvous hashing** have the essential property that removal or addition of one node changes only the set of keys owned by the nodes with **adjacent IDs**, and leaves all other nodes unaffected. Contrast this with a traditional [hash table](https://en.wikipedia.org/wiki/Hash_table) in which addition or removal of one bucket causes nearly the entire keyspace to be remapped. Since any change in ownership typically corresponds to [bandwidth](https://en.wikipedia.org/wiki/Bandwidth_(computing))-intensive movement of objects stored in the DHT from one node to another, minimizing such reorganization is required to efficiently support high rates of churn (node arrival and failure).



#### Consistent hashing

Further information: [Consistent hashing](https://en.wikipedia.org/wiki/Consistent_hashing)

Consistent hashing employs a function $ \delta (k_{1},k_{2}) $ that defines an abstract notion of the distance between the keys $ k_{1} $ and $ k_{2} $, which is unrelated to geographical [distance](https://en.wikipedia.org/wiki/Distance) or network [latency](https://en.wikipedia.org/wiki/Latency_(engineering)). Each node is assigned a single key called its *identifier* (ID). A node with ID $ i_{x} $owns all the keys $ k_{m} $ for which $ i_{x} $ is the closest ID, measured according to $ \delta (k_{m},i_{x}) $.

For example, the [Chord DHT](https://en.wikipedia.org/wiki/Chord_(peer-to-peer)) uses **consistent hashing**, which treats nodes as points on a circle, and $ \delta (k_{1},k_{2}) $ is the distance traveling clockwise around the circle from $ k_{1} $ to $ k_{2} $. Thus, the circular keyspace is split into contiguous segments whose endpoints are the **node identifiers**. If $ i_{1} $ and $ i_{2} $ are two adjacent IDs, with a shorter clockwise distance from $ i_{1} $ to $ i_{2} $, then the node with ID $ i_{2} $owns all the keys that fall between $ i_{1} $ and $ i_{2} $.





#### Rendezvous hashing

Further information: [Rendezvous hashing](https://en.wikipedia.org/wiki/Rendezvous_hashing)

In rendezvous hashing, also called highest random weight hashing, all clients use the same hash function *h()* (chosen ahead of time) to associate a key to one of the *n* available servers. Each client has the same list of identifiers {*S*1, *S*2, ..., *S**n* }, one for each server. Given some key *k*, a client computes *n* hash weights *w*1 = *h*(*S*1, *k*), *w*2 = *h*(*S*2, *k*), ..., *w**n* = *h*(*S**n*, *k*). The client associates that key with the server corresponding to the highest hash weight for that key. A server with ID $ S_{x} $ owns all the keys $ k_{m} $ for which the hash weight $ h(S_{x},k_{m}) $ is higher than the hash weight of any other node for that key.



### Overlay network

Each node maintains a set of [links](https://en.wikipedia.org/wiki/Data_link) to other nodes (its *neighbors* or [routing table](https://en.wikipedia.org/wiki/Routing_table)). Together, these links form the [overlay network](https://en.wikipedia.org/wiki/Overlay_network). A node picks its neighbors according to a certain structure, called the [network's topology](https://en.wikipedia.org/wiki/Network_topology).

All DHT topologies share some variant of the most essential property: for any key *k*, each node either has a **node ID** that owns *k* or has a link to a node whose **node ID** is *closer* to *k*, in terms of the **keyspace distance** defined above. It is then easy to **route** a message to the owner of any key *k* using the following [greedy algorithm](https://en.wikipedia.org/wiki/Greedy_algorithm) (that is not necessarily globally optimal): at each step, forward the message to the neighbor whose ID is closest to *k*. When there is no such neighbor, then we must have arrived at the closest node, which is the owner of *k* as defined above. This style of routing is sometimes called [key-based routing](https://en.wikipedia.org/wiki/Key-based_routing).

***SUMMARY*** : **keyspace distance** 在consistent hashing中定义的；

Beyond basic routing correctness, two important constraints on the topology are to guarantee that the maximum number of [hops](https://en.wikipedia.org/wiki/Hop_(networking))（跳） in any route (route length) is low, so that requests complete quickly; and that the maximum number of neighbors of any node (maximum node [degree](https://en.wikipedia.org/wiki/Degree_(graph_theory))) is low, so that maintenance overhead is not excessive. Of course, having shorter routes requires higher [maximum degree](https://en.wikipedia.org/wiki/Maximum_degree). Some common choices for maximum degree and route length are as follows, where *n* is the number of nodes in the DHT, using [Big O notation](https://en.wikipedia.org/wiki/Big_O_notation):



|    Max. degree     |      Max route length      |                           Used in                            |                             Note                             |
| :----------------: | :------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
|      $ O(1) $      |          $ O(n) $          |                                                              | Worst lookup lengths, with likely much slower lookups times  |
|   $ O(\log n) $    |       $ O(\log n) $        | [Chord](https://en.wikipedia.org/wiki/Chord_(peer-to-peer))  [Kademlia](https://en.wikipedia.org/wiki/Kademlia) [Pastry](https://en.wikipedia.org/wiki/Pastry_(DHT))  [Tapestry](https://en.wikipedia.org/wiki/Tapestry_(DHT)) | Most common, but not optimal (degree/route length). Chord is the most basic version, with Kademlia seeming the most popular optimized variant (should have improved average lookup) |
|   $ O(\log n) $    | $ O(\log n/\log(\log n)) $ |        [Koorde](https://en.wikipedia.org/wiki/Koorde)        | Likely would be more complex to implement, but lookups might be faster (have a lower worst case bound) |
| $ O({\sqrt {n}}) $ |          $ O(1) $          |                                                              | Worst local storage needs, with lots of communication after any node connects or disconnects |



### Algorithms for overlay networks

Aside from routing, there exist many algorithms that exploit the structure of the overlay network for sending a message to all nodes, or a subset of nodes, in a DHT.[[16\]](https://en.wikipedia.org/wiki/Distributed_hash_table#cite_note-16) These algorithms are used by applications to do [overlay multicast](https://en.wikipedia.org/wiki/Overlay_multicast), range queries, or to collect statistics. Two systems that are based on this approach are Structella,[[17\]](https://en.wikipedia.org/wiki/Distributed_hash_table#cite_note-17) which implements flooding and random walks on a Pastry overlay, and DQ-DHT,[[18\]](https://en.wikipedia.org/wiki/Distributed_hash_table#cite_note-18) which implements a dynamic querying search algorithm over a Chord network.



## DHT implementations

Most notable differences encountered in practical instances of DHT implementations include at least the following:

- The address space is a parameter of DHT. Several real world DHTs use 128-bit or 160-bit key space
- Some real-world DHTs use hash functions other than SHA-1.
- In the real world the key $ k $ could be a hash of a file's *content* rather than a hash of a file's *name* to provide [content-addressable storage](https://en.wikipedia.org/wiki/Content-addressable_storage), so that renaming of the file does not prevent users from finding it.
- Some DHTs may also publish objects of different types. For example, key $ k $ could be the node $ ID $ and associated data could describe how to contact this node. This allows publication-of-presence information and often used in IM applications, etc. In the simplest case, $ ID $ is just a random number that is directly used as key $ k $ (so in a 160-bit DHT $ ID $ will be a 160-bit number, usually randomly chosen). In some DHTs, publishing of nodes' IDs is also used to optimize DHT operations.
- Redundancy can be added to improve reliability. The $ (k,data) $ key pair can be stored in more than one node corresponding to the key. Usually, rather than selecting just one node, real world DHT algorithms select $ i $ suitable nodes, with $ i $ being an implementation-specific parameter of the DHT. In some DHT designs, nodes agree to handle a certain keyspace range, the size of which may be chosen dynamically, rather than hard-coded.
- Some advanced DHTs like [Kademlia](https://en.wikipedia.org/wiki/Kademlia) perform iterative lookups through the DHT first in order to select a set of suitable nodes and send $ put(k,data) $ messages only to those nodes, thus drastically reducing useless traffic, since published messages are only sent to nodes that seem suitable for storing the key $ k $; and iterative lookups cover just a small set of nodes rather than the entire DHT, reducing useless forwarding. In such DHTs, forwarding of $ put(k,data) $ messages may only occur as part of a self-healing algorithm: if a target node receives a $ put(k,data) $message, but believes that $ k $ is out of its handled range and a closer node (in terms of DHT keyspace) is known, the message is forwarded to that node. Otherwise, data are indexed locally. This leads to a somewhat self-balancing DHT behavior. Of course, such an algorithm requires nodes to publish their presence data in the DHT so the iterative lookups can be performed.







## Examples

DHT protocols and implementations

- [Apache Cassandra](https://en.wikipedia.org/wiki/Apache_Cassandra)
- [BATON Overlay](https://en.wikipedia.org/wiki/BATON_Overlay)
- [Mainline DHT](https://en.wikipedia.org/wiki/Mainline_DHT) - Standard DHT used by BitTorrent (based on [Kademlia](https://en.wikipedia.org/wiki/Kademlia) as provided by Khashmir[[24\]](https://en.wikipedia.org/wiki/Distributed_hash_table#cite_note-24))
- [CAN](https://en.wikipedia.org/wiki/Content_Addressable_Network) (Content Addressable Network)
- [Chord](https://en.wikipedia.org/wiki/Chord_(DHT))
- [Koorde](https://en.wikipedia.org/wiki/Koorde)
- [Kademlia](https://en.wikipedia.org/wiki/Kademlia)
- [Pastry](https://en.wikipedia.org/wiki/Pastry_(DHT))
- [P-Grid](https://en.wikipedia.org/wiki/P-Grid)
- [Riak](https://en.wikipedia.org/wiki/Riak)
- [Tapestry](https://en.wikipedia.org/wiki/Tapestry_(DHT))
- [TomP2P](https://en.wikipedia.org/wiki/TomP2P)
- [Voldemort](https://en.wikipedia.org/wiki/Voldemort_(distributed_data_store))



Applications employing DHTs

- [BTDigg](https://en.wikipedia.org/wiki/BTDigg): [BitTorrent](https://en.wikipedia.org/wiki/BitTorrent_(protocol)) DHT search engine
- [CloudSNAP](https://en.wikipedia.org/wiki/CloudSNAP): a decentralized web application deployment platform
- [Codeen](https://en.wikipedia.org/wiki/Codeen): web caching
- [Coral Content Distribution Network](https://en.wikipedia.org/wiki/Coral_Content_Distribution_Network)
- [FAROO](https://en.wikipedia.org/wiki/FAROO): peer-to-peer Web search engine
- [Freenet](https://en.wikipedia.org/wiki/Freenet): a censorship-resistant anonymous network
- [GlusterFS](https://en.wikipedia.org/wiki/GlusterFS): a distributed file system used for storage virtualization
- [GNUnet](https://en.wikipedia.org/wiki/GNUnet): Freenet-like distribution network including a DHT implementation
- [I2P](https://en.wikipedia.org/wiki/I2P): An open-source anonymous [peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer) network.
- [I2P-Bote](https://en.wikipedia.org/wiki/I2P): serverless secure anonymous e-mail.
- [IPFS](https://en.wikipedia.org/wiki/InterPlanetary_File_System): A content-addressable, peer-to-peer hypermedia distribution protocol
- [JXTA](https://en.wikipedia.org/wiki/JXTA): open-source P2P platform
- [LBRY](https://en.wikipedia.org/w/index.php?title=LBRY&action=edit&redlink=1): a free, open and community-run digital marketplace
- [Oracle Coherence](https://en.wikipedia.org/wiki/Oracle_Coherence): an in-memory data grid built on top of a Java DHT implementation
- [Perfect Dark](https://en.wikipedia.org/wiki/Perfect_Dark_(P2P)): a [peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer) [file-sharing](https://en.wikipedia.org/wiki/File-sharing) application from Japan
- [Retroshare](https://en.wikipedia.org/wiki/Retroshare): a [Friend-to-friend](https://en.wikipedia.org/wiki/Friend-to-friend) network[[25\]](https://en.wikipedia.org/wiki/Distributed_hash_table#cite_note-25)
- [Ring](https://en.wikipedia.org/wiki/Ring_(software)): a privacy-preserving voice, video and chat communication platform, based on a Kademlia-like DHT
- [Tox](https://en.wikipedia.org/wiki/Tox_(protocol)): an [instant messaging](https://en.wikipedia.org/wiki/Instant_messaging) system intended to function as a [Skype](https://en.wikipedia.org/wiki/Skype)replacement
- [Twister](https://en.wikipedia.org/wiki/Twister_(software)): a [microblogging](https://en.wikipedia.org/wiki/Microblogging) [peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer) platform
- [YaCy](https://en.wikipedia.org/wiki/YaCy): a distributed [search engine](https://en.wikipedia.org/wiki/Web_search_engine)

