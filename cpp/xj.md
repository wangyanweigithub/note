c++的陷阱

[original]
rainychen1022 2016-06-13 16:05:50 [articleRea] 573 [tobarColle] [tobarColle] 收藏
分类专栏：基础文章标签： c++ stl 对象 class
最后发布:2016-06-13 16:05:50首次发布:2016-06-13 16:05:50
版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本声明。
本文链接：https://blog.csdn.net/chengangdzzd/article/details/51658499
版权

正确的初始化对象

对于内置数据类型，系统不会去初始化, 因为这会增加运行成本.
所以,   永远在使用内置类型前将它们手动初始化;

对于内置类型以外的东西,  系统会自动调用构造函数来初始化。
我们要确保每个构造函数都将对象的每一个成员做了初始化.

初始化对象的成员时，必须分清初始化和赋值的区别，防止二次初始化。
using std::string;
class Player{
        Player(string &s, int x, int y);
private:
        string _name;
        int _x;
        int _y;
}

错误的示例1：
Player::Player (string &s, int x, int y){
        _name = s;//这是赋值，而不是初始化，string已经被初始化过了。
        _x = x;
        _y = y;
}

正确的示例2，使用成员初值表 ：
Player::Player(string &s, int x, int y) ：_name(s), _x(x), _y(y){
}
为了避免遗漏，必须在初值表中列出所有成员。

错误的示例3：
Player::Player(string &s, int a) ：_name(s), _y(a), _x(_y){
}
C++有着固定的成员初始化次序: 
基类的初始化在派生类之前, 成员变量按照声明顺序被初始化（而不是初值表里的顺序）.
示例3原意是用a来初始化_y, 再用_y来初始化_x, 实际结果确导致_x是一个未知数。
成员初值表里的顺序必须和声明的顺序相同.

有争议的示例4：
Player::Player (string &s, int x, int y) ：_name(s){
        _x = x;
        _y = y;
}
这样的编码方式带来了一些隐患，比如成员被遗漏，但也带来了某些好处：
由于内置类型不会被自动初始化，所以它们在成员初值表和构造函数体内进行初始化， 在性能和结果上都是一样的。
对于有多个构造函数的类, 每个构造函数都应该有自己的成员初值表.
这会导致不受欢迎的重复.  如果我们把初始化内置数据类型成员的部分放到一个通用的函数内，就可以消除部分重复代
码。

什么时候需要一个虚析构函数

