# 关于本章

## TODO

### thread-safe and data consistency

这是在阅读 wikipedia [Multiversion concurrency control](https://en.wikipedia.org/wiki/Multiversion_concurrency_control) 时，其中的一段话:

> Without concurrency control, if someone is reading from a database at the same time as someone else is writing to it, it is possible that the reader will see a half-written or [inconsistent](https://en.wikipedia.org/wiki/Consistency_(database_systems)) piece of data.

让我想到了从data consistency的角度来对thread safe进行分析:

1、一个thread所写的和另外一个thread所读的，不一致(half-written or [inconsistent](https://en.wikipedia.org/wiki/Consistency_(database_systems)) piece of data)

2、两个thread看到的data不一致