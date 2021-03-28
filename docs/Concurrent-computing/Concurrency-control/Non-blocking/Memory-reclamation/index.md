# Memory reclamation

## 为什么需要memory reclamation?

一、在[libcds](https://github.com/khizmax/libcds)的readme中，提及了memory reclamation:

> safe memory reclamation (SMR) algorithms like [Hazard Pointer](http://en.wikipedia.org/wiki/Hazard_pointer) and user-space [RCU](http://en.wikipedia.org/wiki/Read-copy-update) that is used as an epoch-based SMR.

二、"reclamation"的意思是"回收"，"memory reclamation"即"内存回收"，它和我们平时经常所说的"garbage collection"本质是比较相近，不过两者一般用于不同的领域: garbage collection一般用于programming language，memory reclamation一般用于lock-free data structure。

另外，memory reclamation也可以归入memory management领域。

三、在使用类似于copy-on-write、RCU方法来实现lock-free data structure时，并且host programming language是没有garbage-collection的，则需要由programmer自己来实现对old copy的reclamation，即需要由programmer自己来实现一个garbage collection。

## Safe memory reclamation strategy

本节总结一些memory reclamation strategy。



### Quiescent-state-based-reclamation-QSBR

参见 `Quiescent-state-based-reclamation-QSBR` 章节



### Epoch-Based-Reclamation-EBR

参见 `Epoch-Based-Reclamation-EBR` 章节



### Reference counting

一、这是一种非常常见的方法。

二、参见:

1、工程programming-language的`Reference-counting`章节

2、http://www.cse.chalmers.se/~tsigas/papers/MemoryReclamation-ReferenceCounting-ISPAN05.pdf





### stackoverflow [Quiescent State Based Reclamation vs Epoch Based Reclamation](https://stackoverflow.com/questions/36573370/quiescent-state-based-reclamation-vs-epoch-based-reclamation)

I'm studying the various types of memory reclamation strategies for **lock-free data structures** in a non-garbage collected environment (like C or C++).

In my experiments, I've implemented a few of these strategies successfully - specifically, both **Quiescent State Based Reclamation** (QSBR) and **Epoch Based Reclamation** (EBR).

My question concerns one of the key differences between these two strategies.

Firstly, I'm aware of how both **QSBR** and **EBR** work, and have successfully implemented both of these strategies. QSBR and EBR are in fact very similar. Both of them are *deferred reclamation* strategies - meaning, they avoid race conditions when deallocating memory by simply *deferring* the actual deallocation until it can be proven that it is safe to deallocate the memory. With both QSBR and EBR this is achieved using a global "epoch counter", and then various **thread-local epoch counters** for each participating threads.

> NOTE: 
>
> 1、 "**thread-local epoch counters** "如何理解？
>
> 字面意思: 线程局部的epoch计时器

The main difference between QSBR and EBR is that with QSBR, you basically indicate when a thread does *not* have any references to any shared data. With EBR, you indicate when a thread *does* have a reference to shared data. So, in practice, code that uses EBR ends up looking more like a traditional mutex lock/unlock critical section, like:

```C++
enter_critical_section();

/* do some cool lock-free stuff */

exit_critical_section();
```

...whereas, with QSBR, it's more like:

```C++
/* do some cool lock-free stuff */

quiescent_state(); // this thread is done using shared data
```

> NOTE: 
>
> 1、上面的总结非常好
>
> 2、简而言之: 
>
> QSBR需要由application主动地report quiescent state to QSBR library
>
> EBR需要由application主动地report "thread *does* have a reference to shared data"



So they're very similar. However, one key thing I don't really understand is how all the literature indicates that in practice, QSBR has one major disadvantage: it requires *application level* support, meaning that it isn't really suitable for use in a generic library.

This is mentioned in countless journal articles or library documentation, such as for example in http://www.cs.toronto.edu/~tomhart/papers/tomhart_thesis.pdf, it says:

> The fact that QSBR is application-dependent is the fundamental difference between QSBR and EBR. EBR, by definition, detects grace periods at the library level. QSBR, by contrast, requires that the application report quiescent(沉寂的) states to the QSBR library. As we show in Section 5.2, this gives QSBR a significant performance advantage over

> NOTE: 
>
> 1、简而言之: QSBR需要由application主动地report quiescent state to QSBR library。

The docs for the [User-space RCU project](https://lwn.net/Articles/573424/#Using URCU), which uses a variation of QSBR, also says something similar:

> However, each thread must periodically invoke `rcu_quiescent_state()`, just as in the kernel, where `schedule()` must be invoked periodically. Each thread that is to execute RCU read-side critical sections must also invoke `rcu_register_thread()` after thread creation and `rcu_unregister_thread()` before thread exit. These requirements clearly put a stringent(严格的) constraint on the overall application design that, for example, prohibit the use of QSBR RCU in most library code, but in return, QSBR provides unmatched performance.

I'm having difficulty understanding why this is such a problem. What I gather(收集) here is that with QSBR, the application needs to indicate when it enters a **quiescent state**. But I fail to understand *why* this is so hard to do at the library level?

Couldn't a lock-free library that provides data structures such as stacks and queues simply indicate that it is entering a **quiescent state** after each operation completes? Why are there all these caveats about QSBR that indicate it is somehow not easy to use in library code as opposed to application code?

#### Comments

You might want to read [csng.cs.toronto.edu/publication_files/0000/0159/jpdc07.pdf](http://csng.cs.toronto.edu/publication_files/0000/0159/jpdc07.pdf) and you will see "In the trivial case, a thread could declare a quiescent state after every lockless operation, as shown in Listing 1; however, it is often advantageous to declare quiescent states less frequently" – [JJ15k](https://stackoverflow.com/users/1514498/jj15k) [Jul 20 '16 at 22:19](https://stackoverflow.com/questions/36573370/quiescent-state-based-reclamation-vs-epoch-based-reclamation#comment64385038_36573370) 

#### [A](https://stackoverflow.com/a/42224909)



In QSBR `quiescent_state()` can be called at an arbitrary place where the calling thread holds no references to shared objects. On the other hand, in EBR a thread **must** access shared objects within critical section annotated by `enter_critical_section()` and `exit_critical_section`.

What this difference impiles is:

1、QSBR can outperform EBR because *it can be used with less frequent synchronization*. Yes, as you said, QSBR can be used in the similar way with EBR, but that does not provide the efficiency QSBR claims.

2、In a complex application, identifying quiescent state can be hard. This is why quiescent-based techniques such as RCU usage is mainly restricted to specific environment where there is a natural quiecent state (e.g. context switch in Linux kernel).

### 参考资料

1、http://www.cs.toronto.edu/~tomhart/papers/tomhart_thesis.pdf



## Implementation

### [rmind](https://github.com/rmind)/**[libqsbr](https://github.com/rmind/libqsbr)**



### [en4bz](https://github.com/en4bz)/**[lockfree](https://github.com/en4bz/lockfree)**