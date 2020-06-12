<!-- vim-markdown-toc GFM -->

* [内置函数]
	* [常规]
	* [高级]
	* [描述器]
	* [编译]
	* [判断类型]
	* [functional]
		* [itertools]
		* [map, filter, reduce]
	* [反射]
* [内置常量]
	* [逻辑值判断]
	* [迭代器]
	* [上下文管理器类型]
	* [模块]
	* [对象方法]
* [容器]
	* [iter]
	* [slice]
	* [dict]
		* [dict 构造函数]
		* [dict视图对象]
	* [frozenset([iterable])]
* [容器类型]
* [其他]
* [易错点]
	* [引用与拷贝]
	* [python 10大易错知识点]

<!-- vim-markdown-toc -->
## 内置函数
### 常规
1. callable(object)
> 如果实参 object 是可调用的,返回True,否则返回False。如果返回真,调用仍可能会失败;但如果返回假,则调用 object 肯定会失败。

	1. **注意类是可调用的(调用类会返回一个新的实例)**。**如果实例的类有 __call__() 方法,则它是可调用。**

2. @classmethod  @staticmedthod

2. globals()
> 返回表示当前全局符号表的字典。总是当前模块的字典。在函数或方法中,不是调用它的模块,而是定义它的模块.

3. local()
> 更新并返回表示当前本地符号表的字典。在函数代码块但不是类代码块中调用locals() 时将返回自由变量。
> 请注意在模块层级上,locals() 和globals() 是同一个字典。

	1. **不要更改此字典的内容;更改不会影响解释器使用的局部变量或自由变量的值。**

4. enumerate(iterable, start=0)	：注意，enumerate有第二个参数。
> 返回一个枚举对象。iterable 必须是一个序列,或iterator,或其他支持迭代的对象。

	1. enumerate() 返回的**迭代器**的**__next__() 方法**返回一个元组,里面包含一个计数值(从 start 开始,默认为 0)和通过迭代 iterable 获得的值。

5. max(iterable, *[, key,default]) / max(arg1, arg2, *args[,key] )
> 返回可迭代对象中最大的元素,或者返回两个及以上实参中最大的。

	1. 如果只提供了一个位置参数,它必须是非空iterable,返回可迭代对象中最大的元素;
	2. 如果提供了两个及以上的位置参数,则返回最大的位置参数。
	3. 有两个可选只能用关键字的实参。 key 实参指定排序函数用的参数,如传给list.sort() 的。 
	4. default 实参是当可迭代对象为空时返回的值。
	5. 如果可迭代对象为空,并且没有给 default ,则会触发ValueError。
	6. 如果有多个最大元素,则此函数将返回第一个找到的。
	7. 这和其他稳定排序工具如 sorted(iterable, key=keyfunc, reverse=True)[0] 和 heapq.nlargest(1, iterable, key=keyfunc) 保持一致。
		3.4 新版功能: keyword-only 实参 default 。

6. min(iterable, *[,key,default]) / min(arg1, arg2, *args[,key])

7. next(iterator[,default])

8. open(file, mode='r', buffering=-1, encoding=None, errors=None, newline=None, closefd=True, opener=None)

9. ord() <-> char()

10. pow(x, y[, z])
> x的y次幂，如果z存在，则对z取余

11. range(stop) / range(start, stop[,step])
> 虽然被称为函数，但ragne实际上是一个不可变的序列类型

### 高级
1. hash()
> 返回对对象的哈希值(如果它有的话)。哈希值是整数。

	1. 它们在字典查找元素时用来快速比较字典的键。

2. memoryview(obj)
> 返回由给定实参创建的”内存视图”对象。

