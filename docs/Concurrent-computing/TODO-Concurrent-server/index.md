# 关于本章

一、本章讨论concurrent server相关问题，包括:

1、C10K问题

2、如何实现高效concurrent server

3、process内部如何对request、message进行高效的、并发地处理

4、如何设计thread model，以充分发挥concurrency

二、由于event-driven model非常强大，本章描述的concurrent server其实也可以使用 event-driven model 来进行描述，并且本章的内容和event-driven model章节的内容有些重复，但是两个章节的侧重点是不同的:

1、本章侧重点是在concurrency上，event-driven model章节的侧重点是message、message passing上；

2、本章侧重点是intra-process，event-driven model章节的侧重点是inter-process；



## Resource

下面是涉及这个topic的非常好的内容: 

https://eli.thegreenplace.net/tag/concurrency

https://eli.thegreenplace.net/2017/concurrent-servers-part-1-introduction/

## 使用Event-driven model来描述concurrent server

可以使用Event-driven model对concurrent server进行描述。

### 回调函数

concurrent programming中往往会涉及到当指定的事件发生的时候，需要调用一个回调函数来对发生的事件进行应答；所以它们都会涉及到回调函数；

redis使用c语言编写，它的回调函数的设置是比较简单的；