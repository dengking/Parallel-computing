# Spinning lock with timeout

带timeout的加锁方式是非常重要的，那spinning lock如何实现timeout呢？在 rochester paper [Scalable Queue-Based Spin Locks with Timeout](https://www.cs.rochester.edu/u/scott/papers/2001_PPoPP_Timeout.pdf) 中提出了解决方案，参见 `paper` 章节，其中对这篇paper进行了注解。

论文有点复杂，还没有读完。

## pseudocode [Scalable Queue-Based Spin Locks with Timeout](https://www.cs.rochester.edu/research/synchronization/pseudocode/timeout.html)

> NOTE: 这是 rochester paper [Scalable Queue-Based Spin Locks with Timeout](https://www.cs.rochester.edu/u/scott/papers/2001_PPoPP_Timeout.pdf) 的pseudocode 

