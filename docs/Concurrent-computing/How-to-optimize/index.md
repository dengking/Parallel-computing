# How to optimize？

Optimize的目的是提高**并发**，最终目的是提供performance、遵循optimization principle。



## Read and write tradeoff/根据read and write来选择

在multiple model中，对于shared data的read、write需要进行Concurrency control，在本章总结了多种concurrency control技术，在选择这些技术的时候，非常重要的一点是: read and write tradeoff。

有些技术在read远多于write的时候，是非常有效的。

根据read、write的多少来选择concurrency control technique。

下面是素材:

1、wikipedia [Read-copy-update](http://en.wiki.sxisa.org/wiki/Read-copy-update)

2、wikipedia [Readers–writer lock](https://en.wikipedia.org/wiki/Readers%E2%80%93writer_lock)

3、wikipedia [Seqlock](https://en.wikipedia.org/wiki/Seqlock)



## 合理地设计，减少线程竞争

> TODO: 以redis线程模型为例来进行说明，每个thread一个私有的queue，这样有效地避免竞争

### Maximize cache locality minimize contention

1、`Spinning-lock-optimization`

2、`12-Choose-Concurrency-Friendly-Data-Structures`



### 降低lock的粒度

提高并发的直接做法是降低lock的粒度。



## 使用non-blocking、lock-less技术

它们都能够避免进入system call。

### Thoughts

为了性能，如果加锁的时间不长，可以使用spinning lock，不使用system lock，这样可以避免系统调用问题。



