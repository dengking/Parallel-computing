[TOC]

# [How to call a function every x seconds [closed\]](https://stackoverflow.com/questions/15683221/how-to-call-a-function-every-x-seconds)

**comments**:

for asking whether he needs polling approach or asynchronous timer. – [Ahmed Saleh](https://stackoverflow.com/users/375666/ahmed-saleh) 

## [A](https://stackoverflow.com/a/15683290)

There are two approaches: 1.Asynchronous 2. Synchronous

1. Assuming that you are using Win32, `C++`. You can use Win32 API [SetTimer](http://msdn.microsoft.com/en-us/library/ms644906%28VS.85%29.aspx)

   UINT_PTR timerid = SetTimer(NULL, 0, milliseconds, &callback);

2. If you would like a **Polling** Approach You would better use something like that

   ```cpp
     for(;;) 
       {
   
       Say_Hello();
       // Sleep for 50*1000ms
       Sleep(50000);
   
       }
   ```

## [A](https://stackoverflow.com/a/15683469)

The [Boost](http://boost.org/) library provides for this in [Boost.Asio](http://www.boost.org/doc/libs/release/libs/asio/), and explicitly covers this in its tutorials:

- [Synchronous timer](http://www.boost.org/doc/libs/1_53_0/doc/html/boost_asio/tutorial/tuttimer1.html), i.e. waiting until the timer expires.
- [Asynchronous timer](http://www.boost.org/doc/libs/1_53_0/doc/html/boost_asio/tutorial/tuttimer2.html), i.e. continuing with your program and having the callback function invoked when the timer expires.

If you didn't find the Boost library when searching the web for C++, your google-fu is weak. ;-)



# [[c++\] I want a timer to trigger a function every two seconds without interfering with the rest of the program](https://www.reddit.com/r/learnprogramming/comments/20mqoz/c_i_want_a_timer_to_trigger_a_function_every_two/) 

That code will waste an awful lot of processor cycles constantly checking the time as frequently as it possibly can.

Adding a simple sleep call to the above loop would make it much more processor friendly, but I probably wouldn't do it that way at all. I've used `boost::asio` with success before. It provides a nice portable solution, though some people are opposed to using boost. The [deadline_timer](http://www.boost.org/doc/libs/1_45_0/doc/html/boost_asio/reference/deadline_timer.html) is useful for this kind of thing:
```c++
boost::asio::deadline_timer timer(io_service);

// asynchronously calls handler after 2 seoncds
timer.expires_from_now(boost::posix_time::seconds(2))
timer.async_wait(handler);
```





# [Creating a ticker thread](https://codereview.stackexchange.com/questions/125579/creating-a-ticker-thread)

*A new question about a revised version of the code in this question, based on the advice in the accepted answer can be found here.*

I need to **signal** my program to do something every X seconds. So I decided to make a separate thread that sends such a signal using [ZeroMQ](http://zeromq.org/). All other behaviour is also triggered by ZeroMQ messages, so this seems like a safe way to do it, with respect to multithreading.

I have never used threads in C++ before though, so I'd like some feedback on my code. Note that in the code below I have replaced the actual task with a simple `cout` statement.

**EDIT:** I just realized the thread could very well terminate between setting `run = false` and calling `join`, which causes a `std::system_error` to be thrown. Putting the join statement in an `if`-block with `isjoinable` still suffers from the same problem. How should I handle this? try-catch? Using a locking mechanism instead of an atomic bool?

**Ticker.hpp**

```cpp
#include <thread>
#include <chrono>
#include <atomic>

class Ticker {
private:
    const unsigned int interval;
    std::atomic_bool run;
    std::thread tickthread;
public:
    Ticker(unsigned int interval);
    Ticker(const Ticker& orig) = delete;
    virtual ~Ticker();
    void stop();
private:
    void tickfunction();
};
```

**Ticker.cpp**

```cpp
#include "Ticker.hpp"
#include <iostream>

using namespace std;

Ticker::Ticker(unsigned int interval)
: interval(interval), run(true), tickthread(&Ticker::tickfunction, this) {
}

Ticker::~Ticker() {
    if (tickthread.joinable()) {
        stop();
    }
}

void Ticker::stop() {
    cout << "stopping..." << endl;
    run = false;
    // interrupt sleep?
    tickthread.join();
    cout << "joined" << endl;
}

void Ticker::tickfunction() {
    while (true) {
        this_thread::sleep_for(chrono::seconds(interval));
        if (run) {
            cout << "tick" << endl;
        } else {
            break;
        }
    }
}
```

There are also two specific things I'd like recommendations on:

1. How can I change this class to accept any std::chrono::duration object as input for the interval, instead of just an integer? I tried this, but couldn't figure out how to do it with the templates.
2. Is there a way to interrupt the sleep_for when I call `stop()`? Especially when `interval` is large, it'd be nice to not have to wait for the sleeping thread when stopping it.
















# [Thread-Pool-Timer](https://github.com/bradley219/Thread-Pool-Timer)

TODO



# [C++ 11: Calling a C++ function periodically](https://stackoverflow.com/questions/30425772/c-11-calling-a-c-function-periodically)

***SUMMARY*** : 这篇文章的代码虽然不符合我的要求，但是却对于学习`c++  thread`有参考价值。

TODO



# [Linux, timerfd accuracy](https://stackoverflow.com/questions/3124852/linux-timerfd-accuracy)

TODO

