# Spinlock



## wikipedia [Spinlock](https://en.wikipedia.org/wiki/Spinlock)

In [software engineering](https://en.wikipedia.org/wiki/Software_engineering), a **spinlock** is a [lock](https://en.wikipedia.org/wiki/Lock_(computer_science)) which causes a [thread](https://en.wikipedia.org/wiki/Thread_(computer_science)) trying to acquire it to simply wait in a loop ("spin") while repeatedly checking if the lock is available. Since the thread remains active but is not performing a useful task, the use of such a lock is a kind of [busy waiting](https://en.wikipedia.org/wiki/Busy_waiting). Once acquired, spinlocks will usually be held until they are explicitly released, although in some implementations they may be automatically released if the thread being waited on (the one which holds the lock) blocks, or "goes to sleep".

## stackoverflow [What exactly are “spin-locks”?](https://stackoverflow.com/questions/1957398/what-exactly-are-spin-locks)

[A](https://stackoverflow.com/a/1957464)

> NOTE: 解释得非常好

When you use regular locks (mutexes, critical sections etc), operating system puts your thread in the WAIT state and [preempts](https://en.wikipedia.org/wiki/Preemption_(computing)) it by scheduling other threads on the same core. This has a performance penalty if the wait time is really short, because your thread now has to wait for a preemption to receive CPU time again.

Besides, kernel objects are not available in every state of the kernel, such as in an interrupt handler or when paging is not available etc.

Spinlocks don't cause preemption but wait in a loop ("spin") till the other core releases the lock. This prevents the thread from losing its [quantum](https://en.wikipedia.org/wiki/Preemption_(computing)#Time_slice) and continue as soon as the lock gets released. The simple mechanism of spinlocks allows a kernel to utilize it in almost any state.

That's why on a single core machine a spinlock is simply a "disable interrupts" or "raise IRQL" which prevents thread scheduling completely.

Spinlocks ultimately allow kernels to avoid "Big Kernel Lock"s (a lock acquired when core enters kernel and released at the exit) and have granular locking over kernel primitives, causing better multi-processing on multi-core machines thus better performance.

**EDIT**: A question came up: "Does that mean I should use spinlocks wherever possible?" and I'll try to answer it:

As I mentioned, Spinlocks are only useful in places where anticipated waiting time is shorter than a quantum (read: milliseconds) and preemption doesn't make much sense (e.g. kernel objects aren't available).

If waiting time is unknown, or if you're in user mode Spinlocks aren't efficient. You consume 100% CPU time on the waiting core while checking if a spinlock is available. You prevent other threads from running on that core till your quantum expires. This scenario is only feasible for short bursts at kernel level and unlikely an option for a user-mode application.

Here is a question on SO addressing that: [Spinlocks, How Useful Are They?](https://stackoverflow.com/questions/1456225/spinlocks-how-much-useful-are-they)

## stackoverflow [Spinlocks, How Useful Are They?](https://stackoverflow.com/questions/1456225/spinlocks-how-useful-are-they)



## Implementation

stackoverflow [x86 spinlock using cmpxchg](https://stackoverflow.com/questions/6935442/x86-spinlock-using-cmpxchg)