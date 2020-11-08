# Dataflow programming

在学习TensorFlow时，知道他是基于dataflow architecture的，所以对Dataflow programming进行了一些了解。

## wikipedia [Dataflow programming](http://en.wikipedia.org/wiki/Dataflow_programming)

In [computer programming](https://infogalactic.com/info/Computer_programming), **dataflow programming** is a [programming paradigm](https://infogalactic.com/info/Programming_paradigm) that models a program as a [directed graph](https://infogalactic.com/info/Directed_graph) of the data flowing between operations, thus implementing [dataflow](https://infogalactic.com/info/Dataflow) principles and architecture. 

> NOTE: TensorFlow就是典型的基于dataflow的。TensorFlow中的directed graph被称为computational graph。

Dataflow [programming languages](https://infogalactic.com/info/Programming_language) share some features of [functional languages](https://infogalactic.com/info/Functional_language), and were generally developed in order to bring some functional concepts to a language more suitable for **numeric processing**.

> NOTE: 本段所讨论的是dataflow language 和 functional language。

Some authors use the term Datastream instead of [Dataflow](https://infogalactic.com/info/Dataflow) to avoid confusion with Dataflow Computing or [Dataflow architecture](https://infogalactic.com/info/Dataflow_architecture), based on an indeterministic machine paradigm. 

> NOTE: [Dataflow architecture](https://infogalactic.com/info/Dataflow_architecture)是CPU architecture。

Dataflow programming was pioneered by [Jack Dennis](https://infogalactic.com/info/Jack_Dennis) and his graduate students at MIT in the 1960s.

### Properties of dataflow programming languages

Traditionally, a program is modeled as a series of operations happening in a specific order; this may be referred to as sequential,[[1\]](https://infogalactic.com/info/Dataflow_programming#cite_note-advances-1):p.3 procedural,[[2\]](https://infogalactic.com/info/Dataflow_programming#cite_note-lucid-2) [Control flow](https://infogalactic.com/info/Control_flow)[[2\]](https://infogalactic.com/info/Dataflow_programming#cite_note-lucid-2) (indicating that the program chooses a specific path), or [imperative programming](https://infogalactic.com/info/Imperative_programming)(命令式编程). The program focuses on commands, in line with the [von Neumann](https://infogalactic.com/info/John_von_Neumann)[[1\]](https://infogalactic.com/info/Dataflow_programming#cite_note-advances-1):p.3 vision of sequential programming, where data is normally "at rest"[[2\]](https://infogalactic.com/info/Dataflow_programming#cite_note-lucid-2):p.7

In contrast, dataflow programming emphasizes the movement of data and models programs as a series of connections. Explicitly defined inputs and outputs connect operations, which function like [black boxes](https://infogalactic.com/info/Black_box).[[2\]](https://infogalactic.com/info/Dataflow_programming#cite_note-lucid-2):p.2 An operation runs as soon as all of its inputs become valid.[[3\]](https://infogalactic.com/info/Dataflow_programming#cite_note-labview-3) Thus, dataflow languages are inherently parallel and can work well in large, decentralized systems.[[1\]](https://infogalactic.com/info/Dataflow_programming#cite_note-advances-1):p.3[[4\]](https://infogalactic.com/info/Dataflow_programming#cite_note-4) [[5\]](https://infogalactic.com/info/Dataflow_programming#cite_note-5)

> NOTE: 上面这段话对dataflow programming language和寻常的imperative programming的对比是非常好的。关于它们的差异，在工程machine-learning的`Programming\TensorFlow\Implementation`的`TensorFlow VS compiler`章节中也进行了讨论。



## wisegeek [What Is Dataflow Programming?](https://www.wisegeek.com/what-is-dataflow-programming.htm)





## Dataflow and stream

dataflow其实就是一种stream，TensorFlow的中，显然它的stream的单位是tensor。关于stream，参见工程discrete的`Relation-structure-computation\Model\Stream`章节。

## github topics [dataflow-programming](https://github.com/topics/dataflow-programming)

