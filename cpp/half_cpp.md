## 问题 23
1. 有了auto为什么还需要decltype
    1. auto只能让编译器确定变量的类型，而decltype可以确定一种类型来定义变量。

2. 字面值 "hello " 是什么类型，string还是 const char *。
3. mutable 使用场景。

4. map 下标运算返回的类型是mapped_type，那直接对下标的++，是怎么运算的，mapped_type可以直接进行算术运算吗？
5. 迭代器返回的是指针还是引用，不是拷贝。
6. return hash<string> () (sd.isbn())  什么意思
7. static member copy construct 
8. 聚合类
9. 申请对象一定要用new吗
10. 为什么重载运算符时，要用复合赋值运算符来定义算符运算符，为什不不能反过来。

## 基本类型 38
1. 类型转换
    1. <font color=red>当我们赋给无符号数类型一个超出它表示范围的值时，如果是初始值对无符号数类型表示的数值总数取模后的余数。</font>
    2. 可能会出现无意对无符号数赋一个负值：
        1. 当一个算术表达式既有无符号数又有int值时，那个int值就会转换成为无符号数。
                
                unsigned u = 10;
                int i = -42
                std::out << i + u << std::endl;  //如果int占32位，输出4294967264

        2. 当从无符号数中减去一个值时，不管这个值是不是无符号数，我们都必须确保结果不能是一个负值：

                unsigned u1 = 42， u2 = 10；
                std::cout << u1 - u2 << std::endl;  // 正确：输出32
                std::cout << u2 - u1 << std::endl;  // 正确：不过输出是结果取模

        3. 编译器不管结果的类型，只遵循类型转换，计算时，强制转换类型，然后把值当做无符号数，可能出错

        4. 无符号数不会小于0，这一事实同样关系到循环的写法。

                for (unsigned u = 10; i >= 0; i--)
                    ...
                // 错误，unsigned永远不会小于0

        5. 总结切勿混用带符号数和无符号数
            > 如果表达式里既有带符号数和无符号类型，带符号数会自动转化为无符号数，结果也是无符号数类型，如果出现负数，则出错。

2. 变量
    1. 初始化不是赋值，初始化的含义是创建变量时赋予其一个初始值，而赋值的含义是把对象的当前值擦除，而以一个新值来替代。
    2. 初始化
        - int units_sold = 0;
        - int units_sold = {0}; 
        - int units_sold(0);
        - int units_sold{0};

    3. 列表初始化：
        1. 当用于内置类型的变量时，这种初始化形式有一个重要特点：如果我们使用列表初始化且初始值存在丢失信息的风险，则编译器报错。
            
                long double = 3.1415926;
                int a{ld}, b = {ld};  //错误：转换未执行，因为存在丢失信息的危险。
                int(ld), d = ld;

            1. <font color=red>列表初始化可能是一个精确初始化，不可以以丢失精确度的代价来强制转换。</font>

    4. 声明

	5. const 限定符
		1. const 对象尽在文件内有效,除非声明和定义都加上extern
		2. 引用类型必须与其所对引用的对象类型一致,但有两个例外:
			1. const int &i = (double) 2.3
				1. 在初始化常量引用时允许任意表达式作为初始值,只要该表达式的结果能转换为引用类型即可
				2. 
					1. 例子
					
							double dval = 3.14;
							cons int &ri = dval;

							const int temp = dval;
							const int &ri = temp;

3. 复合类型
    1. 引用
        1. 定义引用时，程序会把引用和它的初始值绑定在一起，而不是将初始值拷贝给引用。一旦初始化完成，引用将和它的初始值绑定在一起。
        2. 因为无法令引用重新绑定到另外一个对象，因此引用必须在定义的时候初始化。
        2. 引用只是一个别名，而不是对象。它的值应该是会被编译器编译为对象的地址，指针是一个单独的空间存储指向对象的地址，而引用本身应该就会解释成对象的地址，没有存储空间。

    2. 指针
        1. void *是特殊的指针类型，可用于存放任意对象的地址。
        2. void *可用的方法有限，因为不确定它的类型，所以类型的方法都不能用。
            1. 拿它和别的指针比较
            2. 作为函数的输入或输出
            3. 赋值给另外一个void * 指针
            4. 不能直接操作void *指针所指的对象，因为我们不知道这个对象是什么类型，可以做那些操作。

    3. 总结
        1. 引用本身不是一个对象，因此不能定义指向引用的指针，但指针是对象，所以存在对指针的引用。
            
                int i = 42;
                int *p;
                int *&r = p;  //r是一个对指针p的引用。

                r = &i;

                // *、&都是类型修饰符，不是类型，只作用于后面的变量名上。

        2. 从右向左阅读r的定义，离变量名最近的符号对变量的类型有最直接的影响。

4. const
    1. const 对象一旦创建后不能改变，所以const对象必须初始化。
    2. 默认情况下，const对象仅在文件中有效
        1. 如果要共享变量，办法是：对于const变量不管是声明还是定义都添加extern关键字。

    3. const和指针
        1. 顶层const表示指针本身是一个常量
        2. 地城const表示指针所指对象是一个常量。

5. constexpr 和常量表达式
    1. 常量表达式
        1. 常量表达式：值不会改变并且在变异过程就能得到结果的表达式。
        2. 一个对象是不是常量表达式 由它的数据类型和初始值共同决定。
        3. 字面值属于常量表达式、常量表达式初始化的const对象也是常量表达式。

    2. constexpr
        1. C++11：允许将变量声明为 constexpr类型以便由编译器来验证变量的值是否是一个常量表达式。
        2. constexpr声明的变量一定是常量，而且必须用常量表达式来初始化

                constexpr int mf = 20;
                constexpr int limit = mf + 1；
                constexpr int sz = size();  //只有当size是一个constexpr函数时，才是一个正确的声明语句。

        3. 字面值类型：算术类型、引用、指针

        4. 一个constexpr指针的初始值必须是nullptr或者0，或者是存储于某个固定地址中的对象。
            1. 函数体内定义的变量一般来说并非存放在固定的地址中，因此constexpr不能指向这样的变量。
            2. 定义于所有函数体之外的对象其地址固定不变，能用来初始化constexpr指针。
            3. constexpr也能绑定到有效范围超出函数体本身的变量，它们有固定的地址。

        5. constexpr 声明中如果定义了一个指针，限定符constexpr仅对指针有效，与指针所指的对象无关。

                const int *p = nullptr;      //  p是一个指向整形常量的指针
                constexpr int *q = nullptr;  //  q是一个指向整数的常量指针。

6. 处理类型
    1. 类型别名
        1. typedef
        2. using SI = Sales_item;
        3. 如果某个类型别名指代的是复合类型或常量，可能出错。
            1. 遇到类型别名，不能简单的把类型别名替换成它本来的样子。

                    typedef char *pstring;
                    const pstring cstr = 0;
                    const pstring *ps;

                    const char *cstr = 0   //是对const pstring cstr错误的理解。

                    声明语句中用到pstring时，基本数据类型是指针。而用char * 重写了声明语句后，数据类型就变成了char， * 成为了声明符的一部分。
        
    2. auto
        > 让编译器替代我们去分析表达式所属的类型。

    3. decltype
        > 选择并返回操作数的数据类型。编译器分析表达式并得到它的类型，却不实际计算表达式的值。

        1. decltype 处理顶层const 和引用的方式和auto不同

                const int ci = 0, &cj = ci;
                decltype(ci) x = 0;
                deltype(cj) y = x; // y的雷兴师const int&,y绑定到x
                deltype(cj) z; // 错误，z是一个引用必须初始化

        2. 如果decltype使用的表达式不是一个变量，则decltype返回表达式结果对应的类型。
            
                int i = 42, *p = &i, &r = i;
                decltype(r + 0) b;
                decltype（*p）c;   //错误：c是int&，必须初始化。 

            1. 如果表达式的内容是解引用操作，则decltype返回引用类型。 **因为解引用是读的指针中保存的地址的变量的值，是那个变量的地址。
                我们猜测过引用就是把引用名解释成和变量一样的地址。这里返回变量的地址，和引用一样的。**

        3. decltype和auto的另一处重要区别是，decltype的结果类型与表达式形式密切相关。

        4. decltype所有的表达式如果变量名加上一对括号的区别。
            1. 不加括号，得到的结果就是该变量的类型
            2. 加括号，编译器把它当做是一个表达式。变量是一种可以作为赋值语句左值的特殊表达式，所以这样的decltype就会得到引用类型。

                    decltype((i)) d;   // 错误，d是int&，必须初始化。
                    decltype(i) e;     // 正确：e是一个未初始化的int 

                1. 切记：decltype((variable)) (双层括号) 的结果永远是引用，而decltype(variable) 结果只有当variable本身就是一个引用时才是引用。

## 基本语法 208
1. 强制类型转换
    1. 形式：  cast-name<type>(expression);
    2. cast-name:
        1. static_cast
            > 任何具有名确定义的类型转换，只要不包括底层const，都可以使用static_const。

        2. dynamic_cast
            > 运行时类型识别

        3. const_cast
            > const_cast 只能改变运算对象的底层const

            1. 去掉const性质
                    
                    const  char *pc;
                    char *p = const_cast<char *>(pc);   //正确：但是通过p写值是未定义的行为。

            2. const_cast不能改变表达式的类型，这时static_cast来改变的。const_cast能改变表达式的常量属性。

        4. reinterpret_cast
            > 通常为运算对象的位模式提供较低层上的重新解释。

            1. eg

                    int *ip;
                    char *pc = reinterpret_cast<char *>(ip);

                    我们必须牢记pc所指的真实对象是一个int而非字符，如果把pc当成普通的字符指针使用就可能在运行时发生错误。

                    string str(pc) // 或导致运行时错误。

2. 循环
    1. switch case中
        1. case标签必须是整形常量表达式。

                char ch = getVal();
                int ival = 34;
                switch(ch){
                    case 3.24; //错误：case标签不是一个整数
                    case ival; //错误：case标签不是一个常量
                }

        2.  标签不应该孤零零出现，它后面必须跟上一条语句或者另外一个case标签。

    2. for 语句中的init-statement可以定义多个对象，但是只能有一条声明语句，因此，所有变量的基础类型必须相同。相当于一个类型定义了多个变量。

            for (decltype(v.size()) i = 0, sz = v.size(); i != sz; ++i)
                v.push_back(v[i]);

    3. 范围for
        > C++11定义，可以遍历容器或其他序列的所有元素

        1. 形式
                
                for (declaration : expression)
                    statement
        
        2. expression:必须是一个序列，比如用花括号括起来的初始值列表、数组、vector、string等，这些类型的共同特点是能返回迭代器的begin和end成员。

        3. declaration定义一个变量
            1. 最简单用auto类型说明符来说明变量类型。
            2. 如果需要对序列中的元素执行写操作，循环变量必须声明成引用类型。

    4. 跳转语句
        1. break；负责终止离它最近的while、do while、for或switch语句。
            > break语句只能出现在迭代语句或者switch语句内部。

        2. continue：终止最近的循环中的当前迭代并立即开始下一次迭代。

        3. goto：从goto语句无条件跳转到统一函数内的另一条语句。
            > 不要在程序中使用goto

            1. 形式： goto label；
            2. 和switch语句类似，goto语句也不能将程序的控制权从变量的作用域之外转移到作用域之内。

            > 不要在程序中使用goto

            1. 形式： goto label；
            2. 和switch语句类似，goto语句也不能将程序的控制权从变量的作用域之外转移到作用域之内。
                        > 不要在程序中使用goto

           
                        1. 形式： goto label；
                        2. 和switch语句类似，goto语句也不能将程序的控制权从变量的作用域之外转移到作用域之内。
                                    > 不要在程序中使用goto

                       
                                    1. 形式： goto label；
                                    2. 和switch语句类似，goto语句也不能将程序的控制权从变量的作用域之外转移到作用域之内。

3. 异常
    1. throw表达式：
        1. 形式：throw + 表达式，表达式的类型是抛出的异常类型。

                if (item1.isbn() != item2.isbn())
                    throw runtime_error("Data must refer to same ISBN");

        2. runtime_error:标准库异常类型的一种，定义在stdexcept头文件中。string对象初始化它。
        
    2. try 语句块：
        1. 形式：try 块之后是一个或多个catch子句。catch子句包括三部分：关键字 catch、括号内一个(可能是未命名)对象的声明(异常声明)、以及一个块。

                try {
                    program-statements;
                } catch (exception-declaration){
                    handler-statements;
                } catch (exception-declaration){
                    handler-statements;
                }

    3. 一套异常类，用于在throw表达式和相关的catch子句之间传递:
        1. 异常类只定义了一个名为what的成员函数，没有参数，返回只是一个指向C字符串的 const char *.

4. 返回数组指针的函数：
    1. 指向数组的指针： int （*p）[10]
    2. 声明一个数组类型的别名： typedef int array[10];  
    3. <font color=green>因为数组的定义是：类型 + 变量名 + [], 所以，数组的类型相当于 int []. 剩下的直接修饰变量名的类型，和顶层const指针有点像</font>
    4. 返回数组指针类型的函数定义：
        1. 使用类型别名：

                typedef int arrT[10];  // arrT是一个类型别名，它表示的类型是含有10个整数的数组。
                using arrT = int [10]; // arrT的等价声明
                arrT *func(int i);     // func 返回一个指向含有10个整数的数组的指针。

        2. 声明一个返回数组指针的函数
            1. 如果想在声明func时不使用类型别名，我们必须牢记被定义的名字后面的数组的维度：

                    int arr[10];
                    int *p1[10];
                    int （*p2）[10] = &arr;  //编译器必须知道这个指针可以指向的内存范围。

            2. 形式： Type （*function(parameter_list)）[dimension]

                    int （*func(int i)）[10];

        3. 尾置返回类型
            > C++11: 任何函数类型定义都可以使用尾置返回，但对于复杂函数最有效

            1. 形式
                1. 尾置返回类型跟在形参列表后面并以一个 -> 符号开头。
                2. 为了表示函数真正的返回类型跟在形参列表之后，我们在本应该出现返回类型的地方放置一个auto：

                        auto func(int i) -> int （*）[10];

        4. 使用decltype
            > 如果我们知道函数返回的指针将指向哪个数组，就可以使用decltype关键字声明返回类型。

            1. eg

                    int odd[] = {1, 3, 5, 7, 9};
                    int even[] = {0, 2, 4, 6, 8};
                    
                    decltype(odd) *arrPtr(int i){
                        return (i % 2) ? &odd : &even;
                    }

            2. decltype(odd) 返回的是一个数组而不是指针，所以定义时，需要自己加上*。

