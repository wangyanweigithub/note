## logging
1. user -> 记录器 ->过滤器-> 格式化 -> 处理程序 -> 适当的目标

2. 日志库采用模块化方法，并提供几类组件：记录器、处理程序、过滤器和格式化程序。

3. root是所有logger的父类,其他logger是子类, basicConfig配置的是root的配置项.如果不关闭向上传递,所有的logger的
消息都会传递到它的所有父类.

4. 如果logger没有配置,会传递给父类,使用父类的配置.

5. logging中的root和logger,不仅是单例,还是继承关系,而且会向父类传递.
**如果一个logger某些配置或者handler没有配置,会使用父类的配置**

### basicConfig
1. **使用标准库提提供的 logging API 最主要的好处是，所有的 Python 模块都可
能参与日志输出，包括你的日志消息和第三方模块的日志消息**。

2. 在一个模块中定义了basiConfig,其他模块也会使用同样的配置,
**是因为logging源文件对所有的log实现了一个单例的root**,如果直接使用logging.debug的话,
实际使用的root.debug,而root是一个单例,任何一个地方配置了这个对象,所有模块都会影响.

3. basiConfig只能配置一次,第二次会忽略.因为root是单例模式.

### handler

### 在记录消息上添加额外信息
1. 使用LoggerAdapter, 重定义process方法.
```
   class CustomAdapter(logging.LoggerAdapter):
       """
       This example adapter expects the passed in dict-like object to have a
       'connid' key, whose value in brackets is prepended to the log message.
       """
       def process(self, msg, kwargs):
           return '[%s] %s' % (self.extra['connid'], msg), kwargs
```

2. 使用Filter,修改继承的类的filter方法,在record对象中添加额外信息作为成员.
```
class ContextFilter(logging.Filter):

   USERS = ['jim', 'fred', 'sheila']
   IPS = ['123.231.231.123', '127.0.0.1', '192.168.0.1']

   def filter(self, record):

	   record.ip = choice(ContextFilter.IPS)
	   record.user = choice(ContextFilter.USERS)
	   return True
```
