# kjellhedstrom2 [Active Object the C++11 way](https://sites.google.com/site/kjellhedstrom2/active-object-with-cpp0x)

## Introduction

When Herb Sutter wrote about **Active Objects** in this Effective Concurrency blog series [blog entry: *[Prefer Using Active Objects Instead of Naked Threads](http://herbsutter.com/2010/07/12/effective-concurrency-prefer-using-active-objects-instead-of-naked-threads/)*] I was thrilled since I was waiting for that particular blog post since his **Effective Concurrency seminar**（研讨会） in Kista, Stockholm back in 2009. Herb explained then and later in his blog post a *cookbook* for getting your **Active Object** right. This could be done either with standard `C++` using an object oriented approach, by passing **message objects** to the **thread queue,** or by using `C++11` with lambda support and `std::thread` and other niceties.

> NOTE: 翻译如下:
>
> "当Herb Sutter在这个有效并发博客系列(博客条目:更喜欢使用活动对象而不是裸线程)中写关于活动对象的文章时，我很激动，因为自从2009年他在斯德哥尔摩Kista的有效并发研讨会之后，我就一直在等待那个特别的博客帖子。Herb后来在他的博客中解释了如何正确使用你的活动对象。这可以通过标准的c++使用面向对象的方法来实现，通过将消息对象传递到线程队列，或者通过使用c++ 11使用lambda支持和std::thread和其他细节。"



Since I thought the “*object oriented*” approach was clumsy and would lead to lots of [boilerplate](http://en.wikipedia.org/wiki/Boilerplate_(text)#Boilerplate_code) （样板）code I worked on a a more clean approach that would to use the features similar to `C++11` but using template magic for (*Gotw83*) [generic callbacks](http://www.gotw.ca/gotw/083.htm) and normal standard `C++` (without using `std::tr1::function`). I showed this in my [old blog](http://kjellkod.blogspot.com/2010/08/active-object.html) with standard `c++` code that can be downloaded from [kjellkod.cc/active_object](http://www.kjellkod.cc/active_object).

However, just recently I made a job move from [HiQ](http://hiq.se/en/) to [Prevas ](http://www.prevas.com/)and with that came a week or so between assignments. This gave me a great opportunity to learn more of the new `C++11` with the help of [just::thread](http://www.stdthread.co.uk/) implementation of the new `C++11` standard library. The result was much cleaner, more readable code with guaranteed cross-platform behavior using a mutex protected message queue for thread communication. This is a huge improvement over the *sketchy “l[ock-free circular fifo](http://www.codeproject.com/KB/threads/LockFree.aspx)”* I used in my first [Active Object example](http://www.kjellkod.cc/active_object).

For the new and improved Active Object I used standard library components and with the just::thread portability bundle I soon had a cross platform Active Object that works like a charm on both Windows (Visual Studio 2010 Express) and Linux Ubuntu (g++) .

Using **Git** you can download the **latest** snapshot of my Active Object example at `git://github.com/KjellKod/active-object.git` or you can download (top right) a possibly old snapshot.

The Active Object is also used in the popular and highly efficient asynchronous logger g2log. You can read about it [**here**](http://www.kjellkod.cc/g2log-efficient-background-io-processign-with-c11), and download its **[latest snapshot](https://bitbucket.org/KjellKod/g2log)** from BitBucket.

> NOTE:
>
> 1、稍微浏览了下面的code，写得一般





## Active Object .h

```C++
Below is the updated Active Object class. The createActive() function is a helper factory function to ensure that the object is fully created before running a thread on it's run() function.

typedef std::function<void()> Callback;

class Active {
private:
    Active(const Active&) = delete;
    Active& operator=(const Active&) = delete;

    Active(); // see: createActive()
    void doDone(){done = true;}
    void run();

    shared_queue<Callback> mq;
    std::thread thd;
    bool done; // flag for finishing
public:
    virtual ~Active();    
    void send(Callback msg_);

    // Factory: safe construction & thread start
    static std::unique_ptr<Active> createActive();
};
```

The main difference from my [first example](https://sites.google.com/site/kjellhedstrom2/active_object#TOC-Active-Object) apart from the queue is that here I use [std::function](http://en.wikipedia.org/wiki/C%2B%2B_Technical_Report_1#Function_objects) to represent the jobs in the work queue instread of a [home baked encapsuled generic callback](https://sites.google.com/site/kjellhedstrom2/active_object#TOC-Backgrounder-Generic-Callbacks)

## Active Object .cpp 

The implementation is very straight forward. The background thread will sleep until a producing thread puts more work onto the thread with send(...). When a new item is put onto the queue the thread is notified, wakes up and starts to process the new item(s).

At destruction a quit message is given from the destructor. This effectively makes sure that all messages are flushed and at the quit message the thread will exit.

```C++
Active::~Active() {
 Callback quit_token = std::bind(&Active::doDone, this);
 send(quit_token); // tell thread to exit
 thd_.join();
}

// Add asynchronously to queue

void Active::send(Callback msg_){

   mq.push(msg_);

}


// Will wait for msgs if queue is empty
void Active::run() {
 while (!done_) {
  // wait till job is available, then retrieve it and
  // executes the retrieved job in this thread (background)
  Callback func;
  mq_.wait_and_pop(func);
  func();
 }
}



// Factory: safe construction of object before thread start

std::unique_ptr<Active> Active::createActive(){

   std::unique_ptr<Active> aPtr(new Active());

   aPtr->thd = std::thread(&Active::run, aPtr.get());

   return aPtr;

}
```



