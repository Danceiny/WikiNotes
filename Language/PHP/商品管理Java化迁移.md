## 目前架构

- view
  - php-blade
    - /manage/product/index
    - publishProduct 
      - /views/manage/product/publish
      - Middleware
        - product_verify(ProductPermissionVerify)
          - 这个中间件直接通过httpquery的id参数查询product 再绑定到request对象上(还得检查是否有编辑权限)
  - js
    - （import by phpblade）
-  controller（web.php => ManageController@）
  - POST addProduct
    - body参数
      - price
      - stock
      - desc
      - pictures
      - name
      - cover
      - paymentType (根据request对象，在php代码中生成)
    - 调用
      - `Product::createSingleSkuProduct`
        - 涉及到的数据表（以下全都是mysql）
          - ProductTag
          - ProductTagValue
          - Sku
          - SkuTagValue
  - PUT updateProduct
    - get参数
      - id
    - body参数
      - price
      - stock
      - desc
      - pictures
      - name
      - cover
      - paymentType (根据request对象，在php代码中生成)
    - 调用
      - product->updateProps
        - 涉及到
          - sku
          - product
  - DELETE removeProduct
    - middleware
      - products_verify(ProductsPermissionVerify)
    - 接受ids数组，直接删除



总结迁移方案如下：

- view层，包括中间件，仍然留在php，先迁移上述controller下面的三个增删改方法，即将js中的api调用直接指向javaapi。

## javaapi项目架构







Error starting ApplicationContext. To display the conditions report re-run your application with 'debug' enabled.
2018-08-15 17:30:27.716 ERROR [bxapplets-javaapi-service,,,] 59100 --- [           main] o.s.b.d.LoggingFailureAnalysisReporter   : 

***************************
APPLICATION FAILED TO START
***************************

Description:

The dependencies of some of the beans in the application context form a cycle:

   productApiController defined in file [/Users/huangzhen/dev/baixing/bxapplets-javaapi/target/classes/com/baixing/bxapplets/javaapi/endpoint/ProductApiController.class]
      ↓
   productServiceImpl defined in file [/Users/huangzhen/dev/baixing/bxapplets-javaapi/target/classes/com/baixing/bxapplets/javaapi/service/impl/ProductServiceImpl.class]
┌─────┐
|  productDaoImpl defined in file [/Users/huangzhen/dev/baixing/bxapplets-javaapi/target/classes/com/baixing/bxapplets/javaapi/dao/impl/ProductDaoImpl.class]
└─────┘





## 一些临时笔记

- dev

  - 192.168.0.7

  - 192.168.0.2

- online
  - 192.168.0.10
- username
  - deploy
- password
  - Feel like BBQ @@





trade回调的params:

```
{
	"callbackId": "b3ebeb61-4b47-4c3a-ac8e-6b861eee3fc3",
	"tradeId": "400600162018090788000006",
	"type": "offline",
	"userId": 16,
	"merchantId": "5b31fb4a5b225100071fe9c3",
	"status": 100,
	"originalPrice": 10,
	"dealPrice": 10,
	"timestamp": 1536550849120,
	"trade": {
		"id": "400600162018090788000006",
		"mchId": "5b31fb4a5b225100071fe9c3",
		"usrId": 16,
		"status": 100,
		"type": "offline",
		"originalPrice": 10,
		"dealPrice": 10,
		"skuList": [{
			"skuId": 656,
			"quantity": 1,
			"originalPrice": 10,
			"dealPrice": 10,
			"snapshot": "{\"product\":{\"id\":673,\"name\":\"刷呀刷呀\",\"desc\":\"刷～\",\"meta\":\"[]\",\"site_id\":\"5b45ca45423ca200083a6484\",\"img\":\"[\\\"\/\/img-weapp.baixing.net\/FtLAnWOybgqrG4D8ddaSP3EUE7vN_640p\\\"]\",\"video\":\"[]\",\"created_at\":\"2018-08-28 09:20:20\",\"updated_at\":\"2018-08-28 09:20:20\",\"cover\":0,\"payment_type\":\"[1, 0]\"},\"tagValue\":[{\"id\":665,\"value\":\"default\",\"created_at\":\"2018-08-28 09:20:20\",\"updated_at\":\"2018-08-28 09:20:20\",\"pivot\":{\"sku_id\":656,\"tag_value_id\":665}}]}"
		}],
		"logistics": "{\"name\":\"王路路\",\"phone\":\"18302126856\",\"address\":\"上海市上海市徐汇区虹梅路2007号远中产业园1号楼8楼百姓网\"}",
		"paymentList": ["{\"platform\":\"applet\",\"payments\":{\"offline\":{\"type\":\"offline\",\"amount\":10}},\"extra\":\"\",\"tradeDesc\":\"购买商品：刷呀刷呀\",\"memo\":\"商品-刷呀刷呀\",\"timestamp\":0}"],
		"comments": [],
		"coupons": null,
		"refundList": null,
		"createdTime": 1536313935000,
		"modifiedTime": 1536550849000,
		"extra": {
			"paidTime": null,
			"payExpireTime": null,
			"deliveredTime": 1536313963819,
			"canceledTime": null,
			"canceledBy": null,
			"finishedTime": 1536550849116,
			"autoFinishKey": "1408716a-8fc5-47a5-9d3e-2d445c3ac84e"
		}
	}
}
```





- 退订回T
- 医美+安建新二选一在通道服务中实现，面试邀请用安建新，先不移。实现医美。
- 分类的通知提醒，source的前缀是“运营”、“市场”
- 促销到账通知、一卡通过期提醒？、促销？







|            | 苏州火车站     | 拙政园         |      |      |      |
| ---------- | -------------- | -------------- | ---- | ---- | ---- |
| 苏州火车站 | (0km, 0min)    | (2.5km, 20min) |      |      |      |
| 拙政园     | (2.5km, 20min) | (0km, 0min)    |      |      |      |
|            |                |                |      |      |      |
|            |                |                |      |      |      |



```
		$smsConfig = \Config::get("notification.bxapplets.userNotification");
		$srcId = $smsConfig['srcId'];
		$mobiles = [$mobile];
		$smsType = $smsConfig['type'];
		$templateId = $smsConfig['templateId'];
		$signId = $smsConfig['signId'];
		$contentParams = [$content];
```



1. 营收_新手礼

- 部门业务名称管理 

1. 营收_返券通知
2. 营收_一卡通过期提醒