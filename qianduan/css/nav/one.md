基本的垂直导航条

通过HTML创建一个无序列表

<div>
<ul class="nav">
        <li><a href="#">home</a></li>
        <li><a href="#">home</a></li>
        <li><a href="#">home</a></li>
        <li><a href="#">home</a></li>
        <li><a href="#">home</a></li>
        <li><a href="#">home</a></li>
</ul>
</div>


利用css处理样式

<style type="text/css">
        ul.nav{
                padding:0px;
                margin:0px;
                list-style-type:none;
                width:80px;
                background-color:#8bd400;
                border:1px solid #486b02;
        }

        /*IE6在列表项的上下添加了额外的空间，为了修复这个BUG，需要将列表上的display属性设置为inline*/
        ul.nav li{
                display: inline;
        }
        /*不对列表项应用样式 而是对其中包含的锚链接应用样式，由此提供更好的浏览器兼容性*/
        ul.nav a{
                display: block;
                color:#2B3f00;
                text-decoration:none;
                border-top:1px solid #e4ffd3;
                border-bottom:1px solid #486b02;
                background:url(img/arrow.gif) no-repeat 2px center;
                padding:5px 10px;
        }
        /*解决最后一个链接的底边框与列表的底边框形成双线的问题*/
        ul.nav {
                border:0;
        }
         ul.nav a:hover,ul nav a:focus,ul.nav .selected a{
                color:#e4ffd3;
                background-color:#6da203;

         }
</style>

结果

[Center]

创建简单的水平导航条

HTML代码

<div>
        <ol class="pagination">
        <!-- 如果要在标签中显示"<"或者">"等特殊字符，可以使用“&”开头，“;”结尾，中间加上字符对应的编码就可以啦 <对应<  >对应>-->
                <li><a href="#" rel="prev"> <<Prev</a></li>
                <li><a href="#" >1</a></li>
                <li class="selected">2</a></li>
                <li><a href="#" >3</a></li>
                <li><a href="#" >4</a></li>
                <li><a href="#" >5</a></li>
                <li><a href="#" rel="next">Next > ></a></li>
        </ol>
</div>


css代码

ol.pagination{
                margin:0px;
                padding:0px;
                list-style-type:none;
        }
        ol.pagination li{
                float:left;/*浮动列表项 使用外边距把他们分开*/
                margin-right:10px;
        }

        ol.pagination a,ol.pagination li.selected{
                display: block;
                padding:5px 10px;/*控制li中的元素与li之间的距离*/
                border:1px solid #ccc;
                text-decoration:none;
        }
        /*鼠标悬停、获得焦点时 改变背景和文字的颜色*/
        ol.pagination a:hover,ol.pagination a:focus,ol.pagination li.selected{
                background-color:blue;
                color:white;
        }

        ol.pagination a[rel="prev"],ol.pagination a[rel="next"]{
                border:none;
        }

[Center]

创建图形化导航栏

html代码

<div>
<ul class="nav">
        <li class="first"><a href="#">home</a></li>
        <li><a href="#">home</a></li>
        <li><a href="#">home</a></li>
        <li><a href="#">home</a></li>
        <li><a href="#">home</a></li>
        <li><a href="#">home</a></li>
</ul>
</div>

css代码

<style type="text/css">
        ul.nav{
                padding:0px;
                margin:0px;
                list-style-type:none;
                width:390px;
                /*background-color:#8bd400;*/
                background: #faa819 url("img/mainNavBg.gif") repeat-x;
                /*float:left;*/
                overflow: hidden;/*可以使用overflow:hidden;技术让父元素包含浮动子元素*/
        }

        ul.nav li{
                /*display: inline;*/
                float: left;/*当元素浮动 将不再占据文档流中的任何位置 父列表实际上没有内容 就会收缩 从而隐藏列表背景*/
        }

        ul.nav a{
                display:block;
                padding:0 10px;/*每个按钮的宽度由锚文本的尺寸决定 所以不设置显示的宽度 而是在每个锚链接的左右设置一定的内边距*/
                line-height: 2.2;
                text-decoration:none;
                color:#fff;
                background:url(img/divider.gif) repeat-y left top;
        }

        ul.nav .first a {
                background-image: none;/*去掉第一个链接不必要的分割线*/
        }
        ul.nav a:hover, ul.nav a:focus{
                color:#333;
        }

