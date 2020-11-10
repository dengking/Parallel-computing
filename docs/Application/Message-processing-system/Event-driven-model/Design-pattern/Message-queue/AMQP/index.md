# AMQP

## 官网

https://www.amqp.org/

协议文档下载网址：https://www.amqp.org/resources/download



## wikipedia [Advanced Message Queuing Protocol](https://en.wikipedia.org/wiki/Advanced_Message_Queuing_Protocol) H1

The **Advanced Message Queuing Protocol** (**AMQP**) is an [open standard](https://en.wikipedia.org/wiki/Open_standard) (开放标准) [application layer](https://en.wikipedia.org/wiki/Application_layer) protocol(应用层协议) for [message-oriented middleware](https://en.wikipedia.org/wiki/Message-oriented_middleware). The defining features of **AMQP** are message orientation, queuing, [routing](https://en.wikipedia.org/wiki/Routing) (including [point-to-point](https://en.wikipedia.org/wiki/Point-to-point_(telecommunications)) and [publish-and-subscribe](https://en.wikipedia.org/wiki/Publish%E2%80%93subscribe)), reliability and security.

> NOTE: AMQP是类似于HTTP一样的属于OSI的application layer protocol。
>
> 思考：作为一种协议，其实我觉得应该要从它所提供的特性来对其进行把握，上面这段话就已经概括了其关键特性；以及使用这些特性，能够为我们解决哪些问题；

AMQP mandates（要求） the behavior of the messaging provider and client to the extent that implementations from different vendors are [interoperable](https://en.wikipedia.org/wiki/Interoperable)(AMQP要求消息传递提供者和客户端的行为达到不同供应商的实现可互操作的程度), in the same way as [SMTP](https://en.wikipedia.org/wiki/SMTP), [HTTP](https://en.wikipedia.org/wiki/HTTP), [FTP](https://en.wikipedia.org/wiki/FTP), etc. have created **interoperable systems**. Previous standardizations of [middleware](https://en.wikipedia.org/wiki/Middleware) have happened at the **API level** (e.g. [JMS](https://en.wikipedia.org/wiki/Java_Message_Service)) and were focused on **standardizing**(标准化) programmer interaction with different middleware implementations, rather than on providing **interoperability**(可交互性) between multiple implementations.[[2\]](https://en.wikipedia.org/wiki/Advanced_Message_Queuing_Protocol#cite_note-2) Unlike JMS, which defines an API and a set of behaviors that a messaging implementation must provide, AMQP is a [wire-level protocol](https://en.wikipedia.org/wiki/Wire_protocol). A **wire-level protocol** is a description of the format of the data that is sent across the network as a [stream](https://en.wikipedia.org/wiki/Byte_stream) of [bytes](https://en.wikipedia.org/wiki/Octet_(computing))(字节流）. Consequently, any tool that can create and interpret messages that conform to this **data format** can interoperate with any other compliant tool **irrespective of**（不管） implementation language.

> NOTE: 协议定义了data format，故只要遵循这个协议，那么不同的实现之间就可以进行interoperate 了；其实看了上面的这段话，我才想起来，protocol就应当如此，比如HTTP协议，显然HTTP协议的实现是非常之多的，比如我在python中，在java中是可以使用不同协议的，但是他们之间是可以进行相互沟通的；

### Overview

AMQP is a [binary](https://en.wikipedia.org/wiki/Binary_protocol), application layer protocol（二进制，应用层协议）, designed to efficiently support a wide variety of messaging applications and communication patterns. It provides flow controlled,[[3\]](https://en.wikipedia.org/wiki/Advanced_Message_Queuing_Protocol#cite_note-3) **message-oriented communication with message-delivery guarantees** such as *at-most-once* (where each message is delivered once or never), *at-least-once* (where each message is certain to be delivered, but may do so multiple times) and *exactly-once* (where the message will always certainly arrive and do so only once),[[4\]](https://en.wikipedia.org/wiki/Advanced_Message_Queuing_Protocol#cite_note-4) and authentication and/or encryption（加密） based on [SASL](https://en.wikipedia.org/wiki/Simple_Authentication_and_Security_Layer) and/or [TLS](https://en.wikipedia.org/wiki/Transport_Layer_Security).[[5\]](https://en.wikipedia.org/wiki/Advanced_Message_Queuing_Protocol#cite_note-5) It assumes an underlying reliable transport layer protocol such as [Transmission Control Protocol](https://en.wikipedia.org/wiki/Transmission_Control_Protocol)(TCP).[[6\]](https://en.wikipedia.org/wiki/Advanced_Message_Queuing_Protocol#cite_note-6)

The AMQP specification is defined in several layers: (i) a type system, (ii) a symmetric（对称的）, asynchronous protocol for the transfer of messages from one process to another, (iii) a standard, extensible message format and (iv) a set of standardised but extensible 'messaging capabilities.'



### Description of AMQP 1.0

#### Type system

**AMQP** defines a [self-describing](https://en.wikipedia.org/wiki/Self-describing) encoding scheme allowing interoperable representation of a wide range of commonly used types. It also allows typed data to be [annotated](https://en.wikipedia.org/wiki/Semantic_annotation) with additional meaning,[[17\]](https://en.wikipedia.org/wiki/Advanced_Message_Queuing_Protocol#cite_note-17) for example a particular string value might be annotated so that it could be understood as a [URL](https://en.wikipedia.org/wiki/URL). Likewise a map value containing key-value pairs for 'name', 'address' etc., might be annotated as being a representation of a 'customer' type.

The type-system is used to define a message format allowing standard and extended meta-data to be expressed and understood by processing entities. It is also used to define the communication primitives through which messages are exchanged between such entities, i.e. the AMQP *frame bodies*.

### Implementations

### Specification

