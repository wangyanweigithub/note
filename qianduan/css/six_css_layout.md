# 实现三栏布局的六种方式
## 一.圣杯布局
1. 圣杯布局是指布局从上到下分为header、container、footer，然后
container部分定为三栏布局。

2. 基础HTML：　　
```
<body>
    <div class="container">
		<!-- 优先渲染 -->
		<div class="center">
            center
        </div>
		<div class="left">
            left
        </div>
        <div class="right">
            right
		</div>
    </div>
</body>
```

3. 基础CSS：

```
 .container {
     overflow: hidden;
 }
 .container > div {
     position: relative;
     float: left;
     height: 100px;
 }
 .center {
     width: 100%;
     background-color: red;
 }
 .left {
     width: 200px;
     background-color: green;
 }
 .right {
     width: 200px;
     background-color: blue;
 }
```

4. 对于container，给他设置一个overflow:hidden使其成为一个BFC（块级格
式化上下文）。BFC的作用: 

	1.消除Margin Collapse

	2.容纳浮动元素

	3.阻止文本换行

5. 使三栏浮动，并相对定位，给左右两个容器设置200px的宽度，中间的容器
设置100%的宽度。此时left和right被相对于父元素container宽度的100%的
center挤到下面。

6. 步骤：

	1.发left放在center上方：由于浮动的原因，给left设置margin-left:100%
	即可使左侧栏浮动到center上方，并位于center左侧之上。

	2.同样设置right margin-left:-200px,这里的长度等于right的长度。

	3.这时center的两侧被left和right覆盖了，因此给container设置padding
	：0 200px

	4.由于left和right也同时往中间缩，因为给left和right分别设置
	left:-200px;right:-200px,往两侧分别偏移自身的宽度去覆盖掉
	contaniner使用padding后的空白位置。

7. 这时，圣杯布局就完成了，但是在拖到很小的时候布局会乱，一下是最终样
式：

```
.container {
  overflow: hidden;
  padding:200px;
}
.container > div {
  position: relative;
  float: left;
  height: 100px;
}
.center {
  width: 100%;
  background-color: red;
}
.left {
  width: 200px;
  background-color: green;
  margin-left: -100%;
  left: -200px;
}
.right {
  width: 200px;
  background-color: blue;
  margin-left: -200px;
  right: -200px;
}
```
## 二.双飞翼布局
1. 这种布局方式同样分为header、container、footer。圣杯布局的缺陷在于
center是在container的padding中的，因此宽度小的时候会出现混乱。

2. 双飞翼布局给center部分报过了一个main，通过设置margin主动的把页面撑
开。

3. 基础HTML：

```
<div class="container">
  <!-- 优先渲染 -->
  <div class="center">
    <div class="main">
      center
    </div>
  </div>
  <div class="left">
    left
  </div>
  <div class="right">
    right
  </div>
</div>
```

4. 步骤1和步骤2同圣杯布局

5. 区别：

	1. 第三步：给main设置margin：0 200px，同时设置overflow：hidden，使其成为一个BFC。

	2. 这时窗口宽度过小时就不会出现混乱的情况了，关键点在于内容部分是包裹在main中。

6. 最终样式如下：

```
.container {
  overflow: hidden;
}
.container > div {
  position: relative;
  float: left;
  height: 100px;
}
.center {
  width: 100%;
  background-color: red;
}
.left {
  width: 200px;
  background-color: green;
  margin-left: -100%;
}
.right {
  width: 200px;
  background-color: blue;
  margin-left: -200px;
}
.main {
  height: 100%;
  margin:200px;
  background-color: rosybrown;
  overflow: hidden;
}
```

## 3.Flex布局
1. Flex布局是由CSS3提供的一种方便的布局方式。

2. 基础HTML：同圣杯布局

