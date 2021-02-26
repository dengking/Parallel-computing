# Out of order execution and memory reordering

由于Out of order execution(主要是memory reordering)也会对导致shared data线程不安全。

在下面文章、章节中，讨论了Out of order execution 和 memory reordering: 



1、"aristeia-C++and-the-Perils-of-Double-Checked-Locking" 章节

其中对此进行了非常好的介绍，这篇文章非常好

2、zhihu [如何理解 C++11 的六种 memory order？](https://www.zhihu.com/question/24301047) # [A](https://www.zhihu.com/question/24301047/answer/1193956492)

3、`Concurrent-computing\Expert-Jeff-Preshing\Lock-Free-Programming`

他的这个系列的文章非常好，可以作为guideline。

4、工程hardware的如下章节:

a、`CPU\Execution-of-instruction`

b、`CPU-memory-access\Memory-ordering`

5、`acmqueue-Shared-Variables-or-Memory-Models`

6、stackoverflow [How to understand acquire and release semantics?](https://stackoverflow.com/questions/24565540/how-to-understand-acquire-and-release-semantics) # [A](https://stackoverflow.com/a/9764313)

收录在工程programming language的`Release-acquire` 章节中，这篇文章非常好。

7、cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order) # Explanation # Relaxed ordering

其中给出了example，非常好。

## Order of write to shared data may be different among different threads

这是在阅读一篇文章时，其中提出的一个观点，我觉得这个观点比较好，在下面文章中，描述了相关的例子:

1、参见 "aristeia-C++and-the-Perils-of-Double-Checked-Locking" 章节

2、queue.acm [You Don’t Know Jack about Shared Variables or Memory Models](https://queue.acm.org/detail.cfm?id=2088916)





