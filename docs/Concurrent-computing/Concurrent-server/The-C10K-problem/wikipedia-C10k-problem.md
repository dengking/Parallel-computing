[TOC]



# [C10k problem](https://en.wikipedia.org/wiki/C10k_problem)

The **C10k problem** is the problem of optimising [network sockets](https://en.wikipedia.org/wiki/Network_socket) to handle a large number of clients at the same time.[[1\]](https://en.wikipedia.org/wiki/C10k_problem#cite_note-C10K-1) The name C10k is a [numeronym](https://en.wikipedia.org/wiki/Numeronym) for [concurrently](https://en.wikipedia.org/wiki/Concurrent_computing) handling ten thousand connections.[[2\]](https://en.wikipedia.org/wiki/C10k_problem#cite_note-Liu-Deters-2) Note that concurrent connections are not the same as [requests per second](https://en.wikipedia.org/wiki/Requests_per_second), though they are similar: handling many requests per second requires high throughput (processing them quickly), while high number of concurrent connections requires efficient scheduling of connections. In other words, handling many requests per second is concerned with the speed of handling requests, whereas a system capable of handling a high number of concurrent connections does not necessarily have to be a fast system, only one where each request will deterministically return a response within a (not necessarily fixed) finite amount of time.

The problem of socket server optimisation has been studied because a number of factors must be considered to allow a web server to support many clients. This can involve a combination of operating system constraints and web server software limitations. According to the scope of services to be made available and the capabilities of the operating system as well as hardware considerations such as multi-processing capabilities, a multi-threading model or a [single threading](https://en.wikipedia.org/wiki/Single_threading) model can be preferred. Concurrently with this aspect, which involves considerations regarding memory management (usually operating system related), strategies implied relate to the very diverse aspects of the I/O management.[[2\]](https://en.wikipedia.org/wiki/C10k_problem#cite_note-Liu-Deters-2)

## History

The term was coined in 1999 by **Dan Kegel**,[[3\]](https://en.wikipedia.org/wiki/C10k_problem#cite_note-aosa2:nginx-3)[[4\]](https://en.wikipedia.org/wiki/C10k_problem#cite_note-Dan_Kegel,_kegel.com,_1999-4) citing the [Simtel](https://en.wikipedia.org/wiki/Simtel) FTP host, [cdrom.com](https://en.wikipedia.org/wiki/Cdrom.com), serving 10,000 clients at once over 1 [gigabit per second](https://en.wikipedia.org/wiki/Gigabit_per_second) [Ethernet](https://en.wikipedia.org/wiki/Ethernet) in that year.[[1\]](https://en.wikipedia.org/wiki/C10k_problem#cite_note-C10K-1)The term has since been used for the general issue of large number of clients, with similar numeronyms for larger number of connections, most recently "C10M" in the 2010s.[[5\]](https://en.wikipedia.org/wiki/C10k_problem#cite_note-:0-5)

By the early 2010s millions of connections on a single commodity 1U server became possible: over 2 million connections ([WhatsApp](https://en.wikipedia.org/wiki/WhatsApp), 12 cores, using [Erlang](https://en.wikipedia.org/wiki/Erlang_(programming_language)) on [FreeBSD](https://en.wikipedia.org/wiki/FreeBSD)),[[6\]](https://en.wikipedia.org/wiki/C10k_problem#cite_note-6)[[7\]](https://en.wikipedia.org/wiki/C10k_problem#cite_note-7) 10–12 million connections (MigratoryData, 12 cores, using [Java](https://en.wikipedia.org/wiki/Java_(Programming_language)) on [Linux](https://en.wikipedia.org/wiki/Linux))[[5\]](https://en.wikipedia.org/wiki/C10k_problem#cite_note-:0-5)[[8\]](https://en.wikipedia.org/wiki/C10k_problem#cite_note-8)

Common applications of very high number of connections include pub/sub servers, chat, file servers, web servers, and software-defined networking.[*citation needed*]

## See also

- [Nginx](https://en.wikipedia.org/wiki/Nginx) was created to solve the C10k problem.[[1\]](https://en.wikipedia.org/wiki/C10k_problem#cite_note-C10K-1)[[3\]](https://en.wikipedia.org/wiki/C10k_problem#cite_note-aosa2:nginx-3)
- [Load balancing (computing)](https://en.wikipedia.org/wiki/Load_balancing_(computing))
- [Event-driven architecture](https://en.wikipedia.org/wiki/Event-driven_architecture)
- [Event-driven programming](https://en.wikipedia.org/wiki/Event-driven_programming)
- [Reactor pattern](https://en.wikipedia.org/wiki/Reactor_pattern)

