# Transactional memory

在阅读 [C++ reference](https://en.cppreference.com/w/cpp)时，无意中发现了 "**[Transactional Memory](https://en.cppreference.com/w/cpp/language/transactional_memory)** (TM TS)"，看了一下，发现它非常类似于Java的写法，遂决定将transactional memory的内容进行整理。

## Transactional memory VS low-level thread synchronization

相比于low-level thread synchronization，transactional memory 的优势包括:

### High-level abstraction

相比于low-level thread synchronization，transaction是一个 high-level abstraction，显然它能够隐藏非常多的底层细节，更容易理解，让programmer能够更好地编写线程安全的程序，在 wikipedia [Software transactional memory](https://en.wikipedia.org/wiki/Software_transactional_memory) # Conceptual advantages and disadvantages章节中，也有关于此的讨论。

关于transaction是一个非常好的抽象，参见`Distributed-computing\Theory\Abstraction`章节。

### Higher concurrency

相比于low-level thread synchronization，transactional memory能够提供higher concurrency，这在: 

1) wikipedia [Transactional memory](https://en.wikipedia.org/wiki/Transactional_memory) # Motivation

2) wikipedia [Software transactional memory](https://en.wikipedia.org/wiki/Software_transactional_memory) # Performance

中都有描述。

### Pessimistic VS optimistic

Transactional memory是optimistic concurrency control，而lock是pessimistic。

## wikipedia [Transactional memory](https://en.wikipedia.org/wiki/Transactional_memory)

In [computer science](https://en.wikipedia.org/wiki/Computer_science) and [engineering](https://en.wikipedia.org/wiki/Computer_engineering), **transactional memory** attempts to simplify [concurrent programming](https://en.wikipedia.org/wiki/Concurrent_programming) by allowing a group of load and store instructions to execute in an [atomic](https://en.wikipedia.org/wiki/Linearizability) way. It is a [concurrency control](https://en.wikipedia.org/wiki/Concurrency_control) mechanism analogous to [database transactions](https://en.wikipedia.org/wiki/Database_transaction) for controlling access to [shared memory](https://en.wikipedia.org/wiki/Shared_memory_(interprocess_communication)) in [concurrent computing](https://en.wikipedia.org/wiki/Concurrent_computing). Transactional memory systems provide high-level abstraction as an alternative to low-level thread synchronization. This abstraction allows for coordination between concurrent reads and writes of shared data in parallel systems.[[1\]](https://en.wikipedia.org/wiki/Transactional_memory#cite_note-1)

> NOTE: 在语言层支持transactional memory的programming language，往往会提供语法糖来让programmer轻松地使用transactional memory；

### Motivation

Low level thread synchronization constructs such as locks are **pessimistic**. Transactional memory provides [optimistic concurrency control](https://en.wikipedia.org/wiki/Optimistic_concurrency_control) by allowing threads to run in parallel with minimal interference. The goal of transactional memory systems is to transparently support regions of code marked as transactions by enforcing [atomicity](https://en.wikipedia.org/wiki/Atomicity_(database_systems)), [consistency](https://en.wikipedia.org/wiki/Consistency_(database_systems)) and [isolation](https://en.wikipedia.org/wiki/Isolation_(database_systems)).

> NOTE: 原文第一段主要从optimistic和pessimistic的角度来进行说明。
>
> Transactional memory意味着ACI(没有D，在DBMS中D是指[durability](https://en.wikipedia.org/wiki/ACID))。



> NOTE: 原文第二段主要介绍的transaction的原理。

With these constructs in place, **transactional memory** provides a high level programming **abstraction** by allowing programmers to enclose their methods within **transactional blocks**. Correct implementations ensure that data cannot be shared between threads without going through a transaction and produce a [serializable](https://en.wikipedia.org/wiki/Serializability) outcome. For example, code can be written as:

```python
def transfer_money(from_account, to_account, amount):
    """Transfer money from one account to another."""
    with transaction():
        from_account -= amount
        to_account   += amount
```

In the code, the block defined by "transaction" is guaranteed atomicity, consistency and isolation by the underlying transactional memory implementation and is transparent to the programmer.

Although transactional memory programs cannot produce a deadlock, programs may still suffer from a livelock or resource [starvation](https://en.wikipedia.org/wiki/Starvation_(computer_science)). For example, longer transactions may repeatedly revert(重复) in response to multiple smaller transactions, wasting both time and energy.[[2\]](https://en.wikipedia.org/wiki/Transactional_memory#cite_note-:0-2)



### Hardware vs. software

The abstraction of atomicity in transactional memory requires a hardware mechanism to detect conflicts and undo any changes made to shared data.[[3\]](https://en.wikipedia.org/wiki/Transactional_memory#cite_note-:1-3)

[Software transactional memory](https://en.wikipedia.org/wiki/Software_transactional_memory) provides transactional memory semantics in a software [runtime library](https://en.wikipedia.org/wiki/Runtime_library) or the programming language,[[9\]](https://en.wikipedia.org/wiki/Transactional_memory#cite_note-9) and requires minimal hardware support (typically an atomic [compare and swap](https://en.wikipedia.org/wiki/Compare_and_swap) operation, or equivalent). As the downside, software implementations usually come with a performance penalty, when compared to hardware solutions.

### History

As of GCC 4.7, an experimental library for transactional memory is available which utilizes a hybrid implementation. The PyPy variant of Python also introduces transactional memory to the language.

> NOTE: 关于C++对transactional memory的支持，参见`C++\Language-reference\Basic-concept\Abstract-machine\Memory-model\Transactional-memory`章节。

## wikipedia [Software transactional memory](https://en.wikipedia.org/wiki/Software_transactional_memory)

In [computer science](https://en.wikipedia.org/wiki/Computer_science), **software [transactional memory](https://en.wikipedia.org/wiki/Transactional_memory)** (**STM**) is a [concurrency control](https://en.wikipedia.org/wiki/Concurrency_control) mechanism analogous to [database transactions](https://en.wikipedia.org/wiki/Database_transaction) for controlling access to [shared memory](https://en.wikipedia.org/wiki/Shared_memory_(interprocess_communication)) in [concurrent computing](https://en.wikipedia.org/wiki/Concurrent_computing). It is an alternative to [lock-based synchronization](https://en.wikipedia.org/wiki/Lock_(computer_science)). STM is a strategy implemented in software, rather than as a hardware component. 

### Performance

Unlike the [locking](https://en.wikipedia.org/wiki/Lock_(computer_science)) techniques used in most modern multithreaded applications, STM is often very [optimistic](https://en.wikipedia.org/wiki/Optimistic_concurrency_control) :



### Conceptual advantages and disadvantages

In addition to their performance benefits[*[citation needed](https://en.wikipedia.org/wiki/Wikipedia:Citation_needed)*], STM greatly simplifies conceptual understanding of multithreaded programs and helps make programs more maintainable by working in harmony with existing high-level abstractions such as objects and modules. Lock-based programming has a number of well-known problems that frequently arise in practice

### Implementations

#### C/C++

- [G++ 4.7](https://gcc.gnu.org/wiki/TransactionalMemory) now supports STM for C/C++ directly in the compiler. The feature is still listed as "experimental", but may still provide the necessary functionality for testing.