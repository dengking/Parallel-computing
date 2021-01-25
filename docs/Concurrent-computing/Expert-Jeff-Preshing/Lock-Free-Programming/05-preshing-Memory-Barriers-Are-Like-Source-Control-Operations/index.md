# preshing [Memory Barriers Are Like Source Control Operations](https://preshing.com/20120710/memory-barriers-are-like-source-control-operations/)

In my last post, I wrote about [memory ordering at compile time](http://preshing.com/20120625/memory-ordering-at-compile-time), which forms one half of the memory ordering puzzle. This post is about the other half: memory ordering at runtime, on the processor itself. Like compiler reordering, processor reordering is invisible to a single-threaded program. It only becomes apparent when [lock-free techniques](http://preshing.com/20120612/an-introduction-to-lock-free-programming) are used – that is, when shared memory is manipulated without any mutual exclusion between threads. However, unlike compiler reordering, the effects of processor reordering are [only visible in multicore and multiprocessor systems](http://preshing.com/20120515/memory-reordering-caught-in-the-act).

## Types of Memory Barrier

![img](https://preshing.com/images/barrier-types.png)

### #LoadLoad

A LoadLoad barrier effectively prevents reordering of loads performed before the barrier with loads performed after the barrier.

![img](https://preshing.com/images/loadload.png)

> NOTE: 注意箭头方向

```C++
if (IsPublished)                   // Load and check shared flag
{
    LOADLOAD_FENCE();              // Prevent reordering of loads
    return Value;                  // Load published value
}
```

### #StoreStore

A StoreStore barrier effectively prevents reordering of stores performed before the barrier with stores performed after the barrier.

```C++
Value = x;                         // Publish some data
STORESTORE_FENCE();
IsPublished = 1;                   // Set shared flag to indicate availability of data
```

### #LoadStore

Unlike `#LoadLoad` and `#StoreStore`, there’s no clever metaphor for `#LoadStore` in terms of source control operations. The best way to understand a `#LoadStore` barrier is, quite simply, in terms of instruction reordering.

### #StoreLoad

A StoreLoad barrier ensures that all stores performed before the barrier are visible to other processors, and that all loads performed after the barrier receive the latest value that is visible at the time of the barrier. In other words, it effectively prevents reordering of all stores before the barrier against all loads after the barrier, respecting the way a [sequentially consistent](http://preshing.com/20120612/an-introduction-to-lock-free-programming#sequential-consistency) multiprocessor would perform those operations.

`#StoreLoad` is unique. It’s the only type of memory barrier that will prevent the result `r1 = r2 = 0` in the example given in [Memory Reordering Caught in the Act](http://preshing.com/20120515/memory-reordering-caught-in-the-act); the same example I’ve repeated earlier in this post.