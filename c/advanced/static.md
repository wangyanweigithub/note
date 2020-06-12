<!-- vim-markdown-toc GFM -->

* [生存周期与作用域的区别：](#生存周期与作用域的区别)
* [中兴通讯2012校招笔试题的一道问答题：](#中兴通讯2012校招笔试题的一道问答题)

<!-- vim-markdown-toc -->
## 生存周期与作用域的区别： 
1. 生存周期: 变量从定义到销毁的时间范围。存放在全局数据区的变量的生存周期存在于整个程序运行期间，而存放在栈中的数据则随着函数等的作用域结束导致出栈而销毁，除了静态变量之外的局部变量都存放于栈中。
 
2. 作用域: 变量的可见代码域（块作用域，函数作用域，类作用域，程序全局作用域）。

3. <font color=green>static变量是指静态的变量，不管是在全局还是局部声明的static变量都存放于程序的全局变量区域，所以它的生命周期是从程序开始到程序结束。但是static变量的作用域并不等同于它的生存周期，它的作用域决定于它被定义的位置。可以认为static变量的作用域<=生存周期。</font>

	1. 举一个局部声明的例子。在函数test中声明静态变量i：
 
			void test() 
			{ 
			int m=3; 
			static int i=5; 
			} 

		1. 局部变量m存放在栈中，当test函数结束，m将被销毁；
		2. 静态变量i不存放在栈中，而是存放于程序的全局变量区域，因此随着函数test的结束，它并不随着出栈操作而被销毁，它的生存周期存在于程序的整个运行期；
		3. 然而m和i的作用域都仅存在于test函数中它们的定义之后，即test调用结束之后，m和i就不再可用，但是i仍存在于内存之中。

	2. 再举一个全局声明的例子。在文件A 中定义静态变量j：
	 
			int n=3; //默认为extern 
			static int j=5; //声明为static 

		1. 全局变量和静态变量j都存放于程序的全局数据区域，它们的生存周期都是程序的整个运行期，但是n的作用域为全局作用域，可以通过extern在其他文件中使用，而j只能在文件A中使用，
		2. 例如在文件B中：
		 
				extern int n; //ok 
				extern int j; //error: j在文件B中不可见 
				int a=n;//ok：但这里有个初始化先后的问题，具体参见参考一 
				int b=j;//error 

		3. 也就是说，在声明全局的static变量时，static没有改变它的生存周期，也即存储位置（因为全局变量本来就存储在全局数据域），而是将变量的作用域限制在当前文件中。

4. <font color=red>注意</font>
	1. include "source" 是将包含的文件source(不管是头文件还是源文件)整个添加到文件内，所以source中定义的内容都相当于本文件定义的内容，source中定义的变量无论是static还是extern，本文件都可以访问，且不需要加extern。
	2. 如果include "source"了，g++后面就可以省略掉被包含文件的源文件了，否则如果某个函数或者变量它和它的头文件都没有被include到main文件里，而又用到它了，需要在编译时将源文件添加上，是的编译器可以找到函数定义。这是在编译时一同加上的源文件中定义的static才是真正的隔离了作用域作用。

	3. 例子
		1. extern_static.h

		```
		#ifndef _EXTERN_STATIC
		#define _EXTERN_STATIC
		#include <stdlib.h>
		#include <stdio.h>

		void print_01();
		extern void print_02(); //函数不写任何声明的情况和写了extern声明的情况等价，函数的声明默认是extern的。
		static void print_03(); //static修饰函数声明，说明此函数仅仅被此文件内部使用，
								//即被extern_stattic.h 和extern_static.cpp内部调用，而不能被main.cpp内调用。
		void print_04();
		#endif
		```

		2. extern_static.cpp
		```
		#include "extern_static.h"

		static int a = 100;
		int b = 200;

		void print_01(){
			printf("test_01\n");
			return;
		}

		void print_02(){

			printf("test_02\n");
			return;
		}

		void print_03(){
			printf("test_03\n");
			return;

		}

		void print_04(){
			printf("call print_03() in print_04() start\n");
			print_03();
			printf("call print_03() in print_04() finsh\n");
			return;

		}
		```

		3. main.cpp
		```
		#include "extern_static.h"

		int main()
		{
			print_01();
			print_02();
			//print_03();
			print_04();
			extern int b;
			printf("b is :%d", b);
			int c = getchar();

		}
		```
		4. <font color=red>编译</font> 
			1. main.cpp include了头文件，所以main.cpp源文件其实被插入了extern_static.h 代码，所有这个头文件中的代码都和main.cpp在头一个文件中，所以它们不管是static还是extern都可以直接用。
			2. 因为没有包含extern_static.cpp源文件，所以编译时要告诉编译器函数定义在哪里：g++ main.cpp extern_static.cpp -o test

			3. 定义在extern_static.cpp中的static变量main不可以访问，因为不在同一个文件中，static限定作用域只有本文件可以访问。extern全部文件可以访问

			4. 如果不写头文件，main直接包含extern_static.cpp也是可以的，这时编译器将整个extern_static.cpp插入main.cpp源文件，这是编译时不用再指定extern_static.cpp源文件:g++ main.cpp -o test。否则会报错:g++ main.cpp extern_static.cpp，因为编译器读到两边extern_static.cpp 内容。
