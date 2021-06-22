# Nginx 



## [nginx documentation](http://nginx.org/en/docs/)



## [Beginner’s Guide](http://nginx.org/en/docs/beginners_guide.html)

nginx has one master process and several worker processes. The main purpose of the master process is to read and evaluate configuration, and maintain worker processes. Worker processes do actual processing of requests. nginx employs event-based model and OS-dependent mechanisms to efficiently distribute requests among worker processes. The number of worker processes is defined in the configuration file and may be fixed for a given configuration or automatically adjusted to the number of available CPU cores (see [worker_processes](http://nginx.org/en/docs/ngx_core_module.html#worker_processes)).

> NOTE: 
>
> master-worker 

#### Starting, Stopping, and Reloading Configuration

To start nginx, run the executable file. Once nginx is started, it can be controlled by invoking the executable with the `-s` parameter. Use the following syntax:

> ```shell
> nginx -s signal
> ```

Where *signal* may be one of the following:

1、`stop` — fast shutdown

2、`quit` — graceful shutdown

3、`reload` — reloading the configuration file

4、`reopen` — reopening the log files

> NOTE: 
>
> 这些请求都是通过OS signal的方式传送给process的