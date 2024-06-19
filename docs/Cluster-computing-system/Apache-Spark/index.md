# Apark Spark

素材:

- 51cto [hadoop spark 和 的区别 spark和hadoop之间的关系](https://blog.51cto.com/u_16099337/6698162) 

  内容比较完备



## wikipedia [Apache Spark](https://en.wikipedia.org/wiki/Apache_Spark)



## [Apache Spark - A Unified engine for large-scale data analytics](https://spark.apache.org/docs/latest/index.html) 





## RDD

- Resilient Distributed Dataset: **弹性分布式数据集** 
- Unit: partition是spark并行计算的单位



### gpt-4-vision

In Apache Spark, an RDD (Resilient Distributed Dataset) is a fundamental data structure that represents an immutable, distributed collection of objects that can be processed in parallel. RDDs can contain any type of Python, Java, or Scala objects, including user-defined classes.

> NOTE:
>
> spark并行计算的单位

RDDs are fault-tolerant, meaning they can automatically recover from node failures. They achieve fault tolerance through a concept called **lineage**(血统), where Spark remembers the series of transformations applied to build the RDD, allowing it to recompute any lost data.

Here are some key features of RDDs:

1. **Immutability**: Once created, the data in an RDD cannot be changed. However, you can create new RDDs by applying transformations to existing ones.

2. **Lazy Evaluation**: Transformations on RDDs are lazily evaluated, meaning that Spark does not execute the transformation immediately. Instead, it creates a lineage graph(血统图), and the actual computation happens when an action is called.

3. **Distributed**: RDDs are distributed across the nodes in a Spark cluster, enabling parallel processing.

4. **Persistence**: Users can choose to persist (or cache) an RDD in memory or on disk for reuse in multiple Spark operations, which can significantly improve performance.

5. **Partitioning**: RDDs are divided into partitions, which are the basic units of parallelism in Spark. Each partition can be processed in parallel across different nodes.

Creating RDDs can be done in several ways:

1. **Parallelizing an existing collection**: You can create an RDD from a Python list using the `parallelize` method.

```python
data = [1, 2, 3, 4, 5]
rdd = sc.parallelize(data)
```

2. **Referencing an external dataset**: You can create an RDD from external storage like HDFS, HBase, or shared file systems.

```python
rdd = sc.textFile("hdfs://path/to/file.txt")
```

3. **Transforming an existing RDD**: You can apply transformations like `map`, `filter`, `flatMap`, etc., to create new RDDs from existing ones.

```python
rdd = sc.parallelize([1, 2, 3, 4, 5])
squared_rdd = rdd.map(lambda x: x * x)
```

RDDs support two types of operations:

1. **Transformations**: These are operations that create a new RDD from an existing one, such as `map`, `filter`, `flatMap`, `reduceByKey`, `join`, etc. Transformations are lazy and are not executed until an action is called.

```python
# Transformation example: filter
filtered_rdd = rdd.filter(lambda x: x % 2 == 0)  # Selects even numbers
```

2. **Actions**: These are operations that return a value to the driver program or write data to external storage, such as `collect`, `count`, `reduce`, `take`, `saveAsTextFile`, etc. Actions trigger the execution of the RDD lineage graph.

```python
# Action example: collect
result = filtered_rdd.collect()  # Returns the filtered data to the driver
```

While RDDs are a powerful abstraction, Spark also provides higher-level APIs like DataFrames and Datasets, which are more optimized and easier to use for certain types of operations. However, RDDs are still useful when you need fine-grained control over data processing or when working with unstructured data.



### cnblogs [Spark学习之路 （三）Spark之RDD](https://www.cnblogs.com/qingyunzong/p/8899715.html)

RDD（Resilient Distributed Dataset）叫做**弹性分布式数据集**，**是Spark中最基本的数据抽象**，它代表一个不可变、可分区、里面的元素可并行计算的集合。