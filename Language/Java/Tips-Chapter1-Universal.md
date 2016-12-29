# Intro
《编写高质量代码：改善Java程序的151个建议》

# 不要在常量和变量中出现易混淆的字母
1 l
0 O
...

# 不要让常量蜕变成变量
常量必须在编译器确定其值，而非运行期。
```Java
public class Client{
    public static void main(String[] args){
        System.out.println("const can change~"+Const.RAND_CONST);
    }
}

interface Const{
    public static final int RAND_CONST = new Random().nextInt();
}
```
# 三元操作符的类型必须一致

# 避免带有变长参数的方法重载

# 别让null值和空值威胁到变长方法
```Java
public class Client{
    public void methodA(String str, Integer...is){}
    public void methodA(String str, String...strs){}
    public static void main(String[] args){
        Client client = new Client();
        client.methodA("China",0);
        client.methodA("China","People");
        client.methodA("China");    // can not compile
        client.methodA("China",null);    // can not compile
    }
}
```

# 覆写（override）变长方法
override必须满足：
1. 重写方法不能缩小访问权限；
2. 参数列表必须与被重写方法相同；
3. 返回类型必须是被重写方法的子集（相同或子类）
4. 重写方法不能抛出新的异常（超出父类范围）

```java
public class Client{
    public static void main(String[] args) {
        // 向上转型
        Base base  = new Sub();
        base.fun(100,50);    //调用子类方法Sub.fun，50作自动类型转换{50}

        // 不转型
        Sub sub = new Sub();
        sub.fun(100,50);// compile error， 50不是int[]类型
    }
}

class Base{
    void fun(int price, int...discounts){
        System.out.println("Base...fun");
    }
}
class Sub extends Base{
    @Override
    void fun(int price, int[] discounts){
        System.out.println("Sub...fun");
    }
}
```

# 警惕自增陷阱
Java/PHP中：
`int count = 0; count = count++` ====>>>> count = 0 !!!!!

C++中：
`count=count++`与`count++`等效。

# 少用静态导入
类似于Python中`from numpy import *`这样的少用为妙。

- 不使用*通配符，除非是导入静态常量类（只包含常量的类或接口）。
- 方法名是具有明确、清晰表象意义的工具类。

# 不要在本类中覆盖静态导入的变量和方法
如覆盖，将优先使用覆盖在上面的东东（后来者居上 or 最短路径原则）。

# 显式声明UID
类实现Serializable接口的目的是可持久化，比如网络传输或本地存储，为系统的分布和异构部署提供先决支持条件。

UID（SerialVersionUID），流标示符（Stream Unique Identifier），可以显式或隐式声明，用以处理序列化和反序列化的类不一致的情形。

显式声明UID后，一端更新代码（添加类成员），另一端也能正常运行，只是不能读到改动部分，这就通过“向JVM撒谎”实现了版本向上兼容。

# 避免用序列化类在构造函数中为不变量赋值
 **final变量有一种较另类的赋值方式：构造函数。**

反序列化规则：
**反序列化时构造函数不会执行。**

**因此，在序列化类中不应使用构造函数为final变量赋值。**
```java
public class Person implements Serializable {
    private static final long SerialVersionUID = 91343523L;
    public final String name;
    public Person(){
        name = "huanshimowang"; // later modified as "detianshi"
    }
}

public class Serialize {
    public static void main(String[] args) {
        SerializationUtils.writeObject(new Person());
    }
}

public class Deserialize{
    public static void main(String[] args) {
        Person p = (Person)SerializationUtils.readObject();
        System.out.println(p.name); // print huanshimowang
    }
}
```

# 避免通过方法为final变量赋值

final会被重新赋值，其中的值指的是简单对象，包括8个基本类型、数组、字符串（情况较复杂，不通过new的话和基本类型相同），但是不能通过方法赋值。

总结就是，反序列化时，final变量在以下情况不会被重新赋值：
- 构造函数；
- 方法；
- final修饰的属性不是基本类型。


# 使用序列化类的私有方法解决部分属性持久化问题
//TODO

