# Deadlock and livelock



## wikipedia [Deadlock](https://en.wikipedia.org/wiki/Deadlock)



## Livelock

### baike [活锁](https://baike.baidu.com/item/%E6%B4%BB%E9%94%81)

#### Example

**单一实体的活锁**

例如[线程](https://baike.baidu.com/item/线程/103101)从队列中拿出一个任务来执行，如果任务执行失败，那么将任务重新加入队列，继续执行。假设任务总是执行失败，或者某种依赖的条件总是不满足，那么线程一直在繁忙却没有任何结果。

**协同导致的活锁**

> NOTE: 
>
> 1、"协同"是指有多人同时参与

生活中的典型例子： 两个人在窄路相遇，同时向一个方向避让，然后又向另一个方向避让，如此反复。

通信中也有类似的例子，多个用户共享信道（最简单的例子是大家都用对讲机），同一时刻只能有一方发送信息。发送信号的用户会进行[冲突检测](https://baike.baidu.com/item/冲突检测/9032239)， 如果发生冲突，就选择避让，然后再发送。 假设避让算法不合理，就导致每次发送，都冲突，避让后再发送，还是冲突。

计算机中的例子：两个线程发生了某些条件的碰撞后重新执行，那么如果再次尝试后依然发生了碰撞，长此下去就有可能发生活锁。

> NOTE: 
>
> 1、从上面的描述可以看出，livelock本质上就是不断地碰撞

#### 解决方法

解决协同活锁的一种方案是调整重试机制。

**比如引入一些随机性。**例如如果检测到冲突，那么就暂停随机的一定时间进行重试。这回大大减少碰撞的可能性。 典型的例子是以太网的[CSMA/CD](https://baike.baidu.com/item/CSMA%2FCD/986847)检测机制。

另外为了避免可能的[死锁](https://baike.baidu.com/item/死锁/2196938)，适当加入一定的重试次数也是有效的解决办法。尽管这在业务上会引起一些复杂的逻辑处理。

**比如约定重试机制避免再次冲突。** 例如自动驾驶的防碰撞系统（假想的例子），可以根据序列号约定检测到相撞风险时，序列号小的飞机朝上飞， 序列号大的飞机朝下飞。 [2]

### 补充案例

#### dining philosopher

所有的哲学家，同时拿起左边的叉子，又同时方向，这是非常经典的案例。



### 素材

guru99 [Livelock: What is, Example, Difference with Deadlock](https://www.guru99.com/what-is-livelock-example.html)

https://www.geeksforgeeks.org/deadlock-starvation-and-livelock/


https://stackoverflow.com/questions/6155951/whats-the-difference-between-deadlock-and-livelock


https://stackoverflow.com/questions/1036364/good-example-of-livelock


https://www.quora.com/What-is-the-difference-between-deadlock-and-livelock-deadlock-infinite-recursion-and-starvation/answer/Akash-Kava