# Read-modify-write



## Wikipedia [Read-modify-write](https://infogalactic.com/info/Read-modify-write)

In [computer science](https://infogalactic.com/info/Computer_science), **read-modify-write** is a class of [atomic operations](https://infogalactic.com/info/Atomic_operation) (such as [test-and-set](https://infogalactic.com/info/Test-and-set), [fetch-and-add](https://infogalactic.com/info/Fetch-and-add), and [compare-and-swap](https://infogalactic.com/info/Compare-and-swap)) that both read a memory location and write a new value into it simultaneously(同时), either with a completely new value or some function of the previous value. 

> NOTE: "simultaneously"其实就意味着 "原子性" 。"some function of previous value" 的意思是 将previous value作为入参输入到一个function中而得到一个新value，然后将这个新value写入到memory中

These operations prevent [race conditions](https://infogalactic.com/info/Race_conditions) in multi-threaded applications. Typically they are used to implement [mutexes](https://infogalactic.com/info/Mutex) or [semaphores](https://infogalactic.com/info/Semaphore_(programming)). These atomic operations are also heavily used in [non-blocking synchronization](https://infogalactic.com/info/Non-blocking_synchronization).

> NOTE: 我们平时所使用的mutex、semaphore都是基于这些atomic operation而实现的。

[Maurice Herlihy](https://infogalactic.com/info/Maurice_Herlihy) (1991) ranks atomic operations by their *[consensus](https://infogalactic.com/info/Consensus_(computer_science)) numbers,* as follows:

> NOTE: 在CPU中，也可以使用multiple model来进行描述，因此它也可以使用consensus得到概念

| rank      |                                                              |
| --------- | ------------------------------------------------------------ |
| *∞*       | memory-to-memory move and swap, augmented queue, [compare-and-swap](https://infogalactic.com/info/Compare-and-swap), [fetch-and-cons](https://infogalactic.com/w/index.php?title=Fetch-and-cons&action=edit&redlink=1), [sticky byte](https://infogalactic.com/w/index.php?title=Sticky_byte&action=edit&redlink=1), [load-link/store-conditional](https://infogalactic.com/info/Load-link/store-conditional) (LL/SC)[[1\]](https://infogalactic.com/info/Read-modify-write#cite_note-1) |
| *2n - 2*: | n-register assignment                                        |
| *2*       | [test-and-set](https://infogalactic.com/info/Test-and-set), swap, [fetch-and-add](https://infogalactic.com/info/Fetch-and-add), queue, stack |
| *1*       | atomic read and atomic write                                 |



> NOTE:上述自底向上依次增长

It is impossible to implement an operation that requires a given **consensus number** with only operations with a lower **consensus number**, no matter how many of such operations one uses.[[2\]](https://infogalactic.com/info/Read-modify-write#cite_note-2)

**Read-modify-write instructions** often produce unexpected results when used on [I/O](https://infogalactic.com/info/I/O) devices, as a write operation may not affect the same internal [register](https://infogalactic.com/info/Hardware_register) that would be accessed in a read operation.[[3\]](https://infogalactic.com/info/Read-modify-write#cite_note-3)



## stackoverflow [Why it's termed read-modify-write but not read-write?](https://stackoverflow.com/questions/49452022/why-its-termed-read-modify-write-but-not-read-write)