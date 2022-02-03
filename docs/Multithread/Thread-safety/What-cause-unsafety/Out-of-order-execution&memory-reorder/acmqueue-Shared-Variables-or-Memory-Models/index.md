# [You Don’t Know Jack about Shared Variables or Memory Models](https://queue.acm.org/detail.cfm?id=2088916)

## Data races are evil.



### Incrementing A Counter



```C++
void incr()
{
  x++;
}
```



#### Using a *mutex*

```C++
void incr()
{
  mtx.lock();
  x++;
  mtx.unlock();
}
```



In Java, this might look like

```java
void incr()
{
  synchronized(mtx) {
    x++;
  }
}
```

or perhaps just

```java
synchronized void incr()
{
  x++;
}
```



#### Assembly

Those are the cases that are less surprising and easier to explain. The final count can also be too high. Consider a case in which the count is bigger than a machine word. To avoid dealing with binary numbers, assume we have a decimal machine in which each word holds three digits, and the counter `x` can hold six digits. The compiler translates `x++` to something like

```C++
tmp_hi = x_hi;
tmp_lo = x_lo;
(tmp_hi, tmp_lo)++;
x_hi = tmp_hi;
x_lo = tmp_lo;
```



### Another Racy Example

We’ve only begun to see the problems caused by data races. Here’s an example commonly tried in real code. One thread initializes a piece of data (say, `x`) and sets a flag (call it `done`) when it finishes. Any thread that later reads `x` first waits for the `done` flag, as in figure 1. What could possibly go wrong?



![](./boehm1.png)



This code may work reliably with a “dumb”(哑巴) compiler, but any “clever” optimizing compiler is likely to break it. When the compiler sees the loop, it is likely to observe that `done` is not modified in the loop (i.e., it is “loop-invariant”). Thus, it gets to assume that `done` does not change in the loop.

