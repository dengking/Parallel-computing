# zookeeper master slave failover



## cnblogs [zookeeper主备切换学习](https://www.cnblogs.com/chen45289/p/7820063.html)



## cnblogs [基于zookeeper的主备切换方法](https://www.cnblogs.com/aquester/p/9891538.html)

继承`CZookeeperHelper`即可快速实现主备切换：

zookeeper的`ZOO_EPHEMERAL`节点（如果`ZOO_EPHEMERAL`满足不了需求，可以考虑和`ZOO_SEQUENCE`结合使用），在会话关闭或过期时，会自动删除，利用这一**特性**可以实现两个或多节点间的主备切换。