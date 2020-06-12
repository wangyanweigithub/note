### 其他
1. 根据python版本来选择导入的包
```
if is_py3:
  def u(s):
	return s
else:
  def u(s):
	return s.decode('unicode-escape')
```


2. <font color=red>列表推导式可以有两个循环</font>
```
[(i,j) for i in range(5) for j in range(4)]
```

3. 函数加默认值时，有默认值的参数必须在没有默认值参数的后面，如:def a(a,b,c=1,d=2)，这样，顺序不能乱，赋值时可以都用列表，
	也可以都用字典，如: a(1,3,*[3,4]),a(**{"a":1,"b":2,"c":3,"d":4}),a(1,2,*[3],**{"d":4})

4. 类函数中定义的函数如果有self说明时类实例函数，需要实例对象以后由对象调用，如果类中函数第一个参数时cls，用@classmethod修饰器修饰，说明时类方法。

	1. 注意：@classmethod 仅仅适用于单独的，与类本身的数据结构无关函数，其实用了它的函数，与使用普通函数无异，
		甚至不能在参数里加入self，如果要在其中使用类的数据结构，仍然需要将类实例化一次才可以，所以要小心使用。
		classmethod修饰的函数第一个参数应该是cls，即调用类为第一个参数，而不能为空。
		cls代表是类本身，如果没有用@classmethod装饰器装饰的话，调用带有cls参数的方法时需要把类本身当作参数传进来。

5. enumerate接受列表或数组。
```
def enumerate (collection ): 
  'Generates an indexed series:  (0,coll[0]), (1,coll[1]) ...'       
  i = 0 
  it = iter (collection ) 
  while 1 : 
	   yield (i , it.next ()) 
		   i += 1 
```

6. 要创建一个generator，有很多种方法。第一种方法很简单，只要把一个列表生成式的[]改成()，就创建了一个generator：
```
g = (x * x for x in range(10))
g  // <generator object <genexpr> at 0x11...>
```

7. yield生成器函数:函数是顺序执行，遇到return语句或者最后一行函数语句就返回。而变成generator的函数，在每次调用next()的时候执行，
遇到yield语句返回，再次执行时从上次返回的yield语句处继续执行。
	- **一个函数不管能不能执行到yield，只有有yield这个关键字，解释其就会将它解释为生成器，而不是函数。**
	- **对嵌套yield from 来说，里层的return在外层的yield 的exeption里捕获。yield from 的值是里层协程的return。**


8. 地板除：
	1. **所有的取整都是向左取整进行的**

	2. eg
	```
	1.7 // 2 = 0
	-1.7 // 2 = -1
	```

9. 字典推导
```
my_phrase = ["No", "one", "expects", "the", "Spanish", "Inquisition"]
my_dict = {key: value for value, key in enumerate(my_phrase)}
print(my_dict)
reversed_dict = {value: key for key, value in my_dict.items()}
print(reversed_dict)
```

10. 高阶函数：
	- map
	- from functools import reduce: reduce
	- filter
	- sorted：它可以接受一个key函数来实现自定义的排序，key指定的函数将作用于list的每一个元素上，并根据key函数返回的结果进行排序。

11. 将字典用在列表的场景时，会将字典的key成为使用的列表。

12. tornado.gen 的 coroutine 的实现已经改变了，一下函数不能那样用了
	1. @coroutine decorator的定义如下

	```
	def coroutine(func):
		def start(*args,**kwargs):
			cr = func(*args,**kwargs)
			cr.next()
			return cr
		return start
	```

	2. 这是因为需要使用yield，但可能忘记第一次初始化，next(target)或者target.send(None),所以写来这个装饰器，
	将yield的第一次next加上。
	```
	import time
	from tornado.gen import coroutine
	
	def follow(thefile,target):
		thefile.seek(0,2)   #go to the end of the file
		while True:
			line = thefile.readline()
			if not line:
				time.sleep(0.1)
				continue
				target.send(line)
		
		@coroutine
		def printer():
			while True:
				line = yield
				print(line)
		
		if __name__ == '__main__':
			f = open("t.txt")                                                           
			follow(f,printer())
	```

	3. 协程使用的就是yield关键字方法，像一个生成器。

