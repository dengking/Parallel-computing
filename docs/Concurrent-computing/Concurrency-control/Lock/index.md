# Lock

1、Lock最最常见的Mutual exclusion，本文对它进行详细说明。

2、Lock是pessimistic的，关于此，参见

## wikipedia [Mutual exclusion](https://en.wikipedia.org/wiki/Mutual_exclusion)

In [computer science](https://en.wikipedia.org/wiki/Computer_science), **mutual exclusion** is a property of [concurrency control](https://en.wikipedia.org/wiki/Concurrency_control), which is instituted for the purpose of preventing [race conditions](https://en.wikipedia.org/wiki/Race_condition). It is the requirement that one [thread of execution](https://en.wikipedia.org/wiki/Thread_(computing)) never enters its [critical section](https://en.wikipedia.org/wiki/Critical_section) at the same time that another [concurrent](https://en.wikipedia.org/wiki/Concurrent_computing) thread of execution enters its own critical section, which refers to an interval of time during which a thread of execution accesses a shared resource, such as [shared memory](https://en.wikipedia.org/wiki/Shared_memory_(interprocess_communication)).





## wikipedia [Lock (computer science)](https://en.wikipedia.org/wiki/Lock_(computer_science))





## TODO

https://stackoverflow.com/questions/2252452/does-a-getter-function-need-a-mutex

https://www.informit.com/articles/article.aspx?p=1750198&seqNum=3

https://stackoverflow.com/questions/17217268/c-sharp-is-locking-within-getters-and-setters-necessary

https://rules.sonarsource.com/cpp/RSPEC-1912

https://help.semmle.com/wiki/display/CSHARP/Inconsistently+synchronized+property

https://books.google.com/books?id=IgAICAAAQBAJ&pg=PA29&lpg=PA29&dq=should+getter+and+setter+be+locked&source=bl&ots=cSQCoTywkd&sig=ACfU3U03vlcYgKl29kmWcubdqe8q2sk-gQ&hl=en&sa=X&ved=2ahUKEwjnh4jtqrDoAhX-yosBHeKkAlEQ6AEwA3oECAgQAQ#v=onepage&q=should%20getter%20and%20setter%20be%20locked&f=false

