  • 目录

廖雪峰的官方网站  Blog

  • 编程
  • 读书
  • Java教程
  • Python教程
  • JavaScript教程
  • 区块链教程
  • SQL教程
  • Git教程
  • 问答
  • 更多
      □ 编程
      □ 读书
      □ Java教程
      □ Python教程
      □ JavaScript教程
      □ 区块链教程
      □ SQL教程
      □ Git教程
      □ 问答

  •  
      □ 编程
      □ 读书
      □ Java教程
      □ Python教程
      □ JavaScript教程
      □ 区块链教程
      □ SQL教程
      □ Git教程
      □ 问答

  •  
      □ 个人资料
      □ 
      □ 登出
  • 登录
  •  
  •  
      □ English
      □ 简体中文

   

  • 目录

    JavaScript教程
    JavaScript简介
    快速入门
    基本语法
    数据类型和变量
    字符串
    数组
    对象
    条件判断
    循环
    Map和Set
    iterable
    函数
    函数定义和调用
    变量作用域与解构赋值
    方法
    高阶函数
    map/reduce
    filter
    sort
    Array
    闭包
    箭头函数
    generator
    标准对象
    Date
    RegExp
    JSON
    面向对象编程
    创建对象
    原型继承
    class继承
    浏览器
    浏览器对象
    操作DOM
    更新DOM
    插入DOM
    删除DOM
    操作表单
    操作文件
    AJAX
    Promise
    Canvas
    jQuery
    选择器
    层级选择器
    查找和过滤
    操作DOM
    修改DOM结构
    事件
    动画
    AJAX
    扩展
    错误处理
    错误传播
    异步错误处理
    underscore
    Collections
    Arrays
    Functions
    Objects
    Chaining
    Node.js
    安装Node.js和npm
    第一个Node程序
    搭建Node开发环境
    模块
    基本模块
    fs
    stream
    http
    crypto
    Web开发
    koa
    koa入门
    处理URL
    使用Nunjucks
    使用MVC
    mysql
    使用Sequelize
    建立Model
    mocha
    编写测试
    异步测试
    Http测试
    WebSocket
    使用ws
    编写聊天室
    REST
    编写REST API
    开发REST API
    MVVM
    单向绑定
    双向绑定
    同步DOM结构
    集成API
    在线电子表格
    自动化工具
    React
    第一个React应用
    元素渲染
    组件
    组件和状态
    期末总结

关于作者

关注公众号不定期领红包：

[0]

关注微博获取实时动态：

map/reduce

阅读: 8307368 编辑
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

如果你读过Google的那篇大名鼎鼎的论文“MapReduce: Simplified Data Processing on Large Clusters”，
你就能大概明白map/reduce的概念。

map

举例说明，比如我们有一个函数f(x)=x^2，要把这个函数作用在一个数组[1, 2, 3, 4, 5, 6, 7, 8, 9]上，
就可以用map实现如下：

map

由于map()方法定义在JavaScript的Array中，我们调用Array的map()方法，传入我们自己的函数，就得到了一
个新的Array作为结果：

'use strict';

function pow(x) {
    return x * x;
}
----
var arr = [1, 2, 3, 4, 5, 6, 7, 8, 9];
var results = arr.map(pow); // [1, 4, 9, 16, 25, 36, 49, 64, 81]
console.log(results);

注意：map()传入的参数是pow，即函数对象本身。

你可能会想，不需要map()，写一个循环，也可以计算出结果：

var f = function (x) {
    return x * x;
};

var arr = [1, 2, 3, 4, 5, 6, 7, 8, 9];
var result = [];
for (var i=0; i<arr.length; i++) {
    result.push(f(arr[i]));
}

的确可以，但是，从上面的循环代码，我们无法一眼看明白“把f(x)作用在Array的每一个元素并把结果生成一
个新的Array”。

所以，map()作为高阶函数，事实上它把运算规则抽象了，因此，我们不但可以计算简单的f(x)=x^2，还可以
计算任意复杂的函数，比如，把Array的所有数字转为字符串：

var arr = [1, 2, 3, 4, 5, 6, 7, 8, 9];
arr.map(String); // ['1', '2', '3', '4', '5', '6', '7', '8', '9']

只需要一行代码。

reduce

再看reduce的用法。Array的reduce()把一个函数作用在这个Array的[x1, x2, x3...]上，这个函数必须接收
两个参数，reduce()把结果继续和序列的下一个元素做累积计算，其效果就是：

[x1, x2, x3, x4].reduce(f) = f(f(f(x1, x2), x3), x4)

比方说对一个Array求和，就可以用reduce实现：

var arr = [1, 3, 5, 7, 9];
arr.reduce(function (x, y) {
    return x + y;
}); // 25

练习：利用reduce()求积：

'use strict';

function product(arr) {
----
    return 0;
----
}

