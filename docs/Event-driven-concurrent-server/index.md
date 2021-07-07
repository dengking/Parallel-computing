# Event-driven model and concurrent server

一、将两者放到同一个章节来进行描述，因为两者存在着一定的关联。

二、由于event-driven model非常强大，因此concurrent server其实也可以使用 event-driven model 来进行描述，因此`Concurrent-server`章节的内容和`Event-driven-model`章节的内容有些重复，但是两个章节的侧重点是不同的:

1、`Concurrent-server`章节的重点内容在concurrency

三、很多library，都融合了两者，比如`libuv`、`Netty`



## time event && file event

大多数concurrent server需要处理:

1、time event 

2、file event
