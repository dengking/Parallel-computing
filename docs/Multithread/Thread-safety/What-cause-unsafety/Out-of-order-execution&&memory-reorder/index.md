# Out of order execution and memory reordering

由于Out of order execution(主要是memory reordering)也会对导致shared data线程不安全。

## Examples

参见工程hardware的`Memory-ordering` 章节。

## Order of write to shared data may be different among different threads

这是在阅读一篇文章时，其中提出的一个观点，我觉得这个观点比较好，在下面文章中，描述了相关的例子:

1、参见 "aristeia-C++and-the-Perils-of-Double-Checked-Locking" 章节

2、queue.acm [You Don’t Know Jack about Shared Variables or Memory Models](https://queue.acm.org/detail.cfm?id=2088916)





