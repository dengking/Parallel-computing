# [Nginx](http://nginx.org/en/docs/beginners_guide.html)

nginx has one master process and several worker processes. The main purpose of the master process is to read and evaluate configuration, and maintain worker processes. Worker processes do actual processing of requests. nginx employs event-based model and OS-dependent mechanisms to efficiently distribute requests among worker processes. The number of worker processes is defined in the configuration file and may be fixed for a given configuration or automatically adjusted to the number of available CPU cores (see [worker_processes](http://nginx.org/en/docs/ngx_core_module.html#worker_processes)).



[Nginx](https://en.wikipedia.org/wiki/Nginx)



# [Gunicorn](http://docs.gunicorn.org/en/latest/design.html)

Gunicorn is based on the pre-fork worker model. This means that there is a central master process that manages a set of worker processes. The master never knows anything about individual clients. All requests and responses are handled completely by worker processes.



# [Celery](http://docs.celeryproject.org/en/latest/userguide/workers.html)





# [Redis](https://redis.io/)





# [libuv](http://docs.libuv.org/en/v1.x/#) 



# [Tornado](http://www.tornadoweb.org/)



# simple http server

## multiplex

https://github.com/jeremycw/httpserver.h

## 多进程

https://github.com/pizhi/HttpServer



https://github.com/ethereum/lahja