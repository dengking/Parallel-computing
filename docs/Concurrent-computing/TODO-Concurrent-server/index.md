# 关于本章

1、本章讨论concurrent server相关问题，包括C10K问题、如何实现高效concurrent server。

2、由于event-driven model非常强大，本章描述的concurrent server其实也可以使用 event-driven model 来进行描述，并且本章的内容和event-driven model章节的内容有些重复，但是两个章节的侧重点是不同的:

本章侧重点是在concurrency上，event-driven model章节的侧重点是message、message passing上；

本章侧重点是intra-process，event-driven model章节的侧重点是inter-process；