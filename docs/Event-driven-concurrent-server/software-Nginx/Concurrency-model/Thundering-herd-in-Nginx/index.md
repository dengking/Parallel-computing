# Thundering herd in Nginx

在下面文章中，对Nginx解决"Thundering herd"的方法进行了解释:

nginx [Inside NGINX: How We Designed for Performance & Scale](https://www.nginx.com/blog/inside-nginx-how-we-designed-for-performance-scale/)

> The NGINX worker processes begin by waiting for events on the listen sockets ([accept_mutex](https://nginx.org/en/docs/ngx_core_module.html#accept_mutex) and [kernel socket sharding](https://www.nginx.com/blog/socket-sharding-nginx-release-1-9-1/)).

[Core functionality](https://nginx.org/en/docs/ngx_core_module.html) # [**accept_mutex**](https://nginx.org/en/docs/ngx_core_module.html#accept_mutex) 

> If `accept_mutex` is enabled, worker processes will accept new connections by turn. Otherwise, all worker processes will be notified about new connections, and if volume of new connections is low, some of the worker processes may just waste system resources.
>
> > There is no need to enable `accept_mutex` on systems that support the [EPOLLEXCLUSIVE](https://nginx.org/en/docs/events.html#epoll) flag (1.11.3) or when using [reuseport](https://nginx.org/en/docs/http/ngx_http_core_module.html#reuseport).
>
> 
>
> > Prior to version 1.11.3, the default value was `on`.

nginx [Socket Sharding in NGINX Release 1.9.1](https://www.nginx.com/blog/socket-sharding-nginx-release-1-9-1/)

