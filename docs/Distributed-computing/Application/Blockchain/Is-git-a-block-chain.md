# [Is Git a Block Chain?](https://news.ycombinator.com/item?id=9436847)

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