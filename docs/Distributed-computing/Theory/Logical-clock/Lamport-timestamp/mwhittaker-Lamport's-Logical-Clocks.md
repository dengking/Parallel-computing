# mwhittaker [Lamport's Logical Clocks](https://mwhittaker.github.io/blog/lamports_logical_clocks/)

[Blog](https://mwhittaker.github.io/blog)

People use physical time to order events. For example, we say that an event at 8:15 AM occurs before an event at 8:16 AM. In distributed systems, physical clocks are not always precise, so we can't rely on physical time to order events. Instead, we can use **logical clocks** to create a partial or total ordering of events. This article explores the concept of and [an implementation](https://github.com/mwhittaker/clocks) of the logical clocks invented by Leslie Lamport in his seminal paper [Time, Clocks, and the Ordering of Events in a Distributed System](https://scholar.google.com/scholar?cluster=4892527405117123487).

## Math Refresher

Before we dive into time, clocks, or ordering, let's quickly review a bit of the underlying math.

### Sets

A **set** is an unordered collection of distinct things. For example, we can bundle the integers 3, 11, and 0 into a set denoted $\set{3, 11, 0}$. Or, we could throw the integer 42 into a set with only one element: $\set{42}$. Or, we could have the empty set $\set{}$. We say an element $a$ is a member of $A$ if $A$ contains $a$, and we denote this $a \in A$.

A **subset** $A$ of a set $B$ is a set whose elements are all members of $B$. We denote that $A$ is a subset of $B$ as $A \subseteq B$. For example: $\set{1, 2} \subseteq \set{1, 2, 3}$, $\set{} \subseteq \set{1, 2, 3}$, and $\set{1, 2, 3} \subseteq \set{1, 2, 3}$. A set $A$ is a **strict subset** of $B$, denoted $A \subset B$, if $A \subseteq B$ and $A \neq B$.

The **Cartesian product** of two sets $A$ and $B$, denoted $A \times B$, is the set of ordered pairs $(a, b)$ for every $a \in A$ and $b \in B$. For example, $\set{1, 2} \times \set{a, b, c} = \{$$(1, a)$$, $$(1, b)$$, $$(1, c)$$, $$(2, a)$$, $$(2, b)$$, $$(2, c)$$\}$.



### Relations

A **binary relation** $R$ on a set $A$ is a subset of $A \times A$. Similarly, a binary relation on a set $A$ and $B$ is a subset of $A \times B$. For example, here's a couple relations on $\set{1, 2}$ and $\set{a, b, c}$: $\set{(1, a), (2, c)set}$, $\set{(1, a), (2, b), (3, c)}$, $\set{}$, and $\set{(1, a), (1, b), (1, c)}$. We can denote $(a, b) \in R$ as $a\,R\,b$. Consider for example the familiar **less-than relation**, $\lt$, on the set of natural numbers. Two naturals $i$, and $j$ are in the relation $\lt$ if $i$ is less than $j$. We denote this $i \lt j$. Concretely, $\lt$ is the infinite set $\set{(0, 1), (0, 2), (0, 3), \ldots, (1, 2), (1, 3), \ldots}$.



### Partial and Total Orderings

An **irreflexive partial ordering** on a set $A$ is a relation on $A$ that satisfies three properties.

1. **irreflexivity**: $a \nless a$
2. **antisymmetry**: if $a \lt b$ then $b \nless a$
3. **transitivity**: if $a \lt b$ and $b \lt c$ then $a \lt c$

For example, the strict subset relation we saw earlier is an example of an irreflexive partial ordering. Here are some examples of the previous three properties being satisfied.

1. **irreflexivity**: $\set{1, 2, 3} \not\subset \set{1, 2, 3}$
2. **antisymmetry**: $\set{1, 2} \subset \set{1, 2, 3}$, so $\set{1, 2, 3} \not\subset \set{1, 2}$
3. **transitivity**: $\set{1} \subset \set{1, 2}$ and $\set{1, 2} \subset \set{1, 2, 3}$, so $\set{1} \subset \set{1, 2, 3}$

It's important to note that for some sets $a$ and $b$, $a \not\subset b$ and $b \not \subset a$. For example, $\set{1, 2} \not \subset \set{2, 3}$ and $\set{2, 3} \not\subset \set{1, 2}$. This is quite different than total orderings.

An **irreflexive total ordering** is a irreflexive partial ordering that satisifes another condition.

1. **totality**: if $a \neq b$ then $a \lt b$ or $b \lt a$.

For example, the "less-than" relation on natural numbers is an irreflexive total ordering. For all naturals $i$ and $j$ where $i \neq j$, $i \lt j$ or $j \lt i$.

## A Partial Ordering

**Physical time** forms a natural "happened-before" irreflexive partial ordering of events. If we consider two events $a$ and $b$, we can use physical time to know whether $a$ happened before $b$, $b$ happened before $a$, or the two are unrelated (i.e. they happened at the same time). Since **physical clocks** are imprecise, we can't use **physical time** in a **distributed system**, but we'd still like an irreflexive partial ordering of events.

We'll define such an ordering, but first, let's formalize our system a bit. Our distributed system consists of a set of processes that each execute their own set of events. There are three events each process can execute:

1. A local event
2. Sending a message to another process
3. Receiving a message from another process

The union of every process' events is the set of events we wish to **order**. We can visualize such a system using a space-time graph, such as the one in Figure 1 below. Each vertical line represents a process. Time flows forward as we traverse the graph upwards through the set of events represented as annotated points. If one process sends a message and another receives a message, the two are events are connected by a colored line.

![img](https://mwhittaker.github.io/blog/lamports_logical_clocks/assets/clock.svg)Figure 1

The system in Figure 1 has three processes: $A$, $B$, and $C$. Process $A$ has four events.

1. $A_0$: Process $A$ sends a message to process $B$
2. $A_1$: Process $A$ receives a message from process $B$
3. $A_2$: Process $A$ executes a local event
4. $A_3$: Process $A$ receives a message from process $B$

Now we can define our "happened-before" irreflexive partial ordering, denoted $\rightarrow$, as the smallest relation on events that satisfies the following three properties.

1. If $a$ and $b$ are events in the same process and $a$ happens before $b$ then $a \rightarrow b$. For example, $A_0 \rightarrow A_1$.
2. If a process sends a message as event $a$ and another process receives the message as event $b$, then $a \rightarrow b$. For example, $A_0 \rightarrow B_2$.
3. If $a \rightarrow b$ and $b \rightarrow c$, then $a \rightarrow c$ For example, $A_0 \rightarrow B_2$ and $B_2 \rightarrow B_4$ and $B_4 \rightarrow C_3$, so $A_0 \rightarrow C_3$.

Graphically, $a \rightarrow b$ if we can trace a path forward through time from $a$ to $b$. For example, we can show that $A_0 \rightarrow C_3$ by tracing from $A_0$ across the blue line to $B_2$ up to $B_4$ and across the purple line to $C_3$. This interpretation also makes it clear that some events such as $A_2$ and $B_3$ are not related because we can't trace a path forward through time from $A_2$ to $B_3$ or from $B_3$ to $A_2$. In terms of physical time, $A_2$ might happen before $B_3$, but our $\rightarrow$ relation is defined independent of **physical time**.



## Logical Clocks

Now let's introduce **clocks** to help us order events according to our $\rightarrow$ ordering. Unlike **physical clocks** which are physical entities that assign **physical times** to events, our **clocks** are simply a conceptualization of a mathematical function that assigns numbers to events. These numbers act as **timestamps** that help us order events. Since our **clocks** logically order **events**, rather than physically order them, we call them **logical clocks**.

More formally, each process $P_i$ has a **clock** $C_i$ which is a function from **events** to the **integers**. The timestamp of an event $e$ in $P_i$ is $C_i(e)$. The system clock, $C$, is also a function from events to the integers where $C(e) = C_i(e)$ when $e$ is an event in $P_i$. In Figure 1, timestamps are associated with horizontal dashed lines beginning at 0 and increasing by 1 forwards through time. For example, events $B_0$ through $B_7$ have timestamps $0$ through $7$.

We evaluate our **logical clocks** with the following correctness criterion known as the **Clock Condition**.

$( \forall a, b.\,a \rightarrow b \implies C(a) \lt C(b) )$

For example, $A_0 \rightarrow B_2$, so in order for a clock $C$ to satisfy the Clock Condition, it must be that $C(A_0) \lt C(B_2)$. Also note that it is *not* the case that $\forall a, b.\, C(a) \lt C(b) \implies a \rightarrow b$. Consider $A_2$ and $B_3$. $C(A_2) \lt C(B_3)$, yet $A_2$ and $B_3$ are not related according to our $\rightarrow$ ordering.

Given a system, we can construct a **logical clock** using a simple algorithm. Each process $P_i$ maintains a mutable timestamp $C_i$. When an event $e$ occurs, we let $C_i(e)$ be $C_i$ when $e$ occurs. Each process $P_i$ increments $C_i$ after every event in $P_i$. Also, when a process $P_i$ sends a message to $P_j$, it includes its timestamp $C_i$ with the message. When $P_j$ receives the message, it sets its timestamp $C_j$ to be greater than the received $C_i$. A concrete implementation of this algorithm is discussed below.



## A Total Ordering

Our algorithm generates a clock that is consistent with our $\rightarrow$ ordering according to the Clock Condition, but what if we want to totally order the events of a system? It's surprisingly simple! First, choose an arbitrary irreflexive total ordering of the processes, denoted $\Rightarrow$. For example, $\prec$ may be $\set{(A, B), (A, C), (B, C)}$. Now, we can define a total ordering of events using $C$ and our $\prec$ ordering as the smallest relation satisfying the following properties.

1. If $C(a) \lt C(b)$, then $a \Rightarrow b$
2. If $C(a) = C(b)$ and $a \prec b$, then $a \Rightarrow b$

Now, we can order previously unrelated events. For example, $A_1 \Rightarrow B_2$ and $A_2 \Rightarrow B_3$.



## Implementation

I implemented [Lamport's logical clocks in Python](https://github.com/mwhittaker/clocks)! The implementation provides a function `lamport.wind` which takes in a list of functions each of which represents a process. The functions take in a `Clock_i` instance that supports three methods: `send(n)`, `recv(n)`, and `local()`. `wind`returns a function which you can invoke to plot the space-time graphs we've been using to visualize the logical clocks. Here's a couple of examples!

```
import lamport

def f(clock):
    clock.send(1)
    clock.recv(1)

def g(clock):
    clock.recv(0)
    clock.send(0)

lamport.wind([f, g])()
```

![img](https://mwhittaker.github.io/blog/lamports_logical_clocks/assets/clock2.svg)

```
import lamport

def f(clock):
    clock.send(1)
    clock.send(2)
    clock.recv(1)
    clock.recv(2)

def g(clock):
    clock.send(0)
    clock.send(2)
    clock.recv(0)
    clock.recv(2)

def h(clock):
    clock.send(0)
    clock.send(1)
    clock.recv(0)
    clock.recv(1)

lamport.wind([f, g, h])()
```

![img](https://mwhittaker.github.io/blog/lamports_logical_clocks/assets/clock3.svg)

```
import lamport

def f(clock):
    clock.send(1)
    clock.recv(1)
    clock.local()
    clock.recv(1)

def g(clock):
    clock.send(0)
    clock.send(2)
    clock.recv(0)
    clock.local()
    clock.send(2)
    clock.send(0)
    clock.local()
    clock.recv(2)

def h(clock):
    clock.local()
    clock.send(1)
    clock.recv(1)
    clock.recv(1)

lamport.wind([f, g, h], "clock.png")()
```

![img](https://mwhittaker.github.io/blog/lamports_logical_clocks/assets/clock.svg)