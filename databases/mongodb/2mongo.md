[3_qq_42039]
一个半路出家的人 CSDN认证博客专家 CSDN认证企业博客
码龄2年

60
    原创

24
    粉丝

56
    获赞

5
    评论

11万+
    访问

1622
    积分

126
    收藏

3万+
    周排名

5万+
    总排名

[blog5]
    等级

[chizhiyihe]
[chizhiyihe]
持之以恒
授予每个自然月内发布4篇或4篇以上原创或翻译IT博文的用户。不积跬步无以至千里，不积小流无以成江海，程序人生的精彩需要坚持不懈
地积累！
[qixiebiaob]
[qixiebiaob]
勤写标兵Lv1
授予每个自然周发布1篇到3篇原创IT博文的用户。本勋章将于次周周三上午根据用户上周的博文发布情况由系统自动颁发。
TA的主页
私信
关注
[                    ] [csdn-sou]

最新文章

  • webpack4,vue开发环境,生产环境打包配置
  • python小知识点
  • Python-Tkinter几何布局管理（转）
  • python + flask + sqlite3做一个小博客
  • Python求圆周率(蒙特卡洛方法)

分类专栏

  • [resize] python 3篇
  • [2019092715] 生活小记
  • [2019092715] 学习日记 22篇
  • [2019092715] c++头文件笔记 1篇
  • [2019092715] python 4篇
  • [2019091814] 前端Html 28篇
  • [2019092715] Html 6篇
  • [2019092715] css 13篇
  • [2019092715] javaS 21篇
  • [2019092715] DOM 6篇
  • [2019091814] less 1篇
  • [2019091813] 移动端 3篇
  • [2019092715] vue 3篇
  • [2019091814] git 3篇
  • [2019091814] gitHub 1篇
  • [2019091814] ES6 1篇
  • [2019091814] 加密安全 2篇
  • [2019091814] 数据库 1篇
  • [2019092715] webpack 2篇
  • [2019092715] flask 1篇
  • [2019092715] sqlite 1篇
  • [2019092715] python小项目 1篇

[arrowDownW]

归档

2019
11月 1篇
8月 3篇
7月 1篇
6月 1篇
5月 3篇
4月 3篇
2018
12月 5篇
11月 7篇
10月 10篇
9月 15篇
8月 17篇
7月 9篇
6月 8篇
5月 4篇

热门文章

  • CSS3 - 设置渐变颜色背景，线性/放射性/循环 [readCountW]25974
  • meta name="viewport" content="width=device-width,initial-scale=1.0" 究竟什么意思 [readCountW]19374
  • MongoDB命令大全 [readCountW]7464
  • css的position属性 [readCountW]7432
  • 用hover操纵同级元素 [readCountW]5041

