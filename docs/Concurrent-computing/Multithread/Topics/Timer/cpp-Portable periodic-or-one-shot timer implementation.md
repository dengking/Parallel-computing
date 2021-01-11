

# [Portable periodic/one-shot timer implementation](https://codereview.stackexchange.com/questions/40473/portable-periodic-one-shot-timer-implementation)

*Note: I have posted a follow-up question for a significantly updated version of this code.*

I have implemented a class that provides portable one-shot or periodic timers.

The API provides a way to schedule one or more **timer callbacks** to fire some number of **milliseconds** in the future, and optionally fire again every so many milliseconds.

The API returns an ID which can be used later to see if the timer still exists, or destroy it.

The implementation assigns increasing IDs to the timers, starting at one. The timer IDs use a 64-bit unsigned integer, to avoid dealing with **wraparound**.

***THINKING*** : 上述wraparound该如何来进行理解？

The implementation stores the context of each `timer Instance` in an `unordered_map` called `active`, keyed by **ID**. Each **instance** has a `next` member, which is a `time_point` that indicates when it needs to fire next.

The implementation uses a `multiset` called `queue` to sort the timers by the `next` member, using a functor.

The queue multiset stores `reference_wrapper` objects that refer directly to the `Instance` objects. This allows the queue's comparator to refer directly to the instances.

A **worker thread** is created to service the timer queue. A `mutex` and `condition_variable` are used for synchronization. The **condition variable** is used to notify the worker thread when a timer is created or destroyed, and to request worker thread shutdown.

The worker thread uses `wait` to wait for notification when there are no timers, and uses `wait_for` to wait until earliest notification needs to fire, or until awakened.

The lock is released during the callback when a timer fires.

The destructor sets the `done` flag to true, notifies the condition variable, releases the lock, then joins with the worker to wait for it to exit.

**EDIT**: I realized that there was a **race condition** that occurs if a timer is destroyed while its callback is in progress. I solved that by having a `running` flag on each instance. The worker thread checks it when a callback returns to see if that `Instance` needs to be destroyed. This avoids dereferencing an `Instance` that was destroyed while the lock was not held during the callback. The `destroy` method was also updated to see if the callback is running, and set `running` to false if so, to indicate to the worker that it needs to be destroyed. If the callback for that timer is not running, `destroy` destroys the `Instance` itself.

**timer.h**

```cpp
#ifndef TIMER_H
#define TIMER_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <functional>
#include <chrono>
#include <unordered_map>
#include <set>
#include <cstdint>

class Timer
{
public:
    typedef uint64_t timer_id;
    typedef std::function<void()> handler_type;

private:
    std::mutex sync;
    typedef std::unique_lock<std::mutex> ScopedLock;

    std::condition_variable wakeUp;

private:
    typedef std::chrono::steady_clock Clock;
    typedef std::chrono::time_point<Clock> Timestamp;
    typedef std::chrono::milliseconds Duration;

    struct Instance
    {
        Instance(timer_id id = 0)
            : id(id)
            , running(false)
        {
        }

        template<typename Tfunction>
        Instance(timer_id id, Timestamp next, Duration period, Tfunction&& handler) noexcept
            : id(id)
            , next(next)
            , period(period)
            , handler(std::forward<Tfunction>(handler))
            , running(false)
        {
        }

        Instance(Instance const& r) = delete;

        Instance(Instance&& r) noexcept
            : id(r.id)
            , next(r.next)
            , period(r.period)
            , handler(std::move(r.handler))
            , running(r.running)
        {
        }

        Instance& operator=(Instance const& r) = delete;

        Instance& operator=(Instance&& r)
        {
            if (this != &r)
            {
                id = r.id;
                next = r.next;
                period = r.period;
                handler = std::move(r.handler);
                running = r.running;
            }
            return *this;
        }

        timer_id id;
        Timestamp next;
        Duration period;
        handler_type handler;
        bool running;
    };

    typedef std::unordered_map<timer_id, Instance> InstanceMap;
    timer_id nextId;
    InstanceMap active;

    // Comparison functor to sort the timer "queue" by Instance::next
    struct NextActiveComparator
    {
        bool operator()(const Instance &a, const Instance &b) const
        {
            return a.next < b.next;
        }
    };
    NextActiveComparator comparator;

    // Queue is a set of references to Instance objects, sorted by next
    typedef std::reference_wrapper<Instance> QueueValue;
    typedef std::multiset<QueueValue, NextActiveComparator> Queue;
    Queue queue;

    // Thread and exit flag
    std::thread worker;
    bool done;
    void threadStart();

public:
    Timer();
    ~Timer();

    timer_id create(uint64_t when, uint64_t period, const handler_type& handler);
    timer_id create(uint64_t when, uint64_t period, handler_type&& handler);

private:
    timer_id createImpl(Instance&& item);

public:
    bool destroy(timer_id id);

    bool exists(timer_id id);
};

#endif // TIMER_H
```

