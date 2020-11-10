
# [Active object](https://en.wikipedia.org/wiki/Active_object)

 The **active object** [design pattern](https://en.wikipedia.org/wiki/Design_pattern) decouples **method execution** from **method invocation** for objects that each reside in their own [thread](https://en.wikipedia.org/wiki/Thread_(computing)) of control.[[1\]](https://en.wikipedia.org/wiki/Active_object#cite_note-1) The goal is to introduce [concurrency](https://en.wikipedia.org/wiki/Concurrency_(computer_science)), by using [asynchronous method invocation](https://en.wikipedia.org/wiki/Asynchronous_method_invocation) and a [scheduler](https://en.wikipedia.org/wiki/Scheduling_(computing)) for handling requests.[[2\]](https://en.wikipedia.org/wiki/Active_object#cite_note-2) 

***SUMMARY*** : 结合下面的例子来看，上面这段话的意思其实是method invocation只是往queue中添加了一个request，method execution在另外一个thread中进行；

***THINKING*** : 这个名称的由来是什么？何为active？

 The pattern consists of six elements: 

- A [proxy](https://en.wikipedia.org/wiki/Proxy_pattern), which provides an interface towards clients with publicly accessible methods.
- An interface which defines the method request on an active object.
- A list of pending requests from clients.
- A [scheduler](https://en.wikipedia.org/wiki/Scheduling_(computing)), which decides which request to execute next.
- The implementation of the active object method.
- A [callback](https://en.wikipedia.org/wiki/Callback_(computer_science)) or [variable](https://en.wikipedia.org/wiki/Variable_(computer_science)) for the client to receive the result.

## Example

### Java

 An example of active object pattern in [Java](https://en.wikipedia.org/wiki/Java_(programming_language)).[[4\]](https://en.wikipedia.org/wiki/Active_object#cite_note-4) 

 Firstly we can see a standard class that provides two methods that set a double to be a certain value. This class does **NOT** conform to the **active object pattern**. 

```JAVA
class MyClass {

    private double val = 0.0;
    
    void doSomething() {
        val = 1.0;
    }

    void doSomethingElse() {
        val = 2.0;
    }
}
```

 The class is dangerous in a multithreading scenario because both methods can be called simultaneously, so the value of `val` (which is not atomic—it's updated in multiple steps) could be undefined—a classic race condition. You can, of course, use synchronization to solve this problem, which in this trivial case is easy. But once the class becomes realistically complex, synchronization can become very difficult. [[5\]](https://en.wikipedia.org/wiki/Active_object#cite_note-5) 

 To rewrite this class as an active object you could do the following: 

```JAVA
class MyActiveObject {

    private double val = 0.0;

    private BlockingQueue<Runnable> dispatchQueue = new LinkedBlockingQueue<Runnable>();

    public MyActiveObject() {
        new Thread (new Runnable() {
                    
                @Override
                public void run() {
                    while(true) {
                        try {
                            dispatchQueue.take().run();
                        } catch (InterruptedException e) {   
                            // okay, just terminate the dispatcher
                        }
                    }
                }
            }
        ).start();
    }

    void doSomething() throws InterruptedException {
        dispatchQueue.put(new Runnable() {
                @Override
                public void run() { 
                    val = 1.0; 
                }
            }
        );
    }

    void doSomethingElse() throws InterruptedException {
        dispatchQueue.put(new Runnable() {
                @Override
                public void run() { 
                    val = 2.0; 
                }
            }
        );
    }
}
```

### Java 8 (alternative)

 Another example of active object pattern in Java instead implemented in Java 8 providing a shorter solution. 

```java
public class MyClass {
    private double val; 
    
    // container for tasks
    // decides which request to execute next 
    // asyncMode=true means our worker thread processes its local task queue in the FIFO order 
    // only single thread may modify internal state
    private final ForkJoinPool fj = new ForkJoinPool(1, ForkJoinPool.defaultForkJoinWorkerThreadFactory, null, true);
    
    // implementation of active object method
    public void doSomething() throws InterruptedException {
        fj.execute(() -> {val = 1.0;});
    }
 
    // implementation of active object method
    public void doSomethingElse() throws InterruptedException {
        fj.execute(() -> {val = 2.0;});
    }
}
```