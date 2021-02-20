### 编译器
1. 编译器的一部分工作是寻找程序文本中的错误。
	> 编译器没有能力检查一个程序是否按照其作者的意图工作，但可以检查<font color=red>形式
	（form）</font>上的错误

	1. 语法错误（syntax error）：程序员犯了C++语言文法上的错误
	2. 类型错误（type error）：C++每个数据项都有其类型。
	3. 声明错误（declaration error）：C++中每个名字都要先声明后使用。

2. c++ 编译命令：
	1. gun编译器：g++ -o prog1 prog1.cc  ：-o是编译器参数，执行了可执行文件的文件名。

	2. windows Visual Studio2010编译器： cl /EHsc prog1.cpp ：cl调用编译器，/EHsc 是编译器选项，
	用来打开标准异常处理，生成的程序和第一个源文件名相同。

	3. 编译器通常都包含一些选项，能对有问题的程序结构发出警告，打开这些选项通常是一个好习惯，
	gun：-Wall，windows：/W4

3. invalid initialization of non-const reference of type "xxx&" from an rvalue of type "xxx"
> 非常量引用的初始值必须为左值

	1. 引用/别名 & 的三条规则
		1. 引用被创建的同时必须被初始化(指针可以在任何时候被初始化)
		2. 不能有NULL引用，引用必须与合法的存储单元关联
		3. 一旦引用被初始化，就不能改变引用的关系(指针则可以随时改变所指的值)

	2. 注意：
		1. 引用的值是右值时，必须是常量引用，即const修饰
		2. **别名意味着一定有一个原始的变量名使得引用没有悬空，而右值没有变量名指向，
			所以编译器生成了一个临时变量，临时变量只能通过const引用，所以是不可更改的。**
		3. 变量的操作变化可能是变量名来操作的，没有变量指向导致右值无法更改，所以非常量引用的
		初始值必须为左值

		4. 右值是否是常量值？肯定不是
		5. 右值引用是不是常量，是左值。

4. 不可以拷贝的对象的拷贝构造函数和拷贝赋值运算符是delete状态的。所以如果对不可以拷贝的
对象调用拷贝构造函数，会报调用了delete函数。

	1. 例子：100 lines c++ ThreadPool 中对package_task的shared_ptr:
		- 这里使用shared_ptr是因为task是不可以拷贝的。
		- 但是emplace是会拷贝一次的（emplace拷贝一次，push_back拷贝了两次，move一次也不拷贝）。
		- 它不可以拷贝所以拷贝构造函数是delete的，如果这里不使用shared_ptr，编译报错使用了delete的函数。

5.为什么c++中的静态非常量变量只能在类外声明
	类中的静态成员必须在类内声明，类外初始化，c++的静态成员属于整个类，而不属于某个对象，
如果在类内初始化，会导致每个对象都包含该静态成员，这是矛盾的。

6. ISO C ++禁止非const静态成员的类初始化(error: ISO C++ forbids in-class initialization of 
non-const static member)

	- 原因是将这样的初始化放在头文件中将在包含头的每个位置复制该初始化代码。
	- 解决方法，在cpp文件中使用::来初始化。

7. stackoverflow问：为什么我不能在类内初始化static成员？同2 
[引用](https://blog.csdn.net/king_djf/article/details/53582580)

	1. 详细

	```
	引用：C++03 9.4.2 Static data members

§4 If a static data member is of const integral or const enumeration type, its declaration in the class definition can specify a constant-initializer which shall be an integral constant expression (5.19). In that case, the member can appear in integral constant expressions. The member shall still be defined in a namespace scope if it is used in the program and the namespace scope definition shall not contain an initializer.（如果一个静态数据成员是constant integral或者const numeration类型，那么它类内的声明中如果包含定义部分，定义部分应该是一个integral constant表达式。在这个情况下，成员可以出现在一个integral constant表达式中。如果这个成员在程序中被使用，应该明确其命名空间的域范围，且不能包含初始化程序）。
	```

	2. 为什么标准不允许这一行为？
		1. Bjarne这样解释：

			一个类会被特别定义在一个头文件中，并且头文件会被包含在许多转换单元里。
		但是，为了防止链接规则过于复杂，C++需要每一个对象有特定的定义。
		如果C++允许存储在内存中的对象进行类内定义，那么这一规则将会被打破。


	3. Why are only static const integral types & enums allowed In-class Initialization?
		为什么只有static const integral类型以及enums被允许类内初始化呢？

	4. 这个答案就在Bjarne的那段话中。“C++需要每一个对象有特定的定义。如果C++允许存储在内存中的对象进行类内定义，那么这一规则将会被打破。”

	5. 注意只有static const integers 会被看作编译时的常量。编译器了解这样的integer变在任何情况下
	都不会改变，因此编译器才可以对其做出特有的优化与改进，编译器会简单的将其内联化这样的变量，
	因而不再使其保存在内存中。因为保存在内存中的需求被移除了，使得他们成了Bjane所说规则的例外。

	6. 值得注意的是，即使static const integral这样的变量被允许使用类内初始化，但是获取这样的变
	量的地址是不被允许的。一个变量只有拥有类外定义的情况下，才能被获得地址。
