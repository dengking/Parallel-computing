



## Wikipedia [Asynchrony (computer programming)](https://infogalactic.com/info/Asynchrony_(computer_programming))

> https://en.wikipedia.org/wiki/Asynchrony_(computer_programming)asynchronous 

**Asynchrony**, in [computer programming](https://infogalactic.com/info/Computer_programming), refers to the occurrence of events independently of the main [program flow](https://infogalactic.com/info/Control_flow) and ways to deal with such events. These may be "outside" events such as the arrival of [signals](https://infogalactic.com/info/Unix_signal), or actions instigated(发起的) by a program that take place [concurrently](https://infogalactic.com/info/Concurrent_computing) with program execution, without the program *blocking* to wait for results.[[1\]](https://infogalactic.com/info/Asynchrony_(computer_programming)#cite_note-davies-1) [Asynchronous input/output](https://infogalactic.com/info/Asynchronous_I/O) is an example of the latter cause(原因) of **asynchrony**, and lets programs issue commands to storage or network devices that service these requests while the [processor](https://infogalactic.com/info/Central_processing_unit) continues executing the program. Doing so provides a degree of [parallelism](https://infogalactic.com/info/Parallel_computing).[[1\]](https://infogalactic.com/info/Asynchrony_(computer_programming)#cite_note-davies-1)

> NOTE: event的出现是独立于 main program flow 的，这是asynchrony的本质含义；
>
> signal是 asynchronous event，并且它是典型的 outside event，它的出现是独立于main program flow的。
>
> "[Asynchronous input/output](https://infogalactic.com/info/Asynchronous_I/O) is an example of the latter cause(原因) of **asynchrony**"的意思是: asynchronous IO引发**asynchrony**的另外一个原因，因为IO完成的event是独立于 main program flow的
>
> 需要注意的是: 往往asynchrony意味着parallel。

