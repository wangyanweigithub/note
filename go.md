## 编译
1. go run xx.go: 静态编译,并且运行临时生成的二进制文件

2. go build xx.go: 编译成为可执行文件

3. go get: 从网络下载源码并执行.

## 语言结构
### package 
1. Go语言的代码是通过package来组织的,package的概念和你知道的其它语言
里的libraries或者modules概念比较类似。一个package会包含一个或多个.go结束的源代码文件。
	每一个源文件都是以一个package xxx的声明语句开头的,比如我们的例子里就是package main。
	这行声明语句表示该文件是属于哪一个package,紧跟着是一系列import的package名,表示这个文件中引入的package。再之后是本文件本身的代码

2. go的标准库已经提供了100多个package,用来完成一门程序语言的一些常见的基本任务
比如输入、输出、排序或者字符串/文本处理

3. package main是一个比较特殊的package。这个package里会定义一个独立的程序,这个程序是可以运行的,
而不是像其它package一样对应一个library。在main这个package里,main函数也是一个特殊的函数,这是我们整个程序的入口(译注:其实C系语言差不多都是这样)

4. 假如你没有在代码里import需要用到的package,程序将无法编译通过

5. 同时当你import了没有用到的package,也会无法编译通过

6. 在import语句之后,则是各种方法、变量、常量、类型的
声明语句(分别用关键字func, var, const, type来进行定义)

### 其他
1. 在Go语言中的函数声明和 { 大括号必须在同一行,而在x + y这样的表达式中,在+号后换行可以,
但是在+号前换行则会有问题(译注:以+结尾的话不会被插入分号分隔符,但是以x结尾的话则会被分号分隔符,从而导致编译错误)

2. gofmt工具会将你的代码格式化为标准格式(译注:这个格式
化工具没有任何可以调整代码格式的参数,Go语言就是这么任性)

3. goimports,会自动地添加你代码里需要用到的import声明以及需要移除的import声
明
