# Event/message source and passing

在 `What-is-event-driven model` 章节中已经描述了event source；不同的source，它们的passing方式是不同的，最最典型的区分是是否需要经过network、是inter-process还是intra-process。



## Wikipedia [Message passing](https://infogalactic.com/info/Message_passing)

## Inter-process message passing

1、显然，这涉及inter-process communication，在工程Linux-OS中有着非常好的总结

2、Inter-process message passing的方式是多种多样的，本节无法枚举

3、最最典型的是经过network

4、关于这种场景的design pattern，参见`Message-processing-system\Event-driven-model\Design-pattern` 章节



## Intra-process message passing

1、显然，可能的message passing方式是非常多的，比如inter-thread communication



