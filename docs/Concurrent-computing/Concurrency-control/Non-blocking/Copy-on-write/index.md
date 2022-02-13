# Copy on write in concurrent lock free data structure

一、Copy-on-write是实现concurrent lock free data structure的一种常用的技术: 

1、在Read-copy-update 、MVCC中，都可以看到它的影子；

2、由于会存在多份copy，因此对于这些copy的reclaimation就变得非常重要，对于没有GC的programming language，需要由programmer实现memory reclamation，在`Memory-reclaimation`章节对此进行了讨论。

二、memory management

通过copy on write、memory reclamation，可以发现在lock-free programming中，memory management是一个非常重要的主题。

## 思考: 是否会导致读到旧数据？

在使用copy-on-write来实现concurrent lock free data structure的时候，是否会导致read到旧数据？答案是不会。

### 以RCU为例来进行分析

在RCU中，就使用了copy-on-write技术，下面以RCU为例来进行分析:

使用read-writer lock: 如果以read-writer lock来进行synchronization，则reader会加上read lock，当一个writer尝试去write的时候，显然它此时会被阻塞，显然它此时对shared data的write无法生效，此时reader所读到的还是原来的数据。

使用RCU: writer会写到copy中，然后进行commit。显然reader下次读的时候，能够读到新数据，这个和read-write lock的效果是一致的，但是并发性更高。

## In Java

Java是自带GC的，因此在使用copy on write来实现lock free data structure的时候，就省去了由programmer来实现memory reclamation的负担。

### openclassrooms [Modify Arrays on Multiple Threads With CopyOnWriteArrayList](https://openclassrooms.com/en/courses/5684021-scale-up-your-code-with-java-concurrency/6677541-modify-arrays-on-multiple-threads-with-copyonwritearraylist)

显然，Java中的`CopyOnWriteArrayList`就是用了copy-on-write技术来实现lock-free data structure；



### stackoverflow [How can CopyOnWriteArrayList be thread-safe?](https://stackoverflow.com/questions/2950871/how-can-copyonwritearraylist-be-thread-safe)

[A](https://stackoverflow.com/a/2950898)

> NOTE: Java是带GC的，因此无需由programmer考虑它的Memory-reclamation

If you look at the underlying array reference you'll see it's marked as `volatile`. When a write operation occurs (such as in the above extract) this `volatile` reference is only updated in the final statement via `setArray`. Up until this point any read operations will return elements from the **old copy** of the array.

The important point is that the **array update is an atomic operation** and hence reads will always see the array in a consistent state.

> NOTE: 不存在half read、half write问题

The advantage of only taking out a lock for write operations is improved throughput for reads: This is because write operations for a `CopyOnWriteArrayList` can potentially be very slow as they involve copying the entire list.



### javamex [Java *copy-on-write* collections](https://www.javamex.com/tutorials/synchronization_concurrency_8_copy_on_write.shtml)



## QT copy on write

参见对应章节。

