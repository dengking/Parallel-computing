# Async/await

"async"的含义是"asynchronous"，"await"的含义是"等待"。

## wikipedia [Async/await](https://en.wikipedia.org/wiki/Async/await)

In [computer programming](https://en.wikipedia.org/wiki/Computer_programming), the **async/await pattern** is a syntactic feature of many programming languages that allows an [asynchronous](https://en.wikipedia.org/wiki/Asynchrony_(computer_programming)), [non-blocking](https://en.wikipedia.org/wiki/Non-blocking_I/O) [function](https://en.wikipedia.org/wiki/Subroutine) to be structured in a way similar to an ordinary **synchronous function**. 

> NOTE: async/await其实是syntax sugar。
>
> asynchronous function的结果will be binded to a promise，参见下面的"Example C#"章节

It is semantically related to the concept of a [coroutine](https://en.wikipedia.org/wiki/Coroutine) (携程) and is often implemented using similar techniques, and is primarily intended to provide opportunities for the program to execute other code while waiting for a long-running, asynchronous task to complete, usually represented by [promises](https://en.wikipedia.org/wiki/Futures_and_promises) or similar data structures. 

> NOTE: 提高并发性

The feature is found in [C# 5.0](https://en.wikipedia.org/wiki/C_Sharp_5.0), [Python](https://en.wikipedia.org/wiki/Python_(programming_language)) 3.5, [Hack](https://en.wikipedia.org/wiki/Hack_(programming_language)), [Dart](https://en.wikipedia.org/wiki/Dart_(programming_language)), [Kotlin](https://en.wikipedia.org/wiki/Kotlin_(programming_language)) 1.1, [Rust](https://en.wikipedia.org/wiki/Rust_(programming_language)) 1.39,[[1\]](https://en.wikipedia.org/wiki/Async/await#cite_note-1) [Nim](https://en.wikipedia.org/wiki/Nim_(programming_language)) 0.9.4[[2\]](https://en.wikipedia.org/wiki/Async/await#cite_note-2) and [JavaScript](https://en.wikipedia.org/wiki/JavaScript) [ES2017](https://en.wikipedia.org/wiki/ES2017), with some experimental work in extensions, beta versions, and particular implementations of [Scala](https://en.wikipedia.org/wiki/Scala_(programming_language))[[3\]](https://en.wikipedia.org/wiki/Async/await#cite_note-scala-async-3) and [C++](https://en.wikipedia.org/wiki/C%2B%2B).

> NOTE: 现代主流语言都开始考虑支持它了

### Example C#

The [C#](https://en.wikipedia.org/wiki/C_Sharp_5.0) function below, which downloads a resource from a URI and returns the resource's length, uses this async/await pattern:

```C#
public async Task<int> FindPageSize(Uri uri) 
{
    byte[] data = await new WebClient().DownloadDataTaskAsync(uri);
    return data.Length;
}
```

1) First, the `async` keyword indicates to `C#` that the method is **asynchronous**, meaning that it may use an arbitrary number of `await` expressions and will **bind** the result to a [promise](https://en.wikipedia.org/wiki/Futures_and_promises).

> NOTE: 这段话的意思是 the `async` keyword 表面: 这个method是asynchronous的，意味着 这个method 可以使用 `await` expression。

2) The return type, `Task<T>`, is C#'s analogue(类似物) to the concept of a promise, and here is indicated to have a result value of type `int`.



3) The first expression to execute when this method is called will be `new WebClient().DownloadDataTaskAsync(uri)`, which is another **asynchronous method** returning a `Task<byte[]>`. Because this method is **asynchronous**, it will not download the entire batch of data before returning. Instead, it will begin the download process using a **non-blocking mechanism** (such as a [background thread](https://en.wikipedia.org/wiki/Thread_(computer_programming))), and immediately return an unresolved, unrejected `Task<byte[]>` to this function.

> NOTE: 立即返回而不阻塞



4) With the `await` keyword attached to the `Task`, this function will immediately proceed to return a `Task<int>` to its caller, who may then continue on with other processing as needed.

> NOTE: `int Length = await FindPageSize(Uri)`，从上面这段话我们可以推测: `await`的作用对象是promise即`Task`，它从promise取得结果给用户。



5) Once `DownloadDataTaskAsync()` finishes its download, it will resolve the `Task` it returned with the downloaded data. This will trigger a **callback** and cause `FindPageSize()` to continue execution by assigning that value to `data`.

> NOTE: 既然是asynchronous的，那么它必须要提供一种机制来保证当asynchronous operation完成时进行通知，通过上述描述来看，它也是通过callback来实现的。通过上述描述还可以看出，`C#`的实现其实保证了一定的顺序性的。



6) Finally, the method returns `data.Length`, a simple integer indicating the length of the array. The compiler re-interprets this as resolving the `Task` it returned earlier, triggering a **callback** in the method's caller to do something with that length value.



A function using `async`/`await` can use as many `await` expressions as it wants, and each will be handled in the same way (though(虽然) a **promise** will only be returned to the caller for the first await, while every other await will utilize(利用) internal callbacks). A function can also hold a **promise** object directly and do other processing first (including starting other **asynchronous tasks**), delaying awaiting the promise until its result is needed. Functions with promises also have promise aggregation methods that allow you to await multiple promises at once or in some special pattern (such as C#'s `Task.WhenAll()`, which returns a valueless `Task` that resolves when all of the tasks in the arguments have resolved). Many promise types also have additional features beyond what the `async`/`await` pattern normally uses, such as being able to set up more than one result callback or inspect the progress(进展) of an especially long-running task.



### In C++

In C++, await (named co_await in C++) has been officially merged into C++20 draft, so it is on course to be formally accepted as a part of official C++20;[[11\]](https://en.wikipedia.org/wiki/Async/await#cite_note-11) also [MSVC](https://en.wikipedia.org/wiki/MSVC) and [Clang](https://en.wikipedia.org/wiki/Clang) compilers are already supporting at least some form of co_await ([GCC](https://en.wikipedia.org/wiki/GNU_Compiler_Collection) still has no support for it).

```C++
#include <future>
#include <iostream>

using namespace std;

future<int> add(int a, int b)
{
    int c = a + b;
    co_return c;
}

future<void> test()
{
    int ret = co_await add(1, 2);
    cout << "return " << ret << endl;
}

int main()
{
    auto fut = test();
    fut.wait();

    return 0;
}
```