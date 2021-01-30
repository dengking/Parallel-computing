# 关于本章

本章记录阅读 lock free 主题的文章。

## 文章总结

### preshing [Memory Reordering Caught in the Act](https://preshing.com/20120515/memory-reordering-caught-in-the-act/)

结合具体的例子来说明 memory reordering的存在，显然memory reordering是编写lock-free concurrent program的挑战、

阅读完这篇文章，有如下疑问:

1、memory barrier 和 full memory barrier

2、memory barrier 和 acquire-release semantic

### preshing [An Introduction to Lock-Free Programming](https://preshing.com/20120612/an-introduction-to-lock-free-programming/)

介绍了lock-free的含义

介绍了lock-free programming的技术总览



### preshing [Memory Barriers Are Like Source Control Operations](https://preshing.com/20120710/memory-barriers-are-like-source-control-operations/)

介绍各种memory barrier，我们使用memory barrier的目的是: "You can enforce correct memory ordering on the processor by issuing any instruction which acts as a **memory barrier**. "

在高级programming language，比如C++中，各种memory semantic其实最终都是依赖于这些memory barrier来实现的。

## 基本的规律



## 控制memory ordering

### 控制runtime memory ordering

这涉及了CPU的运行机制

### 控制compile-time memory ordering

这涉及了compiler、programming language specialization，尤其是expression evaluation的问题

下面是一些draft

#### draft function call acts as a compiler barrier

在阅读[Memory Reordering Caught in the Act](https://preshing.com/20120515/memory-reordering-caught-in-the-act/)的例子的source code的时候，发现了下面的内容:

```C++
class MersenneTwister
{
    unsigned int m_buffer[MT_LEN];
    int m_index;

public:
    MersenneTwister(unsigned int seed);
    // Declare noinline so that the function call acts as a compiler barrier:
    unsigned int integer() __attribute__((noinline));
};
```

显然，上述compiler barrier是compile-time对memory ordering进行控制。

下面是Google: function call acts as a compiler barrier

stackoverflow [c++ atomic: would function call act as memory barrier?](https://stackoverflow.com/questions/40641904/c-atomic-would-function-call-act-as-memory-barrier)

preshing [Memory Ordering at Compile Time](https://preshing.com/20120625/memory-ordering-at-compile-time/)



## 经典例子

[Memory Reordering Caught in the Act](https://preshing.com/20120515/memory-reordering-caught-in-the-act/)

其中的例子非常经典