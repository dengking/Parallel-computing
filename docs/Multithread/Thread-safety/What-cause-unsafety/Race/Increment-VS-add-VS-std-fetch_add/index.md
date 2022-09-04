# Increment operator(++) VS add operator VS `fetch_add`



在 stackoverflow [what is the difference between ++, add operation and fetch_add() in atomic()](https://stackoverflow.com/questions/52069803/what-is-the-difference-between-add-operation-and-fetch-add-in-atomic) # [A](https://stackoverflow.com/a/52069837/10173843) 中说" Increment operator(++)" 是atomic的，但是在 

## stackoverflow [what is the difference between ++, add operation and fetch_add() in atomic()](https://stackoverflow.com/questions/52069803/what-is-the-difference-between-add-operation-and-fetch-add-in-atomic)

I ran following code many times but why the result for prefix increment , `fetch_add()` shows the correct result while with add operation (+), it prints the wrong result?

```cpp
#include <iostream>
#include <mutex>
#include <future>
using namespace std;
atomic<int> cnt(0);
void fun()
{
    for (int i = 0; i < 10000000; ++i)
    {
        // ++cnt; // print the correct result 20000000
        // cnt = cnt+1; // print wrong result, arbitrary numbers
        cnt.fetch_add(1); //  print the correct result 20000000
    }
}
int main()
{
    auto fut1 = async(std::launch::async, fun);
    auto fut2 = async(std::launch::async, fun);
    fut1.get();
    fut2.get();
    cout << "value of cnt: " << cnt << endl;
}
```



[A](https://stackoverflow.com/a/52069837/10173843)

`++cnt` and `cnt.fetch_add(1)` are truly atomic operations. One thread is blocked while the other thread reads, increments, and updates the value. As such, the two threads cannot step on each other's toes. Access to `cnt` is fully serialized, and the final result is as you would expect.

> NOTE:
>
> 一、上面这段话是错误的，`++cnt` 并不是atomic的，这在 stackoverflow [Can num++ be atomic for 'int num'?](https://stackoverflow.com/questions/39393850/can-num-be-atomic-for-int-num) 中进行了讨论
>
> 

## stackoverflow [Can num++ be atomic for 'int num'?](https://stackoverflow.com/questions/39393850/can-num-be-atomic-for-int-num)

In general, for `int num`, `num++` (or `++num`), as a read-modify-write operation, is **not atomic**. But I often see compilers, for example [GCC](https://en.wikipedia.org/wiki/GNU_Compiler_Collection), generate the following code for it ([try here](https://godbolt.org/g/UFKEvp)):

```c
void f()
{
  int num = 0;
  num++;
}
f():
        push    rbp
        mov     rbp, rsp
        mov     DWORD PTR [rbp-4], 0
        add     DWORD PTR [rbp-4], 1
        nop
        pop     rbp
        ret
```

Since line 5, which corresponds to `num++` is one instruction, can we conclude that `num++` **is atomic** in this case?



And if so, **does it mean that so-generated `num++` can be used in concurrent (multi-threaded) scenarios without any danger of data races** (i.e. we don't need to make it, for example, `std::atomic<int>` and impose the associated costs, since it's atomic anyway)?

**UPDATE**

Notice that this question is *not* whether increment *is* atomic (it's not and that was and is the opening line of the question). It's whether it *can* be in particular scenarios, i.e. whether one-instruction nature can in certain cases be exploited to avoid the overhead of the `lock` prefix. And, as the accepted answer mentions in the section about uniprocessor machines, as well as [this answer](https://stackoverflow.com/a/39414316/4973224), the conversation in its comments and others explain, **it can** (although not with C or C++).

**comments**

> NOTE:
>
> 一、memory ordering: 显然上述写法是不能够限制memory ordering的
>
> > given that one of the features of atomics is prevention of specific kinds of reordering during optimization, no, regardless of the atomicity of the actual operation – [jaggedSpire](https://stackoverflow.com/users/4892076/jaggedspire)
>
> 
>
> 二、`add`指令是atomic的吗？"Who told you that `add` is atomic? – [Slava](https://stackoverflow.com/users/432358/slava)" 答案是: 不是的，下面是两个非常好的回答: 
>
> 1、cache line
>
> > During the execution of that `add` instruction, another core could steal that memory address from this core's cache and modify it. On an x86 CPU, the `add` instruction needs a `lock` prefix if the address needs to be locked in cache for the duration of the operation. – [David Schwartz](https://stackoverflow.com/users/721269/david-schwartz)
>
> 上述回答非常到位，可以说是正解。
>
> 2、从`add` instruction执行过程进行说明
>
> stackoverflow [Can num++ be atomic for 'int num'?](https://stackoverflow.com/questions/39393850/can-num-be-atomic-for-int-num) # [A](https://stackoverflow.com/a/39394630/10173843)
>
> 
>
> 三、cross-platform
>
> > I would also like to point out that *if* this is atomic on your platform there is no guarantee that it will be on another pltaform. Be platform independent and express your intention by using a `std::atomic<int>`. – [NathanOliver](https://stackoverflow.com/users/4342498/nathanoliver)
>
> 

### [A](https://stackoverflow.com/a/39394630/10173843)

> NOTE: 
>
> 一、这个回答非常好的解释了"one-instruction-not-guarantee-atomic"

Without many complications an instruction like `add DWORD PTR [rbp-4], 1` is very CISC-style.

It perform three operations: load the operand from memory, increment it, store the operand back to memory.
During these operations the CPU acquire and release the bus twice, in between any other agent can acquire it too and this violates the atomicity.

```lua
AGENT 1          AGENT 2

load X              
inc C
                 load X
                 inc C
                 store X
store X
```

X is incremented only once.

### [A](https://stackoverflow.com/a/39396999)

This is absolutely what C++ defines as a Data Race that causes Undefined Behaviour, even if one compiler happened to produce code that did what you hoped on some target machine. You need to use `std::atomic` for reliable results, but you can use it with `memory_order_relaxed` if you don't care about reordering. 

> NOTE:
>
> 一、atomic 和 memory odering 是 atomic 的两个正交的熟悉

See below for some example code and asm output using `fetch_add`.

---

But first, the assembly language part of the question:

> Since num++ is one instruction (`add dword [num], 1`), can we conclude that num++ is atomic in this case?

**Memory-destination instructions (other than pure stores) are read-modify-write operations that happen in multiple internal steps**. No architectural register is modified, but the CPU has to hold the data internally while it sends it through its [ALU](https://en.wikipedia.org/wiki/Arithmetic_logic_unit). The actual register file is only a small part of the data storage inside even the simplest CPU, with latches holding outputs of one stage as inputs for another stage, etc., etc.

> NOTE:
>
> 一、上面这段话的第一句话其实是可以结合 stackoverflow [Can num++ be atomic for 'int num'?](https://stackoverflow.com/questions/39393850/can-num-be-atomic-for-int-num) # [@Margaret's answer](https://stackoverflow.com/a/39396999) 来进行理解的
>
> 二、上面这段话除了第一句其他的都不理解

Memory operations from other CPUs can become globally visible between the load and store. I.e. two threads running `add dword [num], 1` in a loop would step on each other's stores. (See [@Margaret's answer](https://stackoverflow.com/questions/39393850/can-num-be-atomic-for-an-int-num/39394630#39394630) for a nice diagram). After 40k increments from each of two threads, the counter might have only gone up by ~60k (not 80k) on real multi-core x86 hardware.

> NOTE:
>
> 一、"Memory operations from other CPUs can become globally visible between the load and store. I.e. two threads running `add dword [num], 1` in a loop would step on each other's stores." 这段话中的"visible"其实就意味着非atomic，它对应的是  [@Margaret's answer](https://stackoverflow.com/a/39396999)  的回答

---

**"Atomic", from the Greek word meaning indivisible, means that no observer can \*see\* the operation as separate steps. Happening physically / electrically instantaneously for all bits simultaneously is just one way to achieve this for a load or store, but that's not even possible for an ALU operation.** I went into a lot more detail about pure loads and pure stores in my answer to *[Atomicity on x86](https://stackoverflow.com/questions/38447226/atomicity-on-x86/38465341#38465341)*, while this answer focuses on read-modify-write.

The [`lock` prefix](http://www.felixcloutier.com/x86/LOCK.html) can be applied to many read-modify-write (memory destination) instructions to make the entire operation atomic with respect to all possible observers in the system (other cores and DMA devices, not an oscilloscope hooked up to the CPU pins). That is why it exists. (See also [this Q&A](https://stackoverflow.com/questions/8891067/what-does-the-lock-instruction-mean-in-x86-assembly/8891781#8891781)).

**So `lock add dword [num], 1` \*is\* atomic**. A CPU core running that instruction would keep the cache line pinned in Modified state in its private L1 cache from when the load reads data from cache until the store commits its result back into cache. This prevents any other cache in the system from having a copy of the cache line at any point from load to store, according to the rules of the [MESI cache coherency protocol](https://en.wikipedia.org/wiki/MESI_protocol) (or the MOESI/MESIF versions of it used by multi-core AMD/Intel CPUs, respectively). Thus, operations by other cores appear to happen either before or after, not during.