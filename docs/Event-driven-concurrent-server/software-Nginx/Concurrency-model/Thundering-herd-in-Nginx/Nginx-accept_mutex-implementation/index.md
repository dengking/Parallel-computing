# `accept_mutex` 的实现

一、如何避免dead lock？

## jianshu [Nginx accept锁的机制和实现](https://www.jianshu.com/p/3bac42c50444)

> 本文基于Nginx 0.8.55源代码，并基于epoll机制分析

### 1.1 accpet锁是个什么东西

提到accept锁，就不得不提起惊群问题。

而在Linux内核的较新版本中，`accept`调用本身所引起的惊群问题已经得到了解决，但是在Nginx中，`accept`是交给`epoll`机制来处理的，`epoll`的`accept`带来的惊群问题并没有得到解决（应该是`epoll_wait`本身并没有区别读事件是否来自于一个Listen套接字的能力，所以所有监听这个事件的进程会被这个epoll_wait唤醒。），所以Nginx的accept惊群问题仍然需要定制一个自己的解决方案。

> NOTE: 
>
> 一、`epoll_wait` 导致的 thundering herd
>
> 二、参见 [epoll(7)](https://man7.org/linux/man-pages/man7/epoll.7.html) ，新版本的epoll中，已经解决这个问题了

accept锁就是nginx的解决方案，本质上这是一个跨进程的互斥锁，以这个互斥锁来保证只有一个进程具备监听accept事件的能力。

实现上accept锁是一个跨进程锁，其在Nginx中是一个全局变量，声明如下：



```c
ngx_shmtx_t           ngx_accept_mutex;
```

> NOTE: 
>
> 一、顾名思义，它会是使用shared memory IPC

这是一个在event模块初始化时就分配好的锁，放在一块进程间共享的内存中，以保证所有进程都能访问这一个实例，其加锁解锁是借由linux的原子变量来做CAS，如果加锁失败则立即返回，是一种非阻塞的锁。加解锁代码如下：



```c
static ngx_inline ngx_uint_t                                                   
ngx_shmtx_trylock(ngx_shmtx_t *mtx)                                            
{                                                                              
    return (*mtx->lock == 0 && ngx_atomic_cmp_set(mtx->lock, 0, ngx_pid));     
}                                                                              
                                                                               
#define ngx_shmtx_lock(mtx)   ngx_spinlock((mtx)->lock, ngx_pid, 1024)         
                                                                               
#define ngx_shmtx_unlock(mtx) (void) ngx_atomic_cmp_set((mtx)->lock, ngx_pid, 0)
```

可以看出，调用`ngx_shmtx_trylock`失败后会立刻返回而不会阻塞。

### 1.2 accept锁如何保证只有一个进程能够处理新连接

要解决epoll带来的accept锁的问题也很简单，只需要保证同一时间只有一个进程注册了accept的epoll事件即可。

Nginx采用的处理模式也没什么特别的，大概就是如下的逻辑：



```bash
尝试获取accept锁
if 获取成功：
    在epoll中注册accept事件
else:
    在epoll中注销accept事件
处理所有事件
释放accept锁
```

当然这里忽略了延后事件的处理，这部分我们放到后面讨论。

对于accept锁的处理和epoll中注册注销accept事件的的处理都是在`ngx_trylock_accept_mutex`中进行的。而这一系列过程则是在nginx主体循环中反复调用的`void ngx_process_events_and_timers(ngx_cycle_t *cycle)`中进行。

也就是说，每轮事件的处理都会首先竞争accept锁，竞争成功则在epoll中注册accept事件，失败则注销accept事件，然后处理完事件之后，释放accept锁。由此只有一个进程监听一个listen套接字，从而避免了惊群问题。

### 1.3 事件处理机制为不长时间占用accept锁作了哪些努力

accept锁处理惊群问题的方案看起来似乎很美，但如果完全使用上述逻辑，就会有一个问题：如果服务器非常忙，有非常多事件要处理，那么“处理所有事件这一步”就会消耗非常长的时间，也就是说，某一个进程长时间占用accept锁，而又无暇处理新连接；其他进程又没有占用accept锁，同样无法处理新连接——至此，新连接就处于无人处理的状态，这对服务的实时性无疑是很要命的。

为了解决这个问题，Nginx采用了将事件处理延后的方式。即在`ngx_process_events`的处理中，仅仅将事件放入两个队列中：



```c
ngx_thread_volatile ngx_event_t  *ngx_posted_accept_events;                             ngx_thread_volatile ngx_event_t  *ngx_posted_events; 
```

返回后先处理`ngx_posted_accept_events`后立刻释放accept锁，然后再慢慢处理其他事件。

即`ngx_process_events`仅对`epoll_wait`进行处理，事件的消费则放到accept锁释放之后，来最大限度地缩短占有accept的时间，来让其他进程也有足够的时机处理accept事件。

那么具体是怎么实现的呢？其实就是在`static ngx_int_t ngx_epoll_process_events(ngx_cycle_t *cycle, ngx_msec_t timer, ngx_uint_t flags)`的flags参数中传入一个`NGX_POST_EVENTS`的标志位，处理事件时检查这个标志位即可。

这里只是避免了事件的消费对于accept锁的长期占用，那么万一epoll_wait本身占用的时间很长呢？这种事情也不是不可能发生。这方面的处理也很简单，epoll_wait本身是有超时时间的，限制住它的值就可以了，这个参数保存在`ngx_accept_mutex_delay`这个全局变量中。

下面放上`ngx_process_events_and_timers` 的实现代码，可以大概一观相关的处理：



```c
void                                                                           
ngx_process_events_and_timers(ngx_cycle_t *cycle)                              
{                                                                              
    ngx_uint_t  flags;                                                         
    ngx_msec_t  timer, delta;                                                  
              
    
    /* 省略一些处理时间事件的代码 */                                                                               
    // 这里是处理负载均衡锁和accept锁的时机                                    
    if (ngx_use_accept_mutex) {                                                
        // 如果负载均衡token的值大于0, 则说明负载已满，此时不再处理accept, 同时把这个值减一
        if (ngx_accept_disabled > 0) {                                         
            ngx_accept_disabled--;                                             
                                                                               
        } else {                                                               
            // 尝试拿到accept锁                                                
            if (ngx_trylock_accept_mutex(cycle) == NGX_ERROR) {                
                return;                                                        
            }                                                                  
                                                                               
            // 拿到锁之后把flag加上post标志，让所有事件的处理都延后            
            // 以免太长时间占用accept锁                                        
            if (ngx_accept_mutex_held) {                                       
                flags |= NGX_POST_EVENTS;                                      
                                                                               
            } else {                                                           
                if (timer == NGX_TIMER_INFINITE                                
                    || timer > ngx_accept_mutex_delay)                         
                {                                                              
                    timer = ngx_accept_mutex_delay; // 最多等ngx_accept_mutex_delay个毫秒，防止占用太久accept锁
                }                                                              
            }                                                                  
        }                                                                      
    }                                                                                
    delta = ngx_current_msec;                                                  
                                                                               
    // 调用事件处理模块的process_events，处理一个epoll_wait的方法              
    (void) ngx_process_events(cycle, timer, flags);                            
                                                                               
    delta = ngx_current_msec - delta; //计算处理events事件所消耗的时间         
                                                                               
    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,                         
                   "timer delta: %M", delta);                                  
                                                                               
    // 如果有延后处理的accept事件，那么延后处理这个事件                        
    if (ngx_posted_accept_events) {                                            
        ngx_event_process_posted(cycle, &ngx_posted_accept_events);            
    }                                                                          
                                                                               
    // 释放accept锁                                                            
    if (ngx_accept_mutex_held) {                                               
        ngx_shmtx_unlock(&ngx_accept_mutex);                                   
    }                                                                          
                                                                               
    // 处理所有的超时事件                                                      
    if (delta) {                                                               
        ngx_event_expire_timers();                                             
    }                                                                          
                                                                               
    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,                         
                   "posted events %p", ngx_posted_events);                     
                                                                               
    if (ngx_posted_events) {                                                   
        if (ngx_threaded) {                                                    
            ngx_wakeup_worker_thread(cycle);                                   
                                                                               
        } else {                                                               
            // 处理所有的延后事件                                              
            ngx_event_process_posted(cycle, &ngx_posted_events);               
        }                                                                      
    }                                                                          
}                                                                              
```

再来看看`ngx_epoll_process_events`的相关处理：



```c
        // 读事件                                                                                                                                                           
        if ((revents & EPOLLIN) && rev->active) {
            if ((flags & NGX_POST_THREAD_EVENTS) && !rev->accept) {
                rev->posted_ready = 1;

            } else {
                rev->ready = 1;
            }                                                                                                                                                               
            if (flags & NGX_POST_EVENTS) {
                queue = (ngx_event_t **) (rev->accept ?
                               &ngx_posted_accept_events : &ngx_posted_events);
                ngx_locked_post_event(rev, queue);
            } else {
                rev->handler(rev);
            }
        }                                                                                                                                                                   
        wev = c->write;

        // 写事件
        if ((revents & EPOLLOUT) && wev->active) {
            if (flags & NGX_POST_THREAD_EVENTS) {
                wev->posted_ready = 1;
            } else {
                wev->ready = 1;
            }

            if (flags & NGX_POST_EVENTS) {
                ngx_locked_post_event(wev, &ngx_posted_events);
            } else {
                wev->handler(wev);
            }
        }
```

处理也相对简单，如果拿到了accept锁，就会有`NGX_POST_EVENTS`标志那么就会放到相应的队列中。没有的话就会直接处理事件。



