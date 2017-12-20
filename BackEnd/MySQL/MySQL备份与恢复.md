使用mysqldump / xtrabackup 进行全备和恢复。
Xtrabackup更适用于innodb引擎。



`mysqldump —single-transaction —master-data —triggers —routines —all-databases -uroot -p >> all.sql`





## 基于GTID复制

GTID，全局事务ID。



### 影响主从延迟的因素

- 二进制日志传输时间
- 默认情况下 从 只有一个SQL线程，主 上并发的修改在 从 上变成了串行。所以要配置多线程复制。



### MMM架构（需要安装）

Master-master-monitor

配置文件：`/etc/mysql-mmm/`



监控用户：`grant replication client on *.* to 'mmm_monitor'@'192.168.3.%' identified by '123';`

代理用户：`grant super, replication, client, process on *.* to 'mmm_agent'@'192.168.3.%' identified by '123';`

复制用户：



- 不支持读负载均衡
- 不支持MySQL新复制功能
- 进行主从切换时，易丢失数据
- 监控服务存在单点故障

### MHA （Master High Availability）

