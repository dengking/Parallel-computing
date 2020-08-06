# Lock free

维基百科：https://en.wikipedia.org/wiki/Non-blocking_algorithm

lock-free and Atomicity and memory alignment：https://stackoverflow.com/questions/381244/purpose-of-memory-alignment



https://stackoverflow.com/questions/8918401/does-a-multiple-producer-single-consumer-lock-free-queue-exist-for-c

## Lock free data structure

drdobbs：[Lock-Free Data Structures](https://www.drdobbs.com/lock-free-data-structures/184401865)

kukuruku: [Lock-free Data Structures](https://kukuruku.co/post/lock-free-data-structures-introduction/)

## Implementation

### [syscpp](https://github.com/lightful/syscpp)

Internal lock-free MPSC messages queue

### [SPSCQueue](https://github.com/rigtorp/SPSCQueue)

A bounded single-producer single-consumer wait-free and lock-free queue written in C++11

### [lockfree](https://github.com/huangfcn/lockfree)



### [queues](https://github.com/mstump/queues)

Public domain implementation of four different lock free queues:

- SPSC lock free dynamic queue which requires a memory allocation with each insert.
- MPSC lock free dynamic queue which requires a memory allocation with each insert.
- SPSC wait free bound queue/ring buffer which which uses a fixed size pre-allocated buffer.
- MPMC lock free bound queue/ring buffer which which uses a fixed size pre-allocated buffer.