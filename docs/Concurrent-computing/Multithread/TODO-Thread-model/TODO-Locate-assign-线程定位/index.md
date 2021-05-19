# 定位locate、assignment、线程模型、服务模型

无论是multithread、distributed computing，都会涉及定位locate: 

1、multithread: 这个request由哪个thread来处理

这可以看做是**线程模型**

2、distributed computing: 这个请求要放到哪个node

这可以看做是**服务模式**

下面是各种example。

## Example

1、jemalloc的round-robin线程定位

避免了"cache sloshing-晃动"、提升了cache locality、Reduce lock contention提高了concurrency。

2、之前开发过的一个系统，根据`***Index`、`LoginToken`来进行线程定位

避免了"cache sloshing-晃动"、提升了cache locality、、Reduce lock contention提高了concurrency。

3、Redis cluster的做法



4、consistent hash 和此也有一定的关联



5、Redis的线程模型



## 线程定位 VS thread pool without 线程定位

一、线程定位能够进行cache optimization:

1、避免了"cache sloshing-晃动"

2、提升了cache locality

二、线程定位能够实现concurrency optimization:

减少了lock contention



## 负载均衡

负载均衡是一个衡量指标



## 算法

consistent hash

round-robin

