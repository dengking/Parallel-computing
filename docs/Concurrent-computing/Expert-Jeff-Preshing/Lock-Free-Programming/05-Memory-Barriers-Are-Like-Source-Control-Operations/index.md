# preshing [Memory Barriers Are Like Source Control Operations](https://preshing.com/20120710/memory-barriers-are-like-source-control-operations/)

> NOTE: 
>
> 1、在这一节，作者进行了非常好的比喻
>
> 2、C++ 的实现
>
> a、[atomic_thread_fence](https://en.cppreference.com/w/cpp/atomic/atomic_thread_fence) (C++11)
>
> b、[atomic_signal_fence](https://en.cppreference.com/w/cpp/atomic/atomic_signal_fence) (C++11)

## Memory ordering at compile time VS at runtime

In my last post, I wrote about [memory ordering at compile time](http://preshing.com/20120625/memory-ordering-at-compile-time), which forms one half of the memory ordering puzzle. This post is about the other half: memory ordering at runtime, on the processor itself. Like compiler reordering, processor reordering is invisible to a single-threaded program. It only becomes apparent when [lock-free techniques](http://preshing.com/20120612/an-introduction-to-lock-free-programming) are used – that is, when shared memory is manipulated without any mutual exclusion between threads. However, unlike compiler reordering, the effects of processor reordering are [only visible in multicore and multiprocessor systems](http://preshing.com/20120515/memory-reordering-caught-in-the-act).

## What is memory barrier?

You can enforce correct memory ordering on the processor by issuing any instruction which acts as a **memory barrier**. In some ways, this is the only technique you need to know, because when you use such instructions, compiler ordering is taken care of automatically. Examples of instructions which act as memory barriers include (but are not limited to) the following:

1、Certain inline assembly directives in GCC, such as the PowerPC-specific `asm volatile("lwsync" ::: "memory")`

2、Any [Win32 Interlocked operation](http://msdn.microsoft.com/en-us/library/windows/desktop/ms684122.aspx), except on Xbox 360

3、Many operations on [C++11 atomic types](http://en.cppreference.com/w/cpp/atomic/atomic), such as `load(std::memory_order_acquire)`

4、Operations on POSIX mutexes, such as [`pthread_mutex_lock`](http://linux.die.net/man/3/pthread_mutex_lock)

Just as there are many instructions which act as memory barriers, there are many different types of memory barriers to know about. Indeed, not all of the above instructions produce the same kind of memory barrier – leading to another possible area of confusion when writing lock-free code. In an attempt to clear things up to some extent, I’d like to offer an analogy which I’ve found helpful in understanding the vast majority (but not all) of possible memory barrier types.

## Architecture of a typical multicore system

> NOTE: 此处给出了multicore system的非常好的类比/比喻

To begin with, consider the architecture of a typical multicore system. Here’s a device with two cores, each having 32 KiB of private L1 data cache. There’s 1 MiB of L2 cache shared between both cores, and 512 MiB of main memory.

![img](https://preshing.com/images/cpu-diagram.png)

### 类比/比喻

A multicore system is a bit like a group of programmers collaborating on a project using a bizarre kind of source control strategy. For example, the above dual-core system corresponds to a scenario with just two programmers. Let’s name them Larry and Sergey.

![img](https://preshing.com/images/source-control-analogy.png)

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