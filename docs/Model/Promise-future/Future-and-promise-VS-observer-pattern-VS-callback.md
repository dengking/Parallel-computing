[Futures and promises](https://en.wikipedia.org/wiki/Futures_and_promises)

[Java executors: how to be notified, without blocking, when a task completes?](https://stackoverflow.com/questions/826212/java-executors-how-to-be-notified-without-blocking-when-a-task-completes)


[Futures are better than callbacks](https://www.jayconrod.com/posts/93/futures-are-better-than-callbacks)


[From Callback to Future -> Functor -> Monad](https://hackernoon.com/from-callback-to-future-functor-monad-6c86d9c16cb5)

callback vs promise vs observer pattern

https://itnext.io/javascript-promises-vs-rxjs-observables-de5309583ca2

https://scotch.io/courses/10-need-to-know-javascript-concepts/callbacks-promises-and-async

observer pattern允许开放式的注册、注销

callback则没有这些含义

两者都有“当。。。发生时，执行某个函数”

callback有它的优势，对于一些library，如以so形式提供的，则这种情况下，将其接口指定为一个callback是最最简单的，它就相当于一个function pointer。而这种情况，则不适合使用observer pattern，其实这种情况也是可以使用observer pattern的。