class Base {
public:
    Base(){printf(" Base constructor\n");}
    ~Base(){ printf(" Base distructor\n ");}
};
class Derived: public Base {
public:
      Derived(){ printf(" Derived constructor\n";}
    ~ Derived(){ printf(" Derived distructor\n";}
};
可以看出，Base没有虚函数，不具有多态性质。
如果写出这样的代码：
Base* pBase = new Derived();
delete pBase;
运行结果如下：
Base constructor
Derived constructor
Base distructor
可以看到， Derived的析构函数没有被执行，在实际运用这可能导致很严重的问题。比如内存泄漏

如果基类不具有多态性质，就不能进行多态编码。
如果我们希望基类带多态性质，  或者类中至少有一个虚函数， 就应该申明虚析构函数.
但也不要无端的将所有类的析构函数都声明为虚,  因为这会增加对象的体积, 并降低代码的可移植性。


绝不在构造函数和析构函数中调用虚函数

class Base {
public:
    Base(){Fun();}
    virtual ~Base(){Fun();}
    virtual Fun(){printf("Base Fun\n");}
};
class Derived: public Base {
public:
      Derived(){}
    virtual ~ Derived(){}
    virtual Fun(){printf("Derived Fun\n");}
};

Derived *p = new Derived();
delete p;
运行结果如下：
Base Fun
Base Fun
可见， 不管是构造还是析构，都没有实现多态。原因是：
要实例化 Derived , 首先会调用 Base 的构造函数, 遇到 Fun, 由于 Fun是虚函数, 
应该下降调用 Derived 的 Fun函数, 但是Derived 此时还没有被实例化.
实际上只能调用的是 Base 的Fun 函数.
析构的过程是一样的。

程序员应该避免这种不合理的设计出现. 不要再构造函数和析构函数中调用虚函数。
在这两个过程中，多态无法实现。
而且,   构造函数和析构函数调用的函数里也不能嵌套 virtual 函数.


防止隐式调用构造函数带来的意外

如果构造函数只有一个参数, 或者有多个参数但除第一个参数外其余参数都有默认值,  那么这个构造函数承担了两个角
色:
1 是个构造器  ，
2 是个默认且隐含的类型转换操作符。
所以， 有时候在我们写下如 A = X， 这样的代码， 且恰好X的类型正好是A单参数构造器的参数类型，  这时候编译器
就自动调用这个构造器， 创建一个A的对象。
这样看起来好象很酷， 很方便。但在某些情况下， 却违背了我们的本意。 
这时候就要在这个构造器前面加上explicit修饰， 指定这个构造器只能被明确的调用，不能作为类型转换操作符被隐含
的使用。
class  Test1 {
public :
         Test1( int  n)：_num(n) { } //普通构造函数
private :
         int _ num;
};
class  Test2 {
public :
         explicit  Test2( int  n): _num(n) { } //explicit(显式)构造函数
private :
         int _ num;
};
Test1 t1=6; //隐式调用其构造函数,成功
Test2 t2= 6; //编译错误,不能隐式调用其构造函数
Test2 t2(6); //显式调用成功

了解编译器可能自动生成那些成员函数

如果你写下:
class Empty{};

编译器会自动为你写下这些代码:
class Empty {
public: 
        Empty(){...}                                                           // 构造函数
        Empty(const Empty& rhs){...}                              // 拷贝构造函数
        Empty& operator=(const Empty& rhs){...}        // 赋值函数
        ~Empty(){...}                                                        // 析构函数
 };
如果你自己声明了某个函数, 编译器就不再默认创建它


区分拷贝构造函数和赋值函数，并正确的实现它们


拷贝构造函数和赋值函数非常容易混淆，常导致错写、错用。

拷贝构造函数是在对象被创建时调用的，而赋值函数只能被已经存在了的对象调用。

以下程序中，第三个语句和第四个语句很相似，你分得清楚哪个调用了拷贝构造函数，哪个调用了赋值函数吗？
　　String a(“hello”);
　　String b(“world”);
　　String c = a; // 调用了拷贝构造函数，最好写成 c(a);
　　c = b; // 调用了赋值函数
本例中第三个语句的风格较差，宜改写成String c(a) 以区别于第四个语句。


如果不主动编写拷贝构造函数和赋值函数，编译器将以“位拷贝”的方式自动生成缺省的函数。
倘若类中含有指针变量，那么这两个缺省的函数就隐含了错误。


以类String 的两个对象a,b 为例，假设a.m_data 的内容为“hello”，b.m_data 的内容为“world”。

现将a 赋给b，缺省赋值函数的“位拷贝”意味着执行b.m_data = a.m_data。这将造成三个错误：

一是b.m_data 原有的内存没被释放，造成内存泄露；

二是b.m_data 和a.m_data 指向同一块内存，a 或b 任何一方变动都会影响另一方；

三是在对象被析构时，m_data 被释放了两次。


正确的实现类String 的拷贝构造函数与赋值函数
// 拷贝构造函数
String::String(const String &other){

　　// 允许操作other 的私有成员m_data
　　int length = strlen(other.m_data);
　　m_data = new char[length+1];
　　strcpy(m_data, other.m_data);
}
// 赋值函数
String & String::operator =(const String &other){
　　// (1) 检查自赋值
　　if(this == &other)
　　       return *this;
　　// (2) 释放原有的内存资源
　　delete [] m_data;
　　// （3）分配新的内存资源，并复制内容
　　int length = strlen(other.m_data);
　　m_data = new char[length+1];
　　strcpy(m_data, other.m_data);
　　// （4）返回本对象的引用
　　return *this;
}
类String 的赋值函数比构造函数复杂得多。因为它要检查自我赋值。
你可能会认为多此一举，难道有人会愚蠢到写出 a = a 这样的自赋值语句！

这可不好说。而且，间接的自我赋值很有可能出现，例如
　　a[i] = a[j]; //i，j可能相等

       *px = *py;//两个指针指向同一个地址

自我赋值会导致严重的问题。看看第二步的delete，自杀后还能复制自己吗？

所以，如果发现自赋值，应该马上终止函数。注意不要将检查自赋值的if 语句
　　if(this == &other)
　　错写成为
　　if( *this == other)
注意函数strlen 返回的是有效字符串长度，不包含结束符‘\0’。函数strcpy 则连‘\0’一起复制。
返回本对象的引用，目的是为了实现象 a = b = c 这样的链式表达。


如果我们实在不想编写拷贝构造函数和赋值函数，又不允许别人使用编译器生成的缺省函数，怎么办？
只需将拷贝构造函数和赋值函数声明为私有函数，并不予实现。



更多自我赋值的思考

我们通常用“证同测试”来避免自我赋值，比如这样
String & String::operator =(const String &other){
　　if(this == &other)
　　       return *this;
　　delete [] m_data;
　　m_data = new char[strlen(other.m_data)+1];
　　strcpy(m_data, other.m_data);
　　return *this;
}
实际上，自我复制的发生频率很低，这样证同测试就有点影响效率了。
有一个办法既可以避免自我赋值，又不需要证同测试。
String & String::operator =(const String &other){
       char* p = m_data;
　　m_data = new char[strlen(other.m_data)+1];
　　strcpy(m_data, other.m_data);
       delete p;
　　return *this;
}
还有一个十分巧妙并且更高效的方法：
String & String::operator =(String other){//注意这里是传值
       char* p = other.m_data;
       other.m_data = m_data;
       m_data = p;
       return *this;
}
酷！它把数据拷贝动作从函数体内转移到函数参数的构造阶段，可以让编译器生成更高效的代码。
但是它牺牲了清晰性，请酌情使用。


正确理解对象占用的内存

理论上, 空对象不占用内存, 但是系统为了区分不同的空对象, 仍然分配了一小块地址, 通常是1字节.
系统为每个类维护了一个方法表., 所以方法不会占用对象的内存,  如果一个对象只有方法没有变量, 它占用的内存也是
1字节.
类只需要为它的每个静态变量维护一个实例, 所以静态变量也不会占用对象的内存.

如果类包含虚函数, 系统需要为它维护一个虚函数表. 为了实现多态, 对象必须保存它所属的类的虚函数表的指针.
所以一个只有虚函数的类, 它的对象占用的内存是一个指针大小, 4字节.

虚函数表是一个在内存上连续的函数指针数组, 函数指针分别指向类中的每一个虚函数.
虽然一个子类的对象地址可以被赋值给一个基类指针, 由于这个对象保存了它真正的类的虚函数表地址, 所以它总能正确
的调用自己的虚函数.


正确的在循环中利用迭代器删除map中的元素

错误的方法, 方法 1:
for (iterator it=map.begin(); it!=map.end(); it++){
        if (it->first == a){
            map.erase(it);
        }
}

正确的方法, 方法 2:
for (iterator it=map.begin(); it!=map.end(); ){
        if (it->first == a){
            map.erase(it++);
        }else{
            it++;
        }
}

这样也可以, 方法 3:
for (iterator it=map.begin(); it!=map.end(); it++){
        if (it->first == a){
            it = map.erase(it);
            it--;
        }
}

为什么方法2是对的? 方法1和方法2不是一样的吗?
it++操作主要做三件事情：
1、首先把it备份一下。
2、把it加上1。
3、返回第一步备份的it。

因此，map.erase(it++);

在执行erase之前，it已经被加一了。

erase会使得以前那个未被加一的it失效，而加了一之后的新的it是有效的。

这段代码的真正等效代码是

iterator iterTemp = it;
++it;
map.erase(iterTemp);


反之，如果这样：

map.erase(it);
it++;

那么erase操作直接让it失效，对失效的it进行加一操作也是失效的。


删除map中的一个节点后, map会发生什么变化?

map中其它节点的地址不会发生变化, 变化的只是某些节点的左右节点指针.


明白string带来的额外内存开销


观察一块测试代码

string s;

cout<<sizeof(s)<<endl;//28

cout<<s.size()<<","<<s.capacity()<<endl;//0,15

s.resize(8);

cout<<s.size()<<","<<s.capacity()<<endl;//8,15

s.resize(16);

cout<<s.size()<<","<<s.capacity()<<endl;//16,31

s.resize(1000);

cout<<s.size()<<","<<s.capacity()<<endl;//1000,1007

一开始, string 是空的, 它的 size 是 0, 

但是在实例化的时候, 它已经预先申请了一块内存备用.

这块内存的大小是 15, 我推测, 应该是 16, 有一个字节被内部使用了.

另外, string 的内部数据占用了 28 个字节, 所以一个空string, 也要使用 44 个字节的内存. 有点心疼呀.....

随着 string 里数据的增加, 申请的内存总是 16 的倍数. 










  • [tobarThumb] [tobarThumb] [tobarThumb] 点赞
  • [tobarComme] [tobarComme] 评论
  • [tobarShare] [tobarShare] 分享
    x

    海报分享

    扫一扫，分享海报

  • [tobarColle] [tobarColle] [tobarColle] 收藏
  • [tobarRewar] [tobarRewar] 打赏
   
    打赏

        [3_chengang]
    rainychen1022

        你的鼓励将是我创作的最大动力

    C币余额
    2C币 4C币 6C币 10C币 20C币 50C币

    确定

  • [tobarRepor] [tobarRepor] 举报
  • 关注关注
  • 一键三连

    点赞Mark关注该博主, 随时了解TA的最新博文[closePromp]

已标记关键词清除标记
[INS::INS]
C语言陷阱和缺陷
11-22
 
那些自认为已经“学完”C 语言的人，请你们仔细读阅读这篇文章吧。路还长，很多东西要学。我也是
[anonymous-]
[commentFla] [                    ]
表情包
插入表情
[001] [002] [003] [004] [005] [006] [007] [008] [009] [010] [011] [012] [013] [014] [015] [016] [017]
[018] [019] [020] [021] [022] [023] [024] [025] [026] [027] [028] [029] [030] [031] [032] [033] [034]
[035] [036] [037] [038] [039] [040] [041] [042] [043] [044] [045] [046] [047] [048] [049] [050] [051]
[052] [053] [054] [055] [056] [057] [058] [059] [060] [061] [062] [063] [064]
 
添加代码片

  • HTML/XML
  • objective-c
  • Ruby
  • PHP
  • C
  • C++
  • JavaScript
  • Python
  • Java
  • CSS
  • SQL
  • 其它

还能输入1000个字符 [发表评论]
[评论]
C++程序设计陷阱（中文版）
07-27
 
计算机经典数据——C++ Gotchas Avoiding Common Problems in Coding and Design
C++常见陷阱
u012456479的博客
10-24 [readCountW] 127
 
学习的过程，就是填坑的过程。

C/C++陷阱与套路
沉迷单车的追风少年
07-23 [readCountW] 185
 
一、导语 C++是一门被广泛使用的系统级编程语言，更是高性能后端标准开发语言；C++虽功能强大，灵活巧妙，但却属
于易学难精的专家型语言，不仅新手难以驾驭，就是老司机也容易掉进各种陷阱。本文结合作者的工作经验和学习心得，
对C++语言的一些高级特性，做了简单介绍；对一些常见的误解，做了解释澄清；对比较容易犯错的地方，做了归纳总结
；希望借此能增进大家对C++语言了解，减少编程出错，提升工作效率。二、陷阱我的程序里用了全局变量，为何进程退
出会莫名其妙的core掉？ Rule：C++在不同模块（源文件）
c/c++中的黑洞陷阱问题的简单解析
W_C_H_815的博客
04-07 [readCountW] 486
 
495被称为黑洞数或者陷阱数即：给定任何一个小于1000的正整数，经前位补00后可以得到一个三位数（两位数前面补11
个00，一位数前面补22个00）。如果这个三位数的三个数字不全相等，那么经有限次“重排求差”操作（组成该数的数字重
排后的最大数减去重排后的最小数），总会得到495。这里通过for循环和选择结构的嵌套得到黑洞数：495 #include
<stdio.h> int mai...
c/c++好多陷阱,越来越感觉确实水深...
12-27
 
void freePtr1(int* p1) //虽然没有正确释放p 确真正释放了内存再次p操作很危险 { delete p1; p1 = NULL; } void
freePtr2(int*& p
C++ 常见陷阱之语法
ACmeinan的博客
05-11 [readCountW] 878
 
1.函数参数求值顺序至少在2018的今天，C++的函数参数求值顺序仍然是未定的（交给编译器处理），所以函数参数求值
的顺序可能在某些情况下回导致一些问题。看下面这个例子： int test=5; printf( &amp;quot;%d,%d,%d\n&amp;
quot;,++test,test,test++); test=5; printf( &amp;quot;%d,%d,%d\n&amp;quot;,test++,...
C+++程序设计陷阱（中文版）
03-24
 
C+++程序设计陷阱内容很精彩详实值得一看受益匪浅
c++ 数字黑洞
ayzhkb8029的博客
04-09 [readCountW] 80
 
//运行超时了 ,该使用字符串 #include<iostream>#include<string.h>#include<cstdio>using namespace std;int
di_zeng(int &a){//递增函数 char str[5]; sprintf(str,"%d",a); for(int i=0;i&...
C++中的陷阱(一)
weixin_30525825的博客
03-31 [readCountW] 66
 
导言程序设计语言中充满陷阱，一不小心就会掉入其中万劫不复，之所以有陷阱，是因为语言的设计细节不符合程序员的
直觉所以你会发现，语言越高级越注重顺从程序员的直觉。 c++也有许多陷阱，所谓山不过来，我就过去，因此将c++中
易错点、难点集合于此，会不定期更新。字符串与vector 字符串字面值与标准库string不是同一种类型 string s
("hello"); cout...
C++黑洞数字很详细哦
03-22
 
c++经典程序 int main() { srand((unsigned)time(NULL)); //时钟振荡产生随机数 int number = rand() % 10000;
cout << "
C++,那些可爱的小陷阱（一）
weixin_34357436的博客
11-17 [readCountW] 26
 
此系列是为那些读过TC++PL或者具有类似水平的同学准备的，作为系列的第一篇以及有趣的热身，我们来看一个链接问题
： D1.cpp #include &lt;stdio.h&gt;struct X {    X(int);    X(int, int);};X::X(int = 0, int = 0) { printf
("1\n"); }class D: public X {}; int min...
C/C++陷阱与缺陷
 
09-10 [readCountW] 520
 
//++a与a++ ++a，在a存储的值上增加1并向使用它的表达式“返回”新的a值； a++，向使用它的表达式“返回”a的值，再对
a加1。例如，下列程序： a=5;b=5; printf("%d\n",++a); printf("%d\n",b++); 输出结果：
C/C++的陷阱
abcnull 的博客
03-16 [readCountW] 161
 
//例1 #include &lt;stdio.h&gt; int main() { char *p=(char *)malloc(sizeof(char)*2); printf("%d\n",sizeof
(*p)); return 0; }输出的结果是1而不是2，要是把sizeof(*p)改成sizeof(p)呢？//例2 #include &lt;stdio.h&gt;
int main...
c++陷阱与进阶
yuqangy的专栏
03-22 [readCountW] 660
 
1.  尽量不要给指针拷贝多份多处使用 2. 单例模式注意多线程安全和编译器兼容 ...持续更新
C++ 程序设计陷阱（中文版）
05-14
 
本书是一本专业程序员指南，它能够避免和纠正在C++程序设计和编写过程中所遇到的95%的最常见的，最具破坏性和最有
趣的错误。
C++程序设计陷阱
09-27
 
不适合初学者看,对于提高c++编程能力还是不错的
C++11 陷阱与缺陷
05-21
 
C++11 陷阱与缺陷C++11 陷阱与缺陷C++11 陷阱与缺陷C++11 陷阱与缺陷C++11 陷阱与缺陷C++11 陷阱与缺陷C++11 陷阱
与缺陷
©️2020 CSDN 皮肤主题: 大白设计师:CSDN官方博客返回首页
[3_chengang]
rainychen1022 CSDN认证博客专家 CSDN认证企业博客
码龄5年 [nocErtific] 暂无认证

 
8
    原创

 
44万+
    周排名

 
22万+
    总排名

3万+
    访问

[blog3]
    等级

410
    积分

18
    粉丝

25
    获赞

5
    评论

8
    收藏

私信
关注
[                    ] [csdn-sou]

热门文章

  • 游戏服务器开发和维护的一些经验 [readCountW] 6345
  • 管道和多线程 [readCountW] 6270
  • 如何在进程崩溃后打印堆栈并防止数据丢失 [readCountW] 5206
  • 用原子操作实现无锁编程 [readCountW] 4335
  • 网络编程的技术要点 [readCountW] 3018

分类专栏

  • [resize] 基础 4篇
  • [resize] 应用 4篇

最新评论

  • 游戏服务器开发和维护的一些经验

    weixin_31573357: Mark！

  • 游戏服务器开发和维护的一些经验

    圣西罗南看台1: 牛了

  • 游戏服务器开发和维护的一些经验

    kongkongkkk: 最近也在写一个简易负载均衡器，看了你的文章学习了！

  • Linux进程详解

    u010097777: 基本素养。

  • 网络编程的技术要点

    sinat_32111025: 不明觉厉~

最新文章

  • 如何在进程崩溃后打印堆栈并防止数据丢失
  • Linux进程详解
  • 网络编程的技术要点

2016年8篇
[INS::INS]

目录

目录

[INS::INS]

分类专栏

  • [resize] 基础 4篇
  • [resize] 应用 4篇

[]