13. `__slots__` 方法:限制实例的属性
```
class Student(object):
	__slots__ = ('name','age') #用tuple定义允许绑定的属性名称。
```

    1. 注意，`__slots__`定义的属性仅对当前类实例起作用，对继承的子类是不起作用的。子类如果需要，可以重新定义`__slots__`.

15. type 创建类
    
    1. 通过type()函数创建的类和直接写class是完全一样的，因为Python解释器遇到class定义时，仅仅是扫描一下class定义的语法，
	然后调用type()函数创建出class。

			>>> def fn(self, name='world'): # 先定义函数
			...     print('Hello, %s.' % name)
					Hello = type('Hello', (object,), dict(hello=fn)) # 创建Hello class
					h = Hello()
					h.hello()
			Hello, world

    2. 要创建一个class对象，type()函数依次传入3个参数：

		- class的名称；
		- 继承的父类集合，注意Python支持多重继承，如果只有一个父类，别忘了tuple的单元素写法；
		- class的方法名称与函数绑定，这里我们把函数fn绑定到方法名hello上。

16. 可迭代对象指的是内部有一下三个方法之一的对象。
    __iter__()或者__next__()或者__getitem__() 方法

17. python2 定义可迭代对象时对象内部定义next()方法，python3中需定义__next__()方法。

18. 偏函数：
    1. partial函数，它可以重新绑定函数的可选参数，生成一个callable的partial对象：
		它默认绑定了一个参数，可以用key=value的方法，改变那个默认的参数。

			>>>from functools import partial
			>>>int2 = partial(int,2)
			>>>int2('10')
			TypeError:xxxxxxxxxx
    
    2. 在使用partial函数是，必须声明绑定参数的形参名。
    
			>>>int2 = partial(int,base=2)
			>>>int2('10')
			>>>2
    
	3. 可以在调用偏函数时，重载默认绑定的形参时，一定要制定key=value这种格式，否则函数不知道要改变绑定的默认实参。

	    >>>int2('10',3)
	    TypeError: int() takes at most 2 arguments (3 given)
    
		1. 偏函数默认绑定了一个参数
    
				>>>int2('10',base=3)
				>>>3 

19. update_wrapper函数，它可以把被封装函数的``__name__,__module__,__doc__和__dict__``都复制到封装函数去：

20. wraps函数

		def wraps(wrapped,assigned = WRAPPER_ASSIGNMENTS,updated = WRAPPER_UPDATES):
		    return partial(update_wrapper, wrapped=wrapped,
		                     assigned=assigned, updated=updated)

	从wraps的定义可以看出wraps是update_wrapper的偏函数。

			import sys

			debug_log = sys.stderr

			def trace(func):
			  if debug_log:
				def callf(*args, **kwargs):
				  """A wrapper function."""
				  debug_log.write('Calling function: {}\n'.format(func.__name__))
				  res = func(*args, **kwargs)
				  debug_log.write('Return value: {}\n'.format(res))
				  return res
				return callf
			  else:
				return func

			@trace
			def square(x):
			  """Calculate the square of the given number."""
			  return x * x

			>> import square
			>> help(square)  #print(square.__doc__)
			calf(*args,**kwargs)
				A wrapper function.

    装饰器是返回一个函数，然后把函数重新复制给原函数名，即：

        def _square(x):
            return x * x
        square = trace(_square)

    所以使用过装饰器以后，原函数的__name__,__doc__ ....一类的环境变量会变成装饰器的环境变量，
	使用@functools.wraps(func)装饰器修饰返回的函数时，可以将原函数的环境变量赋予装饰器本身的环境变量。
        
