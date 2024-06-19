# PySpark



[sparkbyexamples-PySpark SQL expr() (Expression) Function](https://sparkbyexamples.com/pyspark/pyspark-sql-expr-expression-function/)

[sparkbyexamples-Spark rlike() Working with Regex Matching Examples](https://sparkbyexamples.com/spark/spark-rlike-regex-matching-examples/)



## [PySpark](https://spark.apache.org/docs/latest/api/python/index.html)





### [User Defined Aggregate Functions (UDAFs)](https://spark.apache.org/docs/latest/sql-ref-functions-udf-aggregate.html) 



danvatterott [Python Aggregate UDFs in PySpark](https://danvatterott.com/blog/2018/09/06/python-aggregate-udfs-in-pyspark/) 

stackoverflow [How can I define user-defined aggregate functions in PySpark?](https://stackoverflow.com/questions/73817326/how-can-i-define-user-defined-aggregate-functions-in-pyspark) 

stackoverflow [Applying UDFs on GroupedData in PySpark (with functioning python example)](https://stackoverflow.com/questions/40006395/applying-udfs-on-groupeddata-in-pyspark-with-functioning-python-example)



Apologies for the confusion. The `GroupedData` object in PySpark does not have an `rdd` attribute. Instead, you can use the `flatMapGroupsWithState` method to process grouped data in a distributed manner. Here's an example:

```python
python复制from pyspark.sql import SparkSession
from pyspark.sql.functions import pandas_udf, PandasUDFType

# Create a SparkSession
spark = SparkSession.builder.getOrCreate()

# Create a sample DataFrame
data = [("John Doe", 25),
        ("Jane Smith", 30),
        ("Bob Johnson", 35),
        ("John Doe", 40)]
df = spark.createDataFrame(data, ["name", "age"])

# Define a custom function to apply to each group
@pandas_udf(df.schema, PandasUDFType.GROUPED_MAP)
def process_group(key, pdf):
    for index, row in pdf.iterrows():
        name = row["name"]
        age = row["age"]
        print(f"Group: {key}, Name: {name}, Age: {age}")
    return pdf

# Group the DataFrame by a column and apply the custom function in a distributed manner
grouped_data = df.groupBy("name").apply(process_group)
grouped_data.show()
```

In this example, we create a DataFrame with two columns: "name" and "age". We define a custom function `process_group` using a Pandas UDF (`pandas_udf`) decorator. This function takes a group key and a Pandas DataFrame (`pdf`) as input and processes each row within the group.

We then use the `groupBy` method on the DataFrame to group the data by the "name" column. The `apply` method is used to apply the `process_group` function to each group in a distributed manner.

Note that the output of the `process_group` function is expected to be a Pandas DataFrame with the same schema as the input DataFrame. In this example, we simply return the input Pandas DataFrame after processing each row within the group.

Please note that using `print` statements within the `process_group` function may not display the output as expected in a distributed environment. It is recommended to use Spark actions like `show` or `collect` to view the results.





```
It is preferred to use 'applyInPandas' over this API. This API will be deprecated in the future releases. See SPARK-28264 for more details.
```



### [pyspark.sql.GroupedData.applyInPandas](https://spark.apache.org/docs/latest/api/python/reference/pyspark.sql/api/pyspark.sql.GroupedData.applyInPandas.html)

