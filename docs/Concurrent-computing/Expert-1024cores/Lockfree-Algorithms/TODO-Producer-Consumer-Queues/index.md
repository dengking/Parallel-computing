# 1024cores [Producer-Consumer Queues](http://www.1024cores.net/home/lock-free-algorithms/queues)

> NOTE: 
> 1、这是我见到的，总结得最最详细的

Producer-consumer queues are one of the most fundamental components in concurrent systems, they represent means to transfer data/messages/tasks/transactions between threads/stages/agents. 

## "one-size-fits-all" 

> NOTE: 
>
> 1、这是非常重要的一个事实
>
> 2、后面作者从多个角度进行了区分，programmer应该根据实际情况进行tradeoff

If you hope that there is a single magical "one-size-fits-all" concurrent queue (MS PPL and Intel TTB fallacy), sorry, there is no. So what flavours of queues are there?

> NOTE: 
>
> 1、后面从各个角度、标准来进行区分

## Number of producer and consumer threads

Depending on **allowed number of producer and consumer threads**:

 \- Multi-producer/multi-consumer queues (MPMC)

 \- Single-producer/multi-consumer queues (SPMC)

 \- Multi-producer/single-consumer queues (MPSC)

 \- Single-producer/single-consumer queues (SPSC)

I hope this aspect is clear - for example, if you have only 1 producer and 1 consumer thread, you can use SPSC queue instead of more general MPMC queue, and as you may guess it will be significantly faster.

## Underlying data structure

Depending on **underlying data structure**:

 \- Array-based

 \- Linked-list-based

 \- Hybrid

Array-based queues are generally faster, however they are usually not strictly [lockfree](http://www.1024cores.net/home/lock-free-algorithms/introduction). The drawback is that they need to preallocate memory for the worst case. Linked-list queues grow dynamically, thus no need to preallocate any memory up-front. And hybrid queues (linked-list of small fixed-size arrays) try to combine advantages of both.