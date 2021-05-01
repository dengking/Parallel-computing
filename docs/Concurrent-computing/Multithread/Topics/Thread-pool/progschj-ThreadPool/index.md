# [progschj](https://github.com/progschj)/**[ThreadPool](https://github.com/progschj/ThreadPool)**





## Read code

1、代码并不长，因此可以很快完全阅读。

2、典型的"asynchronous method invocation-thread pool-message queue mailbox"





```C++
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class ThreadPool {
public:
    ThreadPool(size_t);
    /**
	 * @brief
     *
     * @tparam F
     * @tparam Args
     * @param f
     * @param args
     * @return
     */
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>;
    ~ThreadPool();
private:
    // need to keep track of threads so we can join them
    std::vector< std::thread > workers;
    // the task queue
    std::queue< std::function<void()> > tasks;

    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    /**
     * 是否停止thread pool，在destructor将其设置为true
	 */
    bool stop;
};

// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t threads)
    :   stop(false)
{
    for(size_t i = 0;i<threads;++i)
        workers.emplace_back(
            [this]
            {
                for(;;)
                {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        /**
                                                              * 等待队列非空或者用户停止了thread pool
                         */
                        this->condition.wait(lock,
                            [this]{ return this->stop || !this->tasks.empty(); });
                        /**
                                                               * 用户通知了thread pool
                         */
                        if(this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }

                    task();
                }
            }
        );
}

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // don't allow enqueueing after stopping the pool
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");
        /**
         * "[task](){ (*task)(); }"是lambda expression
         */
        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_one();
    return res;
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &worker: workers)
        worker.join();
}

#endif

```



### 优化

在 `ThreadPool::enqueue(F&& f, Args&&... args)` 中，使用 `task = std::make_shared< std::packaged_task<return_type()>` 即new 来构造task对象，显然它会频繁地进行new、delete；这造成如下问题:

1、内存碎片

2、性能

那如何来进行优化呢？

1、automatic variable + move