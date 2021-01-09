# Primer

## csdn [强一致性、顺序一致性、弱一致性和共识](https://blog.csdn.net/chao2016/article/details/81149674)

> NOTE: 比较难以理解的:
>
> "系统中进程看到的顺序"和 "全局时钟下的顺序"
>
> 因为系统中的每个node都有一份copy，因此它们各自对自己的copy都进行了read、write，因此每个node都有自己的实际操作的顺序，这就是所谓"系统中进程看到的顺序"；
>
> "全局时钟下的顺序"则是指理想的状态(顺序)，这种理想状态可以这样来模拟: 由于所有的node share data，所有的node都是在对shared data进行operation(read、write)，因此可以认为所有的node都在同一个processor上执行，显然这种情况下，它们就有一个全局时钟了，在这个理想的状态(顺序)下，所有的write都立即生效(在实际中，可能无法达成)，就好比在同一个processor，这就是"强一致性（Strong Consistency）"。
>
> sequential consistency也需要将所有的process模拟到同一个processor上。
>
> 由于系统中的每个node都有一份copy，change在它们之间同步是需要时间的。
>
> 不是所有的consistency model都参考了**全局时钟**的，"Sequential Consistency"就没有参考"全局时钟"，貌似只有strong consistency才参考了"全局时钟"。Sequential Consistency貌似仅仅强调sequence；

### 1. 一致性（Consistency）

一致性（Consistency）是指多副本（Replications）问题中的数据一致性。可以分为强一致性、顺序一致性与弱一致性。

### 1.1 强一致性（Strong Consistency）

也称为：

原子一致性（Atomic Consistency）
线性一致性（Linearizable Consistency）

两个要求：

- 任何一次读都能读到某个数据的最近一次写的数据。
- 系统中的所有进程，看到的操作顺序，都和**全局时钟下的顺序一致**。

简言之，在任意时刻，所有节点中的数据是一样的。

例如，对于关系型数据库，要求更新过的数据能被后续的访问都能看到，这是强一致性。

> NOTE: 

### 1.2 顺序一致性（Sequential Consistency）

> the result of any execution is the same as if the operations of all the processors were executed in some sequential order, and the operations of each individual processor appear in this sequence in the order specified by its program. - - Lamport

两个要求：

- 任何一次读都能读到某个数据的最近一次写的数据。
- 系统的所有进程的顺序一致，而且是合理的。即不需要和全局时钟下的顺序一致，错的话一起错，对的话一起对。

举个栗子： 





