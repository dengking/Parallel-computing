# Fork–join model



## 维基百科[Fork–join model](https://en.wikipedia.org/wiki/Fork%E2%80%93join_model)



## Summary

### Fork-join model in OS

在[parallel computing](https://en.wikipedia.org/wiki/Parallel_computing)中，普遍采用fork-join model，当entity是process、thread时，OS提供了system call或api来实现这种model，关于此，在工程Linux-OS的process章节中进行了总结，在apue在也进行了总结。

下面是草稿

fork api：



join api

| entity  | api                                                          |
| ------- | ------------------------------------------------------------ |
| process | [WAIT(2)](http://man7.org/linux/man-pages/man2/waitpid.2.html)、[WAIT4(2)](http://man7.org/linux/man-pages/man2/wait4.2.html) |
| thread  | [PTHREAD_JOIN(3)](http://man7.org/linux/man-pages/man3/pthread_join.3.html)、[pthread_barrier_wait(3)](https://linux.die.net/man/3/pthread_barrier_wait) |

`pthread_join`、`barrier`都可以看做是这种模型。



### Fork-join model、divide-conquer and merge algorithm

[Divide-conquer](https://en.wikipedia.org/wiki/Divide_and_conquer_algorithms)是一种算法设计思想，[merge algorithm](https://en.wikipedia.org/wiki/Merge_algorithm)是一种使用divide conquer思想的算法，它主要应用于sorting领域；Fork-join model主要是描述的parallel computing的模型，利用它来实现一些divide-conquer算法的concurrent版本，APUE 11.6.8节的例子就是fork-join model来concurrent地来实现merge-algorithm。

Fork-join model和divide-conquer之间的关系，在维基百科[Fork–join model](https://en.wikipedia.org/wiki/Fork%E2%80%93join_model)中进行了总结。