3. 步骤：

	1. 给container设置为一个flex容器：display：flex

	2. center的宽度设置为width：100%，left和right设置为200px

	3. 为了不让left和right被center设置的100%压缩，给left和right设置
	flex-shrink：0（定义项目的缩小比例，默认为1，如果空间不足则项目缩
	小，如果有一项为0，其他为1，当空间不足时，前者不缩小）。

	4. 使用order属性给三个部分的DOM结构进行排序：left：order：1，
	center:order:2,right:order:3

4. flex布局是一种极其灵活的布局方式，最终样式如下：（flex存在浏览器兼容性）

```
.container {
  display: flex;
}
.center {
  background-color: red;
  width: 100%;
  order: 2;
}
.left {
  background-color: green;
  width: 200px;
  flex-shrink: 0;
  order: 1;
}
.right {
  background-color: blue;
  width: 200px;
  flex-shrink: 0;
  order: 3;
}
```

## 4.绝对定位布局
1. 基础HTML和圣杯布局一样。

2. 步骤：

	1.给container设置position：relative和overflow：hidden，因为绝对定
	位的元素的参照物为第一个position不为static的祖先元素。

	2.left向左浮动，right向右浮动。

	3.center使用绝对定位，通过设置left和right把两边撑开。

	4.center设置top：0，bottom:0使其高度撑开。

3. css:
```
.center {
    position: absolute;
    left: 200px;
    right: 200px;
    top: 0;
    bottom: 0;
}
```

4. 这种方式的缺点是依赖于left和right的高度，如果两边栏的高度不够，中
间的内容区域的高度也会被压缩。

## 5.table-cell 布局
1. 表格布局的好处是能使三栏的高度统一。

2. 基础HTML：

```
<body>
    <div class="container">
        <div class="left">
            left
        </div>
        <!-- 这时的center要放在中间 -->
        <div class="center">
            center
        </div>
        <div class="right">
            right
        </div>
    </div>
</body>
```

3. 步骤：

	1. 给三栏都设置表格单元：display：table-cell

	2. left和right width:200px , center width:100%

	3. 这时left和right都被到两边去了，因此要分别设置min-widht：200px 确
	保不会被挤压。

4. 最终样式：

```
  .container {
        overflow: hidden;
        position: relative;
    }
    .container > div {
        display: table-cell;
        height: 100%;
    }
    .center {
        margin:200px;
        width: 100%;
        background: red;
    }
    .left {
        width: 200px;
        min-width: 200px;
        background-color: green;
    }
    .right {
        width: 200px;
        min-width: 200px;
        background-color: blue;
    }
```

这种布局方式能使得三栏的高度是统一的，但不能使center放在最前面得到
最先渲染。

## 6.网格布局
1. 网格布局可能是最强大的布局方式了，使用起来极其方便，但目前而言，兼
容性不好。网格布局，可以将页面分割成过个区域，或者用来定义内部元素
的大小，位置，图层关系。

2. 基础HTML：

```
<body>
    <div class="container">
        <div class="left">
            left
        </div>
        <!-- 这时的center要放在中间 -->
        <div class="center">
            center
        </div>
        <div class="right">
            right
        </div>
    </div>
</body>
```

3. 步骤：

	1.给container设置为display：grid

	2.设置三栏的高度：grid-template-rows：100px

	3.设置三栏的宽度，中间自适应，两边固定：grid-template-columns：
	200px auto 200px；

4. css
```
.container {
    display: grid;
    width: 100%;
    grid-template-rows: 100px;
    grid-template-columns: 200px auto 200px;
}
```
仅这四条样式命令就能完成三栏布局，可见网格布局之强大。

## 总结：
1. 圣杯布局、双飞翼布局、flex布局的center高度不受两边影响，取决于其
自身内容所占高度。

2. 绝对布局center高度取决于两侧的高度(因为是通过设置top：0，
bottom:0 把高度撑开的)

3. table-cell布局能让三栏高度统一，但是不能优先渲染center

4. 网格布局也不能优先渲染center，但布局简单，存在兼容性问题。

5. flex布局也存在兼容性问题。

6. 圣杯布局在页面缩小时会出现错乱问题。
