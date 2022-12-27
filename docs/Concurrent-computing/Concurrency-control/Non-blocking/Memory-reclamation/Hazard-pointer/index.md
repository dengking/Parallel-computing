# Hazard-pointer-based reclamation (HPBR)

一、是在阅读 "cs.toronto-Performance-of-memory-reclamation-for-lockless-synchronization" 时，其中提及了 "hazard-pointer-based reclamation"



## Q&A

1、reader、writer

reader 通过 pointer来read shared data，write可能将这个pointer给释放掉

2、hazard pointer VS shared pointer、weak pointer

3、为什么hazard pointer能够避免ABA？

ABA一般出现在由CAS实现的算法中。

4、hazard pointer vs reference counting

参考素材:

zditect [Hazard pointer](https://zditect.com/blog/2721263.html)



## [Hazard Pointer Synchronous Reclamation Beyond Concurrency TS2 - Maged Michael - CppCon 2021](https://www.youtube.com/watch?v=lsy8RRq2hHM)

> NOTE:
>
> 一、[PPT](https://github.com/CppCon/CppCon2021/blob/main/Presentations/hazard_pointer_synchronous_reclamation.pptx) 

### In a nutshell

It's a mechanism for safe reclamation,  basically one thread owns a hazard pointer and it communicates throught it to other threads that may remove the object that they want to protect concurrently while the threads wants to access it, so the protocol between theads is quite simple, you can see here like there's a thread who wants to protect an object and another thread that one wants to remove it and reclaim it. 

(1)so the thread will read a pointer from an atomic source(`SRC`) and that is the one say okay. (2)let me try to protect it: it gonna set a hazard pointer to it, so now it's saying okay maybe it's protected but only if it hasn't been removed yet. (3)It checkes that it hasn't been removed, so now we know that the thread knows that it's protected and it can access it safely.

> NOTE:
>
> 一、作者想表达的意思是: "safe to use pointer A"的两个前提条件:
>
> 1、it's protected by a hazard pointer
>
> ```pseudocode
> set HP to A
> ```
>
> 
>
> 2、it hasn't been removed yet
>
> ```pseudocode
> if SRC == A // it hasn't been removed yet
> ```
>
> 

It goes ahead. (4)another thread removes. It doesn't matter because it removed it but it hasn't reclaimed it. (5)Now the user or protector thread accesses the object safely. (6)When it is done with it, it clears the hazard pointer and it is like it's okay for other thread to reclaim that object. So now that object checks the headers pointers, it's not protecting the object and it goes to safely delete the object so that's kind of like in a nutshell how it works.

 

> NOTE:
>
> 一、从这一段作者的描述来看，它其实是和reference counting非常类似的
>
> 二、protect shared data



### Concurrency TS2 Essential Hazard Pointer Interface



```c++
class hazard_pointer {
  template <typename T> T* protect(const std::atomic<T*>& src) noexcept;
};

hazard_pointer make_hazard_pointer();

template <typename T> class hazard_pointer_obj_base<T> {
  void retire() noexcept;
};

```



### Example Using Hazard Pointers

```c++
class Foo : public hazard_pointer_obj_base<Foo> { /* Foo members */ };

result read_and_use(std::atomic<Foo*>& src, Func fn) { // Called frequently
 folly::hazard_pointer h = folly::make_hazard_pointer();
 Foo* ptr = h.protect(src);
 return fn(ptr); // ptr is protected
}

void update(std::atomic<Foo*>& src, Foo* newptr) { // Called infrequently
  Foo* oldptr = src.exchange(newptr);
  oldptr->retire();
} 

```



## wikipedia [Hazard pointer](https://en.wikipedia.org/wiki/Hazard_pointer)

In a [multithreaded](https://en.wikipedia.org/wiki/Thread_(computer_science)) [computing](https://en.wikipedia.org/wiki/Computer_science) environment, **hazard pointers** are one approach to solving the problems posed by [dynamic memory management](https://en.wikipedia.org/wiki/Dynamic_memory_management) of the nodes in a [lock-free](https://en.wikipedia.org/wiki/Non-blocking_algorithm) [data structure](https://en.wikipedia.org/wiki/Data_structure). These problems generally arise only in environments that don't have [automatic garbage collection](https://en.wikipedia.org/wiki/Automatic_garbage_collection).[[1\]](https://en.wikipedia.org/wiki/Hazard_pointer#cite_note-Williams12-1)

Any lock-free data structure that uses the [compare-and-swap](https://en.wikipedia.org/wiki/Compare-and-swap) primitive must deal with the [ABA problem](https://en.wikipedia.org/wiki/ABA_problem). For example, in a lock-free stack represented as an intrusively linked list, one thread may be attempting to pop an item from the front of the stack (A → B → C). It remembers the second-from-top value "B", and then performs `compare_and_swap(target=&head, newvalue=B, expected=A)`. Unfortunately, in the middle of this operation, another thread may have done two pops and then pushed A back on top, resulting in the stack (A → C). The compare-and-swap succeeds in swapping `head` with `B`, and the result is that the stack now contains garbage (a pointer to the freed element "B").

> NOTE:
>
> 一、`compare_and_swap(target=&head, newvalue=B, expected=A)` 是什么含义？

Hazard pointers can be used to address both of these problems. In a hazard-pointer system, each [thread](https://en.wikipedia.org/wiki/Thread_(computer_science)) keeps a [list](https://en.wikipedia.org/wiki/List_(computing)) of hazard pointers indicating which nodes the thread is currently accessing. (In many systems this "list" may be probably limited to only one[[1\]](https://en.wikipedia.org/wiki/Hazard_pointer#cite_note-Williams12-1)[[2\]](https://en.wikipedia.org/wiki/Hazard_pointer#cite_note-Alexandrescu04-2) or two elements.) Nodes on the **hazard pointer list** must not be modified or deallocated by any other thread.

> NOTE:
>
> 一、**hazard pointer list** 



## drdobbs [Lock-Free Data Structures with Hazard Pointers](https://www.drdobbs.com/lock-free-data-structures-with-hazard-po/184401890)

So, the past article, after introducing the concept of lock-free programming, set out to(开始) implement lock-free "Write-Rarely-Read-Many" maps. These structures are often encountered in the form of global tables, factory objects, caches, and the like. The problem was memory reclamation: Given that things are lock-free, how can memory be freed? As discussed, this is a tough problem because lock-free implies an unrestricted opportunity for a thread to operate on any object, at any time. To address that problem, the past article tried to:

......

What's really needed is some mechanism for readers to tell writers to not reclaim replaced maps from under them, but without allowing the readers to force the writers to hang on to(保留) an unbounded number of replaced maps. 

> NOTE:
>
> 一、上面这段话的后半句如何理解？

There is a solution [3] that is not only lock-free, but is actually wait-free. (To recap(简明扼要地陈述) the definitions in our previous installment: 

1、Lock-free means that progress is guaranteed for some thread in the system; 

2、the stronger wait-free means that progress is guaranteed for all threads.) 

Moreover, this method asks for no special operations—no DCAS, no CAS2, only the trusty CAS. Interested? Read on.



### Hazard Pointers

To bring up the code again, we have a template **`WRRMMap`** that holds a pointer to some classic single-threaded **`Map`** object (think **`std::map`**) and provides a multithreaded lock-free access interface to it:

```c++
template <class K, class V>
class WRRMMap {
   Map<K, V> * pMap_;
   ...
};
```

Whenever the **`WRRMMap`** needs to be updated, the thread wanting to do so creates an entirely new copy of the map pointed to by **`pMap_`**, replaces **`pMap_`** with that new copy, and then disposes of the old **`pMap_`**. We agreed that that's not an inefficient thing to do because **`WRRMMap`** is read often and updated only rarely. The nagging(使人不得安宁的) problem was, "How could we dispose of **`pMap_`** properly, given that there could be other threads reading through it at any time?"

> NOTE:
>
> 一、上面描述的思路和RCU非常类似，在zhihu [Linux中的RCU机制[一] - 原理与使用方法](https://zhuanlan.zhihu.com/p/89439043) 中有如下描述:
>
> > RCU是有唯一意思的，尤其是Linux内核的角度下；而Fedor Pikus说的是，理解“RCU”不能简单地从名字出发。确实，版本号也可以达到类似的“读写并行进行”的效果；但RCU和版本号的区别在于，RCU有特别的内存回收方式，也就是文中描述的——**写者线程**等待**读者线程**退出后者的critical section，再进行**内存回收**，这是RCU最关键的地方。因为它最早是用在Linux内核，而内核知道读者线程的进度而几乎不需要额外的簿记信息，因此能够获得很好的性能。

#### Reader thread

**Hazard pointers** are a safe, efficient mechanism for threads to advertise(通告) to all other threads about their memory usage. Each **reader thread** owns a single-writer/multi-reader shared pointer called "**hazard pointer**". When a **reader thread** assigns the address of a map to its **hazard pointer**, it is basically announcing to other threads (writers), "I am reading this map. You can replace it if you want, but don't change its contents and certainly keep your **delete**ing hands off it."

> NOTE:
>
> 一、hazard pointer是 "single-writer/multi-reader shared pointer"，它的writer是谁？它的reader是谁？
>

#### Writer thread

On their part, **writer threads** have to check the **hazard pointers** of the readers before they can **delete** any replaced maps. So, if a writer removes a map after a reader (or more) has already set its **hazard pointer** to the address of that map, then the writer will not **delete** that map as long as the **hazard pointer** remains unchanged.



Whenever a **writer thread** replaces a map, it keeps the old pointer in a **private list**. After accumulating some number of removed maps (we'll discuss later how to choose that number), the thread scans the hazard pointers of the readers for matches to the addresses of the accumulated maps. If a removed map is not matched by any of the hazard pointers, then it is safe for this map to be deallocated. Otherwise, the **writer thread** keeps the node until its next scan of the **hazard pointers**.

To follow are the essential data structures used. The main shared structure is a singly linked list of hazard pointers (**`HPRecType`**), pointed to by **`pHead_`**. Each entry in the list contains the **hazard pointer** (**`pHazard_`**), a flag that tells whether the hazard pointer is in use or not (**`active_`**), and the obligatory(必须的) pointer to the next node (**`pNext_`**).

**`HPRecType`** offers two primitives: **`Acquire`** and **`Release`**. **`HPRecType::Acquire`** gives a thread a pointer to an **`HPRecType`**, call it **`p`**. From then on, that thread can set **`p->pHazard_`** and rest assured that all other threads will tread(小心处理) carefully around that pointer. When the thread does not use the **hazard pointer** anymore, it calls **`HPRecType::Release(p)`**.



```c++
#include <vector>
using namespace std;

// Hazard pointer record
class HPRecType
{
    HPRecType *pNext_;
    int active_;
    // Global header of the HP list
    static HPRecType *pHead_;
    // The length of the list
    static int listLen_;

public:
    // Can be used by the thread
    // that acquired it
    void *pHazard_;
    static HPRecType *Head()
    {
        return pHead_;
    }
    // Acquires one hazard pointer
    static HPRecType *Acquire()
    {
        // Try to reuse a retired HP record
        HPRecType *p = pHead_;
        for (; p; p = p->pNext_)
        {
            if (p->active_ || !CAS(&p->active_, 0, 1))
                continue;
            // Got one!
            return p;
        }
        // Increment the list length
        int oldLen;
        do
        {
            oldLen = listLen_;
        } while (!CAS(&listLen_, oldLen, oldLen + 1));
        // Allocate a new one
        HPRecType *p = new HPRecType;
        p->active_ = 1;
        p->pHazard_ = 0;
        // Push it to the front
        do
        {
            old = pHead_;
            p->pNext_ = old;
        } while (!CAS(&pHead_, old, p));
        return p;
    }
    // Releases a hazard pointer
    static void Release(HPRecType *p)
    {
        p->pHazard_ = 0;
        p->active_ = 0;
    }
};
// Per-thread private variable
__per_thread__ vector<Map<K, V> *> rlist; // retired-list
```



Each thread holds a "**retired list**" (actually a **`vector<Map<K,V>*>`** in our implementation)—a container keeping track of the pointers that this thread finds are not needed anymore and could be **delete**d as soon as no other threads use them. This vector need not be synchronized because it's in per-thread storage; only one thread ever accesses it. We gloss over(掩盖、隐藏) the tedium of allocating thread-local storage by using the magic qualifier **`__per_thread__`**. Given this setup, all a thread needs to do whenever it wants to dispose of **`pMap_`** is call the **Retire** function. (Note that, as in the previous "`Generic<Programming>`" instance, we do not insert memory barriers for the sake of clarity.)

> NOTE:
>
> 一、"for the sake of clarity"的含义是"为了清晰起见"



```c++
template <class K, class V>
class WRRMMap
{
    Map<K, V> *pMap_;
    // ...
private:
    static void Retire(Map<K, V> *pOld)
    {
        // put it in the retired list
        rlist.push_back(pOld);
        if (rlist.size() >= R)
        {
            Scan(HPRecType::Head());
        }
    }
};
```



Nothing up our sleeves! Now, the **Scan** function performs a "set difference" between the pointers in the current thread's **retired list**, and all hazard pointers for all threads. What does that **set difference** mean? Let's think of it for a second: It's the set of all old **`pMap_`** pointers that this thread considers useless, except those that are found among the hazard pointers of all threads. But, hey, these are exactly the goners(已死者)! By definition of the retired list and that of the hazard pointers, if a pointer is retired and not marked as "hazardous" (for example, "in use") by any thread, the set intersection of the two sets yields precisely the pointers that can be **delete**d.

### The Main Algorithm

Okay, now let's see how to implement the **Scan** algorithm, and what guarantees it can provide. We need to perform a set difference between **`rlist`** and **`pHead_`** whenever performing a scan. That operation is tantamount to: "For each pointer in the retired list, find it in the hazard set. If it's not, it belongs to the difference, so it can be **delete**d." To optimize that, we can sort the hazard pointers before lookup, and then perform one binary search in it for each retired pointer. Let's take a look at such an implementation of **Scan**:

```c++
#include <vector>
#include <algorithm>

using namespace std;

void Scan(HPRecType *head)
{
    // Stage 1: Scan hazard pointers list
    // collecting all non-null ptrs
    vector<void *> hp;
    while (head)
    {
        void *p = head->pHazard_;
        if (p)
            hp.push_back(p);
        head = head->pNext_;
    }
    // Stage 2: sort the hazard pointers
    sort(hp.begin(), hp.end(), less<void *>());
    // Stage 3: Search for'em!
    vector<Map<K, V> *>::iterator i = rlist.begin();
    while (i != rlist.end())
    {
        if (!binary_search(hp.begin(), hp.end(), *i))
        {
            // Aha!
            delete *i;
            if (&*i != &rlist.back())
            {
                *i = rlist.back();
            }
            rlist.pop_back();
        }
        else
        {
            ++i;
        }
    }
}

```



## paprer [Hazard Pointers: Safe Memory Reclamation for Lock-Free Objects](https://web.archive.org/web/20171104135736/http://www.research.ibm.com/people/m/michael/ieeetpds-2004.pdf)

### Abstract

Lock-free objects offer significant performance and reliability advantages over conventional lock-based objects. However, the lack of an efficient portable lock-free method for the reclamation of the memory occupied by dynamic nodes removed from such objects is a major obstacle to their wide use in practice. This paper presents hazard pointers, a memory management methodology that allows memory reclamation for arbitrary reuse. It is very efficient, as demonstrated by our experimental results. It is suitable for user-level applications—as well as system programs—without dependence on special kernel or scheduler support. It is **wait-free**. It requires only single-word reads and writes for memory access in its core operations. It allows reclaimed memory to be returned to the operating system. In addition, it offers a lock-free solution for the **ABA** problem using only practical **single-word instructions**. Our experimental results on a multiprocessor system show that the new methodology offers equal and, more often, significantly better performance than other memory management methods, in addition to its qualitative advantages regarding memory reclamation and independence of special hardware support. We also show that lock-free implementations of important object types, using hazard pointers, offer comparable performance to that of efficient lock-based implementations under no contention and no multiprogramming, and outperform them by significant margins under moderate multiprogramming and/or contention, in addition to guaranteeing continuous progress and availability, even in the presence of thread failures and arbitrary delays.

### INTRODUCTION



However, a major concern regarding these objects is the reclamation of the memory occupied by removed nodes.

In the case of a lock-based object, when a thread removes a node from the object, it is easy to guarantee that no other thread will subsequently access the memory of that node, before it is reused or reallocated. Consequently, it is usually safe for the removing thread to reclaim the memory occupied by the removed node (e.g., using `free`) for arbitrary future reuse by the same or other threads (e.g., using `malloc`).

This is not the case for a typical lock-free dynamic object, when running in programming environments without support for automatic garbage collection.



## TODO



kongfy [Hazard Pointer](http://blog.kongfy.com/2017/02/hazard-pointer/)

cnblogs [实现无锁的栈与队列(5)：Hazard Pointer](https://www.cnblogs.com/catch/p/5129586.html)

stackoverflow [Hazard Pointers with well defined allocators in C++](https://stackoverflow.com/questions/61674826/hazard-pointers-with-well-defined-allocators-in-c)



ticki.github [Fearless concurrency with hazard pointers](http://ticki.github.io/blog/fearless-concurrency-with-hazard-pointers/) 

open-std [Why Hazard Pointers Should Be in C++26](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p2530r0.pdf) 

Herb Sutter [Lock-Free Programming or, How to Juggle Razor Blades](http://www.alfasoft.com/files/herb/40-LockFree.pdf) 



simongui.github [Improving performance of lockless data structures](http://simongui.github.io/2017/01/23/improving-lockless-synchronization-performance.html)



## Implementation

https://github.com/oliver-giersch/hazptr