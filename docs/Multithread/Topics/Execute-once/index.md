# Execute once

在阅读cppreference [`Storage class specifiers#Static local variables`](https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables)，其中谈及了这样的问题：

> If multiple threads attempt to initialize the same static local variable concurrently, the initialization occurs exactly once (similar behavior can be obtained for arbitrary functions with [`std::call_once`](https://en.cppreference.com/w/cpp/thread/call_once)).
>
> Note: usual implementations of this feature use variants of the double-checked locking pattern, which reduces runtime overhead for already-initialized local statics to a single non-atomic boolean comparison.

这是在进行multiple thread programming中，经常会遇到的一类问题，有必要总结一下。

## Implementation

1、cppreference [std::call_once](https://en.cppreference.com/w/cpp/thread/call_once)

2、[pthread_once](https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_once.html) 

3、cppreference [function-local statics](https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables) 

其实就是static local object