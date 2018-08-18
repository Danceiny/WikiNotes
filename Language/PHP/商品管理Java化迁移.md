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