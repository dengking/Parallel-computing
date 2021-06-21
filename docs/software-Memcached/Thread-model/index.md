# 线程模型



## itpub [memcache的线程模型](http://blog.itpub.net/15480802/viewspace-1422565/)

MC采用一master多worker的工作模型，由master负责accept客户端请求，然后以RR分发给worker；

-t 线程数，用于处理请求，默认为4 
-b backlog队列长度，默认1024 

## calixwu [Memcached源码分析之线程模型](http://calixwu.com/2014/11/memcached-yuanmafenxi-xianchengmoxing.html)

