# cl.cam.ac [Practical lock-free data structures](https://www.cl.cam.ac.uk/research/srg/netos/projects/archive/lock-free/)



## Introduction

Through careful design and implementation it's possible to build data structures that are safe for concurrent use without needing to manage locks or block threads. These *non-blocking* data structures can increase performance by allowing extra concurrency and can improve robustness by avoiding some of the problems caused by priority inversion in local settings, or machine and link failures in distributed systems.

The best overall introduction to our non-blocking algorithms is the paper *Concurrent programming without locks*, currently under submission, which covers our designs for multi-word compare-and-swap, word-based software transactional memory and object-based software transactional memory.

The papers *Language support for lightweight transactions* and *Exceptions and side-effects in atomic blocks* cover the integration of a software transactional memory with a managed run-time environment.

Keir Fraser's dissertation, *Practical lock freedom*, presents a large number of new designs for concurrent data structures such as skip-lists, red-black trees and binary search trees, including new lock-based designs as well as lock-free versions.



## Source code



## Publications

