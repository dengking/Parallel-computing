# [Leader election](https://en.wikipedia.org/wiki/Leader_election)

In [distributed computing](https://en.wikipedia.org/wiki/Distributed_computing), **leader election** is the process of designating a single [process](https://en.wikipedia.org/wiki/Process_(computing)) as the organizer of some task distributed among several computers (nodes). Before the task is begun, all network nodes are either unaware which node will serve as the "leader" (or [coordinator](https://en.wikipedia.org/wiki/Distributed_computing#Coordinator_election)) of the task, or unable to communicate with the current coordinator. After a **leader election algorithm** has been run, however, each node throughout the network recognizes a particular, unique node as the task leader.

The network nodes communicate among themselves in order to decide which of them will get into the "leader" state. For that, they need some method in order to break the symmetry among them. For example, if each node has unique and comparable identities, then the nodes can compare their identities, and decide that the node with the highest identity is the leader.

The definition of this problem is often attributed to LeLann, who formalized it as a method to create a new token in a token [ring network](https://en.wikipedia.org/wiki/Ring_network) in which the token has been lost.

Leader election algorithms are designed to be economical in terms of total [bytes](https://en.wikipedia.org/wiki/Byte) transmitted, and time. The algorithm suggested by Gallager, Humblet, and Spira[[1\]](https://en.wikipedia.org/wiki/Leader_election#cite_note-1) for general undirected graphs has had a strong impact on the design of distributed algorithms in general, and won the [Dijkstra Prize](https://en.wikipedia.org/wiki/Dijkstra_Prize) for an influential paper in distributed computing.

Many other algorithms have been suggested for different kinds of network [graphs](https://en.wikipedia.org/wiki/Graph_(discrete_mathematics)), such as undirected rings, unidirectional rings, complete graphs, grids, directed Euler graphs, and others. A general method that decouples the issue of the graph family from the design of the leader election algorithm was suggested by Korach, [Kutten](https://en.wikipedia.org/wiki/Shay_Kutten), and [Moran](https://en.wikipedia.org/wiki/Shlomo_Moran).[[2\]](https://en.wikipedia.org/wiki/Leader_election#cite_note-2)

