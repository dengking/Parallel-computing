[TOC]

# how to implement event loop based timer

首先源于我之前在JavaScript中使用过[`setInterval`](https://www.w3schools.com/jsref/met_win_setinterval.asp) ，而我现在的project中，有非常多基于功能是需要类似[`setInterval`](https://www.w3schools.com/jsref/met_win_setinterval.asp) 功能的，比如定时请求celery；不幸的是，我的project使用的是`c++`，所以我第一想法是：

Google setIntervel C++，从检索结果中，我记录了如下有价值的：

[Using setInterval() in C++](https://stackoverflow.com/questions/33234403/using-setinterval-in-c)

[timercpp](https://github.com/shalithasuranga/timercpp/blob/master/timercpp.h)

[A Simple Timer in C++](https://www.fluentcpp.com/2018/12/28/timer-cpp/)

单独创建一个thread来完成这个工作，该函数的工作非常简单，就是`setinterval`

[real time example of multithreading in c++](http://lac.linuxaudio.org/2012/papers/19.pdf)



刚开始， 我阅读的是[A Simple Timer in C++](https://www.fluentcpp.com/2018/12/28/timer-cpp/)，显然这是一个非常小型的，考虑的情况非常少；

接着阅读[Using setInterval() in C++](https://stackoverflow.com/questions/33234403/using-setinterval-in-c)，这其中对JavaScript的`setIntervel`实现进行了一些分析；







# [C++ std::thread Event Loop with Message Queue and Timer](https://www.codeproject.com/Articles/1169105/%2FArticles%2F1169105%2FCplusplus-std-thread-Event-Loop-with-Message-Queue)



# unix thread-based timer

<https://davejingtian.org/2013/09/28/timer-queue-a-way-to-handle-multiple-timers-using-one-thread/>

<https://github.com/daveti/tq>



# [Is it possible to direct linux timer notification signal to a specific thread?](https://stackoverflow.com/questions/26657744/is-it-possible-to-direct-linux-timer-notification-signal-to-a-specific-thread)



timer event loop implementation[Deeper understanding of event loops and timers [closed\]](https://softwareengineering.stackexchange.com/questions/254800/deeper-understanding-of-event-loops-and-timers)



# [The Node.js Event Loop, Timers, and `process.nextTick()`](https://nodejs.org/de/docs/guides/event-loop-timers-and-nexttick/#the-node-js-event-loop-timers-and-process-nexttick)



# [Deeper understanding of event loops and timers [closed\]](https://softwareengineering.stackexchange.com/questions/254800/deeper-understanding-of-event-loops-and-timers)





# how to

有两种定时：

- 每隔3S取一次行情
- 每隔0.5S，请求一次celery

这两者都需要有一个前提：当前时间在交易时间段内，那如何实现？

有多种方式：

方式1：上述两者分开来进行实现，分别搞一个线程，两个线程，都只需要执行`setInterval`，在每个interval中，都需要执行判断当前是否处于交易时间；

方式2：将两者放在同一个thread中，这个thread执行setInterval，在每个interval中，先取行情，然后再每隔0.5S，请求一次celery；



上述两种方式，都是blocking的，即timer和执行处于同一个thread；这总方式比较不好的就是timer的实现比较复杂，它需要计算在本interval中，所有操作的执行时间然后再来确定剩余的睡眠时间；更好的方式是non-blocking的，即将计时和超时动作分隔开来；





所以，一个问题一个问题来进行处理：

## 交易时间段

系统的时序应该是这样的：开市开始运行系统；系统开始运行，执行指定的操作；闭市停止系统；

但是目前的实现方式是：每次都去校验当前时间是否在交易时间；这种做法会导致编程的复杂，系统维护的困难；

比较好的做法是，有专门的定时线程，当其监控到，已经到达了开市时间时，则启动系统；当其监控到到达了闭市时间后，则停止系统；

实现这个功能，就可以有多种方式：可以使用纯`c++`的方法，也可以使用OS提供的方法；

### 纯`C++`方式

[How to create timer events using C++ 11?](https://stackoverflow.com/questions/14650885/how-to-create-timer-events-using-c-11)

这篇文章中的[这个答案](https://stackoverflow.com/a/14665230)是具有借鉴意义的，但是它并不适合我的project，因为它为每个timer event都创建一个thread，并不适合我的project；

[Issue when scheduling tasks using clock() function](https://stackoverflow.com/questions/11865460/issue-when-scheduling-tasks-using-clock-function)

这篇文章中的[这个答案](https://stackoverflow.com/a/11866539)比较适合我的project，但是需要对其进行改编，它主要的不足是：CPU不友好；可以参考[Portable periodic/one-shot timer implementation](https://codereview.stackexchange.com/questions/40473/portable-periodic-one-shot-timer-implementation)中的实现方式；





[C++11 way to create a timer that can be “stopped” if needed](https://stackoverflow.com/questions/24171395/c11-way-to-create-a-timer-that-can-be-stopped-if-needed)





### 使用OS提供的方法



#### [Is it possible to direct linux timer notification signal to a specific thread?](https://stackoverflow.com/questions/26657744/is-it-possible-to-direct-linux-timer-notification-signal-to-a-specific-thread)



I want to implement a timer handling module in my system using Linux POSIX timers API. A user can start a timer, and give a callback. Invocation of the callback will be done from a thread (and not from the signal handler).

I was thinking of setting `timer_create()` with `SIGEV_SIGNAL`, and the thread waiting for signals using `sigwaitinfo()`, (the signal arg will give the timerId which will invoke the required callback)

How does the signals are routed in the linux kernel? do I need to specify to which thread they are sent?



***COMMENT*** :

Did you look into [poll(2)](http://man7.org/linux/man-pages/man2/poll.2.html) and the Linux-specific [signalfd(2)](http://man7.org/linux/man-pages/man2/signalfd.2.html) & [timerfd_create(2)](http://man7.org/linux/man-pages/man2/timerfd_create.2.html) ? – [Basile Starynkevitch](https://stackoverflow.com/users/841108/basile-starynkevitch) [Oct 30 '14 at 17:20](https://stackoverflow.com/questions/26657744/is-it-possible-to-direct-linux-timer-notification-signal-to-a-specific-thread#comment41921002_26657744)



##### [A](https://stackoverflow.com/a/26659420)

By default a signal can be delivered to any thread, and you shouldn't make assumptions to which one. Posix threads, however, give you some control over this. You will normally use SIGALRM in conjunction with timer, but I have an example with SIGHUP, so I will show it here to demonstrate the workflow.

When your application starts the main thread sets global disposition of the signal to be ignored:

```c
int    n;
sigset_t    set;    
struct sigaction disp;

bzero(&disp, sizeof(disp));
disp.sa_handler = SIG_IGN;
if (sigaction(SIGHUP, &disp, NULL) < 0) {
 syslog(LOG_CRIT, "sigaction_main: %m");
 _exit(1);
}
```

Next, you make sure that the mask a newly spawned thread will inherit has also this signal blocked:

```c
sigemptyset(&set);
sigaddset(&set, SIGHUP);
if ( (errno = pthread_sigmask(SIG_BLOCK, &set, NULL)) != 0) {
 syslog(LOG_CRIT, "sigmask_main: %m");
 _exit(1);
}
```

At this point, you spawn all your threads that should not be interrupted by the signal (HUP). After this is all done, the main thread goes into dedicated mode of the signal waiting and handling:

```c
for (; ;) {
 if ( (errno = sigwait(&set, &n)) != 0) {
  syslog(LOG_CRIT, "sigwait_main: %m");
  _exit(1);
 }
 if (n == SIGHUP) {
  /* do the errands */
 }
}
```





#### [timer_create](http://man7.org/linux/man-pages/man2/timer_create.2.html)



#### [timerfd_create](http://man7.org/linux/man-pages/man2/timerfd_create.2.html)





# 多种实现方式

- 1取行情-》2算特征-》3保存到队列 全部都封装成一个函数，由线程池来进行执行

  线程池中的线程均分所有的标的券，即每个线程都有自己的特定的标的券；

  有多种实现方式：

  方式一：

  ```c++
  class QuotePredictWorker:
  	QuotePredict(int id, stocks){
      	    run_flag_=false;
      }
  	/// 调用此函数来启动worker_运行
  	void run(){
          run_flag_=true;
          cv_.notify_all();
      }
  	void task(){
          for stock in stocks:
          	get_quote();
          	calculate_feature();
      }
  	std::vector stocks_;//标的券
  	bool run_flag_;// condition variable
  	std::condition_variable cv_;
  	std::thread worker_;
  ```

  

- 一个线程池来处理取行情，一个线程池来计算特征

现在看来，我更加倾向于第一种方式

```

```

