1、接收消息，消息源源不断地产生

2、并发

3、消息的种类非常多，不过的observer对消息的处理是不同的



## draft



### Observer pattern and visitor pattern

event source就是是一个abstract structure，event source会产生各种event，对于每种event，不同的listener需要执行不同的algorithm，这个过程其实非常类似于visitor:

1) 将event source看做是由这些event组成的abstract structure，则整个过程就是对这个abstract structure的visit；

2) call back

#### Observer pattern + Visitor pattern for message system

https://stackoverflow.com/questions/32079697/observer-pattern-visitor-pattern-for-message-system