3. super( [ type [ , object-or-type  ] ]  )
> 返回一个代理对象,它会将方法调用委托给 type 指定的父类或兄弟类。这对于访问已在类中被重载的
> 继承方法很有用。搜索顺序与getattr() 所使用的相同,只是 type 指定的类型本身会被跳过

	1. 如果省略第二个参数,则返回的超类对象是未绑定的。
	2. 如果第二个参数为一个对象,则isinstance(obj, type) 必须为真值。
	3. 如果第二个参数为一个类型,则 issubclass(type2,type) 必须为真值(这适用于类方法)。

	4. 两个典型应用
		1. 在具有单继承的类层次结构中，super可用来引用父类而不必显示指定它们的名称。
		2. 在动态执行环境中那支持协作多重继承。python独有，菱形图。

4. class type(name, bases, dict)
> 传入一个参数时,返回 object 的类型。返回值是一个 type 对象,通常与object.__class__ 所返回的对象相同。

	1. 推荐使用isinstance() 内置函数来检测对象的类型,因为它会考虑子类的情况。
	2. 传入三个参数时,返回一个新的 type 对象。这在本质上是 class 语句的一种动态形式。
		1. name 字符串即类名并且会成为__name__ 属性;
		2. bases 元组列出基类并且会成为__bases__ 属性;
		3. 而 dict 字典为包含类主体定义的命名空间并且会被复制到一个标准字典成为__dict__ 属性。
		4. 例如,下面两条语句会创建相同的type 对象:

		```
			>>> class X:
			...
				a = 1
				...
				>>> X = type('X', (object,), dict(a=1))
		```
		5. 在 3.6 版更改: type 的子类如果未重载 type.__new__,将不再能使用一个参数的形式来获取对象的类型。

5. vars( [ object  ]  )
> 返回模块、类、实例或任何其它具有__dict__ 属性的对象的__dict__ 属性。

	1. 不带参数时,vars() 的行为类似locals()。请注意,locals 字典仅对于读取起作用,因为对 locals 字
		典的更新会被忽略。

6. zip(*iterables)
> 创建一个聚合了来自每个可迭代对象中的元素的迭代器。

	1. 返回一个元组的迭代器,其中的第 i 个元组包含来自每个参数序列或可迭代对象的第 i 个元素
	2. 当所输入可迭代对象中最短的一个被耗尽时,迭代器将停止迭代。
	3. 当只有一个可迭代对象参数时,它将返回一个单元组的迭代器。
	4. 不带参数时,它将返回一个空迭代器
		
7. __import__(name, golbals=None, locals=None, fromlist=(), level=0)
	1. importlib.import_module() 不同,这是一个日常 Python 编程中不需要用到的高级函数。
	2. 同样也不建议直接使用__import__() 而应该用importlib.import_module()。
	3. 此函数会由 import 语句发起调用。它可以被替换 (通过导入builtins 模块并赋值给 builtins.
		__import__) 以便修改 import 语句的语义,但是**强烈**不建议这样做

### 描述器
1. class property(fget=None, fset=None, fdel=None, doc=None)
> 返回 property 属性。

	1. fget 是获取属性值的函数。
	2. fset 是用于设置属性值的函数。
	3. fdel 是用于删除属性值的函数。
	4. 并且 doc 为属性对象创建文档字符串。

	5. 一个典型的用法是定义一个托管属性 x:

	```
	class C:
		def __init__(self):
			self._x = None

		def getx(self):
			return self._x

		def setx(self, value):
			self._x = value

		def delx(self):
			del self._x

		x = property(getx, setx, delx, "I'm the 'x' property.")

	```
	6. **如果 c 是C的实例,c.x 将调用getter,c.x = value 将调用 setter,del c.x 将调用 deleter。**

	7. 这令使用property()作为decorator来创建只读属性可以很容易的实现：

		```
		class Parrot:
			def __init__(self):
				self._voltage = 1000

			@property
			def voltage(self):
				return self._voltage
		```

	8. 其他
	
	```
		class C:
			def __init__(self):
				self._x = None

			@property
			def x(self):
				"""I'm the 'x' property."""
				return self._x

			@x.setter
			def x(self, value):
				self._x = value

			@x.deleter
			def x(self):
				del self._x
	``` 

