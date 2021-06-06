# nick-black [Fast UNIX Servers](https://nick-black.com/dankwiki/index.php?title=Fast_UNIX_Servers)

> NOTE: 
>
> 一、在 [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/master/folly)/[io](https://github.com/facebook/folly/tree/master/folly/io)/[async](https://github.com/facebook/folly/tree/master/folly/io/async)/[README.md](https://github.com/facebook/folly/blob/master/folly/io/async/README.md) 中，提及了本文:
>
> Facebook has a lot of experience running services. For background reading, see [The C10k problem](http://www.kegel.com/c10k.html) and [Fast UNIX servers](http://nick-black.com/dankwiki/index.php/Fast_UNIX_Servers)
>
> 二、这篇文章非常好，非常详尽



Dan Kegel's classic site "[The C10K Problem](http://www.kegel.com/c10k.html)" (still updated from time to time) put a Promethean order to the arcana(奥秘) of years, with Jeff Darcy's "[High-Performance Server Architecture](http://pl.atyp.us/content/tech/servers.html)" adding to our understanding. I'm collecting here some followup material to these excellent works (and of course the books of W. Richard Stevens, whose torch we merely carry). Some of these techniques have found, or will find, their way into [libtorque](https://nick-black.com/dankwiki/index.php?title=Libtorque), my multithreaded event unification library (and master's thesis).

> NOTE: 
>
> 翻译如下:
>
> "Dan Kegel的经典网站“The C10K Problem”(仍然不时更新)用普罗米修斯式的顺序解释了多年来的奥秘，Jeff Darcy的“高性能服务器架构”增加了我们的理解。
> 我在这里收集了一些关于这些优秀作品的后续材料(当然还有w·理查德·史蒂文斯的书，我们只是拿着他的火炬)。
> 这些技术中的一些已经或将要在我的多线程事件统一库libtorque中找到它们的方法(以及我的硕士论文)。"

## Central Design Principles

Varghese's *[Network Algorithmics: An Interdisciplinary Approach to Designing Fast Networked Devices](http://www.amazon.com/Network-Algorithmics-Interdisciplinary-Designing-Networking/dp/0120884771)* is in a league of its own in this regard.

> NOTE: 
>
> 翻译如下:
>
> "Varghese的《网络算法:设计快速网络设备的跨学科方法》在这方面有自己的见解。"

1、Principle 1: **Exploit all cycles/bandwidth.** Avoid blocking I/O and unnecessary evictions of cache, but prefetch into cache where appropriate (this applies to page caches just as much as processor caches or any other layer of the [memory hierarchy](https://nick-black.com/dankwiki/index.php?title=Architecture#Memory_Hierarchies)). Be prepared to exploit multiple processing elements. Properly align data and avoid cache-aliasing effects. Use jumbo frames in appropriate scenarios and proactively warn on network degradation (e.g., half-duplex Ethernet due to failed link negotiation).

2、Principle 2: **Don't duplicate work.** Avoid unnecessary copies, context switches, system calls and signals. Use double-buffering or ringbuffers, and calls like [Linux's](https://nick-black.com/dankwiki/index.php?title=Linux_APIs) `splice(2)`.

> NOTE: 
>
> "Avoid unnecessary copies" 对应的是 zero copy，比如 " [Linux'shttps://nick-black.com/dankwiki/index.php?title=Linux_APIs `splice(2)`.](https://man7.org/linux/man-pages/man2/splice.2.html)"

3、Principle 3: **Measure, measure, and measure again, preferably automatically.** Hardware, software and networks will all surprise you. Become friends with your hardware's [performance counters](https://nick-black.com/dankwiki/index.php?title=Performance_Counters) and tools like [eBPF](https://nick-black.com/dankwiki/index.php?title=EBPF), dtrace, ktrace, etc. Build explicit support for performance analysis into the application, especially domain-specific statistics.

