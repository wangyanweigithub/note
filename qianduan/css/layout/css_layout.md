
<!-- vim-markdown-toc GFM -->

* [实现三栏布局的7种方法 14](#实现三栏布局的7种方法-14)
	* [2.左右浮动布局 71](#2左右浮动布局-71)
	* [3.圣杯布局 121](#3圣杯布局-121)
	* [4.双飞翼布局 180](#4双飞翼布局-180)
	* [5. Flex布局 240](#5-flex布局-240)
	* [6.Grid布局 293](#6grid布局-293)
	* [7.table-cell布局 341](#7table-cell布局-341)

<!-- vim-markdown-toc -->
# 实现三栏布局的7种方法 14
## 2.左右浮动布局 71

```
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>三栏布局——左右浮动布局</title>
  <style>
    * {
      margin: 0;
      padding: 0;
    }
    body {
      min-width: 550px;
    }
    .column {
      min-height: 100px;
    }
    .left {
      float: left;
      width: 200px;
      background: #ffbbff;
    }
    .center {
      margin: 0 150px 0 200px;
      background: #bfefff;
    }
    .right {
      float: right;
      width: 150px;
      background: #eeee00;
    }
  </style>
</head>
<body>
  <div class="container">
    <div class="column left">left</div>
    <div class="column right">right</div>
    <div class="column center">center</div>
  </div>
</body>
</html>
```

用这种方法实现的时候遇到了问题，刚开始是把center对应的div放在中间
位置的，结果左中右位置对了，但三栏始终不能显示在同一行上；最后发现
是自己对float属性的理解有偏差，将center放在最后就显示正确了。这种
方法的缺点是三栏高度不统一，center区域的内容要在最后渲染。

## 3.圣杯布局 121
1. 此处忽略header和footer，只关注container。

```
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>三栏布局——圣杯布局</title>
  <style>
    * {
      margin: 0;
      padding: 0;
    }
    body {
      min-width: 550px;
    }
    .container {
      padding: 0 150px 0 200px;
      overflow: hidden;
    }
    .column {
      position: relative;
      float: left;
      min-height: 100px;
    }
    .left {
      right: 200px;
      margin-left: -100%;
      width: 200px;
      background: #ffbbff;
    }
    .center {
      width: 100%;
      background: #bfefff;
    }
    .right {
      left: 150px;
      margin-left: -150px;
      width: 150px;
      background: #eeee00;
    }
  </style>
</head>
<body>
  <div class="container">
    <div class="column center">center</div>
    <div class="column left">left</div>
    <div class="column right">right</div>
  </div>
</body>
</html>
```

通过左浮动加相对定位实现，容易理解。缺点是三栏高度不统一，而且
center是在container的padding中，宽度小的时候会出现混乱，所以最好给
body设置一个最小宽度。

## 4.双飞翼布局 180

此处忽略header和footer，只关注container。

```
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>三栏布局——双飞翼布局</title>
  <style>
    * {
      margin: 0;
      padding: 0;
    }
    body {
      min-width: 550px;
    }
    .container {
      overflow: hidden;
    }
    .column {
      float: left;
      min-height: 100px;
    }
    .left {
      margin-left: -100%;
      width: 200px;
      background: #ffbbff;
    }
    .center {
      width: 100%;
    }
    .center-inner {
      margin: 0 150px 0 200px;
      min-height: 100px;
      background: #bfefff;
    }
    .right {
      margin-left: -150px;
      width: 150px;
      background: #eeee00;
    }
  </style>
</head>
<body>
  <div class="container">
    <div class="column center">
      <div class="center-inner">center</div>
    </div>
    <div class="column left">left</div>
    <div class="column right">right</div>
  </div>
</body>
</html>
```

单纯的通过左浮动配合margin属性实现，比较经典的三栏布局方式。缺点是
多了一层包裹center内容的代码，三栏高度不统一。

## 5. Flex布局 240

```
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>三栏布局——Flex布局</title>
  <style>
    * {
      margin: 0;
      padding: 0;
    }
    body {
      min-width: 550px;
    }
    .container {
      display: flex;
      justify-content: space-between;
    }
    .column {
      min-height: 100px;
    }
    .left {
      order: 1;
      width: 200px;
      background: #ffbbff;
    }
    .center {
      order: 2;
      flex: 1;
      background: #bfefff;
    }
    .right {
      order: 3;
      width: 150px;
      background: #eeee00;
    }
  </style>
</head>
<body>
  <div class="container">
    <div class="column center">center</div>
    <div class="column left">left</div>
    <div class="column right">right</div>
  </div>
</body>
</html>
```

实际高度会根据内容自适应，三栏高度统一。Flex布局在移动端比较常见，
布局灵活，兼容性也还可以，基本能满足大多数需求。

## 6.Grid布局 293

```
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>三栏布局——Grid布局</title>
  <style>
    * {
      margin: 0;
      padding: 0;
    }
    .container {
      display: grid;
      grid-template-columns: 200px auto 150px;
      width: 100%;
      min-height: 100px;
    }
    .left {
      grid-row: 1/2;
      grid-column: 1/2;
      background: #ffbbff;
    }
    .center {
      grid-row: 1/2;
      grid-column: 2/3;
      background: #bfefff;
    }
    .right {
      grid-row: 1/2;
      grid-column: 3/4;
      background: #eeee00;
    }
  </style>
</head>
<body>
  <div class="container">
    <div class="column left">left</div>
    <div class="column center">center</div>
    <div class="column right">right</div>
  </div>
</body>
</html>
```

实际高度会根据内容自适应，三栏高度统一。唯一的缺点就是兼容性不太好

## 7.table-cell布局 341

```
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>三栏布局——table-cell布局</title>
  <style>
    * {
      margin: 0;
      padding: 0;
    }
    .column {
      display: table-cell;
      height: 100px;
      min-height: 100px;
    }
    .left {
      width: 200px;
      min-width: 200px;
      background: #ffbbff;
    }
    .center {
      width: 100%;
      background: #bfefff;
    }
    .right {
      width: 150px;
      min-width: 150px;
      background: #eeee00;
    }
  </style>
</head>
<body>
  <div class="container">
    <div class="column left">left</div>
    <div class="column center">center</div>
    <div class="column right">right</div>
  </div>
</body>
</html>
```

左中右设置高度是为了使最小高度生效；实际高度根据内容自适应，三栏高
度统一。缺点是中间的center区域不能优先渲染。


我平时项目中使用Flex布局最多，当然具体使用哪种还得根据自己的需求和
喜好而定。最后附上一张效果图。
