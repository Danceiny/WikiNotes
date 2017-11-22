```python
global base_db_models
@Singleton
class DBModelFactory():
    def __init__(self):
        global base_db_models
        if base_db_models == None:
            base_db_models = {}

            db_model = self.create_db_model(init_db_config)
            sql = "select `value` from `%s` where `key` = '%s' " % (init_db_config['init_config_table'], init_db_config['init_config_key'])
            db_configs = db_model.GetOne(sql)

            assert (db_configs != None,"db_model is none type")
            db_configs = json.loads(db_configs['value'])

            for config in db_configs:
                self.create_db_model(config)
        # @db_config
    #   name : 数据模型名字，用于区分多个数据库的连接
    #   host : 数据库写地址
    #   host_s : 数据库只读地址，是一个数组
    #   user : 数据库用户名
    #   passwd : 数据库密码
    #   db_name : 数据库名称
    #   port : 数据库端口，默认为3306
    #   charset : 数据库字体集，默认为utf8
    #   cursorclass : 取数据时游标类型，默认为pymysql.cursors.DictCursor（可以自动转为Key-Value的结果集）
    def create_db_model(self,db_config):
        global base_db_models
        db_name = db_config.get('name', 'default')
        if not base_db_models.has_key(db_name):
            # 这个数据操作模型未创建
            base_db_models[db_name] = db_config

        return self.get_db_model(db_name)

    # name : 数据模型名字，用于区分多个数据库的连接
    # readonly : 用来标识要是获取可读写还是只读的数据模型
    def get_db_model(self,name='default', readonly=False):
        global base_db_models
        model = base_db_models.get(name, base_db_models.get('default', None))

        if model is None:
            return None
        db_type = 'db_object' + (readonly and "_r" or "")
        if not model.has_key(db_type):
            model[db_type] = DBModel(model,readonly)

        return model[db_type]

class DBModel():
    # 随机选取只读数据库连接
    @staticmethod
    def choose_mysql_host_s(hosts):
        num = random.randint(0, 100)
        total = 0
        for index in range(0, len(hosts)):
            total += hosts[index]['rate']
            if num <= total:
                return hosts[index]['host']

        return hosts[len(hosts)-1]['host']
    '''
    省略若干orm辅助函数
    '''
    def _generate_sqls(self, sql, options=None):
        '''
        ##TABLE_INDEX##实现分表
        '''
        if options == None or "##TABLE_INDEX##" not in sql:
            return [sql]
            
        if options.get('table_index') != None:
            return [sql.replace("##TABLE_INDEX##", str(options['table_index']))]
        elif options.get('table_count') != None:
            sqls = []
            if options.get('desc', True) == True:
                range_count = range(options['table_count']-1, -1, -1)
            else:
                range_count = range(0, options['table_count'])
            for table_index in range_count:
                sqls.append(sql.replace("##TABLE_INDEX##", str(table_index)))
            return sqls
        elif options.get('table_index_list') != None and type(options['table_index_list']) == list:
            sqls = []
            for index in range(0,len(options['table_index_list'])):
                table_index = options['table_index_list'][index]
                sqls.append(sql.replace("##TABLE_INDEX##", str(table_index)))
            return sqls
        else:
            return [sql]


##### 使用 ######
@Singleton
class XXCenter(object):
    def __init__(self):
        self.conf = ConfigCenter.instance().get_parser('XXCenter')
        self.db_model = DBModelFactory.instance().get_db_model()
        self.db_model_read = DBModelFactory.instance().get_db_model(readonly=True)

        self.table_name = self.conf.get('RDS', 'table_name_exam')
        self.table_name_count = self.conf.getint('RDS', 'table_name_xx_count')
        self.table_name_keys = json.loads(self.conf.get('RDS', 'table_name_xx_keys'))

    def get(self):
        sql_and_dict = {}
        sql = DBModel.sql_select(self.table_name, 
            where=DBModel.sql_and(sql_and_dict))
        dict_list = self.db_model_read.GetList(sql, 
            options={"table_count":self.table_name_count})
    def add(self):
        sql = DBModel.sql_insert(self.table_name, 
            data,
            replace=True, 
            keys=self.table_name_keys)
        flag = self.db_model.execute(sql, 
            options={"table_index":self.table_name_count-1})
```