# 关于本章

本章讨论Stream-based-message-processing-system，即基于stream构建的message processing system，下面是我们经常会看到的stream-based message processing system的另外一些提法:

1) Distributed Stream Processing

在文章linkedin [Spark Streaming vs Flink vs Storm vs Kafka Streams vs Samza : Choose Your Stream Processing Framework](https://www.linkedin.com/pulse/spark-streaming-vs-flink-storm-kafka-streams-samza-choose-prakash/):

> This is why Distributed Stream Processing has become very popular in Big Data world.

在文章scottlogic [Comparing Apache Spark, Storm, Flink and Samza stream processing engines - Part 1](https://blog.scottlogic.com/2018/07/06/comparing-streaming-frameworks-pt1.html):

> Distributed stream processing engines have been on the rise in the last few years, first Hadoop became popular as a batch processing engine, then focus shifted towards stream processing engines. 



## What is stream-based-message-processing-system?

首先需要理解stream的概念，在工程discrete的`Relation-structure-computation\Model\Stream`章节对stream概念进行了总结。其次需要理解message/event stream的含义:

### kafka [What is event streaming?](https://kafka.apache.org/intro#intro_streaming)

**Event streaming** is the digital equivalent of the human body's **central nervous system**(中枢神经系统). It is the technological foundation for the 'always-on' world where businesses are increasingly software-defined and automated, and where the user of software is more software.

Technically speaking, **event streaming** is the practice of capturing data in real-time from event sources like databases, sensors, mobile devices, cloud services, and software applications in the form of **streams of events**; storing these event streams durably for later retrieval; manipulating, processing, and reacting to the **event streams** in real-time as well as retrospectively(回顾); and routing the event streams to different destination technologies as needed. Event streaming thus ensures a continuous flow and interpretation of data so that the right information is at the right place, at the right time.



### wikipedia [Event stream processing](https://en.wikipedia.org/wiki/Event_stream_processing)



## 时代背景

在linkedin [Spark Streaming vs Flink vs Storm vs Kafka Streams vs Samza : Choose Your Stream Processing Framework](https://www.linkedin.com/pulse/spark-streaming-vs-flink-storm-kafka-streams-samza-choose-prakash/)中对stream-based message processing framework兴起的时代背景进行了非常好的总结:

> According to a [recent report by IBM Marketing cloud](https://www.mediapost.com/publications/article/291358/90-of-todays-data-created-in-two-years.html), “*90 percent of the data in the world today has been created in the last two years alone, creating 2.5 quintillion bytes of data every day – and with new devices, sensors and technologies emerging, the data growth rate will likely accelerate even more*”.
>
> Technically this means our Big Data Processing world is going to be more complex and more challenging. And a lot of use cases (e.g. mobile app ads, fraud detection, cab booking, patient monitoring,etc) need data processing in real-time, as and when data arrives, to make quick actionable decisions. This is why Distributed Stream Processing has become very popular in Big Data world.

## Implementations

关于stream-based message processing system，参见下面的较好的文章:

1)  linkedin [Spark Streaming vs Flink vs Storm vs Kafka Streams vs Samza : Choose Your Stream Processing Framework](https://www.linkedin.com/pulse/spark-streaming-vs-flink-storm-kafka-streams-samza-choose-prakash/)

2) scottlogic [Comparing Apache Spark, Storm, Flink and Samza stream processing engines - Part 1](https://blog.scottlogic.com/2018/07/06/comparing-streaming-frameworks-pt1.html)

3) upsolver [7 Popular Stream Processing Frameworks Compared](https://www.upsolver.com/blog/popular-stream-processing-frameworks-compared)。

4) zhuanlan [Apache 两个开源项目比较：Flink vs Spark](https://zhuanlan.zhihu.com/p/68206953)

5) [Flink及主流流框架比较](https://blog.csdn.net/justlpf/article/details/80292375)