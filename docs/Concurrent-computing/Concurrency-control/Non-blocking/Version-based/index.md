# 关于本章

本章标题"version-based concurrency control"的含义是"基于时间来进行并发控制"。将multiple version concurrency control收录进来的原因是: version也是表示的时间，它以简单的数字来表示时间含义。

## Why time can be used in concurrency control?

1、"time"是strict ordering的

2、可以使用timestamp来作为version ID

## Version-based concurrency control is optimistic

根据 `Optimistic-and-pessimistic` 章节的内容可知，version-based concurrency control is optimistic。