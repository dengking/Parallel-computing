# Granularity of lock

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

