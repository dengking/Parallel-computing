

# [c++ Implementing Timed Callback function](https://stackoverflow.com/questions/12904098/c-implementing-timed-callback-function)

I want to implement some system in `c++` so that I can call a function and ask for another function to be called in X milliseconds. Something like this:

```cpp
callfunctiontimed(25, funcName);
```

25 being the amount of milliseconds before the function should be called.

I would like to know if multithreading is required for this and then use some delay function? Other than using function pointer how would a feature like this work?

## [A](https://stackoverflow.com/a/12905380)

For a portable solution, you can use `boost::asio`. Below is a demo I wrote a while ago. You can change

```cpp
t.expires_from_now(boost::posix_time::seconds(1));
```

to suit you need say make function call after 200 milliseonds.

```cpp
t.expires_from_now(boost::posix_time::milliseconds(200)); 
```

Below is a complete working example. It's calling repeatedly but I think it should be easy to call only once by just change a bit.

```cpp
#include <iostream>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost::asio;
using namespace std;

class Deadline 
{
public:
    Deadline(deadline_timer &timer) : t(timer) {
        wait();
    }

    void timeout(const boost::system::error_code &e) {
        if (e)
            return;
        cout << "tick" << endl;
        wait();
    }

    void cancel() {
        t.cancel();
    }


private:
    void wait() {
        t.expires_from_now(boost::posix_time::seconds(1)); //repeat rate here
        t.async_wait(boost::bind(&Deadline::timeout, this, boost::asio::placeholders::error));
    }

    deadline_timer &t;
};


class CancelDeadline {
public:
    CancelDeadline(Deadline &d) :dl(d) { }
    void operator()() {
        string cancel;
        cin >> cancel;
        dl.cancel();
        return;
    }
private:
    Deadline &dl;
};



int main()
{
    io_service io;
    deadline_timer t(io);
    Deadline d(t);
    CancelDeadline cd(d);
    boost::thread thr1(cd);
    io.run();
    return 0;
}



//result:
//it keeps printing tick every second until you enter cancel and enter in the console
tick
tick
tick
```

## [A](https://stackoverflow.com/a/15593342)

Many folks have contributed good answers here on the matter, but I will address the question directly, because I had a similar problem a couple of years ago. I could not use Boost for several reasons--I know Boost has excellent use in a lot of open source software. Moreover, I really wanted to *understand* timers and callbacks specifically as it pertains to Linux based environments. So, I wrote my own.

Fundamentally, I have a `Timer` class and a `TimerCallback` class. A typical callback, implemented as a inherited class of the `TimerCallback` class, will place the operations to be executed upon callback in the `triggered ()` method, implemented specifically for the needs.

Per the usual semantics, a `Timer` object is associated with a **callback object**, which presumably contains all the required information needed for the callback to execute. The timer scheduling is managed by one environment-wide timer minheap which has to be maintained in a separate thread/process. This minheap task does only one thing: it minheapifies the minheap of callback events set in the future. A minheap selects the next event to fire in `O(1)` and can minheapify the remaining in `O(log n)` for `n` timer events. It can also insert a new timer event in `O(log n)` (Read a gentle introduction to heaps [here](http://en.wikipedia.org/wiki/Heap_%28data_structure%29)).

When a timer fires, the minheap scheduler checks if it is a periodic timer, one shot timer or a timer that will execute a specific number of times. Accordingly, the timer object is either removed from the minheap or reinserted back into the minheap with the next execution time. If a timer object is to be removed, then it is removed from the minheap (but the timer object deletion may or may not be left to the task that created it) and the rest of the heap is minheap-ified; i.e., rearranged to satisfy the minheap property.

A working and unit tested implementation is [here](https://github.com/SonnyRajagopalan/TimerAndCallback), and may contain bugs (excerpted from my application), but I thought it may help someone. The implementation is multi-process (`fork()`ed-process) based (and also uses `pthread`s in the main task (process)), and uses POSIX shared memory and POSIX message queues for communication between the processes.

## [A](https://stackoverflow.com/a/12904290)

Do you want it to by **asynchronous** so that the **callback** gets executed when 25 miliseconds are over without blocking the main executing thread? If so, you can execute the **callback** in a separate thread from the timer / timed callback function you implement.

If you do not use multithreading, then your main or the calling function of `callfunctiontimed(25, funcName);` would block while you run a `sleep` / `usleep`. It is your choice now as to what behavior you want to implement.

The real solution would not be as simple as to multithread or not. There are things like, how do you keep different timers/callbacks information considering the function can be called multiple times with different timeouts and functions.

One way to do it, would be like this:

1. Create a sorted list of timers/callbacks, sorted based on the time to expire.

2. Have one **main thread** and one thread which looks at callbacks/timers, call it **timer thread**.

3. When a new callback is added, add it in the sorted list.

4. The **timer thread** could be initialized to wait for the least amount of time in the sorted list, or the head. Re-initialize it as new callbacks get added. There would be some math and conditions to take care of.

5. As the timer thread is done sleeping, it removes and looks at head of the list and executes the function pointer in a new thread. Timer thread is re-initialized with sleep time on the new head of the list.

   ```cpp
   main() {
           //spawn a timer thread with pthread create 
   
       callfunctiontimed(25, test); 
       callfunctiontimed(35, show);
       callfunctiontimed(4,  print);
   }
   
   callfunctionTImed(int time, (func*)function, void*data) //
   {
       //add information to sorted list of timer and callbacks
       //re-initialize sleep_time for timer thread if needed. 
       return. 
   }
   timerThread() {
       while(1){
        sleep(sleep_time);
        //look at head of timer list, remove it, call function in a new thread   
        //adjust sleep time as per new head     
       }
   }
   ```

Hopefully this gives an idea of what I meant, although this is not perfect and has several problems.

# [Timed Callbacks in C++](http://www.drdobbs.com/cpp/timed-callbacks-in-c/184408856)

TODO

