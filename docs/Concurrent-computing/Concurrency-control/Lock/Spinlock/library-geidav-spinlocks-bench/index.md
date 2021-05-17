# github [geidav](https://github.com/geidav)/**[spinlocks-bench](https://github.com/geidav/spinlocks-bench)**

1、提供了多种spin lock的implementation。

## Read the code

1、source code: [spinlocks-bench](https://github.com/geidav/spinlocks-bench)/[excllocks.hpp](https://github.com/geidav/spinlocks-bench/blob/master/excllocks.hpp)

2、典型的 write-release-flag-notify-read-acquire-model

### pthread spinlock

```C++
#include <pthread.h>

class SpinLockPThread
{
public:
    ALWAYS_INLINE SpinLockPThread()
    {
        pthread_spin_init(&Lock, 0);
    }

    ALWAYS_INLINE void Enter()
    {
        pthread_spin_lock(&Lock);
    }

    ALWAYS_INLINE void Leave()
    {
        pthread_spin_unlock(&Lock);
    }

private:
    pthread_spinlock_t Lock;
};

```



### Sequential consistent test-and-set spin lock

```C++
#include <atomic>

class ScTasSpinLock
{
public:
    ALWAYS_INLINE void Enter()
    {
        while (Locked.exchange(true));
    }

    ALWAYS_INLINE void Leave()
    {
        Locked.store(false);
    }

private:
    std::atomic_bool Locked = {false};
};

```



### Acquire-release test-and-set spin lock

```C++
#include <atomic>

class TasSpinLock
{
public:
    ALWAYS_INLINE void Enter()
    {
        while (Locked.exchange(true, std::memory_order_acquire));
    }

    ALWAYS_INLINE void Leave()
    {
        Locked.store(false, std::memory_order_release);
    }

private:
    std::atomic_bool Locked = {false};
};

```



### Acquire-release test-test-and-set spin lock

```C++
#include <atomic>

class TTasSpinLock
{
public:
    ALWAYS_INLINE void Enter()
    {
        do
        {
            while (Locked.load(std::memory_order_relaxed));
        }
        while (Locked.exchange(true, std::memory_order_acquire));
    }

    ALWAYS_INLINE void Leave()
    {
        Locked.store(false, std::memory_order_release);
    }

private:
    std::atomic_bool Locked = {false};
};

```



### Acquire-release test-test-and-set spin lock with `CpuRelax`



```C++
#include <atomic>

class RelaxTTasSpinLock
{
public:
    ALWAYS_INLINE void Enter()
    {
        do
        {
            while (Locked.load(std::memory_order_relaxed))
                CpuRelax();
        }
        while (Locked.exchange(true, std::memory_order_acquire));
    }

    ALWAYS_INLINE void Leave()
    {
        Locked.store(false, std::memory_order_release);
    }

private:
    std::atomic_bool Locked = {false};
};


```





### Acquire-release test-test-and-set spin lock with Exponential backoff `CpuRelax`



```C++
#include <atomic>

class ExpBoRelaxTTasSpinLock
{
public:
    ALWAYS_INLINE void Enter()
    {
        size_t curMaxDelay = MIN_BACKOFF_ITERS;

        while (true)
        {
            WaitUntilLockIsFree();

            if (Locked.exchange(true, std::memory_order_acquire))
                BackoffExp(curMaxDelay);
            else
                break;
        }
    }

    ALWAYS_INLINE void Leave()
    {
        Locked.store(false, std::memory_order_release);
    }

private:
    ALWAYS_INLINE void WaitUntilLockIsFree() const
    {
        size_t numIters = 0;

        while (Locked.load(std::memory_order_relaxed))
        {
            if (numIters < MAX_WAIT_ITERS)
            {
                numIters++;
                CpuRelax();
            }
            else
                YieldSleep();
        }
    }

public:
    std::atomic_bool Locked = {false};

private:
    static const size_t MAX_WAIT_ITERS = 0x10000;
    static const size_t MIN_BACKOFF_ITERS = 32;
};
```

### Ticket Spin Lock

它会存在和 `TTasSpinLock` 相同的问题。

```C++
class TicketSpinLock
{
public:
    ALWAYS_INLINE void Enter()
    {
        const auto myTicketNo = NextTicketNo.fetch_add(1, std::memory_order_relaxed);

        while (ServingTicketNo.load(std::memory_order_acquire) != myTicketNo)
            CpuRelax();
    }

    ALWAYS_INLINE void Leave()
    {
        // We can get around a more expensive read-modify-write operation
        // (std::atomic_size_t::fetch_add()), because noone can modify
        // ServingTicketNo while we're in the critical section.
        const auto newNo = ServingTicketNo.load(std::memory_order_relaxed)+1;
        ServingTicketNo.store(newNo, std::memory_order_release);
    }

private:
    alignas(CACHELINE_SIZE) std::atomic_size_t ServingTicketNo = {0};
    alignas(CACHELINE_SIZE) std::atomic_size_t NextTicketNo = {0};
};
```

### Anderson Spin Lock、A Lock



```C++
#include <atomic>
#include <vector>
#include <thread>
#include <iostream>
#include <stddef.h>

/**
 * @brief
 * 实现原理有点类似于 ticket spin lock，NextServingIdx 表示 下一个得到lock、能够进入critical region的，它就相当于ticket
 * 下面的代码，有些地方没有读懂:
 * 1、`Enter()`
 * ```
 * 		// Ensure overflow never happens
		if (index == 0)
		{
			NextFreeIdx -= LockedFlags.size();
			std::cout<<NextFreeIdx<<std::endl;
		}
 * ```
 *上述是什么原理，显然，第一次的时候，它会发生underflow
 *2、为什么用 NextFreeIdx、NextServingIdx
 *3、它只能够用于固定数量的thread
 */
class AndersonSpinLock
{
public:
	AndersonSpinLock(size_t maxThreads = std::thread::hardware_concurrency()) :
					LockedFlags(maxThreads)
	{
		for (auto &flag : LockedFlags)
			flag.first = true;

		LockedFlags[0].first = false;
	}

	ALWAYS_INLINE void Enter()
	{
		/**
		 * `fetch_add` 返回的是之前的值
		 */
		const size_t index = NextFreeIdx.fetch_add(1) % LockedFlags.size();
		std::cout<<index<<std::endl;
		auto &flag = LockedFlags[index].first;

		// Ensure overflow never happens
		if (index == 0)
		{
			NextFreeIdx -= LockedFlags.size();
			std::cout<<NextFreeIdx<<std::endl;
		}
		while (flag)
			CpuRelax();

		flag = true;
	}

	ALWAYS_INLINE void Leave()
	{
		const size_t idx = NextServingIdx.fetch_add(1);
		LockedFlags[idx % LockedFlags.size()].first = false;
	}

private:
	using PaddedFlag = std::pair<std::atomic_bool, uint8_t[CACHELINE_SIZE-sizeof(std::atomic_bool)]>;
	static_assert(sizeof(PaddedFlag) == CACHELINE_SIZE, "");

	alignas(CACHELINE_SIZE) std::vector<PaddedFlag> LockedFlags;
	alignas(CACHELINE_SIZE) std::atomic_size_t NextFreeIdx = { 0 };
	alignas(CACHELINE_SIZE) std::atomic_size_t NextServingIdx = { 1 };
};

```



### McsLock

```C++
#include <atomic>
#include <vector>
#include <thread>
#include <iostream>
#include <stddef.h>

class McsLock
{
public:
    struct QNode
    {
        std::atomic<QNode *> Next = {nullptr};
        std::atomic_bool     Locked = {false};
    };

public:
    ALWAYS_INLINE void Enter(QNode &node)
    {
        node.Next = nullptr;
        node.Locked = true;

        QNode *oldTail = Tail.exchange(&node); // 将node作为tail

        if (oldTail != nullptr)
        {
            oldTail->Next = &node;

            while (node.Locked == true)
                CpuRelax();
        }
    }

    ALWAYS_INLINE void Leave(QNode &node)
    {
        if (node.Next.load() == nullptr)
        {
            QNode *tailWasMe = &node;
            if (Tail.compare_exchange_strong(tailWasMe, nullptr))
                return;

            while (node.Next.load() == nullptr)
                CpuRelax();
        }

        node.Next.load()->Locked = false;
    }

private:
    std::atomic<QNode *> Tail = {nullptr};
};

```



## 以RAII的方式来进行封装

1、上面的这些相当于是`mutex`，然后使用类似于STL的`std::unique_lock`来进行封装。

## 测试程序

```C++
#include <atomic>
#include <vector>
#include <thread>
#include <iostream>
#include <stddef.h>

#if defined(__SSE2__)
#include <xmmintrin.h>  // _mm_pause
#endif


constexpr size_t CACHELINE_SIZE = 64;

#define WIN     0
#define UNIX    1
#define OS      UNIX

#if (OS == WIN)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#define ALWAYS_INLINE   __forceinline
#elif (OS == UNIX)
#include <pthread.h>

#define ALWAYS_INLINE   inline __attribute__((__always_inline__))
#endif

ALWAYS_INLINE static void CpuRelax()
{
#if (OS == WIN)
    _mm_pause();
#elif defined(__SSE2__)  // AMD and Intel
    _mm_pause();
#elif defined(__i386__) || defined(__x86_64__)
    asm volatile("pause");
#elif defined(__aarch64__)
    asm volatile("wfe");
#elif defined(__armel__) || defined(__ARMEL__)
    asm volatile ("nop" ::: "memory");  // default operation - does nothing => Might lead to passive spinning.
#elif defined(__arm__) || defined(__aarch64__) // arm big endian / arm64
    __asm__ __volatile__ ("yield" ::: "memory");
#elif defined(__ia64__)  // IA64
    __asm__ __volatile__ ("hint @pause");
#elif defined(__powerpc__) || defined(__ppc__) || defined(__PPC__) // PowerPC
     __asm__ __volatile__ ("or 27,27,27" ::: "memory");
#else  // everything else.
	asm volatile ("nop" ::: "memory");
	// default operation - does nothing => Might lead to passive spinning.
#endif
}

/**
 * @brief
 * 实现原理有点类似于 ticket spin lock，NextServingIdx 表示 下一个得到lock、能够进入critical region的，它就相当于ticket
 *
 *
 */
class AndersonSpinLock
{
public:
	AndersonSpinLock(size_t maxThreads = std::thread::hardware_concurrency()) :
					LockedFlags(maxThreads)
	{
		for (auto &flag : LockedFlags)
			flag.first = true;

		LockedFlags[0].first = false;
	}

	ALWAYS_INLINE void Enter()
	{
		/**
		 * `fetch_add` 返回的是之前的值
		 */
		const size_t index = NextFreeIdx.fetch_add(1) % LockedFlags.size();
		std::cout<<index<<std::endl;
		auto &flag = LockedFlags[index].first;

		// Ensure overflow never happens
		if (index == 0)
		{
			NextFreeIdx -= LockedFlags.size();
			std::cout<<NextFreeIdx<<std::endl;
		}
		while (flag)
			CpuRelax();

		flag = true;
	}

	ALWAYS_INLINE void Leave()
	{
		const size_t idx = NextServingIdx.fetch_add(1);
		LockedFlags[idx % LockedFlags.size()].first = false;
	}

private:
	using PaddedFlag = std::pair<std::atomic_bool, uint8_t[CACHELINE_SIZE-sizeof(std::atomic_bool)]>;
	static_assert(sizeof(PaddedFlag) == CACHELINE_SIZE, "");

	alignas(CACHELINE_SIZE) std::vector<PaddedFlag> LockedFlags;
	alignas(CACHELINE_SIZE) std::atomic_size_t NextFreeIdx = { 0 };alignas(CACHELINE_SIZE) std::atomic_size_t NextServingIdx = { 1 };
};

int main()
{
	AndersonSpinLock Lock{3};
	Lock.Enter();
	Lock.Leave();
}
// g++ test.cpp -pedantic -Wall -Wextra --std=c++11

```

