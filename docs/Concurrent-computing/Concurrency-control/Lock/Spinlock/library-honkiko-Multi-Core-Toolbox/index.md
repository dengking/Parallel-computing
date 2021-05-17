# [honkiko](https://github.com/honkiko)/**[Multi-Core-Toolbox](https://github.com/honkiko/Multi-Core-Toolbox)**



## [Multi-Core-Toolbox](https://github.com/honkiko/Multi-Core-Toolbox)/[mcs_lock.h](https://github.com/honkiko/Multi-Core-Toolbox/blob/master/mcs_lock.h)

Algorithm published on ACM TOCS, February 1991, by John M. 
Mellor-Crummey and Michael L. Scott.
http://www.cs.rochester.edu/u/scott/papers/1991_TOCS_synch.pdf

- guarantees FIFO ordering of lock acquisitions;
- spins on locally-accessible flag variables only;
- requires a small constant amount of space per lock; and
- works equally well (requiring only O(1) network transactions per lock
		acquisition) on machines with and without coherent caches.

