# Read and write

在multiple model中，对shared data的操作最终可以概括为如下两种:

1、read

2、write

因此，通过read、write可以对multiple model中的很多问题进行描述。

## DBMS

在DBMS中，对此有着较好的总结，我们可以借用其中的理论:

|           | read        | write    |
| --------- | ----------- | -------- |
| **read**  | no conflict | conflict |
| **write** | conflict    | conflict |

参见工程DB的`Theory\Concurrency-Control-in-DBMS\Problem`章节。

## Concurrent computing

concurrent computing领域中的[Readers–writers problem](https://en.wikipedia.org/wiki/Readers%E2%80%93writers_problem)其实就是read and write，参见 `Concurrent-computing\Classic-problem\Readers–writers-problem` 章节。

### Concurrency control

在 `Concurrent-computing\Concurrency-control` 章节进行了总结。

## Consistency model

consistent model也是基于read、write而建立的，其中所谓的看见，其实是read

