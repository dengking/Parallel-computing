# Transactional memory

在阅读 [C++ reference](https://en.cppreference.com/w/cpp)时，无意中发现了 "**[Transactional Memory](https://en.cppreference.com/w/cpp/language/transactional_memory)** (TM TS)"，看了一下，发现它非常类似于Java的写法，遂决定将transactional memory的内容进行整理。

## wikipedia [Transactional memory](https://en.wikipedia.org/wiki/Transactional_memory)

In [computer science](https://en.wikipedia.org/wiki/Computer_science) and [engineering](https://en.wikipedia.org/wiki/Computer_engineering), **transactional memory** attempts to simplify [concurrent programming](https://en.wikipedia.org/wiki/Concurrent_programming) by allowing a group of load and store instructions to execute in an [atomic](https://en.wikipedia.org/wiki/Linearizability) way. It is a [concurrency control](https://en.wikipedia.org/wiki/Concurrency_control) mechanism analogous to [database transactions](https://en.wikipedia.org/wiki/Database_transaction) for controlling access to [shared memory](https://en.wikipedia.org/wiki/Shared_memory_(interprocess_communication)) in [concurrent computing](https://en.wikipedia.org/wiki/Concurrent_computing). Transactional memory systems provide high-level abstraction as an alternative to low-level thread synchronization. This abstraction allows for coordination between concurrent reads and writes of shared data in parallel systems.[[1\]](https://en.wikipedia.org/wiki/Transactional_memory#cite_note-1)

> NOTE: 在语言层支持transactional memory的programming language，往往会提供语法糖来让programmer轻松地使用transactional memory；
>
> 相比于low-level thread synchronization，显然transactional memory是一个非常好的abstraction的，显然它能够隐藏非常多的底层细节，让programmer能够更好地编写线程安全的程序，在 wikipedia [Software transactional memory](https://en.wikipedia.org/wiki/Software_transactional_memory) 中，也有关于此的讨论:，参见后文。
>
> 关于transaction是一个非常好的抽象，参见`Distributed-computing\Theory\Abstraction`章节。

### Motivation



### Hardware vs. software

### History

As of GCC 4.7, an experimental library for transactional memory is available which utilizes a hybrid implementation. The PyPy variant of Python also introduces transactional memory to the language.



## wikipedia [Software transactional memory](https://en.wikipedia.org/wiki/Software_transactional_memory)

In [computer science](https://en.wikipedia.org/wiki/Computer_science), **software [transactional memory](https://en.wikipedia.org/wiki/Transactional_memory)** (**STM**) is a [concurrency control](https://en.wikipedia.org/wiki/Concurrency_control) mechanism analogous to [database transactions](https://en.wikipedia.org/wiki/Database_transaction) for controlling access to [shared memory](https://en.wikipedia.org/wiki/Shared_memory_(interprocess_communication)) in [concurrent computing](https://en.wikipedia.org/wiki/Concurrent_computing). 

### Conceptual advantages and disadvantages



### Implementations