### 编译
1. compile(source, filename, mode, flags=0, dont_inherit=False, optimize=-1)
	1. 将 source 编译成代码或 AST 对象。代码对象可以被exec() 或eval() 执行。source 可以是常规的字符串、字节字符串,或者 AST 对象。
		参见ast 模块的文档了解如何使用 AST 对象。

	2. filename 实参需要是代码读取的文件名;如果代码不需要从文件中读取,可以传入一些可辨识的值(经常会使用 '<string>')。

	3. mode 实参指定了编译代码必须用的模式。
		- 如果 source 是语句序列,可以是 'exec';
		- 如果是单一表达式,可以是 'eval';
		- 如果是单个交互式语句,可以是 'single'。(在最后一种情况下,如果表达式执行结果不是 None 将会被打印出来。)

	4. 可选参数 flags 和 dont_inherit 控制在编译 source 时要用到哪个 future 语句。
		- 如果两者都未提供(或都为零)则会使用调用compile() 的代码中有效的 future 语句来编译代码。
		- 如果给出了 flags 参数但没有 dont_inherit (或是为零) 则 flags 参数所指定的以及那些无论如何都有效的 future 语句会被使用。
		- 如果dont_inherit 为一个非零整数,则只使用 flags 参数 – 在调用外围有效的 future 语句将被忽略。

		- Future语句使用比特位来指定,多个语句可以通过按位或来指定.

	5. optimize 实参指定编译器的优化级别;
		- 默认值 -1 选择与解释器的 -O 选项相同的优化级别。
		- 显式级别为0 (没有优化;__debug__ 为真)
		- 1 (断言被删除,__debug__ 为假)
		- 2 (文档字符串也被删除)
		- 如果编译的源码不合法,此函数会触发SyntaxError异常;如果源码包含null字节,则会触发ValueError 异常。
		
2. eval(expression, globals=None, locals=None)
> 实参是一个字符串,以及可选的 globals 和 locals。expression 参数会作为一个 Python 表达式(从技术上说是一个条件列表)被解析并求值,
> 使用 globals 和locals 字典作为全局和局部命名空间

	1. globals 实参必须是一个字典。locals 可以是任何映射对象。

	2. 如果省略 locals 字典则其默认值为 globals 字典。如果两个字典同时省略,表达式会在eval() 被调用的环境中执行

	3. 返回值为表达式求值的结果。语法错误将作为异常被报告

	4. 提示:exec() 函数支持动态执行语句。globals() 和locals() 函数各自返回当前的全局和本地字典,因此您可以将它们传递给eval() 或exec() 来使用

3. exec(object [ , globals [ , locals ]] )
> 这个函数支持动态执行 Python 代码。object 必须是字符串或者代码对象

	1. 内置globals() 和locals() 函数各自返回当前的全局和本地字典,因此可以将它们传递
		给exec() 的第二个和第三个实参。

	2. 默认情况下,locals 的行为如下面locals() 函数描述的一样:不要试图改变默认的 locals 字典。
		如果您想在exec() 函数返回时知道代码对 locals 的变动,请明确地传递 locals 字典。

	3. **相同大小的数字变量有相同的哈希值即使它们的类型不同，如1和1.0**

### 判断类型
1. isinstance(object, classinfo)
> 如果 object 实参是 classinfo 实参的实例,或者是(直接、间接或虚拟)子类的实例,则返回 true

	1. classinfo 只能是类型或者类型的**元组**

2. issubclass(object, classinfo)
> 如果 class 是 classinfo 的子类(直接、间接或虚拟 的),则返回 true
	1. classinfo 只能是类型或者类型的**元组**

### functional
#### itertools
1. 概述
	1. 本模块标准化了一个快速、高效利用内存的核心工具集,这些工具本身或组合都很有用。它们一起形成了
		“迭代器代数”,这使得在纯 Python 中有可能创建简洁又高效的专用工具。

	```
	SML 有一个制表工具:tabulate(f),它可产生一个序列 f(0), f(1), ...。
	在 Python 中可以组合map() 和count() 实现:map(f, count())。
	```

	2. 这些内置工具同时也能很好地与operator 模块中的高效函数配合使用。

	```
	sum(map(operator.mul, vector1, vector2))
	```
