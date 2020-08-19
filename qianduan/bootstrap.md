1.<head>里面基本信息：
   <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <title>Bootstrap 101 Template</title>

    <link href="../css/bootstrap.min.css" rel="stylesheet">
    <script src="../script/jquery.min.js"></script>
    <script src="../js/bootstrap.min.js"></script> 

2.Bootstrap 项目的开头应该使用<!DOCTYPE html>

3.为了让 Bootstrap 开发的网站对移动设备友好，确保适当的绘制和触屏缩放，需要在网页的 head 之中添加 viewport meta 标签，如下所示：
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    
    注释：
        1> width 属性控制设备的宽度。假设您的网站将被带有不同屏幕分辨率的设备浏览，那么将它设置为 device-width 可以确保它能正确呈现在不同设备上
        2> initial-scale=1.0 确保网页加载时，以 1:1 的比例呈现，不会有任何的缩放。
        3> 在移动设备浏览器上，通过为 viewport meta 标签添加 user-scalable=no 可以禁用其缩放（zooming）功能。
通常情况下，maximum-scale=1.0 与 user-scalable=no 一起使用。这样禁用缩放功能后，用户只能滚动屏幕，就能让您的网站看上去更像原生应用的感觉。

4.响应式图像
    <img src="..." class="img-responsive" alt="响应式图像">
    在下面的代码中，可以看到img-responsive class 为图像赋予了 max-width: 100%; 和 height: auto; 属性，可以让图像按比例缩放，不超过其父元素的尺寸
    .img-responsive {
        display: inline-block;
        height: auto;
        max-width: 100%;
        }

5.这表明相关的图像呈现为 inline-block。当您把元素的 display 属性设置为 inline-block，元素相对于它周围的内容以内联形式呈现，但与内联不同的是，这种情况下我们可以设置宽度和高度。
设置 height:auto，相关元素的高度取决于浏览器。
设置 max-width 为 100% 会重写任何通过 width 属性指定的宽度。这让图片对响应式布局的支持更友好。

6.body
body {
    font-family: "Helvetica Neue", Helvetica, Arial, sans-serif;
    font-size: 14px;
    line-height: 1.428571429;
    color: #333333;
    background-color: #ffffff;
}
第一条规则设置 body 的默认字体样式为 "Helvetica Neue", Helvetica, Arial, sans-serif。
第二条规则设置文本的默认字体大小为 14 像素。
第三条规则设置默认的行高度为 1.428571429。
第四条规则设置默认的文本颜色为 #333333。
最后一条规则设置默认的背景颜色为白色

7.HTML 的默认强调标签 <small>（设置文本为父文本大小的 85%）、<strong>（设置文本为更粗的文本）、<em>（设置文本为斜体）。

8.锚元素使用disabled class只会改变<a>的外观，不会改变它的功能，需使用js来改变它的功能。

9.您可以在 <a>、<button> 或 <input> 元素上使用按钮 class。但是建议您在 <button> 元素上使用按钮 class，避免跨浏览器的不一致性问题。

10.img
    .img-rounded: 添加 border-radius:6px 来获得图片圆角。
    .img-circle:添加 border-radius:500px 来让整个图片变成圆形。
    .img-thumbnail:添加一些内边距（padding）和一个灰色的边框。
    .img-responsive:图片响应式 (将很好地扩展到父元素)

    通过在 <img> 标签添加 .img-responsive 类来让图片支持响应式设计。 图片将很好地扩展到父元素。
    .img-responsive 类将 max-width: 100%; 和 height: auto; 样式应用在图片上：

二、

11.布局容器
    Bootstrap 需要为页面内容和栅格系统包裹一个 .container 容器。我们提供了两个作此用处的类。注意，由于 padding 等属性的原因，这两种 容器类不能互相嵌套

    1>.container 类用于固定宽度并支持响应式布局的容器。
    2>.container-fluid 类用于 100% 宽度，占据全部视口（viewport）的容器。

12.栅格系统
    Bootstrap 提供了一套响应式、移动设备优先的流式栅格系统，随着屏幕或视口（viewport）尺寸的增加，系统会自动分为最多12列。它包含了易于使用的预定义类，还有强大的mixin 用于生成更具语义的布局。

    1>.“行（row）”必须包含在 .container （固定宽度）或 .container-fluid （100% 宽度）中，以便为其赋予合适的排列（aligment）和内补（padding）。
    2>.通过“行（row）”在水平方向创建一组“列（column）”。
    3>.你的内容应当放置于“列（column）”内，并且，只有“列（column）”可以作为行（row）”的直接子元素。
    4>.类似 .row 和 .col-xs-4 这种预定义的类，可以用来快速创建栅格布局。Bootstrap 源码中定义的 mixin 也可以用来创建语义化的布局。    5>.通过为“列（column）”设置 padding 属性，从而创建列与列之间的间隔（gutter）。通过为 .row 元素设置负值 margin 从而抵消掉为 .container 元素设置的 padding，也就间接为“行（row）”所包含的“列（column）”抵消掉了padding。
    6>.负值的 margin就是下面的示例为什么是向外突出的原因。在栅格列中的内容排成一行。
    7>.栅格系统中的列是通过指定1到12的值来表示其跨越的范围。例如，三个等宽的列可以使用三个 .col-xs-4 来创建。
    8>.如果一“行（row）”中包含了的“列（column）”大于 12，多余的“列（column）”所在的元素将被作为一个整体另起一行排列。

13.流式布局：
    将最外面的布局元素 .container 修改为 .container-fluid，就可以将固定宽度的栅格布局转换为 100% 宽度的布局。

14.响应式列重置：
    即便有上面给出的四组栅格class，你也不免会碰到一些问题，例如，在某些阈值时，某些列可能会出现比别的列高的情况。为了克服这一问题，建议联合使用 .clearfix 和 响应式工具类。
    <div class="row">
        <div class="clearfix visible-xs-block"></div>
    </div>

15.嵌套列
    为了使用内置的栅格系统将内容再次嵌套，可以通过添加一个新的 .row 元素和一系列 .col-sm-* 元素到已经存在的 .col-sm-* 元素内。被嵌套的行（row）所包含的列（column）的个数不能超过12（其实，没有要求你必须占满12列）

16.列排序
    通过使用 .col-md-push-* 和 .col-md-pull-* 类就可以很容易的改变列（column）的顺序。

17.列偏移
    使用 .col-md-offset-* 类可以将列向右侧偏移。这些类实际是通过使用 * 选择器为当前元素增加了左侧的边距（margin）。例如，.col-md-offset-4 类将 .col-md-4 元素向右侧偏移了4个列（column）的宽度。

三、排版

18.标题 .small

19.页面主体
    Bootstrap 将全局 font-size 设置为 14px，line-height 设置为 1.428。这些属性直接赋予 <body> 元素和所有段落元素。另外，<p> （段落）元素还被设置了等于 1/2 行高（即 10px）的底部外边距（margin）。

20.中心内容：.lead

21
