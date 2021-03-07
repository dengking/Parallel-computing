# Active-object+observer+visitor

1、接收消息，消息源源不断地产生

2、并发

3、消息的种类非常多，不同的observer对不同类型的消息的处理是不同的

4、performance:

a、避免频繁的new，使用inplacement new、message queue(memory pool)、使用copy替代new

参见工程`Linux-OS` `Kernel\Guide\Multitasking\Process-model\Process-resource\Process-memory-model\Virtual-address-space\Segment\Stack-VS-heap` 章节

b、一旦使用了message queue(memory pool)，就需要让所有的message都保持一个类型

c、那如何对不同种类的message进行dispatch呢？

通过if-else实现不同类型的消息进行不同的处理

使用visitor pattern实现double dispatch

## draft



### Observer pattern and visitor pattern

event source就是是一个abstract structure，event source会产生各种event，对于每种event，不同的listener需要执行不同的algorithm，这个过程其实非常类似于visitor:

1) 将event source看做是由这些event组成的abstract structure，则整个过程就是对这个abstract structure的visit；

2) call back

#### Observer pattern + Visitor pattern for message system

https://stackoverflow.com/questions/32079697/observer-pattern-visitor-pattern-for-message-system

