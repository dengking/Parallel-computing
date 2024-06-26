# zhihu [【BAT面试题系列】面试官：你了解乐观锁和悲观锁吗？](https://zhuanlan.zhihu.com/p/74372722) 

> NOTE: 
>
> 1、这篇文章讲得不错，它不仅说清了乐观锁、悲观锁的概念，而且还说明了如何来实现。
>
> 2、使用在 `Optimistic-and-pessimistic` 章节中总结的: 使用 transaction 来看到optimistic lock

## **一、基本概念**

**乐观锁**和**悲观锁**是两种思想，用于解决并发场景下的数据竞争问题。

1、乐观锁：**乐观锁**在操作数据时非常**乐观**，认为别人不会同时修改数据。因此**乐观锁**不会上锁，只是在执行**更新**的时候判断一下在此期间别人是否修改了数据：如果别人修改了数据则放弃操作，否则执行操作。

> NOTE: 
>
> 1、典型的transaction方式

2、悲观锁：**悲观锁**在操作数据时比较**悲观**，认为别人会同时修改数据。因此操作数据时直接把数据锁住，直到操作完成后才会释放锁；上锁期间其他人不能修改数据。

## 二、实现方式(含实例)

在说明实现方式之前，需要明确：**乐观锁和悲观锁是两种思想，它们的使用是非常广泛的，不局限于某种编程语言或数据库。**

**悲观锁**的实现方式是加锁，加锁既可以是对代码块加锁（如Java的`synchronized`关键字），也可以是对数据加锁（如MySQL中的排它锁）。

乐观锁的实现方式主要有两种：CAS机制和版本号机制，下面详细介绍。

> NOTE: 
>
> 1、这两种实现是在其他的章节中都有说明

### 1、CAS（Compare And Swap）

下面以Java中的自增操作(i++)为例，看一下悲观锁和CAS分别是如何保证线程安全的。我们知道，在Java中自增操作不是原子操作，它实际上包含三个独立的操作：

（1）读取i值；

（2）加1；

（3）将新值写回i

因此，如果并发执行自增操作，可能导致计算结果的不准确。在下面的代码示例中：value1没有进行任何线程安全方面的保护，value2使用了乐观锁(CAS)，value3使用了悲观锁(synchronized)。运行程序，使用1000个线程同时对value1、value2和value3进行自增操作，可以发现：value2和value3的值总是等于1000，而value1的值常常小于1000。

```java
public class Test {
     
    //value1：线程不安全
    private static int value1 = 0;
    //value2：使用乐观锁
    private static AtomicInteger value2 = new AtomicInteger(0);
    //value3：使用悲观锁
    private static int value3 = 0;
    private static synchronized void increaseValue3(){
        value3++;
    }
     
    public static void main(String[] args) throws Exception {
        //开启1000个线程，并执行自增操作
        for(int i = 0; i < 1000; ++i){
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        Thread.sleep(100);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    value1++;
                    value2.getAndIncrement();
                    increaseValue3();
                }
            }).start();
        }
        //打印结果
        Thread.sleep(1000);
        System.out.println("线程不安全：" + value1);
        System.out.println("乐观锁(AtomicInteger)：" + value2);
        System.out.println("悲观锁(synchronized)：" + value3);
    }
}
```

> NOTE: 
>
> 1、原文后面是对 `getAndIncrement()` 实现的分析，在 `Compare-and-swap` 章节中的 "Example application: atomic adder" 中例子类似，原理相同。





### 2、版本号机制

> NOTE: 
>
> 1、MVCC，通过版本号的变更来判断是否发生了更改。

除了CAS，版本号机制也可以用来实现乐观锁。版本号机制的基本思路是在数据中增加一个字段version，表示该数据的版本号，每当数据被修改，版本号加1。当某个线程查询数据时，将该数据的版本号一起查出来；当该线程更新数据时，判断当前版本号与之前读取的版本号是否一致，如果一致才进行操作。

需要注意的是，这里使用了版本号作为判断数据变化的标记，实际上可以根据实际情况选用其他能够标记数据版本的字段，如时间戳等。

下面以“更新玩家金币数”为例（数据库为MySQL，其他数据库同理），看看悲观锁和版本号机制是如何应对并发问题的。

#### 非线程安全

下面的实现方式，没有进行任何线程安全方面的保护。如果有其他线程在query和update之间更新了玩家的信息，会导致玩家金币数的不准确。

```java
@Transactional
public void updateCoins(Integer playerId){
    //根据player_id查询玩家信息
    Player player = query("select coins, level from player where player_id = {0}", playerId);
    //根据玩家当前信息及其他信息，计算新的金币数
    Long newCoins = ……;
    //更新金币数
    update("update player set coins = {0} where player_id = {1}", newCoins, playerId);
}
```

