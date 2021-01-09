# CAP





## wikipedia [CAP theorem](https://en.wikipedia.org/wiki/CAP_theorem)



In [theoretical computer science](https://en.wikipedia.org/wiki/Theoretical_computer_science), the **CAP theorem**, also named **Brewer's theorem** after computer scientist [Eric Brewer](https://en.wikipedia.org/wiki/Eric_Brewer_(scientist)), states that it is impossible for a [distributed data store](https://en.wikipedia.org/wiki/Distributed_data_store) to simultaneously provide more than two out of the following three guarantees:[[1\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-Gilbert_Lynch-1)[[2\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-2)[[3\]](https://en.wikipedia.org/wiki/CAP_theorem#cite_note-3)

1、*Consistency*: Every read receives the most recent write or an error

> NOTE: 即不会丢失write，如果丢失了write，则可能导致inconsistency，其实这个consistency的定义非常类似于[6.3.1. Strict Consistency](https://www.e-reading.club/chapter.php/143358/217/Tanenbaum_-_Distributed_operating_systems.html) 







## CAP的直观理解

"鱼和熊掌不可兼得":

1) 之前在开发一个application的时候，涉及到在performance和reliability中进行抉择: 

如果实时(同步)地将记录写入到文件，则performance较差，reliability较好，这就是牺牲performance来获取reliability。

如果非实时(异步)地将记录写入到文件，则performance较好，reliability较差，这就是牺牲reliability来获取performance。



