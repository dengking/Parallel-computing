# 如何中断一个正在sleeping的thread

## Summary

总的来说，目前知道的是两种方案，这两种方案在 stackoverflow [Stopping long-sleep threads](https://stackoverflow.com/questions/29775153/stopping-long-sleep-threads) 中都提出来了。

## stackoverflow [how to terminate a sleeping thread in pthread?](https://stackoverflow.com/questions/4778361/how-to-terminate-a-sleeping-thread-in-pthread)

I have thread which sleeps for a long time, then wakes up to do something, then sleep again, like this:

```c++
while(some_condition)
{
    // do something
    sleep(1000);
}
```

How could I make this thread exit gracefully and QUICKLY?

I tried to use `pthread_cancel()`, but **sleeping threads** could not be canceled. I also tried changing the condition of the while loop, but it will still take long to exit. And I don't want to use `pthread_kill()`, since it may kill the thread when it's working.

So, are there any good ideas?

***COMMENTS***

well, i think i make a mistake, `sleep()` is a **cancel point** difined by posix.1 – [qiuxiafei](https://stackoverflow.com/users/495865/qiuxiafei) [Jan 24 '11 at 3:40](https://stackoverflow.com/questions/4778361/how-to-terminate-a-sleeping-thread-in-pthread#comment5291120_4778361)

Just a note - `pthread_kill` does not kill a thread. It sends a **signal** to a **thread**. If the action of that signal is to terminate the **process** (e.g. the default action of `SIGTERM`, or the unblockable action of `SIGKILL`) then it will terminate *the whole process*, not the target thread. Really the only time `pthread_kill` is useful is if you've installed an **interrupting signal handler**, and you want to **interrupt a syscall** that's blocked in a particular thread (which would be a potential solution to your question). – [R..](https://stackoverflow.com/users/379897/r) [May 18 '11 at 23:11](https://stackoverflow.com/questions/4778361/how-to-terminate-a-sleeping-thread-in-pthread#comment7003547_4778361) 

Why did you say that the thread could not exit quickly ? I tried this and the thread canceled immediately upon `thread_cancel`. And I was able to `pthread_join` without delay. – [user1502776](https://stackoverflow.com/users/1502776/user1502776) [Jun 7 at 8:15](https://stackoverflow.com/questions/4778361/how-to-terminate-a-sleeping-thread-in-pthread#comment99569913_4778361)

> NOTE: 关于`pthread_cancel`参见[`pthread_cancel`](http://man7.org/linux/man-pages/man3/pthread_cancel.3.html) ，其中提及`sleep`是一个cancel point

### [A](https://stackoverflow.com/a/4778415)

As an alternative to `sleep`, you could use [pthread_cond_timedwait](http://pubs.opengroup.org/onlinepubs/009695399/functions/pthread_cond_timedwait.html) with a 1000 ms timeout. Then when you want to exit, signal the condition variable.

This is similar to how you might do this in C#/Java using wait and notify.

***COMMENTS***

Correct. This is the simple and non-racy way to do it. – [caf](https://stackoverflow.com/users/134633/caf) [Jan 24 '11 at 4:04](https://stackoverflow.com/questions/4778361/how-to-terminate-a-sleeping-thread-in-pthread#comment5291284_4778415)

This is correct, but cancellation is easier and perhaps slightly more efficient. Of course if you call `pthread_cancel` on the thread, you need to make sure that it can't get cancelled at a point it shouldn't. Calling `pthread_setcancelstate` to disable cancellation when the thread starts, and only enabling it just before the `sleep`, would be a safe approach. – [R..](https://stackoverflow.com/users/379897/r) [May 18 '11 at 23:10](https://stackoverflow.com/questions/4778361/how-to-terminate-a-sleeping-thread-in-pthread#comment7003533_4778415)

IMO, `pthread_cancel()` may be preferable since `sleep()` is already a cancellation point. And be sure to call `pthread_cleanup_push()` and `pthread_cleanup_pop()` to set up cleanup handlers if you have allocated resources. – [zeekvfu](https://stackoverflow.com/users/1108071/zeekvfu) [Oct 22 '13 at 4:45](https://stackoverflow.com/questions/4778361/how-to-terminate-a-sleeping-thread-in-pthread#comment28940474_4778415)

What is the difference between using `pthread_cond_timedwait` with 1000ms timeout and using a flag variable with 100ms sleep in regard to race condition ? – [user1502776](https://stackoverflow.com/users/1502776/user1502776) [Jun 5 at 5:05](https://stackoverflow.com/questions/4778361/how-to-terminate-a-sleeping-thread-in-pthread#comment99501484_4778415)



## stackoverflow [How can I kill a thread? without using stop();](https://stackoverflow.com/questions/5915156/how-can-i-kill-a-thread-without-using-stop)

I'm wanting a reasonably reliable threaded timer, so I've written a timer object that fires a `std::function` on a thread. I would like to give this timer the ability to **stop** before it gets to the next tick; something you can't do with `::sleep` (at least I don't think you can).



So what I've done is put a **condition variable** on a mutex. If the condition times out, I fire the event. If the condition is signalled the thread is exited（一个condition variable兼具两种功能）. So the `Stop` method needs to be able to get the thread to stop and/or interrupt its wait, which I think is what it's doing right now.



There are problems with this however. Sometimes the thread isn't `joinable()` and sometimes the condition is signalled *after* its timeout but before it's put into its wait state.

How can I improve this and make it robust?

The following is a full repo. The wait is 10 seconds here but the program should terminate immediately as the `Foo` is created and then immediately destroyed. It does sometimes but mostly it does not.

```cpp
#include <atomic>
#include <thread>
#include <future>
#include <sstream>
#include <chrono>
#include <iostream>

class Timer
{
public:

    Timer() {}

    ~Timer()
    {           
        Stop();
    }

    void Start(std::chrono::milliseconds const & interval, std::function<void(void)> const & callback)
    {   
        Stop();

        thread = std::thread([=]()
        {
            for(;;)
            {
                auto locked = std::unique_lock<std::mutex>(mutex);
                auto result = terminate.wait_for(locked, interval);

                if (result == std::cv_status::timeout)
                {
                    callback();
                }
                else
                {
                    return;
                }
            }
        });
    }

    void Stop()
    {       
        terminate.notify_one();

        if(thread.joinable())
        {
            thread.join();
        }
    }

private:

    std::thread thread;
    std::mutex mutex;
    std::condition_variable terminate;
};

class Foo
{
public: 

    Foo()
    {
        timer = std::make_unique<Timer>();
        timer->Start(std::chrono::milliseconds(10000), std::bind(&Foo::Callback, this));
    }

    ~Foo()
    {

    }

    void Callback()
    {
        static int count = 0;

        std::ostringstream o;

        std::cout << count++ << std::endl;
    }

    std::unique_ptr<Timer> timer;
};


int main(void)
{
    {
        Foo foo;
    }

    return 0;
}
```



Somewhere you need a **variable** protected by the **mutex** that stores whether or not the thread is supposed to stop. **Condition variables** are stateless -- it is your responsibility to maintain the state of the thing you're waiting for (called the "predicate"). Basically, you've missed the point of **condition variables**. Your `Stop` function notifies the thread. But it hasn't changed any condition that the thread is waiting for! Notice that the mutex doesn't protect anything. This is just totally wrong. – [David Schwartz](https://stackoverflow.com/users/721269/david-schwartz) [Sep 21 '15 at 10:44](https://stackoverflow.com/questions/32693103/threaded-timer-interrupting-a-sleep-stopping-it#comment53229475_32693103) 

Why `return` on a spurious wakeup? (Same problem. You have no way to know whether you should wakeup or not because you have no predicate to check.) – [David Schwartz](https://stackoverflow.com/users/721269/david-schwartz) [Sep 21 '15 at 11:03](https://stackoverflow.com/questions/32693103/threaded-timer-interrupting-a-sleep-stopping-it#comment53230191_32693103) 

***SUMMARY*** : 上述并不涉及**condition**的修改；使用condition variable的一个非常重要的目的是仅仅当condition满足的时候才运行thread继续执行，而当[Spurious wakeup](https://en.wikipedia.org/wiki/Spurious_wakeup)的时候，则不能够允许它允许，所以才需要一个condition；而上述代码则显然没有这样做，如下面的评论所述，它存在着这样的一个风险： `return` on a spurious wakeup

What other kinds of wakeup are there? There's timeout and user signalled the wait. Can the OS wake it up for other reasons? – [Robinson](https://stackoverflow.com/users/416274/robinson) [Sep 21 '15 at 11:04](https://stackoverflow.com/questions/32693103/threaded-timer-interrupting-a-sleep-stopping-it#comment53230250_32693103)

You still don't understand condition variables. They are stateless. You are imagining that you will only be woken up in some particular set of states, but they don't have states. It's your responsibility to track the state. There is no "signalled" state, so you have no way to tell if there was a signal. – [David Schwartz](https://stackoverflow.com/users/721269/david-schwartz) [Sep 21 '15 at 11:06](https://stackoverflow.com/questions/32693103/threaded-timer-interrupting-a-sleep-stopping-it#comment53230324_32693103) 

If you think there is some particular state in which you will be woken up, answer me this -- in your code, what variable holds this state? In my code, it's `stop`. What is it in yours? Tracking state is your responsibility. The call to `notify_one` doesn't change the state to a signaled state. – [David Schwartz](https://stackoverflow.com/users/721269/david-schwartz) [Sep 21 '15 at 11:07](https://stackoverflow.com/questions/32693103/threaded-timer-interrupting-a-sleep-stopping-it#comment53230369_32693103) 

You need some way to know whether the thing the thread is waiting for has happened or not. That is the state. Condition variables are stateless. Maintaining that state is your responsibility. The condition variable cannot assure that a wakeup has occurred without tracking the state itself (how else would it do that?) and it, by design, doesn't do that. – [David Schwartz](https://stackoverflow.com/users/721269/david-schwartz) [Sep 21 '15 at 11:20](https://stackoverflow.com/questions/32693103/threaded-timer-interrupting-a-sleep-stopping-it#comment53230826_32693103)



### [A](https://stackoverflow.com/a/32693379)

See my comment. You forgot to implement the state of the thing the thread is waiting for, leaving the mutex nothing to protect and the thread nothing to wait for. Condition variables are stateless -- your code must track the state of the thing whose change you're notifying the thread about.

Here's the code fixed. Notice that the mutex protects `stop`, and `stop` is the thing the thread is waiting for.

```cpp
    class Timer
    {
    public:

        Timer() {}

        ~Timer()
        {           
            Stop();
        }

        void Start(std::chrono::milliseconds const & interval,
            std::function<void(void)> const & callback)
        {   
            Stop();

            {
                auto locked = std::unique_lock<std::mutex>(mutex);    
                stop = false;
            }

            thread = std::thread([=]()
            {
                auto locked = std::unique_lock<std::mutex>(mutex);    

                while (! stop) // We hold the mutex that protects stop
                {
                    auto result = terminate.wait_for(locked, interval);

                    if (result == std::cv_status::timeout)
                    {
                        callback();
                    }
                }
            });
        }

        void Stop()
        {    
            {     
                // Set the predicate
                auto locked = std::unique_lock<std::mutex>(mutex);
                stop = true;
            }

            // Tell the thread the predicate has changed
            terminate.notify_one();

            if(thread.joinable())
            {
                thread.join();
            }
        }

    private:

        bool stop; // This is the thing the thread is waiting for
        std::thread thread;
        std::mutex mutex;
        std::condition_variable terminate;
    };
```



## stackoverflow [Stopping long-sleep threads](https://stackoverflow.com/questions/29775153/stopping-long-sleep-threads)

Let's suppose I have a thread which should perform some task periodically but this period is ~~6 times each hour~~ 12 times each hour (every 5 minutes), I've often seen code which controls the thread loop with a *is_running* flag which is checked every loop, like this:

```cpp
std::atomic<bool> is_running;

void start()
{
    is_running.store(true);
    std::thread { thread_function }.detach();
}

void stop()
{
    is_running.store(false);
}

void thread_function()
{
    using namespace std::literals;
    while (is_running.load())
    {
        // do some task...
        std::this_thread::sleep_for(5min);
    }
}
```

But if the `stop()` function is called, let's say, 1 millisecond after `start()` the thread would be alive for 299999 additional milliseconds until it awakes, checks the flag, and die.

Is my understanding correct? How to avoid keeping alive (but sleeping) a thread which should have been ended? My best approach until now is the following:

```cpp
void thread_function()
{
    using namespace std::literals;
    while (is_running.load())
    {
        // do some task...
        for (unsigned int b = 0u, e = 1500u; is_running.load() && (b != e); ++b)
        {
            // 1500 * 200 = 300000ms = 5min
            std::this_thread::sleep_for(200ms);
        }
    }
}
```

Is there a less-dirty and more straightforward way to achieve this?

### Comments

[ en.cppreference.com/w/cpp/thread/condition_variable](http://en.cppreference.com/w/cpp/thread/condition_variable), see first sentence there. Instead of sleeping for a fixed amount of time, you enter a signallable wait state for that amount of time so other threads can still interrupt you – [stijn](https://stackoverflow.com/users/128384/stijn) [Apr 21 '15 at 14:40](https://stackoverflow.com/questions/29775153/stopping-long-sleep-threads#comment47681120_29775153) 

Another option is [boost::basic_waitable_timer](http://www.boost.org/doc/libs/1_49_0/doc/html/boost_asio/reference/basic_waitable_timer.html). – [tenfour](https://stackoverflow.com/users/402169/tenfour) [Apr 21 '15 at 14:46](https://stackoverflow.com/questions/29775153/stopping-long-sleep-threads#comment47681359_29775153) 



### [A](https://stackoverflow.com/a/29775639)

Use a condition variable. You wait on the condition variable *or* 5 minutes passing. Remember to check for spurious wakeups.

[cppreference](http://en.cppreference.com/w/cpp/thread/condition_variable)

I cannot find a good stack overflow post on how to use a condition variable in a minute or two of google searching. The tricky part is realizing that the `wait` can wake up with neither 5 minutes passing, nor a signal being sent. The cleanest way to handle this is to use the wait methods with a lambda that double-checks that the wakeup was a "good" one.

> NOTE: double check来避免**spurious wakeup**

[here](http://en.cppreference.com/w/cpp/thread/condition_variable/wait_until) is some sample code over at cppreference that uses `wait_until` with a lambda. (`wait_for` with a lambda is equivalent to `wait_until` with a lambda). I modified it slightly.

Here is an version:

```cpp
#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <future>
#include <chrono>
#include <iostream>
#include <vector>

using namespace std::literals::chrono_literals;

struct timer_killer
{
	// returns false if killed:
	template<class R, class P>
	bool wait_for(std::chrono::duration<R, P> const &time)
	{
		std::unique_lock<std::mutex> lock(m);
		return !cv.wait_for(lock, time, [&]
		{	return terminate;});
	}
	void kill()
	{
		std::unique_lock<std::mutex> lock(m);
		terminate=true;
		cv.notify_all();
	}
private:
	std::condition_variable cv;
	std::mutex m;
	bool terminate = false;
};

timer_killer bob;

int main()
{
	std::vector<std::future<void> > tasks;
	tasks.push_back(std::async(std::launch::async, []
	{
		while(bob.wait_for(500ms))
		{
			std::cout << "thread 1 says hi\n";
		}
		std::cout << "thread 1 dead\n";
	}));
	bob.wait_for(250ms);
	tasks.push_back(std::async(std::launch::async, []
	{
		while(bob.wait_for(500ms))
		{
			std::cout << "thread 2 says hi\n";
		}
		std::cout << "thread 2 dead\n";
	}));
	bob.wait_for(1000ms);
	std::cout << "killing threads\n";
	bob.kill();
	for (auto &&f : tasks)
		f.wait();
	std::cout << "done\n";
	// your code goes here
	return 0;
}
// g++ -std=c++17 -O2 -Wall -pedantic -pthread main.cpp && ./a.out
```

> NOTE: 上述程序是非常值得学习的，为了使程序的效果更加显著，可以使用下面的版本:
>
> ```C++
> #include <iostream>
> #include <thread>
> #include <condition_variable>
> #include <mutex>
> #include <future>
> #include <chrono>
> #include <iostream>
> #include <vector>
> 
> using namespace std::literals::chrono_literals;
> 
> struct timer_killer
> {
> 	// returns false if killed:
> 	template<class R, class P>
> 	bool wait_for(std::chrono::duration<R, P> const &time)
> 	{
> 		std::unique_lock<std::mutex> lock(m);
> 		return !cv.wait_for(lock, time, [&]
> 		{	return terminate;});
> 	}
> 	void kill()
> 	{
> 		std::unique_lock<std::mutex> lock(m);
> 		terminate=true;
> 		cv.notify_all();
> 	}
> private:
> 	std::condition_variable cv;
> 	std::mutex m;
> 	bool terminate = false;
> };
> 
> timer_killer bob;
> 
> int main()
> {
> 	std::vector<std::future<void> > tasks;
> 	tasks.push_back(std::async(std::launch::async, []
> 	{
> 		while(bob.wait_for(50ms))
> 		{
> 			std::cout << "thread 1 says hi\n";
> 		}
> 		std::cout << "thread 1 dead\n";
> 	}));
> 	bob.wait_for(250ms);
> 	tasks.push_back(std::async(std::launch::async, []
> 	{
> 		while(bob.wait_for(50ms))
> 		{
> 			std::cout << "thread 2 says hi\n";
> 		}
> 		std::cout << "thread 2 dead\n";
> 	}));
> 	bob.wait_for(1000ms);
> 	std::cout << "killing threads\n";
> 	bob.kill();
> 	for (auto &&f : tasks)
> 		f.wait();
> 	std::cout << "done\n";
> 	// your code goes here
> 	return 0;
> }
> // g++ -std=c++17 -O2 -Wall -pedantic -pthread main.cpp && ./a.out
> ```
>
> 在 `bob.kill();` 执行之前，`asynchronous task` 会重复执行多次；
>
> 下面是C++11版本的
>
> ```C++
> #include <iostream>
> #include <thread>
> #include <condition_variable>
> #include <mutex>
> #include <future>
> #include <chrono>
> #include <iostream>
> #include <vector>
> 
> struct timer_killer
> {
> 	// returns false if killed:
> 	template<class R, class P>
> 	bool wait_for(std::chrono::duration<R, P> const &time) const
> 	{
> 		std::unique_lock<std::mutex> lock(m);
> 		return !cv.wait_for(lock, time, [&]
> 		{	return terminate;});
> 	}
> 	void kill()
> 	{
> 		std::unique_lock<std::mutex> lock(m);
> 		terminate=true; // should be modified inside mutex lock
> 		cv.notify_all();// it is safe, and *sometimes* optimal, to do this outside the lock
> 	}
> 	// I like to explicitly delete/default special member functions:
> 	timer_killer() = default;
> 	timer_killer(timer_killer&&)=delete;
> 	timer_killer(timer_killer const&)=delete;
> 	timer_killer& operator=(timer_killer&&)=delete;
> 	timer_killer& operator=(timer_killer const&)=delete;
> private:
> 	mutable std::condition_variable cv;
> 	mutable std::mutex m;
> 	bool terminate = false;
> };
> timer_killer bob;
> 
> int main()
> {
> 	std::vector<std::future<void> > tasks;
> 	tasks.push_back(std::async(std::launch::async, []
> 	{
> 		while(bob.wait_for(std::chrono::milliseconds(50)))
> 		{
> 			std::cout << "thread 1 says hi\n";
> 		}
> 		std::cout << "thread 1 dead\n";
> 	}));
> 	bob.wait_for(std::chrono::milliseconds(250));
> 	tasks.push_back(std::async(std::launch::async, []
> 	{
> 		while(bob.wait_for(std::chrono::milliseconds(50)))
> 		{
> 			std::cout << "thread 2 says hi\n";
> 		}
> 		std::cout << "thread 2 dead\n";
> 	}));
> 	bob.wait_for(std::chrono::milliseconds(1000));
> 	std::cout << "killing threads\n";
> 	bob.kill();
> 	for (auto &&f : tasks)
> 		f.wait();
> 	std::cout << "done\n";
> 	// your code goes here
> 	return 0;
> }
> // g++ --std=c++11 test.cpp -lpthread
> ```
>
> 

[live example](http://coliru.stacked-crooked.com/a/66f8b2ee5f9d55e8) (coliru).

You create a `timer_killer` in a shared spot. Client threads can `wait_for( time )`. If it returns false, it means you where killed before your wait was complete.

The controlling thread just calls `kill()` and everyone doing a `wait_for` gets a `false` return.

Note that there is some **contention** (locking of the mutex), so this isn't suitable for infinite threads (but few things are). Consider using a scheduler if you need to have an unbounded number of tasks that are run with arbitrary delays instead of a full thread per delayed repeating task -- each real thread is upwards of a megabyte of system address space used (just for the stack).

> NOTE: 需要理解上述  **contention** 的含义: `std::async(std::launch::async)` 表示每个asynchronous task都有一个对应的thread，因此，在上述程序中，将由多个线程竞争`timer_killer::m`

### [A](https://stackoverflow.com/a/29775578)

There are two traditional ways you could do this.

You could use a **timed wait on a condition variable**, and have the other thread signal your periodic thread to wake up and die when it's time.

Alternately you could `poll` on a pipe with your sleep as a timeout instead of of sleeping. Then you just write a byte to the pipe and the thread wakes up and can exit.

> NOTE: 在Linux APUE上有提出过类似的方法，这种方法简记为: poll-on-a-pipe-with-timeout

## superuser [Can't kill a sleeping process](https://superuser.com/questions/539920/cant-kill-a-sleeping-process)



## stackexchange [Can a signal in one thread, interrupt sleep period of another thread of the same process in linux](https://unix.stackexchange.com/questions/516696/can-a-signal-in-one-thread-interrupt-sleep-period-of-another-thread-of-the-same)



### stackoverflow [How to wake a std::thread while it is sleeping](https://stackoverflow.com/questions/52610776/how-to-wake-a-stdthread-while-it-is-sleeping)

## TODO

### how to interrupt a sleeping thread
https://www.codeproject.com/Questions/619339/How-to-interpt-the-Sleeping-thread-in-cplusplus