</style>


[Center]

“滑动门”标签页式导航

HTMl代码同上
css代码

<style type="text/css">
        ul.nav{
                padding:0px;
                margin:0px;
                list-style-type:none;
                width:500px;
                overflow: hidden;/*可以使用overflow:hidden;技术让父元素包含浮动子元素*/
        }

        ul.nav li{
                float: left;/*当元素浮动 将不再占据文档流中的任何位置 父列表实际上没有内容 就会收缩 从而隐藏列表背景*/
                background:url(img/tab-right.gif) no-repeat right top;/*将组成标签页的两个图像中比较大的图像作为背景图像应用于列表项*/
        }

        ul.nav a{
                display:block;
                padding:0 10px;/*每个按钮的宽度由锚文本的尺寸决定 所以不设置显示的宽度 而是在每个锚链接的左右设置一定的内边距*/
                line-height: 2.2;
                text-decoration:none;
                color:#fff;
                background:url(img/tab-left.gif) no-repeat left top;/*将标签页的左边图像作为背景应用于锚并且左对齐 。当标签页改变尺寸时，该图像总是对准左边，盖在大图像上面，盖住左边的硬边缘*/
        }

        ul.nav a:hover, ul.nav a:focus{
                color:#333;
        }

</style>


运行结果
[Center]

下拉菜单

html代码

<div>
<ul class="nav">
        <li class="first"><a href="#">home</a></li>
        <li><a href="#">home</a>
        <ul>
                <li><a href="#">meau</li>
                <li><a href="#">meau</li>
                <li><a href="#">meau</li>
                <li><a href="#">meau</li>
        </ul>
        </li>
        <li><a href="#">home</a></li>
        <li><a href="#">home </a></li>
        <li><a href="#">home</a></li>
        <li><a href="#">home</a></li>
</ul>
</div>

css代码

<style type="text/css">
        ul.nav,ul.nav ul{
                padding:0px;
                margin:0px;
                list-style-type:none;
                width:492px;
                background-color: #8bd400;
                float:left;

        }

        ul.nav li{
                float: left;/*当元素浮动 将不再占据文档流中的任何位置 父列表实际上没有内容 就会收缩 从而隐藏列表背景*/
                width:80px;
                border:1px solid #fff;
        }

        ul.nav a{
                display:block;
                padding:0 10px;/*每个按钮的宽度由锚文本的尺寸决定 所以不设置显示的宽度 而是在每个锚链接的左右设置一定的内边距*/
                line-height: 2.2;
                text-decoration:none;
                color:#fff;

        }

        ul.nav a:hover, ul.nav a:focus{
                color:#333;
        }
        ul.nav li ul{
                width:80px;
                position:absolute;
                display: none;
                margin-top:2px;

        }
        ul.nav li:hover ul{
                display:block;
        }
</style>

运行结果
当鼠标没有悬停在第二个导航框的时候
[Center]
当鼠标悬停在第二个导航框的时候
[Center]
css3制作动态导航菜单

        .nav{
                  width:560px;
                  height: 50px;
                  font:bold 0/50px Arial;
                  text-align:center;
                  margin:40px auto 0;
              background: #f65f57;
                  /*制作圆*/

          /*制作导航立体风格*/

                }
                .nav a{
                  display: inline-block;
                  -webkit-transition: all 0.2s ease-in;
                  -moz-transition: all 0.2s ease-in;
                  -o-transition: all 0.2s ease-in;
                  -ms-transition: all 0.2s ease-in;
                  transition: all 0.2s ease-in;
                }
                .nav a:hover{
                  -webkit-transform:rotate(10deg);
                  -moz-transform:rotate(10deg);
                  -o-transform:rotate(10deg);
                  -ms-transform:rotate(10deg);
                  transform:rotate(10deg);
                }

                .nav li{
                  position:relative;
                  display:inline-block;
                  padding:0 16px;
                  font-size: 13px;
                  text-shadow:1px 2px 4px rgba(0,0,0,.5);
                  list-style: none outside none;
                }
                /*使用伪元素制作导航列表项分隔线*/

        /*删除第一项和最后一项导航分隔线*/

                .nav a,
                .nav a:hover{
                  color:#fff;
                  text-decoration: none;
                }

运行结果
[Center]
当鼠标放在连接上时  字体会向右倾斜十度
