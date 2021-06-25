# Google Spanner



## paper [Spanner: Google’s Globally Distributed Database](https://dl.acm.org/doi/pdf/10.1145/2491245)



## oschina [Google Spanner 全球级的分布式数据库](https://www.oschina.net/p/google-spanner)

Spanner 是 Google 的全球级的分布式数据库 (Globally-Distributed Database) 。Spanner 可以扩展到数百万的机器，数已百计的数据中心，上万亿的行。更给力的是，除了夸张的扩展性之外，它还能同时通过同步复制和多版本来满足外部一致性，可用性也是很好的。冲破CAP的枷锁，在三者之间完美平衡。

> NOTE: 
>
> 一、"同步复制"即 "synchronous replication"
>
> 二、"多版本"指的是什么？
>
> 三、如何在CAP之间取得平衡？

Spanner是个可扩展，多版本，全球分布式还支持同步复制的数据库。他是Google的第一个可以全球扩展并且支持外部一致 的事务。Spanner能做到这些，离不开一个用GPS和原子钟实现的时间API。这个API能将数据中心之间的时间同步精确到10ms以内。因此有几个 给力的功能：无锁读事务，原子schema修改，读历史数据无block。

> NOTE: 
>
> 一、[distributed computing](https://en.wikipedia.org/wiki/Distributed_computing) 的一个挑战是 [lack of a global clock](https://en.wikipedia.org/wiki/Clock_synchronization) ，
>
> "Spanner能做到这些，离不开一个用GPS和原子钟实现的时间API。这个API能将数据中心之间的时间同步精确到10ms以内"

