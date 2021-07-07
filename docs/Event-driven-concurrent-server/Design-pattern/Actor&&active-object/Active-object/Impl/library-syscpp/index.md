# [lightful](https://github.com/lightful)/**[syscpp](https://github.com/lightful/syscpp)**

## ActorThread: Active Object pattern in C++

Implementation of the [Active Object pattern](http://www.drdobbs.com/parallel/prefer-using-active-objects-instead-of-n/225700095) wrapping a standard C++11 thread.

### Simple

1、Whole implementation contained in **a single header file!**



2、Inherit from a template and you are done. See the tiny example bellow.

> NOTE: 
>
> 1、CRTP mixin from above

### Performance

1、Internal lock-free MPSC messages queue

> NOTE: 需要考虑是如何实现的

2、Extensive internal use of move semantics supporting delivery of non-copiable objects

> NOTE:
> 1、prefer move to copy，`std::vector`也是这样做的，参见 
>
> a、[如何评价 C++11 的右值引用（Rvalue reference）特性？ - Tinro的回答 - 知乎](https://www.zhihu.com/question/22111546/answer/30801982) 

3、Several million msg/sec between each two threads (both Linux and Windows) in ordinary hardware

### Robustness

1、The wrapped thread lifecycle overlaps and is driven by the object existence

2、The object is kept alive by smart pointers (whoever has a reference can safely send messages)

3、No internal strong references (only the final users determine the destruction/end)

4、Nonetheless, callbacks onto already deleted active objects do not crash the application

> NOTE: 
>
> 1、阅读了上述内容，发现它的实现其实和spdlog是非常类似的:
>
> a、spdlog也是使用的active object pattern
>
> b、spdlog中也使用了`std::shared_ptr`



### Example

```C++
// Linux:    g++ -std=c++11 -lpthread demo.cpp -o demo
// Windows:  cl.exe demo.cpp

#include <string>
#include <iostream>
#include <sstream>
#include "ActorThread.hpp"

struct Message { std::string description; };
struct OtherMessage { double beautifulness; };

class Consumer : public ActorThread<Consumer>
{
    friend ActorThread<Consumer>;
    void onMessage(Message& p)
    {
        std::cout << "thread " << std::this_thread::get_id()
                  << " receiving " << p.description << std::endl;
    }
    void onMessage(OtherMessage& p)
    {
        std::cout << "thread " << std::this_thread::get_id()
                  << " receiving " << p.beautifulness << std::endl;
    }
};

class Producer : public ActorThread<Producer>
{
    friend ActorThread<Producer>;
    std::shared_ptr<Consumer> consumer;
    void onMessage(std::shared_ptr<Consumer>& c)
    {
        consumer = c;
        timerStart(true, std::chrono::milliseconds(250), TimerCycle::Periodic);
        timerStart(3.14, std::chrono::milliseconds(333), TimerCycle::Periodic);
    }
    void onTimer(const bool&)
    {
        std::ostringstream report;
        report << "test from thread " << std::this_thread::get_id();
        consumer->send(Message { report.str() });
    }
    void onTimer(const double& value)
    {
        consumer->send(OtherMessage { value });
    }
};

class Application : public ActorThread<Application>
{
    friend ActorThread<Application>;
    void onStart()
    {
        auto consumer = Consumer::create(); // spawn new threads
        auto producer = Producer::create();
        producer->send(consumer);
        std::this_thread::sleep_for(std::chrono::seconds(3));
        stop();
    }
};

int main()
{
    return Application::run(); // re-use existing thread
}
```

> NOTE: 
>
> 1、从上述code来看，它是非常简洁的
>
> 2、它使用了CRTP、mixin-from-above、friend base class
>
> 3、上述`onMessage`，让我想到了: visitor message pattern