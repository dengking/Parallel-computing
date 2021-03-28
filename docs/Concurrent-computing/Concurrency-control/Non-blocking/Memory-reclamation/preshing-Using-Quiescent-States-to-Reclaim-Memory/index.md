# preshing [Using Quiescent States to Reclaim Memory](https://preshing.com/20160726/using-quiescent-states-to-reclaim-memory/)

> NOTE: 
>
> 1、"Quiescent"的含义是: 静止的；不活动的；沉寂的

If you want to support multiple readers for a data structure, while protecting against concurrent writes, a [read-write lock](https://en.wikipedia.org/wiki/Readers–writer_lock) might seem like the only way – but it isn’t! You can achieve the same thing without a read-write lock if you allow several copies of the data structure to exist in memory. You just need a way to delete old copies when they’re no longer in use.

> NOTE: 
>
> 1、显然也是基于copy on write的

Let’s look at one way to achieve that in C++. We’ll start with an example based on a read-write lock.

## Using a Read-Write Lock

Suppose you have a network server with dozens of threads. Each thread broadcasts messages to dozens of connected clients. Once in a while, a new client connects or an existing client disconnects, so the list of connected clients must change. We can store the list of connected clients in a `std::vector` and protect it using a read-write lock such as `std::shared_mutex`.

```c++
class Server {
private:
    std::shared_mutex m_rwLock;                                   // Read-write lock
    std::vector<int> m_clients;                                   // List of connected clients
    
public:
    void broadcast(const void* msg, size_t len) {
        std::shared_lock<std::shared_mutex> shared(m_rwLock);     // Shared lock
        for (int fd : m_clients)
            send(fd, msg, len, 0);
    }

    void addClient(int fd) {
        std::unique_lock<std::shared_mutex> exclusive(m_rwLock);  // Exclusive lock
        m_clients.push_back(fd);
    }

    ...
```

The `broadcast` function reads from the list of connected clients, but doesn’t modify it, so it takes a read lock (also known as a shared lock). `addClient`, on the other hand, needs to modify the list, so it takes a write lock (also known as an exclusive lock).

That’s all fine and dandy. Now let’s eliminate the read-write lock by allowing multiple copies of the list to exist at the same time.

## Eliminating the Read-Write Lock

> NOTE: 消除read-write lock