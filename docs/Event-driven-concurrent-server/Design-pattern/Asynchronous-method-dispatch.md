# Asynchronous method dispatch



## Wikipedia [Asynchronous method dispatch](https://infogalactic.com/info/Asynchronous_method_dispatch)

**Asynchronous method dispatch** (AMD) is a [data communication](https://infogalactic.com/info/Data_communication) method used when there is a need for the server side to handle a large number of long lasting client requests. Using synchronous method dispatch (SMD), this scenario may turn the server into an unavailable busy state resulting in a connection failure response caused by a [network](https://infogalactic.com/info/Computer_network) connection request [timeout](https://infogalactic.com/info/Timeout_(computing)).

### Mechanism

The servicing of a client request is immediately dispatched to an available [thread](https://infogalactic.com/info/Thread_(computer_science)) from a [pool of threads](https://infogalactic.com/info/Thread_pool_pattern) and the client is put in a blocking state. Upon the completion of the task, the server is notified by a callback. The server unblocks the client and transmits the response back to the client. In case of thread starvation, clients are blocked waiting for threads to become available.