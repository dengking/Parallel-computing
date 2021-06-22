# nginx blog [Inside NGINX: How We Designed for Performance & Scale](https://www.nginx.com/blog/inside-nginx-how-we-designed-for-performance-scale/)

## Setting the Scene – The NGINX Process Model

![The NGINX (and NGINX Plus) master process spawns three types of child process: worker, cache manage, and cache loader. They used shared memory for caching, session persistence, rate limits, and logging.](https://www.nginx.com/wp-content/uploads/2015/06/infographic-Inside-NGINX_process-model.png)

## How Does NGINX Work?

Each worker process is single‑threaded and runs independently, grabbing new connections and processing them. The processes can communicate using shared memory for shared cache data, session persistence data, and other shared resources.

> NOTE: 
>
> IPC shared memory

## Inside the NGINX Worker Process

The NGINX worker processes begin by waiting for events on the listen sockets ([accept_mutex](https://nginx.org/en/docs/ngx_core_module.html#accept_mutex) and [kernel socket sharding](https://www.nginx.com/blog/socket-sharding-nginx-release-1-9-1/)). Events are initiated by new incoming connections. These connections are assigned to a *state machine* – the HTTP state machine is the most commonly used, but NGINX also implements state machines for stream (raw TCP) traffic and for a number of mail protocols (SMTP, IMAP, and POP3).