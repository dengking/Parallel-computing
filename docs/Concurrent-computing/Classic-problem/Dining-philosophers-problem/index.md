# Dining philosophers problem

一、

1、这个问题是最最能够体现deadlock、livelock的

2、它最最能够体现circular dependency

二、这个问题本身就包含了circle: "圆桌" --》 circle；因此它天生就有circular dependency问题；

因此解决方案，就是要"破环"，可以:

1、引入Arbitrator，对应 "服务生解法/Arbitrator solution"

2、引入hierarchy

## biancheng [哲学家就餐问题分析（含解决方案）](http://c.biancheng.net/view/1233.html)

> NOTE: 
>
> 1、问题描述非常好

假设有 5 个哲学家，他们的生活只是思考和吃饭。这些哲学家共用一个**圆桌**，每位都有一把椅子。在桌子中央有一碗米饭，在桌子上放着 5 根筷子（图 1 )。

![](Dining-philosophers.gif)

当一位哲学家思考时，他与其他同事不交流。时而，他会感到饥饿，并试图拿起与他相近的两根筷子（筷子在他和他的左或右邻居之间）。一个哲学家一次只能拿起一根筷子。显然，他不能从其他哲学家手里拿走筷子。当一个饥饿的哲学家同时拥有两根筷子时，他就能吃。在吃完后，他会放下两根筷子，并开始思考。

哲学家就餐问题是一个经典的同步问题，这不是因为其本身的实际重要性，也不是因为计算机科学家不喜欢哲学家，而是因为它是大量并发控制问题的一个例子。这个代表型的例子满足：在多个进程之间分配多个资源，而且不会出现死锁和饥饿。

> NOTE: 
>
> 1、饥饿指的是 "resource starvation"

### 信号量

一种简单的解决方法是每只筷子都用一个信号量来表示。一个哲学家通过执行操作 `wait()` 试图获取相应的筷子，他会通过执行操作 `signal()` 以释放相应的筷子。

因此，共享数据为:

```C
semaphore chopstick[5];
```

其中，`chopstick` 的所有元素都初始化为 1。哲学家 `i` 的结构如下所示：

```C
do {
    wait(chopstick[i]);
    wait(chopstick[(i+1) % 5]); // 取下一个筷子
    /* eat for awhile */
    signal(chopstick[i]);
    signal(chopstick[(i+1) % 5]);
    /* think for awhile */
} while (true);
```

虽然这一解决方案保证两个邻居不能同时进食，但是它可能导致死锁，因此还是应被拒绝的。假若所有 5 个哲学家同时饥饿并拿起左边的筷子。所有筷子的信号量现在均为 0。当每个哲学家试图拿右边的筷子时，他会被永远推迟。

死锁问题有多种可能的补救措施：

1、允许最多 4 个哲学家同时坐在桌子上。

2、只有一个哲学家的两根筷子都可用时，他才能拿起它们（他必须在临界区内拿起两根 辕子)。

3、使用非对称解决方案。即单号的哲学家先拿起左边的筷子，接着右边的筷子；而双 号的哲学家先拿起右边的筷子，接着左边的筷子。

## baike [哲学家就餐问题](https://baike.baidu.com/item/%E5%93%B2%E5%AD%A6%E5%AE%B6%E5%B0%B1%E9%A4%90%E9%97%AE%E9%A2%98/10929794?fr=aladdin)

### 问题描述

#### 死锁 和 活锁

哲学家从来不交谈，这就很危险，可能产生死锁，每个哲学家都拿着左手的餐叉，永远都在等右边的餐叉（或者相反）。即使没有死锁，也有可能发生资源耗尽。例如，假设规定当哲学家等待另一只餐叉超过五分钟后就放下自己手里的那一只餐叉，并且再等五分钟后进行下一次尝试。这个策略消除了死锁（系统总会进入到下一个状态），但仍然有可能发生“[活锁](https://baike.baidu.com/item/活锁)”。如果五位哲学家在完全相同的时刻进入餐厅，并同时拿起左边的餐叉，那么这些哲学家就会等待五分钟，同时放下手中的餐叉，再等五分钟，又同时拿起这些餐叉。

### 问题解法

#### 服务生解法/Arbitrator solution





## wikipedia [Dining philosophers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)



## golangprograms [Golang Concurrency # Illustration of the dining philosophers problem in Golang](https://www.golangprograms.com/go-language/concurrency.html)