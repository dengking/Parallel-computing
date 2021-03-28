# Copy on write

1、copy-on-write是实现concurrent lock free data structure的一种常用的技术；

在Read-copy-update 、MVCC中，都可以看到它的影子；

由于会存在多份copy，因此对于这些copy的reclaimation就是一个需要研究的内容，在`Memory-reclaimation`章节对此进行了讨论。

它们都是实现lock free data structure的关键技术之一；

2、memory management

我在Google "is array thread safe"的时候，发现了如下内容:



## openclassrooms [Modify Arrays on Multiple Threads With CopyOnWriteArrayList](https://openclassrooms.com/en/courses/5684021-scale-up-your-code-with-java-concurrency/6677541-modify-arrays-on-multiple-threads-with-copyonwritearraylist)

显然，Java中的`CopyOnWriteArrayList`就是用了copy-on-write技术来实现lock-free data structure；



## stackoverflow [How can CopyOnWriteArrayList be thread-safe?](https://stackoverflow.com/questions/2950871/how-can-copyonwritearraylist-be-thread-safe)

[A](https://stackoverflow.com/a/2950898)

> NOTE: Java是带GC的，因此无需由programmer考虑它的Memory-reclamation

If you look at the underlying array reference you'll see it's marked as `volatile`. When a write operation occurs (such as in the above extract) this `volatile` reference is only updated in the final statement via `setArray`. Up until this point any read operations will return elements from the **old copy** of the array.

The important point is that the **array update is an atomic operation** and hence reads will always see the array in a consistent state.

> NOTE: 不存在half read、half write问题

The advantage of only taking out a lock for write operations is improved throughput for reads: This is because write operations for a `CopyOnWriteArrayList` can potentially be very slow as they involve copying the entire list.



## javamex [Java *copy-on-write* collections](https://www.javamex.com/tutorials/synchronization_concurrency_8_copy_on_write.shtml)





# QT copy on write



## stackoverflow [What is implicit sharing?](https://stackoverflow.com/questions/4637951/what-is-implicit-sharing)

I am building a game engine library in C++. A little while back I was using Qt to build an application and was rather fascinated with its use of [Implicit Sharing](http://doc.trolltech.com/latest/implicit-sharing.html). I am wondering if anybody could explain this technique in greater detail or could offer a simple example of this in action.



### comments

You gave a link to the docs providing an excellent explanation of how it works. It even has a reference to [the thread docs explaining how atomic reference counting helps in multi-threaded apps](http://doc.trolltech.com/latest/threads-modules.html#threads-and-implicitly-shared-classes). If something is still unclear, you should ask a more detailed question about that something. And don't forget you can always look at the Qt's sources. – [Sergei Tachenov](https://stackoverflow.com/users/540312/sergei-tachenov) [Jan 9 '11 at 7:38](https://stackoverflow.com/questions/4637951/what-is-implicit-sharing#comment5104118_4637951) 

The other name for *implicit sharing* is *copy-on-write*. You might want to take a look at questions mentioning this other name on SO like for example this one: [stackoverflow.com/questions/628938/what-is-copy-on-write](http://stackoverflow.com/questions/628938/what-is-copy-on-write) – [Piotr Dobrogost](https://stackoverflow.com/users/95735/piotr-dobrogost) [Jan 9 '11 at 11:14](https://stackoverflow.com/questions/4637951/what-is-implicit-sharing#comment5105131_4637951)

I believe implicit sharing is just Qt's implementation of [copy-on-write](https://en.wikipedia.org/wiki/Copy-on-write). – [HelloGoodbye](https://stackoverflow.com/users/1070480/hellogoodbye) [Sep 15 '15 at 22:09](https://stackoverflow.com/questions/4637951/what-is-implicit-sharing#comment53046333_4637951)



### [A](https://stackoverflow.com/a/4637973)

The key idea behind implicit sharing seems to go around using the more common term *copy-on-write*. The idea behind copy-on-write is to have each object serve as a wrapper around a pointer to the actual implementation. Each implementation object keeps track of the number of pointers into it. Whenever an operation is performed on the wrapper object, it's just forwarded to the implementation object, which does the actual work.

The advantage of this approach is that copying and destruction of these objects are cheap. To make a copy of the object, we just make a new instance of a wrapper, set its pointer to point at the implementation object, and then increment the count of the number of pointers to the object (this is sometimes called the *reference count*, by the way). Destruction is similar - we drop the reference count by one, then see if anyone else is pointing at the implementation. If not, we free its resources. Otherwise, we do nothing and just assume someone else will do the cleanup later.

The challenge in this approach is that it means that multiple different objects will all be pointing at the same implementation. This means that if someone ends up making a change to the implementation, every object referencing that implementation will see the changes - a very serious problem. To fix this, every time an operation is performed that might potentially change the implementation, the operation checks to see if any other objects also reference the implementation by seeing if the reference count is identically 1. If no other objects reference the object, then the operation can just go ahead - there's no possibility of the changes propagating. If there is at least one other object referencing the data, then the wrapper first makes a deep-copy of the implementation for itself and changes its pointer to point to the new object. Now we know there can't be any sharing, and the changes can be made without a hassle.

If you'd like to see some examples of this in action, take a look at lecture examples 15.0 and 16.0 from [Stanford's introductory C++ programming course](http://www.keithschwarz.com/cs106l/fall2010/). It shows how to design an object to hold a list of words using this technique.

Hope this helps!

> NOTE: 上述 15.0、16.0中的code是非常好的



## 15.0 and 16.0 from [Stanford's introductory C++ programming course](http://www.keithschwarz.com/cs106l/fall2010/)



## doc.qt [Implicit Sharing](https://doc.qt.io/qt-5/implicit-sharing.html)

## [Qt-Widgets](https://github.com/Qt-Widgets)/**[thinker-qt-QtAsync](https://github.com/Qt-Widgets/thinker-qt-QtAsync)**