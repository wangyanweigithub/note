## 图的声明

### digraph
1. 使用digraph graph_name可以声明一个图，而图的具体实现代码则由一对花括号包含起来。
```
digraph graph1 {
    //something
}
```

2. 当图中某些内容同属一个类别时，我们可以声明一个子图将其包含在一起。
使用**subgraph** cluster_subgraph_name可以在声明一个子图。

3. 注意：子图的名字必须以cluster开始，否则解析引擎无法进行识别
```
subgraph cluster_subgraph1 {
    // something
}
```

4. 下面我们来看一个具体例子：
```
digraph example1 {
    label = "this is a graph";
    a;b;

    subgraph cluster_sgh {
        label = "this is subgraph";
        bgcolor = greenyellow;
        c;d;
    }
}
```

4. 抛开那些还没讲到的语法。可以看到，这里布局引擎将子图里的c和d
结点聚合在了一起，这在某些场景下是非常有用的，
可以将逻辑上相近的内容放在一起显示。

## 结点和边的声明
1. Dot里结点的声明十分简单。键入结点名字node_name;即可声明一个结点，
同时，结点默认的显示内容就是结点名字。

2. 当结点内容里面有空格时，我们可以将结点名字用一对双引号包含起来，
解析引擎会直接取双引号里面的内容创建结点。

3. 使用符号->可以声明一条边。a -> b就代表一条由结点a连接到结点b的边。
结点可以连接到结点自身
```
a;
b;
a -> b;
```

4. 如果不需要对结点进行单独的属性设置（后面讲解），则可以直接声明边，
解析引擎会自动创建相应的结点。
```
a -> b;
```

5. 声明多条边的便捷语法：

```
a; b; c;
a -> b -> c -> c;
```

## 图的属性
1. 图的属性设置可以写在graph []里，或者直接写在外面：
```
digraph graph1 {
    bgcolor = red;
    graph [bgcolor = red];
}
```

2. 图的属性包括：

	1. bgcolor：设置图的背景颜色，可以使用red、blue、green等形式，
	或者使用"#rrggbb"形式。

	2. label：设置图的描述。label会继承到子图，子图里如果不想重复
	显示必须手动设置

	3. rankdir：设置图的方向，包括：TB（top to bottom）、
	BT（bottom to top）、LR（left to right）、
    RL（right to left）。

	4. rotate：设置图的旋转。如：rotate = 90便是逆时针旋转90度。

	5. ratio：设置图的长宽比，可以是一个浮点数，
	也可以是：fill、compress或者auto。

## 结点属性和边属性
1. 结点和边的属性设置非常简单，只需要在结点或者边的声明后面加上方括号
，然后在方括号里填写属性键值对即可。键值对之间使用逗号进行分割。

```
a [shape = egg, label = "this is node a"];
b [shape = circle, label = "this is node b"];
a -> b [style = dashed, label = "this is edge a to b"];
```

2. 结点的常用属性：

	1. shape：设置结点形状。包括：Mrecord（圆角矩形）、record（矩形）
	、circle（圆形）、box（矩形，和record略有区别，下面会讲到）
	、egg（蛋形）、doublecircle（双圆形）、plaintext（纯文本）、
    ellipse（椭圆，默认）。

	2. label：设置结点的显示内容，内容用双引号包含，
	可以使用转义字符。<font color=red>当结点内容!=结点名时使用</font>

	3. style：设置结点的样式。包括：filled(填充)、dotted（点状边框）、
	solid（普通边框）、dashed（虚线边框）、bold（边框加粗）、invis（隐形）。

	4. color：设置边框颜色。可以使用单词形式或者"#rrggbb"形式。

	5. fillcolor：设置填充颜色，仅style = filled时有效。

	6. width：设置结点宽度。

	7. height：设置结点高度。

	8. peripheries：设置结点边框个数。

	9. fontcolor：设置结点内容颜色。可以使用单词形式或者"#rrggbb"形式。

