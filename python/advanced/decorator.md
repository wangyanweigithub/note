
<!-- vim-markdown-toc GFM -->

	* [@property]
	* [staticmethod]
* [问答]

<!-- vim-markdown-toc -->
### @property
1. eg
```
class Student(object):

	@property
	def score(self):
		return self._score

	@score.setter
	def score(self, value):
		if not isinstance(value, int):
			raise ValueError('score must be an integer!')
		if value < 0 or value > 100:
			raise ValueError('score must between 0 ~ 100!')
		self._score = value
```

2. Python内置的@property装饰器就是负责把一个方法变成属性调用的

3. 把一个getter方法变成属性，只需要加上@property就可以了，
此时，@property本身又创建了另一个装饰器@score.setter，
        
4. 负责把一个setter方法变成属性赋值，于是，我们就拥有一个可控的属性操作
```
s = Student()
s.score = 60
s.score
>>> 60
```

### staticmethod
1. staticmethod像函数一样调用
	1. **装饰器可以像常规函数一样调用**
	2. **staticmethod 像装饰器一样可以像常规函数一样调用，并对结果进行操作**
		1. 例如：某些情况下需要从类主体引用函数并且**希望避免自动转换为实例方法**

```
class C:
	builtin_open = staticmethod(open)
	```

## 问答
1.  问题：
    我已经在CPython中读到过，解释器堆栈(Python函数的列表被称为达到这一点)与C堆栈
(在解释器自己的代码中调用的C函数列表)混合在一起。 如果是这样，那么如何实施发电机和协同程序？
他们如何记住他们的执行状态？ 
CPython是否将每个生成器/协程的堆栈复制到OS堆栈？ 
还是CPython只是将发生器的最上面的堆栈帧保存在堆上，因为发生器只能从最上面的帧中产生？ 

2. answer：
	yield指令将当前正在执行的上下文作为闭包，并将其转换为自己的活动对象。 
	这个对象有一个__iter__方法，它将在这个yield语句之后继续。所以调用栈被转换成一个堆对象。 
    The yield instruction takes the current executing context as a closure, and transforms it into an own living object. 
	This object has a __iter__ method which will continue after this yield statement. So the call stack gets transformed into a heap object.

