# preshing [Roll Your Own Lightweight Mutex](https://preshing.com/20120226/roll-your-own-lightweight-mutex/)

In an earlier post, I pointed out the [importance of using a lightweight mutex](http://preshing.com/20111124/always-use-a-lightweight-mutex). I also mentioned it was possible to write your own, provided you can live with certain limitations.

Why would you do such a thing? Well, in the past, some platforms (like BeOS) didn’t provide a lightweight mutex in the native API. Today, that’s not really a concern. I’m mainly showing this because it’s an interesting look at implementing synchronization primitives in general. As a curiosity, it just so happens this implementation shaves almost 50% off the overhead of the Windows Critical Section in the uncontended case. [*Update: A closer look shows that under very high contention, a Windows Critical Section still performs much better.*]

For the record, there are numerous ways to write your own mutex – or lock – entirely in user space, each with its own tradeoffs:

1、[Spin locks](http://en.wikipedia.org/wiki/Spinlock). These employ a busy-wait strategy which has the potential to waste CPU time, and in the worst case, can lead to livelock when competing threads run on the same core. Still, some programmers have found measurable speed improvements switching to spin locks in certain cases.



2、[Peterson’s algorithm](http://en.wikipedia.org/wiki/Peterson's_algorithm) is like a spinlock for two threads. A neat trick, but seems useless on today’s platforms. I find it noteworthy because Bartosz Milewski used this algorithm as a case study while [discussing the finer points of the x86 memory model](http://bartoszmilewski.com/2008/11/05/who-ordered-memory-fences-on-an-x86/). Others have discussed Peterson’s lock in [similar contexts](http://objectmix.com/c/742068-subtle-difference-between-c-0x-mm-other-mms.html).

3、Charles Bloom has a [long writeup describing various mutex implementations](http://cbloomrants.blogspot.com/2011/07/07-15-11-review-of-many-mutex.html). Excellent information, but possibly greek to anyone unfamiliar with C++11’s atomics library and [Relacy](http://www.1024cores.net/home/relacy-race-detector)’s ($) notation.