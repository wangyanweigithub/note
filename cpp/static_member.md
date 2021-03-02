## C++之构造函数(Constructors)和static
1. 构造函数和静态成员：必须显式定义静态成员变量，不能出现在构造的初始化列表中
```
class Fred {
public:
	Fred();
private:
	int i_;
	static int j_;
}; 
Fred::Fred()
: i_(10)  // OK: you can (and should) initialize member data this way
, j_(42)  // Error: you cannot initialize static member data like this
{}

// You must define static data members this way:
int Fred::j_ = 42; 
```

2. 通常把静态成员的声明放到.H文件，定义放到.cpp中。如果没有定义，
会出现 "undefined external" 的链接错误。

3. 静态变量初始化顺序产生的错误是难以觉察，因为它发生在mian之前，如果你有两个静态成
员x，y，分别位于两个文件x.cpp、y.cpp中，而y在初始化要调用x，这样的场景很常见，出
错的几率有百分之五十。如果先初始化x，一切OK，如果先初始化y，那就惨了。例如：
 // File x.cpp
 #include "Fred.h"
 Fred x; 

// File y.cpp
 #include "Barney.h"
 Barney y; 
 // File Barney.cpp
 #include "Barney.h"
 
 Barney::Barney()
 {
   [dot]
   x.goBowling();
   [dot]
 } 
解决这种静态成员初始化的方法很多，一个简单的方法就是用静态方法x（）替代Fred x，
然后返回这个Fred的引用，如下所示:
 // File x.cpp
 
 #include "Fred.h"
 
 Fred& x()
 {
   static Fred* ans = new Fred();
   return *ans;
 } 
这个静态变量这初始化一次，以后将一直返回同样的Fred对象。这是修改后的代码
 // File Barney.cpp
 #include "Barney.h"
 
 Barney::Barney()
 {
   [dot]
   x().goBowling();
   [dot]
 } 
第一次使用，Fred对象先被构造。但是这个解决方法使用时要慎重，在这里第一选择是使用
静态成员，使用静态指针会有一些副作用，倒不是担心内存泄露，在程序退出时系统会自己
释放这些堆空间。在使用静态变量时要保证第一次使用前被初始化，最后一次使用后被析构
，在这里我们要注意的是析构函数的代码。如果静态变量a、b、c在构造时调用ans没问题，
但是在析构时如果还调用ans，程序极有可能崩溃。这种应用在实际中并不多见，解决的方
法有三种，待以后的主题中在讲。
在这里有个static initialization和static deinitialization，前者意义大家都知道，后
者则是去初始化指的是在应用之前被别的代码给析构了，导致我们用的这个静态量没有初始
化，这个是很致命的，尤其在静态指针中，表现的更为明显。
当然static这个关键字也并非一无是处，下面的代码中的错误就可以用staic来解决：
#include <iostream>
 
 int f();  // forward declaration
 int g();  // forward declaration
 
 int x = f();
 int y = g();
数据  8  
加载  9  int f()
中…… 10  {
   std::cout << "using 'y' (which is " << y << ")\n";
   return 3*y + 7;
 }
 
 int g()
 {
   std::cout << "initializing 'y'\n";
   return 5;
 } 
这段代码显然不能通过编译，下面通过static改变了初始化的顺序
#include <iostream>
 
 int f();  // forward declaration
 int g();  // forward declaration
 
 int x = f();
 int y = g();
 
 int f()
 {
   std::cout << "using 'y' (which is " << y << ")\n";
   return 3*y + 7;
 }
 
 int g()
 {
   std::cout << "initializing 'y'\n";
   return 5;
 }
#include <iostream>
 
 int f();  // forward declaration
 int g();  // forward declaration
 
 int x = f();
 int y = g();
 
 int f()
 {
   std::cout << "using 'y' (which is " << y << ")\n";
   return 3*y + 7;
 }
 
 int g()
 {
   std::cout << "initializing 'y'\n";
   return 5;
 } 

但是上面的更改只限于编译器的内置数据类型，而不是用户自定义的数据类型。
