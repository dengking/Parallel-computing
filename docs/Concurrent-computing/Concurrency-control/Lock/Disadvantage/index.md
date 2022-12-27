# Disadvantages

1、加锁，则存在 "用户态" 到 "内核态"、blocking、context switch，相对而言，这是成本较高的

2、并行 变 串行

3、多核之间进行数据同步、cache coherence、bus traffic

尤其是当多个thread都使用同一个lock的时候。

## wikipedia [Lock (computer science) # Disadvantages](https://en.wikipedia.org/wiki/Lock_(computer_science)#Disadvantages)



## wikipedia [Non-blocking algorithm # Motivation](https://en.wikipedia.org/wiki/Non-blocking_algorithm#Motivation)



