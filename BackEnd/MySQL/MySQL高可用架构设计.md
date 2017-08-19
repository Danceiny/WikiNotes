MySQL复制功能分担读负载
推荐：二进制日志格式 binlog_format=ROW. MIXED（基于行和基于短的混合，由系统决定）

选择建议：mixed / row，如果内网row，还需binlog_row_image=minimal.

基于SQL语名的复制（SBR）：基于段；不要求主从数据库表定义完全相同；

基于行的复制（RBR）：可应用于任何SQL复制包括非确定函数、存储过程等；减少从数据库锁的使用。

mysqldump --master-data=2 -single-transaction
混合innodb和myisam则须加allow-table？参数