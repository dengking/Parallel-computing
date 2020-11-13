# Blocking and non-blocking



## Blocking的广泛含义

"block"的原意是: "阻塞"，是指当前的流程被暂停了，它被暂停的原因有:

1) 等待耗时的操作完成

比如执行了一个非常耗时的操作。

2) 等待条件满足

比如等待lock。



1) process的blocked状态

OS中，process有blocked状态，显然是符合前面的描述的。