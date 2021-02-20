## C++11 result_of函数的使用

1. result_of() 函数使用到了模板元编程技术，对于 F(Arg1, Arg2... Arg3) ，其中F 是一个可调用对象
（函数指针，函数引用，成员函数指针，或者是函数对象）f 的类型，Arg是其参数arg1, arg2 的类型。
那么reslut_of <Func(Arg1, Arg2... Arg3)> ::type 表示的也就是 f（arg1，arg2 ... argn）的返回值的类型。
```
#include <iostream>
#include <functional>

using namespace std;
using namespace std::placeholders;

typedef double(*FUNC)(double);

double func(double d)
{
		return sin(d);
}

int main()
{
	result_of<FUNC(double)>::type d = func(9.0);
	return 0;
}


当没有赋值表达式时，auto 的自动类型推导便会失去功效，此时的 result_of() 一样好用。

template<class Func, class Arg>
typename result_of<Func(Arg)> ::type fcall(Func func, Arg arg)
{
		return func(arg);
}
```
