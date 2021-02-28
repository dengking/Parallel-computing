# Preface

> NOTE: 原书的preface写的非常棒，作者可以说是站在了非常高的角度来总结计算机/互联网技术发展的趋势。

In the last decade we have seen many interesting developments in databases, in distributed systems, and in the ways we build applications on top of them. There are various driving forces for these developments:

1、Internet companies such as Google, Yahoo!, Amazon, Facebook, LinkedIn, Microsoft, and Twitter are handling huge volumes of data and traffic, forcing them to create new tools that enable them to efficiently handle such scale.

> NOTE: 数据爆炸

2、Businesses need to be agile, test hypotheses cheaply, and respond quickly to new market insights by keeping development cycles short and data models flexible.

> NOTE: 业务需要灵活，测试假设成本低，通过保持开发周期短和数据模型灵活，对新的市场洞察做出快速响应。

3、Free and open source software has become very successful and is now preferred to commercial or bespoke in-house software in many environments.

4、CPU clock speeds are barely increasing, but multi-core processors are standard, and networks are getting faster. This means parallelism is only going to increase.

> NOTE: CPU clock speed几乎不增加，multi-core processors被广泛采用，网络越来越快，这些都意味着“parallelism”将会增加。

5、Even if you work on a small team, you can now build systems that are distributed across many machines and even multiple geographic regions, thanks to infrastructure as a service (IaaS) such as Amazon Web Services.

6、Many services are now expected to be highly available; extended downtime due to outages or maintenance is becoming increasingly unacceptable.

> NOTE: HA

*Data-intensive applications* are pushing the boundaries of what is possible by making use of these technological developments. We call an application *data-intensive* if data is its primary challenge—the quantity of data, the complexity of data, or the speed at which it is changing—as opposed to *compute-intensive*, where CPU cycles are the bottleneck.

> NOTE: 作者给出了data-intensive和compute-insensitive的定义。



Fortunately, behind the rapid changes in technology, there are enduring **principles** that remain true, no matter which version of a particular tool you are using. If you understand those **principles**, you’re in a position to see where each tool fits in, how to make good use of it, and how to avoid its pitfalls.

> NOTE: **变**与**不变**

We will dig into the internals of those systems, tease apart their key algorithms, discuss their principles and the trade-offs they have to make. On this journey, we will try to find useful ways of *thinking about data systems*—not just how they work, but also why they work that way, and what questions we need to ask.

> NOTE: 其实就是 [站在设计者的角度来思考](https://dengking.github.io/Post/Thoughts)。

