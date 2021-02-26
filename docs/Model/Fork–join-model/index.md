# Fork–join model



## wikipedia [Fork–join model](https://en.wikipedia.org/wiki/Fork%E2%80%93join_model)

In [parallel computing](https://en.wikipedia.org/wiki/Parallel_computing), the **fork–join model** is a way of setting up and executing parallel programs, such that execution branches off in parallel at designated points in the program, **to "join" (merge)** at a subsequent point and resume sequential execution. Parallel sections may fork [recursively](https://en.wikipedia.org/wiki/Recursion_(computer_science)) until a certain task granularity is reached. Fork–join can be considered a parallel [design pattern](https://en.wikipedia.org/wiki/Design_pattern).[[1\]](https://en.wikipedia.org/wiki/Fork–join_model#cite_note-spp-1):209 ff. It was formulated as early as 1963.[[2\]](https://en.wikipedia.org/wiki/Fork–join_model#cite_note-2)[[3\]](https://en.wikipedia.org/wiki/Fork–join_model#cite_note-3)

> NOTE: 上面提到的 "**to "join" (merge)** " 非常适合使用barrier来实现。

By nesting fork–join computations recursively, one obtains a parallel version of the [divide and conquer](https://en.wikipedia.org/wiki/Divide_and_conquer_algorithms) paradigm, expressed by the following generic [pseudocode](https://en.wikipedia.org/wiki/Pseudocode):[[4\]](https://en.wikipedia.org/wiki/Fork–join_model#cite_note-lea-4)

```C++
solve(problem):
    if problem is small enough:
        solve problem directly (sequential algorithm)
    else:
        for part in subdivide(problem)
            fork subtask to solve(part)
        join all subtasks spawned in previous loop
        return combined results
```

> NOTE: 
>
> 1、在APUE中，有这样的例子，其中是使用的`pthread_barrier`
>
> 2、后续将这种用法称为fork-join-parallel-divide-and-conquer

## Summary

### Fork-join model in OS 

#### Process/thread

在[parallel computing](https://en.wikipedia.org/wiki/Parallel_computing)中，普遍采用fork-join model，当entity是process、thread时，OS提供了system call或api来实现这种model，关于此，在工程[Linux-OS](https://dengking.github.io/Linux-OS/)的`Programming\Process`章节中进行了总结，在apue在也进行了总结。

#### Barrier

`barrier`都可以看做是这种模型。

### Fork-join model、divide-conquer and merge algorithm

[Divide-conquer](https://en.wikipedia.org/wiki/Divide_and_conquer_algorithms)是一种算法设计思想，[merge algorithm](https://en.wikipedia.org/wiki/Merge_algorithm)是一种使用divide conquer思想的算法，它主要应用于sorting领域；Fork-join model主要是描述的parallel computing的模型，利用它来实现一些divide-conquer算法的concurrent版本，APUE 11.6.8节的例子就是fork-join model来concurrent地来实现merge-algorithm。

Fork-join model和divide-conquer之间的关系，在维基百科[Fork–join model](https://en.wikipedia.org/wiki/Fork%E2%80%93join_model)中进行了总结。