5. 函数重载
    > 同一作用域内的几个函数名字相同但形参列表不同，我们称之为 重载函数(overloaded). 只能是形参不同，因为编译器需要知道调用时调用的那个函数，只能通过实参列表和形参来匹配。 

    1. 对于重载函数来说，它们应该在形参数量或者形参类型上有所不同。
    2. 不允许两个函数除了返回类型外其他所有的要素都相同，类型别名也属于相同类型，编译器不能根据实参判断出是原型还是类型别名。
    3. const形参，函数调用时会忽略顶层const，所以底层const形参也不能作为重载函数区别。一个拥有顶层const的形参和另一个没有顶层const的形参无法区别开。
    4. 但是如果形参是某种类型的指针或引用，则通过区分其指向的是常量对象还是非常量对象可以实现函数重载，此时的const是底层的。
    5. 尽管函数重载能在一定程度上减轻我们函数起名的负担，但最好只重载那些非常相似的操作。

    5. 函数匹配/重载确定
        1. 最佳匹配
        2. 无匹配
        3. 二义性调用

    6. 重载和作用域：
        1. C++中，名字查找在类型检查之间，如果编译器在当前作用域中找到了所需的名字，编译器就会忽略外层作用域中的同名实体。

6. 默认参数
    1. 默认参数必须在参数列表末尾，并且默认参数之后都必须有默认实参。
    2. 用作默认实参的名字在函数声明所在的作用域内解析，而这些名字的求职过程发生在函数调用时：
        
            sz wd = 80;
            char deff = ' ';
            sz ht();
            string screen(sz = ht(), sz = wd, char = def);

            void f2(){
                    def = '*';         // 改变了默认实参的值。 注意，这里是重定义了def的值，而不是重新声明一个新的变量，所以该变了外部变量的值。
                sz wd = 100;       //隐藏了外层的wd，但是没有改变默认值。  注意，这里是重新定义了一个新的变量，作用是在这里隐藏了外部的变量，但默认实参在定义的地方确定了实参的值。
                window = screen(); //调用了screen(ht(), 80, '*')
            }

7. 函数类型
    1. using F = int (int *, int);   // F是函数类型，不是指针
    2. using PF = int（*）(int*, int); // PF是指针类型

    3. 和数组类似，函数内不能反悔一个函数或指针，但是可以返回指向函数类型或数组类型的指针。
    4. 返回函数类型时，必须把返回类型写成指针形式，编译器不会自动将函数返回类型当做成对应的指针类型处理。

    5. eg

            PF f1(int);   //正确
            F f1(int);    // 错误，F是函数类型，f1不能返回一个函数
            F *f1(int);   // 正确，显示的指定返回类型是指向函数的指针。

            int （*f1(int)）(int*, int);

            auto f1(int) -> int （*）(int*, int);

    6. 如果明确知道返回的函数是哪一个，可以使用decltype来简化返回函数指针的函数定义。

## 字符串 418
1. 初始化
    1. 拷贝初始化
        1. 使用等号(=)初始化一个变量，实际上执行的是拷贝初始化，编译器把等号右侧的初
		始值拷贝到新创建的对象中去。
		```
		string s2 = s1;
		```

    2. 直接初始化
        1. 不适用等号，执行的是直接初始化
		```
		string s2(s1);
		```

## 字面量 433
> 原始字符串字面量

1. 传统 C++ 里面要编写一个充满特殊字符的字符串其实是非常痛苦的一件事情，
	比如一个包含 HTML 本体的字符串需要添加大量的转义符，例如一个Windows 上的文件路径经常
	会：C:\\What\\The\\Fxxk。

2. C++11 提供了原始字符串字面量的写法，可以在一个字符串前方使用 R 来修饰这个字符串，
	同时，将原始字符串使用括号包裹，例如：
	```
		#include <iostream>
		#include <string>

		int main() {
			std::string str = R"(C:\\What\\The\\Fxxk)";
			std::cout << str << std::endl;
			return 0;
		}
	```

3. 自定义字面量
> C++11 引进了自定义字面量的能力，通过重载双引号后缀运算符实现：
```
// 字符串字面量自定义必须设置如下的参数列表
std::string operator"" _wow1(const char *wow1, size_t len) {
	return std::string(wow1)+"woooooooooow, amazing";
}

std::string operator"" _wow2 (unsigned long long i) {
	return std::to_string(i)+"woooooooooow, amazing";
}

int main() {
	auto str = "abc"_wow1;
	auto num = 1_wow2;
	std::cout << str << std::endl;
	std::cout << num << std::endl;
	return 0;
}
```

4. 自定义字面量支持四种字面量：

    - 整型字面量：重载时必须使用 unsigned long long、const char *、
	模板字面量算符参数，在上面的代码中使用的是前者；

    - 浮点型字面量：重载时必须使用 long double、const char *、模板字面量算符；

    - 字符串字面量：必须使用 (const char *, size_t) 形式的参数表；

    - 字符字面量：参数只能是 char, wchar_t, char16_t, char32_t 这几种类型。

