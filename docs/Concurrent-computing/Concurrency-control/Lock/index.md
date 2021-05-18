# Lock

1、Lock最最常见的Mutual exclusion，本文对它进行详细说明。

2、Lock是pessimistic的，关于此，参见

## wikipedia [Mutual exclusion](https://en.wikipedia.org/wiki/Mutual_exclusion)

In [computer science](https://en.wikipedia.org/wiki/Computer_science), **mutual exclusion** is a property of [concurrency control](https://en.wikipedia.org/wiki/Concurrency_control), which is instituted for the purpose of preventing [race conditions](https://en.wikipedia.org/wiki/Race_condition). It is the requirement that one [thread of execution](https://en.wikipedia.org/wiki/Thread_(computing)) never enters its [critical section](https://en.wikipedia.org/wiki/Critical_section) at the same time that another [concurrent](https://en.wikipedia.org/wiki/Concurrent_computing) thread of execution enters its own critical section, which refers to an interval of time during which a thread of execution accesses a shared resource, such as [shared memory](https://en.wikipedia.org/wiki/Shared_memory_(interprocess_communication)).





## wikipedia [Lock (computer science)](https://en.wikipedia.org/wiki/Lock_(computer_science))



## draft: Granularity of lock

引发我对这个问题思考的是对下面代码的阅读：

```c++
std::map<int, SubRequestMap>::const_iterator citerSubRequestMap = CHqAccessFlow::GetInstance()->m_SubRequestMapByMkt.find(iMktType);
if (citerSubRequestMap != CHqAccessFlow::GetInstance()->m_SubRequestMapByMkt.end())
{
    // 未使用函数方式调用添加未就绪状态的原因(该处情景：正常情况在此处会有上千个券被置位。)
    //        * 减少加减锁的次数。
    //        * 防止在置位时，其他地方触发读锁。导致数据不一致和当前函数响应过慢。
    CReadWriteLock_AUTO_WRLock WRLock(&CHqAccessFlow::GetInstance()->m_hUnreadyStock);
    for (SubRequestMap::const_iterator citerSubRequest = citerSubRequestMap->second.begin(); citerSubRequest != citerSubRequestMap->second.end(); ++citerSubRequest)
    {
        CHqAccessFlow::GetInstance()->m_UnreadyStock[iMktType][citerSubRequest->first.c_str()] = citerSubRequest->second;
    }
}
```

上述代码的注释所讨论的问题是和granularity of lock相关的，显然，上述代码中lock的粒度相较于对分别每个券加锁、操作、解锁的粒度要更大一些



## draft: Getter-setter-lock

在使用OOP的时候，一般我们会提供getter、setter方法，当位于multithread环境的时候，就可能出现如下组合：

- 多个线程同时执行getter
- 多个线程同时执行setter
- 一个线程执行setter、一个线程执行getter

显然，它的复杂度会更高。最最安全的做法是serialize对它的read和write。



## TODO

https://stackoverflow.com/questions/2252452/does-a-getter-function-need-a-mutex

https://www.informit.com/articles/article.aspx?p=1750198&seqNum=3

https://stackoverflow.com/questions/17217268/c-sharp-is-locking-within-getters-and-setters-necessary

https://rules.sonarsource.com/cpp/RSPEC-1912

https://help.semmle.com/wiki/display/CSHARP/Inconsistently+synchronized+property

https://books.google.com/books?id=IgAICAAAQBAJ&pg=PA29&lpg=PA29&dq=should+getter+and+setter+be+locked&source=bl&ots=cSQCoTywkd&sig=ACfU3U03vlcYgKl29kmWcubdqe8q2sk-gQ&hl=en&sa=X&ved=2ahUKEwjnh4jtqrDoAhX-yosBHeKkAlEQ6AEwA3oECAgQAQ#v=onepage&q=should%20getter%20and%20setter%20be%20locked&f=false

