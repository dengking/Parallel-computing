# Memory barrier



## Wikipedia [Memory barrier](https://infogalactic.com/info/Memory_barrier)

A **memory barrier**, also known as a **membar**, **memory fence** or **fence instruction**, is a type of [barrier](https://infogalactic.com/info/Barrier_(computer_science)) [instruction](https://infogalactic.com/info/Instruction_(computer_science)) that causes a [central processing unit](https://infogalactic.com/info/Central_processing_unit) (CPU) or [compiler](https://infogalactic.com/info/Compiler) to enforce an [ordering](https://infogalactic.com/info/Memory_ordering) constraint on [memory](https://infogalactic.com/info/Random-access_memory) operations issued before and after the barrier instruction. This typically means that operations issued prior to the barrier are guaranteed to be performed before operations issued after the barrier.

> NOTE: memory barrier可以从如下角度来进行理解:
>
> 1) ordering and computational，参见工程discrete的`Relation-structure-computation\Make-it-computational`章节。
>
> 2) control theory: 由programmer来添加显式的控制从而使之有序

**Memory barriers** are necessary because most modern CPUs employ performance optimizations that can result in [out-of-order execution](https://infogalactic.com/info/Out-of-order_execution). This reordering of memory operations (loads and stores) normally goes unnoticed within a single [thread of execution](https://infogalactic.com/info/Thread_(computer_science)), but can cause unpredictable behaviour in [concurrent programs](https://infogalactic.com/info/Concurrent_computing) and [device drivers](https://infogalactic.com/info/Device_driver) unless carefully controlled. 

> NOTE: 关于 [out-of-order execution](https://infogalactic.com/info/Out-of-order_execution) ，参见工程hardware的`CPU\Execution-of-instruction\Out-of-order-execution`章节；
>
> memory instruction的out-of-order execution在Wikipedia memory ordering中进行了专门介绍，参见工程hardware的`CPU\Memory-access\Memory-ordering`章节。

The exact nature of an ordering constraint is hardware dependent and defined by the architecture's [memory ordering model](https://infogalactic.com/info/Memory_model_(programming)). Some architectures provide multiple barriers for enforcing different ordering constraints.

### An illustrative example

