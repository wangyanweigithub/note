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

Map和Set

阅读: 19341438 编辑
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

JavaScript的默认对象表示方式{}可以视为其他语言中的Map或Dictionary的数据结构，即一组键值对。

但是JavaScript的对象有个小问题，就是键必须是字符串。但实际上Number或者其他数据类型作为键也是非常
合理的。

为了解决这个问题，最新的ES6规范引入了新的数据类型Map。要测试你的浏览器是否支持ES6规范，请执行以
下代码，如果浏览器报ReferenceError错误，那么你需要换一个支持ES6的浏览器：

'use strict';
var m = new Map();
var s = new Set();
console.log('你的浏览器支持Map和Set！');
----
// 直接运行测试

Map

Map是一组键值对的结构，具有极快的查找速度。

举个例子，假设要根据同学的名字查找对应的成绩，如果用Array实现，需要两个Array：

var names = ['Michael', 'Bob', 'Tracy'];
var scores = [95, 75, 85];

给定一个名字，要查找对应的成绩，就先要在names中找到对应的位置，再从scores取出对应的成绩，Array越
长，耗时越长。

如果用Map实现，只需要一个“名字”-“成绩”的对照表，直接根据名字查找成绩，无论这个表有多大，查找速度
都不会变慢。用JavaScript写一个Map如下：

var m = new Map([['Michael', 95], ['Bob', 75], ['Tracy', 85]]);
m.get('Michael'); // 95

初始化Map需要一个二维数组，或者直接初始化一个空Map。Map具有以下方法：

var m = new Map(); // 空Map
m.set('Adam', 67); // 添加新的key-value
m.set('Bob', 59);
m.has('Adam'); // 是否存在key 'Adam': true
m.get('Adam'); // 67
m.delete('Adam'); // 删除key 'Adam'
m.get('Adam'); // undefined

由于一个key只能对应一个value，所以，多次对一个key放入value，后面的值会把前面的值冲掉：

var m = new Map();
m.set('Adam', 67);
m.set('Adam', 88);
m.get('Adam'); // 88

Set

Set和Map类似，也是一组key的集合，但不存储value。由于key不能重复，所以，在Set中，没有重复的key。

要创建一个Set，需要提供一个Array作为输入，或者直接创建一个空Set：

var s1 = new Set(); // 空Set
var s2 = new Set([1, 2, 3]); // 含1, 2, 3

重复元素在Set中自动被过滤：

var s = new Set([1, 2, 3, 3, '3']);
s; // Set {1, 2, 3, "3"}

注意数字3和字符串'3'是不同的元素。

通过add(key)方法可以添加元素到Set中，可以重复添加，但不会有效果：

s.add(4);
s; // Set {1, 2, 3, 4}
s.add(4);
s; // 仍然是 Set {1, 2, 3, 4}

通过delete(key)方法可以删除元素：

var s = new Set([1, 2, 3]);
s; // Set {1, 2, 3}
s.delete(3);
s; // Set {1, 2}

小结

Map和Set是ES6标准新增的数据类型，请根据浏览器的支持情况决定是否要使用。

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