// 测试:
if (product([1, 2, 3, 4]) === 24 && product([0, 1, 2]) === 0 && product([99, 88, 77, 66]) === 44274384) {
    console.log('测试通过!');
}
else {
    console.log('测试失败!');
}

要把[1, 3, 5, 7, 9]变换成整数13579，reduce()也能派上用场：

var arr = [1, 3, 5, 7, 9];
arr.reduce(function (x, y) {
    return x * 10 + y;
}); // 13579

如果我们继续改进这个例子，想办法把一个字符串13579先变成Array——[1, 3, 5, 7, 9]，再利用reduce()就
可以写出一个把字符串转换为Number的函数。

练习：不要使用JavaScript内置的parseInt()函数，利用map和reduce操作实现一个string2int()函数：

'use strict';

function string2int(s) {
----
    return 0;
----
}

// 测试:
if (string2int('0') === 0 && string2int('12345') === 12345 && string2int('12300') === 12300) {
    if (string2int.toString().indexOf('parseInt') !== -1) {
        console.log('请勿使用parseInt()!');
    } else if (string2int.toString().indexOf('Number') !== -1) {
        console.log('请勿使用Number()!');
    } else {
        console.log('测试通过!');
    }
}
else {
    console.log('测试失败!');
}

练习

请把用户输入的不规范的英文名字，变为首字母大写，其他小写的规范名字。输入：['adam', 'LISA',
'barT']，输出：['Adam', 'Lisa', 'Bart']。

'use strict';

function normalize(arr) {
----
    return [];
----
}

// 测试:
if (normalize(['adam', 'LISA', 'barT']).toString() === ['Adam', 'Lisa', 'Bart'].toString()) {
    console.log('测试通过!');
}
else {
    console.log('测试失败!');
}

小明希望利用map()把字符串变成整数，他写的代码很简洁：

'use strict';

var arr = ['1', '2', '3'];
var r;
----
r = arr.map(parseInt);
----
console.log(r);

结果竟然是1, NaN, NaN，小明百思不得其解，请帮他找到原因并修正代码。

提示：参考Array.prototype.map()的文档。

原因分析

由于map()接收的回调函数可以有3个参数：callback(currentValue, index, array)，通常我们仅需要第一个
参数，而忽略了传入的后面两个参数。不幸的是，parseInt(string, radix)没有忽略第二个参数，导致实际
执行的函数分别是：

  • parseInt('1', 0); // 1, 按十进制转换

  • parseInt('2', 1); // NaN, 没有一进制

  • parseInt('3', 2); // NaN, 按二进制转换不允许出现3

可以改为r = arr.map(Number);，因为Number(value)函数仅接收一个参数。

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

评论

发表评论

登录后发表评论

  • 目录

  • JavaScript教程
  • JavaScript简介
  • 快速入门
  • 基本语法
  • 数据类型和变量
  • 字符串
  • 数组
  • 对象
  • 条件判断
  • 循环
  • Map和Set
  • iterable
  • 函数
  • 函数定义和调用
  • 变量作用域与解构赋值
  • 方法
  • 高阶函数
  • map/reduce
  • filter
  • sort
  • Array
  • 闭包
  • 箭头函数
  • generator
  • 标准对象
  • Date
  • RegExp
  • JSON
  • 面向对象编程
  • 创建对象
  • 原型继承
  • class继承
  • 浏览器
  • 浏览器对象
  • 操作DOM
  • 更新DOM
  • 插入DOM
  • 删除DOM
  • 操作表单
  • 操作文件
  • AJAX
  • Promise
  • Canvas
  • jQuery
  • 选择器
  • 层级选择器
  • 查找和过滤
  • 操作DOM
  • 修改DOM结构
  • 事件
  • 动画
  • AJAX
  • 扩展
  • 错误处理
  • 错误传播
  • 异步错误处理
  • underscore
  • Collections
  • Arrays
  • Functions
  • Objects
  • Chaining
  • Node.js
  • 安装Node.js和npm
  • 第一个Node程序
  • 搭建Node开发环境
  • 模块
  • 基本模块
  • fs
  • stream
  • http
  • crypto
  • Web开发
  • koa
  • koa入门
  • 处理URL
  • 使用Nunjucks
  • 使用MVC
  • mysql
  • 使用Sequelize
  • 建立Model
  • mocha
  • 编写测试
  • 异步测试
  • Http测试
  • WebSocket
  • 使用ws
  • 编写聊天室
  • REST
  • 编写REST API
  • 开发REST API
  • MVVM
  • 单向绑定
  • 双向绑定
  • 同步DOM结构
  • 集成API
  • 在线电子表格
  • 自动化工具
  • React
  • 第一个React应用
  • 元素渲染
  • 组件
  • 组件和状态
  • 期末总结

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

廖雪峰的官方网站 ©Copyright 2019 京ICP备13005808号-8
Powered by iTranswarp

   

意见反馈
使用许可

