# Monitor、notify、dispatch

底层OS提供了notification mechanism(比如IO multiplexing)

Monitor不断地监控，一旦受到了notification，就按照"event和event handler之间的映射关系表"进行dispatch。



## Notification

Linux 提供了很多的notification: 

1、IO multiplexing

2、condition variable也是一种notification，它用于让programmer自定义notification。

3、Semaphore

4、Observer pattern也是一种notification。

典型的notification方式是callback。