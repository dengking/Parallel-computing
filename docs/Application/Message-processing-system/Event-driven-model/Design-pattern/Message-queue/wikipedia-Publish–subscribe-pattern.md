# [Publish–subscribe pattern](https://en.wikipedia.org/wiki/Publish%E2%80%93subscribe_pattern)

In [software architecture](https://en.wikipedia.org/wiki/Software_architecture), **publish–subscribe** is a [messaging pattern](https://en.wikipedia.org/wiki/Messaging_pattern) where senders of [messages](https://en.wikipedia.org/wiki/Message_passing), called **publishers**, do not program the messages to be sent directly to specific receivers, called **subscribers**, but instead **categorize** published messages into classes without knowledge of which subscribers, if any, there may be. Similarly, subscribers express interest in one or more classes and only receive messages that are of interest, without knowledge of which publishers, if any, there are.

Publish–subscribe is a sibling（兄妹） of the [message queue](https://en.wikipedia.org/wiki/Message_queue) paradigm, and is typically one part of a larger [message-oriented middleware](https://en.wikipedia.org/wiki/Message-oriented_middleware) system. Most messaging systems support both the **pub/sub** and **message queue** models in their [API](https://en.wikipedia.org/wiki/Application_programming_interface), e.g. [Java Message Service](https://en.wikipedia.org/wiki/Java_Message_Service) (JMS).

This pattern provides greater network [scalability](https://en.wikipedia.org/wiki/Scalability) and a more dynamic [network topology](https://en.wikipedia.org/wiki/Network_topology), with a resulting decreased flexibility to modify the publisher and the structure of the published data.

## Message filtering

In the publish-subscribe model, subscribers typically receive only a subset of the total messages published. The process of selecting messages for reception and processing is called *filtering*. There are two common forms of filtering: **topic-based** and **content-based**.

In a **topic-based** system, messages are published to "topics" or **named logical channels**. Subscribers in a topic-based system will receive all messages published to the topics to which they subscribe, and all subscribers to a topic will receive the same messages. The publisher is responsible for defining the classes of messages to which subscribers can subscribe.

In a **content-based** system, messages are only delivered to a subscriber if the attributes or content of those messages matches constraints defined by the subscriber. The subscriber is responsible for classifying the messages.

Some systems support a **hybrid** of the two; publishers post messages to a topic while subscribers register content-based subscriptions to one or more topics.



## Topologies

In many pub/sub systems, publishers post messages to an intermediary [message broker or event bus](https://en.wikipedia.org/wiki/Message_broker), and subscribers register subscriptions with that **broker**, letting the **broker** perform the **filtering**. The broker normally performs a [store and forward](https://en.wikipedia.org/wiki/Store_and_forward) function to route messages from publishers to subscribers. In addition, the broker may **prioritize**（按照优先级顺序列出） messages in a [queue](https://en.wikipedia.org/wiki/Priority_queue) before routing.

Subscribers may register for specific messages at build time, initialization time or runtime. In GUI systems, subscribers can be coded to handle user commands (e.g., click of a button), which corresponds to build time registration. Some frameworks and software products use [XML](https://en.wikipedia.org/wiki/XML) configuration files to register subscribers. These configuration files are read at initialization time. The most sophisticated alternative is when subscribers can be added or removed at runtime. This latter approach is used, for example, in [database triggers](https://en.wikipedia.org/wiki/Database_trigger), [mailing lists](https://en.wikipedia.org/wiki/Mailing_list), and [RSS](https://en.wikipedia.org/wiki/RSS).

The [Data Distribution Service](https://en.wikipedia.org/wiki/Data_Distribution_Service) (DDS) middleware does not use a broker in the middle. Instead, each publisher and subscriber in the pub/sub system shares meta-data about each other via [IP multicast](https://en.wikipedia.org/wiki/IP_multicast). The publisher and the subscribers cache this information locally and route messages based on the discovery of each other in the shared cognizance.