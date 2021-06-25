# Rendezvous hashing

"rendezvous"的意思是约会

## wikipedia [Rendezvous hashing](https://en.wikipedia.org/wiki/Rendezvous_hashing)

**Rendezvous** or **highest random weight (HRW) hashing**[[1\]](https://en.wikipedia.org/wiki/Rendezvous_hashing#cite_note-:0-1)[[2\]](https://en.wikipedia.org/wiki/Rendezvous_hashing#cite_note-:1-2) is an algorithm that allows clients to achieve **distributed agreement** on a set of *k* options out of a possible set of *n* options. A typical application is when clients need to agree on which sites (or proxies) objects are assigned to. As shown below, Rendezvous hashing is more general than [Consistent hashing](https://en.wikipedia.org/wiki/Consistent_hashing), which becomes a special case (for *k* = 1) of Rendezvous hashing.

> NOTE: 
>
> 集合或最高随机权重（HRW）散列[1] [2]是一种算法，允许客户端在一组可能的n个选项中实现对一组k个选项的分布式协议。 典型的应用是当客户需要就将objects分配了哪些站点（或代理）达成一致。 如下所示，Rendezvous哈希比一致哈希更通用，它Rendezvous哈希的一种特殊情况（对于k = 1）。

### History

**Rendezvous hashing** was invented by David Thaler and Chinya Ravishankar at the [University of Michigan](https://en.wikipedia.org/wiki/University_of_Michigan) in 1996[[1\]](https://en.wikipedia.org/wiki/Rendezvous_hashing#cite_note-:0-1) . [Consistent hashing](https://en.wikipedia.org/wiki/Consistent_hashing) appeared a year later in the literature. One of the first applications of rendezvous hashing was to enable multicast clients on the Internet (in contexts such as the [MBONE](https://en.wikipedia.org/wiki/MBONE)) to identify multicast rendezvous points in a distributed fashion.[[3\]](https://en.wikipedia.org/wiki/Rendezvous_hashing#cite_note-3)[[4\]](https://en.wikipedia.org/wiki/Rendezvous_hashing#cite_note-4) It was used in 1998 by Microsoft's [Cache Array Routing Protocol](https://en.wikipedia.org/wiki/Cache_Array_Routing_Protocol)(CARP) for distributed cache coordination and routing.[[5\]](https://en.wikipedia.org/wiki/Rendezvous_hashing#cite_note-carp-5)[[6\]](https://en.wikipedia.org/wiki/Rendezvous_hashing#cite_note-6) Some [Protocol Independent Multicast](https://en.wikipedia.org/wiki/Protocol_Independent_Multicast) routing protocols use **rendezvous hashing** to pick a rendezvous point.[[1\]](https://en.wikipedia.org/wiki/Rendezvous_hashing#cite_note-:0-1)

> NOTE: 
>
> Rendezvous哈希是1996年由密歇根大学的David Thaler和Chinya Ravishankar发明的。 一年后，文献中出现了一致的散列。 集合散列的第一个应用之一是使Internet上的多播客户端能够以分布式方式识别多播集合点。 它于1998年被微软的缓存阵列路由协议用于分布式缓存协调和路由。 某些协议独立多播路由协议使用集合散列来选择集合点。

Given its simplicity and generality, rendezvous hashing has been applied in a wide variety of applications, including mobile caching,[[7\]](https://en.wikipedia.org/wiki/Rendezvous_hashing#cite_note-7) router design,[[8\]](https://en.wikipedia.org/wiki/Rendezvous_hashing#cite_note-8) secure [key establishment](https://en.wikipedia.org/wiki/Key_establishment),[[9\]](https://en.wikipedia.org/wiki/Rendezvous_hashing#cite_note-foisting-9) and sharding and distributed databases.[[10\]](https://en.wikipedia.org/wiki/Rendezvous_hashing#cite_note-10)

> NOTE: 上述这些领域是非常重要的

### The HRW algorithm for rendezvous hashing

> NOTE: 
>
> 未读懂



## TODO

https://github.com/drainingsun/ared

https://en.wikipedia.org/wiki/Distributed_hash_table

https://medium.com/@dgryski/consistent-hashing-algorithmic-tradeoffs-ef6b8e2fcae8


https://link.springer.com/chapter/10.1007/978-3-030-29859-3_3

https://blog.kevingomez.fr/2019/04/11/clusters-and-data-sharding-introducing-rendezvous-hashing/



