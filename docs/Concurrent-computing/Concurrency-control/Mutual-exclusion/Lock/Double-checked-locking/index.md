# Double-checked locking



## Application

1、execute once

保证指定的code、logic即使在multi thread、multi process等等情况下只"execute once"是"double-checked locking"的主要application，后续的很多application都是基于它的这个特性的。

2、singleton

保证constructor只能怪被执行一次、只有一个object

3、lazy initialization

参见下面的内容

## Implementation

1、justsoftwaresolutions [Multithreading in C++0x part 6: Lazy initialization and double-checked locking with atomics](https://www.justsoftwaresolutions.co.uk/threading/multithreading-in-c++0x-part-6-double-checked-locking.html)

2、preshing [Double-Checked Locking is Fixed In C++11](https://preshing.com/20130930/double-checked-locking-is-fixed-in-cpp11/)

3、[preshing](https://github.com/preshing)/**[cpp11-on-multicore](https://github.com/preshing/cpp11-on-multicore)** **inmemorylogger**

## wikipedia [Double-checked locking](https://en.wikipedia.org/wiki/Double-checked_locking)

In [software engineering](https://en.wikipedia.org/wiki/Software_engineering), **double-checked locking** (also known as "double-checked locking optimization"[[1\]](https://en.wikipedia.org/wiki/Double-checked_locking#cite_note-1)) is a [software design pattern](https://en.wikipedia.org/wiki/Software_design_pattern) used to reduce the overhead of acquiring a [lock](https://en.wikipedia.org/wiki/Lock_(computer_science)) by testing the locking criterion (the "lock hint") before acquiring the lock. Locking occurs only if the locking criterion check indicates that locking is required.

> NOTE: 
>
> 1、"Double-checked locking"是一种加锁的方式，而不是某种lock

The pattern, when implemented in some language/hardware combinations, can be unsafe. At times, it can be considered an [anti-pattern](https://en.wikipedia.org/wiki/Anti-pattern).[[2\]](https://en.wikipedia.org/wiki/Double-checked_locking#cite_note-bdec-2)

> NOTE: 
>
> 1、虽然原文给出了reference，我目前阅读的最好的文章是:  aristeia [C++ and the Perils of Double-Checked Locking](https://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf) 

It is typically used to reduce locking overhead when implementing "[lazy initialization](https://en.wikipedia.org/wiki/Lazy_initialization)" in a multi-threaded environment, especially as part of the [Singleton pattern](https://en.wikipedia.org/wiki/Singleton_pattern). Lazy initialization avoids initializing a value until the first time it is accessed.

> NOTE: 
>
> 1、这段话，对lazy initialization的解释非常好

### Usage in C++11

For the singleton pattern, double-checked locking is not needed:

> If control enters the declaration concurrently while the variable is being initialized, the concurrent execution shall wait for completion of the initialization.
>
> — § 6.7 [stmt.dcl] p4

```C++
Singleton& GetInstance()
{
	static Singleton s;
	return s;
}

```

C++11 and beyond also provide a built-in double-checked locking pattern in the form of `std::once_flag` and `std::call_once`:

```c++
#include <mutex>
#include <optional> // Since C++17

// Singleton.h
class Singleton
{
public:
	static Singleton* GetInstance();
private:
	Singleton() = default;

	static std::optional<Singleton> s_instance;
	static std::once_flag s_flag;
};

// Singleton.cpp
std::optional<Singleton> Singleton::s_instance;
std::once_flag Singleton::s_flag { };

Singleton* Singleton::GetInstance()
{
	std::call_once(Singleton::s_flag, []()
	{	s_instance.emplace(Singleton
						{});});
	return &*s_instance;
}

```

If one truly wishes to use the double-checked idiom instead of the trivially working example above (for instance because Visual Studio before the 2015 release did not implement the C++11 standard's language about concurrent initialization quoted above [[3\]](https://en.wikipedia.org/wiki/Double-checked_locking#cite_note-3) ), one needs to use acquire and release fences:[[4\]](https://en.wikipedia.org/wiki/Double-checked_locking#cite_note-4)

```c++
#include <atomic>
#include <mutex>

class Singleton
{
public:
	static Singleton* GetInstance();

private:
	Singleton() = default;

	static std::atomic<Singleton*> s_instance;
	static std::mutex s_mutex;
};

Singleton* Singleton::GetInstance()
{
	Singleton *p = s_instance.load(std::memory_order_acquire);
	if (p == nullptr)
	{ // 1st check
		std::lock_guard<std::mutex> lock(s_mutex);
		p = s_instance.load(std::memory_order_relaxed);
		if (p == nullptr)
		{ // 2nd (double) check
			p = new Singleton();
			s_instance.store(p, std::memory_order_release);
		}
	}
	return p;
}

```