**timer.cpp**

```cpp
#include "timer.h"

void Timer::threadStart()
{
    ScopedLock lock(sync);

    while (!done)
    {
        if (queue.empty())
        {
            // Wait (forever) for work
            wakeUp.wait(lock);
        }
        else
        {
            auto firstInstance = queue.begin();
            Instance& instance = *firstInstance;
            auto now = Clock::now();
            if (now >= instance.next)
            {
                queue.erase(firstInstance);

                // Mark it as running to handle racing destroy
                instance.running = true;

                // Call the handler
                lock.unlock();
                instance.handler();
                lock.lock();

                if (done)
                {
                    break;
                }
                else if (!instance.running)
                {
                    // Running was set to false, destroy was called
                    // for this Instance while the callback was in progress
                    // (this thread was not holding the lock during the callback)
                    active.erase(instance.id);
                }
                else
                {
                    instance.running = false;

                    // If it is periodic, schedule a new one
                    if (instance.period.count() > 0)
                    {
                        instance.next = instance.next + instance.period;
                        queue.insert(instance);
                    } else {
                        active.erase(instance.id);
                    }
                }
            } else {
                // Wait until the timer is ready or a timer creation notifies
                wakeUp.wait_until(lock, instance.next);
            }
        }
    }
}

Timer::Timer()
    : nextId(1)
    , queue(comparator)
    , done(false)
{
    ScopedLock lock(sync);
    worker = std::thread(std::bind(&Timer::threadStart, this));
}

Timer::~Timer()
{
    ScopedLock lock(sync);
    done = true;
    wakeUp.notify_all();
    lock.unlock();
    worker.join();
}

Timer::timer_id Timer::create(uint64_t msFromNow, uint64_t msPeriod,
        const std::function<void()> &handler)
{
    return createImpl(Instance(0,
            Clock::now() + Duration(msFromNow), Duration(msPeriod),
            handler));
}

Timer::timer_id Timer::create(uint64_t msFromNow, uint64_t msPeriod,
        std::function<void()>&& handler)
{
    return createImpl(Instance(0,
            Clock::now() + Duration(msFromNow), Duration(msPeriod),
            std::move(handler)));
}

Timer::timer_id Timer::createImpl(Instance&& item)
{
    ScopedLock lock(sync);
    item.id = nextId++;
    auto iter = active.emplace(item.id, std::move(item));
    queue.insert(iter.first->second);
    wakeUp.notify_all();
    return item.id;
}

bool Timer::destroy(timer_id id)
{
    ScopedLock lock(sync);
    auto i = active.find(id);
    if (i == active.end())
        return false;
    else if (i->second.running)
    {
        // A callback is in progress for this Instance,
        // so flag it for deletion in the worker
        i->second.running = false;
    }
    else
    {
        queue.erase(std::ref(i->second));
        active.erase(i);
    }

    wakeUp.notify_all();
    return true;
}

bool Timer::exists(timer_id id)
{
    ScopedLock lock(sync);
    return active.find(id) != active.end();
}
```

**Example:**

```cpp
#include "timer.h"
int main()
{
    Timer t;
    // Timer fires once, one second from now
    t.create(1000, 0,
             []() {
                 std::cout << "Non-periodic timer fired" << std::endl;
             });
    // Timer fires every second, starting five seconds from now
    t.create(5000, 1000,
             []() {
                 std::cout << "Timer fired 0" << std::endl;
             });
    // Timer fires every second, starting now
    t.create(0, 1000,
             []() {
                 std::cout << "Timer fired 1" << std::endl;
             });
    // Timer fires every 100ms, starting now
    t.create(0, 100,
             []() {
                 std::cout << "Timer fired 2" << std::endl;
             });
}
```

The third parameter is a std::function, so it could call a method on some instance of an object, like this:

```cpp
class Foo
{
public:
    void bar() { std::cout << "Foo::bar called" << std::endl; }
};

int something()
{
    Foo example;

    // Assume "t" is a Timer
    auto tid = t.create(0, 100, std::bind(&Foo::bar, &example));
    // ... do stuff ...
```

The `exists` method is intended for scenarios where you desperately need to wait for a timer to go away before something goes out of scope. Definitely not pretty but I don't expect this scenario to occur a lot in my intended use cases:

```cpp
    // Not pretty, but better than nothing:
    t.destroy(tid);
    while (t.exists(tid))
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
```