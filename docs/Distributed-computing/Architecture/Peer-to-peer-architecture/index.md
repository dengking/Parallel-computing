# [Peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer)

**Peer-to-peer** (**P2P**) computing or networking is a [distributed application](https://en.wikipedia.org/wiki/Distributed_application) architecture that **partitions** tasks or workloads between peers. Peers are equally privileged, equipotent（均等的） participants in the application. They are said to form a peer-to-peer network of nodes.

> NOTE: 思考：如何partition？基于什么进行partition？

Peers make a portion of their resources, such as processing power, disk storage or network bandwidth, directly available to other network participants, without the need for central coordination by servers or stable hosts. Peers are both suppliers and consumers of resources, in contrast to the traditional [client-server](https://en.wikipedia.org/wiki/Client-server) model in which the consumption and supply of resources is divided. 

> NOTE: 这段话解释了上一段中所谓的equally privileged、equipotent的含义，简单说来就是peer-to-peer network中，每个节点的同时兼具supplier 和 consumer 角色（或 client 和 server 角色）。在下面的[Architecture](https://en.wikipedia.org/wiki/Peer-to-peer#Architecture)章节也对此进行了说明。

Emerging collaborative P2P systems are going beyond the era of peers doing similar things while sharing resources, and are looking for diverse peers that can bring in unique resources and capabilities to a [virtual community](https://en.wikipedia.org/wiki/Virtual_community) thereby empowering it to engage in greater tasks beyond those that can be accomplished by individual peers, yet that are beneficial to all the peers. 

> NOTE: 这段话的结构是"are going"和"are looking for "是并列的（由"are looking for "前面的"and"连接）。

While P2P systems had previously been used in many application domains, the architecture was popularized by the file sharing system [Napster](https://en.wikipedia.org/wiki/Napster), originally released in 1999. The concept has inspired new structures and philosophies in many areas of human interaction. In such social contexts, [peer-to-peer as a meme](https://en.wikipedia.org/wiki/Peer-to-peer_(meme)) refers to the [egalitarian](https://en.wikipedia.org/wiki/Egalitarianism) [social networking](https://en.wikipedia.org/wiki/Social_network) that has emerged throughout society, enabled by [Internet](https://en.wikipedia.org/wiki/Internet) technologies in general.

## [Historical development](https://en.wikipedia.org/wiki/Peer-to-peer#Historical_development)

The peer-to-peer movement allowed millions of Internet users to connect "directly, forming groups and collaborating to become user-created search engines, virtual supercomputers, and filesystems."

> NOTE: 这句话总结的非常好



## [Architecture](https://en.wikipedia.org/wiki/Peer-to-peer#Architecture)

A peer-to-peer network is designed around the notion of equal *[peer](https://en.wikipedia.org/wiki/Peer_group_(computer_networking))* nodes simultaneously functioning as both "clients" and "servers" to the other nodes on the network. This model of network arrangement differs from the [client–server](https://en.wikipedia.org/wiki/Client–server) model where communication is usually to and from a central server. A typical example of a file transfer that uses the client-server model is the [File Transfer Protocol](https://en.wikipedia.org/wiki/File_Transfer_Protocol) (FTP) service in which the client and server programs are distinct: the clients initiate the transfer, and the servers satisfy these requests.

### Routing and resource discovery

Peer-to-peer networks generally implement some form of **virtual** [overlay network](https://en.wikipedia.org/wiki/Overlay_network) on top of the physical network topology, where the nodes in the overlay form a [subset](https://en.wikipedia.org/wiki/Subset) of the nodes in the physical network. Data is still exchanged directly over the underlying [TCP/IP](https://en.wikipedia.org/wiki/TCP/IP) network, but at the [application layer](https://en.wikipedia.org/wiki/Application_layer) peers are able to communicate with each other directly, via the logical overlay links (each of which corresponds to a path through the underlying physical network). Overlays are used for indexing and peer discovery, and make the P2P system independent from the physical network topology. Based on how the nodes are linked to each other within the overlay network, and how resources are indexed and located, we can classify networks as *unstructured* or *structured* (or as a hybrid between the two).

> NOTE: 读了下面的Structured networks，上面这段话的内容就好理解了。

#### Structured networks

In *structured peer-to-peer networks* the **overlay** is organized into a specific topology, and the protocol ensures that any node can efficiently search the network for a file/resource, even if the resource is extremely rare.

The most common type of structured P2P networks implement a [distributed hash table](https://en.wikipedia.org/wiki/Distributed_hash_table) (DHT), in which a variant of [consistent hashing](https://en.wikipedia.org/wiki/Consistent_hashing) is used to assign ownership of each file to a particular peer. This enables peers to search for resources on the network using a [hash table](https://en.wikipedia.org/wiki/Hash_table): that is, (*key*, *value*) pairs are stored in the DHT, and any participating node can efficiently retrieve the value associated with a given key. 

However, in order to route traffic efficiently through the network, nodes in a structured overlay must maintain lists of neighbors that satisfy specific criteria. This makes them less robust in networks with a high rate of *churn* (i.e. with large numbers of nodes frequently joining and leaving the network). More recent evaluation of P2P resource discovery solutions under real workloads have pointed out several issues in DHT-based solutions such as high cost of advertising/discovering resources and static and dynamic load imbalance.

Notable distributed networks that use DHTs include [Tixati](https://en.wikipedia.org/wiki/Tixati), an alternative to [BitTorrent's](https://en.wikipedia.org/wiki/BitTorrent_(protocol)) distributed tracker, the [Kad network](https://en.wikipedia.org/wiki/Kad_network), the [Storm botnet](https://en.wikipedia.org/wiki/Storm_botnet), [YaCy](https://en.wikipedia.org/wiki/YaCy), and the [Coral Content Distribution Network](https://en.wikipedia.org/wiki/Coral_Content_Distribution_Network). Some prominent research projects include the [Chord project](https://en.wikipedia.org/wiki/Chord_(peer-to-peer)), [Kademlia](https://en.wikipedia.org/wiki/Kademlia), [PAST storage utility](https://en.wikipedia.org/wiki/PAST_storage_utility), [P-Grid](https://en.wikipedia.org/wiki/P-Grid), a self-organized and emerging overlay network, and [CoopNet content distribution system](https://en.wikipedia.org/wiki/CoopNet_content_distribution_system). DHT-based networks have also been widely utilized for accomplishing efficient resource discovery for [grid computing](https://en.wikipedia.org/wiki/Grid_computing) systems, as it aids in resource management and scheduling of applications.

> NOTE: [Redis cluster](https://redis.io/documentation)就是采用的这种结构。

#### Unstructured networks

> NOTE: 目前还没有接触过这种结构的

#### Hybrid models

Hybrid models are a combination of peer-to-peer and client-server models. A common hybrid model is to have a central server that helps peers find each other. 

> NOTE: 目前还没有接触过这种结构

### [Security and trust](https://en.wikipedia.org/wiki/Peer-to-peer#Security_and_trust)



### [Resilient and scalable computer networks](https://en.wikipedia.org/wiki/Peer-to-peer#Resilient_and_scalable_computer_networks)

*See also:* [Wireless mesh network](https://en.wikipedia.org/wiki/Wireless_mesh_network) *and* [Distributed computing](https://en.wikipedia.org/wiki/Distributed_computing)

The decentralized nature of P2P networks increases **robustness** because it removes the [single point of failure](https://en.wikipedia.org/wiki/Reliability_engineering) that can be inherent in a client-server based system. As nodes arrive and demand on the system increases, the total capacity of the system also increases, and the likelihood of failure decreases. If one peer on the network fails to function properly, the whole network is not compromised or damaged. In contrast, in a typical client–server architecture, clients share only their demands with the system, but not their resources. In this case, as more clients join the system, fewer resources are available to serve each client, and if the central server fails, the entire network is taken down.

> NOTE: 这段所阐述的其实的P2P architecture相对于client–server architecture的优势所在。

### [Distributed storage and search](https://en.wikipedia.org/wiki/Peer-to-peer#Distributed_storage_and_search)



## [Applications](https://en.wikipedia.org/wiki/Peer-to-peer#Applications)

### [Content delivery](https://en.wikipedia.org/wiki/Peer-to-peer#Content_delivery)

### [File-sharing networks](https://en.wikipedia.org/wiki/Peer-to-peer#File-sharing_networks)

### [Other P2P applications](https://en.wikipedia.org/wiki/Peer-to-peer#Other_P2P_applications)

- [Bitcoin](https://en.wikipedia.org/wiki/Bitcoin) and alternatives such as [Ether](https://en.wikipedia.org/wiki/Ether_(cryptocurrency)), [Nxt](https://en.wikipedia.org/wiki/Nxt) and [Peercoin](https://en.wikipedia.org/wiki/Peercoin) are peer-to-peer-based [digital](https://en.wikipedia.org/wiki/Digital_currency) [cryptocurrencies](https://en.wikipedia.org/wiki/Cryptocurrency).



如下是补充的：

- [Distributed version control](https://en.wikipedia.org/wiki/Distributed_version_control)
-  [Redis Cluster](https://redis.io/documentation)



# Centralized VS Decentralized

搜罗了一下，维基百科中关于这两者的文章有如下：

- [Decentralized computing](https://en.wikipedia.org/wiki/Decentralized_computing)
- [Centralized computing](https://en.wikipedia.org/wiki/Centralized_computing)
- [Decentralised system](https://en.wikipedia.org/wiki/Decentralised_system)

上述文章中，[Decentralised system](https://en.wikipedia.org/wiki/Decentralised_system)是最最通俗易懂的，前两篇质量一般。