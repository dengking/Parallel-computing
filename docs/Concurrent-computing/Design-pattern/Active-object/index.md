# Active object pattern

## Herb Sutter [Effective Concurrency: Prefer Using Active Objects Instead of Naked Threads](https://herbsutter.com/2010/07/12/effective-concurrency-prefer-using-active-objects-instead-of-naked-threads/)

非常好的文章，已经收录了。

## wikipedia [Active object](https://en.wikipedia.org/wiki/Active_object)

 The **active object** [design pattern](https://en.wikipedia.org/wiki/Design_pattern) decouples **method execution** from **method invocation** for objects that each reside in their own [thread](https://en.wikipedia.org/wiki/Thread_(computing)) of control.[[1\]](https://en.wikipedia.org/wiki/Active_object#cite_note-1) The goal is to introduce [concurrency](https://en.wikipedia.org/wiki/Concurrency_(computer_science)), by using [asynchronous method invocation](https://en.wikipedia.org/wiki/Asynchronous_method_invocation) and a [scheduler](https://en.wikipedia.org/wiki/Scheduling_(computing)) for handling requests.[[2\]](https://en.wikipedia.org/wiki/Active_object#cite_note-2) 

> NOTE: 
>
> 1、结合下面的例子来看，上面这段话的意思其实是method invocation只是往queue中添加了一个request/message，method execution在另外一个thread中进行；
>
> 2、 这个名称的由来是什么？何为active？在 rosettacode [Active object](https://rosettacode.org/wiki/Active_object) 中对这个问题进行了解释
>
> 3、其实我编写的很多multithread application都可以归入到active object pattern的范畴

 The pattern consists of six elements: 

- A [proxy](https://en.wikipedia.org/wiki/Proxy_pattern), which provides an interface towards clients with publicly accessible methods.
- An interface which defines the method request on an active object.
- A list of pending requests from clients.
- A [scheduler](https://en.wikipedia.org/wiki/Scheduling_(computing)), which decides which request to execute next.
- The implementation of the active object method.
- A [callback](https://en.wikipedia.org/wiki/Callback_(computer_science)) or [variable](https://en.wikipedia.org/wiki/Variable_(computer_science)) for the client to receive the result.

### Example

> NOTE: 
>
> 1、原文给出的是Java的例子

## stackoverflow [Explain “Active-object” pattern](https://stackoverflow.com/questions/41676343/explain-active-object-pattern)

### comments

Active-object is the [Command Pattern](https://sourcemaking.com/design_patterns/command), implemented concurrently.

### [A](https://stackoverflow.com/a/41676344)

> NOTE: 
> 1、非常形象生动的比喻
>
> 2、结合下面的比喻来理解wikipedia [Active object](https://en.wikipedia.org/wiki/Active_object)中的内容

The Active object pattern's goal is to separate the method calling from method execution. It is like a waiter in a restaurant, who just hands the orders from the customers to the chef.

When a customer orders some food from the waiter

> the customer is the **Client**, the waiter is the **Proxy**

he writes it up on a paper (obviously doesn't start to cook it),

> the paper is the **MethodRequest**, the table's number on the paper is the **Future** object

gives the paper to the chef, who decides which cook should prepare it (who has time).

> the chef is the **Scheduler** who has a list of papers (**ActivationList**) and the cooks are the **Servant**-s

> NOTE: 
>
> 1、**ActivationList**其实对应的就是queue

When the meal is ready, the cook places it on the serve bar and the waiter brings it to the customers table.

> **Client** reads the method's result, from the **Result** object.

## Implementation

1、需要一个message queue。



## TODO

1、wikipedia [Active object](https://en.wikipedia.org/wiki/Active_object)

2、stackoverflow [To use Active object or not?](https://stackoverflow.com/questions/10237837/to-use-active-object-or-not)

3、https://sites.google.com/site/kjellhedstrom2/active-object-with-cpp0x