最新评论

  • 虚函数

    bjyh345：第12行代码写错了，应该是 class Base2:public Base1 { 吧

  • CSS3 - 设置渐变颜色背景，线...

    lianflower：真的棒！

  • Git的配置,上传项目到gitHu...

    github_37952340：很好，写的很详细。我看懂了[2]

  • 用hover操纵同级元素

    calvities_Man：有点表达不清应该说，我把后面的td设为类名设为td2，第一个为td1，然后我打的是:.td1:hover+.td2
    {background-color:red;}出现的结果为只有第二个为红色。是不是说只能改变最近的盒子元素？

  • 用hover操纵同级元素

    calvities_Man：可以解释一下紧邻和非紧邻吗？是不是只能影响一个元素？因为我做了一个表格，想要的效果是把鼠标放在第一个格
    子的时候左边的格子全部变另一种颜色，我做了之后只有一个变了感觉有点奇怪，我吧剩下的td的类名都设成一样的了。

[INS::INS]

目录

MongoDB命令大全

[reprint]
一个半路出家的人 2019-05-16 15:41:55 [articleRea] 7520 [collect] [tobarColle] 收藏 9
分类专栏：数据库
最后发布:2019-05-16 15:41:55首发:2019-05-16 15:41:55

    一、MongoDB简介

简单介绍一下MongoDB：
传统的关系数据库（如mysql）一般由数据库（database）、表（table）、记录（record）三个层次概念组成，
MongoDB是由数据库（database）、集合（collection）、文档对象（document）三个层次组成。
MongoDB稳定
MongoDB支持索引，索引放在内存中，能够提升随机读写的性能。如果索引不能完全放在内存，一旦出现随机读写比较高的时候，就会频繁
地进行磁盘交换，MongoDB的性能就会急剧下降
MongoDB占用的空间很大，因为它属于典型空间换时间原则的类型。那么它的磁盘空间比普通数据库会浪费一些

    二、MongoDB数据库操作

查看mongo帮助文档

mongo -h

    1

2、登陆

mongo -umiaobt -p --authenticationDatabase
mongo -umiaobt -p331sharp --authenticationDatabase

    1
    2

(以下操作需要登陆数据库才能操作)

// 库操作
3、查询所有库

show dbs;  

    1

4、查询库中的连接

show collecitons;

    1

5、创建数据库/切换数据库

use test1;

    1

如果没有test1这个库，会创建

6、删除数据库

db.dropDatabase();

    1

7、获取数据库名称

db.getName();

    1

8、获取数据库状态

db.stats();

    1

9、当前db版本

db.version();  

    1

10、查看当前db的链接机器地址

db.getMongo();

    1

11、从指定主机上克隆数据库

db.cloneDatabase("127.0.0.1");

    1

12、从指定的机器上复制指定数据库数据到某个数据库

db.copyDatabase("yhb", "test1", "127.0.0.1");  

    1

13、修复数据库

db.repairDatabase();  

    1

在MongoDB中频繁的进行数据增删改时，如果记录变了，例如数据大小发生了变化，这时候容易产生一些数据碎片，出现碎片引发的结果，
一个是索引会出现性能问题，另外一个就是在一定的时间后，所占空间会莫明其妙地增大，所以要定期把数据库做修复，定期重新做索引，
这样会提升MongoDB的稳定性和效率

    三、MongoDB集合操作

1、创建一个聚集集合（table）

//指定数据库大小size，最大存放100个文档，满了，就会mongodb 会删除旧文档。
db.createCollection("human",{"size":1024,capped:true,max:100});
db.createCollection("people");

    1
    2
    3

2、查看集合状态

db.people.stats();

    1

3、获取指定集合

db.getCollection("human");  

    1

4、获取当前db中的所有集合

db.getCollectionNames();

    1

和show collections类似

5、显示当前db所有聚集索引的状态

db.printCollectionStats();  

    1

    四、MongoBD用户操作

1、添加一个用户

db.createUser({user:"zs",pwd:"111",roles:["read"]})

    1

添加用户、设置密码、是否只读

2、数据库认证、安全模式
db.auth(“zs”, “111”);

3、显示当前所有用户，角色

show people;
show roles;

4、删除用户

db.removeUser("zs");  

    1

五、聚集集合查询

1、查询所有记录

    引用块内容

db.people.find();  
相当于：select* from people;

    1
    2

2、查询去掉后的当前聚集集合中的某列的重复数据

db.people.distinct("name");  
相当于：select distict name from people;

    1
    2

3、查询age = 18的记录

db.people.find({"age": 18});  
相当于： select * from people where age = 18;

    1
    2

4、查询age > 18的记录

db.people.find({age: {$gt: 18}});  
相当于：select * from people where age >18;

    1
    2

5、查询age < 18的记录

db.people.find({age: {$lt: 18}});  
相当于：select * from people where age <18;

    1
    2

6、查询age >= 18的记录

db.people.find({age: {$gte: 18}});  
相当于：select * from people where age >= 18;

    1
    2

7、查询age <= 18的记录

db.people.find({age: {$lte: 18}});

    1

8、查询age >= 23 并且 age <= 26

db.people.find({age: {$gte: 23, $lte: 26}});  

    1

9、查询name中包含 mongo的数据

db.people.find({name: /mongo/});  
相当于：select * from people where name like '%mongo%';

    1
    2

10、查询name中以mongo开头的

db.people.find({name: /^mongo/});  

    1

相当于：select * from people where name like ‘mongo%’;

11、查询指定列name、age数据

db.people.find({}, {name: 1, age: 1});  

    1

相当于：select name, age from people;
当然name也可以用true或false,当用ture的情况下河name:1效果一样，如果用false就是排除name，显示name以外的列信息。

12、查询指定列name、age数据, age > 18

db.people.find({age: {$gt: 18}}, {name: 1, age: 1});  
相当于：select name, age from people where age >18;

    1
    2

13、按照年龄排序

升序：db.people.find().sort({age: 1});  
降序：db.people.find().sort({age: -1});  

    1
    2

14、查询name = zhangsan, age = 18的数据

db.people.find({name: 'zhangsan', age: 18});  
相当于：select * from people where name = 'zhangsan' and age = '18';

    1
    2

15、查询前5条数据

db.people.find().limit(5);  
相当于：select * from people Limit 5;

    1
    2

16、查询10条以后的数据

db.people.find().skip(10);  
相当于：select * from people where id not in (select id from people limit 10);

    1
    2

17、查询在5-10之间的数据

db.people.find().limit(10).skip(5);  
可用于分页，limit是pageSize，skip是第几页*pageSize

    1
    2

18、or与查询

db.people.find({$or: [{age: 18}, {age: 18}]});  
相当于：select * from people where age = 18 or age = 18;

    1
    2

19、查询第一条数据

db.people.findOne();  
相当于：select * from people limit 1;
db.people.find().limit(1);

    1
    2
    3

20、查询某个结果集的记录条数

db.people.find({age: {$gte: 18}}).count();  
相当于：select count(*) from people where age >= 20;

    1
    2

21、求总数

db.people.find({sex: {$exists: true}}).count();
相当于：select count(sex) from people;

    六、集合的索引

1、创建索引

db.people.ensureIndex({name: 1});  
db.people.ensureIndex({name: 1, ts: -1});  //联合索引
db.people.ensureIndex({"name":1},{"unique":true}); //唯一索引

    1
    2
    3

2、查询当前聚集集合所有索引

db.people.getIndexes();  

    1

3、查看总索引记录大小

db.people.totalIndexSize();  

    1

4、读取当前集合的所有index信息

db.people.reIndex();  

    1

5、删除指定索引

db.people.dropIndex("name_1");

    1

6、删除所有索引索引

db.people.dropIndexes();  

    1

    七、修改、添加、删除集合数据

1、添加

db.people.save({name: 'zhangsan', age: 18, sex: true});  

    1

添加的数据的数据列，没有固定，根据添加的数据为准

2、修改

db.people.update({age: 18}, {$set: {name: 'changeName'}}, false, true);  
相当于：update people set name = 'changeName' where age = 18;  
db.people.update({name: 'zhangs'}, {$inc: {age: 12}}, false, true);  
相当于：update people set age = age + 12 where name = 'zhangs';  
db.people.update({name: 'zhangs'}, {$inc: {age: 12}, $set: {name: 'hoho'}}, false, true);  
相当于：update people set age = age + 12, name = 'hoho' where name = 'zhangs';  

    1
    2
    3
    4
    5
    6

3、删除

db.people.remove({age: 12});
---------------------
作者：Tonny__
来源：CSDN
原文：https://blog.csdn.net/zc375039901/article/details/79951047
版权声明：本文为博主原创文章，转载请附上博文链接！

展开阅读全文

  • [tobarThumb] [thumbUpWhi] 点赞 1
  • [commentWhi] 评论
  • [shareWhite] 分享
    x

    海报分享

    扫一扫，分享海报

  • [collectWhi] [tobarColle] 收藏 9
  • [mobileWhit] 手机看
   
    分享到微信朋友圈

    x
   
    扫一扫，手机阅读

  • 文章举报

收起全文 [CommentArr]

liqi_q的博客

01-17 [readCountW] 3万+

mongodb常用数据查询筛选命令行操作

下面是我整理了关于mongodb数据表命令行操作数据的插入、修改、删除、求和、最大值、最小值、平均值、排序、分片查询、in和or等用
法的查询在查询指定数据库表的情况下我们可以使用 show collections查询全部数据可以使用find()方法查询查询数据库表中的数据条数
，带查询条件统计数量：db.sidcust.count({"age":{$gte:23,$lte:28}}); 全部：db....
[anonymous-]
[commentFla] [                    ]
 
添加代码片

  • HTML/XML
  • objective-c
  • Ruby
  • PHP
  • C
  • C++
  • JavaScript
  • Python
  • Java
  • CSS
  • SQL
  • 其它

还能输入1000个字符 [取消回复] [发表评论]
[评论]

Riven的博客

06-13 [readCountW] 539

MongoDB常用命令

以吐槽表为例讲解MongoDB常用命令选择和创建数据库选择和创建数据库的语法格式：use 数据库名称如果数据库不存在则自动创建以下语
句创建spit数据库use spitdb插入与查询文档插入文档的语法格式：db.集合名称.insert(数据);我们这里可以插入以下测试数据：
db.spit.insert({content:"听说十次方课程很给力呀",userid:"10......

LeoJame的博客

03-24 [readCountW] 108

mongodb命令使用大全（常用命令）

mongodb命令使用大全（常用命令）数据库常用命令1、Help查看命令提示helpdb.help();db.yourColl.help();db.youColl.find().help();
rs.help();2、切换/创建数据库use yourDB; 当创建一个集合(table)的时候会自动创建当前数据库3、查询所有数据库show dbs;4、
删......

weixin_34308389的博客

05-25 [readCountW] 16

【转】Mongodb的基本使用总结

MongoDB基本使用成功启动MongoDB后，再打开一个命令行窗口输入mongo，就可以进行数据库的一些操作。输入help可以看到基本操作命令
：show dbs:显示数据库列表show collections：显示当前数据库中的集合（类似关系数据库中的表）show users：显示用户use <db name>
：切换当前数据库，这和MS-SQL里面的意思一样......

一只蛋糕糕的博客

07-19 [readCountW] 6377

MongoDB数据库命令行操作

参考文章https://blog.csdn.net/u013614451/article/details/48900867MongoDB数据库的下载：https://www.mongodb.com/
download-center?jmp=nav#community版本：mongodb-win32-x86_64-2008plus-ssl-3.6.6-signed.msi解压后的状态...

piaocoder

08-31 [readCountW] 2万+

MongoDB常用操作命令大全

如果你想创建一个“myTest”的数据库，先运行use myTest命令，之后就做一些操作（如：db.createCollection('user')）,这样就可以创建
一个名叫“myTest”的数据库。一、数据库常用命令1、Help查看命令提示复制代码代码如下:helpdb.help();db.yourColl.help();
db.youColl.find().he...

孙朝阳的博客

04-05 [readCountW] 9241

Mongodb 对内嵌数组的增删改查操作

先做一个初始化，设置一个User类，其初始数据如下：{ arr: [ 1, 2 ], _id: 5ac5ee12a79131259413c40f, name: 'scy', __v: 0 }每次
以初始数据为基，进行操作。 1、向内嵌数组添加数据使用操作符 $push，向数组末尾添加数据，可重复//第一个参数是匹配条件第二个参
数是具体操作User.upda......

_Somethingwill

11-12 [readCountW] 4031

MongoDB常用操作命令

原文链接：点击打开链接如果你想创建一个“myTest”的数据库，先运行use myTest命令，之后就做一些操作（如：db.createCollection
('user')）,这样就可以创建一个名叫“myTest”的数据库。一、数据库常用命令1、Help查看命令提示[sql] view plain copyhelp...

一个人的博客

03-27 [readCountW] 45

mongodb数据库常用命令整理（包括集群搭建）

对mongodb的常用操作批量插入单个字段db.getCollection(‘projectInfo’).find().forEach(function(item){db.getCollection
(‘projectInfo’).update({"_id":item._id,“updateTime”:{KaTeX parse error: Expected 'EOF', got '}' a...

懂哥的博客

11-11 [readCountW] 505

Mongodb 常用命令整理

一、超级用户相关：进入数据库admin use admin 增加或修改用户密码 db.addUser(‘name’,’pwd’) 查看用户列表 db.system.users.find
() 用户认证 db.auth(‘name’,’pwd’) 删除用户 db.removeUser(‘name’) 查看所有用户 show users ...

vbirdbest的博客

07-24 [readCountW] 3万+

MongoDB基础篇：MongoDB Shell命令大全

要想能熟练操作MongoDB，首先先要熟练使用MongoDBShell，再次要熟练使用驱动mongodb-java-driver,最后是spring-data-mongodb的使用
在使用Shell中的方法时，注意很多方法都是有重载函数的一：启动服务和连接服务1、mongod用于启动MongoDB服务MongoDB的数据时存储在
磁盘上的，而不是存储在内存中的，所以在启动MongoDB服务...

坦GA的博客

05-23 [readCountW] 4488

Mongodb命令行基本命令使用大全

原文地址：https://blog.csdn.net/qq_27093465/article/details/54601598(先声明下，下面的命令，我没有都测试过，这是我转载的，
以备后用，所以喷子且留情，不然小心我三十米的大刀！！！想想我都怕！！！)成功启动MongoDB后，再打开一个命令行窗口输入mongo，
就可以进行数据库的一些操作。输入help可以看到基本操作命令：show dbs:显示数......

CSDN资讯

01-18 [readCountW] 6061

MongoDB 凉了？

英国《卫报》正式告别MongoDB，完成了向PostgreSQL的迁移；RedHat移除MongoDB，Fedora、Debian亦在其列；再加上前不久刚因程...

广工最菜的琛

06-10 [readCountW] 327

MongoDB指令大全

声明：这是一份查看指令的文档而不是教程，对于一些不多用的操作不录入。文章目录一、系统操作1.1 启动服务器1.2 连接服务器1.3 安
装服务1.4 使用场景二、基本操作2.1 数据类型2.1.1 浮点数2.1.2 整数2.1.3 字符串2.1.4 正则表达式2.1.5 数组2.1.6 日期2.1.7 内嵌
文档2.1.8 ObjectId2.1.9二进制2.1.10 JS代码2.2 数据库操作2......

云深海阔专栏

06-10 [readCountW] 5243

MongoDB基本命令归纳

普及MongoDB一些知识默认监听端口号 27017/28017进程名 mongod mongo数据传输协议 TCP数据库目录 /data/db服务名 mongodb一、软件
安装1) 下载完安装包，并解压 tgz（以下演示的是 64 位 Linux上的安装）liuys@ubuntu:~$ sudo wget https://fastdl.mongodb.org/
linux/mongodb-linu......

lizhengyu891231的博客

02-28 [readCountW] 610

MongoDB基本命令用

转载自：https://www.cnblogs.com/wpjzh/p/5999363.htmlMongoDB基本命令用成功启动MongoDB后，再打开一个命令行窗口输入mongo，就
可以进行数据库的一些操作。输入help可以看到基本操作命令：show dbs:显示数据库列表 show collections：显示当前数据库中的集合（
类似关系数据库中的表） show......

weixin_30835933的博客

11-10 [readCountW] 95

MongoDB常用操作

一、查询find方法db.collection_name.find();查询所有的结果：select * from users;db.users.find();指定返回那些列（键）：select
name, skills from users;db.users.find({},{'name':1,'skills':1});补充说明...

盗情的博客

03-31 [readCountW] 2147

MongoDB

**&amp;amp;amp;gt;&amp;amp;amp;gt; ****~ ****&amp;amp;amp;gt;&amp;amp;amp;gt; **

Cowboy

08-02 [readCountW] 1万+

MongoDb 命令查询所有数据库列表

MongoDb 命令查询所有数据库列表CODE:&gt; show dbs如果想查看当前连接在哪个数据库下面，可以直接输入dbCODE:&gt; dbAdmin想切换
到test数据库下面CODE:&gt; use testswitched to db test&gt; dbTest想查看test下有哪些表或者叫collection，可以输入CODE:......
Java C语言 Python C++ C# Visual Basic .NET JavaScript PHP SQL Go语言 R语言 Assembly language Swift Ruby MATLAB PL/SQL Perl
Visual Basic Objective-C Delphi/Object Pascal Unity3D
[feedLoadin]

没有更多推荐了，返回首页

©️2019 CSDN 皮肤主题: 书香水墨设计师: CSDN官方博客

目录

[INS::INS]

分类专栏

  • [resize] python 3篇
  • [2019091814] 生活小记
  • [2019092715] 学习日记 22篇
  • [2019092715] c++头文件笔记 1篇
  • [2019092715] python 4篇
  • [2019092715] 前端Html 28篇
  • [2019091814] Html 6篇
  • [2019092715] css 13篇
  • [2019092715] javaS 21篇
  • [2019092715] DOM 6篇
  • [2019092715] less 1篇
  • [2019092715] 移动端 3篇
  • [2019091814] vue 3篇
  • [2019091814] git 3篇
  • [2019091814] gitHub 1篇
  • [2019091814] ES6 1篇
  • [2019092715] 加密安全 2篇
  • [2019091813] 数据库 1篇
  • [2019091813] webpack 2篇
  • [2019092715] flask 1篇
  • [2019091814] sqlite 1篇
  • [2019092715] python小项目 1篇

应支付0元
[pay-time-o] 点击重新获取
[wx][zfb][JD]扫码支付

支付成功即可阅读

[]