2. 无穷迭代器：
	1. count(): start, [step]; start, start+step, ...

	2. cycle(): p            ; p0, p1,...plast, p0, p1, ...

	3. repeat(); elem[,n]    ; elem, elem, elem, ...重复无限次数或者n次。

3. 根据最短输入序列长度停止的迭代器
	1. accumulate(): p[,func]; p0, p0+p1, p0+p1+p2, ...
	> class accumulate(builtins.object)
	> accumulate(iterable[, func]) --> accumulate object
	> Return series of accumulated sums (or other binary function results).

	2. chain():p, q, ...    ; p0, p1, ...plast, q0, q1, ...

	3. chain.from_iterable(): iterable  ;  p0, p1, ... q0, q1, ...

	4. compress(): data, selectors      ; (d[0] if s[0]), (d[1] if s[1]);

		```
		compress('ABCDEF', [1,0,1,0,1,1]) --> A C E F
		```

	5. dropwhile(): pred, seq           ; seq[n], seq[n+1], ... 从 pred首次真值测试失败开始

		> dropwhile(predicate, iterable) --> dropwhile object
		> Drop items from the iterable while **predicate(item)** is true.
		> Afterwards, return every element until the iterable is exhausted.

		```
		dropwhile(lambda x: x<5, [1,4,6,4,1]) --> 6 4 1
		```

	6. filterfalse(): pred, seq          ; seq 中 pred(x) 为假值的元素,x 是 seq 中的元素。
		
		```
		filterfalse(lambda x: x%2,range(10)) --> 0 2 4 6 8
		```

!	7. groupby(): iterable[, key]         ; 根据 key(v) 值分组的迭代器
		> groupby(iterable, key=None) -> make an iterator that returns consecutive
		> keys and groups from the iterable.  If the key function is not specified or
		> is None, the element itself is used for grouping.

	8. islice(): 

	9. startmap()

	10. takewhile()

	11. tee()

	12. zip_longest():

3. 排列组合迭代器

	1. product()

	2. per

#### map, filter, reduce
1. filter(function, iterable)
> 用 iterable 中函数 function 返回真的那些元素,构建一个新的迭代器。iterable 可以是一个序列,一个支持迭代的容器,或一个迭代器。
	如果 function 是 None ,则会假设它是一个身份函数,即 iterable 中所有返回假的元素会被移除。

2. map(function, iterable, ...)
> 返回一个将 function 应用于 iterable 中每一项并输出其结果的迭代器。

	1. 如果传入了额外的 iterable 参数,function 必须接受相同个数的实参并被应用于从所有可迭代对象中并行获取的项。
	2. 当有多个可迭代对象时,最短的可迭代对象耗尽则整个迭代就将结束。
	3. 对于函数的输入已经是参数元组的情况,请参阅itertools.starmap()。

3. itertools.starmap(fucntion, iterable)
> 创建一个迭代器，使用从可迭代对象中获取的参数来计算该函数。

	1. 当参数对应的形参已从一个单独可迭代对象组合为元组时(数据已被“预组对”)可用此函数代替map()。
	2. map() 与starmap() 之间的区别可以类比 function(a,b) 与 function(*c) 的区别。大致相当于:
		```
		def starmap(function, iterable):
		# starmap(pow, [(2,5), (3,2), (10,3)]) --> 32 9 1000
			for args in iterable:
				yield function(*args)
		```

### 反射
1. delattr(object, name)
> 实参是一个对象和一个字符串。该字符串必须是对象的某个属性。

	1. 如果对象允许,该函数将删除指定的属性。例如 delattr(x, 'foobar') 等价于 del x.foobar 。

2. setattr(object, name, value)
> 此函数与getattr() 两相对应。其参数为一个对象、一个字符串和一个任意值。

	1. 字符串指定一个现有属性或者新增属性。
	2. 函数会将值赋给该属性,只要对象允许这种操作。例如,setattr(x, 'foobar',123) 等价于 x.foobar = 123。

