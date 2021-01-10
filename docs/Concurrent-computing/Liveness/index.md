# Liveness

在阅读 wikipedia [Non-blocking algorithm](https://en.wikipedia.org/wiki/Non-blocking_algorithm#Wait-freedom) 时，发现的它；

我觉得它和以下内容有关:

1、wait-free

2、lock-free

3、obstruction-free

它们都与"progress"有关。

## wikipedia [Liveness](https://en.wikipedia.org/wiki/Liveness)

In [concurrent computing](https://en.wikipedia.org/wiki/Concurrent_computing), **liveness** refers to a set of properties of [concurrent systems](https://en.wikipedia.org/wiki/Concurrent_system), that require a system to make progress despite the fact that its concurrently executing components ("processes") may have to "take turns" in [critical sections](https://en.wikipedia.org/wiki/Critical_section), parts of the program that cannot be simultaneously run by multiple processes.[[1\]](https://en.wikipedia.org/wiki/Liveness#cite_note-1) Liveness guarantees are important properties in operating systems and [distributed systems](https://en.wikipedia.org/wiki/Distributed_systems).[[2\]](https://en.wikipedia.org/wiki/Liveness#cite_note-2)