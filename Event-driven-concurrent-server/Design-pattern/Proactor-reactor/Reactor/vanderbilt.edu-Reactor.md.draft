# [Reactor](http://www.dre.vanderbilt.edu/~schmidt/PDF/reactor-siemens.pdf)

An Object Behavioral Pattern for Demultiplexing and Dispatching Handles for **Synchronous** Events

***SUMMARY*** : demultiplexing 是借用的 [telecommunications](https://en.wikipedia.org/wiki/Telecommunications) and [computer networks](https://en.wikipedia.org/wiki/Computer_networks)中的概念，参见[这篇文章](https://en.wikipedia.org/wiki/Multiplexing)。

## 1 Intent



The Reactor design pattern handles **service requests** that are delivered concurrently to an application by one or more clients. Each **service** in an application may consist of serveral methods and is represented by a separate **event handler** that is responsible for dispatching **service-specific requests**. Dispatching of **event handlers** is performed by an **initiation dispatcher**, which manages the registered **event handlers**. Demultiplexing of **service requests** is performed by a **synchronous event demultiplexer**.

***SUMMARY*** : 本文中的 **synchronous event demultiplexer**是使用OS提供的机制来实现的，比如`select`系统调用；

***SUMMARY*** : 上面这段话提及了两个dispatch，`event handler` is responsible for dispatch service-specific request，`initiation dispatch` perform dispatching `event handler`。那么我就有一个问题：service-specific request是否会经过`initiation dispatcher`？service-specific request和event handler之间是否存在着映射？如果存在，这个映射应该是有谁来进行维护？

***SUMMARY*** : 根据下面的描述，event源自于OS 的handle；关于handle，参见[Handles](# Handles) 。



***SUMMARY*** : service 和 event handler之间是什么关系？是否是每个service都需要由一个event handler？在[6 Solution](# 6 Solution)中提及了这样的一段话：For each **service** the application offers, introduce a separate `Event Handler ` ；

***SUMMARY*** : event是否源自于handle？是的，在[Handles](# Handles)中已经有描述，

## 2 Also Known As

Dispatcher, Notifier

## 3 Example

To illustrate the **Reactor pattern**, consider the **event-driven server** for a distributed logging service shown in Figure 1.Client applications use the logging service to record information about their status in a distributed environment. This status information commonly includes error notifications, debugging traces, and performance reports. Logging records are sent to a central logging server, which can write the
records to various output devices,such as a console,a printer,a file, or a network management database.

The logging server shown in Figure 1 handles logging records and connection requests sent by clients. Logging records and connection requests can arrive concurrently on multiple ***handles***. A ***handle*** identifies network communication resources managed within an OS.

***THINKING*** : 上面这段话中的***handles***要如何理解？参见[Handles](# Handles) 

The **logging server** communicates with clients using a connection-oriented protocol, such as TCP [1]. Clients that want to log data must first send a **connection request** to the server. The server **waits for** these **connection requests** using a ***handle factory*** that listens on an address known to clients.When a **connection request** arrives, the **handle factory** establishes a connection between the client and the server by creating a new **handle** that represents an endpoint of the connection. This handle is returned to the server, which then waits for client service requests to arrive on the handle. Once clients are connected, they can send logging records concurrently to the server. The server receives these records via the
connected socket handles.

***SUMMARY*** : 此处需要network knowledge；

Perhaps the most intuitive way to develop a concurrent logging server is to use multiple threads that can process multiple clients concurrently, as shown in Figure 2. This approach synchronously accepts network connections and spawns a “thread-per-connection” to handle client logging records.

However,using multi-threading to implement the processing of logging records in the server fails to resolve the following forces:

- **Efficiency**: Threading may lead to poor performance due to context switching, synchronization, and data movement
- **Programming simplicity**: Threading may require complex concurrency control schemes;
- **Portability**: Threading is not available on all OS platforms.

As a result of these drawbacks, multi-threading is often not the most efficient nor the least complex solution to develop a concurrent logging server.

## 4 Context

A server application in a distributed system that receives events from one or more clients concurrently.

***SUMMARY*** : context所描述的是该模式的使用场景；对比Proactor pattern的context可以看到，Reactor pattern的event来自于client；而

## 5 Problem

**Server applications** in a distributed system must handle multiple clients that send them service requests. Before invoking a specific service, however, the **server application** must **demultiplex** and **dispatch** each incoming request to its corresponding service provider. Developing an effective server mechanisms for demultiplexing and dispatching client requests requires the resolution of the following forces:

- **Availability**: The server must be available to handle incoming requests even if it is waiting for other requests to arrive(即能够同时处理request和等待request). In particular, a server must not block indefinitely handling any single source of events at the exclusion（此处应该是execution） of other event sources since this may significantly delay the responseness to other clients(server不得在执行其他事件源时无限制地阻止处理任何单个事件源，因为这可能会显着延迟对其他客户端的响应。).
- **Efficiency**: A server must minimize latency, maximize throughput, and avoid utilizing the CPU(s) unnecessarily.
- **Programming simplicity**: The design of a server should simplify the use of suitable concurrency strategies.
- **Adaptability**: Integrating new or improved services, such as changing message formats or adding server-side caching, should incur minimal modifications and maintenance costs for existing code. For instance, implementing new **application services** should not require modifications to the generic event demultiplexing and dispatching mechanisms.
- **Portability**: Porting a server to a new OS platform should not require significant effort.



## 6 Solution

Integrate the synchronous demultiplexing of events and the dispatching of their corresponding **event handlers** that process the events. In addition, decouple the application-specific dispatching and implementation of **services** from the general-purpose event demultiplexing and dispatching
mechanisms. 

***THINKING*** : application-specific dispatching指的是什么？

For each **service** the application offers, introduce a separate `Event Handler ` that processes certain types of events. All `Event Handlers` implement the same interface. `Event Handlers` register with an `Initiation Dispatcher`, which uses a `Synchronous Event Demultiplexer ` to wait for events to occur. When **events** occur, the `Synchronous Event Demultiplexer` notifies the `Initiation Dispatcher`, which **synchronously** calls back to the `Event Handler` associated with the event. The `Event Handler` then dispatches the **event** to the method that implements the requested service.

## 7 Structure

The key participants in the **Reactor pattern** include the following:

### Handles

***SUMMARY*** : handle是Windows OS中喜欢使用的属于，在linux OS中往往喜欢使用descriptor。

Identify resources that are managed by an OS.These resources commonly include **network connections**, **open files**, **timers**, **synchronization objects**, etc.`Handles` are used in the logging server to identify
**socket endpoints** so that a `Synchronous Event Demultiplexer` can wait for events to occur on
them. The two types of events the logging server is interested in are connection events and read events,which represent incoming client connections and logging data,respectively. The logging server maintains a separate connection for each client. Every connection is represented in the server by a **socket handle**.

***SUMMARY*** : 根据上面的描述，event源自于handle，由`Synchronous Event Demultiplexer`监控。

### Synchronous Event Demultiplexer

Blocks awaiting **events** to occur on a set of `Handles`.It returns when it is possible to initiate an operation
on a `Handle` without blocking. A common demultiplexer for I/O events is `select`[1], which is an event
demultiplexing system call provided by the UNIX and Win32 OS platforms. The `select` call indicates which `Handles` can have operations invoked on them synchronously without blocking the application process.

***SUMMARY***  : 关于`select`，参见APUE chapter 14.4 I/O Multiplexing

### Initiation Dispatcher

Defines an interface for registering, removing, and dispatching **Event Handlers**. Ultimately, the
`Synchronous Event Demultiplexer` is responsible for waiting until new events occur. When
it detects new events, it informs the `Initiation Dispatcher` to call back（回调） application-specific event handlers. Common events include connection acceptance events, data input and output events, and timeout events.

### Event Handler

Specifies an **interface** consisting of a **hook method** [3] that abstractly represents the dispatching operation for service-specific events. This method must be implemented by application-specific services.

### Concrete Event Handler

Implements the hook method, as well as the methods to process these events in an application-specific
manner. Applications register `Concrete Event Handlers` with the `Initiation Dispatcher` to process certain types of events. When these events arrive, the `Initiation Dispatcher` calls back the hook method of the appropriate `Concrete Event Handler`.

There are two `Concrete Event Handlers` in the logging server: `Logging Handler` and `Logging Acceptor`. The `Logging Handler` is responsible for receiving and processing logging records. The `Logging Acceptor` creates and connects `Logging Handlers` that process subsequent logging records from clients.

The structure of the participants of the Reactor pattern is illustrated in the following OMT class diagram:

## 8 Dynamics

### 8.1 General Collaborations

The following collaborations occur in the Reactor pattern:

- When an application registers a `Concrete Event Handler` with the `Initiation Dispatcher` , the
  application indicates the type of event(s) this `Event Handler` wants the `Initiation Dispatcher` to
  notify it about when the event(s) occur on the associated `Handle`.

  应用程序使用`Initiation Dispatcher`注册`Concrete Event Handler`，根据上面对`Initiation Dispatcher`的介绍，它提供了`register`接口。通过此，application指示此`Event Handler`希望`Initiation Dispatcher`通知它关于何时在关联的Handle上发生事件的事件类型。

- The `Initiation Dispatcher` requests each `Event Handler` to pass back（传回） its internal `Handle`.This `Handle` identifies the `Event Handler` to the OS.

- After all `Event Handlers` are registered,an application calls `handle_events` to start the `Initiation`
  `Dispatcher`’s event loop. At this point, the `Initiation Dispatcher` combines the `Handle` from each registered `Event Handler` and uses the `Synchronous Event Demultiplexer` to wait for **events** to occur on these Handles. For instance, the TCP protocol layer uses the `select` synchronous event demultiplexing operation to wait for client logging record events to arrive on connected socket `Handles`.

  ***THINKING*** : `Initiation Dispatcher`的main loop执行了哪些操作？

- The `Synchronous Event Demultiplexer` notifies the `Initiation Dispatcher` when a `Handle` corresponding to an event source becomes “ready,” e.g., that a TCP socket is “ready for reading.”

- The `Initiation Dispatcher` triggers `Event Handler` hook method in response to events on
  the ready `Handles`. When events occur, the `Initiation Dispatcher` uses the `Handles` activated by the event sources as “keys” to locate and dispatch the appropriate `Event Handler`’s hook method.

- The `Initiation Dispatcher` calls back to the `handle_event` hook method of the `Event Handler` to perform application-specific functionality in response to an event. The type of event that occurred can be passed as a parameter to the method and used internally by this method to perform additional service-specific demultiplexing and dispatching. An alternative dispatching approach is described in Section 9.4.



The following interaction diagram illustrates the collaboration between application code and participants in the Reactor pattern:







## 8.2 Collaboration Scenarios



The collaborations within the Reactor pattern for the logging server can be illustrated with two scenarios. These scenarios show how a logging server designed using reactive event dispatching handles connection requests and logging data from multiple clients.



### 8.2.1 Client Connects to a Reactive Logging Server



The first scenario shows the steps taken when a client connects to the logging server.

This sequence of steps can be summarized as follows:

1. The logging server (1) registers the `Logging Acceptor` with the `Initiation Dispatcher` to handle connection requests;
2. The logging server invokes the `handle_events` method (2) of the `Initiation Dispatcher`;
3. The `Initiation Dispatcher` invokes the synchronous event demultiplexing `select` (3) operation
  to wait for connection requests or logging data to arrive;
4. A client connects (4) to the logging server;
5.  The `Logging Acceptor` is notified by the `Initiation Dispatcher` (5) of the new connection request;
6. The `Logging Acceptor` accepts (6) the new connection;
7. The `Logging Acceptor` creates (7) a `Logging Handler` to service the new client;
8. `Logging Handler` registers (8) its socket handle with the `Initiation Dispatcher` and instructs
  the dispatcher to notify it when the socket becomes “ready for reading.”

### 8.2.2 Client Sends Logging Record to a Reactive Logging Server

The second scenario shows the sequence of steps that the reactive logging server takes to service a logging record.

This sequence of steps can be summarized as follows:

1. The client sends (1) a logging record;
2. The `Initiation Dispatcher` notifies (2) the associated `Logging Handler` when a client logging record is queued on its socket handle by OS;
3.  The record is received (3) in a non-blocking manner (steps 2 and 3 repeat until the logging record has been received completely);
4.  The `Logging Handler` processes the logging record and writes (4) it to the standard output.
5. The `Logging Handler` returns (5) control to the `Initiation Dispatcher`’s event loop.

## 9 Implementation

This section describes how to implement the Reactor pattern in `C++`. The implementation described below is influenced by the reusable components provided in the ACE communication software framework [2].

### 9.1 Select the Synchronous Event Demultiplexer Mechanism

The Initiation Dispatcher uses a Synchronous Event Demultiplexer to wait synchronously until one or more events occur. This is commonly implemented using an OS event demultiplexing system call like `select`.
The `select` call indicates which `Handle`(s) are ready to perform I/O operations without blocking the OS process in which the application-specific service handlers reside. In general, the `Synchronous Event Demultiplexer` is based upon existing OS mechanisms, rather than developed by implementers of the **Reactor pattern**.

### 9.2 Develop an Initiation Dispatcher

The following are the steps necessary to develop the `Initiation Dispatcher`:

**Implement the Event Handler table**: A `Initiation Dispatcher` maintains a table of `Concrete Event`
`Handlers`. Therefore, the `Initiation Dispatcher` provides methods to register and remove the handlers from this table at run-time. This table can be implemented in various ways, e.g., using **hashing**, **linear search**, or **direct indexing** if handles are represented as a contiguous range of small integral values.

**Implement the event loop entry point**: The **entry point** into the **event loop** of the `Initiation Dispatcher` should be provided by a `handle_events` method. This method controls the `Handle` demultiplexing provided by the `Synchronous Event Demultiplexer`, as well as performing `Event Handler` dispatching. Often, the main event loop of the entire application is controlled by this entry point.

When events occur, the `Initiation Dispatcher` returns from the synchronous event demultiplexing call
and “reacts” by dispatching the `Event Handler`’s handle event hook method for each handle that is
“ready.” This hook method executes user-defined code and returns control to the Initiation Dispatcher when it completes.

The following `C++` class illustrates the core methods on the `Initiation Dispatcher`’s public interface:

```c++
enum Event_Type
// = TITLE
// Types of events handled by the
// Initiation_Dispatcher.
//
// = DESCRIPTION
// These values are powers of two so
// their bits can be efficiently ‘‘or’d’’
// together to form composite values.
{
ACCEPT_EVENT = 01,
READ_EVENT = 02,
WRITE_EVENT = 04,
TIMEOUT_EVENT = 010,
SIGNAL_EVENT = 020,
CLOSE_EVENT = 040
}
```

```c++
class Initiation_Dispatcher
// = TITLE
// Demultiplex and dispatch Event_Handlers
// in response to client requests.
{
public:
// Register an Event_Handler of a particular
// Event_Type (e.g., READ_EVENT, ACCEPT_EVENT,
// etc.).
int register_handler (Event_Handler *eh,
Event_Type et);
// Remove an Event_Handler of a particular
// Event_Type.
int remove_handler (Event_Handler *eh,
Event_Type et);
// Entry point into the reactive event loop.
int handle_events (Time_Value *timeout = 0);
};
```

**Implement the necessary synchronization mechanisms**: If the **Reactor pattern** is used in an application with only one thread of control it is possible to eliminate all synchronization. In this case, the **Initiation Dispatcher** serializes the `Event Handler` `handle_event` hooks within the application’s process.

However, the `Initiation Dispatcher` can also serve as a central event dispatcher in multi-threaded applications. In this case, critical sections within the `Initiation Dispatcher` must be serialized to prevent **race conditions** when modifying or activating shared state variables (such as the table holding the Event Handlers). A common technique for preventing race conditions uses mutual exclusion mechanisms like semaphores or mutex variables.

To prevent self-deadlock, mutual exclusion mechanisms can use `recursive locks` [4]. Recursive locks hold prevent deadlock when locks are held by the same thread across Event Handler hook methods within the `Initiation Dispatcher`. A recursive lock may be re-acquired by the thread that owns the lock without blocking the thread. This property is important since the Reactor’s handle events method calls back on application-specific Event Handlers. Application hook method code may subsequently re-enter the `Initiation Dispatcher` via its `register_handler` and `remove_handler` methods.

### 9.3 Determine the Type of the Dispatching Target

Two different types of `Event Handlers` can be associated with a Handle to serve as the target of an
Initiation Dispatcher’s dispatching logic. Implementations of the Reactor pattern can choose either one or
both of the following dispatching alternatives:

Event Handler objects: A common way to associate an Event Handler with a Handle is to make the Event
Handler an object. For instance,the Reactor pattern implementation shown in Section 7 registers Event Handler subclass objects with an Initiation Dispatcher.
Using an object as the dispatchingtarget makesit convenient
to subclass Event Handlersin orderto reuse and extend

