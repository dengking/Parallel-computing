# Cilk

在 阅读 ustc.edu [False Sharing](https://scc.ustc.edu.cn/zlsc/sugon/intel/compiler_c/main_cls/cref_cls/common/cilk_false_sharing.htm) 时，其中给出的 example使用 `cilk_spawn`、`cilk_sync`，从而使我发现了Cilk。



## wikipedia [Cilk](https://en.wikipedia.org/wiki/Cilk)

**Cilk**, **Cilk++** and **Cilk Plus** are [general-purpose](https://en.wikipedia.org/wiki/General-purpose_programming_language) [programming languages](https://en.wikipedia.org/wiki/Programming_language) designed for [multithreaded](https://en.wikipedia.org/wiki/Thread_(computer_science)) [parallel computing](https://en.wikipedia.org/wiki/Parallel_computing). They are based on the [C](https://en.wikipedia.org/wiki/C_(programming_language)) and [C++](https://en.wikipedia.org/wiki/C%2B%2B) programming languages, which they extend with constructs to express parallel loops and the [fork–join idiom](https://en.wikipedia.org/wiki/Fork–join_model).



## mit.edu [Programming in Cilk](https://cilk.mit.edu/programming/)

The Cilk programming language provides a simple extension to the C and C++ languages that allow programmers to expose logically parallel tasks. Cilk extends C and C++ with three keywords: `cilk_spawn`, `cilk_sync`, and `cilk_for`. This page describes the Cilk language extension.