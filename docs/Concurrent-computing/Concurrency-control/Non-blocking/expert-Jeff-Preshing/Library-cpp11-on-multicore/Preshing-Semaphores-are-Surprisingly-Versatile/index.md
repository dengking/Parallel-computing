# preshing [Semaphores are Surprisingly Versatile](https://preshing.com/20150316/semaphores-are-surprisingly-versatile/)

My opinion changed once I realized that, using only semaphores and atomic operations, it’s possible to implement all of the following primitives:

1. A Lightweight Mutex
2. A Lightweight Auto-Reset Event Object
3. A Lightweight Read-Write Lock
4. Another Solution to the Dining Philosophers Problem
5. A Lightweight Semaphore With Partial Spinning