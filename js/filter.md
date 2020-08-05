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

filter

阅读: 3501070 编辑
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

filter也是一个常用的操作，它用于把Array的某些元素过滤掉，然后返回剩下的元素。

和map()类似，Array的filter()也接收一个函数。和map()不同的是，filter()把传入的函数依次作用于每个
元素，然后根据返回值是true还是false决定保留还是丢弃该元素。

例如，在一个Array中，删掉偶数，只保留奇数，可以这么写：

var arr = [1, 2, 4, 5, 6, 9, 10, 15];
var r = arr.filter(function (x) {
    return x % 2 !== 0;
});
r; // [1, 5, 9, 15]

把一个Array中的空字符串删掉，可以这么写：

var arr = ['A', '', 'B', null, undefined, 'C', '  '];
var r = arr.filter(function (s) {
    return s && s.trim(); // 注意：IE9以下的版本没有trim()方法
});
r; // ['A', 'B', 'C']

可见用filter()这个高阶函数，关键在于正确实现一个“筛选”函数。

回调函数

filter()接收的回调函数，其实可以有多个参数。通常我们仅使用第一个参数，表示Array的某个元素。回调
函数还可以接收另外两个参数，表示元素的位置和数组本身：

var arr = ['A', 'B', 'C'];
var r = arr.filter(function (element, index, self) {
    console.log(element); // 依次打印'A', 'B', 'C'
    console.log(index); // 依次打印0, 1, 2
    console.log(self); // self就是变量arr
    return true;
});

利用filter，可以巧妙地去除Array的重复元素：

'use strict';

var
    r,
    arr = ['apple', 'strawberry', 'banana', 'pear', 'apple', 'orange', 'orange', 'strawberry'];
----
r = arr.filter(function (element, index, self) {
    return self.indexOf(element) === index;
});
----
console.log(r.toString());

去除重复元素依靠的是indexOf总是返回第一个元素的位置，后续的重复元素位置与indexOf返回的位置不相等
，因此被filter滤掉了。

练习

请尝试用filter()筛选出素数：

'use strict';

function get_primes(arr) {
----
    return [];
----
}

// 测试:
var
    x,
    r,
    arr = [];
for (x = 1; x < 100; x++) {
    arr.push(x);
}
r = get_primes(arr);
if (r.toString() === [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97].toString()) {
    console.log('测试通过!');
} else {
    console.log('测试失败: ' + r.toString());
}

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

