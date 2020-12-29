# Consistency

"consistency"的含义是"一致"。

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