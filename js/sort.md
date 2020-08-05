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

sort

阅读: 2153429 编辑
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

排序算法

排序也是在程序中经常用到的算法。无论使用冒泡排序还是快速排序，排序的核心是比较两个元素的大小。如
果是数字，我们可以直接比较，但如果是字符串或者两个对象呢？直接比较数学上的大小是没有意义的，因此
，比较的过程必须通过函数抽象出来。通常规定，对于两个元素x和y，如果认为x < y，则返回-1，如果认为x
== y，则返回0，如果认为x > y，则返回1，这样，排序算法就不用关心具体的比较过程，而是根据比较结果
直接排序。

JavaScript的Array的sort()方法就是用于排序的，但是排序结果可能让你大吃一惊：

// 看上去正常的结果:
['Google', 'Apple', 'Microsoft'].sort(); // ['Apple', 'Google', 'Microsoft'];

// apple排在了最后:
['Google', 'apple', 'Microsoft'].sort(); // ['Google', 'Microsoft", 'apple']

// 无法理解的结果:
[10, 20, 1, 2].sort(); // [1, 10, 2, 20]

第二个排序把apple排在了最后，是因为字符串根据ASCII码进行排序，而小写字母a的ASCII码在大写字母之后
。

第三个排序结果是什么鬼？简单的数字排序都能错？

这是因为Array的sort()方法默认把所有元素先转换为String再排序，结果'10'排在了'2'的前面，因为字符
'1'比字符'2'的ASCII码小。

douwo

如果不知道sort()方法的默认排序规则，直接对数字排序，绝对栽进坑里！

幸运的是，sort()方法也是一个高阶函数，它还可以接收一个比较函数来实现自定义的排序。

要按数字大小排序，我们可以这么写：

'use strict';

var arr = [10, 20, 1, 2];
----
arr.sort(function (x, y) {
    if (x < y) {
        return -1;
    }
    if (x > y) {
        return 1;
    }
    return 0;
});
console.log(arr); // [1, 2, 10, 20]

如果要倒序排序，我们可以把大的数放前面：

var arr = [10, 20, 1, 2];
arr.sort(function (x, y) {
    if (x < y) {
        return 1;
    }
    if (x > y) {
        return -1;
    }
    return 0;
}); // [20, 10, 2, 1]

默认情况下，对字符串排序，是按照ASCII的大小比较的，现在，我们提出排序应该忽略大小写，按照字母序
排序。要实现这个算法，不必对现有代码大加改动，只要我们能定义出忽略大小写的比较算法就可以：

var arr = ['Google', 'apple', 'Microsoft'];
arr.sort(function (s1, s2) {
    x1 = s1.toUpperCase();
    x2 = s2.toUpperCase();
    if (x1 < x2) {
        return -1;
    }
    if (x1 > x2) {
        return 1;
    }
    return 0;
}); // ['apple', 'Google', 'Microsoft']

忽略大小写来比较两个字符串，实际上就是先把字符串都变成大写（或者都变成小写），再比较。

从上述例子可以看出，高阶函数的抽象能力是非常强大的，而且，核心代码可以保持得非常简洁。

最后友情提示，sort()方法会直接对Array进行修改，它返回的结果仍是当前Array：

var a1 = ['B', 'A', 'C'];
var a2 = a1.sort();
a1; // ['A', 'B', 'C']
a2; // ['A', 'B', 'C']
a1 === a2; // true, a1和a2是同一对象

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

