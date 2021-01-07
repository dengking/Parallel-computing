
如何来理解consistency呢？目前我在多种地方看到了consistency的介绍，如下：

- [一致性](https://baike.baidu.com/item/%E4%B8%80%E8%87%B4%E6%80%A7/9840083?fr=aladdin)
- [CAP theorem](https://en.wikipedia.org/wiki/CAP_theorem) 和 [Consistency (database systems)](https://en.wikipedia.org/wiki/Consistency_(database_systems))
- [Consistency model](https://en.wikipedia.org/wiki/Consistency_model) 和 [Consistency](https://en.wikipedia.org/wiki/Consistency)



因此，在理解[Consistency model](https://en.wikipedia.org/wiki/Consistency_model) 和 [CAP theorem](https://en.wikipedia.org/wiki/CAP_theorem) 之前，需要首先搞清楚它们的consistency的含义是什么？



# [CAP theorem](https://en.wikipedia.org/wiki/CAP_theorem)



In [theoretical computer science](https://en.wikipedia.org/wiki/Theoretical_computer_science), the **CAP theorem**, also named **Brewer's theorem** after computer scientist [Eric Brewer](https://en.wikipedia.org/wiki/Eric_Brewer_(scientist)), states that it is impossible for a [distributed data store](https://en.wikipedia.org/wiki/Distributed_data_store) to simultaneously provide more than two out of the following three guarantees:[[1\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-Gilbert_Lynch-1)[[2\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-2)[[3\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-3)

- *Consistency*: Every read receives the most recent write or an error

  ***SUMMARY*** : 即不会丢失write，如果丢失了write，则可能导致inconsistency，其实这个consistency的定义非常类似于[6.3.1. Strict Consistency](https://www.e-reading.club/chapter.php/143358/217/Tanenbaum_-_Distributed_operating_systems.html) 





# [一致性](https://baike.baidu.com/item/%E4%B8%80%E8%87%B4%E6%80%A7/9840083?fr=aladdin)

一致性就是数据保持一致，在分布式系统中，可以理解为多个节点中数据的值是一致的（即不会丢失数据）。



# wikipedia [Consistency](https://en.wikipedia.org/wiki/Consistency)

In [classical](https://en.wikipedia.org/wiki/Classical_logic) [deductive logic](https://en.wikipedia.org/wiki/Deductive_logic), a **consistent** [theory](https://en.wikipedia.org/wiki/Theory_(mathematical_logic)) is one that does not entail a [contradiction](https://en.wikipedia.org/wiki/Contradiction).[[1\]](https://en.wikipedia.org/wiki/Consistency#cite_note-1) The lack of contradiction can be defined in either semantic or syntactic terms. The semantic definition states that a theory is consistent if it has a [model](https://en.wikipedia.org/wiki/Model_theory#First-order_logic), i.e., there exists an [interpretation](https://en.wikipedia.org/wiki/Interpretation_(logic)) under which all [formulas](https://en.wikipedia.org/wiki/Well-formed_formula) in the theory are true. This is the sense used in traditional [Aristotelian logic](https://en.wikipedia.org/wiki/Term_logic), although in contemporary mathematical logic the term **satisfiable** is used instead. The syntactic definition states a theory $ T $ is consistent if there is no [formula](https://en.wikipedia.org/wiki/Formula_(mathematical_logic)) $ \varphi $ such that both $ \varphi $ and its negation $ \lnot \varphi $ are elements of the set of consequences of $ T $. Let $ A $ be a set of [closed sentences](https://en.wikipedia.org/wiki/Closed-form_expression) (informally "axioms") and $ \langle A\rangle $ the set of closed sentences provable from $ A $ under some (specified, possibly implicitly) formal deductive system. The set of axioms $ A $ is **consistent** when $ \varphi ,\lnot \varphi \in \langle A\rangle $ for no formula $ \varphi $.[[2\]](https://en.wikipedia.org/wiki/Consistency#cite_note-2)

If there exists a deductive system for which these semantic and syntactic definitions are equivalent for any theory formulated in a particular deductive [logic](https://en.wikipedia.org/wiki/Mathematical_logic#Formal_logical_systems), the logic is called **complete**.[*citation needed*] The completeness of the [sentential calculus](https://en.wikipedia.org/wiki/Sentential_calculus) was proved by [Paul Bernays](https://en.wikipedia.org/wiki/Paul_Bernays) in 1918[*citation needed*][[3\]](https://en.wikipedia.org/wiki/Consistency#cite_note-3) and [Emil Post](https://en.wikipedia.org/wiki/Emil_Post) in 1921,[[4\]](https://en.wikipedia.org/wiki/Consistency#cite_note-4) while the completeness of [predicate calculus](https://en.wikipedia.org/wiki/Predicate_calculus) was proved by [Kurt Gödel](https://en.wikipedia.org/wiki/Kurt_Gödel) in 1930,[[5\]](https://en.wikipedia.org/wiki/Consistency#cite_note-5) and consistency proofs for arithmetics restricted with respect to the [induction axiom schema](https://en.wikipedia.org/wiki/Mathematical_induction) were proved by Ackermann (1924), von Neumann (1927) and Herbrand (1931).[[6\]](https://en.wikipedia.org/wiki/Consistency#cite_note-6) Stronger logics, such as [second-order logic](https://en.wikipedia.org/wiki/Second-order_logic), are not complete.

A **consistency proof** is a [mathematical proof](https://en.wikipedia.org/wiki/Mathematical_proof) that a particular theory is consistent.[[7\]](https://en.wikipedia.org/wiki/Consistency#cite_note-7) The early development of mathematical [proof theory](https://en.wikipedia.org/wiki/Proof_theory) was driven by the desire to provide finitary consistency proofs for all of mathematics as part of [Hilbert's program](https://en.wikipedia.org/wiki/Hilbert's_program). Hilbert's program was strongly impacted by [incompleteness theorems](https://en.wikipedia.org/wiki/Gödel's_incompleteness_theorems), which showed that sufficiently strong proof theories cannot prove their own consistency (provided that they are in fact consistent).

Although consistency can be proved by means of model theory, it is often done in a purely syntactical way, without any need to reference some model of the logic. The [cut-elimination](https://en.wikipedia.org/wiki/Cut-elimination) (or equivalently the [normalization](https://en.wikipedia.org/wiki/Normalization_property) of the [underlying calculus](https://en.wikipedia.org/wiki/Curry-Howard) if there is one) implies the consistency of the calculus: since there is no cut-free proof of falsity, there is no contradiction in general.