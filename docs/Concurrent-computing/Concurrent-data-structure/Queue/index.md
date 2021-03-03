# 关于本章

1、queue是最最常见的一种需求。

2、关于concurrent queue，我觉得总结的非常好的文章是: 1024cores [Producer-Consumer Queues](http://www.1024cores.net/home/lock-free-algorithms/queues)

这篇文章非常系统。

## Implementation

下面按照优先级进行排序。

### [cameron314](https://github.com/cameron314)

非常成熟的实现。

1、[readerwriterqueue](https://github.com/cameron314/readerwriterqueue)

A fast single-producer, single-consumer lock-free queue for C++

2、[concurrentqueue](https://github.com/cameron314/concurrentqueue)

A fast multi-producer, multi-consumer lock-free concurrent queue for C++11



### [rigtorp](https://github.com/rigtorp)

较成熟的实现。

1、[SPSCQueue](https://github.com/rigtorp/SPSCQueue)

A bounded single-producer single-consumer wait-free and lock-free queue written in C++11

2、[MPMCQueue](https://github.com/rigtorp/MPMCQueue)



### mstump [queues](https://github.com/mstump/queues)

A public domain lock free queues implemented in C++11

Public domain implementation of four different lock free queues:

- SPSC lock free dynamic queue which requires a memory allocation with each insert.
- MPSC lock free dynamic queue which requires a memory allocation with each insert.
- SPSC wait free bound queue/ring buffer which which uses a fixed size pre-allocated buffer.
- MPMC lock free bound queue/ring buffer which which uses a fixed size pre-allocated buffer.



### huangfcn [lockfree](https://github.com/huangfcn/lockfree)




