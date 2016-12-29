```scala
val sc = new SparkContext(...)
val links = sc.parallelize(Array(('A', Array('D')), ('B',Array('A')),))
```