3. getattr(object, name [ , default ] )
> 返回对象命名属性的值。

	1. name 必须是字符串。如果该字符串是对象的属性之一,则返回该属性的值。
		例如,getattr(x, 'foobar') 等同于 x.foobar。如果指定的属性不存在,且提供了 default 值,则返回它,否则触发AttributeError。

4. hasattr(object, name)
> 该实参是一个对象和一个字符串。
	
	1. 如果字符串是对象的属性之一的名称,则返回 True,否则返回False。(此功能是通过调用 getattr(object, name) 看是否有AttributeError 异常来实现的。)

## 内置常量
### 逻辑值判断
1. **任何对象都可以进行逻辑值的检测**
2. **一个对象在默认情况下均被视为真值,除非当该对象被调用时其所属类定义了 __bool__() 方法且返回False 或是定义了 __len__() 方法且返回零。**
3. 特殊用法：<font color=grern>要例外:布尔运算 or 和 and 总是返回其中一个操作数</font>

### 迭代器
1. <font color=green>迭代器协议</font>
迭代器对象需要支持以下两个方法，它们共同组成了<font color=red>迭代器协议</font>
	1. iterator.__iter__()
		- 返回迭代器对象本身。这是同时允许容器和迭代器配合 for 和 in 语句使用所必须的。
		- 此方法对应于Python/C API 中 Python 对象类型结构体的 tp_iter 槽位。

	2. iterator.__next__()
		- 从容器中返回下一项。如果已经没有项可返回,则会引发StopIteration 异常。此方法对应于
		- Python/C API 中 Python 对象类型结构体的 tp_iternext 槽位。

2. <font color=green>容器对象支持迭代，只需要定义一个方法</font>
	1. container.__iter__()
		1. 返回一个迭代器对象

3. 生成器
	- Python 的generator 提供了一种实现迭代器协议的便捷方式。
	- 如果容器对象 __iter__() 方法被实现为一个生成器,它将自动返回一个迭代器对象(从技术上说是一个生成器对象),
		该对象提供 __iter__() 和__next__() 方法。有关生成器的更多信息可以参阅 yield 表达式的文档。

4. 总结：
	1. 支持迭代的容器只需要实现一个__iter__就可以，iter返回的迭代器必须支持迭代器协议，也就是同时支持next和iter
	2. 迭代器的iter返回的是自己，而支持迭代器的容器iter返回的迭代器可以不是自己
	3. 生成器如果iter被实现为一个生成器，python解释器自动添加next方法。

### 上下文管理器类型
> 在语句体被执行前进入该上下文,并在语句执行完毕时退出该上下文

1. contextmanager.__enter__()
	- 进入运行时上下文并返回此对象或关联到该运行时上下文的其他对象。
	- 此方法的返回值会绑定到使用此上下文管理器的 with 语句的 as 子句中的标识符。
	- 一个返回其自身的上下文管理器的例子是file object。文件对象会从 __enter__() 返回其自身,以允
		许open() 被用作 with 语句中的上下文表达式。
	- 一个返回关联对象的上下文管理器的例子是decimal.localcontext() 所返回的对象。此种管理器
		会将活动的 decimal 上下文设为原始 decimal 上下文的一个副本并返回该副本。这允许对 with 语句的
		语句体中的当前 decimal 上下文进行更改,而不会影响 with 语句以外的代码。

2. contextmanager.__exit__(exc_type, exc_val, exc_tb)
	- 退出运行时上下文并返回一个布尔值旗标来表明所发生的任何异常是否应当被屏蔽。

3. **简易的上下文管理器**
	- Python 的generator 和contextlib.contextmanager装饰器提供了实现这些协议的便捷方式。 
	- 如果使用contextlib.contextmanager装饰器来装饰一个生成器函数,它将返回一个实现了必要的
		__enter__() and __exit__() 方法的上下文管理器,而不再是由未经装饰的生成器函数所产生的迭代器。

