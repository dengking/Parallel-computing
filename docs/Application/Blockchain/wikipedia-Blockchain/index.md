# wikipedia [Blockchain](https://en.wikipedia.org/wiki/Blockchain)

A **blockchain**, originally **block chain**, is a growing list of [records](https://en.wikipedia.org/wiki/Record_(computer_science)), called *blocks*, that are linked using [cryptography](https://en.wikipedia.org/wiki/Cryptography). Each block contains a [cryptographic hash](https://en.wikipedia.org/wiki/Cryptographic_hash_function) of the previous block, a [timestamp](https://en.wikipedia.org/wiki/Trusted_timestamping), and transaction data (generally represented as a [Merkle tree](https://en.wikipedia.org/wiki/Merkle_tree)).

> NOTE: 
>
> 下面是Blockchain的简单模型：
>
> ![](https://www.researchgate.net/profile/Alin_Tomescu/publication/316789505/figure/fig1/AS:492107526873088@1494338985254/The-Bitcoin-blockchain-is-a-hash-chain-of-blocks-Each-block-has-a-Merkle-tree-of.png)
>
> 原文链接：https://www.researchgate.net/figure/The-Bitcoin-blockchain-is-a-hash-chain-of-blocks-Each-block-has-a-Merkle-tree-of_fig1_316789505
>
> > The Bitcoin blockchain is a hash chain of blocks. Each block has a Merkle tree of transactions. 
>
> 可以加个blockchain简单地看做是[hash chain](https://en.wikipedia.org/wiki/Hash_chain)。

By design, a blockchain is **resistant to modification of the data**. It is "an open, distributed ledger that can record transactions between two parties efficiently and in a verifiable and permanent way".

> NOTE: 
>
> 这种特性（resistant to modification of the data）对于一个[ledger](https://en.wikipedia.org/wiki/Ledger) （账本）而言是必须的，否则这个ledger就完全没有意义了。

For use as a [distributed ledger](https://en.wikipedia.org/wiki/Distributed_ledger), a blockchain is typically managed by a [peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer) network collectively adhering to a [protocol](https://en.wikipedia.org/wiki/Protocol_(communication)) for **inter-node communication** and validating new **blocks**. Once recorded, the data in any given block cannot be altered retroactively(逆向的) without alteration of all subsequent blocks, which requires consensus of the network majority. Although blockchain records are not unalterable, blockchains may be considered [secure by design](https://en.wikipedia.org/wiki/Secure_by_design) and exemplify(展示了、例证了) a distributed computing system with high [Byzantine fault tolerance](https://en.wikipedia.org/wiki/Byzantine_fault_tolerance). [Decentralized](https://en.wikipedia.org/wiki/Decentralized) consensus has therefore been claimed with a blockchain.

> NOTE: 
>
> blockchain的设计是resistant to modification of the data，也就是上面这段话中的 [secure by design](https://en.wikipedia.org/wiki/Secure_by_design) ，并且它已经达到了比较高的[Byzantine fault tolerance](https://en.wikipedia.org/wiki/Byzantine_fault_tolerance)标准，所以虽然从技术上来说是是能够篡改record的，但是这并不能够否定blockchain的可用性。

## History

The first work on a cryptographically secured chain of blocks was described in 1991 by Stuart Haber and W. Scott Stornetta.  They wanted to implement a system where document timestamps could not be tampered(篡改) with. In 1992, Bayer, Haber and Stornetta incorporated(合并、编入) [Merkle trees](https://en.wikipedia.org/wiki/Merkle_tree) to the design, which improved its efficiency by allowing several document certificates to be collected into one block.

> NOTE: git中也使用了  [Merkle trees](https://en.wikipedia.org/wiki/Merkle_tree) 

> NOTE: 上述描述的问题是一类叫做：[Trusted timestamping](https://en.wikipedia.org/wiki/Trusted_timestamping)的问题



## [Structure](https://en.wikipedia.org/wiki/Blockchain#Structure)

A blockchain is a [decentralized](https://en.wikipedia.org/wiki/Decentralized_computing), [distributed](https://en.wikipedia.org/wiki/Distributed_computing), and oftentimes public, digital ledger that is used to record transactions(交易) across many computers so that any involved [record](https://en.wikipedia.org/wiki/Storage_record) cannot be altered retroactively(逆向的), without the alteration of all subsequent blocks. This allows the participants to verify(验证) and audit(审计) transactions independently and relatively inexpensively. A blockchain database is managed autonomously(自治的) using a [peer-to-peer](https://en.wikipedia.org/wiki/Peer-to-peer) network and a distributed timestamping server. They are [authenticated](https://en.wikipedia.org/wiki/Authentication) by [mass collaboration](https://en.wikipedia.org/wiki/Mass_collaboration) powered by [collective](https://en.wikipedia.org/wiki/Collective) [self-interests](https://en.wikipedia.org/wiki/Self-interest). Such a design facilitates [robust](https://en.wikipedia.org/wiki/Robustness_(computer_science)) [workflow](https://en.wikipedia.org/wiki/Workflow) where participants' uncertainty regarding data security is marginal(微不足道的). The use of a blockchain removes the characteristic of infinite [reproducibility](https://en.wikipedia.org/wiki/Reproduction_(economics)) from a digital asset. It confirms that each unit of value was transferred only once, solving the long-standing problem of [double spending](https://en.wikipedia.org/wiki/Double_spending). A blockchain has been described as a *value-exchange protocol*. A blockchain can maintain [title rights](https://en.wikipedia.org/wiki/Title_(property)) because, when properly set up to detail the exchange agreement, it provides a record that compels [offer and acceptance](https://en.wikipedia.org/wiki/Offer_and_acceptance).

> NOTE: 
>
> 一、翻译如下:
>
> "区块链是一种去中心化的、分布式的、通常是公开的数字分类帐，用于记录跨多台计算机的交易，以便在不改变所有后续区块的情况下，任何涉及的记录都不能被追溯性地更改。这允许参与者以相对较低的成本独立地验证和审计事务。区块链数据库使用点对点网络和分布式时间戳服务器进行自主管理。它们是由由集体自身利益驱动的大规模协作所验证的。这样的设计促进了健壮的工作流，参与者对于数据安全的不确定性微乎其微。区块链的使用消除了数字资产的无限复制特性。确认每单位价值只转移一次，解决了长期存在的双重支出问题。区块链被描述为“价值交换协议”。区块链可以维持所有权，因为当适当设置以详细说明交换协议时，它提供了一个强制要约和接受的记录。"

### [Blocks](https://en.wikipedia.org/wiki/Blockchain#Blocks)

Blocks hold batches of valid [transactions](https://en.wikipedia.org/wiki/Transaction_processing) that are hashed and encoded into a [Merkle tree](https://en.wikipedia.org/wiki/Merkle_tree). Each block includes the [cryptographic hash](https://en.wikipedia.org/wiki/Cryptographic_hash) of the prior block in the blockchain, linking the two. The linked blocks form a chain. This [iterative](https://en.wikipedia.org/wiki/Iteration) process confirms the integrity of the previous block, all the way back to the original **genesis block**（源头）. 

Sometimes separate blocks can be produced concurrently, creating a temporary fork. In addition to a secure hash-based history, any blockchain has a specified algorithm for scoring different versions of the history so that one with a higher score can be selected over others. Blocks not selected for inclusion in the chain are called **orphan blocks**. Peers supporting the **database** have different versions of the history from time to time. They keep only the **highest-scoring version** of the database known to them. Whenever a peer receives a **higher-scoring version** (usually the old version with a single new block added) they extend or overwrite their own **database** and retransmit the improvement to their peers. There is never an absolute guarantee that any particular entry will remain in the best version of the history forever. Blockchains are typically built to add the score of new blocks onto old blocks and are given incentives to extend with new blocks rather than overwrite old blocks. 

> NOTE: database中保存什么数据呢？是整个blockchain？

### [Decentralization](https://en.wikipedia.org/wiki/Blockchain#Decentralization)

> NOTE: 其实这一段所描述的peer-to-peer network的优势。



