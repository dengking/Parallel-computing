# Consistent hashing

一致性哈希。

1、cnblogs [给面试官讲明白：一致性Hash的原理和实践](https://www.cnblogs.com/jingangtx/p/11338592.html)

其中结合了具体的案例来说明，比较容易理解。



## wikipedia [Consistent hashing](https://en.wikipedia.org/wiki/Consistent_hashing)

In [computer science](https://en.wikipedia.org/wiki/Computer_science), **consistent hashing** is a special kind of [hashing](https://en.wikipedia.org/wiki/Hash_function) such that when a **hash table** is resized, only $ K/n $ keys need to be remapped(重新策划) on average, where $ K $ is the number of keys, and $ n $ is the number of slots. In contrast, in most traditional [hash tables](https://en.wikipedia.org/wiki/Hash_table), a change in the number of array slots causes nearly all keys to be remapped because the mapping between the keys and the slots is defined by a [modular operation](https://en.wikipedia.org/wiki/Modular_arithmetic).

Consistent hashing achieves some of the goals of [rendezvous hashing](https://en.wikipedia.org/wiki/Rendezvous_hashing) (also called HRW Hashing), which is more general, since consistent hashing has been shown to be a special case of [rendezvous hashing](https://en.wikipedia.org/wiki/Rendezvous_hashing). [Rendezvous hashing](https://en.wikipedia.org/wiki/Rendezvous_hashing) was first described in 1996, while **consistent hashing** appeared in 1997. The two techniques use different algorithms.

### History

The term "**consistent hashing**" was introduced by [Karger](https://en.wikipedia.org/wiki/David_Karger) *et al.* at MIT for use in [**distributed caching**](https://en.wikipedia.org/wiki/Distributed_cache). This academic paper from 1997 introduced the term "consistent hashing" as a way of distributing（分布） requests among a changing population of Web servers. Each **slot** is then represented by a node in a distributed system. The addition (joins) and removal (leaves/failures) of nodes only requires $ K/n $ items to be re-shuffled when the number of slots/nodes change. The authors mention [Linear hashing](https://en.wikipedia.org/wiki/Linear_hashing) and its ability to handle sequential addition and removal of nodes, while **consistent hashing** allows buckets to be added and removed in arbitrary order. [[1\]](https://en.wikipedia.org/wiki/Consistent_hashing#cite_note-KargerEtAl1997-1)



[Teradata](https://en.wikipedia.org/wiki/Teradata) used this technique in their distributed database, released in 1986, although they did not use this term. Teradata still use the concept of a [Hash table](https://en.wikipedia.org/wiki/Hash_table) to fulfill exactly this purpose. [Akamai Technologies](https://en.wikipedia.org/wiki/Akamai_Technologies) was founded in 1998 by the scientists [Daniel Lewin](https://en.wikipedia.org/wiki/Daniel_Lewin) and [F. Thomson Leighton](https://en.wikipedia.org/wiki/F._Thomson_Leighton) (co-authors of the article coining "consistent hashing") to apply this algorithm, which gave birth to the [content delivery network](https://en.wikipedia.org/wiki/Content_delivery_network) industry.

Consistent hashing has also been used to reduce the impact of partial system failures in large Web applications as to allow for robust caches without incurring the system wide fallout of a failure.[[2\]](https://en.wikipedia.org/wiki/Consistent_hashing#cite_note-KargerEtAl1999-2)

The **consistent hashing** concept also applies to the design of [distributed hash tables](https://en.wikipedia.org/wiki/Distributed_hash_table) (DHTs). DHTs use consistent hashing to partition a keyspace among a distributed set of nodes, and additionally provide an **overlay network** that connects nodes such that the node responsible for any key can be efficiently located.

[Rendezvous hashing](https://en.wikipedia.org/wiki/Rendezvous_hashing), designed at the same time as consistent hashing, achieves the same goals using the very different Highest Random Weight (HRW) algorithm.



### Need for consistent hashing

While running collections of caching machines some limitations are experienced. A common way of load balancing $ n $ cache machines is to put object $ o $ in cache machine number $ {\text{hash}}(o)\;\left({\text{mod }}n\right) $. But this will not work if a cache machine is added or removed because $ n $ changes and every object is hashed to a new location. This can be disastrous(灾难性的) since the originating content servers are flooded with requests from the cache machines. Hence consistent hashing is needed to avoid swamping(覆没) of servers.

**Consistent hashing** maps objects to the same cache machine, as far as possible. It means when a cache machine is added, it takes its share of objects from all the other cache machines and when it is removed, its objects are shared among the remaining machines.

The main idea behind the consistent hashing algorithm is to associate each cache with one or more hash value intervals(间隔) where the interval boundaries are determined by calculating the hash of each cache identifier. (The hash function used to define the intervals does not have to be the same function used to hash the cached values. Only the range of the two functions need match.) If the cache is removed its interval is taken over by a cache with an adjacent interval. All the remaining caches are unchanged.

### Technique

**Consistent hashing** is based on mapping each object to a point on a circle (or equivalently, mapping each object to a real angle). The system maps each available machine (or other storage bucket) to many pseudo-randomly distributed points on the same circle.

To find where an object should be placed, the system finds the location of that object's key on the circle; then walks around the circle until falling into the first bucket it encounters (or equivalently, the first available bucket with a higher angle). The result is that each bucket contains all the resources located between each one of its points and the previous points that belong to other buckets.

If a bucket becomes unavailable (for example because the computer it resides on is not reachable), then the points it maps to will be removed. Requests for resources that would have mapped to each of those points now map to the next highest points. Since each bucket is associated with many pseudo-randomly distributed points, the resources that were held by that bucket will now map to many different buckets. The items that mapped to the lost bucket must be redistributed among the remaining ones, but values mapping to other buckets will still do so and do not need to be moved.

A similar process occurs when a bucket is added. By adding new bucket points, we make any resources between those and the points corresponding to the next smaller angles map to the new bucket. These resources will no longer be associated with the previous buckets, and any value previously stored there will not be found by the selection method described above.

The portion of the keys associated with each bucket can be altered by altering the number of angles that bucket maps to.







### Complexity

**Asymptotic time complexities for N nodes (or slots) and** **K keys**

|               | Classic hash table | Consistent hashing |
| :-----------: | :----------------: | ------------------ |
|  add a node   |        O(K)        | O(K/N + log(N))    |
| remove a node |        O(K)        | O(K/N + log(N))    |
|   add a key   |        O(1)        | O(log(N))          |
| remove a key  |        O(1)        | O(log(N))          |

The O(log(N)) complexity for consistent hashing comes from the fact that a binary search among nodes angles is required to find the next node on the ring.

### Examples of use

Some known instances where consistent hashing is used are:

1、[Couchbase](https://en.wikipedia.org/wiki/Couchbase) automated data partitioning

2、Partitioning component of Amazon's storage system [Dynamo](https://en.wikipedia.org/wiki/Dynamo_(storage_system))[[4\]](https://en.wikipedia.org/wiki/Consistent_hashing#cite_note-Amazon2007-4)

3、Data partitioning in [Apache Cassandra](https://en.wikipedia.org/wiki/Apache_Cassandra)[[5\]](https://en.wikipedia.org/wiki/Consistent_hashing#cite_note-Lakshman2010b-5)

4、Data Partitioning in [Voldemort](https://en.wikipedia.org/wiki/Voldemort_(distributed_data_store))[[6\]](https://en.wikipedia.org/wiki/Consistent_hashing#cite_note-6)

5、[Akka](https://en.wikipedia.org/wiki/Akka_(toolkit))'s consistent hashing router[[7\]](https://en.wikipedia.org/wiki/Consistent_hashing#cite_note-akka-routing-7)

6、[Riak](https://en.wikipedia.org/wiki/Riak), a distributed key-value database[[8\]](https://en.wikipedia.org/wiki/Consistent_hashing#cite_note-riak-consistent-hashing-8)



