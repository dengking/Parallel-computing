# preshing [Lightweight In-Memory Logging](https://preshing.com/20120522/lightweight-in-memory-logging/)

> NOTE: 这篇文章描述的debug multithread的方式是非常值得借鉴的

When debugging multithreaded code, it’s not always easy to determine which codepath was taken. You can’t always reproduce the bug while stepping through the debugger, nor can you always sprinkle(撒、散播) `printf`s throughout the code, as you might in a single-threaded program. There might be millions of events before the bug occurs, and `printf` can easily slow the application to a crawl, mask the bug, or create a spam fest in the output log.

One way of attacking such problems is to instrument(装配) the code so that events are logged to a circular buffer in memory. This is similar to adding `printf`s, except that only the most recent events are kept in the log, and the performance overhead can be made very low using **lock-free** techniques.

> NOTE: lockless circular buffer

```C++
#include <windows.h>
#include <intrin.h>

namespace Logger
{
    struct Event
    {
        DWORD tid;        // Thread ID
        const char* msg;  // Message string
        DWORD param;      // A parameter which can mean anything you want
    };

    static const int BUFFER_SIZE = 65536;   // Must be a power of 2
    extern Event g_events[BUFFER_SIZE];
    extern LONG g_pos;

    inline void Log(const char* msg, DWORD param)
    {
        // Get next event index
        LONG index = _InterlockedIncrement(&g_pos);
        // Write an event at this index
        Event* e = g_events + (index & (BUFFER_SIZE - 1));  // Wrap to buffer size
        e->tid = ((DWORD*) __readfsdword(24))[9];           // Get thread ID
        e->msg = msg;
        e->param = param;
    }
}

#define LOG(m, p) Logger::Log(m, p)
```

> NOTE: 
>
> 一个问题: why `static const int BUFFER_SIZE = 65536;   // Must be a power of 2`？
>
> 解答如下:
>
> 1、65536是$2^{16}$，正好是"a power of 2"
>
> 2、它其实执行的是Modulo operation: $index \% BUFFER\_SIZE$，这是使用的一个optimization technique，在 wikipedia [Modulo operation#Performance issues](https://en.wikipedia.org/wiki/Modulo_operation#Performance_issues) 中给出了解答
>
> 

And you must place the following in a `.cpp` file.

```C++
namespace Logger
{
    Event g_events[BUFFER_SIZE];
    LONG g_pos = -1;
}
```

This is perhaps one of the simplest examples of lock-free programming which actually does something useful. There’s a single macro `LOG`, which writes to the log. It uses `_InterlockedIncrement`, an atomic operation which I’ve talked about in [previous posts](http://preshing.com/20120226/roll-your-own-lightweight-mutex), for thread safety. There are no readers. *You* are meant to be the reader when you inspect the process in the debugger, such as when the program crashes, or when the bug is otherwise caught.

> NOTE: 
>
> 上述代码的lock-free programming的实现是非常值得借鉴的，它有如下特性:
>
> 1、memory pool/buffer: `g_events`，显然它是proallocation的
>
> 2、多个thread同时使用shared data `g_events`，由于每个thread都会thread-safe地获得一个独立的`index`，因此在`e`上不存在race，因此它是允许多个thread同时使用`g_events`
>
> 3、多个thread同时使用`g_pos`是存在race condition的，因此它使用 `_InterlockedIncrement`
>
> 4、使用multiple model的read and write来进行分析: multiple writer，没有reader，因此就无需进行lock

## Using It to Debug My Previous Post

> NOTE: 略

## What Makes This Lightweight?

> NOTE: 略

## Implementation

[preshing](https://github.com/preshing)/[cpp11-on-multicore](https://github.com/preshing/cpp11-on-multicore)/[common](https://github.com/preshing/cpp11-on-multicore/tree/master/common)/[inmemorylogger.h](https://github.com/preshing/cpp11-on-multicore/blob/master/common/inmemorylogger.h)