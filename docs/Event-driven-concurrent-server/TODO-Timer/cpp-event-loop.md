# [C++ std::thread Event Loop with Message Queue and Timer](https://www.codeproject.com/Articles/1169105/%2FArticles%2F1169105%2FCplusplus-std-thread-Event-Loop-with-Message-Queue)

## Introduction

An event loop, or sometimes called a message loop, is a thread that waits for and dispatches incoming events. The thread blocks waiting for requests to arrive and then dispatches the event to an **event handler function**. A **message queue** is typically used by the loop to hold incoming messages. Each message is sequentially dequeued, decoded, and then an action is performed. Event loops are one way to implement inter-process communication.

All operating systems provide support for multi-threaded applications. Each OS has unique function calls for creating **threads**, **message queues** and **timers**. With the advent of the `C++11` thread support library, it’s now possible to create **portable code** and avoid the OS-specific function calls. This article provides a simple example of how to create a thread event loop, message queue and timer services while only relying upon the `C++` Standard Library. Any `C++11` compiler supporting the thread library should be able to compile the attached source.

## Background

Typically, I need a thread to operate as an event loop. Incoming messages are dequeued by the thread and data is dispatched to an appropriate **function handler** based on a unique message identifier. Timer support capable of invoking a function is handy for low speed polling or to generate a **timeout** if something doesn’t happen in the expected amount of time. Many times, the worker thread is created at startup and isn’t destroyed until the application terminates.

A key requirement for the implementation is that the incoming messages must execute on the same thread instance. Whereas say `std::async `may use a temporary thread from a pool, this class ensures that all incoming messages use the same thread. For instance, a subsystem could be implemented with code that is not thread-safe. A single `WorkerThread ` instance is used to safely dispatch function calls into the subsystem.

At first glance, the `C++` thread support seems to be missing some key features. Yes, `std::thread `is available to spin off(甩掉) a thread but there is no **thread-safe queue** and no **timers** – services that most OS’s provide. I’ll show how to use the `C++` Standard Library to create these “missing” features and provide an event processing loop familiar to many programmers.

## WorkerThread

The `WorkerThread ` class encapsulates all the necessary event loop mechanisms. A simple class interface allows thread creation, posting messages to the event loop, and eventual thread termination. The interface is shown below:

