# [6.3. CONSISTENCY MODELS](https://www.e-reading.club/chapter.php/143358/216/Tanenbaum_-_Distributed_operating_systems.html) 

Although modern multiprocessors have a great deal in common with **distributed shared memory systems**, it is time to leave the subject of multiprocessors and move on. In our brief introduction to DSM(distributed shared memory) systems at the start of this chapter, we said that they have one or more copies of each read-only page and one copy of each writable page. In the simplest implementation, when a **writable page** is referenced by a remote machine, a trap occurs and the page is fetched. However, if some **writable pages** are heavily shared, having only a single copy of each one can be a serious performance bottleneck. 

> 第一句话的意思是: 尽管现代多处理器与分布式共享内存系统有许多共同之处

Allowing multiple **copies** eases the performance problem, since it is then sufficient to update any copy, but doing so introduces a new problem: how to keep all the copies **consistent**. Maintaining **perfect consistency** is especially painful when the various copies are on different machines that can only communicate by sending messages over a slow (compared to memory speeds) network. In some DSM (and multiprocessor) systems, the solution is to accept less than **perfect consistency** as the price for **better performance**. Precisely what **consistency** means and how it can be relaxed without making programming unbearable is a major issue among DSM researchers. 

> NOTE: tradeoff: 降低consistency来获取performance。

A **consistency model** is essentially a **contract**(约定) between the **software** and the **memory** (Adve and Hill, 1990). It says that if the **software** agrees to obey certain **rules**, the **memory** promises to work correctly. If the **software** violates these **rules**, all bets are off and correctness of memory operation is no longer guaranteed. A wide spectrum of **contracts** have been devised, ranging from contracts that place only minor restrictions on the **software** to those that make normal programming nearly impossible. As you probably already guessed, the ones with minor restrictions do not perform nearly as well as the ones with major restrictions. Such is life. In this section we will study a variety of **consistency models** used in DSM systems. For additional information, see the paper by Mosberger (1993). 

> NOTE: consistency model是涵盖非常广泛的框架，在uniprocessor，multiple threading，distributed system中都可以找到它的影子；

