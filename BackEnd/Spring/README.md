
### IoC部分：配置和逻辑的分水岭

1. 实现基本的容器——包括注入和获取功能
- BeanDefinition 保存bean和配置信息

```java
package us.codecraft.tinyioc.beans;

/**
 * bean的内容及元数据，保存在BeanFactory中，包装bean的实体
 * 
 */
public class BeanDefinition {

	private Object bean;

	private Class beanClass;

	private String beanClassName;

	private PropertyValues propertyValues = new PropertyValues();

    ...

    public void setBeanClassName(String beanClassName) {
		this.beanClassName = beanClassName;
		try {
			this.beanClass = Class.forName(beanClassName);
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
	}
}
```

- BeanFactory 对bean进行管理

2. 管理Bean的生命周期
- 抽象BeanFactory 面向接口易扩展
- 在AbstractBeanFactory内部初始化Bean

3. 为Bean注入属性
- PropertyValue 保存属性注入信息
- AutowireCapableBeanFactory 可自动装配的BeanFactory
```java
package us.codecraft.tinyioc.beans.factory;

import us.codecraft.tinyioc.BeanReference;
import us.codecraft.tinyioc.aop.BeanFactoryAware;
import us.codecraft.tinyioc.beans.BeanDefinition;
import us.codecraft.tinyioc.beans.PropertyValue;

import java.lang.reflect.Field;
import java.lang.reflect.Method;

/**
 * 可自动装配内容的BeanFactory
 * 
 * @author yihua.huang@dianping.com
 */
public class AutowireCapableBeanFactory extends AbstractBeanFactory {

	protected void applyPropertyValues(Object bean, BeanDefinition mbd) throws Exception {
		if (bean instanceof BeanFactoryAware) {
			((BeanFactoryAware) bean).setBeanFactory(this);
		}
		for (PropertyValue propertyValue : mbd.getPropertyValues().getPropertyValues()) {
			Object value = propertyValue.getValue();
			if (value instanceof BeanReference) {
				BeanReference beanReference = (BeanReference) value;
				value = getBean(beanReference.getName());
			}

			try {
				Method declaredMethod = bean.getClass().getDeclaredMethod(
						"set" + propertyValue.getName().substring(0, 1).toUpperCase()
								+ propertyValue.getName().substring(1), value.getClass());
				declaredMethod.setAccessible(true);

				declaredMethod.invoke(bean, value);
			} catch (NoSuchMethodException e) {
				Field declaredField = bean.getClass().getDeclaredField(propertyValue.getName());
				declaredField.setAccessible(true);
				declaredField.set(bean, value);
			}
		}
	}
}
```

4. 从xml中读取BeanDefinition
- BeanDefinitionReader
- XMLBeanDefinition 从xml中读取配置
- Resource 定位资源文件

5. 将Bean注入Bean 解析依赖
- BeanReference 保存对Bean的引用
- getBean() 调用createBean() -lazy init

6. 引入ApplicationContext 包装Bean的初始化流程，对引用透明
- BeanFactory =》 ApplicationContext =》AbstractApplicationContext =》ClassPathXmlApplicationContext

- 组合优于继承
- refresh() 变懒加载为提前初始化

### AOP
AOP分为配置(Pointcut，Advice)，织入(Weave)两部分工作，当然还有一部分是将AOP整合到整个容器的生命周期中。 ds
- Pointcut 切哪里

- Aspect - 在哪切
- Advisor - 只有一个advice的Aspect

基本步骤：
<!-- 1. 知道切哪里：定义Pointcut
2. 怎么切：定义Advisor
3. 开始切：Weave -->

7. method-interceptor-by-jdk-dynamic-proxy
从aop织入开始：
- AdvisedSupport 保存aop配置
```java
package com.ysj.tinySpring.aop;

import org.aopalliance.intercept.MethodInterceptor;

/**
 * AdvisedSupport封装了TargetSource, MethodInterceptor和MethodMatcher
 *
 */
public class AdvisedSupport {

	// 要拦截的对象
	private TargetSource targetSource;
		
	/**
	 * 方法拦截器
     * Spring的AOP只支持方法级别的调用，所以其实在AopProxy里，我们只需要将MethodInterceptor放入对象的方法调用
	 */
    private MethodInterceptor methodInterceptor;
    
    // 方法匹配器，判断是否是需要拦截的方法
    private MethodMatcher methodMatcher;
    
    
    
    public TargetSource getTargetSource() {
        return targetSource;
    }

    public void setTargetSource(TargetSource targetSource) {
        this.targetSource = targetSource;
    }

    public MethodInterceptor getMethodInterceptor() {
        return methodInterceptor;
    }

    public void setMethodInterceptor(MethodInterceptor methodInterceptor) {
        this.methodInterceptor = methodInterceptor;
    }

    public MethodMatcher getMethodMatcher() {
        return methodMatcher;
    }

    public void setMethodMatcher(MethodMatcher methodMatcher) {
        this.methodMatcher = methodMatcher;
    }
}
```
- TargetSource 保存被代理的数据

织入和代理：
- AopProxy 对目标对象做代理， 在调用目标方法前先调用它
- JdkDynamicAopProxy 使用JDK动态代理对接口做代理
- ReflectiveMethodInvocation 将反射的Method封装为Joinpoint
