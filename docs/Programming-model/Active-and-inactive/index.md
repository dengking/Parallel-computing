# 关于本章

polling是active，即主动；

notify是inactive，即被动；

## Active polling example

redis IO thread 

## Inactive notify example

spdlog

## Example

### 如何实现精准的时间控制？

线程调度时间也包括在内

polling：

不让thread接入sleeping状态，而是不断地检查时间

non-polling：sleep

https://www.softwaretestinghelp.com/cpp-sleep/

https://stackoverflow.com/questions/12823598/effect-of-usleep0-in-c-on-linux



基准的时间控制，让我想起了Nginx的实现；



bit mask需要进行总结



### spinlock VS mutex

参见 modernescpp [The Atomic Flag](https://www.modernescpp.com/index.php/the-atomic-flag)。