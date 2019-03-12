##  2018-12-02

DONE

- 读了镐京里muye,listing/powerlisting相关的部分代码

TODO

- 新牧野系统的设计草案：回滚，Meta/Schema服务，db/cache等

THINKING

- 镐京的代码还是博大精深啊，一些php“花式”写法看起来还有点吃力，业务上的hardcode影响阅读，但是应该不是主因，还是得**提高抽丝剥茧的阅读能力**。突然想起一开始学代码的时候，一位师兄说读代码比写代码难，当时持反对意见，因为那时候看书里的代码很好懂而自己不会写代码，而现在深以为然啊。

## 2018-12-03

DONE

- nothing

TODO

- 新牧野的详细设计方案
  

THINKING

- 今天主要熟悉并讨论了新牧野系统相关的一些服务边界以及设计草案，一个感受就是要重构老代码，不熟悉业务的话会有点“难上加难”。之前以为新牧野是要一步步接入到现在的镐京，在设计上就会有非常多的对现状的妥协，然后得知整个镐京要大改，心情大概是“真有勇气啊”，感觉大概是重写一个镐京吧，那些以前没人敢改的代码总要有人改了……对此进程满怀期待。


## 2018-12-04

DONE

- 媒体服务：上线test环境，修复了bootstrap.yml的问题
- 新牧野的dict-service数据库表结构设计以及API设计

TODO

- 完善dict-service的相关设计

THINKING

- dict-service本身是一个足够简单的服务，典型的读多写少场景，而且数据量比较少，主要难点还是在性能层面，这应该是一个蛮有意思的挑战。



## 2018-12-05

DONE

- dict-service评审前的一些细节问题
- 开始dict-service开发：建表 && API && Entity

TODO

- dict-service开发：service/impl

THINKING

- 看到一篇很好的讲缓存的文章：https://tech.meituan.com/cache_about.html，有空要加以实践。
- 收到伟哥的试用期到期邮件了，这周末准备一下转正答辩~

## 2018-12-06

DONE

- 媒体内容服务：测试阶段bug修复，预计下周一准备上线
- dict-service：service的一些实现设计，car查询相关的编码

TODO

- dict-service：car && area

THINKING

- 一个JpaRepository的bean找不到，匪夷所思，也暴露出对java的maven包管理以及spring boot的包扫描等等机制的无知…………革命尚未成功……



## 2018-12-10

DONE

- 字典服务：为car,category,area抽象出一个BaseTreeService，完成了简单的基于数据库的增加和查询；洗了大概一千多条CarType表数据，car的增加和查询接口基本可用。

TODO

- 字典服务：BaseTreeService的迭代；area接口

THINKING

- 暂无

## 2018-12-11

DONE

- 字典服务：给BaseTreeService加上了节点关系缓存以及节点实体数据缓存，增加了节点修改和删除时的相关缓存更新机制；洗了一万多条area数据，area的增加和查询接口基本可用；实现了接口请求数据与响应数据的打平。

TODO

- bugfix + 小区接口

THINKING

- 暂无

## 2018-12-12

DONE

- 字典服务：php洗数据脚本的整理；小区接口的完善；第一次car+area+community全量数据洗入；BaseTreeService缓存相关的完善。

TODO

- 字典服务：节点权重排序的实现

THINKING

- RedisTemplete用来用去还是<String, String>最好用
- fastjson和jackson混用有风险。



## 2018-12-13

DONE

- 拆镐京动员大会
- 字典服务：节点权重排序

TODO

- 字典服务：数据库查询改造；补单元测试

THINKING

- 在mysql中搞一个字段存巨大的json，性能瓶颈还是很明显的，默认的jparepository那一套简单的默认接口应该搞不定，需要自定义一下
- 拆镐京动员大会挺成功的，了解了镐京的历史，建硕讲的“工程师的愤怒”，太形象了哈哈
- 线下这spring项目访问mysql总是很慢，一条sql就要50ms的量级这样子，之前有听闻和连接池数量问题有关，也不知道到底解决没有，很迷，希望听到架构或者有谁能解释一下……
- 今天做排序，一开始的思路是要完全自己写排序，以为可以减少额外的复杂度（比如不用调用库方法排完序又重新遍历去做一些特殊的事情），然后仔细想想，感觉日常业务排序场景的性能瓶颈基本都是来自外部数据访问，也就是说优化空间应该是在a = arr[i]以及arr[i] = a的执行次数上面，那啥几大排序算法什么的基本上用不着操心（Arrays.sort这种DualPivotQuickSort够强了~）。



