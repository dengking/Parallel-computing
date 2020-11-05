# Computation system/Computation engine

在学习TensorFlow的实现的时候，想到了这个主题，觉得有必要总结一下。

## 时代背景

1) 算力增强

2) 网络增强

3) big data

4) AI

对big data的computation，能够实现非常大的价值，本章就是对这个话题的探讨，作为software engineer，我们需要考虑如下问题:

1) 如何实现? 如何架构?



下面结合具体实现来看: 

## Implementation: Microsoft [Naiad](https://www.microsoft.com/en-us/research/project/naiad/)

> The Naiad project is an investigation of **data-parallel dataflow computation**, like Dryad and DryadLINQ, but with a focus on low-latency streaming and cyclic computations. Naiad introduces a new **computational model**, timely dataflow, which combines low-latency asynchronous message flow with lightweight coordination when required. These **primitives**(原语) allow the efficient implementation of many **dataflow patterns**, from **bulk** and streaming computation to iterative graph processing and machine learning.

"timely dataflow"的含义是"及时数据流"。

"primitives"即"原语"，它告诉我们Naiad提供了一些API来供user对computation进行描述。

从最后一段话可以看出，Naiad是非常灵活的。

### 实现思路: 

1) stream model

原文中的dataflow其实就是stream。

2) data-parallel dataflow computation 

3) distributed/parallel computing

4) symbolic programming

## Implementation: Google TensorFlow

### 实现思路: 

主要是借鉴Microsoft Naiad的实现思路。