# 使用脚本语言实现易变业务
C:/model.js
```javascript
function formula(var1,var2)
    return var1 + var2*factor;   
}
```
```java
public static void main(String[] args) throws Exception{
    ScriptEngine engine = new ScriptEngineManager().getEngineByName("javascript");

    //建立上下文变量
    Bindings bind = engine.createBindings();
    bind.put("factor",1);

    //绑定上下文，作用域是当前引擎范围
    engine.setBindings(bind,ScriptContext.ENGINE_SCOPE);
    Scanner input = new Scanner(System.in);
    while(input.hasNextInt()){
        int first = input.nextInt();
        int sec = input.nextInt();
        System.out.println("输入参数是："+first+", "+sec);
        //执行js代码
        engine.eval(new FileReader("C:\Users\huangzhen\Documents\dan\model.js"));
        //是否可调用方法
        if(engine isinstanceof Invocable){
            Invocable in = (Invocable) engine;
            //执行js中的函数
            Double result = (Double)in.invokeFunction("formula",first,sec);
            System.out.println("运算结果："+result.intValue());
        }
    }
}
```

# 慎用动态编译
```Java
public class Client{
    public static void main(String[] args) throws Exception{
        // Java source code
        String sourceStr = "public class Hello{public String sayHello(String name)
        {return\"Hello,\"+name+\"!\";}}";

        // class name and file name
        String clsName = "Hello";
        // method name
        String methodName = "sayHello";
        // current compiler
        JavaCompiler cmp = ToolProvider.getSystemJavaCompiler();

        // Java std file manager
        StandardJavaFileManager fm = cmp.getStandardFileManager(null,null,null);
        // Java file obj
        JavaFileObject jfo = new StringJavaObject(clsName, sourceStr);
        // compile parameters
        List<String> optionsList = new ArrayList<String>();
        // the place to store compile files. only for eclipse
        optionsList.addAll(Arrays.asList("-d","./bin"));

        // the unit to compile
        List<JavaFileObject> jfos = Arrays.asList(jfo);
        //set up compile env
        JavaCompiler.CompilationTask task = cmp.getTask(null,fm,null,optionsList,null,jfos);
        // compile succeeded
        if(task.call()){
            Object obj = Class.forName(clsName).newInstance();
            Class<?extends Object> cls = obj.getClass();
            // invoke sayHello method
            Method m = cls.getMethod(methodName,String.class);
            String str = (String)m.invoke(obj,"Dynamic Compilation");
            System.out.println(str);
        }

    }
}

// Java obj in text file
class StringJavaObject extends SimpleJavaFileObject{
    private String content = "";
    public StringJavaObject(String_javaFileName,String_content){
        super(_createStringJavaObjectUri(_javaFileName),Kind.SOURCE);
        content = _content;
    }

    // make a URL
    private static URI_createStringJavaObjectUri(String name){
        return URI.create("String:///"+name+Kind.SOURCE.extension);
    }

    @Override
    public CharSequence getCharContent(boolean ignoreEncodingErrors) throws IOException{
        return content;
    }
}
```

只要是符合Java规范的就都可以在运行期动态加载，其实现方式就是实现JavaFileObject接口，重写各种方法。

即使需要动态编译，也有很好的替代方案，例如Groovy、JRuby等。

使用动态编译时应注意：
1. 在框架中谨慎使用；
2. 不要在要求高性能的项目使用；
3. 要考虑安全问题；
4. 记录动态编译过程；



# 避免instanceof非预期结果
左侧是null就直接返回false，做强制类型转换也没用。

# 断言场景
断言在下面两种情况不宜使用：
1. 对外公开的方法中；
2. 执行逻辑代码的情况下；

使用场景：
1. 私有方法中放置assert作为输入参数的校验；
2. 流程控制中不可能达到的区域；
3. 建立程序探针；


# 不要只替换一个类
常量类：
```Java
public class Constant{
    public final static int MAX_AGE = 150;
}
```
引用：
```Java
public class Client{
    public static void main(String[] args) {
        System.out.println(""+Constant.MAX_AGE);
    }
}
```

修改Constant类的MAX_AGE，并重新编译。运行Client类，结果没变化！（排除智能IDE影响）

原理分析：
>对于final修饰的基本类型和String类型，编译器认为是稳定态(Immutable Status)，所以在编译时就直接把值编译到字节码中了，避免了运行期引用(Run-time Reference)。只要不重新编译Client类，运行Client类的字节码还是没有变，所以输出也就不会变了。

**下一章：基本类型**
