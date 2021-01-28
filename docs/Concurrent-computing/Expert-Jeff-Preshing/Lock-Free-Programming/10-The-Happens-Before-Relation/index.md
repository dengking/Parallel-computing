# preshing [The Happens-Before Relation](https://preshing.com/20130702/the-happens-before-relation/)

> NOTE: 在cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order) 章节中，有关于happens-before的准确定义:
>
> > #### Happens-before
> >
> > Regardless of threads, evaluation A *happens-before* evaluation B if any of the following is true:
> >
> > 1) A is *sequenced-before* B
> >
> > 2) A *inter-thread happens before* B
>
> 在本文中，对这两种happens-before都有描述: 
>
> 1、"*Happens-Before* Does Not Imply Happening Before"章节描述的是sequenced-before的例子
>
> 2、"Happening Before Does Not Imply *Happens-Before*"章节描述的是*inter-thread happens before*的例子
>
> 

## What is *Happens-before* relation?

*Happens-before* is a modern computer science term which is instrumental in describing the [software memory models](http://preshing.com/20120930/weak-vs-strong-memory-models) behind C++11, Java, Go and even LLVM.

### Definition 

Roughly speaking, the common definition can be stated as follows:

> Let A and B represent operations performed by a multithreaded process. If A ***happens-before*** B, then the memory effects of A effectively become visible to the thread performing B before B is performed.

> NOTE: 
>
> 一、上面描述的其实是inter-thread happens before
>
> 二、上面这段话中"memory effects"、"visible"等其实还是比较抽象，我们初读还是不容易理解，那有什么具体案例吗？下面是案例:
>
> 1、在"Happening Before Does Not Imply *Happens-Before*( inter-thread happens before )"中，就列举了非常好的案例，在此我们可以简单先介绍一下: 
>
> a、 (2) 和 (3) 分别有不同的thread执行
>
> b、如果 (2) happens-before (3)  -蕴含-> (3) reads the value written by (2)
>
> 在下一篇 preshing [The Synchronizes-With Relation](https://preshing.com/20130823/the-synchronizes-with-relation/) 中，对 ***happens-before*** 的解释为:
>
> > safely propagate modifications from one thread to another once they’re complete. That’s where the ***synchronizes-with*** relation comes in
>
> 

When you consider the various ways in which memory reordering [can complicate](http://preshing.com/20120515/memory-reordering-caught-in-the-act) lock-free programming, the guarantee that A *happens-before* B is a desirable one. 

> NOTE: make it computational ordering

There are several ways to obtain this guarantee, differing slightly from one programming language to next – though obviously, all languages must rely on the same mechanisms at the processor level.

## Sequenced-before

> NOTE: 本节所描述的是Programmer从source code的角度来理解*Happens-before* relation，按照cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order) 中的说法，这其实是Sequenced-before。

No matter which programming language you use, they all have one thing in common: If operations A and B are performed by the same thread, and A’s statement comes before B’s statement in program order, then A *happens-before* B. This is basically a formalization of the “cardinal(基本的) rule of memory ordering” I [mentioned in an earlier post](http://preshing.com/20120625/memory-ordering-at-compile-time).

> NOTE: 
>
> 上面这段话中给出的最终结论"A *happens-before* B"是有前提的:
>
> 1、"operation A和operation B are performed by **the same thread**"
>
> 单线程执行，这是非常重要的一个前提
>
> 2、"A’s statement comes before B’s statement in program order"
>
> 在阅读了这段话后，我想到了compiler会执行optimization，可能造成 [instruction reordering](http://preshing.com/20120625/memory-ordering-at-compile-time)，那上面这段话中描述的岂不是错误的，后来反复思考，判断上面这段话并没有错误，原因如下:  
>
> 1、compiler的optimization是遵循AS-IF rule的，并不会更改这些基本的effect，也就是compiler optimization并不会改变happens-before relation，即实际的执行不会改变happens-before relation，因此可以认为上述断言是正确的；
>
> > NOTE: abstraction and optimization
>
> 2、这再此提醒了我们: happens-before relation是基于effect而定义的，而不是基于statement in programmer order而定义的，这样的设置，允许CPU、compiler进行最大化的optimization(只要它保证effect即可)，这个认知非常重要，下面的"*Happens-Before* Does Not Imply Happening Before"其实就是论证这个观点的最好的例子。compiler optimization后生成的code能够保证single thread的正确，但是并不能够保证multithread、multicore的正确；
>
> 3、sequenced-before是C++的默认行为，它是简单直观的: 顺序的，它让programmer能够从source code的角度对program的运行结果进行有效的推测；programmer推测的过程其实可以这样来理解:
>
> programmer模拟了一个abstract machine，这个abstract machine顺序执行我们的program，然后输出我们期望的结果
>
> 4、如果要实现inter-thread happens-before，需要由programmer添加额外的控制，作者在下一篇中进行了介绍

```C++
int A, B;

void foo()
{
    // This store to A ...
    A = 5;

    // ... effectively becomes visible before the following loads. Duh!
    B = A * A;
}
```

That’s not the only way to achieve a *happens-before* relation. The C++11 standard states that, among other methods, you also can achieve it between operations in different threads using [acquire and release semantics](http://preshing.com/20120913/acquire-and-release-semantics). I’ll talk about that more in the next post about [*synchronizes-with*](http://preshing.com/20130823/the-synchronizes-with-relation).

## *happens-before* VS happening before 

> NOTE: 
>
> *happens-before* 是基于effect而定义的，它不是基于time而定义的，不容易理解，与直觉相异
>
> happening before 是基于实际执行顺序而定义的，它是基于time而定义的，容易理解，与直觉相同
>
> 造成 "Ambiguity between the *happens-before* relation and the actual order of operations(happening before) "的根本原因是memory reordering，它可能发生于compile-time，也可能发生于run-time。
>
> 在下面的"Ambiguity between the *happens-before* relation and the actual order of operations(happening before) "的: 
>
> 1、"*Happens-Before* Does Not Imply Happening Before"
>
> 2、Happening Before Does Not Imply *Happens-Before*
>
> 其实就是对两者之间的Ambiguity进行非常详细的分析；
>
> 在" Ambiguity between the *happens-before* relation and the actual order of operations(happening before) " 章节，作者进行了总结

I’m pretty sure that the name of this relation may lead to confusion for some. It’s worth **clearing up**(澄清) right away: The *happens-before* relation, under the definition given above, is not the same as A actually happening before B! In particular,

1、A *happens-before* B does not imply A happening before B.

2、A happening before B does not imply A *happens-before* B.

### *happens-before* exists independently of the concept of time

> NOTE: 这段是对*happens-before*不是基于时间定义的进行说明，下面会给出例子进行论证

These statements might appear paradoxical(矛盾的、似是而非的), but they’re not. I’ll try to explain them in the following sections. Remember, *happens-before* is a formal relation between operations, defined by a family of language specifications; **it exists independently of the concept of time**. This is different from what we usually mean when we say that “A happens before B”; referring the order, in time, of real-world events. Throughout this post, I’ve been careful to always hyphenate(连字符号) the former term *happens-before*, in order to distinguish it from the latter.

## *Happens-Before* Does Not Imply Happening Before(Sequenced-before)

> NOTE: 作者本地给出了一个例子来论证"*Happens-Before* Does Not Imply Happening Before"；作者的这个例子展示了  [instruction reordering](http://preshing.com/20120625/memory-ordering-at-compile-time) ，虽然发生了  [instruction reordering](http://preshing.com/20120625/memory-ordering-at-compile-time) ，但是并没有违背 *Happens-Before* relation，`A`、`B` 的 value 还是被正确的更新了；这个例子比较牵强的地方在于: A 和 B之间并没有依赖关系；
>
> 这个例子让我认知了  [instruction reordering](http://preshing.com/20120625/memory-ordering-at-compile-time)  的广泛存在，联想 aristeia-C++and-the-Perils-of-Double-Checked-Locking ，显然在multithread、multicore的情况下，  [instruction reordering](http://preshing.com/20120625/memory-ordering-at-compile-time)  会造成诸多问题。

Here’s an example of operations having a *happens-before* relationship without actually happening in that order. The following code performs (1) a store to `A`, followed by (2) a store to `B`. According to the rule of program ordering, (1) *happens-before* (2).

```C++
int A = 0;
int B = 0;

void foo()
{
    A = B + 1;              // (1)
    B = 1;                  // (2)
}
```

However, if we compile this code with `-O2` using `GCC`, the compiler performs some [instruction reordering](http://preshing.com/20120625/memory-ordering-at-compile-time). As a result, when we step through the resulting code at the disassembly level in the debugger, we clearly see that after the second machine instruction, the store to `B` has completed, but the store to `A` has not. In other words, (1) doesn’t actually happen before (2)!

![img](https://preshing.com/images/foo-debugger.png)

Has the *happens-before* relation been violated? Let’s see. According to the definition, the memory effects of (1) must effectively be visible before (2) is performed. In other words, the store to `A` must have a chance to influence the store to `B`.

In this case, though, the store to `A` doesn’t actually influence the store to `B`. (2) still behaves the same as it would have even if the effects of (1) *had been* visible, which is *effectively* the same as (1)’s effects being visible. Therefore, this doesn’t count as a violation of the *happens-before* rule. I’ll admit, this explanation is a bit dicey(不准确的), but I’m fairly confident it’s consistent with the meaning of *happen-before* in all those language specifications.

## Happening Before Does Not Imply *Happens-Before*( inter-thread happens before )

> NOTE: Happening Before 并不能保证 happens-before

Here’s an example of operations which clearly happen in a specific order without constituting(构成) a *happens-before* relationship. In the following code, imagine that one thread calls `publishMessage`, while another thread calls `consumeMessage`. Since we’re manipulating shared variables concurrently, let’s keep it simple and assume that plain loads and stores of `int` are [atomic](http://preshing.com/20130618/atomic-vs-non-atomic-operations). Because of program ordering, there is a *happens-before* relation between (1) and (2), and another *happens-before* relation between (3) and (4).

> NOTE: 需要注意的是: 
>
> "Because of program ordering, there is a *happens-before* relation between (1) and (2), and another *happens-before* relation between (3) and (4)."
>
> 上面这段话中用的是*happens-before*，即effect；

```C++
int isReady = 0;
int answer = 0;

void publishMessage()
{
    answer = 42;                      // (1)
    isReady = 1;                      // (2)
}

void consumeMessage()
{
    if (isReady)                      // (3) <-- Let's suppose this line reads 1
        printf("%d\n", answer);       // (4)
}
```

### (2) and (3): 说明Happening Before Does Not Imply *Happens-Before*

> NOTE: 下面描述的(2) and (3)的场景就可以说明: Happening Before Does Not Imply *Happens-Before*

Furthermore, let’s suppose that at runtime, the line marked (3) ends up reading 1, the value that was stored at line (2) in the other thread. In this case, we know that (2) must have happened before (3). But that doesn’t mean there is a *happens-before* relationship between (2) and (3)!

> NOTE: 在这一段描述的场景中: (2) have happened before (3)，但是这并不意味"there is a *happens-before* relationship between (2) and (3)"。

The *happens-before* relationship only exists where the language standards say it exists. And since these are plain loads and stores, the C++11 standard has no rule which introduces a *happens-before* relation between (2) and (3), even when (3) reads the value written by (2).

> NOTE: 
>
> 1、看了上面这一段的最后一句话，我明白了happens-before的效果: 
>
> 如果 (2) happens-before (3)  -蕴含-> (3) reads the value written by (2)
>
> 2、上面这段话的意思是: 按照C++标准的定义，(2) and (3)之间并不存在happens-before关系，虽然实际运行效果"(3) reads the value written by (2)"，是和happens-before关系一致的，但是它并不能够说明(2) and (3)之间存在happens-before关系

### (1) and (4)

Furthermore, because there is no *happens-before* relation between (2) and (3), there is no *happens-before* relation between (1) and (4), either. Therefore, the memory interactions of (1) and (4) can be reordered, either due to compiler instruction reordering or memory reordering on the processor itself, such that (4) ends up printing “0”, even though (3) reads 1.

#### Access outside of object lifetime

上述(4) 如果read/load的值是0的话，其实就相当于"access outside of object lifetime"，即access了为初始化的value；

---

This post hasn’t really shown anything new. We already knew that [memory interactions can be reordered](http://preshing.com/20121019/this-is-why-they-call-it-a-weakly-ordered-cpu) when executing lock-free code. We’ve simply examined a term used in C++11, Java, Go and LLVM to formally specify the cases when memory reordering can be observed in those languages. Even [Mintomic](http://mintomic.github.io/), a library I published several weeks ago, relies on the guarantees of *happens-before*, since it mimics the behavior of specific C++11 atomic library functions.

## Ambiguity between the *happens-before* and happening before

I believe the ambiguity that exists between the *happens-before* relation and the actual order of operations is part of what makes low-level [lock-free programming](http://preshing.com/20120612/an-introduction-to-lock-free-programming) so tricky. If nothing else, this post should have demonstrated that *happens-before* is a useful guarantee which doesn’t come cheaply between threads. I’ll expand on that further in the next post.