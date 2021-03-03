# [khizmax](https://github.com/khizmax)/**[libcds](https://github.com/khizmax/libcds)**

> NOTE: 
> 1、非常齐全，包含多种data structure
>
> 2、比较成熟，给出了implementation的reference paper，参见 [References](https://github.com/khizmax/libcds#references) 段
>
> 

The Concurrent Data Structures (CDS) library is a collection of concurrent containers that don't require external (manual) synchronization for shared access, and safe memory reclamation (SMR) algorithms like [Hazard Pointer](http://en.wikipedia.org/wiki/Hazard_pointer) and user-space [RCU](http://en.wikipedia.org/wiki/Read-copy-update) that is used as an epoch-based SMR.

The library contains the implementations of the following containers:

1、[lock-free](http://en.wikipedia.org/wiki/Non-blocking_algorithm) stack with optional elimination(消除) support

2、several algo for lock-free queue, including classic Michael & Scott algorithm and its derivatives, the flat combining queue, the segmented queue.

3、several implementation of unordered set/map - lock-free and fine-grained lock-based

4、[flat-combining](http://mcg.cs.tau.ac.il/projects/projects/flat-combining) technique

5、lock-free [skip-list](http://en.wikipedia.org/wiki/Skip_list)

6、lock-free FeldmanHashMap/Set [Multi-Level Array Hash](http://samos-conference.com/Resources_Samos_Websites/Proceedings_Repository_SAMOS/2013/Files/2013-IC-20.pdf) with thread-safe bidirectional iterator support

7、Bronson's et al algorithm for fine-grained lock-based AVL tree

> NOTE: 
> 1、上述内容已经添加了tag，便于被检索到



Generally, each container has an intrusive and non-intrusive (STL-like) version belonging to *cds::intrusive* and *cds::container* namespace respectively.

> NOTE: 
>
> 1、"intrusive and non-intrusive " 是什么含义