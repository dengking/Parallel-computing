# wikipedia [Copy-on-write](https://en.wikipedia.org/wiki/Copy-on-write)

**Copy-on-write** (**CoW** or **COW**), sometimes referred to as **implicit sharing** or **shadowing**, is a **resource-management technique** used in [computer programming](https://en.wikipedia.org/wiki/Computer_programming) to efficiently implement a "duplicate" or "copy" operation on modifiable resources. If a resource is duplicated but not modified, it is not necessary to create a new resource; the resource can be shared between the copy and the original. Modifications must still create a copy, hence the technique: **the copy operation is deferred to the first write**（这句话是对copy on write的最好的解释）. By sharing resources in this way, it is possible to significantly reduce the resource consumption of unmodified copies, while adding a small overhead to resource-modifying operations.





## In virtual memory management

Copy-on-write finds its main use in sharing the [virtual memory](https://en.wikipedia.org/wiki/Virtual_memory) of [operating system](https://en.wikipedia.org/wiki/Operating_system) [processes](https://en.wikipedia.org/wiki/Computer_process), in the implementation of the [fork system call](https://en.wikipedia.org/wiki/Fork_(system_call)). Typically, the process does not modify any memory and immediately executes a new process, replacing the address space entirely. Thus, it would be wasteful to copy all of the process's memory during a fork, and instead the **copy-on-write technique** is used.

**Copy-on-write** can be implemented efficiently using the [page table](https://en.wikipedia.org/wiki/Page_table) by marking（标志） certain pages of [memory](https://en.wikipedia.org/wiki/Computer_storage) as **read-only** and keeping a count of the number of references（引用计数） to the page. When data is written to these pages, the [kernel](https://en.wikipedia.org/wiki/Kernel_(computing)) intercepts（拦截） the write attempt and allocates a new physical page, initialized with the copy-on-write data, although the allocation can be skipped if there is only one reference. The kernel then updates the page table with the new (writable) page, decrements the number of references, and performs the write. The new allocation ensures that a change in the memory of one process is not visible in another's.

The copy-on-write technique can be extended to support efficient [memory allocation](https://en.wikipedia.org/wiki/Memory_allocation) by having a page of [physical memory](https://en.wikipedia.org/wiki/Physical_memory) filled with zeros. When the memory is allocated, all the pages returned refer to the page of zeros and are all marked copy-on-write. This way, physical memory is not allocated for the process until data is written, allowing processes to reserve more virtual memory than physical memory and use memory sparsely, at the risk of running out of virtual address space. The combined algorithm is similar to [demand paging](https://en.wikipedia.org/wiki/Demand_paging). 

Copy-on-write pages are also used in the [Linux kernel](https://en.wikipedia.org/wiki/Linux_kernel)'s [kernel same-page merging](https://en.wikipedia.org/wiki/Kernel_same-page_merging) feature. 

Loading the libraries for an application is also a use of copy-on-write technique. The dynamic linker maps libraries as private like follows. Any writing action on the libraries will trigger a COW in virtual memory management. 



## In software

COW is also used in [library](https://en.wikipedia.org/wiki/Library_(computer_science)), [application](https://en.wikipedia.org/wiki/Application_software) and [system](https://en.wikipedia.org/wiki/System_software) code.

In [multithreaded](https://en.wikipedia.org/wiki/Multithreading_(computer_architecture)) systems, COW can be implemented without the use of traditional [locking](https://en.wikipedia.org/wiki/Lock_(software_engineering)) and instead use [compare-and-swap](https://en.wikipedia.org/wiki/Compare-and-swap) to increment or decrement the internal reference counter. Since the original resource will never be altered, it can safely be copied by multiple threads (after the reference count was increased) without the need of performance-expensive locking such as [mutexes](https://en.wikipedia.org/wiki/Lock_(computer_science)). If the reference counter turns 0, then by definition only 1 thread was holding a reference so the resource can safely be de-allocated from memory, again without the use of performance-expensive locking mechanisms. The benefit of not having to copy the resource (and the resulting performance gain over traditional deep-copying) will therefore be valid in both single- and multithreaded systems.