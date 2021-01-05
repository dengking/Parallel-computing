# How to optimize？

Optimize的目的是提高**并发**。

## 根据read and write来选择

根据read、write的多少来选择concurrency control technique。

下面是素材:

1、wikipedia [Read-copy-update](http://en.wiki.sxisa.org/wiki/Read-copy-update)

2、wikipedia [Readers–writer lock](https://en.wikipedia.org/wiki/Readers%E2%80%93writer_lock)

3、wikipedia [Seqlock](https://en.wikipedia.org/wiki/Seqlock)



## 降低lock的粒度

提高并发的直接做法是降低lock的粒度。