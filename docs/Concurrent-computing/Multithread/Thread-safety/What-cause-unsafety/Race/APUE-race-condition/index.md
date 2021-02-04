# Race condition and how to avoid it

APUE的下列章节包含了race condition相关的内容：

- 8.9  Race Conditions
- 11.6 Thread Synchronization
- 3.10 File Sharing
- 3.11 Atomic Operations
- 3.3 `open` and `openat` Functions  time-of-check-to-time-of-use 

- 12.5 Reentrancy
- 10.6 Reentrant Functions



## Race condition and reentrant function



## Reentrant  and async-signal safe and thread-safe

APUE 12.5 Reentrancy给出的definition如下：

> If a function is **reentrant** with respect to multiple threads, we say that it is ***thread-safe***. This doesn’t tell us, however, whether the function is reentrant with respect to signal handlers. We say that a function that is safe to be reentered from an asynchronous signal handler is ***async-signal safe***. We saw the async-signal safe functions in Figure 10.4 when we discussed reentrant functions in Section 10.6.



APUE 10.6 Reentrant Functions给出的definition如下：

> The Single UNIX Specification specifies the functions that are guaranteed to be safe to call from within a signal handler. These functions are reentrant and are called ***async-signal safe*** by the Single UNIX Specification. Besides being reentrant, they block any signals during operation if delivery of a signal might cause inconsistencies.



在Wikipedia的[Reentrancy (computing)](https://en.wikipedia.org/wiki/Reentrancy_(computing))中讨论了Reentrancy 和 [thread-safety](https://en.wikipedia.org/wiki/Thread-safety) 