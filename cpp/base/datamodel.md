## 容器
	../stl/容器.md

## CallableObject
1. 可被某种方式调用其某些函数的对象,它可以是:
	1. 一个函数,接受额外传入的args作为实参(arument)

	2. 一个指向成员函数的指针,<font color=green>当你通过对象调用它,该对象被传递成为第一个实参
	(必须是个reference或pointer)</font>,其他实参则一一对应成员函数的参数.

	3. 一个函数对象(function object,该对象拥有operator()),附带的args被传递作为实参.

	4. 一个lambda,严格地说,它是一种函数对象.