## 动态内存 485
1. 动态内存和智能指针
    0. 动态内存是通过一对运算符来完成的：
        1. new ：在动态内存中为对象分配空间并返回该对象的指针，可以选择为它进行出世哈
        2. delete : 接受一个动态对象的指针，销毁该对象，并释放与之关联的内存。

    1. shared_ptr类
        1. 只能指针也是模板，所以申请时需要指明指向的类型： shared_ptr<string> p1; shared_ptr<list<int>> p2;
        2. 默认初始化的智能指针中保存着一个空指针
        3. 智能指针的使用和普通指针类似，解引用一个智能指针返回它所指的对象
        4. 如果在条件判断中使用一个智能指针，效果就是检测它是否为空。
        5. 操作
            1. shared_ptr和uniue_ptr都支持的操作
                1. shared_ptr<T> sp
                2. unique_ptr<T> up   空智能指针
                3. p                  条件判断，若p指向一个对象，则为true
                4. *p
                5. p->mem
                6. p.get()           返回p中保存的指针，要小心使用，若智能指针释放了起对象，返回的指针所指的对象也就消失了，应该可以和弱引用相互使用。
                7. swap(p, q)        交换p和q中的指针
                8. p.swap(q)

            2. shared_ptr独有的操作
                1. make_shared<T> (args)   :返回一个shared_ptr对象，指向一个动态分配的T类型对象，使用args来初始化此对象
                2. shared_ptr<T> p(q) :    p是shared_ptr q的拷贝，此操作会递增q中的计数器，q中的指针必须能转换为T*
                3. p = q;
                4. p.unique()
                5. p.use_count()

        6. make_shared函数
            > 定义在memory头文件中
                
            1. shared_ptr<int> p3 = make_shared<int>(42);
            2. 类似于容器的emplace函数，make_shared 用其参数来构造给定类型的对象。

        7. shared_ptr 的拷贝和赋值

        8. shared_ptr自动销毁所管理的对象

        9. shared_ptr 还会自动释放相关联的内存
            
    2. 直接管理内存
        1. new分配和初始化对象
            1. 自由空间分配的内存是无名的，因此，new无法为其分配的对象命名，而是返回一个指向该对象的指针
            2. 默认情况下，动态分配的对象是默认初始化的：内置类型或组合类型值将是未定义的，类类型是默认构造函数
            3. 直接初始化
                1. int *pi = new int(1025)
                2. string *p = new string(10, '9')
                3. vector<int> *pv = new vector<int>{0, 1, 2};

            4. 也可以对动态分配的内存值初始化：只需在类型名之后跟一对空括号即可
                1. string *ps1 = new string() / string *ps2 = new string;
                2. 值初始化后，就可以使用auto来判断申请的类型，但是编译器需要用初始化的类型来推断分配的类型，所以括号里只能有一个初始化器时才可以

            5. 动态分配的const对象

            6. 内存耗尽
                1. 程序的内存耗尽，new会失败，会抛出一个类型为 bad_alloc的异常，可以用 nothrow 来组织它抛出异常

                        int *p = new int;
                        int *p = new (nothrow) int;   // 如果分配失败，new返回一个空指针。

            7. 释放内存
                1. delete表达式：delete 接受一个指针，指向我们要释放的对象
                
            8. 动态对象的生存期直到被释放时为止

            9. 动态内存管理容易出错
                1. 忘记delete内存
                2. 使用已经释放掉的对象
                3. 同一块内存释放两次。

            10. delete 之后要重置指针值
                1. **delete一个指针后，指针值就无效了，就是所谓的空悬指针**
                2. 避免空悬指针的方法：
                    1. 在指针离开其作用域之前释放掉它所关联的内存，可以避免空悬指针。
                    2. 如果需要保留指针，可以在delete之后，将nullptr赋予指针。

    3. shared_ptr和new结合使用
        1. 如果不初始化一个智能指针，他就会被初始化为一个空指针
        2. 接受指针参数的智能指针构造函数是explicit的，因此，不能将一个内置指针转换为一个智能指针，必须使用直接初始化形式来初始化一个智能指针

                shared_ptr<int> p1 = new int(1024);  //错误，必须使用直接初始化形式
                shared_ptr<int> p1(new int(1024));   //正确
    
                shared_ptr<int> clone(int p){
                    return new int(p);    //错误，隐式转换为shared_ptr<int>
                    return shared_ptr<int>(new int(p));
                }

        3. 智能指针是一个独特的类型，记住第2点，它的构造函数是explicit的，不能讲普通指针隐式转换为智能指针，因为没有申请对象的引用计数对象。new返回的是普通指针。也就是说，普通指针只能作为智能指针的构造函数的参数，不能拷贝赋值。

        4. 默认情况下，初始化智能指针的普通指针必须指向动态内存，因为智能指针默认使用delete释放它所关联的对象。
        5. 但是也可以将智能指针绑定到一个指向其他类型的资源的指针上，但这样做，**必须提供自己的操作来代替delete。**

        6. 定义和改变shared_ptr的其他方法
            1. shared_ptr<T> p(q);
            2. shared_ptr<T> p(u);   将u置为空。
            3. shared_ptr<T> p(q, d);   d是代替delete的。p接管了内置指针q所指对象的所有权
            4. shared_ptr<T> p(p2, d);   d是代替delete的。p接管了shared_ptr p2所指对象的所有权
            5. p.reset()
            6. p.reset(q)           p不再指向原来的内存，接受一个内置指针的参数q。注意，p被设为空指针，如果没有q，并且如果p是唯一的shared_ptr，对象会被删除。d代替delete
            7. p.reset(q, d)

        7. 不要混合使用普通指针和智能指针
            1. 使用一个内置指针来访问一个智能指针所负责的对象是很危险的，因为我们无法知道对象何时会被销毁。

        9. 也不要使用get初始化另一个智能指针或为智能指针赋值。
            1. 智能指针的get函数返回一个内置指针，指向智能指针管理的对象
            2. 场景：
                1. 需要向不能使用智能指针的代码传递一个内置指针
                2. 使用get返回的指针的代码不能delete此指针。

            3. get指针来将指针的访问权限传递个代码，确定代码不会delete指针的情况下，才能用get。永远不要get初始化另一个智能指针或为另一个智能指针赋值。

        10. 其他shared_ptr操作
            1. reset将一个新指针赋值给一个shared_ptr：
                1. p = new int(1024);  //错误
                2. p.reset(new int(1024));
            2. shared_ptr像指向一个内置指针，一是初始化式接受内置指针的参数，二是reset接受内置参数。

    4. 智能指针和异常
        1. 使用智能指针，即便程序块过早释放结束，智能指针类也能确保在内存不再需要时将其释放。而内置指针在异常发生时很可能没有释放资源。
        2. 智能指针和哑类
            1. 分配了资源，而又没有定义析构函数来释放这些资源，可能会遇到与使用动态内存相同的错误，没有释放资源。在资源分配和资源释放之间发生了异常，也会发生资源泄露。
            2. 以shared_ptr指向分配的资源，以自定义的释放资源的类来代替shared_ptr的delete函数。

        3. 智能指针陷阱：
            1. 不适用相同的内置指针初始化或reset多个智能指针
            2. 不delete get()返回的指针
            3. 不适用get() 初始化或reset另一个智能指针。
            4. get()返回的指针，当最后一个对应的智能指针销毁后，就无效了
            5. 如果智能指针管理的资源不是new分配的内存，记住传递给他一个删除器。

    5. unique_ptr
        1. unique_ptr 拥有它所指的对象，unique_ptr被销毁时，它所指的对象也被销毁。
        2. 定义一个unique_ptr时，必须将它绑定到一个new返回的指针上。初始化unique_ptr和shared_ptr类似，必须采用直接初始化形式。
            
                unique_ptr<double> p1;
                unique_ptr<int> p2(new int(42));

        3. unique_ptr 独占它所指向的对象，所以unique_ptr不支持普通的拷贝或赋值操作。
            
                unique_ptr<string> p1(new string("Stegosaurus"));
                unique_ptr<string> p1(p2);             //错误，unique_ptr不支持拷贝。 
                unique_ptr<string> p3;
                p3 = p2                                  //错误，unique_ptr不支持赋值

        4. unique_ptr操作
            1. unique_ptr<T> u1 
            2. unique_ptr<T, D> u2     u2会使用一个类型为D的可调用对象来释放它的指针
            3. unique_ptr<T, D> u(d)    d相当于delete
            4. u = nullptr
            5. u.release()     u放弃指针的控制权，返回指针，并置为空
            6. u.reset()  释放对象
            7. u.reset(q)      如果提供了内置指针q，令u指向这个对象，否则将u置为空。
            8. u.reset(nullptr)

        5. 不能拷贝或赋值unique_ptr，但可以通过release或reset将指针所有权从一个unique_ptr转移给另一个。
            
                unique_ptr<string> p2(p1.release());
                unique_ptr<stirng> p3(new string("Trex");
                p2.reset(p3.release())

        6. 如果unique_ptr不为空，reset原来的对象被释放。
        7. release会切断unique_ptr和它原来管理的对象间的联系。release返回的指针通常会被用来初始化另一个智能指针或给另一个智能指针赋值。如果不用智能指针来保存release返回的指针，需要自己来delete。

                p2.release();   //错误，p2不会释放内存，并且我们丢失了指针
                auto p = p2.release()  //正确，但是我们必须自己delete(p).
        
        8. 传递unique_ptr参数和返回unique_ptr
            1. 不能靠别unique_ptr的规则有一个例外：我们可以拷贝或赋值一个将要被销毁的unique_ptr，最常见的例子是函数返回一个unique_ptr

                    unique_ptr<int> clone(int p){
                        return unique_ptr<int>(new int(p));
                        }

                    unique_ptr<int> clone(int p){
                        unique_ptr<int> ret(new int(p));
                        return ret;
                        }

        9. 向unique_ptr 传递删除器
            1. 类似于shared_ptr，unique_ptr默认情况下用delete释放它指向的对象，我们也可以重载一个unique_ptr中的默认删除器。这和shared_ptr直接初始化指定删除器不同。
            2. unique_ptr<objT, delT> p(new objT, fcn);

    6. weak_ptr
        1. weak_ptr是不控制所指对象生存期的智能指针。
        2. 指向shared_ptr管理的对象
        3. 不改变shared_ptr引用计数
        4. 操作；   
            1. weak_ptr<T> w
            2. weak_ptr<T> w(sp)
            3. w = p        shared_ptr或者weak_ptr
            4. w.reset()
            5. w.use_count()
            6. w.expired()
            7. w.lock()       如果expired为true，返回一个空的shared_ptr，否则返回一个指向w的shared_ptr
        5. 创建一个weak_ptr，必须用一个shared_ptr来初始化它。
        6. 因为对象可能不存在，所以我们不能使用weak_ptr来直接访问对象，而必须调用lock。
        7. 核查指针类

2. 动态数组
    1. new和数组
        1. 为了让new分配一个对象数组，在类型名之后跟一对方括号，指明要分配的对象的数目。
        2. new分配要求数量对象并返回指向第一个对象的指针。

                int *pia = new int[get_size()];
                方括号里大大小必须是整数，但不必是常量。

        3. 分配一个数组会得到一个元素类型的指针。
            1. new T[]分配一个数组时，我们得到的是一个数组元素类型的指针，所以不可以使用数组的方法比如 begin、end、for
            2. 动态数组是指针，而不是数组。

        4. 初始化动态分配对象的数组
            1. 默认情况下，new分配的兑现都是默认初始化的。可以进行值初始化，方法是在后面加一对空括号。

                    int *pia = new int[10];
                    int *pia = new int[10]();
            2. 还可以提供一个元素初始化器的花括号列表
                
                    int *pia3 = new int[10]{0, 1, 3, 4,5,6,6};

            3. 可以加上空括号来值初始化，但是括号里不能给出初始化器。

        5. 动态分配一个空数组是合法的
            
                char arr[0];   //错误：不能定义长度为0的数组
                char *cp = new char[0]   //正确：但cp不能解引用。

        6. 释放动态数组
            1. 为了释放动态数组，需要特殊形式的delete：在指针前加上一个空方括号对：
                    
                    delete p;    //p必须指向一个动态分配到对象或空
                    delet [] pa; //pa必须指向一个动态分配的数组或空。

        7. 智能指针和动态数组
            1. 标注库提供了一个可以管理new分配数组的unique_ptr版本

                    unique_ptr<int[]> up(new int[10]);
                    up.release();   //自动调用delete [] 销毁其指针。   注意，unique_ptr的release本身不会释放资源的，不能用这个方法释放指针。但是动态数组似乎重载了方法。

            2. 当一个unique_ptr指向一个数组时，不能用点和箭头成员运算符，但是可以用下标运算符。

            3. shared_ptr不支持管理动态数组。如果一定要使用shared_ptr管理动态数组，必须自己提供定义的删除器。

                    shared_ptr<int> sp(new int[10], [](int *p) {delete[] p;});
                    sp.reset()
            4. shared_ptr不直接支持动态数组管理会影响我们访问数组元素

                    for(size_t i = 0; i != 10; i++)
                        *(sp.get() + i) = i;  

                    shared_ptr未定义下标运算符，而且智能指针类型不支持指针算术运算，因此必须使用get来获取一个内置指针。

    2. allocator类  
        1. new 将内存分配和对象构造连在一起，allocator定义在memory中，它将内存分配和对象构造分离开来。
        2. 它分配的内存是原始的，未构造的

                allocator<string> alloc;               //可以分配string的allocator对象
                auto const p = alloc.allocate(n)       //分配n个未初始化的string
                这个allocate调用为n个string分配了内存

        3. 操作
            1. allocator<T> a
            2. a.allocate(n)
            3. a.deallocate(p, n)    调用之前，用户必须对每个在这块内存中创建的对象调用destroy
            4. a.construct(p, args)  args被传递给类型为T的构造函数
            5. a.destroy(p)

        4. allocator分配未构造的内存
            1. 为了使用allocate返回的内存，我们必须使用construct构造对象，使用未构造的内存，其行为未定义。
            2. construct接受一个指针和零个或多个的额外参数，额外参数传递个类型对象的构造函数做参数
                    
                    allocator<string> alloc;
                    auto const p = alloc.allocate(n)

                    auto q = p;
                    alloc.construct(q++);
                    alloc.construct(q++, 10, 'c');
                    alloc.construct(q++, "hi");

        5. 使用完对象后，必须对每个构造元素调用destroy来销它们
            
                while (q != p):
                    alloc.destroy(--q);

            1. 我们只能对真正构造了的元素进行destroy操作。
            2. 一旦元素被销毁，就可以重新使用这部分内存来保存其他的string，也可以将其归还给系统，释放内存通过调用deallocate来完成。

                    alloc.deallocate(p, n);

                    p必须是allocate分配的内存指针，n必须与分配的内存大小一致。

        6. 拷贝和填充未初始化内存的算法
            1. memory文件中定义了两个伴随算法，可以在未初始化的内存中创建对象。
            2. 算法
                1. uninitialized_copy(b, e, b2)
                2. uninitialized_copy_n(b, n, b2)
                3. uninitialized_fill(b, e, t)
                4. uninitialized_fill_n(b, n, t)

3. 使用标准库

## 类设计 1131
### 类 1132
1. 定义抽象数据类
    1. 函数定义在类内默认转化为inline函数。
    2. 函数一般在类外定义。如果是很简单的方法，可以在类内定义。
    3. 引入this，传递个类方法的默认参数，相当于python的self，只不过默认传递，可以返回this。
    4. const成员函数，如果类方法不改变类本身，只是执行一些读取操作之类的方法，
	可以定义为const方法，在参数列表之后加上const，表示这个方法不会改变类对象本身。

2. 友元
3. 类的其他特性
4. 类的作用域
5. 构造函数
6. 静态成员

### 拷贝控制 1146
0. 拷贝控制操作
	1. 五种成员函数
	2. 威慑么编译器自动为类定义缺失的拷贝控制函数,我们还是要自己定义.

1. 拷贝,复制与销毁
	1. 拷贝构造函数
		> 如果一个构造函数的第一个参数是自身类型的引用,且任何额外的参数都有默认值,则此构造函数是拷贝构造函数.  为什么是引用才可以,不能是值?

		1. 第一个参数总是const引用,但不是必须的
		2. 因为会被隐式调用,所以不能是explict的
		3. 合成拷贝构造函数
			1. 总是会有一个拷贝构造函数,我们没定义,编译器定义,而且别的构造函数不会覆盖拷贝构造函数,不像是合成的默认构造函数.

		4. 拷贝初始化
			1. 编译器匹配
				1.直接初始化
				2. 拷贝初始化

			2. 场景
				1. = 定义变量
				2. 非引用对象实参
					1. **这就是为什么拷贝构造函数必须是引用类型,因为不是引用的的实参调用拷贝函数,这样会陷入无限循环.**
					2. **因为仅仅是参数是否是引用编译器不能区分实参调用时匹配到那个构造函数,所以仅仅形参是否是引用不同不能重载.因此,如果定义类一个构造函数是对象的非引用类型的形参,就不可以定义拷贝构造函数类,因此定义一个以对象本身的非引用形参的构造函数是非法的,因为我们必须要有一个拷贝构造函数的.**
				3. 非引用类型的函数返回值
				4. 花括号列表初始化一个数组元素或聚合类中的成员.
				5. 容器初始化或者insert或push成员,也是拷贝初始化, emplace是直接初始化

	2. 拷贝赋值运算符
		1. 像拷贝构造函数一样,如果类没有定义的话,编译器合成一个.
		2. 赋值运算符通常应该返回一个指向其左侧运算符对象的引用.
		3. 拷贝赋值运算符会将右侧运算对象的每个非static对象赋予左侧运算对象的对应成员.

	3. 析构函数
		1. 构造函数初始化对象的非static数据成员,还可能做些其他工作,析构函数释放对象使用的资源,并销毁对象的非static数据成员
		2. 析构函数不接受参数,因此不会重载,每个类,只会有一个析构函数.
		3. 隐式销毁一个内置指针类型的成员不会delete它所指向的对象.
			1. 因为内置类型没有析构函数.智能指针有析构函数.
		4. **当指向一个对象的指针或引用离开作用域时,析构函数不会执行**
		5. 对于某些类,析构函数用来阻止对该类型的对象的销毁,如果不是这种情况,合成析构函数就为空.
		6. **析构函数体自身并不直接销毁对象,成员是在析构函数体之后隐含的析构阶段中被销毁的.
			1. 析构函数会调用成员自身的类型的析构函数来销毁成员.所以如果成员由普通指针指向的动态内存,合成的析构函数就不能销毁动态内存.

	4. 三/五法则
		1. 如果一个类需要自定义析构函数,几乎可肯定它也需要自定义拷贝赋值运算符和拷贝构造函数.
		2. 需要拷贝操作的类也需要赋值操作,反之亦然.

	5. 使用default
		1. 只能对具有合成版本的成员函数使用=default(即,默认构造函数或拷贝控制函数).
		2. 通过就爱你狗成员定义为=default,来显式的要求编译器生成合成的版本.

	6. 阻止拷贝
		1. 阻止编译器合成的某些方法
		2. 虽然声明类这些函数,但是不能以任何方法调用它们,这就是删除函数.
		3. 在函数的形参列表后面加上 =delete.
		4. 析构函数不能是删除成员.
			1. 对于一个删除类析构函数的类型,编译器不允许定义该类型的变量或创建该类型的变量.
			2. 如果一个成员无法被销毁,则对像整体也就无法被销毁类.

		5. 合成的拷贝控制成员可能是删除的.
			1. p450.
			2. 本质上,这些规则的含义是:如果一个类有数据成员不能默认构造,拷贝,复制或销毁,则对应的成员函数将定义为删除的.
			 
2. 拷贝控制和资源管理
	1. 行为像值的类
        1. 在类里面有指针时，复制指针所指向的值而不是复制指针。  拷贝构造函数、析构函数来释放复制的指针、拷贝赋值运算符
        2. 关键概念
            1. 如果将一个对象赋予它自身，赋值运算符必须能正确工作。
            2. 大多数赋值运算符组合了析构函数和拷贝构造函数的工作。

	2. 定义行为像指针的类

3. 交换操作
	

4. 拷贝控制示例

5. 动态内存管理类

6. 对象移动
	1. 右值引用
	2. 移动构造函数和移动赋值运算符
	3. 右值引用和成员函数

7. 总结

### 操作重载与类型转换 1232

### 面向对象 1234
1. 面向对象设计基于三个基本概念：
    1. 数据抽象：可以将类的接口与实现分离
    2. 继承:  可以定义相似的类型并对其相似关系建模
    3. 动态绑定：可以在一定程度上忽略相似类型的区别

2. 继承
    1. 虚函数
        > 对于某些函数，基类希望它的派生类个自定义适合自己的版本，
		此时基类就将这些函数声明成为虚函数

        1. 派生类必须在其内部对所有重新定义的虚函数进行声明。

        2. 动态绑定发生在程序运行时，
		因为只有运行时才知道到底绑定了那个版本的虚函数。

        3. 必须为每一个虚函数都提供定义，而不管它是否被调用到，
		因为连编译器而已无法确定到底会使用哪个虚函数。

        4. 动态绑定只有当我们通过指针或引用调用虚函数时才会发生。

        5. note: p563

    2. 派生类使用类派生列表明确指定它是从哪个(哪些)基类继承而来。
	格式，先是一个冒号，后面紧跟逗号分隔的基类列表，每个基类前可以有访问说明符

    3. 动态绑定
        > 在C++语言中，当我们使用基类的引用(或指针)调用一个虚函数时，
		将发生动态绑定。

3. 定义基类和派生类
    1. 基类通常都应该定义一个虚析构函数，
	即使该函数不执行任何实际操作也是如此。
    
    2. 定义基类
        1. 基类两种成员函数
            1. 希望派生类覆盖的函数，将其定义为虚函数。
			当使用指针或引用调用虚函数时，该调用将被动态绑定，
			根据引用或指针所绑定对象的类型不同，
			该调用执行不同的派生或者基类版本。

            2. 希望派生类继承不要改变的函数。

        2. virtual只能出现在类内部的声明语句之前。

        3. **成员函数如果没被声明为虚函数，
		则其解析过程发生在编译时而非运行时。**

        4. protected，派生类可以访问，其他对象不可访问，
		private对象，派生类也不可以访问。

    4. 定义派生类
        1. 派生类必须通过类派生列表指定基类

        2. 每个基类前可以有一下三种说明符：public、protected、private

        3. 派生类必须将继承来的成员函数中需要覆盖的那些重新声明。
		这些函数前面可以加上virtual，但不强制这样做。
		C++11允许在重写的成员函数后添加override关键字

        4. 内存中，派生类的内存空间是：前面是基类的对象空间，
		后面是派生类新定义的对象。但内存不一定是连续的存储的。

        5. 因为在派生类对象中含有基类对应的部分，所以我们可以把派生类当
		成基类对象来使用。而且也可以把基类的指针或引用绑定到派生类
		对象中的基类部分上。

        6. 派生类构造函数
            1. 派生类不能直接初始化基类的成员，
			必须使用基类的构造函数来初始化它的基类部分。

            2. 每个类控制它自己的成员的初始化过程。

            3. 派生类同样是通过构造函数初始化列表来将
			实参传递给基类构造函数的：
			```
			quote(const std::string& book, double p, std::size_t qty, 
				double disc):Base(book, p), min_qty(qty), discount(disc) {}
			```

            4. 首先初始化基类的部分，然后按照声明的顺序依次初始化派生类的成员。

                1. 始化列表的顺序要跟你在类声明的顺序要一致，否则可能出现问题。

                2. 只要成员变量的初始化不依赖其他成员变量，即使顺序不同也能正确的初始化

                3. **成员变量在使用初始化列表初始化时，与构造函数中初始化成员列表的顺序无关，
				只与定义成员变量的顺序有关**

    5. 静态成员
        1. 如果基类定义了一个静态成员，则在整个继承体系中只存在该成员的唯一定义。
		无论派生多少类，每个静态成员都只存在唯一的实例。 

    6. 派生类的声明
        1. 派生类的声明包含类型但是不包含它的派生列表：
            
                class quote: base;   //错误，派生列表不能出现在这里
                class quote;         //正确

    7. 派生类的基类必须是已经定义而非是仅仅声明的。因此，一个类不能派生它本身。

    8. 不能继承的类：在类名后跟一个关键字final。

    9. 类型转换
        1. 智能指针和内置指针一样都支持指向基类的指针可以绑定到派生类。

    10. 权限
        1. 类体内的修饰
            1. private ：只能被定义的类本身的成员函数所访问，即使是类的对象都不能访问，
			它只能被定义类的类体内的成员访问。

            2. protected：只能被类和类的子类所访问，即使是定义它的类的对象都不可以访问。
			它的访问范围就是类定义体内的成员函数、子类定义体内的成员函数。

            3. public：所有对象都可以访问。

            4. 总结
                1. private、protected都只能被类本身的成员函数所访问，而不能被对象所访问。
				只有public才可以被对象访问。

        2. 派生访问说明符：
            1. 公有继承    继承自父类的成员保持不变。
            2. 私有继承    继承自父类的成员全部变为私有成员。
            3. 保护继承    继承自父类的公有成员变为保护成员，其余不变。

            4. 总结：
                1. 这里的公有、私有指的是派生类里面对象的权限，而不是访问base类里的权限。

                2. 所以即使是private继承，原来base类里的protected和public，
				派生类里定义的方法也是可以访问的。

    11. 派生类向基类转换的可访问性
        1. 派生类向基类转换时，只会使用派生类继承的那部分成员，
		只要保证对象访问那部分基类的部分权限不变就可以。

        2. 一句话就是：原来可以访问基类成员的对象(类对象、类成员、派生类成员)的
		权限没有被改变就可以由派生类向基类转换。

            1. 类对象: 派生类的类对象只能访问public的成员，所以，派生向基类转换过程中，
			派生类的类对象还是能访问基类public成员。只有public继承基类，
			派生类对象才可以访问基类的public成员。

            2. 类成员函数: 类成员函数可以访问所有类属性，不管是public、protected还是private，
			类成员都能访问，所以派生类成员函数访问所有的派生类从base继承过来的成员。

            3. D继承B，则D的派生类E只能访问D中protected和public成员，
			所以对派生类成员函数来说，可以使用这两种的继承方式。E不能访问D中的private，
			所以D的private继承不能转换。

            4. 友元和类方法是一致的。

﻿## C++17 简介
1. 主要入选特性
	非类型模板参数的 auto
	std::variant<>
	结构化绑定(Structured bindings)
	变量声明的强化
2. 未入选特性
	Concepts

### 主要入选特性
1. 非类型模板参数的 auto
	1. 模板参数分为两种，一种是类型模板参数，也是我们用得最多的一种：

			template <typename T, typename U>
			auto add(T t, U u) {
				return t+u;
			} 

	里面的 T 和 U 都是类型模板参数。另一种是非类型模板参数，它可以让不同的字面量成为模板的参数：

		template <typename T, int BufSize>
		class buffer_t {
		public:
			T& alloc();
			void free(T& item);
		private:
			T data[BufSize];
		}

		buffer_t<int, 100> buf; // 100 作为模板参数

	2. 遗憾的是我们在编写模板的时候就必须明确非类型模板参数的具体类型，
		C++17 打破了这一限制，让我们能够在非类型模板参数中使用 auto 关键字，从而让编译器推导具体的类型：

			template <auto value> void foo() {
				return;
			}

			foo<10>();  // value 被推导为 int 类型

### std::variant<>
1. 熟悉 boost 的人应该很早就听说过 variant<> 了。
	- variant<> 可以用于存储和操作不同类型的对象。
	- 我们在前面（对标准库的扩充：新增容器）对于迭代 std::tuple 时，简单使用了 boost::variant<>。
	- 提供给 variant<> 的类型模板参数可以让一个 variant<> 从而容纳提供的几种类型的变量
	- （在其他语言(例如 Python/JavaScript 等)表现为动态类型）。

2. C++17 正式将 variant<> 纳入标准库，摇身一变成为 std::variant<>，有了它之后，我们可以将前面的代码更改为：

		#include <variant>
		template <size_t n, typename... Args>
		std::variant<Args...> _tuple_index(size_t i, const std::tuple<Args...>& tpl) {
			if (i == n)
				return std::get<n>(tpl);
			else if (n == sizeof...(Args) - 1)
				throw std::out_of_range("越界.");
			else
				return _tuple_index<(n < sizeof...(Args)-1 ? n+1 : 0)>(i, tpl);
		}
		template <typename... Args>
		std::variant<Args...> tuple_index(size_t i, const std::tuple<Args...>& tpl) {
			return _tuple_index<0>(i, tpl);
		}

### 结构化绑定(Structured bindings)
> 结构化绑定提供了类似其他语言中提供的多返回值的功能。

1. 到目前为止，我们可以通过 std::tuple 来构造一个元组，囊括多个返回值。但缺陷是显而易见的，我们没有一种简单的方法直接从元组中拿到并定义元组中的元素，尽管我们可以使用 std::tie 对元组进行拆包，但我们依然必须非常清楚这个元组包含多少个对象，各个对象是什么类型。

2. C++17 给出的结构化绑定可以让我们写出这样的代码：

		std::tuple<int,double,std::string> f() {
			return std::make_tuple(1,2.3,"456");
		}
		auto [x,y,z] = f(); // x,y,z 分别被推导为int,double,std::string

### 变量声明的强化
1. 变量的声明在虽然能够位于任何位置，甚至于 for 语句内能够声明一个临时变量 int，
	但始终没有办法在 if 和 switch 语句中声明一个临时的变量。例如：

		auto p = map_container.try_emplace(key, value);
		if(!p.second) {
			//...
		} else {
			//...
		}

2. C++17 消除了这一限制，使得我们可以：

		if (auto p = m.try_emplace(key, value); !p.second) {   
			//...
		} else {
			//...
		}

### 未入选特性
1. Concepts TS
> Concepts 是对 C++ 模板编程的进一步增强扩展。

1. 简单来说，Concepts 是一种编译期的特性，它能够让编译器在编译期时对模板参数进行判断，
从而大幅度增强我们在 C++ 中模板编程的体验。使用模板进行编程时候我们经常会遇到各种令人发指的错误，
这是因为到目前为止我们始终不能够对模板参数进行检查与限制，例如下面简单的两行代码会造成大量的几乎不可读的编译错误：

		#include <list>
		#include <algorithm>
		int main() {
			std::list<int> l = {1, 2, 3};
			std::sort(l.begin(), l.end());
			return 0;
		}

2. 而这段代码出现错误的根本原因在于，std::sort 对排序容器必须提供随机迭代器，否则就不能使用，
	而我们知道 std::list 是不支持随机访问的。
	
3. 用 Concepts 的话来说就是：std::list中的迭代器不满足std::sort中随机迭代器这个 Concepts（概念） 的 requirements（要求）。
	1. 有了 Concepts，我们就可以这样：

			template <typename T> 
			requires Sortable<T>    // Sortable 是一个 concept
			void sort(T& c);

		缩写为：

			template<Sortable T>    // T 是一个 Sortable 的类型名
			void sort(T& c)

	2. 甚至于直接将其作为类型来使用：

			void sort(Sortable& c); // c 是一个 Sortable 类型的对象

4. 遗憾的是，C++组委会没有将 Concetps 纳入新标准，而是将其作为TS正式发布
（其实早在 C++11 最终定案之前就已经有 Concepts 的呼声了，但 Concepts TS 是2015年才完整正式发布），
也就是我们现在看到的 Concepts TS。C++组委会拒绝将 Concepts 纳入新标准的原因其实很简单，并不是技术层面上的原因，纯粹是觉得它还不够成熟。

Concepts TS 的发布到最后一次 C++17 的讨论会只相隔了不到四个月的时间，Concepts 的（唯一）实现只存在于一个未发布的 gcc 版本中。而 gcc 中关于 Concepts 的实现就是由撰写 Concepts TS 的人开发的，虽然它能够进行相关测试，但还没有认真讨论过这份 TS 会产生哪些不良后果，更何况这份 TS 都没有被测试过。此外，已知的 Concepts 的一个明显的作用就是去辅助实现 Ranges TS 等提案，但实际上它们也没有被选入 C++17，所以可以把 Concepts 继续延后。

###总结
1. 总的来说，类似于 Concepts/Ranges/Modules 这些令人兴奋的特性并没有入选至 C++17，
	这注定了 C++17 某种意义上来说相较于 C++11/14 依然只是小幅度更新，但我们有望在 C++2x 中看到这些东西的出现，
	这些内容对于一门已经三十多岁『高龄』的编程语言，依然是充满魅力的。
## C++ 静态变量之为什么只初始化一次

1. 首先问题来自这个函数:

```
int fun() 
{ 
    static int i=0; 
    i++; 
    return i; 
} 
```

2. 当这个函数被反复调用时,i的值是会一直加的,也就是静态变量只被初始化了一次.

3. 我对此产生了疑惑.不知道大家有没有.当改为:
```
int fun() 
{ 
    int i=0; 
    i++; 
    return i; 
} 
```

4. 反复调用时都返回的是1,这个例子很明白,
	1. 虽然i这个变量时同名的,但是地址是不一样的,这是一个新的变量,因为在作
	用域后,原变量被销毁了.接着创建一个新的变量.

	2. 那么这里我们就知道了一件事情:就是静态变量是全局的,和程序的生命周期是一样的.

	3. 那么原来的staic的i依然存在. 但是它的可访问区域就只能是定义的作用域,而不一定能全局访问,
	这是全局变量的一个差别.

5. 而静态变量的必须初始化,如果没有显示初始化,则初始化为0 ,'0'或者其他.

6. <font color=red>初始化在程序第一次加载中开始.并设置了相应的标志位!</font>
	1. 所以当变量重复时,编译器忽略了重复初始化的代码,到这里就算是C++只能初始化一次的原因了!

### 另外参考:
1. 操作系统在加载程序时会根据程序中的声明部分为程序分配内存空间(这部分数据是由编译器生成的)。
程序所支配的内存空间分为两大部分：
	1. 静态区域和动态区域(至于为什么这样区分涉及到硬件知识，建议LZ不要深究)：
	2. 动态区域用于存储经常会变动的数据
	3. (动态区域又分为两大部分：栈和堆，关于这两个部分大家应该都很熟悉了)；
	4. 静态区域(Java里称为永久区域)用于存储不会经常变化的数据，例如程序的指令代码(C/C++里就是各个函数
	编译后得到的代码)、用户类型(结构体、类)的声明代码、全局变量、静态变量……PS：有些例子提到用静态变
	量来做递归的计数器，那只是为了说明静态变量的特性。实际上不推荐这种用法，因为这样会
	对程序性能造成轻微的影响。

	5. 静态区域内的数据会在程序加载时进行初始化，生存期为程序运行的全部时间。

	6. 另外，纠正LZ的一个误解：任何变量都只进行一次初始化。局部变量在程序块结
	束时生存期就结束了，下次再调用这个程序块时从原理上说声明的是另一个变量了(分配到的地址也不一定一样)

	PS：在不同编译器的不同编译情况时，实际的内存分区可能不同。例如TC的Small模式下堆和栈区是重合的，
	而Tiny模式下连静态区域和动态区域都是重合的。
## CallableObject
1. 可被某种方式调用其某些函数的对象,它可以是:
	1. 一个函数,接受额外传入的args作为实参(arument)

	2. 一个指向成员函数的指针,<font color=green>当你通过对象调用它,该对象被传递成为第一个实参
	(必须是个reference或pointer)</font>,其他实参则一一对应成员函数的参数.

	3. 一个函数对象(function object,该对象拥有operator()),附带的args被传递作为实参.

	4. 一个lambda,严格地说,它是一种函数对象.
## base
1. 50.md
﻿## istream
> 使用isteam对象当做条件时，其效果是检测流的状态。

1. 例子
```
	while (std::cin >> value)
		pass
```

2. 如果流是有效的，即流未遇到错误，那么检测成功。

3. 当遇到**文件结束符**，或遇到一个无效的输入时(例如读入一个和**value的类型**不同的对象), 
istream对象的状态变为无效。处理无效的状态的istream对象会使条件变为假。

4. 文件结束符
	1. windows 系统的文件结束符是**Ctrl+Z**，然后按Enter或Return键。
	2. unix系统和Max OS是**Ctrl+D**
    
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
﻿## 新类型
> long long int

1. long long int 并不是 C++11 最先引入的，其实早在 C99，long long int 就已经被纳入 C 标准中，
	所以大部分的编译器早已支持。C++11 的工作则是正式把它纳入标准库，
	规定了一个 long long int 类型至少具备 64 位的比特数。

### noexcept 的修饰和操作
1. C++ 相比于 C 的一大优势就在于 C++ 本身就定义了一套完整的异常处理机制。
	然而在 C++11 之前，几乎没有人去使用在函数名后书写异常声明表达式，
	从 C++11 开始，这套机制被弃用，所以我们不去讨论也不去介绍以前这套机制是如何工作如何使用，你更不应该主动去了解它。

2. C++11 将异常的声明简化为以下两种情况：

    - 函数可能抛出任何异常
    - 函数不能抛出任何异常

	1. 并使用 noexcept 对这两种行为进行限制，例如：

			void may_throw();           // 可能抛出异常
			void no_throw() noexcept;   // 不可能抛出异常

	2. 使用 noexcept 修饰过的函数如果抛出异常，**编译器会使用 std::terminate() 来立即终止程序运行**。

	3. noexcept 还能用作操作符，用于操作一个表达式，当表达式无异常时，返回 true，否则返回 false。

			#include <iostream>

			void may_throw() {
				throw true;
			}
			auto non_block_throw = []{
				may_throw();
			};
			void no_throw() noexcept {
				return;
			}

			auto block_throw = []() noexcept {
				no_throw();
			};

			int main()
			{
				std::cout << std::boolalpha
				<< "may_throw() noexcept? " << noexcept(may_throw()) << std::endl
				<< "no_throw() noexcept? " << noexcept(no_throw()) << std::endl
				<< "lmay_throw() noexcept? " << noexcept(non_block_throw()) << std::endl
				<< "lno_throw() noexcept? " << noexcept(block_throw()) << std::endl;

				return 0;
			}

3. noexcept 修饰完一个函数之后能够起到封锁异常扩散的功效，如果内部产生异常，外部也不会触发。例如：

		try {
			may_throw();
		} catch (...) {
			std::cout << "捕获异常, 来自 my_throw()" << std::endl;
		}

		try {
			non_block_throw();
		} catch (...) {
			std::cout << "捕获异常, 来自 non_block_throw()" << std::endl;
		}

		try {
			block_throw();
		} catch (...) {
			std::cout << "捕获异常, 来自 block_throw()" << std::endl;
		}

	最终输出为：

		捕获异常, 来自 my_throw()
		捕获异常, 来自 non_block_throw()

### 总结
1. noexcept 是最为重要的特性，它的一个功能在于能够阻止异常的扩散传播，
让编译器能最大限度的优化我们的代码。
﻿## 函数对象包装器
1. std::function
	1. Lambda 表达式的本质是一个函数对象，当 Lambda 表达式的捕获列表为空时，
		Lambda 表达式还能够作为一个函数指针进行传递，例如：

			#include <iostream>

			using foo = void(int);  // 定义函数指针, using 的使用见上一节中的别名语法
			void functional(foo f) {
				f(1);
			}

			int main() {
				auto f = [](int value) {
					std::cout << value << std::endl;
				};
				functional(f);  // 函数指针调用
				f(1);           // lambda 表达式调用
				return 0;
			}

	2. 上面的代码给出了两种不同的调用形式，一种是将 Lambda 作为函数指针传递进行调用，而另一种则是直接调用 Lambda 表达式，
	在 C++11 中，统一了这些概念，将能够被调用的对象的类型，统一称之为可调用类型。
	而这种类型，便是通过 std::function 引入的。

	3. C++11 std::function 是一种通用、多态的函数封装，**它的实例可以对任何可以调用的目标实体进行存储、复制和调用操作**，
	它也是对 C++中现有的可调用实体的一种类型安全的包裹（相对来说，函数指针的调用不是类型安全的），
	换句话说，就是函数的容器。当我们有了函数的容器之后便能够更加方便的将函数、函数指针作为对象进行处理。例如：

			#include <functional>
			#include <iostream>

			int foo(int para) {
				return para;
			}

			int main() {
				// std::function 包装了一个返回值为 int, 参数为 int 的函数
				std::function<int(int)> func = foo;

				int important = 10;
				std::function<int(int)> func2 = [&](int value) -> int {
					return 1+value+important;
				};
				std::cout << func(10) << std::endl;
				std::cout << func2(10) << std::endl;
			}

2. std::bind/std::placeholder

	1. std::bind									
	> 是用来绑定函数调用的参数的，它解决的需求是我们有时候可能并不
	> 一定能够一次性获得调用某个函数的全部参数，通过这个函数，我们可以将部分
	> 调用参数提前绑定到函数身上成为一个新的对象，然后在参数齐全后，完成调用。例如：

			int foo(int a, int b, int c) {
				;
			}
			int main() {
				// 将参数1,2绑定到函数 foo 上，但是使用 std::placeholders::_1 来对第一个参数进行占位
				auto bindFoo = std::bind(foo, std::placeholders::_1, 1,2);
				// 这时调用 bindFoo 时，只需要提供第一个参数即可
				bindFoo(1);
			}

    2. 提示：注意 auto 关键字的妙用。有时候我们可能不太熟悉一个函数的返回值类型，但是我们却可以通过 auto 的使用来规避这一问题的出现。

## 匿名函数
1. [=]以=caputure值，表明：在lambda被声明时已有效的所有符号都以by value形式传进lambda体内。
﻿### 基础知识
1. 基本类型：
   int、short、long、float、double、unsigned、boolean、char、long double

2. const
	把对象转换为一个常量，定义后不能修改，定义时必须初始化(只读内存),是文件局部变量，
全局变量加上external。
  
3. 声明和定义：
	声明：声明类型和名字。使用external。不分配空间

	定义：1.分配空间 2.指定初始值。
  
3. 初始化必须分配空间，所以有初始化的声明其实也分配了空间，也就是定义。
http://www.cnblogs.com/yc_sunniwell/archive/2010/07/14/1777416.html

4. reference：绑定一个别名，不能绑定到另外的对象--> 映射内存位置不能变，必须初始化
（它指向内存中存在的一个变量），不能没有初始化，bss端但不能自己申请空间。

5. 枚举：初始化枚举必须是常量表达式，枚举值本身就是常量表达式

6. 类类型：
	类类型默认是private 权限，stuct默认是public，其他两者等价
  
7. 字面值常量：text段，池化技术。和String类型不同 

8. c++ 纯虚函数，在函数方发声明的最后加上 “= 0” 
	virtual void clear() = 0;
﻿## 面向对象增强
1. 委托构造
> C++11 引入了委托构造的概念，这使得构造函数可以在同一个类中一个构造函数调用另一个构造函数，从而达到简化代码的目的：

		class Base {
		public:
			int value1;
			int value2;
			Base() {
				value1 = 1;
			}
			Base(int value) : Base() {  // 委托 Base() 构造函数
				value2 = 2;
			}
		};

		int main() {
			Base b(2);
			std::cout << b.value1 << std::endl;
			std::cout << b.value2 << std::endl;
		}

2. 继承构造
> 在传统 C++ 中，构造函数如果需要继承是需要将参数一一传递的，这将导致效率低下。C++11 利用关键字 using 引入了继承构造函数的概念：

		class Base {
		public:
			int value1;
			int value2;
			Base() {
				value1 = 1;
			}
			Base(int value) : Base() {                          // 委托 Base() 构造函数
				value2 = 2;
			}
		};
		class Subclass : public Base {
		public:
			using Base::Base;  // 继承构造
		};
		int main() {
			Subclass s(3);
			std::cout << s.value1 << std::endl;
			std::cout << s.value2 << std::endl;
		}

3. 显式虚函数重载
> 在传统 C++中，经常容易发生意外重载虚函数的事情。例如：

		struct Base {
			virtual void foo();
		};
		struct SubClass: Base {
			void foo();
		};

	1. SubClass::foo 可能并不是程序员尝试重载虚函数，只是恰好加入了一个具有相同名字的函数。
		另一个可能的情形是，当基类的虚函数被删除后，子类拥有旧的函数就不再重载该虚拟函数并摇身
		一变成为了一个普通的类方法，这将造成灾难性的后果。

	2. C++11 引入了 override 和 final 这两个关键字来防止上述情形的发生。

	3. override
	> 当重载虚函数时，引入 override 关键字将显式的告知编译器进行重载，
	> 编译器将检查基函数是否存在这样的虚函数，否则将无法通过编译：

			struct Base {
				virtual void foo(int);
			};
			struct SubClass: Base {
				virtual void foo(int) override; // 合法
				virtual void foo(float) override; // 非法, 父类没有此虚函数
			};

	4. final
	> final 则是为了防止类被继续继承以及终止虚函数继续重载引入的。

			struct Base {
					virtual void foo() final;
			};
			struct SubClass1 final: Base {
			};                  // 合法

			struct SubClass2 : SubClass1 {
			};                  // 非法, SubClass 已 final

			struct SubClass3: Base {
					void foo(); // 非法, foo 已 final
			};

4. 显式禁用默认函数
	1. 在传统 C++ 中，如果程序员没有提供，编译器会默认为对象生成默认构造函数、复制构造、赋值算符以及析构函数。
		另外，C++ 也为所有类定义了诸如 new delete 这样的运算符。当程序员有需要时，可以重载这部分函数。

	2. 这就引发了一些需求：无法精确控制默认函数的生成行为。
		例如禁止类的拷贝时，必须将赋值构造函数与赋值算符声明为 private。
		尝试使用这些未定义的函数将导致编译或链接错误，则是一种非常不优雅的方式。
		并且，编译器产生的默认构造函数与用户定义的构造函数无法同时存在。
		若用户定义了任何构造函数，编译器将不再生成默认构造函数，但有时候我们却希望同时拥有这两种构造函数，这就造成了尴尬。

	3. C++11 提供了上述需求的解决方案，允许显式的声明采用或拒绝编译器自带的函数。例如：

			class Magic {
			public:
				Magic() = default;  // 显式声明使用编译器生成的构造
				Magic& operator=(const Magic&) = delete; // 显式声明拒绝编译器生成构造
				Magic(int magic_number);
			}

## 强类型枚举
1.在传统 C++中，枚举类型并非类型安全，枚举类型会被视作整数，则会让两种完全不同的枚举类型可以进行直接的比较
	（虽然编译器给出了检查，但并非所有），甚至枚举类型的枚举值名字不能相同，这不是我们希望看到的结果。

2. C++11 引入了枚举类（enumaration class），并使用 enum class 的语法进行声明：

		enum class new_enum : unsigned int {
			value1,
			value2,
			value3 = 100,
			value4 = 100
		};

	1. 这样定义的枚举实现了类型安全，
		- 首先他不能够被隐式的转换为整数，
		- 同时也不能够将其与整数数字进行比较，
		- 更不可能对不同的枚举类型的枚举值进行比较。
		- 但相同枚举值之间如果指定的值相同，那么可以进行比较：

				if (new_enum::value3 == new_enum::value4) {
					// 会输出
					std::cout << "new_enum::value3 == new_enum::value4" << std::endl;
				}

	2. 在这个语法中，枚举类型后面使用了冒号及类型关键字来指定枚举中枚举值的类型，这使得我们能够为枚举赋值（未指定时将默认使用 int）。
	3. 而我们希望获得枚举值的值时，将必须显式的进行类型转换，不过我们可以通过重载 << 这个算符来进行输出，可以收藏下面这个代码段：

			#include <iostream>
			template<typename T>
			std::ostream& operator<<(typename std::enable_if<std::is_enum<T>::value, std::ostream>::type& stream, const T& e)
			{
				return stream << static_cast<typename std::underlying_type<T>::type>(e);
			}

			这时，下面的代码将能够被编译：

			std::cout << new_enum::value3 << std::endl
﻿## 模板与泛型编程
> c++primer 5 16：

1. 所谓泛型编程就是以独立与任何类型的方式编写代码。使用泛型程序时，
我们需要提供具体程序实例所操作的类型或值。

2. 泛型编程与面向对象编程一样，都依赖于某种形式的<font color=green>多态性</font>

3. 模板是泛型编程的基础，模板是创建类的蓝图或公式。

### 模板定义
1. 定义：
	1. 定义函数模板
		> 模板定义以关键字 templte开始，后接**模板形参列表**(template paramter list),
		**模板**形参列表式用尖括号括住的一个或多个**模板形参**的列表，形参之间以逗号分隔。

		> <font color=green>模板形参表不能为空</font>
		> <font color=green>但是模板特化的时候,模板形参列表可以为空,但函数名后要加上尖括号,
		并在尖括号中写入特化的类型,如:template<> int add<int, float>(){...}</font>

		1. 模板形参表
			> 模板形参可以是表示类型的**类型形参**,也可以是表示常量表达式的**非类型形参**.

			1. **非类型形参**跟在类型说明符之后声明
			2. **类型形参**跟在关键字class或typename之后定义。class和typename在这里没有区别。

		2. 使用函数模板
			> 使用函数模板时，编译器推断哪些模板实参绑定到模板形参，这就是实例化

			1. 编译器将确定用什么类型代替每个类型形参，以及非类型形参。编译器使用实参代替相应的模板形参产生并并编译该版本的函数。
			
					int main()
					{
						//T is int; compiler instaniates int compare(const int&, const int&)
						cout << compare(1, 0) << endl;
						//T is string; compiler instaniates int compare(const string&, const string&)
						string s1 = "hi", s2 = "world";
						cout << compare(s1, s2) << endl;
					}

		3. inline:
			> 说明符放在模板形参表之后，返回类型之前，不能放在template之前

					template <typename T> inline T min(const T&, const T&);

	2. 定义类模板
		> 类模板也是模板，以以关键字template开头，后接模板形参表。

				template <class T> class Queue {
				pubic:
					Queue();
					T &front();
					const T &fonst() const;
				privte:
					//...
				};

		1. 使用类模板
			
				Queue<int> qi;
				Queue<vector<double>> qc;
				Queue<string> qs;

			1. 编译器使用实参来实例化这个类的特定类型版本。
			2. 实际上编译器用用户提供的实际特定类型代替类T，重新编写了Queue类。这里编译器实例化类三个类。

		2. 模板可以只声明不定义：
			
				template <class T> int compare(const T&, const T&);
2. 模板形参
	> 模板形参的唯一含义是区分形参是类型形参还是非类型形参。
	> <font color=green>每个模板类型形参前面都必须带上关键字class或typename，每个非类型形参前面必须带上类型名字，省略关键字或类型说明符都是错误的</font>

	1. 模板类型形参
		1. 在模板定义内部指定类型
			
				template <class Param, class U>
				Param fcn(Param* array, U value){
					typename Param::size_type *p;
				}

			1. 之所以在param前面加上typename，是因为不知道size_type是Param的数据成员还是类型成员。
				而默认情况下，编译器假定这样的名字指定数据成员，而不是类型.

			2. 所以在成员名之前加上typename作为前缀，可以告诉编译器将成员当作类型
				这样也导致了实例化fcn的类型必须具有size_type的成员。

			3. 在类型前加typename没有害处，即使是不必要的也没有关系，所以拿不定的时候就加上。

					template <class Param, class U>
					Param fcn(Param* arry, U value)
					{
						Param::size_type *p;//这里不指导size_type 到底是类型名还是对象名，可以有多重定义
					}


					如果希望编译器将size_type当作类型，则必须显示告诉编译器这样做：
					template <class Param, class U>
					Param fnc(Param* array, U value)
					{
						typename Param::size_type *p;
					}

	2. 模板非类型形参
		> 在调用函数时非类型形参将用值代替，值得类型在模板形参表中指定

		1. 模板非类型形参值式模板定义内部的常量值，再需要常量表达式的时候，可使用非类型形参指定数组长度（应用场景）

				template <class T, size_t N> void array_init(T (&param)[N])
				{ 
					for (size_t i = 0; i != N; i++)
					  param[i] = 0;
				}

		2. 当调用array_init实例化具体类型时，编译器会从数组实参计算非类型形参的值：

				int x[42];
				double y[10];
				array_init(x);
				array_init(y);

			1. 编译器将为array_init调用中用到的每种数组实例化一个array_init版本。
				</font color = red>注意编译器实例化函数版本时没有调用尖括号来确定具体类型，
				而是通过调用实参来确定所有的模板形参。</font>
		3. <font color=red>模板非类型形参最常用与确定数组长度，注意模板中的形参也是形参，所有数组形参在函数中都是一个指针。
				所以如果实例化模板时传进来的是一个数组名或者指针，都会退化成为一个指针，编译器推断不出来数组长度，会报错。
				只有模板形参那里是一个引用，实例化时才会将实参引用传递过去，才可以推断处数组长度。</font>

### 实例化
1. 概述
	> 模板是一个蓝图，它本身不是类或函数，编译器用木板产生指定的类或函数特定类型版本。产生模板的特定类实例的过程称为</font color=green>实例化</font>
	> </font color=red>模板在使用时将进行实例化，类模板在引用实际模板类类型时实例化，函数模板在调用它或用它对函数指针进行初始化或者赋值时实例化。</font>

	1. 类的实例化
		1. 类模板的每次实例化都会产生一个独立的类类型。为int类型实例化的Queue与任意其他Queue类型没有关系，对其他Queue类型的成员也没有特殊访问权限。

	2. 类模板形参是必须的
		1. </font color=red>想要使用类模板，就必须显示指定模板实参： Queue<int>.Queue 不是类型，Queue<int>才是类型</font>

	3. 函数模板实例化
		1. 使用函数模板时，编译器通常会为我们推断模板实参

2. 模板实参推断
	1. 多个类型形参的实参必须完全匹配
	2. 类型实参的受限转换
		1. 不会转换实参以匹配已有的实例化，相反，会产生新的实例。只有两种情况会发生转换
		2. 两种转换
			1. const转换
			2. 数组或函数到指针的转换
	
	3. 应用于非模板实参的常规转换		
		1. </font color=green>类型转换的限制只适用于类型为模板形参的那些实参，如果形参不是模板类型，可以自动转换</font>

				template <class Type> Type sum(const Type &op1, int op2){
				
					return op1 + op2;
				}

				double d = 3.14;
				sum(1024, d); ok

	4. 模板实参推断与函数指针
	> 可以使用函数模板对函数指针进行初始化或赋值，
	> 这样做的时候，编译器使用指针的类型实例化具有适当模板实参的模板版本。	
	
		1. 例子
				
				template <typename T> int compare(color T&, const T&);
				int (*pf1) (const int&, const int&) = compare;

		1. 获取函数模板实例化的地址的时候，上下文必须是这样的：它允许为每个模板形参确定唯一的类型或值
		2. 如果不能从函数指针确定模板实参，就会出错。
			1. 例子：重载两个同名函数，不能唯一确定模板实参，实例化时会失败

3. 函数模板的显式实参
> 某些情况，不能推断模板实参，最常出现在:函数的返回类型必须和形参表中所用的所有类型都不想同。
> 这种情况下，有必要覆盖模板实参推断机制，并显式指定**为模板形参所用的类型或值**

	1. 指定显示模板实参
		1. template <typename T, typename U> ??? sum(T, U);   返回结果肯定是T和U里范围最大的类型，int、long……
			但是不能确定返回类型到底式T还是U。

		2. 解决办法： 强制sum调用者将较小的类型强制转换为希望作为结果使用的类型：

				int (static_cast<int>(s), i); //这样不管返回类型是T还是U都正确

		3. <font color=gray>调用时，调整调用实参类型，使得返回类型可以预测**</font>

	2. 在返回类型中使用类型形参
		1. 指定返回类型的一种方式是引入第三个模板形参，它必须由调用者显示指定：
			
				//T1 cannot be deduced:it doesn't appear in the funccion paramer list
				template <typename T1, typename T2, typename T3> 
				T1 sum(T2, T3);

		2. 问题：没有实参可用来推导形参的类型，所以调用者必须在每次调用sum时为该形参显式提供实参。
		3. </font color=red>为调用者提供显示实参和定义模板类相似：在以逗号分隔、用尖括号括住的列表中指定显式模板实参</font>

				//T1 explicitly specified;T2 and T3 inferred from arguent types
				long val3 = sum<long>(i, lng);

			1. </font color=red>注意：显示模板实参从左至右与对应模板形参相匹配，第一个模板实参与第一个模板形参匹配，以此类推。</font>
				
					template <class T1, class T2, class T3>
					T3 alternative_sum(T2, T1);
				
			2. </font color=red>则必须为三个所有形参指定实参</font>
				
					long val3 = alternative_sum<long, int, long>(i, lng);

					编译器只会对模板形参一个一个对照，如果尖括号里实参数目和模板形参不一样，则后面几个模板形参根据函数调用实参确定。

	3. 显式实参与函数模板的指针
		1. 有二义性的程序，通过使用显式模板实参能够消除二义性：

				template <typename T> int compare(const T&, const T&);
				void func(int(*) (const string&, const string&);
				void func(int(*) (const int&, const int&);
				func(compare<int>);

### 模板编译模型
1. 产生实例的阶段：
	1. 编译器看到模板定义不会立即产生代码，只有看到使用模板时，如调用函数模板或者类模板的对象时候，编译器才会产生模板实例。

2. 普通函数定义和类方法定义在源文件中。但模板实例化时需要提前知道模板函数定义。  

3. 两种编译模型
> 所有编译器都支持第一种包含模型，少部分编译器支持分别编译模型，这里需要查看编译器指南
	1. 包含编译模型
	> 在这种编译模型中，编译器必须能看到用到的所有模板的定义
		
		1. 一般而言，可以在声明函数模板或类模板的头文件中加一条#include，添加定义了相关模板的源文件，等于说是在声明的头文件中#include 定义的源文件。

		2. 这样可以保持头文件和实现文件的分离，但是需要编译器在编译使用的模板时必须能看到两种文件。
		3. 可能在不同的源文件中使用同一个模板却生成多个实例，编译器优化查看文档。

	2. 分别编译模型
	> 编译器会为我们跟踪相关的模板定义，但是必须让编译器知道要记住给定的模板定义，可以使用**export 关键字**来做这件事。
		1. 头文件一般不包含变量、结构和类对象定义，因为这样可能导致重复定义的编译错误，
			解决办法是，在某个代码文件中进行定义，在其他用户代码文件用extern来引用它们。

		2. 对模板类型，可以在头文件中声明模板类和模板函数;					
			在代码源文件中，使用extern来定义具体的模板类和模板函数; 
			然后在其他用户代码文件中，包含声明头文件后，就可以使用这些对象和函数了。

		3. export 不必在模板声明中出现

		4. 定义
			1. 在函数模板定义中指明函数模板为导出，通过在template前面包含export

					export template <typename Type>
					Type sum(Type t1, Type t2);

				1. 这个函数模板的申明像通常一样应放在头文件中，声明不必指定export

			2. 类模板使用export
				1. 头文件中的类定义体不应该使用关键字export，如果在头文件中使用了export，
					则该文件只能被程序中的一个源文件使用

				2. 相反，应该在类的实现文件中使用export。

			2. 

### 类模板成员
1. 类模板成员函数
2. 非类型形参的模板实参
3. 类模型中的友元声明
4. Queue和QueueItem的友元声明
5. 成员模板
6. 完整的Queue类
7. 类模板的static 成员

### 一个泛型句柄类
1. 定义句柄类
2. 使用句柄类

### 模板特化
1. 函数模板的特化
2. 类模板的特化
3. 特化成员而不特化类
4. 类模板的部分特化

### 重载与函数模板
﻿## 多线程
### C++ 11/14 高速上手教程 - 语言级线程支持
1. 本节内容包括：
    1. 对标准库的扩充: 语言级线程支持
        - std::thread
        - std::mutex/std::unique_lock
        - std::future/std::packaged_task
        - std::condition_variable

    2. 提示：本节代码编译需要使用 -pthread 选项，例如：

			g++ main.cpp -std=c++14 -pthread

### std::thread
> 用于创建一个执行的线程实例，所以它是一切并发编程的基础，使用时需要包含<thread>头文件，
> 它提供了很多基本的线程操作，例如get_id()来获取所创建线程的线程 ID，例如使用 join() 来加入一个线程等等，例如：

		#include <iostream>
		#include <thread>
		void foo() {
			std::cout << "hello world" << std::endl;
		}
		int main() {
			std::thread t(foo);
			t.join();
			return 0;
		}

### std::mutex, std::unique_lock
> C++11引入了 mutex 相关的类，其所有相关的函数都放在 <mutex> 头文件中。

1. std::mutex 是 C++11 中最基本的 mutex 类，通过实例化 std::mutex 可以创建互斥量，
	而通过其成员函数 lock() 可以仅此能上锁，
	unlock() 可以进行解锁。

2. 但是在在实际编写代码的过程中，最好不去直接调用成员函数，
	因为调用成员函数就需要在每个临界区的出口处调用 unlock()，当然，还包括异常。
	
3. 这时候 C++11 还为互斥量提供了一个 RAII 语法的模板类std::lock_gurad。
	RAII 在不失代码简洁性的同时，很好的保证了代码的异常安全性。

4. 在 RAII 用法下，对于临界区的互斥量的创建只需要在作用域的开始部分，例如：

		void some_operation(const std::string &message) {
			static std::mutex mutex;
			std::lock_guard<std::mutex> lock(mutex);

			// ...操作

			// 当离开这个作用域的时候，互斥锁会被析构，同时unlock互斥锁
			// 因此这个函数内部的可以认为是临界区
		}

	由于 C++保证了所有栈对象在声明周期结束时会被销毁，所以这样的代码也是异常安全的。
	无论 some_operation() 正常返回、还是在中途抛出异常，都会引发堆栈回退，也就自动调用了 unlock()。

5. 而 std::unique_lock 则相对于 std::lock_guard 出现的，std::unique_lock 更加灵活，
	std::unique_lock 的对象会以独占所有权
	（没有其他的 unique_lock 对象同时拥有某个 mutex 对象的所有权）的方式管理 mutex 对象上的上锁和解锁的操作。
	所以在并发编程中，推荐使用 std::unique_lock。例如：

		#include <iostream>
		#include <thread>
		#include <mutex>

		std::mutex mtx;

		void block_area() {
			std::unique_lock<std::mutex> lock(mtx);
			//...临界区
		}
		int main() {
			std::thread thd1(block_area);

			thd1.join();

			return 0;
		}

### std::future, std::packaged_task
> std::future 则是提供了一个访问异步操作结果的途径，这句话很不好理解。

1. 而 C++11 提供的 std::future 可以用来获取异步任务的结果。我们很容易能够想象到把它作为一种简单的线程同步手段。

2. 此外，std::packaged_task 可以用来封装任何可以调用的目标，从而用于实现异步的调用。例如：

		#include <iostream>
		#include <future>
		#include <thread>

		int main() {
			// 将一个返回值为7的 lambda 表达式封装到 task 中
			// std::packaged_task 的模板参数为要封装函数的类型
			std::packaged_task<int()> task([](){return 7;});
			// 获得 task 的 future
			std::future<int> result = task.get_future();    // 在一个线程中执行 task
			std::thread(std::move(task)).detach();    
			std::cout << "Waiting...";
			result.wait();
			// 输出执行结果
			std::cout << "Done!" << std:: endl << "Result is " << result.get() << '\n';
		}

	1. 在封装好要调用的目标后，可以使用 get_future() 来获得一个 std::future 对象，以便之后事实线程同步。

### std::condition_variable
1. std::condition_variable 是为了解决死锁而生的。
	- 当互斥操作不够用而引入的。
	- 比如，线程可能需要等待某个条件为真才能继续执行，而一个忙等待循环中可能会导致所有其他线程都无
		法进入临界区使得条件为真时，就会发生死锁。
	- 所以，condition_variable 实例被创建出现主要就是用于唤醒等待线程从而避免死锁。
	- std::condition_variable的 notify_one() 用于唤醒一个线程；
	- notify_all() 则是通知所有线程。
	
2. 下面是一个生产者和消费者模型的例子：

		#include <condition_variable>
		#include <mutex>
		#include <thread>
		#include <iostream>
		#include <queue>
		#include <chrono>

		int main()
		{
			// 生产者数量
			std::queue<int> produced_nums;
			// 互斥锁
			std::mutex m;
			// 条件变量
			std::condition_variable cond_var;
			// 结束标志
			bool done = false;
			// 通知标志
			bool notified = false;

			// 生产者线程
			std::thread producer([&]() {
				for (int i = 0; i < 5; ++i) {
					std::this_thread::sleep_for(std::chrono::seconds(1));
					// 创建互斥锁
					std::unique_lock<std::mutex> lock(m);
					std::cout << "producing " << i << '\n';
					produced_nums.push(i);
					notified = true;
					// 通知一个线程
					cond_var.notify_one();
				}   
				done = true;
				cond_var.notify_one();
			}); 

			// 消费者线程
			std::thread consumer([&]() {
				std::unique_lock<std::mutex> lock(m);
				while (!done) {
					while (!notified) {  // 循环避免虚假唤醒
						cond_var.wait(lock);
					}   
					while (!produced_nums.empty()) {
						std::cout << "consuming " << produced_nums.front() << '\n';
						produced_nums.pop();
					}   
					notified = false;
				}   
			}); 

			producer.join();
			consumer.join();
		}
﻿## 右值
### 右值引用
1. 右值引用是 C++11 引入的与 Lambda 表达式齐名的重要特性之一。它的引入解决了 C++ 中大量的历史遗留问题，消除了诸如 std::vector、std::string 之类的额外开销，也才使得函数对象容器 std::function 成为了可能。

2. 左值、右值的纯右值、将亡值、右值
	1. 左值(lvalue, left value)，顾名思义就是赋值符号左边的值。
		准确来说，左值是表达式（不一定是赋值表达式）后依然存在的持久对象。

	2. 右值(rvalue, right value)，右边的值，是指表达式结束后就不再存在的临时对象。

	3. 而 C++11 中为了引入强大的右值引用，将右值的概念进行了进一步的划分，分为：纯右值、将亡值。

		1. 纯右值(prvalue, pure rvalue)，纯粹的右值，
			- 要么是纯粹的字面量，例如 10, true；
			- 要么是求值结果相当于字面量或匿名临时对象，例如 1+2。
			- 非引用返回的临时变量、运算表达式产生的临时变量、原始字面量、Lambda 表达式都属于纯右值。
			- <font color=green>个人觉得那些只有值却没有变量名，无法再次由变量名确定的值都是右值</font>

		2. 将亡值(xvalue, expiring value)，是 C++11 为了引入右值引用而提出的概念
			（因此在传统 C++中，纯右值和右值是统一个概念），也就是即将被销毁、却能够被移动的值。

			1. 将亡值可能稍有些难以理解，我们来看这样的代码：

					std::vector<int> foo() {
						std::vector<int> temp = {1, 2, 3, 4};
						return temp;
					}

					std::vector<int> v = foo();

			2. 在这样的代码中，函数 foo 的返回值 temp 在内部创建然后被赋值给 v，
				然而 v 获得这个对象时，会将整个 temp 拷贝一份，然后把 temp 销毁，
				如果这个 temp 非常大，这将造成大量额外的开销（这也就是传统 C++ 一直被诟病的问题）。
				在最后一行中，v 是左值、foo() 返回的值就是右值（也是纯右值）。

			3. 但是，v 可以被别的变量捕获到，而 foo() 产生的那个返回值作为一个临时值，
				一旦被 v 复制后，将立即被销毁，无法获取、也不能修改。

			4. 将亡值就定义了这样一种行为：<font color=green>临时的值能够被识别、同时又能够被移动。</font>

3. 右值引用和左值引用
	> 需要拿到一个将亡值，就需要用到右值引用的申明：T &&，
	> 其中 T 是类型。右值引用的声明让这个临时值的生命周期得以延长、只要变量还活着，那么将亡值将继续存活。

	1. C++11 提供了 std::move 这个方法将左值参数无条件的转换为右值，有了它我们就能够方便的获得一个右值临时对象，例如：

			#include <iostream>
			#include <string>

			void reference(std::string& str) {
				std::cout << "左值" << std::endl;
			}
			void reference(std::string&& str) {
				std::cout << "右值" << std::endl;
			}

			int main()
			{
				std::string  lv1 = "string,";       // lv1 是一个左值
				// std::string&& r1 = s1;           // 非法, s1 在全局上下文中没有声明
				std::string&& rv1 = std::move(lv1); // 合法, std::move 可以将左值转移为右值
				std::cout << "rv1 = " << rv1 << std::endl;      // string,

				const std::string& lv2 = lv1 + lv1; // 合法, 常量左值引用能够延长临时变量的生命周期**
				// lv2 += "Test";                   // 非法, 引用的右值无法被修改
				std::cout << "lv2 = "<<lv2 << std::endl;      // string,string

				std::string&& rv2 = lv1 + lv2;      // 合法, 右值引用延长临时对象的生命周期
				rv2 += "string";                    // 合法, 非常量引用能够修改临时变量
				std::cout << "rv2 = " << rv2 << std::endl;      // string,string,string,

				reference(rv2);                     // 输出左值
			}

		1. 注意：
			1. rv2 虽然引用了一个右值，但由于它是一个引用，所以 rv2 依然是一个左值。
			2. 右值的引用必须是const 常量引用。
			3. 上面的lv1和lv2的值是错误的，它们都是空字符串。

		2. 移动构造
			1. std::move 函数何以非常简单的方式将左值引用转换为右值引用。
				**从实现上讲，std::move基本等同于一个类型转换：static_cast<T&&>(lvalue);**
			2. 通过std::move,可以避免不必要的拷贝
			3. std::move 是将对象的状态或者所有权动从一个对象转移到另一个对象，只是转移，没有内存的移动和拷贝。

		3. 用法
			1. C++ 标准库使用比如vector::push_back 等这类函数时,会对参数的对象进行复制,连数据也会复制.
				这就会造成对象内存的额外创建, 本来原意是想把参数push_back进去就行了,通过std::move，
				可以避免不必要的拷贝操作。
		    2. std::move是将对象的状态或者所有权从一个对象转移到另一个对象，只是转移，
				没有内存的搬迁或者内存拷贝所以可以提高利用效率,改善性能.。
			3. 对指针类型的标准库对象并不需要这么做.


		4. 说明：
			1. value = std::move(t) 用来表明对象t是可以moved from的，它允许高效的从t资源转换到value上。
			2. 标准库对象支持moved from 的左值在moved之后它的对象原值是有效的(可以正常析构)，但是是unspecified的，可以理解为空数据，但是这个对象的其他方法返回值不一定是0,比如size(),所以，moved from之后的对象最好还是不要使用了。
			3. 对本身进行move，并赋值给本身是undefined的行为。

4. 移动语义

> 传统的 C++ 没有区分『移动』和『拷贝』的概念，造成了大量的数据移动，浪费时间和空间。
> 右值引用的出现恰好就解决了这两个概念的混淆问题，例如：

		#include <iostream>

		class A {
		public:
			int *pointer;
			A() :pointer(new int(1)) { 
				std::cout << "构造" << pointer << std::endl; 
			}
			// 无意义的对象拷贝
			A(A& a) :pointer(new int(*a.pointer)) { 
				std::cout << "拷贝" << pointer << std::endl; 
			}    

			A(A&& a) :pointer(a.pointer) { 
				a.pointer = nullptr; 
				std::cout << "移动" << pointer << std::endl; 
			}

			~A() { 
				std::cout << "析构" << pointer << std::endl; 
				delete pointer; 
			}
		};
		// 防止编译器优化
		A return_rvalue(bool test) {
			A a,b;
			if(test) return a;
			else return b;
		}
		int main() {
			A obj = return_rvalue(false);
			std::cout << "obj:" << std::endl;
			std::cout << obj.pointer << std::endl;
			std::cout << *obj.pointer << std::endl;

			return 0;
		}

	1. 在上面的代码中：
		1. 首先会在 return_rvalue 内部构造两个 A 对象，于是获得两个构造函数的输出；
		2. 函数返回后，产生一个将亡值，被 A 的移动构造（A(A&&)）引用，从而延长生命周期，并将这个右值中的指针拿到，保存到了 obj 中
		3. 而将亡值的指针被设置为 nullptr，防止了这块内存区域被销毁。
		4. 从而避免了无意义的拷贝构造，加强了性能。
		
	2. 再来看看涉及标准库的例子：

			#include <iostream> // std::cout
			#include <utility>  // std::move
			#include <vector>   // std::vector
			#include <string>   // std::string

			int main() {

				std::string str = "Hello world.";
				std::vector<std::string> v;

				// 将使用 push_back(const T&), 即产生拷贝行为
				v.push_back(str);
				// 将输出 "str: Hello world."
				std::cout << "str: " << str << std::endl;

				// 将使用 push_back(const T&&), 不会出现拷贝行为
				// 而整个字符串会被移动到 vector 中，所以有时候 std::move 会用来减少拷贝出现的开销
				// 这步操作后, str 中的值会变为空
				v.push_back(std::move(str));
				// 将输出 "str: "
				std::cout << "str: " << str << std::endl;

				return 0;
			}

5. 完美转发 
> 前面我们提到了，一个声明的右值引用其实是一个左值。这就为我们进行参数转发（传递）造成了问题：

		void reference(int& v) {
			std::cout << "左值" << std::endl;
		}
		void reference(int&& v) {
			std::cout << "右值" << std::endl;
		}
		template <typename T>
		void pass(T&& v) {
			std::cout << "普通传参:";
			reference(v);   // 始终调用 reference(int& )
		}
		int main() {
			std::cout << "传递右值:" << std::endl;
			pass(1);        // 1是右值, 但输出左值

			std::cout << "传递左值:" << std::endl;    
			int v = 1;
			pass(v);        // v是左引用, 输出左值

			return 0;
		}

	1. 对于 pass(1) 来说，虽然传递的是右值，但由于 v 是一个引用，所以同时也是左值。
		因此 reference(v) 会调用 reference(int&)，输出『左值』。
		而对于pass(v)而言，v是一个左值，为什么会成功传递给 pass(T&&) 呢？

	2. 这是基于引用坍缩规则的：
		1. 在传统 C++ 中，我们不能够对一个引用类型继续进行引用
		2. 但 C++ 由于右值引用的出现而放宽了这一做法，从而产生了引用坍缩规则，
			允许我们对引用进行引用，既能左引用，又能右引用。但是却遵循如下规则：

				函数形参类型 	实参参数类型 	推导后函数形参类型
				T& 	左引用 	T&
				T& 	右引用 	T&
				T&& 	左引用 	T&
				T&& 	右引用 	T&&

		3. 因此，模板函数中使用 T&& 不一定能进行右值引用，当传入左值时，此函数的引用将被推导为左值。
			更准确的讲，**无论模板参数是什么类型的引用，当且仅当实参类型为右引用时，模板参数才能被推导为右引用类型**
			这才使得 v 作为左值的成功传递。

		4. 完美转发就是基于上述规律产生的。
		> 所谓完美转发，就是为了让我们在传递参数的时候，保持原来的参数类型（左引用保持左引用，右引用保持右引用）。
		
			1. 为了解决这个问题，我们应该使用 std::forward 来进行参数的转发（传递）：

					#include <iostream>
					#include <utility>

					void reference(int& v) {
						std::cout << "左值引用" << std::endl;
					}

					void reference(int&& v) {
						std::cout << "右值引用" << std::endl;
					}

					template <typename T>
					void pass(T&& v) {
						std::cout << "普通传参:";
						reference(v);
						std::cout << "std::move 传参:";
						reference(std::move(v));
						std::cout << "std::forward 传参:";
						reference(std::forward<T>(v));
					}

					int main() {
						std::cout << "传递右值:" << std::endl;
						pass(1);

						std::cout << "传递左值:" << std::endl;
						int v = 1;
						pass(v);

						return 0;
					}

		2. 输出结果为：

				传递右值:
				普通传参:左值引用
				std::move 传参:右值引用
				std::forward 传参:右值引用
				传递左值:
				普通传参:左值引用
				std::move 传参:右值引用
				std::forward 传参:左值引用

		3. 无论传递参数为左值还是右值，普通传参都会将参数作为左值进行转发，所以 std::move 总会接受到一个左值，
			从而转发调用了reference(int&&) 输出右值引用。

		4. 唯独 std::forward 即没有造成任何多余的拷贝，同时完美转发(传递)了函数的实参给了内部调用的其他函数。

		5. std::forward 和 std::move 一样，没有做任何事情，
			- std::move 单纯的将左值转化为右值
			- std::forward 也只是单纯的将参数做了一个类型的转换
			- 从实现来看，std::forward<T>(v) 和 static_cast<T&&>(v) 是完全一样的。

### <font color=green>总结</font>
1. 右值虽然在内存中，但是没有直接的变量直接指向它，所以一般变量的规则不适用它。例如，右值引用
2. 右值引用用在变量马上要清除(将亡值)时，仅仅去掉了变量名(在可执行文件中将名字符号里去掉)，
	但是变量名指向的存储空间没有消除，而是被右值引用所指向。
3. 函数返回的结果return，正常情况下先是将return 结果复制给一个函数体外的临时变量，然后临时变量在复制给调用函数的地方
	这里就进行了两次复制。只不过编译器会优化，缩减拷贝次数。
	函数返回肯定要调用一次复制的，因为需要将函数栈空间的变量复制出去，栈空间是要回收的。
	这个临时变量是右值引用。右值引用只是一个特殊类型而已。

4. move 仅仅是一个static_cast,做的就是将变量去掉，只要它存储的值。看 advanced/函数返回值优化..md.
5. 如果一个值是右值(没有变量名),就会自动调用右值引用或移动构造函数，记得**右值引用是一种类型，可以适配多态函数**
﻿## 正则表达式库
1. 对标准库的扩充: 正则表达式库
	1. 正则表达式简介
		- 普通字符
		- 特殊字符
		- 限定符

	2. std::regex 及其相关
		- std::regex
		- std::regex_match
		- std::match_results

### 正则表达式简介
> 正则表达式不是 C++ 语言的一部分，这里仅做简单的介绍。

1. 正则表达式描述了一种字符串匹配的模式。一般使用正则表达式主要是实现下面三个需求：

    - 检查一个串是否包含某种形式的子串；
    - 将匹配的子串替换；
    - 从某个串中取出符合条件的子串。

2. 正则表达式是由普通字符（例如 a 到 z）以及特殊字符组成的文字模式。
	- 模式描述在搜索文本时要匹配的一个或多个字符串。
	- 正则表达式作为一个模板，将某个字符模式与所搜索的字符串进行匹配。
	
3. 普通字符
	1. 普通字符包括没有显式指定为元字符的所有可打印和不可打印字符。
		- 这包括所有大写和小写字母、所有数字、所有标点符号和一些其他符号。
		
4. 特殊字符
> 特殊字符是正则表达式里有特殊含义的字符，也是正则表达式的核心匹配语法。

	1. 参见下表：
		特别字符 	描述
		- $ 	匹配输入字符串的结尾位置。
		- (,) 	标记一个子表达式的开始和结束位置。子表达式可以获取供以后使用。
		- * 	匹配前面的子表达式零次或多次。
		- + 	匹配前面的子表达式一次或多次。
		- . 	匹配除换行符 \n 之外的任何单字符。
		- [ 	标记一个中括号表达式的开始。
		- ? 	匹配前面的子表达式零次或一次，或指明一个非贪婪限定符。
		- \ 	将下一个字符标记为或特殊字符、或原义字符、或向后引用、或八进制转义符。例如， n 匹配字符 n。\n 匹配换行符。序列 \\ 匹配 '\' 字符，而 \( 则匹配 '(' 字符。
		- ^ 	匹配输入字符串的开始位置，除非在方括号表达式中使用，此时它表示不接受该字符集合。
		- { 	标记限定符表达式的开始。
		- | 	指明两项之间的一个选择。
		
5. 限定符
> 限定符用来指定正则表达式的一个给定的组件必须要出现多少次才能满足匹配。

1. 见下表：
	字符 	描述
	- * 	匹配前面的子表达式零次或多次。例如，foo* 能匹配 fo 以及 foooo。* 等价于{0,}。
	- + 	匹配前面的子表达式一次或多次。例如，foo+ 能匹配 foo 以及 foooo，但不能匹配 fo。+ 等价于 {1,}。
	- ? 	匹配前面的子表达式零次或一次。例如，Your(s)? 可以匹配 Your 或 Yours 中的Your 。? 等价于 {0,1}。
	- {n} 	n 是一个非负整数。匹配确定的 n 次。例如，f{2} 不能匹配 for 中的 o，但是能匹配 foo 中的两个 o。
	- {n,} 	n 是一个非负整数。至少匹配 n 次。例如，f{2,} 不能匹配 for 中的 o，但能匹配 foooooo 中的所有 o。o{1,} 等价于 o+。o{0,} 则等价于 o*。
	- {n,m} 	m 和 n 均为非负整数，其中 n 小于等于 m。最少匹配 n 次且最多匹配 m 次。例如，o{1,3} 将匹配 foooooo 中的前三个 o。o{0,1} 等价于 o?。注意，在逗号和两个数之间不能有空格。

### std::regex 及其相关
1. 一般的解决方案就是使用 boost 的正则表达式库。
2. 而 C++11 正式将正则表达式的的处理方法纳入标准库的行列，从语言级上提供了标准的支持，不再依赖第三方。
3. C++11 提供的正则表达式库操作 std::string 对象
	- 模式 std::regex (本质是 std::basic_regex)进行初始化
	- 通过 std::regex_match 进行匹配
	- 从而产生 std::smatch （本质是 std::match_results 对象）。

4. 我们通过一个简单的例子来简单介绍这个库的使用。考虑下面的正则表达式

	- [a-z]+\.txt: 在这个正则表达式中, [a-z] 表示匹配一个小写字母, + 可以使前面的表达式匹配多次，因此 [a-z]+ 能够匹配一个小写字母组成的字符串。在正则表达式中一个 . 表示匹配任意字符，而 \. 则表示匹配字符 .，最后的 txt 表示严格匹配 txt 则三个字母。因此这个正则表达式的所要匹配的内容就是由纯小写字母组成的文本文件。

### std::regex_match 
1. 用于匹配字符串和正则表达式，有很多不同的重载形式。
	- 最简单的一个形式就是传入std::string 以及一个td::regex 进行匹配，当匹配成功时，会返回 true，否则返回 false。例如：

			#include <iostream>
			#include <string>
			#include <regex>

			int main() {
				std::string fnames[] = {"foo.txt", "bar.txt", "test", "a0.txt", "AAA.txt"};
				// 在 C++ 中 `\` 会被作为字符串内的转义符，为使 `\.` 作为正则表达式传递进去生效，
				// 需要对 `\` 进行二次转义，从而有 `\\.`
				std::regex txt_regex("[a-z]+\\.txt");
				for (const auto &fname: fnames)
					std::cout << fname << ": " << std::regex_match(fname, txt_regex) << std::endl;
			}

	- 另一种常用的形式就是依次传入 std::string/std::smatch/std::regex 三个参数，
		其中 std::smatch 的本质其实是 std::match_results，在标准库中， 
		std::smatch 被定义为了 std::match_results<std::string::const_iterator>，
		也就是一个子串迭代器类型的 match_results。
		使用 std::smatch 可以方便的对匹配的结果进行获取，例如：

			std::regex base_regex("([a-z]+)\\.txt");
			std::smatch base_match;
			for(const auto &fname: fnames) {
				if (std::regex_match(fname, base_match, base_regex)) {
					// sub_match 的第一个元素匹配整个字符串
					// sub_match 的第二个元素匹配了第一个括号表达式
					if (base_match.size() == 2) {
						std::string base = base_match[1].str();
						std::cout << "sub-match[0]: " << base_match[0].str() << std::endl;
						std::cout << fname << " sub-match[1]: " << base << std::endl;
					}
				}
			}

		以上两个代码段的输出结果为：

			foo.txt: 1
			bar.txt: 1
			test: 0
			a0.txt: 0
			AAA.txt: 0
			sub-match[0]: foo.txt
			foo.txt sub-match[1]: foo
			sub-match[0]: bar.txt
			bar.txt sub-match[1]: bar
﻿##  智能指针和引用计数
- RAII 与引用计数
- std::shared_ptr
- std::unique_ptr
- std::weak_ptr

### RAII 与引用计数
> 基本想法是对于动态分配的对象，进行引用计数，每当增加一次对同一个对象的引用，那么引用对象的引用计数就会增加一次，
> 每删除一次引用，引用计数就会减一，当一个对象的引用计数减为零时，就自动删除指向的堆内存。

1. 在传统 C++ 中，『记得』手动释放资源，总不是最佳实践。因为我们很有可能就忘记了去释放资源而导致泄露。
2. 所以通常的做法是对于一个对象而言，我们在构造函数的时候申请空间，而在析构函数（在离开作用域时调用）的时候释放空间，
	也就是我们常说的** RAII 资源获取即初始化技术**。

3. 注意：引用计数不是垃圾回收，引用技术能够尽快收回不再被使用的对象，同时在回收的过程中也不会造成长时间的等待，
	更能够清晰明确的表明资源的生命周期。

### std::shared_ptr
> std::shared_ptr 是一种智能指针，它能够记录多少个 shared_ptr 共同指向一个对象，
> 从而消除显示的调用 delete，当引用计数变为零的时候就会将对象自动删除。

1. 但还不够，因为使用 std::shared_ptr 仍然需要使用 new 来调用，这使得代码出现了某种程度上的不对称。

	1. std::make_shared 就能够用来消除显示的使用 new，所以std::make_shared 
		会分配创建传入参数中的对象，并返回这个对象类型的std::shared_ptr指针。例如：

			#include <iostream>
			#include <memory>

			void foo(std::shared_ptr<int> i)
			{
			(*i)++;
			}
			int main()
			{
			// auto pointer = new int(10); // 非法, 不允许直接赋值
			// 构造了一个 std::shared_ptr
			auto pointer = std::make_shared<int>(10);
			foo(pointer);
			std::cout << *pointer << std::endl; // 11

			// 离开作用域前，shared_ptr 会被析构，从而释放内存
			return 0;
			}

	2. std::shared_ptr 可以通过 get() 方法来获取原始指针
	3. 通过 reset() 来减少一个引用计数
	4. 并通过get_count()来查看一个对象的引用计数。例如：

			auto pointer = std::make_shared<int>(10);
			auto pointer2 = pointer;    // 引用计数+1
			auto pointer3 = pointer;    // 引用计数+1
			int *p = pointer.get();             // 这样不会增加引用计数

			std::cout << "pointer.use_count() = " << pointer.use_count() << std::endl;      // 3
			std::cout << "pointer2.use_count() = " << pointer2.use_count() << std::endl;    // 3
			std::cout << "pointer3.use_count() = " << pointer3.use_count() << std::endl;    // 3

			pointer2.reset();
			std::cout << "reset pointer2:" << std::endl;
			std::cout << "pointer.use_count() = " << pointer.use_count() << std::endl;      // 2
			std::cout << "pointer2.use_count() = " << pointer2.use_count() << std::endl;    // 0, pointer2 已 reset
			std::cout << "pointer3.use_count() = " << pointer3.use_count() << std::endl;    // 2

			pointer3.reset();
			std::cout << "reset pointer3:" << std::endl;
			std::cout << "pointer.use_count() = " << pointer.use_count() << std::endl;      // 1
			std::cout << "pointer2.use_count() = " << pointer2.use_count() << std::endl;    // 0
			std::cout << "pointer3.use_count() = " << pointer3.use_count() << std::endl;    // 0, pointer3 已 reset

### std::unique_ptr
> std::unique_ptr 是一种独占的智能指针，它禁止其他智能指针与其共享同一个对象，从而保证了代码的安全：

		std::unique_ptr<int> pointer = std::make_unique<int>(10);   // make_unique 从 C++14 引入
		std::unique_ptr<int> pointer2 = pointer;    // 非法

1. make_unique 并不复杂，C++11 没有提供 std::make_unique，可以自行实现：

		template<typename T, typename ...Args>
		std::unique_ptr<T> make_unique( Args&& ...args ) {
			return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
		}

	1. 至于为什么没有提供，C++ 标准委员会主席 Herb Sutter 在他的博客中提到原因是因为『被他们忘记了』。

2. 既然是独占，换句话说就是不可复制。但是，我们可以利用 std::move 将其转移给其他的 unique_ptr，例如：

### std::weak_ptr
> 如果你仔细思考 std::shared_ptr 就会发现依然存在着资源无法释放的问题。看下面这个例子：

		#include <iostream>
		#include <memory>

		class A;
		class B;

		class A {
		public:
		std::shared_ptr<B> pointer;
		~A() {
			std::cout << "A 被销毁" << std::endl;
		}
		};
		class B {
		public:
		std::shared_ptr<A> pointer;
		~B() {
			std::cout << "B 被销毁" << std::endl;
		}
		};
		int main() {
		std::shared_ptr<A> a = std::make_shared<A>();
		std::shared_ptr<B> b = std::make_shared<B>();
		a->pointer = b;
		b->pointer = a;

		return 0;
		}

1. 运行结果是 A, B 都不会被销毁，这是因为 a,b 内部的 pointer 同时又引用了 a,b，
	这使得 a,b 的引用计数均变为了 2，而离开作用域时，a,b 智能指针被析构，却智能造成这块区域的引用计数减一，
	这样就导致了 a,b 对象指向的内存区域引用计数不为零，而外部已经没有办法找到这块区域了，也就造成了内存泄露

2. 解决这个问题的办法就是使用弱引用指针 std::weak_ptr
> std::weak_ptr是一种弱引用（相比较而言 std::shared_ptr 就是一种强引用）。
> **弱引用不会引起引用计数增加，当换用弱引用时候，最终的释放流程如下图**所示：


3. 在上图中，最后一步只剩下 B，而 B 并没有任何智能指针引用它，因此这块内存资源也会被释放。

4. std::weak_ptr 没有 * 运算符和 -> 运算符，所以不能够对资源进行操作，它的唯一作用就是用于检查 std::shared_ptr 是否存在，expired() 方法在资源未被释放时，会返回 true，否则返回 false。

正确的代码如下：

		#include <iostream>
		#include <memory>

		class A;
		class B;

		class A {
		public:
		// A 或 B 中至少有一个使用 weak_ptr
		std::weak_ptr<B> pointer;
		~A() {
			std::cout << "A 被销毁" << std::endl;
		}
		};
		class B {
		public:
		std::shared_ptr<A> pointer;
		~B() {
			std::cout << "B 被销毁" << std::endl;
		}
		};
		int main() {
		std::shared_ptr<A> a = std::make_shared<A>();
		std::shared_ptr<B> b = std::make_shared<B>();
		a->pointer = b;
		b->pointer = a;

		return 0;
		}

### 总结
1. 智能指针这种技术并不新奇，在很多语言中都是一种常见的技术，C++1x 将这项技术引进，
	在一定程度上消除了 new/delete 的滥用，是一种更加成熟的编程范式。

### 进一步阅读的参考资料
1. stackoverflow 上关于『C++11为什么没有 make_unique』的讨论
