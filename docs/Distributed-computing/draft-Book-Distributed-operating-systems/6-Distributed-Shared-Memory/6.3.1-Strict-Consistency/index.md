# [6.3.1. Strict Consistency](https://www.e-reading.club/chapter.php/143358/217/Tanenbaum_-_Distributed_operating_systems.html) 



## What is strict consistency

The most stringent（严厉的） **consistency model** is called **strict consistency**. It is defined by the following condition: 

> Any read to a memory location `x` returns the value stored by the most recent write operation to `x`.

This definition is natural and obvious, although it implicitly assumes the existence of **absolute global time** (as in Newtonian physics(牛顿物理学)) so that the determination of "most recent" is unambiguous. 

> NOTE: stricken consistency的要求是非常苛刻的: 它的实现要求**绝对的实时**，但是以目前的技术水平，这是无法实现的。从上面的定义可以看出strict consistency的定义是基于时间的。
>
> 在**分布式**或者**多线程**的情况下其实实现起来是需要考虑很多问题的: 
>
> 分布式系统的挑战: lack of global clock，那分布式系统中，实现strict consistency可能非常困难的。

## 分情况讨论

> NOTE: 原文的后面部分，作者对各种可能的情况下strict consistency的情况进行了分类讨论

### Uniprocessors 

**Uniprocessors** have traditionally observed **strict consistency** and uniprocessor programmers have come to expect such behavior as a matter of course. A system on which the program 

```python
a = 1; 
a = 2; 
print(a); 
```

printed `1` or any value other than `2` would quickly lead to a lot of very agitated（激动的） programmers (in this chapter, `print` is a procedure that prints its parameter or parameters).  

### DSM

In a **DSM**(distributed shared memory) system, the matter is more complicated. Suppose `x` is a variable stored only on machine B. Imagine that a process on machine A reads `x` at time `T1`, which means that a message is then sent to B to get `x`. Slightly later（说明`T2`是晚于`T1`的）, at `T2`, a process on B does a write to `x`. If **strict consistency** holds, the read should always return the old value regardless of where the machines are and how close `T2` is to `T1`. However, if `T2–T1` is, say, 1 nanosecond(两者之间的时间间隔只有1NS), and the machines are 3 meters apart, in order to propagate the read request from A to B to get there before the write, the signal would have to travel at 10 times the speed of light, something forbidden by Einstein's special theory of relativity(狭义相对论). Is it reasonable for programmers to demand that the system be **strictly consistent**, even if this requires violating the laws of physics? 

> NOTE: 上面这段话的含义是: 在目前的技术水平，实现strict ordering是不可能的

### Multiprocessor

This brings us to the matter of the contract between the **software** and the **memory**. If the contract implicitly or explicitly promises **strict consistency**, then the memory had better deliver it. On the other hand, a programmer who really expects **strict consistency**, and whose program fails if it is not present（如果它不出现，则程序就会出现错误）, is living dangerously. Even on a small multiprocessor, if one processor starts to write memory location `a`, and a nanosecond later another processor starts to read `a` the reader will probably get the old value from its local cache. Anyone who writes programs that fail under these circumstances should be made to stay after school and write a program to print 100 times: "I will avoid race conditions." 

> NOTE: 即使在multiprocessor computer中，实现strict consistency也是非常困难的

## Do we really need strict consistency? 

> NOTE: 原文并没有这样的标题，它是我们添加的。我们需要思考，我们是否需要strict consistency的，原文中，作者以足球直播为例来进行说明，从这个举例中，我们可以知道，现实中， 我们往往是不需要strict consistency也能够满足我们的需求的。

As a more realistic example, one could imagine a system to provide sports fans with up-to-the-minute (but perhaps not up-to-the-nanosecond) scores for sporting events worldwide(其实所谓的实时，都存在一定的延时). Answering a query as if it had been made 2 nanoseconds earlier or later might well be acceptable in this case, especially if it gave much better performance by allowing multiple copies of the data to be stored. In this case **strict consistency** is not promised, delivered, or needed. 

## Notation to illustrate consistency

> NOTE: 本节标题的含义是: illustrate consistency的符号，这是作者创造的展示consistency的一种非常简便的方法。

To study **consistency** in detail, we will give numerous examples. To make these examples precise, we need a special notation. In this notation, several processes, *P1* , *P2*, and so on can be shown at different heights in the figure. The operations done by each process are shown horizontally, with time increasing to the right. Straight lines separate the processes. The symbols 

*W(x)a* and *R(y)b* 

mean that a write to `x` with the value `a` and a read from `y` returning `b` have been done, respectively. The initial value of all variables in these diagrams throughout this chapter is assumed to be 0. As an example, in Fig. 6-12(a) *P1* does a write to location `x`, storing the value 1. Later, *P2* reads `x` and sees the 1. This behavior is correct for a **strictly consistent memory**.  

![](https://www.e-reading.club/illustrations/143/143358-Any2FbImgLoader149)

Fig. 6-12.  Behavior of two processes. The horizontal axis is time. (a) **Strictly consistent memory**. (b) Memory that is not **strictly consistent**. 

In contrast, in Fig. 6-12(b), *P2* does a read after the write (possibly only a nanosecond after it, but still after it), and gets 0. A subsequent read gives 1. Such behavior is incorrect for a **strictly consistent memory**. 

## Summary

In summary, when memory is **strictly consistent**, all writes are instantaneously（即可） visible to all processes and an **absolute global time order** is maintained. If a memory location is changed, all subsequent reads from that location see the new value, no matter how soon after the change the reads are done and no matter which processes are doing the reading and where they are located. Similarly, if a read is done, it gets the **then-current value**（当时的当前值）, no matter how quickly the next write is done. 

> NOTE: 这是对strict consistency再一次讨论，显然在当前技术水平是无法实现的。

