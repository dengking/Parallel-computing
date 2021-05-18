# how to stop a blocked thread

今天在使用[redis-plus-plus](https://github.com/sewenew/redis-plus-plus)的[`pub/sub`](https://github.com/sewenew/redis-plus-plus#publishsubscribe)功能的时候，其文档中有这样的描述：
> `Subscriber::consume` waits for message from the underlying connection. If the `ConnectionOptions::socket_timeout` is reached, and there's no message sent to this connection, `Subscriber::consume` throws a `TimeoutError` exception. If `ConnectionOptions::socket_timeout` is `0ms`, `Subscriber::consume` blocks until it receives a message.

在我们的application中，我使用一个thread来执行`Subscriber::consume`，这意味中在没有message的时候我的thread将block；那对于一个blocked的thread，是否会存在和sleeping thread一样的stop的问题呢？

## stackoverflow [Terminate thread c++11 blocked on read](https://stackoverflow.com/questions/51742179/terminate-thread-c11-blocked-on-read)

I've got the following code:

```cpp
class Foo {
private:
    std::thread thread;
    void run();
    std::atomic_flag running;
    std::thread::native_handle_type native;
public:
    Foo(const std::string& filename);
    virtual ~Foo();
    virtual void doOnChange();
    void start();
    void quit();
};

#include "Foo.h"
#include <functional>

#include <iostream>

Foo::Foo(const std::string& filename) :
        thread(), running(ATOMIC_FLAG_INIT) {
    file = filename;
    native = 0;
}

Foo::~Foo() {
    quit();
}

void Foo::start() {
    running.test_and_set();
    try {
        thread = std::thread(&Foo::run, this);
    } catch (...) {
        running.clear();
        throw;
    }
    native = thread.native_handle();
}

void Foo::quit() {
    running.clear();
    pthread_cancel(native);
    pthread_join(native, nullptr);
    //c++11-style not working here
    /*if (thread.joinable()) {
        thread.join();
        thread.detach();
    }*/
}

void Foo::run() {
   while (running.test_and_set()) {
        numRead = read(fd, buf, BUF_LEN);
        .....bla bla bla.......
   }
}
```

I'm trying to quit from this thread in my program cleanup code. Using pthread works but I'm wondering if I can do something better with `c++11` only (no native handle). It seems to me there's no good way to handle all cases using `c++11` code. As you can see here the thread is **blocked** on a read system call. So even if I clear the flag the thread will be still blocked and join call will block forever. So what I really need is an **interrupt** (in this case `pthread_cancel`). But if I call `pthread_cancel` I can't call anymore the `c++11` `join()` method because it fails, I can only call `pthread_join()`. So it seems the standard has a really big limitation, am I miss anything?

> NOTE: `read`是阻塞IO，可能永远地将thread阻塞

Edit:

After discussion below I changed the `Foo` class implementation replacing `std::atomic_flag` with `std::atomic` and using **signal handler**. I used the **signal handler** because in my opinion is better to have a general base class, using the **self-pipe trick** is too hard in a base class, the logic should be delegated to the child. Final implementation:

```cpp
#include <thread>
#include <atomic>

class Foo {
private:
    std::thread thread;
    void mainFoo();
    std::atomic<bool> running;
    std::string name;
    std::thread::native_handle_type native;
    static void signalHandler(int signal);
    void run();
public:
    Thread(const std::string& name);
    virtual ~Thread();
    void start();
    void quit();
    void interrupt();
    void join();
    void detach();
    const std::string& getName() const;
    bool isRunning() const;
};
```

Cpp file:

```cpp
#include <functional>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <Foo.h>
#include <csignal>
#include <iostream>

Foo::Foo(const std::string& name) :
        name(name) {
    running = false;
    native = 0;
    this->name.resize(16, '\0');
}

Foo::~Foo() {
}

void Foo::start() {
    running = true;
    try {
        thread = std::thread(&Foo::mainFoo, this);
    } catch (...) {
        running = false;
        throw;
    }
    native = thread.native_handle();
    pthread_setname_np(native, name.c_str());
}

void Foo::quit() {
    if (running) {
        running = false;
        pthread_kill(native, SIGINT);//向自己发送信号
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void Foo::mainFoo() {
 //enforce POSIX semantics
 siginterrupt(SIGINT, true);
 std::signal(SIGINT, signalHandler);
    run();
    running = false;
}

void Foo::join() {
    if (thread.joinable())
        thread.join();
}

void Foo::signalHandler(int signal) {
}

void Foo::interrupt() {
    pthread_kill(native, SIGINT);
}

void Foo::detach() {
    if (thread.joinable())
        thread.detach();
}

const std::string& Foo::getName() const {
    return name;
}

bool Foo::isRunning() const {
    return running;
}

void Foo::run() {
    while(isRunning()) {
         num = read(.....);
         //if read is interrupted loop again, this time
         //isRunning() will return false
    }
}
```

***COMMENTS*** :

***SUMMARY*** : 根据下面的讨论，作者的这个问题可以总结为：**non-cooperatively kill a single thread** in a multi-thread program 和  **non-cooperative thread cancellation** 

Forcibly "killing" a thread is never a good idea, as the thread will not be able to release any possible resources it might have allocated. If you need to be able to *ask* a thread to exit before it's end, then consider using e.g. **non-blocking I/O** or similar. – [Some programmer dude](https://stackoverflow.com/users/440558/some-programmer-dude) [Aug 8 '18 at 8:40](https://stackoverflow.com/questions/51742179/terminate-thread-c11-blocked-on-read#comment90444376_51742179)

There are some platforms (MS Windows) where terminating a thread will leave your application in an **unstable state**. This is documented by MS. Simple example: thread is holding the (internal) `C++` heap lock when it gets terminated - now you have no heap. – [Richard Critten](https://stackoverflow.com/users/3370124/richard-critten) [Aug 8 '18 at 8:41](https://stackoverflow.com/questions/51742179/terminate-thread-c11-blocked-on-read#comment90444417_51742179) 

@Some programmer dude It's actually possible to call a **cleanup handler** using pthread, not in my example but possible – greywolf82 Aug 8 '18 at 8:41 

It's still not enough. Lets take this hypothetical case: You allocate objects in a loop in the thread with `new`. The allocation have succeeded, but the assignment to the pointer haven't been done yet. Then the thread is killed, and you leak the object (not to mention the possible **UB**(undefined behavior) if the pointer is not initialized). Unless you can synchronize the thread and the killing of it there will always be chances of leaks and UB. And if you can synchronize the killing of the thread, why forcibly kill it in the first place instead of letting it clean up itself? – [Some programmer dude](https://stackoverflow.com/users/440558/some-programmer-dude) [Aug 8 '18 at 8:51](https://stackoverflow.com/questions/51742179/terminate-thread-c11-blocked-on-read#comment90444827_51742179) 

***SUMMARY*** : 这段中所列举的例子非常好，它否定了使用cleanup handler和kill thread

So the reply to my question is: using `c++11` (maybe even without it) don't call any **blocking system call** in the thread. It seems a big limitation to me. – [greywolf82](https://stackoverflow.com/users/2944616/greywolf82) [Aug 8 '18 at 8:54](https://stackoverflow.com/questions/51742179/terminate-thread-c11-blocked-on-read#comment90444944_51742179)

May be helpful: [stackoverflow.com/a/12207835/5376789](https://stackoverflow.com/a/12207835/5376789) – [xskxzr](https://stackoverflow.com/users/5376789/xskxzr) [Aug 8 '18 at 8:58](https://stackoverflow.com/questions/51742179/terminate-thread-c11-blocked-on-read#comment90445064_51742179)

@xskxzr I know there is no portable way in `C++11` to **non-cooperatively kill a single thread** in a multi-thread program, but I'm wondering how to handle a case where the thread is blocked in IO and the response seems to be: *never use blocking io in a thread*. – [greywolf82](https://stackoverflow.com/users/2944616/greywolf82) [Aug 8 '18 at 9:01](https://stackoverflow.com/questions/51742179/terminate-thread-c11-blocked-on-read#comment90445171_51742179)

Don't **block**. Look into "readsome", "in_avail", and related functions. – [Jive Dadson](https://stackoverflow.com/users/445296/jive-dadson) [Aug 8 '18 at 9:03](https://stackoverflow.com/questions/51742179/terminate-thread-c11-blocked-on-read#comment90445268_51742179)

***SUMMARY*** :  [readsome](https://en.cppreference.com/w/cpp/io/basic_istream/readsome)  [in_avail](https://en.cppreference.com/w/cpp/io/basic_streambuf/in_avail)

@Someprogrammerdude Just to add info here: if you use a **deferred cancellation point**, the thread will be canceled when calling the next **cancellation point**, not immediately. So in your example and with this configuration no leak can happen because the thread won't be stopped during allocation but only to the next cancellation point. – [greywolf82](https://stackoverflow.com/users/2944616/greywolf82) [Aug 8 '18 at 9:06](https://stackoverflow.com/questions/51742179/terminate-thread-c11-blocked-on-read#comment90445388_51742179) 

***SUMMARY*** : pthread_cancel也是一种解法

Yup, **non-blocking read**. Its the only clean solution imo. – [Galik](https://stackoverflow.com/users/3807729/galik) [Aug 8 '18 at 9:21](https://stackoverflow.com/questions/51742179/terminate-thread-c11-blocked-on-read#comment90445985_51742179) 

Just a side note: The use of `ATOMIC_FLAG_INIT` in a (default) member initializer seems to be unspecified since C++14: [stackoverflow.com/questions/24437396/…](https://stackoverflow.com/questions/24437396/stdatomic-flag-as-member-variable) . My suggestion: Use `std::atomic<bool>`. – [Julius](https://stackoverflow.com/users/2615118/julius)[Aug 8 '18 at 9:37](https://stackoverflow.com/questions/51742179/terminate-thread-c11-blocked-on-read#comment90446647_51742179)

@greywolf82 to add to the reasons to avoid **non-cooperative thread cancellation** in C++: on GNU/Linux `pthread_cancel` is implemented with a magic unstoppable exception, so if any function in the thread's call stack is `noexcept` then `pthread_cancel` will terminate the whole process, and you can't prevent it. – [Jonathan Wakely](https://stackoverflow.com/users/981959/jonathan-wakely) [Aug 8 '18 at 10:47](https://stackoverflow.com/questions/51742179/terminate-thread-c11-blocked-on-read#comment90449169_51742179) 

If on Linux or POSIX learn about [poll(2)](http://man7.org/linux/man-pages/man2/poll.2.html) (you'll call it before the blocking read) – [Basile Starynkevitch](https://stackoverflow.com/users/841108/basile-starynkevitch) [Aug 9 '18 at 10:26](https://stackoverflow.com/questions/51742179/terminate-thread-c11-blocked-on-read#comment90487790_51742179)

That's right, don't use any indefinitely(不确定的，无限的) blocking system calls, with threads or without. – [n.m.](https://stackoverflow.com/users/775806/n-m) [Aug 9 '18 at 10:33](https://stackoverflow.com/questions/51742179/terminate-thread-c11-blocked-on-read#comment90487998_51742179)

## system call with max blocking time

今天在阅读《redis设计与实现》这本书的第12.3节《事件的调度与执行》的时候，作者所给出的伪代码让我突然意识到max blocking time参数的重要价值，在youdao 《`redis设计与实现-第12章-事件.md`》中对这个进行了总结，我觉得有必要将其copy过来：
```
def aeProcessEvents:
	# 获得到达时间离当前时间最接近的时间事件
	time_event = aeSearchNearestTimer()
	# 计算最接近的时间事件距离到达还有多少毫秒
	remained_ms = time_event.when - unix_ts_now()
	# 如果时间已经到达，那么remained_ms则可能为负数，将它设置为0
	
	if remained_ms < 0:
		remained_ms = 0
		
	# 根据remained_ms的值，创建timeval结构
	timeval = create_timeval_with_ms(remained_ms)
	
	# 阻塞并等待文件事件产生，最大阻塞时间由传入的timeval结构决定
	# 如果remained_ms的值为0，那么aeApiPoll调用之后马上返回，不阻塞
	aeApiPoll(timeval)
	
	# 处理所有已经产生的文件事件
	processFileEvents()
	
	# 处理所有已经到达的时间事件
	processTimeEvents()
```

Unix所提供的很多system call，如果涉及到阻塞执行的线程，很多都会提供一个`timeout`来作为参数，表示最大阻塞时间；现在想来这个`timeout`是非常重要的，如果不提供这个`timeout`参数，这些system call就存在将执行它的thread永远地阻塞下去的可能性，一旦出现了这种情况，则是比较不好搞的，这个问题在youdao的《`Unix-thread-blocked.md`》进行了总结；现在想来，当我不知道这个参数的用途的时候，往往容易忽视这个参数，其实这些参数的价值是非常大的；

还有，提供一个`timeout`参数，其实能够达到一箭双雕的效果，第一雕已经在上一段中描述了，即防止永远地阻塞thread，第二雕则是提供了定时的功能，尤其到我们的程序中涉及到timer的时候，这个功能的价值是比较高的；这让我想起了使用c++的thread library的[`wait_until`](https://en.cppreference.com/w/cpp/thread/condition_variable/wait_until)来避免`sleep`带来的可能导致无法将thead随时唤醒的问题；

## system call with max blocking time

<http://www.cs.um.edu.mt/~ssrg/AThesis.pdf>



## self-pipe



## Interrupted System Calls

使用signal了来interrupte System Calls，如果process正blocked 在一个low system call，这种方法是比较好的；上面所采用的就是这种方法；

celery停止的时候也是采用的这种方法，celery所发送的是term；