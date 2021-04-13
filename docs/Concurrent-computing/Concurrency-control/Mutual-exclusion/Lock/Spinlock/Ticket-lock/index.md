# Ticket lock

是在思考 "spinning lock with timeout" 时，想到的这个问题

## wikipedia [Ticket lock](https://en.wikipedia.org/wiki/Ticket_lock)

> NOTE: 
>
> 1、非常类似于在银行里面"取号排队、叫号服务"的方式，这是典型的one to many
>
> 2、原理非常简单、实现也非常简单，同时有着非常好的特性

In [computer science](https://en.wikipedia.org/wiki/Computer_science), a **ticket lock** is a synchronization mechanism, or [locking algorithm](https://en.wikipedia.org/wiki/Lock_(computer_science)), that is a type of [spinlock](https://en.wikipedia.org/wiki/Spinlock) that uses "tickets" to control which [thread](https://en.wikipedia.org/wiki/Thread_(computer_science)) of execution is allowed to enter a [critical section](https://en.wikipedia.org/wiki/Critical_section).

### Overview

The basic concept of a ticket lock is similar to the **ticket queue management system**. This is the method that many bakeries(面包店) and delis(熟食店) use to serve customers in the order that they arrive, without making them stand in a line. Generally, there is some type of dispenser(分配器) from which customers pull sequentially numbered tickets upon arrival. The dispenser usually has a sign above or near it stating something like "Please take a number". There is also typically a dynamic sign, usually digital, that displays the ticket number that is now being served. Each time the next ticket number (customer) is ready to be served, the "Now Serving" sign is incremented and the number called out. This allows all of the waiting customers to know how many people are still ahead of them in the queue or line.

> NOTE: 
>
> 1、"**ticket queue management system**"即"取号排队"
>
> 2、相比于普通的lock，它有"queue"

Like this system, a ticket lock is a [first in first out (FIFO)](https://en.wikipedia.org/wiki/FIFO_(computing_and_electronics)) queue-based mechanism. It adds the benefit of [fairness](https://en.wikipedia.org/wiki/Unbounded_nondeterminism) of lock acquisition and works as follows; there are two integer values which begin at 0. The first value is the queue ticket, the second is the dequeue ticket. The queue ticket is the thread's position in the queue, and the dequeue ticket is the ticket, or queue position, that now has the lock (Now Serving).

> NOTE: 
>
> 1、"fairness" 即 公平

#### 运行逻辑

> NOTE: 
>
> 1、从下面的描述库看出，它基本上是模拟了"取号排队、叫号服务"

When a thread arrives, it atomically obtains and then increments the queue ticket. The [atomicity](https://en.wikipedia.org/wiki/Atomicity_(programming)) of this operation is required to prevent two threads from simultaneously being able to obtain the same ticket number. It then compares its ticket value, before the increment, with the dequeue ticket's value. If they are the same, the thread is permitted to enter the critical section. If they are not the same, then another thread must already be in the critical section and this thread must [busy-wait](https://en.wikipedia.org/wiki/Busy_waiting) or yield. When a thread leaves the critical section controlled by the lock, it atomically increments the dequeue ticket. This permits the next waiting thread, the one with the next sequential ticket number, to enter the critical section.[[1\]](https://en.wikipedia.org/wiki/Ticket_lock#cite_note-:0-1)