# [High-Performance Server Architecture](http://pl.atyp.us/content/tech/servers.html)

The rest of this article is going to be centered around what I'll call the Four Horsemen of Poor Performance:

1、Data copies

2、Context switches

3、Memory allocation

4、Lock contention

There will also be a catch-all section at the end, but these are the biggest performance-killers. If you can handle most requests without copying data, without a context switch, without going through the memory allocator and without contending for locks, you'll have a server that performs well even if it gets some of the minor parts wrong.

