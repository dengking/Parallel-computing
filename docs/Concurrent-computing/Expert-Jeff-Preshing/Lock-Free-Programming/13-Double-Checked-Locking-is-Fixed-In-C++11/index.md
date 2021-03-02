# [Double-Checked Locking is Fixed In C++11](https://preshing.com/20130930/double-checked-locking-is-fixed-in-cpp11/)

## What Is Double-Checked Locking?

```C++
Singleton* Singleton::getInstance() {
    Singleton* tmp = m_instance;
    ...                     // insert memory barrier
    if (tmp == NULL) {
        Lock lock;
        tmp = m_instance;
        if (tmp == NULL) {
            tmp = new Singleton;
            ...             // insert memory barrier
            m_instance = tmp;
        }
    }
    return tmp;
}
```

## Using C++11 Acquire and Release Fences

```c++
std::atomic<Singleton*> Singleton::m_instance;
std::mutex Singleton::m_mutex;

Singleton* Singleton::getInstance() {
    Singleton* tmp = m_instance.load(std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_acquire);
    if (tmp == nullptr) {
        std::lock_guard<std::mutex> lock(m_mutex);
        tmp = m_instance.load(std::memory_order_relaxed);
        if (tmp == nullptr) {
            tmp = new Singleton;
            std::atomic_thread_fence(std::memory_order_release);
            m_instance.store(tmp, std::memory_order_relaxed);
        }
    }
    return tmp;
}
```



![img](https://preshing.com/images/two-cones-dclp.png)



## Using Mintomic Fences

```C++
mint_atomicPtr_t Singleton::m_instance = { 0 };
mint_mutex_t Singleton::m_mutex;

Singleton* Singleton::getInstance() {
    Singleton* tmp = (Singleton*) mint_load_ptr_relaxed(&m_instance);
    mint_thread_fence_acquire();
    if (tmp == NULL) {
        mint_mutex_lock(&m_mutex);
        tmp = (Singleton*) mint_load_ptr_relaxed(&m_instance);
        if (tmp == NULL) {
            tmp = new Singleton;
            mint_thread_fence_release();
            mint_store_ptr_relaxed(&m_instance, tmp);
        }
        mint_mutex_unlock(&m_mutex);
    }
    return tmp;
}
```

## Using C++11 Low-Level Ordering Constraints

```C++
std::atomic<Singleton*> Singleton::m_instance;
std::mutex Singleton::m_mutex;

Singleton* Singleton::getInstance() {
    Singleton* tmp = m_instance.load(std::memory_order_acquire);
    if (tmp == nullptr) {
        std::lock_guard<std::mutex> lock(m_mutex);
        tmp = m_instance.load(std::memory_order_relaxed);
        if (tmp == nullptr) {
            tmp = new Singleton;
            m_instance.store(tmp, std::memory_order_release);
        }
    }
    return tmp;
}
```

## Using C++11 Sequentially Consistent Atomics

```C++
std::atomic<Singleton*> Singleton::m_instance;
std::mutex Singleton::m_mutex;

Singleton* Singleton::getInstance() {
    Singleton* tmp = m_instance.load();
    if (tmp == nullptr) {
        std::lock_guard<std::mutex> lock(m_mutex);
        tmp = m_instance.load();
        if (tmp == nullptr) {
            tmp = new Singleton;
            m_instance.store(tmp);
        }
    }
    return tmp;
}
```