21. 给一类的实例绑定一个方法需要使用MethodType方法
    
 	eg.

        >>>def set_age(self,age):
                self.age = age
        >>>from types import MethodType
        >>>s.set_age = MethodType(set_age,s)
        >>>s.set_age(25)
        >>>s.age
        25

    如果不是用MethodType方法而直接将s.set_age=set_age会导致调用方法时self参数也需要指定对象，即set_age方法只是一个普通函数，不会自动指定self参数。
    而且s.set_age只是对set_age方法的引用，他们指针位置相同。

	    >>>a.set_age
	    <function set_age at 0x10493cbf8>
	    >>>set_age
	    <function set_age at 0x10493cbf8>

	1. 注意：但是如果用MethodType绑定方法到一个实例中，那其他的实例是不起作用的，如果想给所有实例都绑定方法，可以给class绑定方法：
	
    		>>>Student.set_score = MethodType(set_score,Student)

    通常情况下，这些可以直接定义在class中，但动态绑定允许我们在程序运行过程中动态的添加功能。

22. 如果一个类想被用于for ... in 循环，类似list或tuple那样，就必须实现一个 __iter__方法， 该方法返回一个迭代对象，
   然后python的for循环就会不断调用该迭代对象的__next__()方法拿到循环的下一个值。

23. ``__getitem__``

> 按照下标取出元素，需要实现__getitem__()方法：

		class Fib(object):
			def __getitem__(self,n):
				a,b = 1,1
				for x in range(n):
					a,b = b,a+b
				return a

24. 任何类，只需要定义一个``__call__``()方法，就可以直接对实例进行调用。

25. yield

	1. x = yield y相当于两句话
    
	2. **阻塞在yield 后，send 发送数据重新唤醒生成器，并将新的数据复制给变量**

			def loop1():
			  """ 循环1负责抛出一个函数和对应的参数, 并接收结果
			  """
			  a = 0
			  ret = 1
			  while True:
			  //!!!!!!!!!!!此处相当于两句话，如果next()调用的是第一句话，yeild sum,[a,ret],执行完这句话阻塞程序。
			  //然后再次send()方法调用第二句话，ret ＝yield，此处不阻塞程序。
				ret = yield sum, [a, ret] 
				a, ret = ret, a
				print("Loop1 ret", ret)

			def loop2():
			  """ 循环2 负责接收函数并计算结果, 然后 yield 出结果
			  """
			  while True:
				func, args = yield
				print('yield 1')
				yield func(args)
				print('yield 2')
				print("Loop2")

			l1 = loop1()
			l2 = loop2()
			tmp = next(l1)
			
			for i in range(10):
			  next(l2)
			  ret = l2.send(tmp)
			  tmp = l1.send(ret) 

26. json.dumps

    1. python 中的字符串分普通字符串和unicode字符串，一般从数据库读取的字符串会自动被转换为unicode字符串
		该函数默认转化为普通字符，如果转化的对象中有unicode值，需要加上一个参数，即json.dumps(obj,ensure_ascii=False)

    2. default
		default是一个函数，返回将对象转化为json的方法。这个函数的对象是要转化的可迭代对象中的一个。
		如果是dump字典，则作用在字典的value上，key必须是不可变对象。

			c={'b': 'hello', 'a': datetime.datetime(2016, 1, 4, 22, 54, 14, 868667)}
			def d(obj):
			...   if isinstance(obj,datetime.datetime):
			...     return obj.strftime('%Y-%m-%d %H:%M:%S')
			...   else: 
			...     raise 'type error'
			e = json.dumps(c,default=d)
			e
			'{"b": "hello", "a": "2016-01-04 22:54:14"}'

        所以可以用来转化哪些字典或列表或其他对象中的不规范的对象。
		default定义的函数似乎只有在不是dumps能解析的常规类型才会去调用，如果不是函数定义的几种类型，可以最后加一个类型报错。

27. 协程时使用send方法，生成器主要使用next方法，协程不是异步，除非协程调用的是异步io，异步io需要系统底层支持。

