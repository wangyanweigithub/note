﻿## lambda 表达式
1. C++11提供了对匿名函数的支持,称为Lambda函数(也叫Lambda表达式). Lambda表达式具体形式如下:

　　　　[capture](parameters)->return-type{body}

2. 如果没有参数,空的圆括号()可以省略.返回值也可以省略,如果函数体只由一条return语句组成或返回类型为void的话.形如:

　　 　 [capture](parameters){body}

3. 下面举了几个Lambda函数的例子:  　　　　

		[](int x, int y) { return x + y; } // 隐式返回类型
		[](int& x) { ++x; }   // 没有return语句 -> lambda 函数的返回类型是'void'
		[]() { ++global_x; }  // 没有参数,仅访问某个全局变量
		[]{ ++global_x; }     // 与上一个相同,省略了()

	　　//可以像下面这样显示指定返回类型:  
		[](int x, int y) -> int { int z = x + y; return z; }
	　　在这个例子中创建了一个临时变量z来存储中间值. 和普通函数一样,这个中间值不会保存到下次调用. 

4. 什么也不返回的Lambda函数可以省略返回类型, 而不需要使用 -> void 形式.

5. Lambda函数可以引用在它之外声明的变量. 这些变量的集合叫做一个闭包. 闭包被定义在Lambda表达式声明中的方括号[]内. 
	这个机制允许这些变量被按值或按引用捕获.下面这些例子就是:  

		[]        //未定义变量.试图在Lambda内使用任何外部变量都是错误的.
		[x, &y]   //x 按值捕获, y 按引用捕获.
		[&]       //用到的任何外部变量都隐式按引用捕获
		[=]       //用到的任何外部变量都隐式按值捕获 [&, x]    //x显式地按值捕获. 其它变量按引用捕获
		[=, &z]   //z按引用捕获. 其它变量按值捕获

	1. 接下来的两个例子演示了Lambda表达式的用法.  


	　　1. 此例计算list中所有元素的总和. 变量total被存为lambda函数闭包的一部分. 因为它是栈变量(局部变量)total的引用,所以可以改变它的值.  

				std::vector<int> some_list;
				int total = 0;
				for (int i=0;i<5;++i) some_list.push_back(i);
				std::for_each(begin(some_list), end(some_list), [&total](int x)
				{
					total += x;
				});

	　　2. 此例中total会存为引用, value则会存一份值拷贝. 对this的捕获比较特殊, 它只能按值捕获. 

			- this只有当包含它的最靠近它的函数不是静态成员函数时才能被捕获.
			- 对protect和priviate成员来说, 这个lambda函数与创建它的成员函数有相同的访问控制. 
			- 如果this被捕获了,不管是显式还隐式的,那么它的类的作用域对Lambda函数就是可见的. 
			- 访问this的成员不必使用this->语法,可以直接访问.

					std::vector<int> some_list;
					int total = 0;
					int value = 5;
					std::for_each(begin(some_list), end(some_list), [&, value, this](int x)
					{
						total += x * value * this->some_func();
					});

6. **注意**：<font color=green> 不同编译器的具体实现可以有所不同,但期望的结果是:</font>
	- 按引用捕获的任何变量,lambda函数实际存储的应该是这些变量在创建这个lambda函数的函数的栈指针,而不是lambda函数本身栈变量的引用.
	- 不管怎样, 因为大数lambda函数都很小且在局部作用中, 与候选的内联函数很类似, 所以按引用捕获的那些变量不需要额外的存储空间.
	- 如果一个闭包含有局部变量的引用,在超出创建它的作用域之外的地方被使用的话,这种行为是未定义的!
　　- lambda函数是一个依赖于实现的函数对象类型,这个类型的名字只有编译器知道. 
	- 如果用户想把lambda函数做为一个参数来传递, 那么形参的类型必须是模板类型或者必须能创建一个std::function类似的对象去捕获lambda函数.
	- 使用 auto关键字可以帮助存储lambda函数,  

			auto my_lambda_func = [&](int x) { /*...*/ };
			auto my_onheap_lambda_func = new auto([=](int x) { /*...*/ });

　　- 这里有一个例子, 把匿名函数存储在变量,数组或vector中,并把它们当做命名参数来传递 


			#include <functional>
			#include <vector>
			#include <iostream>

			double eval(std::function<double(double)> f, double x = 2.0){return f(x);}

			int main()
			{
				std::function<double(double)> f0 = [](double x){return 1;};
				auto f1 = [](double x){return x;};
				decltype(f0) fa[3] = {f0, f1, [](double x){return x*x;}};
				std::vector<decltype(f0)> fv = {f0, f1};
				fv.push_back([](double x){return x*x;});
				for(int i=0; i<fv.size(); i++)
					std::cout << fv[i](2.0) << std::endl;

				for(int i=0; i<3; i++)
					std::cout << fa[i](2.0) << std::endl;

				for(auto &f : fv)
					std::cout << f(2.0) << "\n";

				for(auto &f : fa)
					std::cout << f(2.0) << "\n";

				std::cout << eval(f0) << std::endl;
				std::cout << eval(f1) << std::endl;
				return 0;
			}

7. 一个没有指定任何捕获的lambda函数,可以显式转换成一个具有相同声明形式函数指针.所以,像下面这样做是合法的:
 
		auto a_lambda_func = [](int x) { /*...*/ };
		void(*func_ptr)(int) = a_lambda_func;
		func_ptr(4); //calls the lambda.

8. 泛型 Lambda (C++14)
	1. 独占指针不能拷贝	
	
			#include <iostream>
			#include <utility>
			void learn_lambda_func_3(){
				auto important = std::make_unique<int>(1);
				auto add = [v1 = 1, v2 = std::move(important)](int x, int y) -> int {
					return x+y+v1+(*v2);
				};
				std::cout << "add(3, 4) = " << add(3, 4) << std::endl;
			}

		 1. 在上面的代码中，important 是一个独占指针，是不能够被捕获到的，这时候我们需要将其转移为右值，在表达式中初始化。


	2. auto 关键字不能够用在参数表里，这是因为这样的写法会与模板的功能产生冲突。
		1. 但是 Lambda 表达式并不是普通函数，所以 Lambda表达式并不能够模板化。这就为我们造成了一定程度上的麻烦：
			**参数表不能够泛化，必须明确参数表类型**。

	3. 幸运的是，这种麻烦只存在于 C++11 中，从 C++14 开始，Lambda 函数的形式参数可以使用 auto 关键字来产生意义上的泛型：

			void learn_lambda_func_4(){
				auto generic = [](auto x, auto y) {
					return x+y;
				};

				std::cout << "generic(1,2) = " << generic(1, 2) << std::endl;
				std::cout << "generic(1.1,2.2) = " << generic(1.1, 2.2) << std::endl;
			}
