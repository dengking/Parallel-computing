# Hand-over-hand lock

1、在阅读 drdobbs [Choose Concurrency-Friendly Data Structures](https://www.drdobbs.com/parallel/choose-concurrency-friendly-data-structu/208801371) 时，其中介绍了专用于 linked list 的 hand-over-hand lock。

2、hand-over-hand-lock是典型的需要lock multiple lock的

3、tag-lock granularity-fine-grained-reduce contention-降低锁粒度-提高并发性-减少竞争

4、它的中文翻译是: 交替锁(hand-over-hand locking)

## courses.csail.mit [6.852 Lecture 21](http://courses.csail.mit.edu/6.852/08/lectures/Lecture21.pdf)

看了一下，其中对"hand-over-hand lock"进行了比较好的描述。





## jgjin [Hand-over-hand locking with the RAII pattern](https://jgjin.com/posts/raii_hoh_locking/)





## csdn [七周七并发之线程与锁](https://blog.csdn.net/weixin_33922672/article/details/91570576)