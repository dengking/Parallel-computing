# Actor proactor and reactor

在[GotW.ca: Herb Sutter](http://gotw.ca/)首页中，给出了**[Effective Concurrency: Know When to Use an Active Object Instead of a Mutex](http://www.drdobbs.com/go-parallel/article/showArticle.jhtml?articleID=227500074)**，其中的active object引起了我的注意。

在stackoverflow [To use Active object or not?](https://stackoverflow.com/questions/10237837/to-use-active-object-or-not)中总结对比了这三种pattern。

## Actor/Active object 

维基百科 [Active object](https://en.wikipedia.org/wiki/Active_object)

stackoverflow [To use Active object or not?](https://stackoverflow.com/questions/10237837/to-use-active-object-or-not)



https://sites.google.com/site/kjellhedstrom2/active-object-with-cpp0x



codeproject [A Technique for Active Object Aggregation in C++](https://www.codeproject.com/articles/9474/a-technique-for-active-object-aggregation-in-c)



herbsutter [Effective Concurrency: Prefer Using Active Objects Instead of Naked Threads](https://herbsutter.com/2010/07/12/effective-concurrency-prefer-using-active-objects-instead-of-naked-threads/)



### Implementation

[syscpp](https://github.com/lightful/syscpp)

## Proactor 