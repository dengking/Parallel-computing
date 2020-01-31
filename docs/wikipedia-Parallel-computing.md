[TOC]

# [Parallel computing](https://en.wikipedia.org/wiki/Parallel_computing)

**Parallel computing** is a type of [computation](https://en.wikipedia.org/wiki/Computing) in which many calculations or the execution of [processes](https://en.wikipedia.org/wiki/Process_(computing)) are carried out **simultaneously**. Large problems can often be divided into smaller ones, which can then be solved at the same time. 

> NOTE: 上述**Parallel computing**的定义是非常宽泛的，按照这个定义，将一个大问题拆解为小问题，然后**同时**解决这些小问题的都是**parallel computing**。显然，非常多的[computation](https://en.wikipedia.org/wiki/Computing)形态都可以归为parallel computing，参见下面的Types of parallelism。

There are several different forms of **parallel computing**: [bit-level](https://en.wikipedia.org/wiki/Bit-level_parallelism), [instruction-level](https://en.wikipedia.org/wiki/Instruction-level_parallelism), [data](https://en.wikipedia.org/wiki/Data_parallelism), and [task parallelism](https://en.wikipedia.org/wiki/Task_parallelism). 

> NOTE: 对于software engineer而言，更多的是关注 [task parallelism](https://en.wikipedia.org/wiki/Task_parallelism)。关于分类的详细内容，参见[Types of parallelism](https://en.wikipedia.org/wiki/Parallel_computing#Types_of_parallelism)章节。

**Parallelism** has long been employed in [high-performance computing](https://en.wikipedia.org/wiki/High-performance_computing), but it's gaining broader interest due to the physical constraints preventing [frequency scaling](https://en.wikipedia.org/wiki/Frequency_scaling). As power consumption (and consequently heat generation) by computers has become a concern in recent years, parallel computing has become the dominant paradigm in [computer architecture](https://en.wikipedia.org/wiki/Computer_architecture), mainly in the form of [multi-core processors](https://en.wikipedia.org/wiki/Multi-core_processor). 

**Parallel computing** is closely related to [concurrent computing](https://en.wikipedia.org/wiki/Concurrent_computing)—they are frequently used together, and often conflated, though the two are distinct: it is possible to have parallelism without concurrency (such as [bit-level parallelism](https://en.wikipedia.org/wiki/Bit-level_parallelism)), and concurrency without parallelism (such as **multitasking** by [time-sharing](https://en.wikipedia.org/wiki/Time-sharing) on a single-core CPU). In parallel computing, a computational task is typically broken down into several, often many, very similar **subtasks** that can be processed independently and whose results are combined afterwards, upon completion. In contrast, in concurrent computing, the various processes often do not address related tasks; when they do, as is typical in [distributed computing](https://en.wikipedia.org/wiki/Distributed_computing), the separate tasks may have a varied nature and often require some [inter-process communication](https://en.wikipedia.org/wiki/Inter-process_communication) during execution.

> NOTE：关于concurrent computing和parallelism之间的关联与差别，[这个回答](https://softwareengineering.stackexchange.com/questions/190719/the-difference-between-concurrent-and-parallel-execution)比较好：
>
> Concurrency and parallelism are two related but distinct concepts.
>
> Concurrency means, essentially, that task A and task B both need to happen independently of each other, and A starts running, and then B starts before A is finished.
>
> There are various different ways of accomplishing concurrency. One of them is parallelism--having multiple CPUs working on the different tasks at the same time. But that's not the only way. Another is by *task switching,* which works like this: Task A works up to a certain point, then the CPU working on it stops and switches over to task B, works on it for a while, and then switches back to task A. If the time slices are small enough, it may appear to the user that both things are being run in parallel, even though they're actually being processed in serial by a multitasking CPU.

Parallel computers can be roughly classified according to the level at which the **hardware** supports parallelism, with [multi-core](https://en.wikipedia.org/wiki/Multi-core) and [multi-processor](https://en.wikipedia.org/wiki/Symmetric_multiprocessing) computers having multiple [processing elements](https://en.wikipedia.org/wiki/Processing_element) within a single machine, while [clusters](https://en.wikipedia.org/wiki/Computer_cluster), [MPPs](https://en.wikipedia.org/wiki/Massively_parallel_(computing)), and [grids](https://en.wikipedia.org/wiki/Grid_computing) use multiple computers to work on the same task. Specialized parallel computer architectures are sometimes used alongside traditional processors, for accelerating specific tasks.

In some cases parallelism is transparent to the programmer, such as in bit-level or instruction-level parallelism, but explicitly [parallel algorithms](https://en.wikipedia.org/wiki/Parallel_algorithm), particularly those that use **concurrency**, are more difficult to write than sequential ones, because concurrency introduces several new classes of potential [software bugs](https://en.wikipedia.org/wiki/Software_bug), of which [race conditions](https://en.wikipedia.org/wiki/Race_condition) are the most common. [Communication](https://en.wikipedia.org/wiki/Computer_networking) and [synchronization](https://en.wikipedia.org/wiki/Synchronization_(computer_science)) between the different subtasks are typically some of the greatest obstacles（障碍) to getting good parallel program performance.

A theoretical [upper bound](https://en.wikipedia.org/wiki/Upper_bound) on the [speed-up](https://en.wikipedia.org/wiki/Speedup) of a single program as a result of parallelization is given by [Amdahl's law](https://en.wikipedia.org/wiki/Amdahl%27s_law).

## Background

To deal with the problem of power consumption and overheating the major [central processing unit](https://en.wikipedia.org/wiki/Central_processing_unit) (CPU or processor) manufacturers started to produce power efficient processors with multiple cores. The core is the computing unit of the processor and in multi-core processors each core is independent and can access the same memory concurrently. [Multi-core processors](https://en.wikipedia.org/wiki/Multi-core_processor) have brought parallel computing to [desktop computers](https://en.wikipedia.org/wiki/Desktop_computers). Thus parallelisation of serial programmes has become a mainstream programming task.

> 总结：从[Frequency scaling](https://en.wikipedia.org/wiki/Frequency_scaling)到[parallel computing](https://en.wikipedia.org/wiki/Parallel_computing)

### [Dependencies](https://en.wikipedia.org/wiki/Parallel_computing#Dependencies)

Understanding [data dependencies](https://en.wikipedia.org/wiki/Data_dependency) is fundamental in implementing [parallel algorithms](https://en.wikipedia.org/wiki/Parallel_algorithm). No program can run more quickly than the longest chain of dependent calculations (known as the [critical path](https://en.wikipedia.org/wiki/Critical_path_method)), since calculations that depend upon prior calculations in the chain must be executed in order. However, most algorithms do not consist of just a long chain of dependent calculations; there are usually opportunities to execute independent calculations in parallel.

总结：上述[critical path](https://en.wikipedia.org/wiki/Critical_path_method)的概念是非常重要的；

### [Race conditions, mutual exclusion, synchronization, and parallel slowdown](https://en.wikipedia.org/wiki/Parallel_computing#Race_conditions,_mutual_exclusion,_synchronization,_and_parallel_slowdown)

> NOTE: 这些内容在Unix-like-operating-system中已经包含了，此处省略。仅仅给出一些链接。

[Race condition](https://en.wikipedia.org/wiki/Race_condition)

[Lock (computer science)](https://en.wikipedia.org/wiki/Lock_(computer_science))

[Mutual exclusion](https://en.wikipedia.org/wiki/Mutual_exclusion)

[Critical section](https://en.wikipedia.org/wiki/Critical_section)

[Software lockout](https://en.wikipedia.org/wiki/Software_lockout)

[Deadlock](https://en.wikipedia.org/wiki/Deadlock)

[Non-blocking algorithm](https://en.wikipedia.org/wiki/Non-blocking_algorithm)

[Parallel slowdown](https://en.wikipedia.org/wiki/Parallel_slowdown)

[Barrier (computer science)](https://en.wikipedia.org/wiki/Barrier_(computer_science))

[Semaphore (programming)](https://en.wikipedia.org/wiki/Semaphore_(programming))

[Synchronization (computer science)](https://en.wikipedia.org/wiki/Synchronization_(computer_science))



### [Fine-grained, coarse-grained, and embarrassing parallelism](https://en.wikipedia.org/wiki/Parallel_computing#Fine-grained,_coarse-grained,_and_embarrassing_parallelism)

Applications are often classified according to how often their subtasks need to synchronize or communicate with each other. An application exhibits fine-grained parallelism if its subtasks must communicate many times per second; it exhibits coarse-grained parallelism if they do not communicate many times per second, and it exhibits [embarrassing parallelism](https://en.wikipedia.org/wiki/Embarrassingly_parallel) if they rarely or never have to communicate. Embarrassingly parallel applications are considered the easiest to parallelize.

### [Consistency models](https://en.wikipedia.org/wiki/Parallel_computing#Consistency_models)

*Main article:* [Consistency model](https://en.wikipedia.org/wiki/Consistency_model)

Parallel programming languages and parallel computers must have a [consistency model](https://en.wikipedia.org/wiki/Consistency_model) (also known as a memory model). The consistency model defines rules for how operations on [computer memory](https://en.wikipedia.org/wiki/Computer_data_storage) occur and how results are produced.

## [Types of parallelism](https://en.wikipedia.org/wiki/Parallel_computing#Types_of_parallelism)

### Bit-level parallelism

*Main article:* [Bit-level parallelism](https://en.wikipedia.org/wiki/Bit-level_parallelism)

### Instruction-level parallelism

*Main article:* [Instruction-level parallelism](https://en.wikipedia.org/wiki/Instruction-level_parallelism)

### Task parallelism

*Main article:* [Task parallelism](https://en.wikipedia.org/wiki/Task_parallelism)



## Hardware

### Memory and communication

> NOTE: 这一段内容与底层hardware关联，对于software engineer而言，可以跳过

### Classes of parallel computers

**Parallel computers** can be roughly classified according to the level at which the hardware supports parallelism. This classification is broadly analogous to the **distance** between basic **computing nodes**. These are not mutually exclusive; for example, clusters of symmetric multiprocessors are relatively common.

#### Multi-core computing

*Main article:* [Multi-core processor](https://en.wikipedia.org/wiki/Multi-core_processor)

A multi-core processor is a processor that includes multiple [processing units](https://en.wikipedia.org/wiki/Central_processing_unit) (called "cores") on the same chip. 

#### Symmetric multiprocessing

*Main article:* [Symmetric multiprocessing](https://en.wikipedia.org/wiki/Symmetric_multiprocessing)

A symmetric multiprocessor (SMP) is a computer system with multiple identical processors that share memory and connect via a bus.

#### Distributed computing

*Main article:* [Distributed computing](https://en.wikipedia.org/wiki/Distributed_computing)

A distributed computer (also known as a distributed memory multiprocessor) is a distributed memory computer system in which the processing elements are connected by a network. Distributed computers are highly scalable. The terms "[concurrent computing](https://en.wikipedia.org/wiki/Concurrent_computing)", "**parallel computing**", and "**distributed computing**" have a lot of overlap, and no clear distinction exists between them. The same system may be characterized both as "parallel" and "distributed"; the processors in a typical distributed system run concurrently in parallel. 

##### Cluster computing

*Main article:* [Computer cluster](https://en.wikipedia.org/wiki/Computer_cluster)

##### Massively parallel computing

*Main article:* [Massively parallel (computing)](https://en.wikipedia.org/wiki/Massively_parallel_(computing))

##### Grid computing

*Main article:* [Grid computing](https://en.wikipedia.org/wiki/Grid_computing)

#### Specialized parallel computers

Within parallel computing, there are specialized parallel devices that remain niche areas of interest. While not [domain-specific](https://en.wikipedia.org/wiki/Domain-specific_programming_language), they tend to be applicable to only a few classes of parallel problems.

## Software

### Parallel programming languages

*Main article:* [List of concurrent and parallel programming languages](https://en.wikipedia.org/wiki/List_of_concurrent_and_parallel_programming_languages)

## [Algorithmic methods](https://en.wikipedia.org/wiki/Parallel_computing#Algorithmic_methods)



## Fault tolerance

*Further information:* [Fault-tolerant computer system](https://en.wikipedia.org/wiki/Fault-tolerant_computer_system)

As parallel computers become larger and faster, we are now able to solve problems that had previously taken too long to run. Fields as varied as [bioinformatics](https://en.wikipedia.org/wiki/Bioinformatics) (for [protein folding](https://en.wikipedia.org/wiki/Protein_folding) and [sequence analysis](https://en.wikipedia.org/wiki/Sequence_analysis)) and economics (for [mathematical finance](https://en.wikipedia.org/wiki/Mathematical_finance)) have taken advantage of parallel computing. Common types of problems in parallel computing applications include: 

- Dense [linear algebra](https://en.wikipedia.org/wiki/Linear_algebra)
- Sparse linear algebra
- Spectral methods (such as [Cooley–Tukey fast Fourier transform](https://en.wikipedia.org/wiki/Cooley–Tukey_FFT_algorithm))
- [*N*-body problems](https://en.wikipedia.org/wiki/N-body_problem) (such as [Barnes–Hut simulation](https://en.wikipedia.org/wiki/Barnes–Hut_simulation))
- [structured grid](https://en.wikipedia.org/wiki/Regular_grid) problems (such as [Lattice Boltzmann methods](https://en.wikipedia.org/wiki/Lattice_Boltzmann_methods))
- [Unstructured grid](https://en.wikipedia.org/wiki/Unstructured_grid) problems (such as found in [finite element analysis](https://en.wikipedia.org/wiki/Finite_element_analysis))
- [Monte Carlo method](https://en.wikipedia.org/wiki/Monte_Carlo_method)
- [Combinational logic](https://en.wikipedia.org/wiki/Combinational_logic) (such as [brute-force cryptographic techniques](https://en.wikipedia.org/wiki/Brute_force_attack))
- [Graph traversal](https://en.wikipedia.org/wiki/Graph_traversal) (such as [sorting algorithms](https://en.wikipedia.org/wiki/Sorting_algorithm))
- [Dynamic programming](https://en.wikipedia.org/wiki/Dynamic_programming)
- [Branch and bound](https://en.wikipedia.org/wiki/Branch_and_bound) methods
- [Graphical models](https://en.wikipedia.org/wiki/Graphical_model) (such as detecting [hidden Markov models](https://en.wikipedia.org/wiki/Hidden_Markov_model) and constructing [Bayesian networks](https://en.wikipedia.org/wiki/Bayesian_network))
- [Finite-state machine](https://en.wikipedia.org/wiki/Finite-state_machine) simulation