Hide   Shrink ![img](https://www.codeproject.com/images/arrow-up-16.png)   Copy Code

```c++
class WorkerThread
{
public:
    /// Constructor
    WorkerThread(const char* threadName);

    /// Destructor
    ~WorkerThread();

    /// Called once to create the worker thread
    /// @return TRUE if thread is created. FALSE otherwise.
    bool CreateThread();

    /// Called once a program exit to exit the worker thread
    void ExitThread();

    /// Get the ID of this thread instance
    /// @return The worker thread ID
    std::thread::id GetThreadId();

    /// Get the ID of the currently executing thread
    /// @return The current thread ID
    static std::thread::id GetCurrentThreadId();

    /// Add a message to thread queue.
    /// @param[in] data - thread specific information created on the heap using operator new.
    void PostMsg(const UserData* data);

private:
    WorkerThread(const WorkerThread&);
    WorkerThread& operator=(const WorkerThread&);

    /// Entry point for the worker thread
    void Process();

    /// Entry point for timer thread
    void TimerThread();

    std::thread* m_thread;
    std::queue<ThreadMsg*> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::atomic<bool> m_timerExit;
    const char* THREAD_NAME;
};
```

The first thing to notice is that `std::thread `is used to create a main worker thread. The main worker thread function is `Process()`.

Hide   Copy Code

```cpp
bool WorkerThread::CreateThread()
{
    if (!m_thread)
        m_thread = new thread(&WorkerThread::Process, this);
    return true;
}
```

## Event Loop

The `Process() `event loop is shown below. The thread relies upon a `std::queue<ThreadMsg*> `for the message queue. `std::queue `is not thread-safe so all access to the queue must be protected by mutex. A `std::condition_variable `is used to suspend the thread until notified that a new message has been added to the queue.

Hide   Shrink ![img](https://www.codeproject.com/images/arrow-up-16.png)   Copy Code

```cpp
void WorkerThread::Process()
{
    m_timerExit = false;
    std::thread timerThread(&WorkerThread::TimerThread, this);

    while (1)
    {
        ThreadMsg* msg = 0;
        {
            // Wait for a message to be added to the queue
            std::unique_lock<std::mutex> lk(m_mutex);
            while (m_queue.empty())
                m_cv.wait(lk);

            if (m_queue.empty())
                continue;

            msg = m_queue.front();
            m_queue.pop();
        }

        switch (msg->id)
        {
            case MSG_POST_USER_DATA:
            {
                ASSERT_TRUE(msg->msg != NULL);

                // Convert the ThreadMsg void* data back to a UserData*
                const UserData* userData = static_cast<const UserData*>(msg->msg);

                cout << userData->msg.c_str() << " " << userData->year << " on " << THREAD_NAME << endl;

                // Delete dynamic data passed through message queue
                delete userData;
                delete msg;
                break;
            }

            case MSG_TIMER:
                cout << "Timer expired on " << THREAD_NAME << endl;
                delete msg;
                break;

            case MSG_EXIT_THREAD:
            {
                m_timerExit = true;
                timerThread.join();

                delete msg;
                std::unique_lock<std::mutex> lk(m_mutex);
                while (!m_queue.empty())
                {
                    msg = m_queue.front();
                    m_queue.pop();
                    delete msg;
                }

                cout << "Exit thread on " << THREAD_NAME << endl;
                return;
            }

            default:
                ASSERT();
        }
    }
}
```

`PostMsg() `creates a new `ThreadMsg `on the heap, adds the message to the queue, and then notifies the worker thread using a condition variable.

Hide   Copy Code

```
void WorkerThread::PostMsg(const UserData* data)
{
    ASSERT_TRUE(m_thread);

    ThreadMsg* threadMsg = new ThreadMsg(MSG_POST_USER_DATA, data);

    // Add user data msg to queue and notify worker thread
    std::unique_lock<std::mutex> lk(m_mutex);
    m_queue.push(threadMsg);
    m_cv.notify_one();
}
```

The loop will continue to process messages until the `MSG_EXIT_THREAD `is received and the thread exits.

Hide   Copy Code

```
void WorkerThread::ExitThread()
{
    if (!m_thread)
        return;

    // Create a new ThreadMsg
    ThreadMsg* threadMsg = new ThreadMsg(MSG_EXIT_THREAD, 0);

    // Put exit thread message into the queue
    {
        lock_guard<mutex> lock(m_mutex);
        m_queue.push(threadMsg);
        m_cv.notify_one();
    }

    m_thread->join();
    delete m_thread;
    m_thread = 0;
}
```

### Event Loop (Win32)

The code snippet below contrasts the `std::thread `event loop above with a similar Win32 version using the Windows API. Notice `GetMessage() `API is used in lieu of the `std::queue`. Messages are posted to the OS message queue using `PostThreadMessage()`. And finally, `timerSetEvent() `is used to place `WM_USER_TIMER `messages into the queue. All of these services are provided by the OS. The `std::thread WorkerThread `implementation presented here avoids the raw OS calls yet the implementation functionality is the same as the Win32 version while relying only upon only the C++ Standard Library.

Hide   Shrink ![img](https://www.codeproject.com/images/arrow-up-16.png)   Copy Code

```
unsigned long WorkerThread::Process(void* parameter)
{
    MSG msg;
    BOOL bRet;

    // Start periodic timer
    MMRESULT timerId = timeSetEvent(250, 10, &WorkerThread::TimerExpired, 
                       reinterpret_cast<DWORD>(this), TIME_PERIODIC);

    while ((bRet = GetMessage(&msg, NULL, WM_USER_BEGIN, WM_USER_END)) != 0)
    {
        switch (msg.message)
        {
            case WM_DISPATCH_DELEGATE:
            {
                ASSERT_TRUE(msg.wParam != NULL);

                // Convert the ThreadMsg void* data back to a UserData*
                const UserData* userData = static_cast<const UserData*>(msg.wParam);

                cout << userData->msg.c_str() << " " << userData->year << " on " << THREAD_NAME << endl;

                // Delete dynamic data passed through message queue
                delete userData;
                break;
            }

            case WM_USER_TIMER:
                cout << "Timer expired on " << THREAD_NAME << endl;
                break;

            case WM_EXIT_THREAD:
                timeKillEvent(timerId);
                return 0;

            default:
                ASSERT();
        }
    }
    return 0;
}
```

## Timer

A low-resolution periodic timer message is inserted into the queue using a secondary private thread. The timer thread is created inside `Process()`.

Hide   Copy Code

```
void WorkerThread::Process()
{
    m_timerExit = false;
    std::thread timerThread(&WorkerThread::TimerThread, this);

...
```

The timer thread’s sole responsibility is to insert a `MSG_TIMER `message every 250ms. In this implementation, there’s no protection against the timer thread injecting more than one timer message into the queue. This could happen if the worker thread falls behind and can’t service the message queue fast enough. Depending on the worker thread, processing load, and how fast the timer messages are inserted, additional logic could be employed to prevent flooding the queue.

Hide   Copy Code

```
void WorkerThread::TimerThread()
{
    while (!m_timerExit)
    {
        // Sleep for 250ms then put a MSG_TIMER message into queue
        std::this_thread::sleep_for(250ms);

        ThreadMsg* threadMsg = new ThreadMsg(MSG_TIMER, 0);

        // Add timer msg to queue and notify worker thread
        std::unique_lock<std::mutex> lk(m_mutex);
        m_queue.push(threadMsg);
        m_cv.notify_one();
    }
}
```

## Usage

The `main()` function below shows how to use the `WorkerThread `class. Two worker threads are created and a message is posted to each one. After a short delay, both threads exit.

Hide   Shrink ![img](https://www.codeproject.com/images/arrow-up-16.png)   Copy Code

```cpp
// Worker thread instances
WorkerThread workerThread1("WorkerThread1");
WorkerThread workerThread2("WorkerThread2");

int main(void)
{  
    // Create worker threads
    workerThread1.CreateThread();
    workerThread2.CreateThread();

    // Create message to send to worker thread 1
    UserData* userData1 = new UserData();
    userData1->msg = "Hello world";
    userData1->year = 2017;

    // Post the message to worker thread 1
    workerThread1.PostMsg(userData1);

    // Create message to send to worker thread 2
    UserData* userData2 = new UserData();
    userData2->msg = "Goodbye world";
    userData2->year = 2017;

    // Post the message to worker thread 2
    workerThread2.PostMsg(userData2);

    // Give time for messages processing on worker threads
    this_thread::sleep_for(1s);

    workerThread1.ExitThread();
    workerThread2.ExitThread();

    return 0;
}
```

## Conclusion

The C++ thread support library offers a platform independent way to write multi-threaded application code without reliance upon OS-specific API’s. The `WorkerThread `class presented here is a bare-bones implementation of an event loop, yet all the basics are there ready to be expanded upon.