3. 需要重点强调的是：
	如果设置了style = filled，此时设置的color是代表整体颜色。
但是如果设置了fillcolor再设置color的话，color代表边框颜色。

4. 边的常用属性：

	1. style：设置边的形状。包括：solid（实线）、dashed（虚线）、
	dotted（点线）、bold（加粗）、invis（隐形）。

	2. label：设置边标签。内容用双引号包含，可以使用转义字符。

	3. color：设置边颜色。可以使用单词形式或者"#rrggbb"形式。

	4. arrowhead：设置结点箭头样式。包括：none、empty、odiamond等。

5. 使用node []和edge []可以分别设置结点和边的全局设置：
```
digraph graph1 {
    node [shape = egg];
    edge [style = dashed];

    a -> b;
}
全局设置
```

## 其他应用
1. 使dot支持中文
	Graphviz默认是不支持中文的，输入的中文在生成的图中显示为一个空方块。
如果想要让其支持中文，可以尝试以下方法：

	- 在命令行里指定-Nfontname=xxx.ttf，在gv文件中输入utf-8编码的汉字。

	- 给graph、node和edge均设置fontname = xxx.ttf然后设置label。

2. 使用record的label属性生成表格
	前面提到record和box有所区别。原因就是：设置为record和Mrecord的结点
的label属性可以很方便地生成单列的表格和UML图等。（类似于XAML里面的stack）

3. 用一对双引号+一对花括号包含起来的就是表格内容，不同的格子之间用符号 |
隔开，尖括号里的内容表示一个锚点（具体讲解见下文）。
```
example [ shape = record, label = "{<head>cell1 | cell2 | cell3}" ];
```

4. 也可以生成空格只保留锚点：
```
label="{<b1>|<b2>|<b3>}"
```

5. 使用html标签生成表格
	如果record生成的表格不符合预期，还可以使用html标签生成表格。只需要将结点的label属性
设置为相应的html代码（用一对尖括号包含）即可。
```
table1 [label=<
<table>
    <tr>
        <td port="one">1</td>
        <td>2</td>
    </tr>
    <tr>
        <td>3</td>
        <td>4</td>
    </tr>
</table>
>];
```

6. port属性可以为td增加一个锚点。

7. 表格锚点的应用
	1. cell的锚点可以让使用者在cell之间画线。
		引用cell的锚点的语法为：table: anchor_name

	2. 示例代码：
	```
	digraph example2 {
		node [shape = record];
		table1 [label = "{<head>cell1 | cell2 | cell3}"];
		table2 [label = "{<head>cell1 | cell2}"];

		table1: head -> table2: head;
	}
	```

8. 生成图形
	我们还可以巧妙地使用结点的某些属性来生成图形。如：
	circle [label="", shape="circle", width=0.5, fixedsize=true, style=filled, color=black];

	就生成了一个实心黑色圆形。

9. 命令行全局设置
	不仅可以使用代码里的全局设置，还可以在命令行里进行全局设置，这样做的好处就是可以根据不同要
求来生成图形。
```
dot -Grankdir=LR -Nshape="plaintext" -Earrowhead="odiamond" -Tpng example.dot -o example.png
```

	- Grankdir即 graph rankdir
	- Nshape即 node shape
	- Earrowhead即 edge arrowhead
	- 其他的属性均按照这种规则进行填写。

10. 添加注释
	Dot的注释使用//（单行）或者/**/（多行）。

11. 最后，关于graphviz结点的布局：
    - graphviz的节点出现在画布上的位置事实上是不确定的，依赖于所使用的布局算法，
	而不是在脚本中出现的位置，这可能使刚开始接触graphviz的开发人员有点不适应。
	
	- graphviz的强项在于自动布局，当图中的顶点和边的数目变得很多的时候，
	才能很好的体会这一特性的好处。

## note
1. 当node是record时, label加大括号是和rankdir方向一致,不加大括号时rankdir相反.
