#### 数据库迁移
从正式服到测试服。测试服相当于裸机。mysql版本，dump并scp发给测试服，测试服创建对应的数据库，导入scp得到的dump文件（sql）。

#### Python虚拟环境
Server OS: Ubunt 14.04
- virtualenv方案。
- `pip install -r requirements`: `build/temp.linux-x86_64-2.7/_openssl.c:12:24: fatal error: pyconfig.h: No such file or directory`. Solution: `apt-get install python-dev libssl-dev`

运行时：`No module named MySQLdb`. Solution: `sudo apt-get install python-pip python-dev libmysqlclient-dev python-mysqldb &&  pip install MySQL-python`.


#### TASK LIST
1. /api/v1/projects, module filter by module.family_id
2.  


CREATE TABLE IF NOT EXISTS `project`(
`id` INT AUTO_INCREMENT,
   `date_created` INT UNSIGNED AUTO_INCREMENT,
   `date_modified` DATE,
   `is_deleted` INT,
   `name` VARCHAR(128),
   `description` VARCHAR(64),
   `default_env` VARCHAR(64),
   `family_id` VARCHAR(64),
   `latest_version` Float(32),
   `latest_version` VARCHAR(32),
   PRIMARY KEY ( `id` )
)ENGINE=InnoDB DEFAULT CHARSET=utf8;


#### Celery Priority, How to define and set up?
必读：http://docs.celeryproject.org/en/latest/faq.html

优先级分类
task + queue + worker + redis/ambq

task priority 
```python
@celery_app.task
def fun():
   pass
PRIORITY = random.randInt(0,9)   # do machine learning here
fun.apply_async(priority=PRIORITY)
```


queue priority

worker priority

redis priority
