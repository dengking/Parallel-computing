# Outline of This Book

读一本书，首先看看作者是如何来组织内容的，也就是看看书的目录，在原书的《Outline of This Book》对此进行了说明。

这本书分为three parts：

在Part I，作者首先描述的是“fundamental ideas that underpin（支持） the design of data-intensive applications”，即设计一个data-intensive application的指导思想、理念，这些“ideas”是非常重要的，它指导着设计者进行设计，它们是设计者追求的目标，在文章[想法](https://dengking.github.io/Post/Thoughts/)的“意图决定最终的结果”中，对此有描述。

接着作者开始描述“data stored on one machine”相关的内容。

在Part II, 作者开始描述“ data that is distributed across multiple machines”

> In Part I of this book, we discussed aspects of data systems that apply when data is stored on a single machine. Now, in Part II, we move up a level and ask: what happens if multiple machines are involved in storage and retrieval of data?

从single machine到multiple machine，《redis设计与实现(第二版)》也是按照这种思路来组织的。

在Part III，作者讨论的是integrate several different data system的问题。



总的来说，作者的编写思路是循序渐进，由易到难。