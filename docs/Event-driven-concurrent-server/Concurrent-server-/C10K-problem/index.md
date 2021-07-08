# C10k problem

参考文章:

1、wikipedia [C10k problem](https://en.wikipedia.org/wiki/C10k_problem)

2、[libevent – an event notification library](http://libevent.org/)

3、nick-black [Fast UNIX Servers](https://nick-black.com/dankwiki/index.php?title=Fast_UNIX_Servers)

4、kegel [The C10K problem](http://www.kegel.com/c10k.html)

这是最早提出这个问题的。

## wikipedia [C10k problem](https://en.wikipedia.org/wiki/C10k_problem)

The **C10k problem** is the problem of optimising [network sockets](https://en.wikipedia.org/wiki/Network_socket) to handle a large number of clients at the same time.[[1\]](https://en.wikipedia.org/wiki/C10k_problem#cite_note-C10K-1) The name C10k is a [numeronym](https://en.wikipedia.org/wiki/Numeronym) for [concurrently](https://en.wikipedia.org/wiki/Concurrent_computing) handling ten thousand connections.[[2\]](https://en.wikipedia.org/wiki/C10k_problem#cite_note-Liu-Deters-2) Note that concurrent connections are not the same as [requests per second](https://en.wikipedia.org/wiki/Requests_per_second), though they are similar: handling many requests per second requires high throughput (processing them quickly), while high number of concurrent connections requires efficient scheduling of connections. In other words, handling many requests per second is concerned with the speed of handling requests, whereas a system capable of handling a high number of concurrent connections does not necessarily have to be a fast system, only one where each request will deterministically return a response within a (not necessarily fixed) finite amount of time.

The problem of socket server optimisation has been studied because a number of factors must be considered to allow a web server to support many clients. This can involve a combination of operating system constraints and web server software limitations. According to the scope of services to be made available and the capabilities of the operating system as well as hardware considerations such as multi-processing capabilities, a multi-threading model or a [single threading](https://en.wikipedia.org/wiki/Single_threading) model can be preferred. Concurrently with this aspect, which involves considerations regarding memory management (usually operating system related), strategies implied relate to the very diverse aspects of the I/O management.[[2\]](https://en.wikipedia.org/wiki/C10k_problem#cite_note-Liu-Deters-2)

