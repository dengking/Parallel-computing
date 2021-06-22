# nginx [Socket Sharding in NGINX Release 1.9.1](https://www.nginx.com/blog/socket-sharding-nginx-release-1-9-1/)

[NGINX](https://nginx.org/en) 1.9.1 introduces a new feature that enables use of the `SO_REUSEPORT` socket option, which is available in newer versions of many operating systems, including DragonFly BSD and Linux (kernel version 3.9 and later). This socket option allows multiple sockets to listen on the same IP address and port combination. The kernel then load balances incoming connections across the sockets.

As depicted in the figure, when the `SO_REUSEPORT` option is not enabled, a single listening socket notifies workers about incoming connections, and each worker tries to take a connection.

![SO_REUSEPORT option is not enabled](https://cdn.wp.nginx.com/wp-content/uploads/2015/05/Slack-for-iOS-Upload-1-e1432652484191.png)

With the `SO_REUSEPORT` option enabled, there are multiple socket listeners for each IP address and port combination, one for each worker process. The kernel determines which available socket listener (and by implication, which worker) gets the connection. This can reduce lock contention between workers accepting new connections, and improve performance on multicore systems. However, it can also mean that when a worker is stalled by a blocking operation, the block affects not only connections that the worker has already accepted, but also connection requests that the kernel has assigned to the worker since it became blocked.

![SO_REUSEPORT option is enabled](https://cdn.wp.nginx.com/wp-content/uploads/2015/05/Slack-for-iOS-Upload-e1432652376641.png)