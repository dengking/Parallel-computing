# Event-driven model

本章描述[Event-driven model](https://dengking.github.io/Post/Abstraction/Abstraction-and-model/#event-driven-model)，它是一个非常强大的模型，能够描述非常非常广泛的问题。在本文，我们将说明什么是event-driven model、什么是event、如何实现event-driven model。

## Event-driven model概述

Event-driven model可以看做是一个**abstract machine**: 它需要持续不断地进行运转以**监控**事件源、收集event，一旦收集到了event，就进行dispatch，即根据event和event handler之间的映射关系，调用这个event对应的event handler。

### on

可以使用Event-driven model来进行描述的问题的一个典型的模式就是: "**当**某个事件(**event**)发生的时候，就执行某个函数(**event handler**)"。

现代programming language如何来描述这种模式呢？对比地看了一下，现代主流programming language都提供了**类似**的**programming model**来描述这种模式，比如:

1) JavaScript，参见工程programming的`JavaScript\Event-driven-model`章节

使用这些programming model能够让programmer方便地来描述这种模式。总的来说它依赖于如下programming language feature:

1) First class function

> 参见: 
>
> 1) Wikipedia [First-class function](https://infogalactic.com/info/First-class_function)
>
> 2) 工程programming-language的`Theory\Programming-paradigm\Functional-programming`章节

2) Fluent interface

> 参见: 
>
> 1) Wikipedia [Fluent interface](https://infogalactic.com/info/Fluent_interface)
>
> 2) 工程programming-language的`Theory\Programming-paradigm\Object-oriented-programming\Design-pattern\Fluent-API`章节



### Example

