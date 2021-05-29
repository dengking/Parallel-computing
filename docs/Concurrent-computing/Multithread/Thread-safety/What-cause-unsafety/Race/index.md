# Race condition



## Race condition的本质



### Many-to-one

Wikipedia中关于[Race condition](https://en.wikipedia.org/wiki/Race_condition)的介绍是非常好的，显然它的核心在于race，即竞争，race的本质在于many-to-one，"many **share** the one and  system's  substantive behavior is dependent on the sequence or timing of the many operate the one"；

> NOTE: 刚开始的时候，我是这样定义的：many **share** the one and operate the one at the same time；后来想到[time of checker to time of use](https://en.wikipedia.org/wiki/Time_of_check_to_time_of_use)，显然TOCTTOU中的race并不是operate the one  at the same time，所以operate the one at the same time的这个限制是太过于局限的；后来参考Wikipedia中的内容修改为上述形式；除了TOCTTOU，还有一种情况就是在APUE的10.6 Reentrant Functions中提及的，这种情况中，many也没有operate the one at the same time，但是依然发生了race；

#### Many

many可能是many threads，many process，也可能是 [different ends of the same network](https://en.wikipedia.org/wiki/Race_condition#Networking)，是一切可以operate the one的computing entity；

#### The one

the one是many 可以 operate的target，它可能是一个variable，file，也可能是一种抽象的[privilege](https://en.wikipedia.org/wiki/Race_condition#Networking)；

需要注意的是：many对the one的operate可能at the same time，也可能前后发生；



### 从指令级别来分析race condition

1、单条instruction才是atomic的

2、一般programming language statement都对应多条instruction，则不是atomic的

3、Preemptive-multitasking

4、在执行中途被Preemped了，则导致了各种问题



## 出现race condition的情况

通过上述总结可以看出，会出现race condition的情况是非常之多的；

race condition可能发生于application内部：

1、application内部的多个computing entity operate the one in a uncontrollable sequence or time；如在application内部使用了thread pool，process pool；

2、在APUE 的10.6 Reentrant Functions章节介绍的；在这种情况下，application并没有使用thread pool，process pool，但是依然存在race，在这种情况下， many所指为对async-signal unsafe function（或者称之为**unreentrant** function，注意它不是只的signal handler）的[re-enter](https://en.wikipedia.org/wiki/Reentrancy_(computing))（重入），the one则指async-signal unsafe function中使用到的data structure；

> NOTE: 
>
> 在 drdobbs [Use Lock Hierarchies to Avoid Deadlock](https://www.drdobbs.com/parallel/use-lock-hierarchies-to-avoid-deadlock/204801163)  中，也阐述了相同观点

3、可能发生于application与OS 中的其他application之间，即OS中的多个不同computing entity operate the one in a uncontrollable sequence or time，这种情况最最典型的就是[time of checker to time of use](https://en.wikipedia.org/wiki/Time_of_check_to_time_of_use) ；

4、可能发生于network的不同end之间，这种情况在Wikipedia的[Race condition](https://en.wikipedia.org/wiki/Race_condition) 的[Networking](https://en.wikipedia.org/wiki/Race_condition#Networking)章节中有一个非常好的例子；



## Consequence

1、read 到了 中间状态

2、overwrite 

典型的例子就是：`i++`



## How to avoid it

为避免race condition可能造成的不良后果，computer science中有各种各样的应对方法；

programmer应该根据avoid race 发生的范围采取合适的方法 to avoid it；现在看来，avoid race condition的各种方法的本质是避免many对the one的以uncontrollable sequence进行operate，而是让many对the one的operate以controllable sequence；

### 互斥

比如在[Wikipedia Race condition](https://en.wikipedia.org/wiki/Race_condition)的[example](https://en.wikipedia.org/wiki/Race_condition#Example)中，通过 [mutually exclusive](https://en.wikipedia.org/wiki/Mutually_exclusive) 来避免这种会造成错误的操作方式；



### Atomic

多个thread同时对原子变量进行操作，原子变量能够保证原子性，即没有中间状态，从而保证了线程安全；

使用原子变量进行lockless programming还需要克服的一个问题是：memory reordering，这在"memory ordering"章节中会进行描述；





