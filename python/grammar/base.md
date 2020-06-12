
<!-- vim-markdown-toc GFM -->

* [type]
* [for else]
* [迭代器：]
* [成员属性：]

<!-- vim-markdown-toc -->
### type
                
    1. 通过type()函数创建的类和直接写class是完全一样的，因为Python解释器遇到class定义时，仅仅是扫描一下class定义的语法，然后调用type()函数创建出class。

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
### for else
1. When the items are exhausted (which is immediately 
when the sequence is empty), the suite in the else clause, 
if present, is executed, and the loop terminates.

2. A break statement executed in the first suite terminates the 
loop without executing the else clause’s suite. A continue 
statement executed in the first suite skips the rest of the suite and 
continues with the next item, or with the else clause if there was no next item.

3. https://docs.python.org/2/reference/compound_stmts.html#the-for-statement

4. 大意是说当迭代的对象迭代完并为空时，位于else的子句将执行，而如果在for循环中含有
break时则直接终止循环，并不会执行else子句。

### 迭代器：
	1. 迭代器是一次性的，它会保持一个状态记录执行到什么地方，如果一个迭代器执行完，迭代器就会变成空。
	
			>>> numbers = [1, 2, 3, 5, 7]
			>>> squares = (n**2 for n in numbers)

			>>> 9 in squares
			True

			>>> 9 in squares
			False

			>>> 4 in squares
			False

		迭代器消耗了就没有了，所以第二次迭代器已经执行到了3**2 才可以判断出来。前面的两个数字也消耗了

	2. <font color=green>询问迭代器中是否包含某些东西将会部分地消耗迭代器。如果没有循环遍历迭代器，那么是没有办法知道某个东西是否在迭代器中。</font>

	3. 参考链接：
		1. [迭代器](https://linux.cn/article-9681-1.html)

### 成员属性：
1. private:
	1. 如果要让内部属性不被外部访问，可以把属性的名称前加上两个下划线`__`，在Python中，实例的变量名如果以`__`开头，
	就变成了一个私有变量（private），只有内部可以访问，外部不能访问

	2. 双下划线开头的实例变量是不是一定不能从外部访问呢？其实也不是。
	不能直接访问`__name`是因为Python解释器对外把`__name`变量改成了`_类名_属性名`：`_Student__name`，
	所以，仍然可以通过`_Student__name`来访问`__name`变量：

2. 特殊变量 
	需要注意的是，在Python中，变量名类似__xxx__的，也就是以双下划线开头，并且以双下划线结尾的，是特殊变量，特殊变量是可以直接访问的，
	不是private变量，所以，不能用__name__、__score__这样的变量名。

3. 单下划线 
	有些时候，你会看到以一个下划线开头的实例变量名，比如_name，这样的实例变量外部是可以访问的.
	但是，按照约定俗成的规定，当你看到这样的变量时，意思就是，“虽然我可以被访问，但是，请把我视为私有变量，不要随意访问”。

