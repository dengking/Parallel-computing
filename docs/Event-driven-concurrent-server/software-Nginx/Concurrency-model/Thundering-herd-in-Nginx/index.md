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

> [NGINX](https://nginx.org/en) 1.9.1 introduces a new feature that enables use of the `SO_REUSEPORT` socket option, which is available in newer versions of many operating systems, including DragonFly BSD and Linux (kernel version 3.9 and later). This socket option allows multiple sockets to listen on the same IP address and port combination. The kernel then load balances incoming connections across the sockets.

nginx [Performance Tuning – Tips & Tricks](https://www.nginx.com/blog/performance-tuning-tips-tricks/)

> [`accept_mutex`](https://nginx.org/en/docs/ngx_core_module.html#accept_mutex) `off` – All worker processes are notified about new connections (the default in NGINX 1.11.3 and later, and NGINX Plus R10 and later). If enabled, worker processes accept new connections by turns.
>
> 
>
> We recommend keeping the default value (`off`) unless you have extensive knowledge of your app’s performance and the opportunity to test under a variety of conditions, but it can lead to inefficient use of system resources if the volume of new connections is low. Changing the value to `on` might be beneficial under some high loads.



## stackoverflow [Why is 'accept_mutex' 'on' as default in Nginx?](https://stackoverflow.com/questions/15636319/why-is-accept-mutex-on-as-default-in-nginx)



[A](https://stackoverflow.com/a/39584532)

As of nginx mainline version 1.11.3 (released 2016-07-26), [`accept_mutex` now defaults to `off`](https://nginx.org/en/docs/ngx_core_module.html#accept_mutex). This is partly because the new `EPOLLEXCLUSIVE` flag provides the benefits of `accept_mutex` without the extra overhead.

[A](https://stackoverflow.com/a/63562215)

Check the

- *accept Serialization - Multiple Sockets*
- *accept Serialization - Single Socket*

sections of this Apache documentation. https://httpd.apache.org/docs/2.4/misc/perf-tuning.html. (This doc matters most, compared to the rest of 2 I posted)

It's very inspiring and I also read the

http://nginx.org/en/docs/ngx_core_module.html#accept_mutex explaination as well as https://www.nginx.com/blog/performance-tuning-tips-tricks/ (search page with **accept_mutex**).

Aftering reading all of these, I guess Nginx is very similar to Apache in this aspect.