### 模块
	- 模块唯一的特殊操作是属性访问: m.name,这里 m 为一个模块而 name 访问定义在 m 的符号表中的一个名称。
	- 模块属性可以被赋值。
	- 每个模块都有一个特殊属性__dict__
	- 修改此字典将实际改变模块的符号表,但是无法直接对__dict__ 赋值

### 对象方法
	- 绑定方法具有两个特殊的只读属性:m.__self__ 操作该方法的对象,而 m.__func__ 是实现该方法的函数。
	- 调用 m(arg-1, arg-2, ...,arg-n) 完全等价于调用 m.__func__(m.__self__, arg-1, arg-2, ..., arg-n)。
	- 与函数对象类似,绑定方法对象也支持获取任意属性。但是,由于方法属性实际上保存于下层的函数对象中 (meth.__func__),因此不允许设置绑定方法的方法属性

		```
			>>> class C:
			...
			def method(self):
			...
			pass
			...
			>>> c = C()
			>>> c.method.whoami = 'my name is method' # can't set on the method
			Traceback (most recent call last):
			File "<stdin>", line 1, in <module>
			AttributeError: 'method' object has no attribute 'whoami'
			>>> c.method.__func__.whoami = 'my name is method'
			>>> c.method.whoami
			'my name is method'
		```
## 容器
	1. 通过内建的set、list、tuple、和dict类，以及collections模块来了解其他容器。

### iter
1. **iter(object, [, sentinel])**
> 返回一个iterator 对象。根据是否存在第二个实参,第一个实参的解释是非常不同的。

	1. 如果没有第二个实参
		- object 必须是支持迭代协议(有 __iter__() 方法)的集合对象
		- 或必须支持序列协议(有__getitem__() 方法,且数字参数从 0 开始)。
		- 如果它不支持这些协议,会触发TypeError。
		
	2. 如果有第二个实参 sentinel,那么 object 必须是可调用的对象。
		1. 这种情况下生成的迭代器,每次迭代调用它的__next__() 方法时都会不带实参地调用 object;
		2. 如果返回的结果是 sentinel 则触发StopIteration,否则返回调用结果。
		3. 例子：
			1. 构建块读取器: 从二进制数据库文件中读取固定宽度的块,直至到达文件的末尾:

			```
			from functools import partial
			with open("mydata.db", "rb") as f:
				for block in iter(partial(f.read, 64), b''):
					process_block(block)
			
			```
		4. 解释：iter 返回一个迭代器，这个迭代器定义了一个__next__方法，将object封装到了迭代器里面。
			for每次调用的是 in后面的iterator 的next方法。就是无参数调用object

		5. for iter in obj： 
			1. 先调用obj的iter方法，获得一个迭代器
			2. 每次循环都是调用这个迭代器的的next方法。

### slice
1. class slice(stop) / class slice(start, stop [,step])
> 返回一个表示由 range(start, stop, step) 所指定索引集的slice 对象。

	1. 其中 start 和 step 参数默认为 None。
	2. 切片对象具有仅会返回对应参数值(或其默认值)的**只读数据属性 start, stop 和step**。
	3. 它们没有其他的显式功能;
	4. 不过它们会被 NumPy 以及其他第三方扩展所使用。
	5. 切片对象也会在使用扩展索引语法时被生成。例如: a[start:stop:step] 或 a[start:stop, i]。

### dict
1. class dict(**kwarg)
2. class dict(mapping, **kwarg)
3. class dict(iterable, **kwarg)
	- 创建一个新的字典。dict 对象是一个字典类。参见dict 和映射类型 — dict 了解这个类。
	- 其他容器类型,请参见内置的list、set 和tuple 类,以及collections 模块。

#### dict 构造函数 
1. dict() -> new empty dictionary
2. dict(mapping) -> new dictionary initialized from a mapping object's
    (key, value) pairs
3. dict(iterable) -> new dictionary initialized as if via:

	```
	d = {}
	for k, v in iterable:
		d[k] = v
	```

4. dict(**kwargs) -> new dictionary initialized with the name=value pairs
    in the keyword argument list.  For example:  dict(one=1, two=2)

