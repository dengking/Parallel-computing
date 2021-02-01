# [preshing](https://github.com/preshing)/**[cpp11-on-multicore](https://github.com/preshing/cpp11-on-multicore)** **inmemorylogger**

## Source code

[cpp11-on-multicore](https://github.com/preshing/cpp11-on-multicore)/[common](https://github.com/preshing/cpp11-on-multicore/tree/master/common)/[**inmemorylogger.h**](https://github.com/preshing/cpp11-on-multicore/blob/master/common/inmemorylogger.h)

[cpp11-on-multicore](https://github.com/preshing/cpp11-on-multicore)/[common](https://github.com/preshing/cpp11-on-multicore/tree/master/common)/[**inmemorylogger.cpp**](https://github.com/preshing/cpp11-on-multicore/blob/master/common/inmemorylogger.cpp)



## Double checked locking

在使用double checked locking来实现singleton时，只有当`if (pInstance == 0)// 1st test`满足的时候，才会进行lock，由于TOCTTOU，因此，在lock后，还需要进行第二次check: `if (pInstance == 0)// 2nd test`。

在 **inmemorylogger** 中，也使用 double checked locking，它只有当当前page满的时候(1st test)，它才需要进行allocate，才会进行lock，由于TOCTTOU，因此，在lock后，还需要进行第二次check: `if (oldTail->index.load(std::memory_order_relaxed) < EVENTS_PER_PAGE)`。