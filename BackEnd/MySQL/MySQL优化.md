# 优化目的
- 避免由于数据库连接timeout产生5xx（服务期内部）错误。
- 避免由于慢查询造成页面无法加载。
- 由于阻塞造成数据无法提交。

# 如何分析SQL查询？
http://dev.mysql.com/doc/index-other.html [sakila-db文档](https://dev.mysql.com/doc/sakila/en/)

慢查询日志发现有效率问题的sql语句。
show variables like 'slow_query_log' 查看，默认是关闭的。需要手动打开。
show global slow_query_log_file = '/home/mysql/log/mysql-slow.log'
set global log_queries_not_using_indexes = on;
set global slow_query_log = on
set global long_query_time = 1;

慢查询日志查看分析工具：mysqldumpslow， pt-query-digest.

检查sql：explain SQL

子查询一般要优化为join。

Group-by的优化：

limit的优化：
1. 使用有索引的列或主键进行order by操作
2. 记录上次查询的位置

#### 如何选择合适的列建立索引？
1. where、group by、order by、on从句中出现的列。
2. 索引字段越小越好
3. 离散度大的列（数量多）放到联合索引（and）的前面。


#### 重复及冗余索引
可以利用工具 pt-duplicate-key-cheker 检查。

#### 查找并删除不用的索引
mysql目前只能用慢查询日志配合pt-index-usage工具。

# 表结构的优化
## 选择合适的数据类型
尽量not null + default value。
不要text，如果要就分表。
