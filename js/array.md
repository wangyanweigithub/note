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

Array

阅读: 1595052 编辑
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

对于数组，除了map()、reduce、filter()、sort()这些方法可以传入一个函数外，Array对象还提供了很多非
常实用的高阶函数。

every

every()方法可以判断数组的所有元素是否满足测试条件。

例如，给定一个包含若干字符串的数组，判断所有字符串是否满足指定的测试条件：

'use strict';
----
var arr = ['Apple', 'pear', 'orange'];
console.log(arr.every(function (s) {
    return s.length > 0;
})); // true, 因为每个元素都满足s.length>0

console.log(arr.every(function (s) {
    return s.toLowerCase() === s;
})); // false, 因为不是每个元素都全部是小写

find

find()方法用于查找符合条件的第一个元素，如果找到了，返回这个元素，否则，返回undefined：

'use strict';
----
var arr = ['Apple', 'pear', 'orange'];
console.log(arr.find(function (s) {
    return s.toLowerCase() === s;
})); // 'pear', 因为pear全部是小写

console.log(arr.find(function (s) {
    return s.toUpperCase() === s;
})); // undefined, 因为没有全部是大写的元素

findIndex

findIndex()和find()类似，也是查找符合条件的第一个元素，不同之处在于findIndex()会返回这个元素的索
引，如果没有找到，返回-1：

'use strict';
----
var arr = ['Apple', 'pear', 'orange'];
console.log(arr.findIndex(function (s) {
    return s.toLowerCase() === s;
})); // 1, 因为'pear'的索引是1

console.log(arr.findIndex(function (s) {
    return s.toUpperCase() === s;
})); // -1

forEach

forEach()和map()类似，它也把每个元素依次作用于传入的函数，但不会返回新的数组。forEach()常用于遍
历数组，因此，传入的函数不需要返回值：

'use strict';
----
var arr = ['Apple', 'pear', 'orange'];
arr.forEach(console.log); // 依次打印每个元素

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

