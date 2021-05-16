# Challenge in concurrency control

本文总结在concurrency control中的一些challenge。



## wikipedia [Starvation](https://en.wikipedia.org/wiki/Starvation_(computer_science)) 

In [computer science](https://en.wikipedia.org/wiki/Computer_science), **resource starvation** is a problem encountered in [concurrent computing](https://en.wikipedia.org/wiki/Concurrent_computing) where a [process](https://en.wikipedia.org/wiki/Computer_process) is perpetually denied necessary [resources](https://en.wikipedia.org/wiki/Resource_(computer_science)) to process its work.[[1\]](https://en.wikipedia.org/wiki/Starvation_(computer_science)#cite_note-1) Starvation may be caused by errors in a scheduling or [mutual exclusion](https://en.wikipedia.org/wiki/Mutual_exclusion) algorithm, but can also be caused by [resource leaks](https://en.wikipedia.org/wiki/Resource_leak), and can be intentionally caused via a [denial-of-service attack](https://en.wikipedia.org/wiki/Denial-of-service_attack) such as a [fork bomb](https://en.wikipedia.org/wiki/Fork_bomb).



## Fairness and starvation

1、如果能够保证"fairness" ，那么就能够免于starvation

### 素材: wikipedia [Test-and-set](https://en.wikipedia.org/wiki/Test-and-set) # [Performance evaluation of test-and-set locks](https://en.wikipedia.org/wiki/Test-and-set#Performance_evaluation_of_test-and-set_locks)

When we consider fairness, we consider if a processor gets a fair chance of acquiring the lock when it is set free. In an extreme situation the processor might **starve** i.e. it might not be able to acquire the lock for an extended period of time even though it has become free during that time.