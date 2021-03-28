# Comparative Performance of Memory Reclamation Strategies for Lock-free and Concurrently-readable Data Structures

## Abstract

Despite their advantages, lock-free algorithms are often not adopted in practice, partly due to the perception that they perform poorly relative to lock-based alternatives in common situations when there is little contention for objects or the CPUs.

We show that memory reclamation can be a dominant performance cost for lock-free algorithms; therefore, choosing the most efficient memory reclamation method is essential to having lock-free algorithms perform well. We compare the costs of three memory reclamation strategies: 

1、quiescent-state-based reclamation, 

2、epoch-based reclamation, and

3、safe memory reclamation. 

Our experiments show that changing the workload or execution environment can change which of these schemes is the most efficient. We therefore demonstrate that there is, to date, no panacea(灵丹妙药；万能药) for memory reclamation for lock-free algorithms.

Using a common reclamation scheme, we fairly compare lock-free and concurrently readable hash tables. Our evaluation shows that programmers can choose memory reclamation schemes mostly independently of the target algorithm.


## 1.2 Memory Reclamation

Memory reclamation is required for all dynamic lock-free and concurrently-readable data structures, such as linked lists and queues. We distinguish logical deletion of a node, `N` (removing it from a shared data structure so that no new references to `N` may be created) from physical deletion of that node (allowing the memory used for `N` to be reclaimed for arbitrary reuse). If a thread `T1` logically deletes a node `N` from a lock-free data structure, it cannot physically delete `N` until no other thread `T2` holds a reference to `N`, since physically deleting `N` may cause `T2` to crash or execute incorrectly. Never physically deleting logically deleted nodes is also unacceptable, since this will eventually lead to out-of-memory errors which will stop threads from making progress.



Choosing an inefficient memory reclamation scheme can ruin the performance of a lock-free or concurrently-readable algorithm. **Reference counting** [61, 12], for example, has high overhead in the base case, and scales poorly in structures for which long chains of nodes must be traversed [47]. Little work has been done on comparing different memory reclamation strategies; we address this deficiency, showing that the performance of memory reclamation schemes depends both on their base costs, and on the target workload and execution environment. We expect our results can provide some guidance to implementers of lock-free and concurrently-readable algorithms in choosing an appropriate scheme for their specifc applications.