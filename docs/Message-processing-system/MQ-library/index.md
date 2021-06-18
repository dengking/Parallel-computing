# MQ implementation



## MQ对比

### csdn [三大主流消息中间件优缺点](https://blog.csdn.net/qq_29066329/article/details/97299716)

rabbitmq：
优点：轻量，迅捷，容易部署和使用，拥有灵活的路由配置
缺点：性能和吞吐量较差，不易进行二次开发
rocketmq：
优点：性能好，稳定可靠，有活跃的中文社区，特点响应快
缺点：兼容性较差，但随意影响力的扩大，该问题会有改善
kafka：
优点：拥有强大的性能及吞吐量，兼容性很好
缺点：由于“攒一波再处理”导致延迟比较高，有可能消息重复消费

### cnblogs [Kafka学习之路 （一）Kafka的简介](https://www.cnblogs.com/qingyunzong/p/9004509.html) # 三、常用Message Queue对比

**3.1　RabbitMQ**

RabbitMQ是使用Erlang编写的一个开源的消息队列，本身支持很多的协议：AMQP，XMPP, SMTP, STOMP，也正因如此，它非常重量级，更适合于企业级的开发。同时实现了Broker构架，这意味着消息在发送给客户端时先在中心队列排队。对路由，负载均衡或者数据持久化都有很好的支持。



**3.2　Redis**

Redis是一个基于Key-Value对的NoSQL数据库，开发维护很活跃。虽然它是一个Key-Value数据库存储系统，但它本身支持MQ功能，所以完全可以当做一个轻量级的队列服务来使用。对于RabbitMQ和Redis的入队和出队操作，各执行100万次，每10万次记录一次执行时间。测试数据分为128Bytes、512Bytes、1K和10K四个不同大小的数据。实验表明：入队时，当数据比较小时Redis的性能要高于RabbitMQ，而如果数据大小超过了10K，Redis则慢的无法忍受；出队时，无论数据大小，Redis都表现出非常好的性能，而RabbitMQ的出队性能则远低于Redis。



**3.3　ZeroMQ**

ZeroMQ号称最快的消息队列系统，尤其针对大吞吐量的需求场景。ZeroMQ能够实现RabbitMQ不擅长的高级/复杂的队列，但是开发人员需要自己组合多种技术框架，技术上的复杂度是对这MQ能够应用成功的挑战。ZeroMQ具有一个独特的非中间件的模式，你不需要安装和运行一个消息服务器或中间件，因为你的应用程序将扮演这个服务器角色。你只需要简单的引用ZeroMQ程序库，可以使用NuGet安装，然后你就可以愉快的在应用程序之间发送消息了。但是ZeroMQ仅提供非持久性的队列，也就是说如果宕机，数据将会丢失。其中，Twitter的Storm 0.9.0以前的版本中默认使用ZeroMQ作为数据流的传输（Storm从0.9版本开始同时支持ZeroMQ和Netty作为传输模块）。



**3.4　ActiveMQ**

ActiveMQ是Apache下的一个子项目。 类似于ZeroMQ，它能够以代理人和点对点的技术实现队列。同时类似于RabbitMQ，它少量代码就可以高效地实现高级应用场景。



**3.5　Kafka/Jafka**

Kafka是Apache下的一个子项目，是一个高性能跨语言分布式发布/订阅消息队列系统，而Jafka是在Kafka之上孵化而来的，即Kafka的一个升级版。具有以下特性：快速持久化，可以在O(1)的系统开销下进行消息持久化；高吞吐，在一台普通的服务器上既可以达到10W/s的吞吐速率；完全的分布式系统，Broker、Producer、Consumer都原生自动支持分布式，自动实现负载均衡；支持Hadoop数据并行加载，对于像Hadoop的一样的日志数据和离线分析系统，但又要求实时处理的限制，这是一个可行的解决方案。Kafka通过Hadoop的并行加载机制统一了在线和离线的消息处理。Apache Kafka相对于ActiveMQ是一个非常轻量级的消息系统，除了性能非常好之外，还是一个工作良好的分布式系统。



### 4u4v [中间件是什么，常用的中间件有哪些？](http://www.4u4v.net/zhong-jian-jian-shi-shi-me-chang-yong-di-zhong-jian-jian-you-na-xie.html) 



**消息队列（Message Quequing）是在消息传输过程中保存消息的容器，消息中间件即为消息队列的承载形式**。消息是两台计算机间传送的数据单位，消息队列在将消息从它的源中继到它的目标时充当中间人，主要目的是提供路由并保证消息的传递；如果发送消息时接收者不可用，消息队列会保留消息，直到可以成功地传递它，主要解决传统结构耦合性问题、系统异步性问题以及缓解大数据量并发的问题等。

![img](http://www.4u4v.net/wp-content/uploads/2020/02/v2-f81627167eea6848f7c7c586ce636c86_hd.jpg)

消息队列有较多的型号，较为常用的为ActiveMQ、Rabbit MQ、RocketMQ和Kafk a。由于消息队列使用消息将应用程序连接起来，这些消息通过像Rabbit MQ的消息代理服务器在应用程序之间路由。

![img](http://www.4u4v.net/wp-content/uploads/2020/02/v2-ca64d2224870d45711bceba593e873c3_hd.jpg)