理解event-driven model的最最简单的例子就是Linux OS kernel和hardware的交互，在工程[Linux-OS](https://dengking.github.io/Linux-OS)的文章[Linux-OS-kernel-is-event-driven](https://dengking.github.io/Linux-OS/Kernel/Guide/Linux-OS's-interaction-with-the-hardware/Linux-OS-kernel-is-event-driven/)中我们已经总结了可以使用[Event-driven model](https://dengking.github.io/Post/Abstraction/Abstraction-and-model/#event-driven-model)来描述linux OS kernel和hardware的交互。在hardware层，一旦通电，则hardware就持续不断地运转起来了，一旦触发了interrupt，则它的**interrupt handler**就会被执行，Linux OS kernel维护了interrupt和interrupt handler之间的映射关系。

### Event-driven model/machine的component(组成)

一个event-driven model的组成成分：

| 组成部分                             | 是否必须 | 说明                                                         |
| ------------------------------------ | -------- | ------------------------------------------------------------ |
| monitor/listener                     | yes      | 监控事件源、收集event                                        |
| event和event handler之间的映射关系表 | yes      |                                                              |
| message passing                      | yes      | 传递信息，message queue/broker                               |
| dispatcher                           | yes      | 派发event，即按照event和event handler之间的映射关系，通知executor执行event handler |
| executor                             | yes      | 执行event handler                                            |

### See also

martinfowler [What do you mean by “Event-Driven”?](https://martinfowler.com/articles/201701-event-driven.html)

## What is event?

Event是一个非常概括、宽泛的概念，在computer science中，非常多的行为都会产生event，我们把它称为source of event，我们可以将source of event划分为两类：

### 1) external environment

我们以自底向上的思路来分析源自external environment的event，一个[computing system](https://dengking.github.io/Linux-OS/Architecture/Architecture-of-computing-system/)的最底层是hardware，hardware产生的[interrupt](https://en.wikipedia.org/wiki/Interrupt)，然后由OS kernel将这些interrupt“转换”为**signal**（现代programming language会使用**exception**来抽象**signal**）、IO event（因为IO的实现是依赖于interrupt的，IO包括了非常多的内容，用户操作、网络通信等都可以看做是IO，events can represent availability of new data for reading a file or network stream.）等，并通知到application process。

关于这一点，参见维基百科[Event (computing)](https://en.wikipedia.org/wiki/Event_(computing))的[Event handler](https://en.wikipedia.org/wiki/Event_(computing)#Event_handler)段。

### 2) 程序内部

Event可能源自于external environment，也可能源自于程序之内，即程序内部将一些条件等看做event，比如condition variable。

TODO: 需要补充一些具体例子。

下面补充了维基百科[Event (computing)](https://en.wikipedia.org/wiki/Event_(computing))来进行详细说明。

### See also: 

维基百科[Event (computing)](https://en.wikipedia.org/wiki/Event_(computing))



## Implementation of event-driven model

如何实现event-driven model？前面已经描述了even driven model的组成，显然我们需要分别实现这些component。由于event-driven model被广泛使用，它已经成为software engineering中的基础了，并且对于它研究已经非常成熟了，因此目前有非常多的、非常成熟的library，这在`./Library`章节进行了总结。

### IoC原则

非常适合使用IoC，参见`Software-design\Control-theory\Inversion-of-control`章节。

### Monitor：如何进行持续监控

在硬件层，只要通电后，则hardware就持续不断地运转起来了，一旦触发了interrupt，则它的interrupt handler就会被执行。在软件层，我们需要显式地使用一个 [event loop](./Event-loop/Event-loop.md) 来指示整个model/machine需要不断地运转下去。

关于这一点，参见维基百科[Event-driven programming](https://en.wikipedia.org/wiki/Event-driven_programming)的第二段。

### 建立event和event handler映射关系

Event-driven model肯定需要记录下event和event handler之间的映射关系，实现这种映射关系的方式是非常多的，可以显示地使用诸如map的数据结构。

下面罗列了一些实现案例：

| 案例                       | 说明                                                         |
| -------------------------- | ------------------------------------------------------------ |
| Interrupt Descriptor Table | 参见：<br>- 工程[Linux-OS](https://dengking.github.io/Linux-OS)的[4.2-Interrupts-and-Exceptions](https://dengking.github.io/Linux-OS/Kernel/Book-Understanding-the-Linux-Kernel/Chapter-4-Interrupts-and-Exceptions/4.2-Interrupts-and-Exceptions/)的[4.2.3. Interrupt Descriptor Table](https://dengking.github.io/Linux-OS/Kernel/Book-Understanding-the-Linux-Kernel/Chapter-4-Interrupts-and-Exceptions/4.2-Interrupts-and-Exceptions/#423-interrupt-descriptor-table) <br>- 维基百科[Interrupt descriptor table](https://en.wikipedia.org/wiki/Interrupt_descriptor_table) |
|                            |                                                              |



### Dispatcher

所谓的event dispatcher是指当event发生时，event-driven model将event传递到executor，通知executor执行对应的handler。Dispatcher的实现方式是有executor决定的， 在下一节对此进行具体情况具体说明。



### Executor: execution of event handler

如何来执行event handler？在决定如何来执行event handler的时候，开发者需要考虑如下问题：

1) event handler执行的成本，此处的成本可以有多种解释，比如，它可以表示event handler执行的时长、可以表示event handler执行的资源耗费

2) 并发性，同时发生的事件发生可能多，如何快速地处理这些事件呢？显然这就涉及了concurrency的问题，即并发地执行handler（event and concurrency）

所以开发者需要根据需求选择合适的实现方式。下面罗列一些执行方式：

|                   | Single process                                               | Multiple process                                             |
| ----------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| **Single thread** | monitor和executor位于同一个线程，这种比较适合event handler的执行成本比较小的情况 | monitor和executor分别处于两个不同的进程，这种情况dispatcher的实现显然涉及inter-process communication。<br>对于这种情况，可以将整体看做是一个event-driven system，也可以看做是多个event-driven system进行pipeline |
| **Multi thread**  | monitor和executor分别处于两个不同的线程，这种情况dispatcher的实现显然涉及到inter-thread communication |                                                              |





## 维基百科[Event-driven programming](https://en.wikipedia.org/wiki/Event-driven_programming)

In [computer programming](https://en.wikipedia.org/wiki/Computer_programming), **event-driven programming** is a [programming paradigm](https://en.wikipedia.org/wiki/Programming_paradigm) in which the [flow of the program](https://en.wikipedia.org/wiki/Control_flow) is determined by [events](https://en.wikipedia.org/wiki/Event_(computing)) such as user actions ([mouse](https://en.wikipedia.org/wiki/Computer_mouse) clicks, key presses), [sensor](https://en.wikipedia.org/wiki/Sensor) outputs, or [messages](https://en.wikipedia.org/wiki/Message_passing) from other programs or [threads](https://en.wikipedia.org/wiki/Thread_(computer_science)). Event-driven programming is the dominant paradigm used in [graphical user interfaces](https://en.wikipedia.org/wiki/Graphical_user_interface) and other applications (e.g., JavaScript [web applications](https://en.wikipedia.org/wiki/Web_application)) that are centered on performing certain actions in response to [user input](https://en.wikipedia.org/wiki/Input/output). This is also true of programming for [device drivers](https://en.wikipedia.org/wiki/Device_driver) (e.g., [P](https://en.wikipedia.org/wiki/P_(programming_language)) in USB device driver stacks).

In an event-driven application, there is generally a [main loop](https://en.wikipedia.org/wiki/Event_loop) that listens for events, and then triggers a [callback function](https://en.wikipedia.org/wiki/Callback_(computer_programming)) when one of those events is detected. In [embedded systems](https://en.wikipedia.org/wiki/Embedded_system), the same may be achieved using [hardware interrupts](https://en.wikipedia.org/wiki/Hardware_interrupt) instead of a constantly running main loop. Event-driven programs can be written in any [programming language](https://en.wikipedia.org/wiki/Programming_language), although the task is easier in languages that provide [high-level abstractions](https://en.wikipedia.org/wiki/Abstraction_(computer_science)), such as [await](https://en.wikipedia.org/wiki/Async/await) and [closures](https://en.wikipedia.org/wiki/Closure_(computer_programming)).

> NOTE: 维基百科的这篇文章对event-driven programming总结地非常好。

### Event handlers

Main article: [Event handler](https://en.wikipedia.org/wiki/Event_handler)

#### A trivial event handler

Because the code for checking for events and the [main loop](https://en.wikipedia.org/wiki/Main_loop) are common amongst applications, many programming frameworks take care of their implementation and expect the user to provide only the code for the event handlers. 

> NOTE: programming framework always do things common

In this simple example there may be a call to an event handler called `OnKeyEnter()` that includes an argument with a string of characters, corresponding to what the user typed before hitting the ENTER key. To add two numbers, storage outside the event handler must be used. The implementation might look like below.

### Common uses

In addition, systems such as Node.js are also event-driven.



## Pattern

参见`./Design-pattern`。

## Framework/library

参见 `./Library`。





## Case study

### Case: Linux OS kernel

参见: [Linux OS kernel is event-driven](https://dengking.github.io/Linux-OS/Kernel/Guide/Linux-OS's-interaction-with-the-hardware/Linux-OS-kernel-is-event-driven/)

## TODO

一些关乎event-driven model的内容：

- [Event (computing)](https://en.wikipedia.org/wiki/Event_(computing))

- [Event-driven programming](https://en.wikipedia.org/wiki/Event-driven_programming)

- [Event-driven architecture](https://en.wikipedia.org/wiki/Event-driven_architecture)

- [Event handler](https://en.wikipedia.org/wiki/Event_(computing)#Event_handler)

- [Interrupt](https://en.wikipedia.org/wiki/Interrupt)

- [Interrupt-driven](https://en.wikipedia.org/wiki/Interrupt)

- [Interrupt handler](https://en.wikipedia.org/wiki/Interrupt_handler)



