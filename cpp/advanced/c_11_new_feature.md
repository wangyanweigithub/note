<!-- vim-markdown-toc GFM -->

* [Q 15](#q-15)
* [normal 20](#normal-20)
* [note:c++11/14 新特性 23](#notec1114-新特性-23)
	* [1. nullptr: 24](#1-nullptr-24)
	* [2. constexpr: 28](#2-constexpr-28)
	* [3. 类型推导 36](#3-类型推导-36)
	* [4. 区间迭代 88](#4-区间迭代-88)
	* [5. 初始化列表 98](#5-初始化列表-98)
	* [6. 模板增强 167](#6-模板增强-167)

<!-- vim-markdown-toc -->

## Q 15
1. foo(int) 和foo(char) 能不能重载，int和char可以隐式转换？
2. 模板的声明和定义？
3. 模板实例化的时间？

## normal 20
1. 类的前置声明：前置声明只是声明有这个类，但是没有给出具体方法，所以在类定义前不能使用类的方法。只能使用看到的。

## note:c++11/14 新特性 23
### 1. nullptr: 24
1. NULL 根据编译器定义，可能是0,也可能式 (void*)0.
2. nullptr 就是为了区分NULL定义的(void *)0 和空指针的。

### 2. constexpr: 28
1. 在c++98之前，常量表达式中的变量必须声明为const。
2. 有些场景只能使用常量表达式：
	1. 数组的长度：a[10](c99 可以使用变长数组)

3. constexpr 显示声明函数或对象构造函数在编译阶段会成为常数。
4. constexpr 函数在c++11中不可以递归，而c++14可以使用递归。

### 3. 类型推导 36
1. auto
	1. auto必须用在可以推导的关键字前面，如左值、
	2. auto 不能用于一下几种情况：
		1. auto 不能用于函数传参。
		2. auto 不能用于推导数组类型
			
				auto i = 5;
				auto arr[10] = {0};  //错误，可能是这个初始化式不能看出式什么类型
				int arr1[10] = {0};
				auto auto_arr = arr1;       //正确，可以准确推导出是int []类型。

2. decltype：它是为了解决auto关键字只能对变量进行类型推导的缺陷而出现的，它和sizeof很相似:
	> decltype(表达式)

	1. 例子

			auto x = 1;
			auto y = 2;
			decltype(x+y) z;

	2. 区别
		1. <font color=green>auto是根据变量被赋予的值来确定变量的类型</font>
		2. <font color=green>decltype是计算表达式的值的类型来确定变量的类型</font>
		3. auto必须用在赋值语句，decltype可以用在声明和定义处。

3. 尾返回类型、auto与decltype配合
	1. 模板 

			template<typename R, typename T, typename U>
			R add(T x, u y){
				return x + y;
			}

	2. 尾返回类型和auto结合

			decltype(x+y) add(T x, U y);

			但事实上这样写C++11不能编译通过，因为读到decltype(x+y)时，x和y没有定义。为了解决这个问题，c++11 引入 尾返回类型，利用auto关键字将返回类型后置。

			template<typename T, typename U>
			auto add(T x, U y) -> decltype(x+y) {
				return x+y;
			}

	3. C++14 开始可以直接让普通函数具备返回值推导

			template<typename T, typename U>
			auto add(T x, U y){
				return x+y;
			}

### 4. 区间迭代 88
1. 相当于将可迭代对象中的每个子对象赋予中间的那个变量。&引用可读写，变量本身只可读。

		int arr[] = {1, 2, 3, 5, 6};
		for (auto x: arr)
			cout << x ;

		for (auto &i:arr)
			cout << i << endl;

### 5. 初始化列表 98
1. 传统c++，不同的对象有着不同的初始化方法
	1. 普通数组、POD(plain old data,没有构造函数、析构函数的虚函数的类或结构体)类型
		都可时使用**{}**进行初始化,也就是我们所说的**初始化列表**。
	2. 类对象的初始化，
		1. 类对象拷贝构造函数
		2. 使用**()**调用符号调用构造函数。
	3. 两者不可混用。

2. 为了解决这个问题C++11 <font color=green>将初始化列表的概念绑定到了类型上,
	并将之成为 std::initializer_list</font>允许构造函数或其他函数像参数一样使用初始化列表。

3. initializer_list是C++11提供的新类型，定义在头文件中。
	1. 用于表示某种特定类型的值的数组，和vector一样，initializer_list也是一种模板类型。

			template< class T >
			class initializer_list;

	2. C++11扩大了初始化列表的适用范围，使其可用于所有内置类型和用户定义的类型。
		 无论是初始化对象还是某些时候为对象赋新值，都可以使用这样一组由花括号括起来的初始值了。
		 使用初始化列表时，可添加=，也可不添加。

			int units_sold=0;
			int units_sold(0);
			int units_sold={0};  //列表初始化
			int units_sold{0};    //列表初始化

	3. 当初始化列表用于内置类型的变量时，这种初始化形式有一个重要特点：
		1. 如果我们使用列表初始化值存在丢失信息的风险，则编译器将报错：
		
				long double ld=3.1415926536;
				int a={ld},b={ld}; //错误:转换未执行，因为存在丢失信息的风险
				int c(ld),d=ld;   //正确:转换执行，且确实丢失了部分值

	4. 注意：
		1. <font color=green>initializer_list对象中的元素永远是常量值，我们无法改变initializer_list对象中元素的值。</font>
		2. <font color=green>拷贝或赋值一个initializer_list对象不会拷贝列表中的元素，
			其实只是引用而已，原始列表和副本共享元素。</font>

				initializer_list<T> lst; //默认初始化；T类型元素的空列表 
				initializer_list<T> lst{a,b,c...}; 
				//lst的元素数量和初始值一样多；lst的元素是对应初始值的副本 
				lst2(lst) 
				lst2=lst 
				//拷贝或赋值一个initializer_list对象不会拷贝列表中的元素；拷贝后，原始列表和副本元素共享 
				lst.size() //列表中的元素数量 
				lst.begin() //返回指向lst中首元素的指针 
				lst.end() //返回指向lst中尾元素下一位置的指针

	5. 如果想向initializer_list形参中传递一个值的序列，则必须把序列放在一对花括号内

	6. python for 循环

			#include <iostream>

			int main(int argc, char **argv)
			{
				for(auto n : {1,2,3,4,5,6})
				{
					std::cout<<n<<std::endl;
				}

			}
	7. C++11 统一语法来初始化任意对象：
		
			construct(int x, int y): x(a), y(b) {}

			这样可以直接用{}来初始化类。

### 6. 模板增强 167
> 模板是用来产生类型的。

1. 外部模板
	1. 传统 C++ 中，模板只有在使用时才会被编译器实例化。
	2. 所以只要在每个编译单元（文件）中编译的代码中遇到了被完整定义的模板，都会实例化。
	3. C++11 引入了外部模板，扩充了原来的强制编译器在特定位置实例化模板的语法，使得能够显式的告诉编译器何时进行模板的实例化：

			template class std::vector<bool>;            // 强行实例化
			extern template class std::vector<double>;  // 不在该编译文件中实例化模板 

2. 类型别名
	1. 在传统 C++中，typedef 可以为类型定义一个新的名称，但是却没有办法为模板定义一个新的名称。因为，模板不是类型。例如：
	
			template< typename T, typename U, int value>
			class SuckType {
			public:
				T a;
				U b;
				SuckType():a(value),b(value){}
			};
			template< typename U>
			typedef SuckType<std::vector<int>, U, 1> NewType; // 不合法

	2. 
C++11 使用 using 引入了下面这种形式的写法，并且同时支持对传统 typedef 相同的功效：

		1. 通常我们使用 typedef 定义别名的语法是：typedef 原名称 新名称;
		2. 但是对函数指针等别名的定义语法却不相同，这通常给直接阅读造成了一定程度的困难。

				typedef int (*process)(void *);  // 定义了一个返回类型为 int，参数为 void* 的函数指针类型，名字叫做 process
				using process = int(*)(void *); // 同上, 更加直观

				template <typename T>
				using NewType = SuckType<int, T, 1>;    // 合法

3. 默认模板参数
	
		template<typename T = int, typename U = int>
		auto add(T x, U y) -> decltype(x+y) {
			return x+y;

		}

4. 变长参数模板
	1. C++11 加入了新的表示方法，允许任意个数、任意类别的模板参数，同时也不需要在定义时将参数的个数固定。

			template<typename... Ts> class Magic;

		1. 模板类 Magic 的对象，能够接受不受限制个数的 typename 作为模板的形式参数，例如下面的定义：

				class Magic<int, std::vector<int>, std::map<std::string, std::vector<int>>> darkMagic;

		2. 既然是任意形式，所以个数为0的模板参数也是可以的：class Magic<> nothing;。

		3. 如果不希望产生的模板参数个数为0，可以手动的定义至少一个模板参数：

				template<typename Require, typename... Args> class Magic;

5. 变长参数模板也能被直接调整到到模板函数上。
	1. 传统 C 中的 printf 函数，虽然也能达成不定个数的形参的调用，但其并非类别安全。
	2. 而 C++11 除了能定义类别安全的变长参数函数外，还可以使类似 printf 的函数能自然地处理非自带类别的对象。
	3. 除了在模板参数中能使用 ... 表示不定长模板参数外，函数参数也使用同样的表示法代表不定长参数，这也就为我们简单编写变长参数函数提供了便捷的手段，例如：

			template<typename... Args> void printf(const std::string &str, Args... args);

	4. 那么我们定义了变长的模板参数，如何对参数进行解包呢？

		1. 首先，我们可以使用 sizeof... 来计算参数的个数，：

				template<typename... Args>
				void magic(Args... args) {
					std::cout << sizeof(args) << std::endl;
				}

		2. 我们可以传递任意个参数给 magic 函数：

				magic();        // 输出0
				magic(1);       // 输出1
				magic(1, "");   // 输出2

		3. 其次，对参数进行解包，到目前为止还没有一种简单的方法能够处理参数包，但有两种经典的处理手法：

			1. 递归模板函数

				- 递归是非常容易想到的一种手段，也是最经典的处理方法。
				- 这种方法不断递归的向函数传递模板参数，进而达到递归遍历所有模板参数的目的：

							#include <iostream>
							template<typename T>
							void printf(T value) {
								std::cout << value << std::endl;
							}
							template<typename T, typename... Args>
							void printf(T value, Args... args) {
								std::cout << value << std::endl;
								printf(args...);
							}
							int main() {
								printf(1, 2, "123", 1.1);
								return 0;
							}

			2. 初始化列表展开
				> 递归模板函数是一种标准的做法，但缺点显而易见的在于必须定义一个终止递归的函数。

				1. 这里介绍一种使用初始化列表展开的黑魔法：

						// 编译这个代码需要开启 -std=c++14
						// 因为版本原因，实验环境中的 g++ 尚不支持此特性，此处可以使用 clang++ 替代 g++
						template<typename T, typename... Args>
						auto print(T value, Args... args) {
							std::cout << value << std::endl;
							return std::initializer_list<T>{([&] {
								std::cout << args << std::endl;
							}(), value)...};
						}
						int main() {
							print(1, 2.1, "123");
							return 0;
						}

				2. 通过初始化列表，(lambda 表达式, value)... 将会被展开。由于逗号表达式的出现，首先会执行前面的 lambda 表达式，完成参数的输出。唯一不美观的地方在于如果不使用 return 编译器会给出未使用的变量作为警告。

				3. 事实上，有时候我们虽然使用了变参模板，却不一定需要对参数做逐个遍历，我们可以利用 std::bind 及完美转发等特性实现对函数和参数的绑定，从而达到成功调用的目的。

				4. 关于这方面的使用技巧，可以通过项目课：100 行 C++ 代码实现线程池 进行进一步巩固学习。
