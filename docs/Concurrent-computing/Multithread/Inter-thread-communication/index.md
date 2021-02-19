# Inter-thread communication

标题的含义是: "线程间通信"。这个名称是这样得来的: 

1、在阅读 cppreference [std::condition_variable](https://en.cppreference.com/w/cpp/thread/condition_variable) 时，其中提及了  "**inter-thread communication**"

2、由 [Inter-process communication](https://en.wikipedia.org/wiki/Inter-process_communication) 类推而得出的名称



## 实现方式

1、condition variable

2、promise-future channel

3、Semaphore