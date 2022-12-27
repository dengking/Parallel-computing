# Lock convoy

这是在阅读 rdrop [Making Lockless Synchronization Fast: Performance Implications of Memory Reclamation](http://www.rdrop.com/users/paulmck/RCU/hart_ipdps06.pdf) 时，其中有如下描述:

> Many programmers avoid locking to improve performance, while others replace locks with non-blocking synchronization to protect against deadlock, priority inversion, and convoying.



## wikipedia [Lock convoy](https://en.wikipedia.org/wiki/Lock_convoy)