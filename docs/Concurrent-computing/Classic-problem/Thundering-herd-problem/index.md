# Thundering herd problem

"Thundering Herd" 的意思是 "惊群效应"。



## wanweibaike [Thundering herd problem](https://en.wanweibaike.com/wiki-Thundering%20herd%20problem)

In [computer science](https://en.wanweibaike.com/wiki-Computer_science), the **thundering herd problem** occurs when a large number of processes or threads waiting for an event are awoken when that event occurs, but only one process is able to handle the event. After all the processes wake up, they will start to handle the event, but only one will win. All processes will compete for resources, possibly freezing the computer, until the herd is calmed down again.[[1\]](https://en.wanweibaike.com/wiki-Thundering herd problem#cite_note-1)[[2\]](https://en.wanweibaike.com/wiki-Thundering herd problem#cite_note-2)



### Mitigation

The Linux-kernel will serialize(串行) responses for requests to a single file descriptor, so only one thread (process) is woken up.[[3\]](https://en.wanweibaike.com/wiki-Thundering herd problem#cite_note-3)



Similarly in Microsoft Windows, [I/O completion ports](https://en.wanweibaike.com/wiki-Input/output_completion_port) can mitigate the thundering herd problem, as they can be configured such that only one of the threads waiting on the completion port is woken up when an event occurs.[[4\]](https://en.wanweibaike.com/wiki-Thundering herd problem#cite_note-4)



## stackoverflow [Does the Thundering Herd Problem exist on Linux anymore?](https://stackoverflow.com/questions/2213779/does-the-thundering-herd-problem-exist-on-linux-anymore)



### A

For years, most unix/linux kernels serialize response to accept(2)s, in other words, only one thread is waken up if more than one are blocking on accept(2) against a single open file descriptor.

OTOH, many (if not all) kernels still have the thundering herd problem in the select-accept pattern as you describe.

I have written a simple script ( https://gist.github.com/kazuho/10436253 ) to verify the existence of the problem, and found out that the problem exists on linux 2.6.32 and Darwin 12.5.0 (OS X 10.8.5).





## cnblogs [UNIX 历史问题 分布式系统的Thundering Herd效应 惊群效应](https://www.cnblogs.com/rsapaper/p/10835868.html)
