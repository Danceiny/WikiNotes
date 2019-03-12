`\c`可取消正在输入的一个跨行命令。

在MySQL中，可以写由‘'’或‘"’字符括起来的字符串 (例如，'hello'或"goodbye")，并且mysql允许输入跨越多行的字符串。当看到一个 '> 或 "> 提示符时，这意味着已经输入了包含以‘'’或‘"’括号字符开始的字符串的一行，但是还没有输入终止字符串的匹配引号。这显示你粗心地省掉了一个引号字符。


mysql> LOAD DATA LOCAL INFILE '/path/pet.txt' INTO TABLE pet LINES TERMINATED BY '\r\n'



# 查看帮助
`mysql> ? contents`

查看列出的项（不区分大小写）：
`mysql> ? data manipulation`

查看当前服务器编码规则：
`show variables like 'character_set_server'`

`DELIMITER //`告诉命令行，使用`//`作为临时的语句结束分隔符。


# Procedure
```mysql
CREATE PROCEDURE productpricing(
    OUT pl DECIMAL(8,2),
    OUT ph DECIMAL(8,2),
    OUT pa DECIMAL(8,2)
    )
BEGIN
    SELECT Min(prod_price)
    INTO pl
    FROM products;
    SELECT Max(prod_price)
    INTO ph
    FROM products;
    SELECT Avg(prod_price)
    INTO pa
    FROM products;
END;
```
调用：
```mysql
CALL productpricing(@pricelow,
                    @pricehigh,
                    @priceaverage);

SELECT @priceaverage;
```


# Cursor 游标

# Trigger 触发器
```
CREATE TRIGGER neworder AFTER INSERT ON orders
FOR EACH ROW SELECT NEW.order_num
```
创建名为neworder的触发器，在插入一个新订单到orders表时，生成新订单号保存到order_num中，触发器从NEW.order_num取得这个值并返回它。

# 管理事务处理
- 事务 transaction 指一组sql语句；
- 回退 rollback 指撤销指定sql语句的过程；
- 提交 commit 指将未存储的sql语句结果写入数据库表
- 保留点 savepoint 指事务处理中设置的临时占位符placeholder，用以回退。


# 管理用户
`show grants for danceiny@localhost`


# 数据库维护
- `ANALYZE TABLE table_name`
- `CHECK TABLE table_name`
  -


# mysql
`--prompt="\u@\h : \d \r:\m:\s> "`

`--tee="./record.txt"` 将所有输入和输出内容都记录进文件。

# mysqladmin
检测 MySQL Server 是否还能正常提供服务:
sky@sky:~# mysqladmin -usky -ppwd -h localhost ping

获取当前 MySQL Server 的几个基本的状态值：
sky@sky:~# mysqladmin -u sky -ppwd -h localhost status

获取当前数据库的连接线程信息：
sky@sky:~# mysqladmin -u sky -ppwd -h localhost processlist

# mysqlimport
csv

# mysqldump

# 权限
SHOW GRANTS FOR 'username'@'hostname'

## 权限级别
1. Global level
  `GRANT SELECT,UPDATE ON *.* TO username@hostname`

2. Database level
  `GRANT SELECT,UPDATE ON database.* TO username@hostname, username2@hostname`

Database Level 的权限授予，可以在当前不存在该
数据库的时候就完成授权。
`SHOW GRANTS FOR username@hostname`

3. Table level
  `GRANT INDEX ON database.table TO username@hostname`

4. Column level
  只支持SELECT、UPDATE、INSERT。
  `GRANT SELECT(id,value) ON database.table TO username@hostname`

5. Routine level
  只支持EXECUTE和ALTER  ROUTINE两种，针对procedure、function。


那内存结构中的权限信息更新之后对已经连接上的用户何时生效呢？
对于 Global Level 的权限信息的修改，仅仅只有更改之后新建连接才会用到，对于已
经连接上的 session 并不会受到影响。而对于 Database Level 的权限信息的修改，只有当
客户端请求执行了“USE database_name”命令之后，才会在重新校验中使用到新的权限信
息。所以有些时候如果在做了比较紧急的Global和Database这两个Level的权限变更之 后 ，
可能需要通过“KILL”命令将已经连接在 MySQL 中的 session 杀掉强迫他们重新连接以使
用更新后的权限。对于 Table Level 和 Column Level 的权限，则会在下一次需要使用到该
权限的 Query 被请求的时候生效，也就是说，对于应用来讲，这两个 Level 的权限，更新之
后立刻就生效了，而不会需要执行“KILL”命令。


