# Hazard-pointer-based reclamation (HPBR)

一、是在阅读 "cs.toronto-Performance-of-memory-reclamation-for-lockless-synchronization" 时，其中提及了 "hazard-pointer-based reclamation"

二、下面是一些比较好的文章:

1、infoq [Lock-free Programming in C++ with Herb Sutter](https://www.infoq.com/news/2014/10/cpp-lock-free-programming/)

2、Herb Sutter [Lock-Free Programming or, How to Juggle Razor Blades](http://www.alfasoft.com/files/herb/40-LockFree.pdf) 

3、ticki.github [Fearless concurrency with hazard pointers](http://ticki.github.io/blog/fearless-concurrency-with-hazard-pointers/)

4、simongui.github [Improving performance of lockless data structures](http://simongui.github.io/2017/01/23/improving-lockless-synchronization-performance.html)



三、https://github.com/topics/hazard-pointer





## wikipedia [Hazard pointer](https://en.wikipedia.org/wiki/Hazard_pointer)

In a [multithreaded](https://en.wikipedia.org/wiki/Thread_(computer_science)) [computing](https://en.wikipedia.org/wiki/Computer_science) environment, **hazard pointers** are one approach to solving the problems posed by [dynamic memory management](https://en.wikipedia.org/wiki/Dynamic_memory_management) of the nodes in a [lock-free](https://en.wikipedia.org/wiki/Non-blocking_algorithm) [data structure](https://en.wikipedia.org/wiki/Data_structure). These problems generally arise only in environments that don't have [automatic garbage collection](https://en.wikipedia.org/wiki/Automatic_garbage_collection).[[1\]](https://en.wikipedia.org/wiki/Hazard_pointer#cite_note-Williams12-1)

Any lock-free data structure that uses the [compare-and-swap](https://en.wikipedia.org/wiki/Compare-and-swap) primitive must deal with the [ABA problem](https://en.wikipedia.org/wiki/ABA_problem). For example, in a lock-free stack represented as an intrusively linked list, one thread may be attempting to pop an item from the front of the stack (A → B → C). It remembers the second-from-top value "B", and then performs `compare_and_swap(target=&head, newvalue=B, expected=A)`. Unfortunately, in the middle of this operation, another thread may have done two pops and then pushed A back on top, resulting in the stack (A → C). The compare-and-swap succeeds in swapping `head` with `B`, and the result is that the stack now contains garbage (a pointer to the freed element "B").



Hazard pointers can be used to address both of these problems. In a hazard-pointer system, each [thread](https://en.wikipedia.org/wiki/Thread_(computer_science)) keeps a [list](https://en.wikipedia.org/wiki/List_(computing)) of hazard pointers indicating which nodes the thread is currently accessing. (In many systems this "list" may be probably limited to only one[[1\]](https://en.wikipedia.org/wiki/Hazard_pointer#cite_note-Williams12-1)[[2\]](https://en.wikipedia.org/wiki/Hazard_pointer#cite_note-Alexandrescu04-2) or two elements.) Nodes on the hazard pointer list must not be modified or deallocated by any other thread.



## TODO

ticki.github [Fearless concurrency with hazard pointers](http://ticki.github.io/blog/fearless-concurrency-with-hazard-pointers/) 

open-std [Why Hazard Pointers Should Be in C++26](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p2530r0.pdf) 

## Implementation

https://github.com/oliver-giersch/hazptr