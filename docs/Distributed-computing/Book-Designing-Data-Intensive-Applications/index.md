# 关于本章

本章是阅读 [Designing Data-Intensive Applications: The Big Ideas Behind Reliable, Scalable, and Maintainable Systems](https://www.amazon.com/Designing-Data-Intensive-Applications-Reliable-Maintainable/dp/1449373321) 的笔记。“data-intensive”的含义是“数据密集的”。

在zhihu [有哪些你看了以后大呼过瘾的编程书？](https://www.zhihu.com/question/50408698) 中，有关于这本书的讨论。

## 写作思路

作者写作本书的思路可以概括如下:

### 我们的目标

在`CHAPTER 1 Reliable, Scalable, and Maintainable Applications`中，作者首先阐述了目标，后面的很多内容都是围绕这些目标而展开的。

### 为什么需要distributed computing？

这个问题在Preface章节中给出了回答，下面是我认为的非常重要的原因: 

1) Parallel computing的兴起

2) Networks are getting faster

3) HA

显然distributed computing的兴起，背后是由于当今计算机科学的现状所决定的。

### distributed computing的挑战有哪些？

这个问题在`PART II Distributed Data#CHAPTER 8 The Trouble with Distributed Systems`进行了讨论，主要原因如下:

1) Faults and Partial Failures

2) Unreliable Networks

3) Unreliable Clocks

4) Knowledge, Truth, and Lies



我觉得上述这些挑战的根本原因在于: distributed computing中，缺乏中心角色，即它是去中心化的。



### 如何解决这些挑战？

在`CHAPTER 9 Consistency and Consensus`中，作者给出了解决这些问题的一般思路: 创造合适的抽象。