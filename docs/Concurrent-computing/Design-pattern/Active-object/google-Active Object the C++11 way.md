# [Active Object the C++11 way](https://sites.google.com/site/kjellhedstrom2/active-object-with-cpp0x)

## Introduction

When Herb Sutter wrote about **Active Objects** in this Effective Concurrency blog series [blog entry: *[Prefer Using Active Objects Instead of Naked Threads](http://herbsutter.com/2010/07/12/effective-concurrency-prefer-using-active-objects-instead-of-naked-threads/)*] I was thrilled since I was waiting for that particular blog post since his **Effective Concurrency seminar**（研讨会） in Kista, Stockholm back in 2009. Herb explained then and later in his blog post a *cookbook* for getting your **Active Object** right. This could be done either with standard `C++` using an object oriented approach, by passing **message objects** to the **thread queue,** or by using `C++11` with lambda support and `std::thread` and other niceties.

当Herb Sutter在这个有效并发博客系列(博客条目:更喜欢使用活动对象而不是裸线程)中写关于活动对象的文章时，我很激动，因为自从2009年他在斯德哥尔摩Kista的有效并发研讨会之后，我就一直在等待那个特别的博客帖子。Herb后来在他的博客中解释了如何正确使用你的活动对象。这可以通过标准的c++使用面向对象的方法来实现，通过将消息对象传递到线程队列，或者通过使用c++ 11使用lambda支持和std::thread和其他细节。

***SUMMARY*** : 最后一段给出了两种实现方式

Since I thought the “*object oriented*” approach was clumsy and would lead to lots of [boilerplate](http://en.wikipedia.org/wiki/Boilerplate_(text)#Boilerplate_code) （样板）code I worked on a a more clean approach that would to use the features similar to `C++11` but using template magic for (*Gotw83*) [generic callbacks](http://www.gotw.ca/gotw/083.htm) and normal standard `C++` (without using `std::tr1::function`). I showed this in my [old blog](http://kjellkod.blogspot.com/2010/08/active-object.html) with standard `c++` code that can be downloaded from [kjellkod.cc/active_object](http://www.kjellkod.cc/active_object).

However, just recently I made a job move from [HiQ](http://hiq.se/en/) to [Prevas ](http://www.prevas.com/)and with that came a week or so between assignments. This gave me a great opportunity to learn more of the new `C++11` with the help of [just::thread](http://www.stdthread.co.uk/) implementation of the new `C++11` standard library. The result was much cleaner, more readable code with guaranteed cross-platform behavior using a mutex protected message queue for thread communication. This is a huge improvement over the *sketchy “l[ock-free circular fifo](http://www.codeproject.com/KB/threads/LockFree.aspx)”* I used in my first [Active Object example](http://www.kjellkod.cc/active_object).

For the new and improved Active Object I used standard library components and with the just::thread portability bundle I soon had a cross platform Active Object that works like a charm on both Windows (Visual Studio 2010 Express) and Linux Ubuntu (g++) .

Using **Git** you can download the **latest** snapshot of my Active Object example at git://github.com/KjellKod/active-object.git or you can download (top right) a possibly old snapshot.

The Active Object is also used in the popular and highly efficient asynchronous logger g2log. You can read about it [**here**](http://www.kjellkod.cc/g2log-efficient-background-io-processign-with-c11), and download its **[latest snapshot](https://bitbucket.org/KjellKod/g2log)** from BitBucket.



## Disclaimer

This example is heavily inspired by Herb Sutter's article mentioned above. If I got it right it's all thanks to him, if I didn't then I'm sure some finer points went past me ;) Either way this is just an example of how you could do it. Feel free to use it, change it, do whatever with it. If you liked it then it's always nice to [drop me a line](http://kjellkod.wordpress.com/2010/09/11/lets-try-the-new-c0x-type-of-active-object/). As usual all the code you can find here at KjellKod.cc is yours to use but comes with no guarantee whatsoever.





## Active Object .h