#### dict视图对象
1. 由dict.keys(), dict.values() 和dict.items() 所返回的对象是 视图对象。
2. <font color=red>该对象提供字典条目的一个动态视图,这意味着当字典改变时,视图也会相应改变.</font>
3. <font color=green>一般对容器循环时，不允许改变容器对象里面的值，但是视图对象可以改变对象本身的值。</font>
### frozenset([iterable])

## 容器类型
1. namedtuple(): 创建命名元组子类的工厂函数
2. deque: 类似列表 (list) 的容器,实现了在两端快速添加 (append) 和弹出 (pop)
3. ChainMap: 类似字典 (dict) 的容器类,将多个映射集合到一个视图里面
4. Counter: 字典的子类,提供了可哈希对象的计数功能
5. OrderedDict: 字典的子类,保存了他们被添加的顺序
6. defaultdict: 字典的子类,提供了一个工厂函数,为字典查询提供一个默认值
7. UserDict: 封装了字典对象,简化了字典子类化
8. UserList: 封装了列表对象,简化了列表子类化
9. UserString: 封装了列表对象,简化了字符串子类化

## 其他
1. optparse  
```
from optparse import OptionParser  

parser = OptionParser()  

parser.add_option("-f", "--file", dest="filename",  
				  help="write report to FILE", metavar="FILE")  
parser.add_option("-q", "--quiet",  
				  action="store_false", dest="verbose", default=True,  
				  help="don't print status messages to stdout")  
														
(options, args) = parser.parse_args()  
print(options.filename)

  parser.add_option("-f", "--file",  
				  action="store", type="string", dest="filename")  
  args = ["-f", "foo.txt"]  
  (options, args) = parser.parse_args(args)  
  print options.filename  
```

2. *args and **kwargs
	1. 当函数的参数不确定时，可以使用*args和**kwargs。args在函数中相当于一个元组，kwargs在函数中是字典。
	```
	def fun_var_args(*args):
		print(args,*args)
	```

	参数里时*args时，传进来的多个参数被组装成一个tuple，
			
	```
	a = [1,2,3]
	fun_var_args(a,'q','2',3)
	>([1, 2, 3], 'q', '2', 3) [1, 2, 3] q 2 3
	```
			
	1. 由上可以得出:
		- 函数定义中的\*表示把调用该函数时传进来的多个参数组装成为tuple,注意是传进来的参数变成tuple，如上例:a只是tuple中的一个元素。
		- 而在调用函数中参数加上\*表示把可迭代对象中(<font color=green>**如果是字典的话，
			会取出dict的key，所有将字典当做列表来操作的都会作用到字典的keys的列表上**</font>)的元素取出，
			相当于调用时输入多个参数，在函数内部会讲他们封装为tuple，

		- 在函数内部对可迭代对象(此处不包括字典)使用*表示把此对象中的所有对象取出，相当于一层解包。

	2. eg
	```
	def cc(**kwargs):
		print(kwargs,'   ',*kwargs)
		print(type(kwargs))
		print(type(**kwargs))
	```

		- 在函数定义中使用\*\* 表示在调用该函数时接受数目不定的键值对（注意调用时是键值对，不是字典，如果是字典，
			还需要使用**将字典中对象取出来，且还需注意字典的kye和函数定义的形参必须一样。这样函数中就不是一个字典了，
			而是多个键值对），并把它们转化为一个字典。
		- 在函数调用中使用**表示把字典转化为键值对。
		- 在函数体中不能用**，用*只能取出字典的key。

	3. 总结：
		- 函数定义就是把多个参数组装为一个tuple，每个参数都是tuple中的一项。其他时候*将可迭代对象解包。
		- 函数定义就是把多个键值对参数组装成一个字典，函数调用中表示把一个字典变为多个键值对。其他时候不可用。

