1、[Memory Reordering Caught in the Act](https://preshing.com/20120515/memory-reordering-caught-in-the-act/)

结合具体的例子来说明 memory reordering的存在，显然memory reordering是编写lock-free concurrent program的挑战



[Memory Barriers Are Like Source Control Operations](https://preshing.com/20120710/memory-barriers-are-like-source-control-operations/)

介绍各种memory barrier，我们使用memory barrier的目的是: "You can enforce correct memory ordering on the processor by issuing any instruction which acts as a **memory barrier**. "

在高级programming language，比如C++中，各种memory semantic其实最终都是依赖于这些memory barrier来实现的。

## 基本的规律



## 经典例子

[Memory Reordering Caught in the Act](https://preshing.com/20120515/memory-reordering-caught-in-the-act/)

其中的例子非常经典