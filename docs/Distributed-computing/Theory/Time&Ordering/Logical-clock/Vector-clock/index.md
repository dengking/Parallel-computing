# Vector clock

读物:

[zhihu-陈清扬-向量时钟（Vector Clock）的本质](https://zhuanlan.zhihu.com/p/419944615)

> 读到一篇ACM Queue上好[文章](https://link.zhihu.com/?target=https%3A//queue.acm.org/detail.cfm%3Fid%3D2917756)，与诸君分享。
>
> 先说背景，分布式系统中有两大问题，一没有全局时钟，二没有共享内存。很多时候我们都需要引入*时间* 的概念，譬如来确定事件之间的顺序和因果关系。那分布式系统中既然没有全局的时钟，我们又该如何确定事件的顺序呢？

[acmqueue-Why Logical Clocks are Easy Sometimes all you need is the right language.](https://queue.acm.org/detail.cfm?id=2917756)

## wikipedia [Vector clock](https://en.wikipedia.org/wiki/Vector_clock)