#### select …… for update 排它锁

为了避免这个问题，悲观锁通过加锁解决这个问题，代码如下所示。在查询玩家信息时，使用select …… for update进行查询；该查询语句会为该玩家数据加上排它锁，直到事务提交或回滚时才会释放排它锁；在此期间，如果其他线程试图更新该玩家信息或者执行select for update，会被阻塞。

```java
@Transactional
public void updateCoins(Integer playerId){
    //根据player_id查询玩家信息（加排它锁）
    Player player = queryForUpdate("select coins, level from player where player_id = {0} for update", playerId);
    //根据玩家当前信息及其他信息，计算新的金币数
    Long newCoins = ……;
    //更新金币数
    update("update player set coins = {0} where player_id = {1}", newCoins, playerId);
}
```

#### 版本号机制

版本号机制则是另一种思路，它为玩家信息增加一个字段：version。在初次查询玩家信息时，同时查询出version信息；在执行update操作时，校验version是否发生了变化，如果version变化，则不进行更新。

```java
@Transactional
public void updateCoins(Integer playerId){
    //根据player_id查询玩家信息，包含version信息
    Player player = query("select coins, level, version from player where player_id = {0}", playerId);
    //根据玩家当前信息及其他信息，计算新的金币数
    Long newCoins = ……;
    //更新金币数，条件中增加对version的校验
    update("update player set coins = {0}, version = version + 1 where player_id = {1} and version = {2}", newCoins, playerId, player.version);
}
```





## 三、优缺点和适用场景

乐观锁和悲观锁并没有优劣之分，它们有各自适合的场景；下面从两个方面进行说明。

### 1、功能限制

与悲观锁相比，乐观锁适用的场景受到了更多的限制，无论是CAS还是版本号机制。

例如，CAS只能保证单个变量操作的原子性，当涉及到多个变量时，CAS是无能为力的，而`synchronized`则可以通过对整个代码块加锁来处理。再比如版本号机制，如果query的时候是针对表1，而update的时候是针对表2，也很难通过简单的版本号来实现乐观锁。



### 2、竞争激烈程度

如果悲观锁和乐观锁都可以使用，那么选择就要考虑竞争的激烈程度：

a、当竞争不激烈 (出现并发冲突的概率小)时，乐观锁更有优势，因为悲观锁会锁住代码块或数据，其他线程无法同时访问，影响并发，而且加锁和释放锁都需要消耗额外的资源。

b、当竞争激烈(出现并发冲突的概率大)时，悲观锁更有优势，因为乐观锁在执行更新时频繁失败，需要不断重试，浪费CPU资源。



## 四、面试官追问：乐观锁加锁吗？

下面是我对这个问题的理解：

（1）乐观锁本身是不加锁的，只是在更新时判断一下数据是否被其他线程更新了；AtomicInteger便是一个例子。

（2）有时乐观锁可能与加锁操作合作，例如，在前述updateCoins()的例子中，MySQL在执行update时会加排它锁。但这只是乐观锁与加锁操作合作的例子，不能改变“乐观锁本身不加锁”这一事实。

## 五、面试官追问：CAS有哪些缺点？

下面是CAS一些不那么完美的地方：

### 1、ABA问题

在`AtomicInteger`的例子中，ABA似乎没有什么危害。但是在某些场景下，ABA却会带来隐患，例如栈顶问题：一个栈的栈顶经过两次(或多次)变化又恢复了原值，但是栈可能已发生了变化。

对于ABA问题，比较有效的方案是引入版本号，内存中的值每发生一次变化，版本号都+1；在进行CAS操作时，不仅比较内存中的值，也会比较版本号，只有当二者都没有变化时，CAS才能执行成功。Java中的`AtomicStampedReference`类便是使用版本号来解决ABA问题的。

### 2、高竞争下的开销问题

在并发冲突概率大的高竞争环境下，如果CAS一直失败，会一直重试，CPU开销较大。针对这个问题的一个思路是引入退出机制，如重试次数超过一定阈值后失败退出。当然，更重要的是避免在高竞争环境下使用乐观锁。

### 3、功能限制

CAS的功能是比较受限的，例如CAS只能保证单个变量（或者说单个内存值）操作的原子性，这意味着：(1)原子性不一定能保证线程安全，例如在Java中需要与volatile配合来保证线程安全；(2)当涉及到多个变量(内存值)时，CAS也无能为力。

> NOTE: 
>
> 1、多个CAS  optimistic lock无法工作

除此之外，CAS的实现需要硬件层面处理器的支持，在Java中普通用户无法直接使用，只能借助atomic包下的原子类使用，灵活性受到限制。