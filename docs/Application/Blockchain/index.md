# Blockchain

在学习了[git](https://git-scm.com/)后，我对它作为[distributed version control](https://en.wikipedia.org/wiki/Distributed_version_control)的诸多特性以及它的原理印象深刻，比如它的[Data Assurance](https://git-scm.com/about/info-assurance)特性、使用[peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer)、[decentralized system](http://en.wikipedia.org/wiki/Decentralised_system)等。后来我联想到了在阅读新闻时浏览过的[区块链](https://baike.baidu.com/item/%E5%8C%BA%E5%9D%97%E9%93%BE/13465666?fr=aladdin)，记得介绍说它本质上是一个[分布式账本](https://baike.baidu.com/item/%E5%8C%BA%E5%9D%97%E9%93%BE/13465666?fr=aladdin#7_1)，它所采用的也是[peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer)，也是[decentralized system](http://en.wikipedia.org/wiki/Decentralised_system)。这样对比了一下后，发现两者挺类似的。后来我深入思考两者，发现，Ledger 和 Version control本质上也有着非常类似：

## Ledger VS Version control

两者之间是存在着共通之处的：

1、两者都需要追溯历史

2、两者都需要记录变化

3、两者对准确性都有着要求



在[百度百科区块链](https://baike.baidu.com/item/%E5%8C%BA%E5%9D%97%E9%93%BE/13465666?fr=aladdin)中，对区块链特性有如下定义：

> 区块链是一个[信息技术](https://baike.baidu.com/item/信息技术/138928)领域的术语。从本质上讲，它是一个共享[数据库](https://baike.baidu.com/item/数据库/103728)，存储于其中的数据或信息，具有“不可伪造”“全程留痕”“可以追溯”“公开透明”“集体维护”等特征。基于这些特征，[区块链技术](https://baike.baidu.com/item/区块链技术/23686191)奠定了坚实的“信任“基础，创造了可靠的“合作”机制，具有广阔的运用前景。

看到了这段话后，我想到了[git](https://git-scm.com/)的[Data Assurance](https://git-scm.com/about/info-assurance)特性：

> It is also impossible to change any file, date, commit message, or any other data in a Git repository without changing the IDs of everything after it. This means that if you have a commit ID, you can be assured not only that your project is exactly the same as when it was committed, but that nothing in its history was changed.

我猜想，正是由于Ledger 和 Version control 两者之间的一些相同的需求，所以两者在实现这些需求的时候才会选择类似的实现方式。

所以开始了[Blockchain](https://en.wikipedia.org/wiki/Blockchain)的学习，由于我是是由[git](https://git-scm.com/)而联想到Blockchain的，所以下面在对Blockchain进行介绍的时候，我会使用git中的一些概念来进行类比。





## zhihu [中国在区块链领域有哪些专家或公认的牛人？](https://www.zhihu.com/question/48049768/answer/871446114)



### [陈乐群的回答](https://www.zhihu.com/question/48049768/answer/871446114) 

当然是 [@Determinant](https://www.zhihu.com/people/2a0d9b04b92b53a2fbe1ad5df4cb6529) 啦！

### [宝术的回答](https://www.zhihu.com/question/48049768/answer/121446591) 

再搜索国内作者的书籍，看看目录就知道不可买。浪费了半天时间品尝别人的口水，还是臭的，我呸呸呸。。。

最终让我对比特币和区块链拨云见雾的是比特币白皮书和一个github项目。读完它们各花了1小时，其实很简单很简单，因为中本聪并没有发明任何新技术，但巧妙的把很多老技术组合在了一起。

1、理论。[比特币白皮书：一种点对点的电子现金系统](https://link.zhihu.com/?target=http%3A//www.8btc.com/wiki/bitcoin-a-peer-to-peer-electronic-cash-system) 注意别看这个中文版的，翻译错漏太多，比如其中第2节的交易图把“签名”和“公钥”都弄混了，不能忍！请对照看原文[https://bitcoin.org/bitcoin.pdf](https://link.zhihu.com/?target=https%3A//bitcoin.org/bitcoin.pdf)

2、实现。[GitHub - izqui/blockchain: A blockchain implementation](https://link.zhihu.com/?target=https%3A//github.com/izqui/blockchain) 这是一个golang实现。把源码读一遍,一切都如在掌中。。。看下面关于交易的代码定义，简洁明了：

> NOTE: 
>
> ```golang
> type Transaction struct 
> {	
> 	 Header    TransactionHeader	
> 	 Signature []byte	
> 	 Payload   []byte
> } 
>  
> type TransactionHeader struct 
> {	
> 	 From          []byte	
> 	 To            []byte	
> 	 Timestamp     uint32	
> 	 PayloadHash   []byte	
> 	 PayloadLength uint32	
> 	 Nonce         uint32
> }
> ```
>
> 

在阅读这些东西之前，你也只需要具备一些知识或技术经验：

1、不对称加密算法的基本原理及其应用如签名、验证

2、Hash算法如SHA的特性，如离散、不可逆等

3、Merkle Tree的基本原理，其实就是二叉树Hash

4、懂一门编程语言和计算机常识，如随机数、二进制、Timestamp

5、懂一些P2P网络的常识如BitTorrent更有助于理解

以上看懂就入门了。

个人觉得，比特币其实较难理解的是几个层次：

1，他的静态结构设计怎么样？为什么这样？如target的难度设计，加密算法的作用。

2，他是如何活生生的运行呢？如交易过程，生成新的区块，分叉的竞争，如何篡改，矿工的激励。

3，他有哪些未解决的难题？如区块扩容，交易并发能力，安全风险等。

4，比特币的扩展生态，如闪电网络，交易所，矿池，钱包。

5，抛开数字货币，**区块链**在其他场景的应用应该如何设计？如pow的替代机制、联盟链、智能合约。

比特币提供了一种思路，即如何让一群陌生人达成不可篡改的共识并留存在案。
