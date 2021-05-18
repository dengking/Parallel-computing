# mariusbancila [Dining Philosophers in C++11](https://mariusbancila.ro/blog/2017/01/16/dining-philosophers-in-cpp11/)

> NOTE: 
>
> 1、实际上是需要C++17支持的
>
> 

**UPDATE**: *for an implementation of the Chandy/Misra solution see [Dining philosophers in C++11: Chandy-Misra algorithm](https://mariusbancila.ro/blog/2017/01/20/dining-philosophers-in-c11-chandy-misra-algorithm/)*

The idea is to find a solution so that none of the philosophers would starve, i.e. never have the chance to acquire the forks necessary for him to eat.

Below I propose a simple implementation to this problem using C++11 language and library features. The following classes are defined:

## `fork` 

`fork` represents a fork at the table; the only member of this structure is a `std::mutex` that will be locked when the philosopher picks up the fork and unlocked when he puts it down.

```C++
struct fork
{
	std::mutex mutex;
};

```

## `table` 

`table` represents the round table where the philosophers are dining. It has an array of forks, but also an atomic boolean that indicates that the table is ready for the philosophers to start thinking and eating.

```C++
struct table
{
	std::atomic<bool> ready { false };
	std::array<fork, no_of_philosophers> forks;
};

```

## `philosopher` 

`philosopher` represents a philosopher dining at the table. It has a name and a reference to the forks on his left and right.

```C++
struct philosopher
{
private:
	std::string const name;
	table const &dinnertable;
	fork &left_fork; // 左边的叉子
	fork &right_fork; // 右边的叉子
	std::thread lifethread;
	std::mt19937 rng { std::random_device { }() };
};

```

Most of the implementation of the solution is part of the `philosopher` class. When an object of this class is instantiated, a thread is started. This thread is joined when the object is destroyed. The thread runs a loop of thinking and eating until the dinner is signaled to end by setting the `ready` member of the table to false. There are three main methods in the `philosopher` class:

### `dine()`

> NOTE: 
>
> 1、线程执行函数

`dine()` is the thread function; this is implemented as a simple loop of thinking and eating.

```C++
   void dine()
   {
      while (!dinnertable.ready);

      do
      {
         think();
         eat();
      } while (dinnertable.ready);
   }
```

### `think()`

`think()` is the method that represents the thinking period. To model this the thread sleeps for a random period of time.

```C++
   void think()
   {
      static thread_local std::uniform_int_distribution<> wait(1, 6);
      std::this_thread::sleep_for(std::chrono::milliseconds(wait(rng) * 150));

      print(" is thinking ");
   }
```

### `eat()`

> NOTE: 
>
> 一、通过查阅文档可知， [std::lock](http://en.cppreference.com/w/cpp/thread/lock) 是已经实现了"deadlock avoidance algorithm"，因此直接使用它就可以解决starvation的问题了。

`eat()` is the method that models the eating. The left and right forks are acquired in a **deadlock free** manner using [std::lock](http://en.cppreference.com/w/cpp/thread/lock). After the forks, i.e. mutexes, are acquired, their ownership is transfered to a [std::lock_guard](http://en.cppreference.com/w/cpp/thread/lock_guard) object, so that the mutexes are correctly released when the function returns. Eating is simulated with a sleep.

```C++
   void eat()
   {
      std::lock(left_fork.mutex, right_fork.mutex);

      std::lock_guard<std::mutex> left_lock(left_fork.mutex,   std::adopt_lock);
      std::lock_guard<std::mutex> right_lock(right_fork.mutex, std::adopt_lock);

      print(" started eating.");

      static thread_local std::uniform_int_distribution<> dist(1, 6);
      std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng) * 50));

      print(" finished eating.");
   }
```

## 完整的程序

The whole implementation is show below:

```C++
#include <array>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <random>
#include <iomanip>
#include <string_view>

std::mutex g_lockprint;
constexpr int no_of_philosophers = 5;

struct fork
{
	std::mutex mutex;
};

struct table
{
	std::atomic<bool> ready { false };
	std::array<fork, no_of_philosophers> forks;
};

struct philosopher
{
private:
	std::string const name;
	table const &dinnertable;
	fork &left_fork;
	fork &right_fork;
	std::thread lifethread;
	std::mt19937 rng { std::random_device { }() };
public:
	philosopher(std::string_view n, table const &t, fork &l, fork &r) :
					name(n), dinnertable(t), left_fork(l), right_fork(r), lifethread(&philosopher::dine, this)
	{
	}

	~philosopher()
	{
		lifethread.join();
	}

	void dine()
	{
		while (!dinnertable.ready)
			;

		do
		{
			think();
			eat();
		} while (dinnertable.ready);
	}

	void print(std::string_view text)
	{
		std::lock_guard<std::mutex> cout_lock(g_lockprint);
		std::cout << std::left << std::setw(10) << std::setfill(' ') << name << text << std::endl;
	}

	void eat()
	{
		std::lock(left_fork.mutex, right_fork.mutex);

		std::lock_guard<std::mutex> left_lock(left_fork.mutex, std::adopt_lock);
		std::lock_guard<std::mutex> right_lock(right_fork.mutex, std::adopt_lock);

		print(" started eating.");

		static thread_local std::uniform_int_distribution<> dist(1, 6);
		std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng) * 50));

		print(" finished eating.");
	}

	void think()
	{
		static thread_local std::uniform_int_distribution<> wait(1, 6);
		std::this_thread::sleep_for(std::chrono::milliseconds(wait(rng) * 150));

		print(" is thinking ");
	}
};

void dine()
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "Dinner started!" << std::endl;

	{
		table table;
		std::array<philosopher, no_of_philosophers> philosophers { { { "Aristotle", table, table.forks[0], table.forks[1] }, { "Platon", table, table.forks[1], table.forks[2] }, { "Descartes", table, table.forks[2], table.forks[3] }, { "Kant", table, table.forks[3], table.forks[4] }, { "Nietzsche", table, table.forks[4], table.forks[0] }, } };

		table.ready = true;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		table.ready = false;
	}

	std::cout << "Dinner done!" << std::endl;
}

int main()
{
	dine();

	return 0;
}
// g++ test.cpp -pedantic -Wall -Wextra --std=c++17 -lpthread


```

从实际的运行结构来看，并没有发生deadlock而导致starve。