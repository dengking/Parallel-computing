# [Active object](https://rosettacode.org/wiki/Active_object)

In [object-oriented programming](https://rosettacode.org/wiki/Object-oriented_programming) an object is **active** when its **state** depends on clock（随着时间的流逝，它的state可能会变更，这就是active的）. Usually an **active object** encapsulates a [task](https://rosettacode.org/wiki/Task) that updates the object's state. To the outer world the object looks like a normal object with methods that can be called from outside. Implementation of such methods must have a certain **synchronization mechanism** with the encapsulated task in order to prevent object's state corruption.

A typical instance of an **active object** is an animation widget. The widget state changes with the time, while as an object it has all properties of a normal widget.

**The task**

Implement an active integrator object. The object has an **input** and **output**. The **input** can be set using the method *Input*. The **input** is a function of time（关于时间的函数）. The output can be queried using the method *Output*. The object integrates its input over the time and the result becomes the object's output. So if the input is *K*(*t*) and the output is *S*, the object state *S* is changed to *S* + (*K*(*t*1) + *K*(*t*0)) * (*t*1 - *t*0) / 2, i.e. it integrates *K* using the trapeze method. Initially *K* is constant 0 and *S* is 0.

In order to test the object:

1. set its input to sin (2π *f t*), where the frequency *f*=0.5Hz. The phase is irrelevant.
2. wait 2s
3. set the input to constant 0
4. wait 0.5s

Verify that now the object's output is approximately 0 (the sine has the period of 2s). The accuracy of the result will depend on the [OS](https://rosettacode.org/wiki/OS) scheduler time slicing and the accuracy of the clock.

## [C](https://rosettacode.org/wiki/Category:C)

Uses POSIX threads.

**Library:** [pthread](https://rosettacode.org/wiki/Category:Pthread)

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>
 
/* no need to lock the object: at worst the readout would be 1 tick off,
   which is no worse than integrator's inate inaccuracy */
typedef struct {
	double (*func)(double);
	struct timeval start;
	double v, last_v, last_t;
	pthread_t id;
} integ_t, *integ;
 
void update(integ x)
{
	struct timeval tv;
	double t, v, (*f)(double);
 
	f = x->func;
	gettimeofday(&tv, 0);
	t = ((tv.tv_sec - x->start.tv_sec) * 1000000
		+ tv.tv_usec - x->start.tv_usec) * 1e-6;
	v = f ? f(t) : 0;
	x->v += (x->last_v + v) * (t - x->last_t) / 2;
	x->last_t = t;
}
 
void* tick(void *a)
{
	integ x = a;
	while (1) {
		usleep(100000); /* update every .1 sec */
		update(x);
	}
}
 
void set_input(integ x, double (*func)(double))
{
	update(x);
	x->func = func;
	x->last_t = 0;
	x->last_v = func ? func(0) : 0;
}
 
integ new_integ(double (*func)(double))
{
	integ x = malloc(sizeof(integ_t));
	x->v = x->last_v = 0;
	x->func = 0;
	gettimeofday(&x->start, 0);
	set_input(x, func);
	pthread_create(&x->id, 0, tick, x);
	return x;
}
 
double sine(double t) { return sin(4 * atan2(1, 1) * t); }
 
int main()
{
	integ x = new_integ(sine);
	sleep(2);
	set_input(x, 0);
	usleep(500000);
	printf("%g\n", x->v);
 
	return 0;
}
```

## [C++](https://rosettacode.org/wiki/Category:C%2B%2B)

 **Works with**: [C++14](https://rosettacode.org/wiki/C%2B%2B14) 

```c++
#include <atomic>
#include <chrono>
#include <cmath>
#include <iostream>
#include <mutex>
#include <thread>
 
using namespace std::chrono_literals;
 
class Integrator
{
  public:
    using clock_type = std::chrono::high_resolution_clock;
    using dur_t      = std::chrono::duration<double>;
    using func_t     = double(*)(double);
 
    explicit Integrator(func_t f = nullptr);
    ~Integrator();
    void input(func_t new_input);
    double output() { return integrate(); }
 
  private:
    std::atomic_flag continue_;
    std::mutex       mutex;
    std::thread      worker;
 
    func_t                       func;
    double                       state = 0;
    //Improves precision by reducing sin result error on large values
    clock_type::time_point const beginning = clock_type::now();
    clock_type::time_point       t_prev = beginning;
 
    void do_work();
    double integrate();
};
 
Integrator::Integrator(func_t f) : func(f)
{
    continue_.test_and_set();
    worker = std::thread(&Integrator::do_work, this);
}
 
Integrator::~Integrator()
{
    continue_.clear();
    worker.join();
}
 
void Integrator::input(func_t new_input)
{
    integrate();
    std::lock_guard<std::mutex> lock(mutex);
    func = new_input;
}
 
void Integrator::do_work()
{
    while(continue_.test_and_set()) {
        integrate();
        std::this_thread::sleep_for(1ms);
    }
}
 
double Integrator::integrate()
{
    std::lock_guard<std::mutex> lock(mutex);
    auto now = clock_type::now();
    dur_t start = t_prev - beginning;
    dur_t fin   =    now - beginning;
    if(func)
        state += (func(start.count()) + func(fin.count())) * (fin - start).count() / 2;
    t_prev = now;
    return state;
}
 
double sine(double time)
{
    constexpr double PI = 3.1415926535897932;
    return std::sin(2 * PI * 0.5 * time);
}
 
int main()
{
    Integrator foo(sine);
    std::this_thread::sleep_for(2s);
    foo.input(nullptr);
    std::this_thread::sleep_for(500ms);
    std::cout << foo.output();
}
```

## [Python](https://rosettacode.org/wiki/Category:Python)

 Assignment is thread-safe in Python, so no extra locks are needed in this case. 

```python
from time import time, sleep
from threading import Thread
 
class Integrator(Thread):
    'continuously integrate a function `K`, at each `interval` seconds'
    def __init__(self, K=lambda t:0, interval=1e-4):
        Thread.__init__(self)
        self.interval  = interval
        self.K   = K
        self.S   = 0.0
        self.__run = True
        self.start()
 
    def run(self):
        "entry point for the thread"
        interval = self.interval
        start = time()
        t0, k0 = 0, self.K(0)
        while self.__run:
            sleep(interval)
            t1 = time() - start
            k1 = self.K(t1)
            self.S += (k1 + k0)*(t1 - t0)/2.0
            t0, k0 = t1, k1
 
    def join(self):
        self.__run = False
        Thread.join(self)
 
if __name__ == "__main__":
    from math import sin, pi
 
    ai = Integrator(lambda t: sin(pi*t))
    sleep(2)
    print ai.S
    ai.K = lambda t: 0
    sleep(0.5)
    print ai.S
```

