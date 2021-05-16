# Seqlock

是在阅读 [P1478R1: Byte-wise atomic memcpy](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1478r1.html) 发现的它。



## wowotech [Linux内核同步机制之（六）：Seqlock](http://www.wowotech.net/kernel_synchronization/seqlock.html)

普通的spin lock对待reader和writer是一视同仁，RW spin lock给reader赋予了更高的优先级，那么有没有让writer优先的锁的机制呢？答案就是seqlock。

## wikipedia [Seqlock](https://en.wikipedia.org/wiki/Seqlock)

A **seqlock** (short for *sequence lock*) is a special [locking](https://en.wikipedia.org/wiki/Lock_(computer)) mechanism used in [Linux](https://en.wikipedia.org/wiki/Linux) for supporting fast writes of shared variables between two parallel [operating system](https://en.wikipedia.org/wiki/Operating_system) routines. The semantics stabilized as of version 2.5.59, and they are present in the 2.6.x stable kernel series. 



## stackoverflow [Why rwlock is more popular than seqlock in linux kernel? [closed]](https://stackoverflow.com/questions/55746320/why-rwlock-is-more-popular-than-seqlock-in-linux-kernel)

[A](https://stackoverflow.com/a/55750699)

A *seqlock* has a strong **limitation**, that *readers* should correctly work with **inconsistent data**.

Not every processing algorithm allows inconstistent data. In most cases, such data can only be *numbers*: integers, booleans, etc. They rarely can be pointers, because a stale pointer may point to the memory which is already freed, so dereferencing such pointer is no-no.

Locks (and rw-locks among them) doesn't have "inconsitent data" limitations, so they can be used in much more cases.

### Example of inconstisten data under seqlock

Assume there are two structure's fields protected by the single seqlock. The first field, `a` is incremented by each "write", the second field, `b` is decremented by each "write". Both fields initially are `0`.

On may assume, that a *reader* will always find `a + b` to be `0`.

But in case of seqlock, this is not true. E.g., between reading `a` and `b` it could a "write", so `a` value will be old, and `b` value will be new, and `a + b` gives `-1`.