![这里写图片描述](https://img-blog.csdn.net/20180721214208274?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NoYW8yMDE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

> NOTE: 上图其实是一个space-time diagram，横轴表示的全局时钟; 下面是模拟程序:
>
> ```C++
>            Global
>            int x=0, y=0;
> 
> Process 1           Process 2
> x = 4;              y=2
> cout<<y;            cout<<x;
> ```
>
> 

Write(x, 4)：写入x=4 
Read(x, 0)：读出x=0

1）图a是满足**顺序一致性**，但是不满足**强一致性**的。原因在于，从**全局时钟**的观点来看，P2进程对变量`X`的读操作在P1进程对变量X的写操作之后，然而读出来的却是旧的数据。但是这个图却是满足**顺序一致性**的，因为两个进程P1，P2的一致性并没有冲突。从这两个进程的角度来看，**顺序**应该是这样的：Write(y,2) , Read(x,0) , Write(x,4), Read(y,2)，每个进程内部的读写顺序都是合理的，但是这个顺序与**全局时钟**下看到的顺序并不一样。

> NOTE: 输出为`02`;
>
> "从**全局时钟**的观点来看，P2进程对变量`X`的读操作在P1进程对变量X的写操作之后，然而读出来的却是旧的数据"是可能方式的: 
>
> `Write(x, 4)`由于网络延时，导致更新的数据没有及时同步到P2中，因此P2读出的是旧值
>
> 每个P中，load和store是维持的；
>
> 上述 "从这两个进程的角度来看，**顺序**应该是这样的：Write(y,2) , Read(x,0) , Write(x,4), Read(y,2)"，其中得出的顺序其实也是将两个process模拟到同一个processor上得出的。

2）图b满足**强一致性**，因为每个读操作都读到了该变量的最新写的结果，同时两个进程看到的操作顺序与全局时钟的顺序一样，都是Write(y,2) , Read(x,4) , Write(x,4), Read(y,2)。

> NOTE: "Write(y,2) , Read(x,4) , Write(x,4), Read(y,2)"是有误的 ，应该是"Write(y,2) , Write(x,4), Read(x,4) , Read(y,2)"

3）图c不满足顺序一致性，当然也就不满足强一致性了。因为从进程P1的角度看，它对变量Y的读操作返回了结果0。那么就是说，P1进程的对变量Y的读操作在P2进程对变量Y的写操作之前，这意味着它认为的顺序是这样的：write(x,4) , Read(y,0) , Write(y,2), Read(x,0)，显然这个顺序又是不能被满足的，因为最后一个对变量x的读操作读出来也是旧的数据。因此这个顺序是有冲突的，不满足顺序一致性。

> NOTE: 输出为`00`;
>
> 可能方式: 
>
> `Write(x, 4)`由于网络延时，导致更新的数据没有及时同步到P2中，因此P2读出的是旧值
>
> `Write(y, 2)`由于网络延时，导致更新的数据没有及时同步到P2中，因此P1读出的是旧值
>
> 此时每个P中，load和store的order被reorder了；



### 1.3 弱一致性

数据更新后，如果能容忍后续的访问只能访问到部分或者全部访问不到，则是**弱一致性**。

**最终一致性**就属于**弱一致性**。

#### 最终一致性

不保证在任意时刻任意节点上的同一份数据都是相同的，但是随着时间的迁移，不同节点上的同一份数据总是在向趋同的方向变化。

简单说，就是在一段时间后，节点间的数据会最终达到一致状态。

**最终一致性**根据更新数据后各进程访问到数据的时间和方式的不同，又可以区分为：

1、因果一致性（Casual Consistency）。如果进程A通知进程B它已更新了一个数据项，那么进程B的后续访问将返回更新后的值，且一次写入将保证取代前一次写入。与进程A无因果关系的进程C的访问，遵守一般的最终一致性规则。

2、“读己之所写（read-your-writes）”一致性。当进程A自己更新一个数据项之后，它总是访问到更新过的值，绝不会看到旧值。这是因果一致性模型的一个特例。

3、会话（Session）一致性。这是上一个模型的实用版本，它把访问存储系统的进程放到会话的上下文中。只要会话还存在，系统就保证“读己之所写”一致性。如果由于某些失败情形令会话终止，就要建立新的会话，而且系统的保证不会延续到新的会话。

4、单调（Monotonic）读一致性。如果进程已经看到过数据对象的某个值，那么任何后续访问都不会返回在那个值之前的值。

5、单调写一致性。系统保证来自同一个进程的写操作顺序执行。要是系统不能保证这种程度的一致性，就非常难以编程了。

### 共识（Consensus)

共识问题中所有的节点要最终达成共识，由于最终目标是所有节点都要达成一致，所以根本不存在一致性强弱之分。

例如，Paxos是共识（Consensus）算法而不是强一致性（Consistency）协议。共识算法没有一致性级别的区分。

————————————————
版权声明：本文为CSDN博主「chao2016」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/chao2016/article/details/81149674



## cnblogs [强一致性、弱一致性、最终一致性](https://www.cnblogs.com/gotodsp/p/6651310.html)

强一致性：系统中的某个数据被成功更新后，后续任何对该数据的读取操作都将得到更新后的值；

弱一致性：系统中的某个数据被更新后，后续对该数据的读取操作可能得到更新后的值，也可能是更改前的值。但经过“不一致时间窗口”这段时间后，后续对该数据的读取都是更新后的值；

最终一致性：是弱一致性的特殊形式，存储系统保证在没有新的更新的条件下，最终所有的访问都是最后更新的值。



标签: [分布式系统](https://www.cnblogs.com/gotodsp/tag/分布式系统/), [事务](https://www.cnblogs.com/gotodsp/tag/事务/), [一致性](https://www.cnblogs.com/gotodsp/tag/一致性/)

