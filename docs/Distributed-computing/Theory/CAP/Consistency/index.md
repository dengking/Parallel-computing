# Consistency

"consistency"的含义是"一致"。

## 如何掌握consistency model?

### Consistency model是关于data/memory的model

对data/memory的operation包括

1、read

2、write

### Consistency model是一个非常强大的、适用面非常广泛的model

它能够对multiple model中的shared data进行描述，multiple model能够描述非常多的，下面可以看到:

### Multiple model

Multiple model中的entity可以为:

1、multicore: processor core

2、multithread: thread

3、multiprocess: process

4、distributed computing: node

因此下面的描述的内容，有的时候并不针对上述情况中的一种，而是描述的一种通用的原理。

### 理想 与 现实

**现实**: 各个entity之间存在着**通信延时**(**delay**)，一旦entity对自己的**copy of shared data**做出了write，在完成和其他的entity的**synchronization**之前(**同步延时**)，各个entity的**copy of shared data**是存在着差异的，即各个entity所看到的**shared data**是不同的。

**理想**: 各个entity之间的**通信延时**为0，或者说，它们之间不存在延时，这样就不存在**同步延时**，这样任何一个entity对自己的copy of shared data的修改能够立即同步到其他的entity中，各个entity的**copy of shared data**是不存在差异的，即各个entity所看到的**shared data**是相同的。其实这就是**strict consistency**。



### Consistency model abstract machine

为了便于理解各种consistency model，本节提出一个consistency model abstract machine，它其实就是programmer站在全局的视角来看待并运行整个multiple model，从而得到输出，它的简单定义如下:

1、所有的entity都独立地运行在这台abstract machine上

> NOTE: 可以认为每个entity有一个自己的processor

2、shared data

> NOTE: 可以认为是memory

3、每个entity都有自己的copy of shared data

> NOTE: 可以认为每个entity都全量的cache shared data

4、每个entity需要将自己对shared data的更改同步到shared data中

5、**全局时钟**、abstract machine的时钟，其实就是墙上时钟，programmer以这个时钟为参考，来看abstract machine上的各个entity，其实就是从programmer的角度来看各个entity的活动。这位programmer提供了观察整个系统的视角。

#### 模拟strict consistency model

**Strict consistency model**是最最理想的情况，同时它也是非常容易理解的。

在strict consistency中，由于:

1、**所有的write都立即生效**(在实际中，可能无法达成)

2、entity的copy of shared data能够一直保持相同

因此**Strict consistency model**可以这样模拟: 

所有的**entity**都在同一个processor上执行，使用同一个shared data，这个processor没有cache，所有的write立即生效。



### Consistency model是contract

1、它是一种contract，遵循一定的contract，则可以实现预期的效果；



### Level and tradeoff

> NOTE: 有哪些constrain、相较于上一层，放松了哪些constrain。

2、每种consistency model都有constrain，从而决定了consistency model的strong、weak

1、strict consistency是最最理想的、容易理解情况，可以以它来作为对比

2、在现实应用中，strict consistency的实现是比较困难的，并且有的application不需要strict consistency，因此可以放宽constrain，使用weak consistency model

3、programmer需要进行tradeoff



### Sequential consistency model

相较于strict consistency model，放松了**所有的write立即生效**的限制，因此在Consistency model abstract machine上的运行结果可能存在dirty data的；

Constrain: 实际的运行结果能够使用下面描述的方式进行**模拟**:

能够使用C++11 `std::atomic` 的 sequential consistency来进行模拟，下面简单地描述一下对应关系: 

1、所有entity按照program中的次序运行

2、每个entity对应一个thread，shared data对应process中所有线程共享的数据

3、在一个process中进行运行，可以得到在Consistency model abstract machine上的运行结果。

参见: stackoverflow [C++11 introduced a standardized memory model. What does it mean? And how is it going to affect C++ programming?](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g) # [A](https://stackoverflow.com/a/6319356)，其中有非常好的描述。



## 含义

multiple之间保持consistency，在computer science中，主要是data 保持consistency。

下面是一些例子:

### Memory and database

在 [Generic: Change the Way You Write Exception-Safe Code — Forever](https://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758) 中描述了这样的情况:

```C++
void User::AddFriend(User& newFriend)
{
    // Add the new friend to the vector of friends
    // If this throws, the friend is not added to
    //     the vector, nor the database
    friends_.push_back(&newFriend);
    // Add the new friend to the database
    pDB_->AddFriend(GetName(), newFriend.GetName());
}
```

This definitely causes **consistency** in the case of `vector::push_back` failing. Unfortunately, as you consult `UserDatabase::AddFriend`'s documentation, you discover with annoyance that *it* can throw an exception, too! Now you might end up with the friend in the vector, but not in the database!

## 什么时候需要考虑consistency？

HA的一种方式是replication，这就同时存在着多个entity，如果是data system的话，就涉及如何保证这些entity之间的data的consistency。





## TODO

csdn [Zookeeper专题——3、分布式一致性，几种实现的优缺点](https://blog.csdn.net/Happy_wu/article/details/80197356)

csdn [分布式一致性协议](https://blog.csdn.net/demon7552003/article/details/86657767)

这篇文章非常好，总结了各种可能的实现方式。

jianshu [取代 ZooKeeper！高并发下的分布式一致性开源组件 StateSynchronizer](https://www.jianshu.com/p/2691ef1c2b35)

