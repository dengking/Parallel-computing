# Memory reclamation

"reclamation"的意思是"回收"。

在[libcds](https://github.com/khizmax/libcds)的readme中，提及了memory reclamation



## preshing [New Concurrent Hash Maps for C++](https://preshing.com/20160201/new-concurrent-hash-maps-for-cpp/) :

### Safe Memory Reclamation

All Junction maps rely on a form of safe memory reclamation known as QSBR, or [quiescent state-based memory reclamation](http://preshing.com/20160726/using-quiescent-states-to-reclaim-memory). QSBR could be described as a primitive garbage collector.