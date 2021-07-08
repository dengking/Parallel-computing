# 关于本章

一、本章讨论concurrent server相关问题，包括:

1、C10K问题

2、如何实现高效concurrent server

3、process内部如何对request、message进行高效的、并发地处理

4、如何设计thread model，以充分发挥concurrency

二、由于event-driven model非常强大，本章描述的concurrent server其实也可以使用 event-driven model 来进行描述，并且本章的内容和event-driven model章节的内容有些重复，但是两个章节的侧重点是不同的:

1、本章侧重点是在concurrency上，event-driven model章节的侧重点是message、message passing上；

2、本章侧重点是intra-process



## TODO

下面是涉及这个topic的非常好的内容: 

https://eli.thegreenplace.net/tag/concurrency

https://eli.thegreenplace.net/2017/concurrent-servers-part-1-introduction/





- [Nginx](https://en.wikipedia.org/wiki/Nginx) was created to solve the C10k problem.[[1\]](https://en.wikipedia.org/wiki/C10k_problem#cite_note-C10K-1)[[3\]](https://en.wikipedia.org/wiki/C10k_problem#cite_note-aosa2:nginx-3)
- [Load balancing (computing)](https://en.wikipedia.org/wiki/Load_balancing_(computing))
- [Event-driven architecture](https://en.wikipedia.org/wiki/Event-driven_architecture)
- [Event-driven programming](https://en.wikipedia.org/wiki/Event-driven_programming)
- [Reactor pattern](https://en.wikipedia.org/wiki/Reactor_pattern)

