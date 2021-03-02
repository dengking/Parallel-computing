# [Home](http://www.1024cores.net/home)‎ > ‎[Lockfree Algorithms](http://www.1024cores.net/home/lock-free-algorithms)‎ > ‎[Introduction](http://www.1024cores.net/home/lock-free-algorithms/introduction)

> NOTE: 
>
> 1、wait-free > Lock-free > obstruction-free

I bet you had heard terms like "lockfree" and "waitfree". So what it's all about? Let's start with some definitions.

## **Wait-freedom**

Wait-freedom means that each thread moves forward regardless of external factors like contention from other threads, other thread blocking. Each operations is executed in a bounded number of steps. It's the strongest guarantee for **synchronization algorithms**. Wait-free algorithms usually use such primitives as `atomic_exchange`, 

`atomic_fetch_add` (`InterlockedExchange`, `InterlockedIncrement`, `InterlockedExchangeAdd`, `__sync_fetch_and_add`), 

and they do not contain cycles that can be affected by other threads. `atomic_compare_exchange` primitive (`InterlockedCompareExchange`, `__sync_val_compare_and_swap`) is usually not used, because it is usually tied with a "repeat until succeed" cycle.

> NOTE: 
>
> 上述cycle是什么含义？参见下面的**Lock-freedom**章节中给出的例子；
>
> 上面这段话 + 下面的两个例子，我们可以看出如下对应关系:
>
> 

Below is an example of a **wait-free algorithm**:

```C
void increment_reference_counter(rc_base* obj)
{
    atomic_increment(obj->rc);
}

void decrement_reference_counter(rc_base* obj)
{
    if (0 == atomic_decrement(obj->rc))
        delete obj;
} 
```

> NOTE: 参见:
>
> 1、atomic_increment: [std::atomic_fetch_add](https://en.cppreference.com/w/cpp/atomic/atomic_fetch_add)

Each thread is able to execute the function in a bounded number of steps regardless of any external factors.

## **Lock-freedom**

Lock-freedom means that a system as a whole moves forward regardless of anything. Forward progress for each individual thread is not guaranteed (that is, individual threads can starve). It's a weaker guarantee than wait-freedom. Lockfree algorithms usually use `atomic_compare_exchange` primitive (`InterlockedCompareExchange`, `__sync_val_compare_and_swap`).

An example of a lockfree algorithm is:

```C++
void stack_push(stack* s, node* n)
{
    node* head;
    do
    {
        head = s->head;
        n->next = head;
    }
    while ( ! atomic_compare_exchange(s->head, head, n));
} 
```

> NOTE: 参见:
>
> 1、cppreference [std::atomic_compare_exchange_strong](https://en.cppreference.com/w/cpp/atomic/atomic_compare_exchange)
>
> 上述过程让我想到了busy-waiting、CAS

As can be seen, a thread can "whirl"(回旋) in the cycle theoretically infinitely. But every repeat of the cycle means that some other thread had made forward progress (that is, successfully pushed a node to the stack). A blocked/interrupted/terminated thread can not prevent(阻止) forward progress of other threads. Consequently, the system as a whole undoubtedly makes forward progress.

## **Obstruction-freedom**

> NOTE: "Obstruction"的意思是: 阻碍

Obstruction-freedom guarantee means that a thread makes forward progress only if it does not encounter contention from other threads. That is, two threads can prevent each other's progress and lead to a **livelock**. It's even weaker guarantee than loсk-freedom. This guarantee may look a bit strange at first. However, note that 

(1) blocked/interrupted/terminated threads can not prevent progress of other threads, and 

(2) obstruction-free algorithms can be faster than lockfree algorithms.

I am unable to come up with a single example, so I refer you to the original paper [Obstruction-Free Synchronization: Double-Ended Queues as an Example](http://www.cs.brown.edu/~mph/HerlihyLM03/main.pdf).

## **Termination-safety**

Waitfree, lockfree and obstruction-free algorithms provide a guarantee of termination-safety. That is, a terminated thread does not prevent system-wide forward progress.

> NOTE: 它们能够保证当thread terminate的时候，system-wide forward progress不会被他阻止

