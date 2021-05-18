# mariusbancila [Dining philosophers in C++11: Chandy-Misra algorithm](https://mariusbancila.ro/blog/2017/01/20/dining-philosophers-in-c11-chandy-misra-algorithm/)



In my previous post, [Dining Philosophers in C++11](https://mariusbancila.ro/blog/2017/01/16/dining-philosophers-in-cpp11/), I have provided an implementation for the dining philosophers problem using modern C++ features, such as threads and mutexes. However, it was [noted in the comments](https://mariusbancila.ro/blog/2017/01/16/dining-philosophers-in-cpp11/#comment-390346) that the implementation did not prevent the philosophers starving to death when you remove the waiting times.

An algorithm that prevents the philosophers from starving was proposed by Mani Chandy and J. Misra and is known as the [Chandy/Misra solution](https://en.wikipedia.org/wiki/Dining_philosophers_problem#Chandy.2FMisra_solution). This is a bit different than the original problem because it requires the philosophers to communicate with each other. The algorithm, as described on Wikipedia, is the following:

> 1. For every pair of philosophers contending for a resource, create a fork and give it to the philosopher with the lower ID (n for agent Pn). Each fork can either be dirty or clean. Initially, all forks are dirty.
> 2. When a philosopher wants to use a set of resources (i.e. eat), said philosopher must obtain the forks from their contending neighbors. For all such forks the philosopher does not have, they send a request message.
> 3. When a philosopher with a fork receives a request message, they keep the fork if it is clean, but give it up when it is dirty. If the philosopher sends the fork over, they clean the fork before doing so.
> 4. After a philosopher is done eating, all their forks become dirty. If another philosopher had previously requested one of the forks, the philosopher that has just finished eating cleans the fork and sends it.

In order to implement this, we must make several changes to the solution proposed in the previous post:

1、forks and philosophers must have identifiers

> NOTE: 
>
> 1、初始化的时候，ID小的philosophers 得到fork

2、there is an initial setup of both forks and philosophers

3、use `std::condition_variable` to communicate between threads

> NOTE: 
>
> 1、channel

4、increase the number of philosophers

Because it has been also argued that `string_view` is only available in C++17 and this implementation is supposed to work in C++11, I have replaced that with `std::string const&`.

## `sync_channel`

In this implementation, philosophers, i.e. threads, need to communicate with each other to request the forks, i.e. resources. For this, we will use a `std::condition_variable`, which is a synchronization primitive that enables the blocking of one or more threads until another thread notifies it. A `std::condition_variable` requires a `std::mutex` to protect access to a shared variable. The following class, `sync_channel`, contains both a condition variable and a mutex and provides two methods: one that waits on the condition variable, blocking the calling thread(s), and one that notifies the condition variable, unblocking all the threads that are waiting for a signal.

> NOTE: 
>
> 1、tag-std condition variable channel block notify_all唤醒通知multiple waiting thread
>
> 这种做法是值得借鉴的

```C++
class sync_channel
{
   std::mutex              mutex;
   std::condition_variable cv;

public:
   void wait()
   {
      std::unique_lock<std::mutex> lock(mutex);
      cv.wait(lock);
   }

   void notifyall()
   {
      std::unique_lock<std::mutex> lock(mutex);
      cv.notify_all();
   }
};
```

## `table_setup`

The `table` class from the previous implementation is modified: the forks are no longer defined here, but a `sync_channel` is used to prevent philosophers start dining until the table setup is completed. Its name has been changed to `table_setup`.

```
struct table_setup
{
   std::atomic<bool> done{ false };
   sync_channel      channel;
};
```

## `fork` 

The `fork` class is no longer a wrapper for a mutex. It has an identifier, an owner, a flag to indicate whether it is dirty or clean, a `mutex`, and a `sync_channel` that enables owners to request used forks. It has two methods:

### `request()` method

 `request()` that enables a philosopher to request the fork. If the fork is dirty, it is set to clean, and the ownership is given to the philosopher that asked for it. If the fork is clean (i.e. the current owner is eating), than the philosopher that asked for it will block, waiting for it to become dirty (i.e. the current owner has finished eating).

```C++
void request(int const ownerId)
{
   while (owner != ownerId)
   {
      if (dirty)
      {
         std::lock_guard<std::mutex> lock(mutex);

         dirty = false;
         owner = ownerId;
      }
      else
      {
         channel.wait();
      }
   }
}
```

### `done_using()` method

 `done_using()` a philosopher indicates that has finished eating and notifies other philosopher that is waiting for the fork that it can have it.

```C++
void done_using()
{
   dirty = true;
   channel.notifyall();
}
```

## `philosopher` 

There are less changes to the `philosopher` class: it has an identifier, and there are no more waiting times to simulate eating and thinking. There are some small changes to the following methods:

### `dine()` method

each philosopher only starts eating after the entire table has been setup. A condition variable, from the `table_setup` object is used for this.

```c++
void dine()
{
   setup.channel.wait();

   do
   {
      think();
      eat();
   } while (!setup.done);
}
```

### `eat()` method

each philosopher first requests the left and right fork. When they are available, they are locked using `std::lock()` to avoid possible deadlocks, and then their ownership is transfered to a `std::lock_guard` object, so they are properly released when done. After eating, the fork is set as dirty and other philosophers waiting for it are notified of this.

```C++
void eat()
{
   left_fork.request(id);
   right_fork.request(id);

   std::lock(left_fork.getmutex(), right_fork.getmutex());

   std::lock_guard<std::mutex> left_lock(left_fork.getmutex(), std::adopt_lock);
   std::lock_guard<std::mutex> right_lock(right_fork.getmutex(), std::adopt_lock);

   print(" started eating.");
   print(" finished eating.");

   left_fork.done_using();
   right_fork.done_using();
}
```

According to the initial setup, each fork is given to the philosopher with the lower ID. That means fork 1, placed between philosopher 1 and N, goes to philosopher 1. Fork 2, placed between philosophers 2 and 3 is given to philosopher 2. Eventually, fork N, placed between philosophers N and 1, is given to philosopher 1. Overall, this means all philosophers have initially 1 fork, except for the first one that has two, and the last philosopher, that has none.

## 完整程序

Put all together, the code looks like this:

```C++
#include <array>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <iomanip>
#include <condition_variable>

std::mutex g_lockprint;
constexpr int no_of_philosophers = 7;

class sync_channel
{
	std::mutex mutex;
	std::condition_variable cv;

public:
	void wait()
	{
		std::unique_lock<std::mutex> lock(mutex);
		cv.wait(lock);
	}

	void notifyall()
	{
		std::unique_lock<std::mutex> lock(mutex);
		cv.notify_all();
	}
};

struct table_setup
{
	std::atomic<bool> done { false };
	sync_channel channel;
};

class fork
{
	int id;
	int owner;
	bool dirty;
	std::mutex mutex;
	sync_channel channel;

public:
	fork(int const forkId, int const ownerId) :
					id(forkId), owner(ownerId), dirty(true)
	{
	}

	void request(int const ownerId)
	{
		while (owner != ownerId)
		{
			if (dirty)
			{
				std::lock_guard<std::mutex> lock(mutex);

				dirty = false;
				owner = ownerId;
			}
			else
			{
				channel.wait();
			}
		}
	}

	void done_using()
	{
		dirty = true;
		channel.notifyall();
	}

	std::mutex& getmutex()
	{
		return mutex;
	}
};

struct philosopher
{
private:
	int id;
	std::string const name;
	table_setup &setup;
	fork &left_fork;
	fork &right_fork;
	std::thread lifethread;
public:
	philosopher(int const id, std::string const &n, table_setup &s, fork &l, fork &r) :
					id(id), name(n), setup(s), left_fork(l), right_fork(r), lifethread(&philosopher::dine, this)
	{
	}

	~philosopher()
	{
		lifethread.join();
	}

	void dine()
	{
		setup.channel.wait();

		do
		{
			think();
			eat();
		} while (!setup.done);
	}

	void print(std::string const &text)
	{
		std::lock_guard<std::mutex> cout_lock(g_lockprint);
		std::cout << std::left << std::setw(10) << std::setfill(' ') << name << text << std::endl;
	}

	void eat()
	{
		print(" get left fork ");
		left_fork.request(id);
		print(" get right fork ");
		right_fork.request(id);

		std::lock(left_fork.getmutex(), right_fork.getmutex());

		std::lock_guard<std::mutex> left_lock(left_fork.getmutex(), std::adopt_lock);
		std::lock_guard<std::mutex> right_lock(right_fork.getmutex(), std::adopt_lock);

		print(" started eating.");
		print(" finished eating.");

		left_fork.done_using();
		right_fork.done_using();
	}

	void think()
	{
		print(" is thinking ");
	}
};

class table
{
	table_setup setup;

	/**
	 * 最后一个叉子是要给第一个philosopher的
	 */
	std::array<fork, no_of_philosophers> forks { { { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 }, { 6, 6 }, { 7, 1 }, } };

	std::array<philosopher, no_of_philosophers> philosophers { { { 1, "Aristotle", setup, forks[0], forks[1] }, { 2, "Platon", setup, forks[1], forks[2] }, { 3, "Descartes", setup, forks[2], forks[3] }, { 4, "Kant", setup, forks[3], forks[4] }, { 5, "Nietzsche", setup, forks[4], forks[5] }, { 6, "Hume", setup, forks[5], forks[6] }, { 7, "Russell", setup, forks[6], forks[0] }, } };

public:
	void start()
	{
		setup.channel.notifyall();
	}

	void stop()
	{
		setup.done = true;
	}
};

void dine()
{
	std::cout << "Dinner started!" << std::endl;

	{
		table table;

		table.start();
		std::this_thread::sleep_for(std::chrono::seconds(60));
		table.stop();
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

## 缺陷分析

作者给出的原版程序是存在问题的: 

它尝试去stop a blocked thread，显然它是无法被join的，因此被阻塞在下面的函数中:

```C++
	~philosopher()
	{
		lifethread.join();
	}
```

显然，在停止的时候，需要将阻塞的thread唤醒，下面是我修改的一个版本，它没有改彻底，还是存在无法停止的问题，关于完整的修改方法，可以参考:

1、`Stop-a-blocked-thread`

2、`Stop-a-sleeping-thread`

### 一个不彻底的修改

```C++
#include <array>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <iomanip>
#include <condition_variable>

std::mutex g_lockprint;
constexpr int no_of_philosophers = 7;

class sync_channel
{
	std::mutex mutex;
	std::condition_variable cv;

public:
	void wait()
	{
		std::unique_lock<std::mutex> lock(mutex);
		cv.wait(lock);
	}

	void notifyall()
	{
		std::unique_lock<std::mutex> lock(mutex);
		cv.notify_all();
	}
};

struct table_setup
{
	std::atomic<bool> done { false };
	sync_channel channel;
};

class fork
{
	int id;
	int owner;
	bool dirty;
	std::mutex mutex;

public:
	sync_channel channel;

	fork(int const forkId, int const ownerId) :
					id(forkId), owner(ownerId), dirty(true)
	{
	}

	void request(int const ownerId)
	{
		while (owner != ownerId)
		{
			if (dirty)
			{
				std::lock_guard<std::mutex> lock(mutex);

				dirty = false;
				owner = ownerId;
			}
			else
			{
				channel.wait();
			}
		}
	}

	void done_using()
	{
		dirty = true;
		channel.notifyall();
	}

	std::mutex& getmutex()
	{
		return mutex;
	}
};

struct philosopher
{
private:
	int id;
	std::string const name;
	table_setup &setup;
	fork &left_fork;
	fork &right_fork;
	std::thread lifethread;
public:
	philosopher(int const id, std::string const &n, table_setup &s, fork &l, fork &r) :
					id(id), name(n), setup(s), left_fork(l), right_fork(r), lifethread(&philosopher::dine, this)
	{
	}

	~philosopher()
	{
		lifethread.join();
	}

	void dine()
	{
		setup.channel.wait();

		do
		{
			think();
			eat();
		} while (!setup.done);
	}

	void print(std::string const &text)
	{
		std::lock_guard<std::mutex> cout_lock(g_lockprint);
		std::cout << std::left << std::setw(10) << std::setfill(' ') << name << text << std::endl;
	}

	void eat()
	{
		left_fork.request(id);
		right_fork.request(id);

		std::lock(left_fork.getmutex(), right_fork.getmutex());

		std::lock_guard<std::mutex> left_lock(left_fork.getmutex(), std::adopt_lock);
		std::lock_guard<std::mutex> right_lock(right_fork.getmutex(), std::adopt_lock);

		print(" started eating.");
		print(" finished eating.");

		left_fork.done_using();
		right_fork.done_using();
	}

	void think()
	{
		print(" is thinking ");
	}
};

class table
{
	table_setup setup;

	/**
	 * 最后一个叉子是要给第一个philosopher的
	 */
	std::array<fork, no_of_philosophers> forks { { { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 }, { 6, 6 }, { 7, 1 }, } };

	std::array<philosopher, no_of_philosophers> philosophers { { { 1, "Aristotle", setup, forks[0], forks[1] }, { 2, "Platon", setup, forks[1], forks[2] }, { 3, "Descartes", setup, forks[2], forks[3] }, { 4, "Kant", setup, forks[3], forks[4] }, { 5, "Nietzsche", setup, forks[4], forks[5] }, { 6, "Hume", setup, forks[5], forks[6] }, { 7, "Russell", setup, forks[6], forks[0] }, } };

public:
	void start()
	{

		setup.channel.notifyall();
	}

	void stop()
	{
		setup.done = true;
		for (auto &&fork : forks)
		{
			fork.channel.notifyall();
		}
	}
};

void dine()
{
	std::cout << "Dinner started!" << std::endl;

	{
		table table;

		table.start();
		std::this_thread::sleep_for(std::chrono::seconds(30));
		std::cout << "停止" << std::endl;
		table.stop();
	}

	std::cout << "Dinner done!" << std::endl;
}

int main()
{
	dine();

	return 0;
}
// g++ test.cpp -pedantic -Wall -Wextra --std=c++11 -lpthread


```



## 总结分析

由于一个fork只有两个philosopher使用，因此上述implementation中，每个fork使用一个condition variable即可，如果说，三个甚至更多的philosopher使用同一个fork，那么在这些philosopher之间如何来进行协调分配呢？显然，这种情况下，使用一个condition variable是不行的。