# wikipedia [Happened-before](https://en.wikipedia.org/wiki/Happened-before)

For example, in some programming languages such as Java, C++ or Rust, a **happens-before** edge exists if memory written to by statement A is visible to statement B, that is, if statement A completes its write before statement B starts its read.

> NOTE: 
>
> 1、上述三种programming language的memory model都是基于happens-before relation而建立的

Like all strict partial orders, the happened-before relation is *[transitive](https://en.wikipedia.org/wiki/Transitive_relation)*, *[irreflexive](https://en.wikipedia.org/wiki/Irreflexive_relation)* and *[antisymmetric](https://en.wikipedia.org/wiki/Antisymmetric_relation)*, i.e.



The processes that make up a distributed system have no knowledge of the happened-before relation unless they use a [logical clock](https://en.wikipedia.org/wiki/Logical_clock), like a [Lamport clock](https://en.wikipedia.org/wiki/Lamport_clock) or a [vector clock](https://en.wikipedia.org/wiki/Vector_clock). This allows one to design algorithms for [mutual exclusion](https://en.wikipedia.org/wiki/Mutual_exclusion), and tasks like debugging or optimising distributed systems.

> NOTE: 
>
> 1、它是构建distributed system的基础
>
> 2、formal description 