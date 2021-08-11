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



## stackoverflow [Why is Git not considered a “block chain”?](https://stackoverflow.com/questions/46192377/why-is-git-not-considered-a-block-chain)





## ycombinator [Is Git a Block Chain?](https://news.ycombinator.com/item?id=9436847)

​	

[ntonozzi](https://news.ycombinator.com/user?id=ntonozzi) [on Apr 25, 2015](https://news.ycombinator.com/item?id=9437021)

Both git repositories and bitcoin are specialized Merkle trees. Merkle trees are incredibly useful and general; they are used in many kinds of verification, especially of large chunks of data.

http://en.wikipedia.org/wiki/Merkle_tree





[ejholmes](https://news.ycombinator.com/user?id=ejholmes) [on Apr 25, 2015](https://news.ycombinator.com/item?id=9437300)

The power of the Merkle tree is pretty amazing. [IPFS](https://en.wikipedia.org/wiki/InterPlanetary_File_System) is a good example





[ngoldbaum](https://news.ycombinator.com/user?id=ngoldbaum) [on Apr 25, 2015](https://news.ycombinator.com/item?id=9437188)

I thought it was a directed acyclic graph. Care to share the distinction?

> NOTE: 指的是git



[petertodd](https://news.ycombinator.com/user?id=petertodd) [on Apr 25, 2015](https://news.ycombinator.com/item?id=9437421)

The terminology used in #bitcoin-wizards is to call it a **merkelized dag**, or merkle-dag.

Similarly, we also refer to "Merkelized Abstract Syntax Trees", a way of hashing code originally proposed by Pieter Wuille and Russel O'connor(1) that will probably be added to Bitcoin's scripting system eventually.

Pretty much any data structure can have hash functions added to it to "merkelize" it, producing an authenticated data structure: http://www.cs.umd.edu/~amiller/gpads/

1) [https://download.wpsoftware.net/bitcoin/wizards/2014-12-16.h...](https://download.wpsoftware.net/bitcoin/wizards/2014-12-16.html)







[jscottmiller](https://news.ycombinator.com/user?id=jscottmiller) [on Apr 25, 2015](https://news.ycombinator.com/item?id=9436947)

"Blockchain" is generally used to refer to systems that either 

1) use the sequence of blocks to model changes in custodianship or 

2) (more generally) enforce a set of rules governing the **correctness** of a given **block**.

That is, a block in the bitcoin blockchain is valid not only if its hash matches what one would expect **given** the included transactions, but that those transactions adhere to the rules of bitcoin. (No double-spends, no dust transactions, etc).

While there are data structures in a git commit that must be present and/or follow a particular set of semantics, git does not enforce anything about the _contents_ of those commits.

Another key distinction: blockchains seek **consensus**, whereas divergent forks in git repos are by design.

EDIT: I should probably not distinguish too much between consensus and rule enforcement, as those two are obviously intertwined. :)





[wmf](https://news.ycombinator.com/user?id=wmf) [on Apr 25, 2015](https://news.ycombinator.com/item?id=9436922)

Consensus seems like an essential property that distinguishes a blockchain from a Merkle tree, and git does not provide consensus.



[oleganza](https://news.ycombinator.com/user?id=oleganza) [on Apr 25, 2015](https://news.ycombinator.com/item?id=9438177)

I'll quote myself (http://blog.oleganza.com/post/85111558553/bitcoin-is-like):

Bitcoin is like Git: in Git (a distributed version control system) all your changes(commit) are organized in a **chain** protected by cryptographic hashes. If you trust the **latest hash**, you can get all the previous information (or any part of it) from any source and still verify that it is what you expect. Similarly, in Bitcoin, all transactions are organized in a **chain** (the blockchain) and once validated, no matter where they are stored, you can always trust any piece of blockchain by checking a chain of hashes that link to a hash you already trust. This naturally enables distributed storage and easy integrity checks.

Bitcoin is unlike Git in a way that everyone strives to work on a single branch. In Git everyone may have several branches and fork and merge them all day long. In Bitcoin one cannot “merge” forks. Blockchain is a actually a tree of transaction histories, but there is always one biggest branch (which has the value) and some accidental mini-branches (no more than one-two blocks long) that have no value at all. In Git **content** matters (regardless of the branch), in Bitcoin **consensus** matters (regardless of the content).