## 易错点
### 引用与拷贝
1. 大多数序列类型，支持collections.abc.Sequence的ABC支持的
	1. s * n 或 n * s ：相当于 s 与自身进行 n 次拼接
	2. <font color=red>请注意序列 s 中的项并不会被拷贝;它们会被多次引用</font>

		```
		>>> lists = [[]] * 3
		>>> lists
		[[], [], []]
		>>> lists[0].append(3)
		>>> lists
		[[3], [3], [3]]
		```
	3. 解决办法：

		```
		>>> lists = [[] for i in range(3)]
		>>> lists[0].append(3)
		>>> lists[1].append(5)
		>>> lists[2].append(7)
		>>> lists
		[[3], [5], [7]]
		```

	4. 进一步的解释可以在 FAQ 条目 faq-multidimensional-list 中查看。

### python 10大易错知识点
1. 在函数中乱用表达式作为默认值
	Python允许给一个函数的某个参数设置默认值以使该参数成为一个可选参数。尽管这是这门语言很棒的一个功能，
	但是这当这个默认值是可变对象（mutable）时，那就有些麻烦了。例如，看下面这个Python函数定义：

		>>> def foo(bar=[]):        
		...    bar.append("baz")    
		...    return bar

	1. 人们常犯的一个错误是认为每次调用这个函数时不给这个可选参数赋值的话，它总是会被赋予这个默认表达式的值。
		例如，在上面的代码中，程序员可能会认为重复调用函数foo() （不传参数bar给这个函数），
		这个函数会总是返回‘baz’，因为我们假定认为每次调用foo()的时候（不传bar），参数bar会被置为[]（即，一个空的列表）。

			>>> foo()
			["baz"]
			>>> foo()
			["baz", "baz"]
			>>> foo()
			["baz", "baz", "baz"]

	2. 答案是一个函数参数的默认值，仅仅在该函数定义的时候，被赋值一次。如此，只有当函数foo()第一次被定义的时候，
		才将参数bar的默认值初始化到它的默认值（即一个空的列表）。当调用foo()的时候（不给参数bar），会继续使用bar最早初始化时的那个列表。

			>>> def foo(bar=None):
				...    if bar is None:      
			# 或者用 if not bar:
			   ...        bar = []
			   ...    bar.append("baz")
			   ...    return bar
			   ...
			   >>> foo()
			["baz"]
			>>> foo()
			["baz"]
			>>> foo()
			["baz"]

	3. 在给函数的参数赋值时，不要用可变对象

2. 类属性

		>>> class A(object):
		...     x = 1
		...
		>>> class B(A):
		...     pass
		...
		>>> class C(A):
		...     pass
		...
		>>> print A.x, B.x, C.x
		1 1 1
		>>> B.x = 2

	1. 在Python里，类变量通常在内部被当做字典来处理并遵循通常所说的方法解析顺序（Method Resolution Order (MRO)）。
	2. 因此在上面的代码中，因为属性x在类C中找不到，因此它会往上去它的基类中查找（在上面的例子中只有A这个类，
	当然Python是支持多重继承（multiple inheritance）的）。换句话说，C没有它自己独立于A的属性x。因此对C.x的引用实际上是对A.x的引用。
	（B.x不是对A.x的引用是因为在第二步里B.x=2将B.x引用到了2这个对象上，倘若没有如此，B.x仍然是引用到A.x上的。——译者注）

	3. 注意类变量和对象变量的区别。

		1. 类变量存在类中，所有类对象拥有对类变量的引用，就像类里的常量一样，如果类改变变量的值，对象也都会改变该值，
			因为对象一开始拥有类变量的引用，但就像类常量一样，如果对象改变类变量，那么对象中的变量就不是引用，而是重新为对象中的那个类变量赋予新值，
			此时，对象中类变量的id和类变量中的id是不同的，但如果对象没有更改过类变量，则对象的类变量id和类本身的变量的id相同。

		2. 类也是对象，所以在对象方法中也可以增添类变量或方法，只不过对象不能使self，直接引用类的名字：

				class A(object):
					def test(self):
						A.address = "earch"

		3. 这样，这个类本身就被改变了，address这个属性存在于类A的__dict__属性中。
