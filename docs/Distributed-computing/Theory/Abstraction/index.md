# Abstraction in distributed computing

在distributed computing中，创建合适的**abstraction**是解决很多问题的关键，这个思想在Book-Designing-Data-Intensive-Applications的`CHAPTER 9 Consistency and Consensus`中进行了阐述，把握这个思想，是掌握distributed computing中各种algorithm的关键所在。在Book-Designing-Data-Intensive-Applications中，作者已经表达了这样的观点，下面是将原书中关于abstraction的一些章节的摘录，从这些内容基本上可以了解作者的思想:

1) `Chapter1. Reliable, Scalable, and Maintainable Applications#Simplicity: Managing Complexity` :

> NOTE: 原文这段的核心思想是: creat abstraction to remove accidental complexity. 

Making a system simpler does not necessarily mean reducing its functionality; it can also mean removing ***accidental complexity***. Moseley and Marks [32] define complexity as **accidental** if it is not inherent in the problem that the software solves (as seen by the users) but arises only from the implementation.

> NOTE: **accidental complexity**是指不是由问题本身引入的complexity，而是由implementation引入的complexity。

One of the best tools we have for removing accidental complexity is ***abstraction***. A good **abstraction** can hide a great deal of implementation detail behind a clean, simple-to-understand façade. A good abstraction can also be used for a wide range of different applications. Not only is this reuse more efficient than reimplementing a similar thing multiple times, but it also leads to higher-quality software, as quality improvements in the abstracted component benefit all applications that use it.

For example, **high-level programming languages** are **abstractions** that hide machine code, CPU registers, and syscalls. **SQL** is an **abstraction** that hides complex on-disk and in-memory data structures, concurrent requests from other clients, and inconsistencies after crashes. Of course, when programming in a high-level language, we are still using machine code; we are just not using it directly, because the programming language **abstraction** saves us from having to think about it.

However, finding good abstractions is very hard. In the field of **distributed systems**, although there are many good algorithms, it is much less clear how we should be packaging them into abstractions that help us keep the complexity of the system at a manageable level.

> NOTE: 找到一个good abstraction是非常困难的

Throughout this book, we will keep our eyes open for good **abstractions** that allow us to extract parts of a large system into well-defined, reusable components.



2) `Chapter1. Reliable, Scalable, and Maintainable Applications#Summary` :

> NOTE: 紧随上一段而来

**Maintainability** has many facets(面), but in essence it’s about making life better for the engineering and operations teams who need to work with the system. Good **abstractions** can help reduce complexity and make the system easier to modify and adapt for new use cases.

3) `CHAPTER 2 Data Models and Query Languages`:

> NOTE: level model，参见工程`software-engineering`的`Software-design\Principle\Abstraction`。

In a complex application there may be more intermediary(中间) levels, such as APIs built upon APIs, but the basic idea is still the same: each layer hides the complexity of the layers below it by providing a clean data model. These **abstractions** allow different groups of people—for example, the engineers at the database vendor and the application developers using their database—to work together effectively.



4) `CHAPTER 7 Transactions#Summary`

> NOTE: 核心思想是: Transactions are an **abstraction layer**.

Transactions are an **abstraction layer** that allows an application to pretend that certain concurrency problems and certain kinds of hardware and software faults don’t exist. A large class of errors is reduced down to a simple transaction abort, and the application just needs to try again.



5) `CHAPTER 8 The Trouble with Distributed Systems#Knowledge, Truth, and Lies#System Model and Reality`

> NOTE: 核心思想是: **system models** are abstractions

Many algorithms have been designed to solve distributed systems problems—for example, we will examine solutions for the **consensus** problem in Chapter 9. In order to be useful, these algorithms need to tolerate the various faults of distributed systems that we discussed in this chapter.

Algorithms need to be written in a way that does not depend too heavily on the details of the hardware and software configuration on which they are run. This in turn requires that we somehow formalize the kinds of faults that we expect to happen in a system. We do this by defining a **system model**, which is an **abstraction** that describes what things an algorithm may assume.





6) `CHAPTER 8 The Trouble with Distributed Systems#Knowledge, Truth, and Lies#System Model and Reality#Mapping system models to the real world`

> NOTE: 核心思想是:  abstraction is simplification of reality. 

However, when implementing an algorithm in practice, the messy facts of reality come back to bite you again, and it becomes clear that the **system model** is a simplified **abstraction** of reality.



7) `CHAPTER 9 Consistency and Consensus`

> NOTE: 这段所总结的是核心思想: 
>
> "The best way of building **fault-tolerant** systems is to find some **general-purpose abstractions** with useful guarantees, implement them once, and then let applications rely on those guarantees." 
>
> 

The best way of building **fault-tolerant** systems is to find some **general-purpose abstractions** with useful guarantees, implement them once, and then let applications rely on those guarantees. This is the same approach as we used with **transactions** in Chapter 7: by using a **transaction**, the application can pretend that there are no crashes (**atomicity**), that nobody else is concurrently accessing the database (**isolation**), and that storage devices are perfectly reliable (**durability**). Even though crashes, race conditions, and disk failures do occur, the **transaction abstraction** hides those problems so that the application doesn’t need to worry about them.

We will now continue along the same lines, and seek **abstractions** that can allow an application to ignore some of the problems with **distributed systems**. For example, one of the most important **abstractions** for distributed systems is **consensus**: that is, getting all of the nodes to agree on something.

But first we first need to explore the range of **guarantees** and **abstractions** that can be provided in a distributed system.

