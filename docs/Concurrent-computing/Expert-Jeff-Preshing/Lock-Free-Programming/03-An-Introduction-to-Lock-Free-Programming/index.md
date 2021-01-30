# preshing [An Introduction to Lock-Free Programming](https://preshing.com/20120612/an-introduction-to-lock-free-programming/)

I was fortunate in that my first introduction to lock-free (also known as lockless) programming was Bruce Dawson’s excellent and comprehensive white paper, [Lockless Programming Considerations](http://msdn.microsoft.com/en-us/library/windows/desktop/ee418650(v=vs.85).aspx). And like many, I’ve had the occasion to put Bruce’s advice into practice developing and debugging lock-free code on platforms such as the Xbox 360.

> NOTE:  [Lockless Programming Considerations](http://msdn.microsoft.com/en-us/library/windows/desktop/ee418650(v=vs.85).aspx) 所链接的文章收录在 "microsoft-Lockless-Programming-Considerations-for-Xbox-360-and-Microsoft-Windows" 章节

## What Is It?

People often describe lock-free programming as programming without mutexes, which are also referred to as [locks](http://preshing.com/20111118/locks-arent-slow-lock-contention-is). That’s true, but it’s only part of the story. The generally accepted definition, based on academic literature, is a bit more broad. At its essence, lock-free is a property used to describe some code, without saying too much about how that code was actually written.

> NOTE: "At its essence, lock-free is a property used to describe some code, without saying too much about how that code was actually written." 这段话的意思是: lock-free 所描述的是 property，它的definition并不涉及实现，因此我们在理解lock free的时候，应该是从性质入手，而不是从实现入手

Basically, if some part of your program satisfies the following conditions, then that part can rightfully be considered lock-free. Conversely, if a given part of your code doesn’t satisfy these conditions, then that part is not lock-free.

![img](https://preshing.com/images/its-lock-free.png)

> NOTE: "can the threads block each other?"的注释 "is there some way to schedule the threads which would lock up indefinitely?" 的字面意思是: 是否有一些方法来调度那些将被无限期锁定的线程，那它要如何理解？

### lock 的含义

> NOTE: lock的含义是: "the possibility of “locking up” the entire application in some way, whether it’s deadlock, livelock – or even due to hypothetical thread scheduling decisions made by your worst enemy"
>
> 因此，lock-less中的lock，除了包括 mutexes，还包括下面的"Example "章节中的例子

In this sense, the *lock* in lock-free does not refer directly to mutexes, but rather to the possibility of “locking up” the entire application in some way, whether it’s deadlock, livelock – or even due to hypothetical thread scheduling decisions made by your worst enemy(下面的"Example"章节中的例子就是对此的说明). That last point sounds funny, but it’s key. Shared mutexes are ruled out(排除) trivially, because as soon as one thread obtains the mutex, your worst enemy could simply never schedule that thread again. Of course, real operating systems don’t work that way – we’re merely defining terms.

### Example 

Here’s a simple example of an operation which contains no mutexes, but is still not lock-free. Initially, `X = 0`. As an exercise for the reader, consider how two threads could be scheduled in a way such that neither thread exits the loop.

```C++
while (X == 0)
{
    X = 1 - X;
}
```

> NOTE: 可能导致"“locking up” the entire application"的调度策略是:
>
> 1、两个thread同时进入`while`
>
> 2、此时`X`为0，第一个thread执行`X = 1 - X;`，则`X`为1；
>
> 3、此时`X`为1，第二个thread执行`X = 1 - X;`，则`X`为0，显然此时回到了原点
>
> 4、按照1-3执行，则永远陷入这个死循环

---

Nobody expects a large application to be entirely lock-free. Typically, we identify a specific set of lock-free operations out of the whole codebase. For example, in a lock-free queue, there might be a handful of lock-free operations such as `push`, `pop`, perhaps `isEmpty`, and so on.

### Definition  by Herlihy & Shavit, authors of [The Art of Multiprocessor Programming](http://www.amazon.com/gp/product/0123973376/ref=as_li_ss_tl?ie=UTF8&tag=preshonprogr-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=0123973376)

> NOTE: 由Herlihy & Shavit给出的lock free的定义

[![img](https://preshing.com/images/art-of-multiprocessor.png)](http://www.amazon.com/gp/product/0123973376/ref=as_li_ss_tl?ie=UTF8&tag=preshonprogr-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=0123973376)



Herlihy & Shavit, authors of [The Art of Multiprocessor Programming](http://www.amazon.com/gp/product/0123973376/ref=as_li_ss_tl?ie=UTF8&tag=preshonprogr-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=0123973376), tend to express such operations as class methods, and offer the following succinct definition of lock-free (see [slide 150](https://docs.google.com/viewer?a=v&q=cache:HaWgz4g5e7QJ:www.elsevierdirect.com/companions/9780123705914/Lecture%20Slides/05~Chapter_05.ppt+&hl=en&gl=ca&pid=bl&srcid=ADGEESghbD6JBTSkCnlPP8ZjPwxS2kM6bbvEGUJaHozCN1CGYW0hnR0WkwmG7LvVj5BUOYZTfTXUClM7uXmr-nXPYlOvZulPJMgYXHaXqqo_m9qkn38gw8qMn01tFoxTmTkvjalHzQOB&sig=AHIEtbRChU00kpYARLAr5Cv5Z5aB2NLo5w)): “In an infinite execution, infinitely often some method call finishes.” In other words, as long as the program is able to keep *calling* those lock-free operations, the number of *completed* calls keeps increasing, no matter what. It is algorithmically impossible for the system to lock up during those operations.

> NOTE: 字面意思: Herlihy & Shavit，《多处理器编程的艺术》的作者，倾向于将这些操作表示为类方法，并提供了以下无锁的简洁定义(见幻灯片150):“在无限执行中，一些方法调用总是无限地结束。
> 换句话说，只要程序能够继续*调用*这些无锁操作，*完成的*调用的数量就会不断增加，无论发生什么。
> 在这些操作期间，系统在算法上是不可能锁定的。

### Consequence of lock-free programming 

> NOTE: lock-free programming 的性性质的一些应用场景

One important consequence of lock-free programming is that if you suspend a single thread, it will never prevent other threads from making progress, as a group, through their own lock-free operations. This hints(暗示) at the value of lock-free programming when writing interrupt handlers and real-time systems, where certain tasks must complete within a certain time limit, no matter what state the rest of the program is in.

A final precision: Operations that are *designed* to block do not disqualify the algorithm. For example, a queue’s pop operation may intentionally block when the queue is empty. The remaining codepaths can still be considered lock-free.

> NOTE: 这一段的字面意思: 最终精度:被设计为阻塞的操作不会取消算法的资格。例如，当队列为空时，队列的pop操作可能会故意阻塞。其余的代码路径仍然可以认为是无锁的。
>
> 含义是: blocking operation并不会使得使用它的algorithm成为非lock-free的

## Lock-Free Programming Techniques

So how do these techniques relate to one another? To illustrate, I’ve put together the following flowchart. I’ll elaborate on each one below.

![img](https://preshing.com/images/techniques.png)

### Atomic Read-Modify-Write Operations

Atomic operations are ones which manipulate memory in a way that appears indivisible: No thread can observe the operation half-complete. On modern processors, lots of operations are already atomic. For example, **aligned reads and writes of simple types are usually atomic**.

![img](https://preshing.com/images/rmw-turnstile-2.png)

[Read-modify-write](http://en.wikipedia.org/wiki/Read-modify-write) (RMW) operations go a step further, allowing you to perform more complex transactions atomically. They’re especially useful when a lock-free algorithm must support multiple writers, because when multiple threads attempt an RMW on the same address, they’ll effectively line up in a row and execute those operations one-at-a-time. I’ve already touched upon RMW operations in this blog, such as when implementing a [lightweight mutex](http://preshing.com/20120226/roll-your-own-lightweight-mutex), a [recursive mutex](http://preshing.com/20120305/implementing-a-recursive-mutex) and a [lightweight logging system](http://preshing.com/20120522/lightweight-in-memory-logging).

Examples of RMW operations include 

1、[`_InterlockedIncrement`](http://msdn.microsoft.com/en-us/library/2ddez55b(v=vs.90).aspx) on Win32, 

2、[`OSAtomicAdd32`](http://developer.apple.com/library/ios/#DOCUMENTATION/System/Conceptual/ManPages_iPhoneOS/man3/OSAtomicAdd32.3.html) on iOS, and 

3、[`std::atomic::fetch_add`](http://www.stdthread.co.uk/doc/headers/atomic/atomic/specializations/integral/fetch_add.html) in C++11. 

Be aware that the C++11 atomic standard does not guarantee that the implementation will be lock-free on every platform, so it’s best to know the capabilities of your platform and toolchain. You can call [`std::atomic<>::is_lock_free`](http://www.stdthread.co.uk/doc/headers/atomic/atomic/specializations/integral/is_lock_free.html) to make sure.

Different CPU families [support RMW in different ways](http://jfdube.wordpress.com/2011/11/30/understanding-atomic-operations/). Processors such as PowerPC and ARM expose [load-link/store-conditional](http://en.wikipedia.org/wiki/Load-link/store-conditional) instructions, which effectively allow you to implement your own RMW primitive at a low level, though this is not often done. The common RMW operations are usually sufficient.

As illustrated by the flowchart, atomic RMWs are a necessary part of lock-free programming even on single-processor systems. Without atomicity, a thread could be interrupted halfway through the transaction, possibly leading to an inconsistent state.

### Compare-And-Swap Loops

Perhaps the most often-discussed RMW operation is [compare-and-swap](http://en.wikipedia.org/wiki/Compare-and-swap) (CAS). On Win32, CAS is provided via a family of intrinsics such as [`_InterlockedCompareExchange`](http://msdn.microsoft.com/en-us/library/ttk2z1ws.aspx). Often, programmers perform compare-and-swap in a loop to repeatedly attempt a transaction. This pattern typically involves copying a shared variable to a local variable, performing some speculative work, and attempting to publish the changes using CAS:

```c++
void LockFreeQueue::push(Node* newHead)
{
    for (;;)
    {
        // Copy a shared variable (m_Head) to a local.
        Node* oldHead = m_Head;

        // Do some speculative work, not yet visible to other threads.
        newHead->next = oldHead;

        // Next, attempt to publish our changes to the shared variable.
        // If the shared variable hasn't changed, the CAS succeeds and we return.
        // Otherwise, repeat.
        if (_InterlockedCompareExchange(&m_Head, newHead, oldHead) == oldHead)
            return;
    }
}
```

> NOTE: 
>
> 代码理解: 如果`m_Head`(当前的head)和`oldHead`(之前的head)相等，说明在这之间，没有thread修改queue，则将`newHead`写入；
>
> 参见 [_InterlockedCompareExchange](https://docs.microsoft.com/en-us/cpp/intrinsics/interlockedcompareexchange-intrinsic-functions?view=msvc-160) :
>
> ```C++
> long _InterlockedCompareExchange(
>    long volatile * Destination,
>    long Exchange,
>    long Comparand
> );
> ```
>
> `_InterlockedCompareExchange` does an atomic comparison of the `Destination` value with the `Comparand` value. If the `Destination` value is equal to the `Comparand` value, the `Exchange` value is stored in the address specified by `Destination`. Otherwise, does no operation.

Such loops still qualify(有资格) as lock-free, because if the test fails for one thread, it means it must have succeeded for another – though some architectures offer a [weaker variant of CAS](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2748.html) where that’s not necessarily true. Whenever implementing a CAS loop, special care must be taken to avoid the [ABA problem](http://en.wikipedia.org/wiki/ABA_problem).

### Sequential Consistency

Sequential consistency means that all threads agree on the order in which memory operations occurred, and that order is consistent with the order of operations in the program source code. Under sequential consistency, it’s impossible to experience memory reordering shenanigans(恶作剧) like [the one I demonstrated in a previous post](http://preshing.com/20120515/memory-reordering-caught-in-the-act).

A simple (but obviously impractical) way to achieve sequential consistency is to disable compiler optimizations and force all your threads to run on a single processor. A processor never sees its own memory effects out of order, even when threads are pre-empted and scheduled at arbitrary times.

Some programming languages offer sequentially consistency even for optimized code running in a multiprocessor environment. In C++11, you can declare all shared variables as C++11 atomic types with default memory ordering constraints. In Java, you can mark all shared variables as `volatile`. Here’s the example from my [previous post](http://preshing.com/20120515/memory-reordering-caught-in-the-act), rewritten in C++11 style:

```C++
std::atomic<int> X(0), Y(0);
int r1, r2;

void thread1()
{
    X.store(1);
    r1 = Y.load();
}

void thread2()
{
    Y.store(1);
    r2 = X.load();
}
```

> NOTE: 这个例子是sequential consistency的典型例子

Because the C++11 atomic types guarantee sequential consistency, the outcome `r1 = r2 = 0` is impossible. To achieve this, the compiler outputs additional instructions behind the scenes – typically memory fences and/or RMW operations. Those additional instructions may make the implementation less efficient compared to one where the programmer has dealt with memory ordering directly.

### Memory Ordering

As the flowchart suggests, any time you do lock-free programming for multicore (or any [symmetric multiprocessor](http://en.wikipedia.org/wiki/Symmetric_multiprocessing)), and your environment does not guarantee sequential consistency, you must consider how to prevent [memory reordering](http://preshing.com/20120515/memory-reordering-caught-in-the-act).

On today’s architectures, the tools to enforce correct memory ordering generally fall into three categories, which prevent both [compiler reordering](http://preshing.com/20120625/memory-ordering-at-compile-time) and [processor reordering](http://preshing.com/20120710/memory-barriers-are-like-source-control-operations):

1、A lightweight sync or fence instruction, which I’ll talk about in [future posts](http://preshing.com/20120913/acquire-and-release-semantics);

2、A full memory fence instruction, which I’ve [demonstrated previously](http://preshing.com/20120522/lightweight-in-memory-logging);

3、Memory operations which provide **acquire or release semantics**.

Acquire semantics prevent memory reordering of operations which follow it in program order, and release semantics prevent memory reordering of operations preceding it. These semantics are particularly suitable in cases when there’s a producer/consumer relationship, where one thread publishes some information and the other reads it. I’ll also talk about this more in a [future post](http://preshing.com/20120913/acquire-and-release-semantics).

### Different Processors Have Different Memory Models

[Different CPU families have different habits](http://www.linuxjournal.com/node/8212/print) when it comes to memory reordering. The rules are documented by each CPU vendor and followed strictly by the hardware. For instance, PowerPC and ARM processors can change the order of memory stores relative to the instructions themselves, but normally, the x86/64 family of processors from Intel and AMD do not. We say the former processors have a more [relaxed memory model](http://preshing.com/20120930/weak-vs-strong-memory-models).

There’s a temptation to abstract away such platform-specific details, especially with C++11 offering us a standard way to write portable lock-free code. But currently, I think most lock-free programmers have at least some appreciation of platform differences. If there’s one key difference to remember, it’s that at the x86/64 instruction level, every load from memory comes with acquire semantics, and every store to memory provides release semantics – at least for non-SSE instructions and non-write-combined memory. As a result, it’s been common in the past to write lock-free code which works on x86/64, but [fails on other processors](http://www.drdobbs.com/parallel/208801974).

If you’re interested in the hardware details of how and why processors perform memory reordering, I’d recommend Appendix C of [Is Parallel Programming Hard](http://kernel.org/pub/linux/kernel/people/paulmck/perfbook/perfbook.2011.01.02a.pdf). In any case, keep in mind that memory reordering can also occur due to compiler reordering of instructions.

In this post, I haven’t said much about the practical side of lock-free programming, such as: When do we do it? How much do we really need? I also haven’t mentioned the importance of validating your lock-free algorithms. Nonetheless, I hope for some readers, this introduction has provided a basic familiarity with lock-free concepts, so you can proceed into the additional reading without feeling too bewildered. As usual, if you spot any inaccuracies, let me know in the comments.

*[This article was featured in Issue #29 of [Hacker Monthly](http://hackermonthly.com/issue-29.html).]*

## Additional References

![img](https://preshing.com/images/concurrency-in-action.png)

- [Anthony Williams’ blog](http://www.justsoftwaresolutions.co.uk/blog/) and his book, [C++ Concurrency in Action](http://www.amazon.com/gp/product/1933988770/ref=as_li_ss_tl?ie=UTF8&tag=preshonprogr-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=1933988770)
- [Dmitriy V’jukov’s website](http://www.1024cores.net/) and various [forum discussions](https://groups.google.com/forum/?fromgroups#!forum/lock-free)
- [Bartosz Milewski’s blog](http://bartoszmilewski.com/)
- Charles Bloom’s [Low-Level Threading series](http://cbloomrants.blogspot.ca/2012/06/06-12-12-another-threading-post-index.html) on his blog
- Doug Lea’s [JSR-133 Cookbook](http://g.oswego.edu/dl/jmm/cookbook.html)
- Howells and McKenney’s [memory-barriers.txt](http://www.kernel.org/doc/Documentation/memory-barriers.txt) document
- Hans Boehm’s [collection of links](http://www.hpl.hp.com/personal/Hans_Boehm/c++mm/) about the C++11 memory model
- Herb Sutter’s [Effective Concurrency](http://www.gotw.ca/publications/) series