29. ``__file__``
	1. 当"print os.path.dirname(__file__)"所在脚本是以完整路径被运行的， 那么将输出该脚本所在的完整路径，就是文件所在的目录，比如：

             python d:/pythonSrc/test/test.py

             那么将输出 d:/pythonSrc/test

	2. 当"print os.path.dirname(__file__)"所在脚本是以相对路径被运行的， 那么将输出空目录，比如：

         python test.py

         那么将输出空字符串

	3. sys.path[0] 也可以输出文件运行的目录。它不管运行的目录时绝对路径还是相对路径，都输出绝对路径。

30. 类的``__dict__``方法，注意这个方法不能把继承类的所有方法输出为字典。

        class Fruit:
            price = 0
            __a = 22
            def __init__(self):
                self.__color = "red"
                self.__price = 11

        class Apple(Fruit):             # Apple继承了Fruit
            __b = 10
            c = 33  

        if __name__ == "__main__":
            apple = Apple()

            print apple.__dict__ 

	只有color,price是用self来操作的，表示他们并不是类属性，而是实例属性。apple=Apple()中apple就是实例，Apple是类。
	apple.__dict__显示的是apple这个实例的属性，就是用self修饰的属性，也就是在父类中的__color和__price。

	你可以试试print Apple.__dict__ 坐下对比

34. super 不代表父类，super指的是MRO中的下一个类。

	    def super(cls,inst):
	        mro = inst__class__.mro()
	        return mro[mro.index(cls) + 1]

    eg.

        class Root(object):
            pass
        class B(Root):
            pass
        class C(Root):
            pass
        class D(B,C):
            pass
        d = D()
        print(d.__class__.mro())

        >>>(<class '__main__.D'>, <class '__main__.B'>, <class '__main__.C'>, <class '__main__.Root'>, <type 'object'>)

38. 花三分钟接入ssl证书

	http://www.tuicool.com/articles/mU7nErr

39. 正则表达式
	1. 常用零宽断言语法

		- (?=exp) 匹配exp前面的位置
		- (?<=exp) 匹配exp后面的位置
		- (?!exp) 匹配后面跟的不是exp的位置
		- (?<!exp) 匹配前面不是exp的位置

39. 异步IO和阻塞IO

	1. 同步IO和异步IO的区别就在于：数据拷贝的时候进程是否阻塞！

	2. 阻塞IO和非阻塞IO的区别就在于：应用程序的调用是否立即返回！


41. 类方法的隐含调用参数是类，而类实例方法的隐含调用参数是类的实例，静态方法没有隐含调用参数


47. **open不指明选项，默认是只读**。这里注意，如果写的话会出错


48. 函数如果没有返回值类型，没有返回值时，会默认返回None。

49. 函数里的return如果返回多于一个值，他们是组装成一个tupple的，是不可变的。

50. Lists are mutable and can't be used as dict keys,数组是可变的，所有数组不能作为字典的key。

		unhashable type: 'dict'
		unhashable type: 'list'

51. Python中对可变变量和不可变变量封装的很好，
	1. 如，函数返回多个结果，他就封装成了tupple，
	2. list不能做字典的key，所以，在需要不可变对象时，肯定不能会是list，dict等可变对象，
	3. 需要可变对象时，也肯定不能使不可变对象，如tupple。

52. 工厂函数如list()，dict()等函数的拷贝其实是一种浅拷贝。 

53. 列表的+= 和 = a + b

	1. 
			a = [1,2,3]
			id(a)   //139918095676872
			b = ['end']
			a = a + b  //相当于将a+b，然后将新产生的列表付给另外一个变量，这个a的id和上面那个a的id不同。
			id(a)   //139918095677384

	2. 
			a = [1,2,3]
			id(a)     //139918095676872
			b = ['end']
			a += b
			id(a)     //139918095676872

	3. 
		- += 表示左边的变量还是原来的变量，改变它的值。
		- = a + b 表示算出一个新的值，然后赋予一个新的变量，左边的变量的id也变了

54. 字符串可以用下标访问字符，但是不能赋值。

	    name = 'snow storm'
	    name[0] = 's'
	    name[0] = 'h'  //error ，字符串是不可变对象。不可以按下标赋值。

54. python中不要用++和--，i++会报错，++i不会报错但是不会改变i的值。
