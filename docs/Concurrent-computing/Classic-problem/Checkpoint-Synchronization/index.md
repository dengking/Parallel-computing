# Checkpoint Synchronization

是在阅读 golangprograms [Golang Concurrency](https://www.golangprograms.com/go-language/concurrency.html) 时，发现的这个topic。

## golangprograms [Golang Concurrency # Illustration of Checkpoint Synchronization in Golang](https://www.golangprograms.com/go-language/concurrency.html)



## rosettacode [Checkpoint synchronization](https://rosettacode.org/wiki/Checkpoint_synchronization)

The checkpoint synchronization is a problem of synchronizing multiple [tasks](https://rosettacode.org/wiki/Task). Consider a workshop where several workers ([tasks](https://rosettacode.org/wiki/Task)) assembly details of some mechanism. When each of them completes his work they put the details together. There is no store, so a worker who finished its part first must wait for others before starting another one. Putting details together is the *checkpoint* at which [tasks](https://rosettacode.org/wiki/Task) synchronize themselves before going their paths apart.