# 备份
root@localhost : test 10:02:02> SELECT * INTO OUTFILE '/tmp/dump.text'
-> FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"'
-> LINES TERMINATED BY '\n'
-> FROM test_outfile limit 100;


mysqldump -uroot -T/tmp/mysqldump test test_outfile --fields-
enclosed-by=\" --fields-terminated-by=,


# 恢复备份

逻辑备份能做什么？不能做什么？
在清楚了如何使用逻辑备份进行相应的恢复之后，我们需要知道我们可以利用这些逻辑
备份做些什么。
1、通过逻辑备份，我们可以通过执行相关 SQL 或者命令将数据库中的相关数据完全恢
复到备份时候所处的状态，而不影响不相关的数据；
2、通过全库的逻辑备份，我们可以在新的 MySQL 环境下完全重建出一个于备份时候完
全一样的数据库，并且不受 MySQL 所处的平台类型限制；
3、通过特定条件的逻辑备份，我们可以将某些特定数据轻松迁移（或者同步）到其他
的 MySQL 或者另外的数据库环境；
4、通过逻辑备份，我们可以仅仅恢复备份集中的部分数据而不需要全部恢复。
在知道了逻辑备份能做什么之后，我们必须还要清楚他不能做什么，这样我们自己才能
清楚的知道这样的一个备份能否满足自己的预期，是否确实是自己想要的。
1、逻辑备份无法让数据恢复到备份时刻以外的任何一个时刻；



## 如果我们是备份的以特殊分隔符分隔的纯数据文本文件
a、第一步和 INSERT 备份文件没有区别，就是将最接近崩溃时刻的备份文件准备好；
b、通过特定工具或者命令将数据导入如到数据库中：
由于数据库结构创建脚本和纯文本数据备份文件分开存放，所以我们首先需要执行数据
库结构创建脚本，然后再导入数据。结构创建脚本的方法和上面第一种备份的恢复测试中的
b 步骤完全一样。
有了数据库结构之后，我们就可以导入备份数据了，如下：
mysqlimport --user=name --password=pwd test --fields-enclosed-by=\" --
fields-terminated-by=, /tmp/test_outfile.txt
或者
LOAD DATA INFILE '/tmp/test_outfile.txt' INTO TABLE test_outfile FIELDS
TERMINATED BY '"' ENCLOSED BY ',';
后面的步骤就和备份文件为 INSERT 语句备份的恢复完全一样了，这里就不再累述。（source backup.sql）

# 热备份
MySQL 自己提供了一个使用程序 mysqlhotcopy，这个程序就是专门用来备份 MyISAM 存
储引擎的。不过如果你有除了 MyISAM 之外的其他非事务性存储引擎，也可以通过合适的参
数设置，或者微调该备份脚本，也都能通过 mysqlhotcopy 程序来完成相应的备份任务，基
本用法如下：
mysqlhotcopy db_name[./table_regex/] [new_db_name | directory]


# 以下几类数据都是不适合在数据库中存放的：
1. 二进制多媒体数据
  将二进制多媒体数据存放在数据库中，一个问题是数据库空间资源耗用非常严重，另一个问题
  是这些数据的存储很消耗数据库主机的 CPU 资源。这种数据主要包括图片，音频、视频和其他一些
  相关的二进制文件。这些数据的处理本不是数据的优势，如果我们硬要将他们塞入数据库，肯定会
  造成数据库的处理资源消耗严重。
2. 流水队列数据
  我们都知道，数据库为了保证事务的安全性（支持事务的存储引擎）以及可恢复性，都是需要
  记录所有变更的日志信息的。而流水队列数据的用途就决定了存放这种数据的表中的数据会不断的
  被 INSERT，UPDATE 和 DELETE，而每一个操作都会生成与之对应的日志信息。在 MySQL 中，如果是支
  持事务的存储引擎，这个日志的产生量更是要翻倍。而如果我们通过一些成熟的第三方队列软件来
  实现这个 Queue 数据的处理功能，性能将会成倍的提升。
