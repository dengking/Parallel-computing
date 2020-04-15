# 前言

在学习了[git](https://git-scm.com/)后，我对它作为[distributed version control](https://en.wikipedia.org/wiki/Distributed_version_control)的诸多特性以及它的原理印象深刻，比如它的[Data Assurance](https://git-scm.com/about/info-assurance)特性、使用[peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer)、[decentralized system](http://en.wikipedia.org/wiki/Decentralised_system)等。后来我联想到了在阅读新闻时浏览过的[区块链](https://baike.baidu.com/item/%E5%8C%BA%E5%9D%97%E9%93%BE/13465666?fr=aladdin)，记得介绍说它本质上是一个[分布式账本](https://baike.baidu.com/item/%E5%8C%BA%E5%9D%97%E9%93%BE/13465666?fr=aladdin#7_1)，它所采用的也是[peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer)，也是[decentralized system](http://en.wikipedia.org/wiki/Decentralised_system)。这样对比了一下后，发现两者挺类似的。后来我深入思考两者，发现，Ledger 和 Version control本质上也有着非常类似：

## Ledger VS Version control

两者之间是存在着共通之处的：

- 两者都需要追溯历史
- 两者都需要记录变化
- 两者对准确性都有着要求



在[百度百科区块链](https://baike.baidu.com/item/%E5%8C%BA%E5%9D%97%E9%93%BE/13465666?fr=aladdin)中，对区块链特性有如下定义：

> 区块链是一个[信息技术](https://baike.baidu.com/item/信息技术/138928)领域的术语。从本质上讲，它是一个共享[数据库](https://baike.baidu.com/item/数据库/103728)，存储于其中的数据或信息，具有“不可伪造”“全程留痕”“可以追溯”“公开透明”“集体维护”等特征。基于这些特征，[区块链技术](https://baike.baidu.com/item/区块链技术/23686191)奠定了坚实的“信任“基础，创造了可靠的“合作”机制，具有广阔的运用前景。

看到了这段话后，我想到了[git](https://git-scm.com/)的[Data Assurance](https://git-scm.com/about/info-assurance)特性：

> It is also impossible to change any file, date, commit message, or any other data in a Git repository without changing the IDs of everything after it. This means that if you have a commit ID, you can be assured not only that your project is exactly the same as when it was committed, but that nothing in its history was changed.



我猜想，正是由于Ledger 和 Version control 两者之间的一些相同的需求，所以两者在实现这些需求的时候才会选择类似的实现方式。

所以开始了[Blockchain](https://en.wikipedia.org/wiki/Blockchain)的学习，由于我是是由[git](https://git-scm.com/)而联想到Blockchain的，所以下面在对Blockchain进行介绍的时候，我会使用git中的一些概念来进行类比。

# [Blockchain](https://en.wikipedia.org/wiki/Blockchain)

A **blockchain**, originally **block chain**, is a growing list of [records](https://en.wikipedia.org/wiki/Record_(computer_science)), called *blocks*, that are linked using [cryptography](https://en.wikipedia.org/wiki/Cryptography). Each block contains a [cryptographic hash](https://en.wikipedia.org/wiki/Cryptographic_hash_function) of the previous block, a [timestamp](https://en.wikipedia.org/wiki/Trusted_timestamping), and transaction data (generally represented as a [Merkle tree](https://en.wikipedia.org/wiki/Merkle_tree)).

> NOTE: 下面是Blockchain的简单模型：
>
> ![](https://www.researchgate.net/profile/Alin_Tomescu/publication/316789505/figure/fig1/AS:492107526873088@1494338985254/The-Bitcoin-blockchain-is-a-hash-chain-of-blocks-Each-block-has-a-Merkle-tree-of.png)
>
> 原文链接：https://www.researchgate.net/figure/The-Bitcoin-blockchain-is-a-hash-chain-of-blocks-Each-block-has-a-Merkle-tree-of_fig1_316789505
>
> > The Bitcoin blockchain is a hash chain of blocks. Each block has a Merkle tree of transactions. 
>
> 可以加个blockchain简单地看做是[hash chain](https://en.wikipedia.org/wiki/Hash_chain)。

By design, a blockchain is **resistant to modification of the data**. It is "an open, distributed ledger that can record transactions between two parties efficiently and in a verifiable and permanent way".

> NOTE: 这种特性（resistant to modification of the data）对于一个[ledger](https://en.wikipedia.org/wiki/Ledger) （账本）而言是必须的，否则这个ledger就完全没有意义了。
>

For use as a [distributed ledger](https://en.wikipedia.org/wiki/Distributed_ledger), a blockchain is typically managed by a [peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer) network collectively adhering to a [protocol](https://en.wikipedia.org/wiki/Protocol_(communication)) for inter-node communication and validating new **blocks**. Once recorded, the data in any given block cannot be altered retroactively without alteration of all subsequent blocks, which requires consensus of the network majority. Although blockchain records are not unalterable, blockchains may be considered [secure by design](https://en.wikipedia.org/wiki/Secure_by_design) and exemplify a distributed computing system with high [Byzantine fault tolerance](https://en.wikipedia.org/wiki/Byzantine_fault_tolerance). [Decentralized](https://en.wikipedia.org/wiki/Decentralized) consensus has therefore been claimed with a blockchain.

> NOTE: blockchain的设计是resistant to modification of the data，也就是上面这段话中的 [secure by design](https://en.wikipedia.org/wiki/Secure_by_design) ，并且它已经达到了比较高的[Byzantine fault tolerance](https://en.wikipedia.org/wiki/Byzantine_fault_tolerance)标准，所以虽然从技术上来说是是能够篡改record的，但是这并不能够否定blockchain的可用性。

## History

The first work on a cryptographically secured chain of blocks was described in 1991 by Stuart Haber and W. Scott Stornetta.  They wanted to implement a system where document timestamps could not be tampered with. In 1992, Bayer, Haber and Stornetta incorporated [Merkle trees](https://en.wikipedia.org/wiki/Merkle_tree) to the design, which improved its efficiency by allowing several document certificates to be collected into one block.

> NOTE: git中也使用了  [Merkle trees](https://en.wikipedia.org/wiki/Merkle_tree) 

> NOTE: 上述描述的问题是一类叫做：[Trusted timestamping](https://en.wikipedia.org/wiki/Trusted_timestamping)的问题



## [Structure](https://en.wikipedia.org/wiki/Blockchain#Structure)

A blockchain is a [decentralized](https://en.wikipedia.org/wiki/Decentralized_computing), [distributed](https://en.wikipedia.org/wiki/Distributed_computing), and oftentimes public, digital ledger that is used to record transactions across many computers so that any involved [record](https://en.wikipedia.org/wiki/Storage_record) cannot be altered retroactively, without the alteration of all subsequent blocks. This allows the participants to verify and audit transactions independently and relatively inexpensively. A blockchain database is managed autonomously using a [peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer) network and a distributed timestamping server. They are [authenticated](https://en.wikipedia.org/wiki/Authentication) by [mass collaboration](https://en.wikipedia.org/wiki/Mass_collaboration) powered by [collective](https://en.wikipedia.org/wiki/Collective) [self-interests](https://en.wikipedia.org/wiki/Self-interest). Such a design facilitates [robust](https://en.wikipedia.org/wiki/Robustness_(computer_science)) [workflow](https://en.wikipedia.org/wiki/Workflow) where participants' uncertainty regarding data security is marginal. The use of a blockchain removes the characteristic of infinite [reproducibility](https://en.wikipedia.org/wiki/Reproduction_(economics)) from a digital asset. It confirms that each unit of value was transferred only once, solving the long-standing problem of [double spending](https://en.wikipedia.org/wiki/Double_spending). A blockchain has been described as a *value-exchange protocol*. A blockchain can maintain [title rights](https://en.wikipedia.org/wiki/Title_(property)) because, when properly set up to detail the exchange agreement, it provides a record that compels [offer and acceptance](https://en.wikipedia.org/wiki/Offer_and_acceptance).

### [Blocks](https://en.wikipedia.org/wiki/Blockchain#Blocks)

Blocks hold batches of valid [transactions](https://en.wikipedia.org/wiki/Transaction_processing) that are hashed and encoded into a [Merkle tree](https://en.wikipedia.org/wiki/Merkle_tree). Each block includes the [cryptographic hash](https://en.wikipedia.org/wiki/Cryptographic_hash) of the prior block in the blockchain, linking the two. The linked blocks form a chain. This [iterative](https://en.wikipedia.org/wiki/Iteration) process confirms the integrity of the previous block, all the way back to the original **genesis block**（源头）. 

Sometimes separate blocks can be produced concurrently, creating a temporary fork. In addition to a secure hash-based history, any blockchain has a specified algorithm for scoring different versions of the history so that one with a higher score can be selected over others. Blocks not selected for inclusion in the chain are called **orphan blocks**. Peers supporting the **database** have different versions of the history from time to time. They keep only the **highest-scoring version** of the database known to them. Whenever a peer receives a **higher-scoring version** (usually the old version with a single new block added) they extend or overwrite their own **database** and retransmit the improvement to their peers. There is never an absolute guarantee that any particular entry will remain in the best version of the history forever. Blockchains are typically built to add the score of new blocks onto old blocks and are given incentives to extend with new blocks rather than overwrite old blocks. 

> NOTE: database中保存什么数据呢？是整个blockchain？

### [Decentralization](https://en.wikipedia.org/wiki/Blockchain#Decentralization)

> NOTE: 其实这一段所描述的peer-to-peer network的优势。



# Git VS Blockchain



**Blockchain**的block，非常类似于git的commit；

blockchain中chain的含义就是linked block。git中的**branch**其实对应的就是linked commit。

blockchain使用[cryptographic hash](https://en.wikipedia.org/wiki/Cryptographic_hash_function) 来作为两个block的link，这其实就是[Hash Chain](http://en.wikipedia.org/wiki/Hash_chain)。blockchain的这种组织方式实现了Data Assurance（在前面章节中所介绍的）:

> Once recorded, the data in any given block cannot be altered retroactively without alteration of all subsequent blocks, which requires consensus of the network majority. 

git也采用了类似的link来组织commit，git的这种组织方式实现Data Assurance，参见：

- [Data Assurance](https://git-scm.com/about/info-assurance)

- [Cryptographic authentication of history](https://en.wikipedia.org/wiki/Git#Characteristics)

可以看到，两者都使用了[Hash Chain](http://en.wikipedia.org/wiki/Hash_chain)，这种链式依赖的结构能够非常好的实现Data Assurance。

blockchain中使用 [Merkle tree](https://en.wikipedia.org/wiki/Merkle_tree) 来组织 transaction data。git中使用  [Merkle tree](https://en.wikipedia.org/wiki/Merkle_tree) 来组织 tracked file。

blockchain中添加一个新的block需要network中的所有的peer的consensus 。git中添加一个新的commit不涉及这些问题，git中使用pull和push来在各个peer（repository）之间的同步。

blockchain使用peer-to-peer架构；git使用peer-to-peer架构；

思考：git的每个peer都有一个自己的repository来保存完整的commit数据，blockchain呢？是否也是类似这样的？应该是这样的。



git和blockchain都可以看做是[Merkle tree](https://en.wikipedia.org/wiki/Merkle_tree)，不同的是git是支持branch，所以git的commit所组成的是tree，而blockchain则是线性的。

## 看看别人怎么说

### [Why is Git not considered a “block chain”?](https://stackoverflow.com/questions/46192377/why-is-git-not-considered-a-block-chain)

