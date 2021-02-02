# [preshing](https://github.com/preshing)/**[cpp11-on-multicore](https://github.com/preshing/cpp11-on-multicore)** **inmemorylogger**

## Source code

[cpp11-on-multicore](https://github.com/preshing/cpp11-on-multicore)/[common](https://github.com/preshing/cpp11-on-multicore/tree/master/common)/[**inmemorylogger.h**](https://github.com/preshing/cpp11-on-multicore/blob/master/common/inmemorylogger.h)

[cpp11-on-multicore](https://github.com/preshing/cpp11-on-multicore)/[common](https://github.com/preshing/cpp11-on-multicore/tree/master/common)/[**inmemorylogger.cpp**](https://github.com/preshing/cpp11-on-multicore/blob/master/common/inmemorylogger.cpp)





## Shared variable/data



```C++
class InMemoryLogger
{
	struct Page
	{
		std::unique_ptr<Page> next;
		std::atomic<int> index;     // This can exceed EVENTS_PER_PAGE, but it's harmless. Just means page is full.
		Event events[EVENTS_PER_PAGE];

		Page() :
						index(0)
		{
		}
	};
	std::atomic<Page*> m_tail;
};

```

`m_tail`、`Page.index` 就是shared variable/data，因此将它们都声明为`std::atomic`类型；

## Double checked locking

在使用double checked locking来实现singleton时，只有当`if (pInstance == 0)// 1st test`满足的时候，才会进行lock，由于TOCTTOU，因此，在lock后，还需要进行第二次check: `if (pInstance == 0)// 2nd test`。

在 **inmemorylogger** 中，也使用 double checked locking，它只有当当前page满的时候(1st test)，它才需要进行allocate，才会进行lock，由于TOCTTOU，因此，在lock后，还需要进行第二次check: `if (oldTail->index.load(std::memory_order_relaxed) < EVENTS_PER_PAGE)`。

## `log`

```C++
    void log(const char* msg, size_t param = 0)
    {
        std::atomic_signal_fence(std::memory_order_seq_cst);    // Compiler barrier
        // On weak CPUs and current C++ compilers, memory_order_consume costs the same as acquire. :(
        // (If you don't like that, you can probably demote this load to relaxed and get away with it.
        // Technically, you'd be violating the spec, but in practice it will likely work. Just
        // inspect the assembly and make sure there is a data dependency between m_tail.load and
        // both subsequent uses of page, and you're golden. The only way I can imagine the dependency
        // chain being broken is if the compiler knows the addresses that will be allocated
        // in allocateEventFromNewPage at runtime, which is a huuuuuuuuuge leap of the imagination.)
        // http://preshing.com/20140709/the-purpose-of-memory_order_consume-in-cpp11
        Page* page = m_tail.load(std::memory_order_consume);
        Event* evt;
        int index = page->index.fetch_add(1, std::memory_order_relaxed);
        if (index < EVENTS_PER_PAGE)
            evt = &page->events[index];
        else
            evt = allocateEventFromNewPage();   // Double-checked locking is performed inside here.
        evt->tid = std::this_thread::get_id();
        evt->msg = msg;
        evt->param = param;
        std::atomic_signal_fence(std::memory_order_seq_cst);    // Compiler barrier
    }
```

### 如何理解`std::atomic_signal_fence(std::memory_order_seq_cst)`？





preshing [The Purpose of memory_order_consume in C++11](https://preshing.com/20140709/the-purpose-of-memory_order_consume-in-cpp11/)