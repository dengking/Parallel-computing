# preshing [An Introduction to Lock-Free Programming](https://preshing.com/20120612/an-introduction-to-lock-free-programming/)

## What Is It?

![img](https://preshing.com/images/its-lock-free.png)

## Lock-Free Programming Techniques

![img](https://preshing.com/images/techniques.png)

### Atomic Read-Modify-Write Operations

Atomic operations are ones which manipulate memory in a way that appears indivisible: No thread can observe the operation half-complete. On modern processors, lots of operations are already atomic. For example, aligned reads and writes of simple types are usually atomic.

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

Such loops still qualify as lock-free, because if the test fails for one thread, it means it must have succeeded for another – though some architectures offer a [weaker variant of CAS](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2748.html) where that’s not necessarily true. Whenever implementing a CAS loop, special care must be taken to avoid the [ABA problem](http://en.wikipedia.org/wiki/ABA_problem).

### Sequential Consistency

Sequential consistency means that all threads agree on the order in which memory operations occurred, and that order is consistent with the order of operations in the program source code. Under sequential consistency, it’s impossible to experience memory reordering shenanigans like [the one I demonstrated in a previous post](http://preshing.com/20120515/memory-reordering-caught-in-the-act).

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

Because the C++11 atomic types guarantee sequential consistency, the outcome r1 = r2 = 0 is impossible. To achieve this, the compiler outputs additional instructions behind the scenes – typically memory fences and/or RMW operations. Those additional instructions may make the implementation less efficient compared to one where the programmer has dealt with memory ordering directly.

### Memory Ordering

As the flowchart suggests, any time you do lock-free programming for multicore (or any [symmetric multiprocessor](http://en.wikipedia.org/wiki/Symmetric_multiprocessing)), and your environment does not guarantee sequential consistency, you must consider how to prevent [memory reordering](http://preshing.com/20120515/memory-reordering-caught-in-the-act).

On today’s architectures, the tools to enforce correct memory ordering generally fall into three categories, which prevent both [compiler reordering](http://preshing.com/20120625/memory-ordering-at-compile-time) and [processor reordering](http://preshing.com/20120710/memory-barriers-are-like-source-control-operations):

- A lightweight sync or fence instruction, which I’ll talk about in [future posts](http://preshing.com/20120913/acquire-and-release-semantics);
- A full memory fence instruction, which I’ve [demonstrated previously](http://preshing.com/20120522/lightweight-in-memory-logging);
- Memory operations which provide **acquire or release semantics**.

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