3. 超大文本数据
  对于 5.0.3 之前的 MySQL 版本，VARCHAR 类型的数据最长只能存放 255 个字节，如果需要存储更
  长的文本数据到一个字段，我们就必须使用 TEXT 类型（最大可存放 64KB）的字段，甚至是更大的
  LONGTEXT 类型（最大 4GB）。而 TEXT 类型数据的处理性能要远比 VARCHAR 类型数据的处理性能低下
  很多。从 5.0.3 版本开始，VARCHAR 类型的最大长度被调整到 64KB 了，但是当实际数据小于 255
  Bytes 的时候，实际存储空间和实际的数据长度一样，可一旦长度超过 255 Bytes 之后，所占用的存
  储空间就是实际数据长度的两倍。
  所以，超大文本数据存放在数据库中不仅会带来性能低下的问题，还会带来空间占用的浪费问
  题。



## 隔离

- READ UNCOMMITTED 脏读、不可重复读、虚读都可能发生
- READ COMMITTED 防止脏读，不可重复读、虚读可能发生
- REPEATABLE READ 防止脏读、不可重复读，虚读可能发生
- SERIALIZABLE 防止脏读、不可重复读、虚读



脏读：一个事务读到另一个事务中未提交的数据

不可重复读：一个事务读到 另一个事务中已提交的数据

虚读：一个事务读到了另一个事务insert的数据



查看当前隔离级别

`select @@version; select @@tx_isolation;`



更改隔离级别

`set TRANSACTION ISOLATION LEVEL read uncommitted;`





#### 如何监控数据库的并发请求数量

`show global status like 'Threads_running'`



![](http://opkk27k9n.bkt.clouddn.com/18-3-8/85736014.jpg)



### truncate 和 delete, drop的区别

http://www.cnblogs.com/8765h/archive/2011/11/25/2374167.html

相同点：

1.truncate和不带where子句的delete、以及drop都会删除表内的数据。

 

2.drop、truncate都是DDL语句(数据定义语言),执行后会自动提交。

 

不同点：

1. truncate 和 delete 只删除数据不删除表的结构(定义)
   drop 语句将删除表的结构被依赖的约束(constrain)、触发器(trigger)、索引(index)；依赖于该表的存储过程/函数将保留,但是变为 invalid 状态。


1. delete 语句是数据库操作语言(dml)，这个操作会放到 rollback segement 中，事务提交之后才生效；如果有相应的 trigger，执行的时候将被触发。
   truncate、drop 是数据库定义语言(ddl)，操作立即生效，原数据不放到 rollback segment 中，不能回滚，操作不触发 trigger。

3.delete 语句不影响表所占用的 extent，高水线(high watermark)保持原位置不动
drop 语句将表所占用的空间全部释放。
truncate 语句缺省情况下见空间释放到 minextents个 extent，除非使用reuse storage；truncate 会将高水线复位(回到最开始)。

 

4.速度，一般来说: drop> truncate > delete

 

5.安全性：小心使用 drop 和 truncate，尤其没有备份的时候.否则哭都来不及
使用上,想删除部分数据行用 delete，注意带上where子句. 回滚段要足够大.
想删除表,当然用 drop
想保留表而将所有数据删除，如果和事务无关，用truncate即可。如果和事务有关,或者想触发trigger,还是用delete。
如果是整理表内部的碎片，可以用truncate跟上reuse stroage，再重新导入/插入数据。

6.delete是DML语句,不会自动提交。drop/truncate都是DDL语句,执行后会自动提交。

 

7、TRUNCATE   TABLE   在功能上与不带   WHERE   子句的   DELETE   语句相同：二者均删除表中的全部行。但   TRUNCATE   TABLE   比   DELETE   速度快，且使用的系统和事务日志资源少。DELETE   语句每次删除一行，并在事务日志中为所删除的每行记录一项。TRUNCATE   TABLE   通过释放存储表数据所用的数据页来删除数据，并且只在事务日志中记录页的释放。 

 

8、TRUNCATE   TABLE   删除表中的所有行，但表结构及其列、约束、索引等保持不变。新行标识所用的计数值重置为该列的种子。如果想保留标识计数值，请改用   DELETE。如果要删除表定义及其数据，请使用   DROP   TABLE   语句。  
    
9、对于由   FOREIGN   KEY   约束引用的表，不能使用   TRUNCATE   TABLE，而应使用不带   WHERE   子句的   DELETE   语句。由于   TRUNCATE   TABLE   不记录在日志中，所以它不能激活触发器。    

10、TRUNCATE   TABLE   不能用于参与了索引视图的表。  