| 序列长度\耗时（纳秒） | 手写排序Entity（Integer） | Arrays.sort（）int | entity保存次数 |
| --------------------- | ------------------------- | ------------------ | -------------- |
| 1                     | 21644365                  | 3221486            |                |
| 2                     | 4754169                   | 1823               |                |
| 3                     | 183698                    | 1319               |                |
| 5                     | 297772                    | 1971               |                |
| 10                    | 1245709                   | 2640               |                |
| 51                    | 11086665                  | 16162              |                |
| 100                   | 16473797                  | 50738              |                |
| 399                   | 55055219                  | 121423             |                |
| 1001                  | 138695628                 | 312629             |                |
| 10000                 | 1235230918                | 2854737            |                |
| 100000                | 8344704887                | 87511757           |                |

第二次测试：

| 序列长度\耗时（纳秒） | 手写排序Entity（Integer） | Arrays.sort（）int | entity保存次数 |
| --------------------- | ------------------------- | ------------------ | -------------- |
| 1                     | 12563786                  | 187729             | 0              |
| 2                     | 2185549                   | 1814               | 0              |
| 3                     | 1476043                   | 2238               | 1              |
| 5                     | 942407                    | 2328               | 3              |
| 10                    | 1660051                   | 2926               | 10             |
| 51                    | 8089159                   | 14980              | 51             |
| 100                   | 18097219                  | 27454              | 100            |
| 399                   | 41888548                  | 156203             | 399            |
| 1001                  | 136758572                 | 317405             | 1001           |
| 10000                 | 1120263699                | 2424386            | 10000          |
| 100000                | 9853589344                | 10043714           | 99996          |



第三次测试：换用好的数组

| 序列长度\耗时（纳秒） | 手写排序Entity（Integer） | Arrays.sort（）int | entity保存次数 |
| --------------------- | ------------------------- | ------------------ | -------------- |
| 1                     | 12563786                  | 187729             | 0              |
| 2                     | 2185549                   | 1814               | 0              |
| 3                     | 1476043                   | 2238               | 1              |
| 5                     | 942407                    | 2328               | 3              |
| 10                    | 1660051                   | 2926               | 10             |
| 51                    | 8089159                   | 14980              | 51             |
| 100                   | 18097219                  | 27454              | 100            |
| 399                   | 41888548                  | 156203             | 399            |
| 1001                  | 136758572                 | 317405             | 1001           |
| 10000                 | 1120263699                | 2424386            | 10000          |
| 100000                | 9853589344                | 10043714           | 99996          |



## 2018-12-17

DONE

- 字典服务：排序算法的一点优化；service层的单元测试；attr属性的拆分（读写使用裸sql单独处理）

TODO

- 字典服务：swagger文档；继续补ut

THINKING

- 关于attr字段单独从数据库取，感觉有点为难，从现有数据来看，attr字段的量级并没有特别大，也就是小区表会超过10000，而类目表的attr比较小。对于10000这个量级，从解析json字符串这一角度讲，因为json嵌套层次并不深，稍微测一测，在响应时间上感觉不到拆分attr读写有什么好处，但是传输数据量剧减，数据库有缓存之后的响应时间略有降低。
- 关于排序，以最少数据库写请求为目标，有最佳的算法路径，但是想想感觉实现复杂度有点高，现在算是实现了一个初级版。按照我现在的估计，真正产品交互上做的排序调整应该基本是少量改动的，不太可能把顺序改得面目全非，如果是这样的话，现在的排序也很够用了。





```
-Dsonar.coverage.exclusions=$PWD/dict-service-api/src/main/java/com/baixing/dict/api/constant/*,$PWD/dict-service-api/src/main/java/com/baixing/dict/api/req/*,$PWD/dict-service-api/src/main/java/com/baixing/dict/api/resp/*,$PWD/dict-service-common/src/main/java/com/baixing/dict/common/models/*,$PWD/dict-service-common/src/main/java/com/baixing/dict/common/utils/*,$PWD/dict-service-persistence/src/main/java/com/baixing/dict/persistence/entity/interfaces/*,$PWD/dict-service-persistence/src/main/java/com/baixing/dict/persistence/entity/*,$PWD/dict-service-persistence/src/main/java/com/baixing/dict/persistence/repository/*,$PWD/dict-service-web/src/main/java/com/baixing/dict/exception/DictExceptionHandler.java,$PWD/dict-service-web/src/main/java/com/baixing/dict/web/controller/DocumentationController.java,$PWD/dict-service-business/src/main/java/com/baixing/dict/business/redis/*
```