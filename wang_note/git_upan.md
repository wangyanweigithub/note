[3_u0109285]
u010928583 CSDN认证博客专家 CSDN认证企业博客
码龄7年 [nocErtific]暂无认证

11
    原创

1
    粉丝

1
    获赞

0
    评论

6486
    访问

168
    积分

4
    收藏

39万+
    周排名

38万+
    总排名

[blog2]
    等级

TA的主页
私信
关注
[                    ] [csdn-sou]

最新文章

  • javascript学习记录-2：this和对象模型
  • javascript学习记录-1：作用域和闭包
  • VUE生命周期图解
  • CommonJS-AMD-CMD-UMD-ES6前端模块化
  • webpack多页面，动态生成模板并插入对应的js（代码分析）

分类专栏

  • [2019092715] 前端技术 6篇
  • [2019092715] SVG 3篇
  • [2019091814] VUE 1篇
  • [2019091814] webpack 2篇
  • [2019091814] JS模块化 2篇
  • [2019091814] Javascript 2篇
  • [2019092715] 小玩意 1篇

[arrowDownW]

归档

2019
1月 1篇
2018
12月 3篇
11月 3篇
10月 3篇
9月 1篇

热门文章

  • webpack多页面，动态生成模板并插入对应的js（代码分析） [readCountW]3183
  • 利用git,使U盘作为仓库 [readCountW]1032
  • 微信分享（JSSDK使用） [readCountW]752
  • commons-chunk-plugin 使用方法 [readCountW]354
  • svg viewbox和preserveAspectRatio理解 [readCountW]194

[INS::INS]

目录

利用git,使U盘作为仓库

[original]
u010928583 2019-01-15 17:51:12 [articleRea] 1037 [tobarColle] [tobarColle] 收藏
分类专栏：小玩意
最后发布:2019-01-15 17:51:12首发:2019-01-15 17:51:12
 
版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本
声明。
本文链接：https://blog.csdn.net/u010928583/article/details/86496734
版权

(前提条件：电脑本地有安装git。)

U盘作为仓库操作步骤

步骤1、首先U盘上创建一个repository 名称： fill_backup_repos

git init –bare  fill_backup_repos  //裸库，没有work目录

步骤2、电脑文件夹创建一个源项目名称：git_myfill

   git init git_myfill

   //放入需要同步管理的文件
   git add .    //添加文件、如果有不需要管理的文件，加入 .gitignore文件；

   git commit -m "initialized."   //提交到本地仓库

   git remote add usb F:/fill_backup_repos    //把u盘上的裸库fill_backup添加为远程仓库

   git push usb master

步骤3、在U盘上的fill_backup 裸仓库里头建立一个本地仓库

  git init  myfill //建一个本地仓库

   git remote add usb F:/fill_backup_repos     //把u盘上的裸库fill_backup_repos添加为远程仓库

   git pull myusb master   //完成代码同步

备注remote相关命令：
1、查看remote： git remote
2、删除remote: git remote remove

展开阅读全文

  • [tobarThumb] [tobarThumb] 点赞
  • [tobarComme] 评论
  • [tobarShare] 分享
    x

    海报分享

    扫一扫，分享海报

  • [tobarColle] [tobarColle] 收藏
  • [tobarMobil] 手机看
   
    分享到微信朋友圈

    x
   
    扫一扫，手机阅读

  • [lookMore]
      □ [tobarRewar] 打赏
      □ 文章举报
   
    打赏

        [3_u0109285]
    u010928583

        “你的鼓励将是我创作的最大动力”

    5C币 10C币 20C币 50C币 100C币 200C币

    确定

  • 关注

收起全文 [CommentArr]

Wzq的博客

07-09 [readCountW] 1万+

Git本地远程仓库的搭建(局域网内也可提交)

Git本地远程仓库的搭建(windows 7)你是否也这样想过？可以在自己的两台或者多台电脑中创建
一个仓库，其他电脑的文件或者代码都可以push到这台机器中的git仓库进行版本管理，相当于有
一个自己的git远程仓库。具体的功能可以这样理解： 1.功能需求1)创建本机仓库(比如D盘仓库)
里文件可见。2)本地仓库(C盘仓库)可关联本机仓库(D盘仓库)并提交。3)局域网......

peirenlei

08-16 [readCountW] 101

在U盘里建立git仓库

 1、windows下安装msysgit作为git的管理工具，下载地址：http://code.google.com/p/msysgit
/ 。安装成功后，可以看到git bash和git gui两种操作方式，以下通过git bash进行操作。2、
假设欲在 I:\\code_repo\\dcshop目录建立仓库作为服务端，依次执行以下命令：cd I:\\
code_repo\\dcshop......
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

weixin_38171468的博客

06-01 [readCountW] 77

Git 下载安装及本地配置

Git安装非常简单，按照步骤一步一步操作就行了安装环境：Windows10 64bit下载 Git网址：
https://git-scm.com/download/win版本：Git-2.21.0-64-bit

yuzx2008的博客

12-04 [readCountW] 622

使用 U 盘做 Git 仓库的步骤

创建本地仓库，更改 GIT_PROJECTcd /home/yuzx/git# 这里定义了一个变量表示工程目录GIT
_PROJECT=env_install# 切换到工程目录，包含 .gitignorecd ${GIT_PROJECT}/# 建库git init
git add .git commit -m 'init commit'# 拷贝裸库，其实就是工程目录里的 .git 目录c

便携版git——git-portable

12-17
 
git-portable是针对windows版git所设计的一款软件。顾名思义，它就是便携版或移动版的git。
你可以选择将它安装在U盘等便携设备上。它不需要安装，也不会在注册表上留下记录，有关内容
可以

JAYSITE的博客

10-07 [readCountW] 150

用U盘作为远程git仓库管理代码

用过git的朋友都能体会到其代码版本管理的便利性，很多公司也都是用类似的工具进行代码管理
的，如果是一般的代码，我们可以在github上建立远程仓库进行代码管理，但是如果代码涉及到
保密性的话，放在github上我们可能会担心其安全性，这种情况下我们可以用一个U盘来作为远程
仓库的载体，具体操作方法如下：在本地创建一个仓库new_repo并初始化 git init 添加文件并
commi......

9.冄2.7.號的博客

10-21 [readCountW] 3万+

Git的安装与使用教程（超详细！！！）

git 提交全部文件1、get.add .git add xx 命令可以将xx文件添加到暂存区，如果有很多改动可
以通过get add -A .来一次添加所有改变的文件。注意 -A 选项后面还有一个句点。git add -A
表示添加所有内容，git add . 表示添加新文件和编辑过的文件不包括删除的文件；git add -u
表示添加编辑或者删除的文件，不包括新添加的文件。2.、git ......

smilebigdear的专栏

05-07 [readCountW] 432

把Git Repository建到U盘上去(转载)

Git很火。原因有三：它是大神Linus Torvalds的作品，天然地具备神二代的气质和品质；促进了
生产力的发展，Git的分布式版本控制理念，并非首创，但非常适合开源社区的协作方式（不存在
master-slave的关系） GitHub GitHub很好，号称代码界的facebook. facebook，twitter，
Microsoft，vmware，redhat，Linke...

Dream Fly Together

02-14 [readCountW] 1万+

将ubuntu安装到U盘上，实现即插即用

亲身实践了将Ubuntu系统安装到移动U盘上，实现即插即用，有两种方法，如下文所述。第一种：
VMware虚拟机+1U盘【参考文章：https://baijiahao.baidu.com/s?id=1599727812387460443&
amp;wfr=spider&amp;for=pc】将Linux（ubuntu）安装到U盘上，实现即插即用本教程是说明如何
将ubuntu系统安装到U盘上（也......

u010928583的博客

11-01 [readCountW] 356

commons-chunk-plugin 使用方法

commons-chunk-plugin 使用方法1、commons-chunk-plugin概述CommonsChunkPlugin提取代码中
的公共模块，然后将公共模块打包到一个独立的文件中，也可以将第三方模块分开打包，以便在
其他的入口和模块中使用。需要在html中单独引入抽离出来的公共模块。2、commons-chunk-plu
gin配置参数{ name: string, // o......

u010928583的博客

11-06 [readCountW] 3188

webpack多页面，动态生成模板并插入对应的js（代码分析）

1、工具代码分析(utils.js)var path = require('path')var glob = require('glob')/** * 读
取指定路径下的文件 * @param {String} glob 表达式 * @param {String} base 基础路径 */
exports.getEntries = function (globPath, base, r...

weixin_30887919的博客

08-27 [readCountW] 45

git怎么创建本地版本仓库

git怎么创建本地版本仓库安装git我就不用说了吧！下载地址：https://github.com/msysgit/
msysgit/releases/download/Git-1.9.4-preview20140815/Git-1.9.4-preview20140815.exe 安
装完后点击桌面Git Bash ：命令如下$ mkdir testcms$ cd testcms$ ...

weixin_30648587的博客

11-07 [readCountW] 73

使用U盘或在本地电脑作为git远程仓库进行托管

情景描述:当有两台电脑需要共同维护一段代码，其中一台电脑不希望（或者不能）通过网络的方
式进行访问git仓库（即不使用github），那么可以使用U盘作为介质将其作为远程仓库，或者使
用局域网中一台电脑作为远程仓库。使用U盘作为远程仓库1. 首先我们先建立一个本地仓库，如
果有则跳过这一步。2. 建立远程托管仓库。将建立好的本地仓库clone出一个bare的your.git
仓......

企鹅肖

08-10 [readCountW] 4247

Git--团队开发必备神器

花了两天时间专门搞了一下git，整理一下分享给大家。下面我们开始。。。转载请注明出处：
http://blog.csdn.net/Hello_Chillax/article/details/47405887更多精彩请访问：http://
blog.csdn.net/hello_chillax，谢谢一.学习使用git之前的准备（Windows环境下）。在本地装
一个git，并装上GitBash。(...

快乐&&平凡

04-17 [readCountW] 3万+

git 使用详解（8）-- 分支HEAD

有人把 Git 的分支模型称为“必杀技特性”，而正是因为它，将 Git 从版本控制系统家族里区分
出来。Git 有何特别之处呢？Git 的分支可谓是难以置信的轻量级，它的新建操作几乎可以在瞬
间完成，并且在不同分支间切换起来也差不多一样快。和许多其他版本控制系统不同，Git鼓励在
工作流程中频繁使用分支与合并，哪怕一天之内进行许多次都没有关系。理解分支的概念并熟练
运用后，你才会意识到为什么 Git...

dianxuequ6825的博客

06-24 [readCountW] 40

u盘作为git仓库，完成不同地方的代码同步

参考网上一篇 “把Git Repository建到U盘上去” 1.我经常有这样的需求，工作、家里需要对同一
份代码或文档编辑，并希望在不同地方能同步；如果是私密性不那么强，可以直接使用github托
管哈，当然付费也是可以私密的~~ 2.首先U盘上创建一个repository 名称： git_mycbb_repos 
git init –bare git_mycbb......

crjmail的博客

08-21 [readCountW] 202

如何用u盘当做远程仓库

1、现在u盘里新建一个文件夹，如git_mycbb_repos;2、通过命令符cd到该目录下，然后git init
--bare3、假设在A电脑中，已经建好了相关的git文件夹，并且已经提交。git remoteadd usb/I/
git_mycbb_repos，将U盘设置为该项git文件夹的远程工作目录；4、git push usb master ,将本
地数据推送到U盘。......

程序员的兼职技能课

04-24
获取讲师答疑方式：在付费视频第一节(触摸命令_ALL)片头有二维码及加群流程介绍限时福利原
价99元，今日仅需39元！购课添加小助手（微信号：csdn590）按提示还可领取价值800元的编程
大礼包！讲师介绍：苏奕嘉 前阿里UC项目工程师脚本开发平台官方认证满级（六级）开发者。我
将如何教会你通过【定制脚本】赚到你人生的第一桶金？零基础程序定制脚本开发课程，是完全
针对零脚本开发经验的小白而设计，课程内容共分为3大阶段： ①前期将带你掌握Q开发语言和界
面交互开发能力； ②中期通过实战来制作有具体需求的定制脚本； ③后期将解锁脚本的更高阶玩
法，打通任督二脉； ④应用定制脚本合法赚取额外收入的完整经验分享，带你通过程序定制脚本
开发这项副业，赚取到你的第一桶金！

Windows版YOLOv4目标检测实战：训练自己的数据集

04-26
课程演示环境：Windows10; cuda 10.2; cudnn7.6.5; Python3.7; VisualStudio2019;
OpenCV3.4 需要学习ubuntu系统上YOLOv4的同学请前往：《YOLOv4目标检测实战：训练自己的数
据集》课程链接：https://edu.csdn.net/course/detail/28745 YOLOv4来了！速度和精度双提升
！与 YOLOv3 相比，新版本的 AP (精度)和 FPS （每秒帧率）分别提高了 10% 和 12%。 YOLO系
列是基于深度学习的端到端实时目标检测方法。本课程将手把手地教大家使用labelImg标注和使
用YOLOv4训练自己的数据集。课程实战分为两个项目：单目标检测（足球目标检测）和多目标检
测（足球和梅西同时检测）。本课程的YOLOv4使用AlexyAB/darknet，在Windows系统上做项目演
示。包括：安装软件环境、安装YOLOv4、标注自己的数据集、整理自己的数据集、修改配置文件
、训练自己的数据集、测试训练出的网络模型、性能统计(mAP计算)和先验框聚类分析。还将介绍
改善YOLOv4目标检测性能的技巧。除本课程《Windows版YOLOv4目标检测实战：训练自己的数据集
》外，本人将推出有关YOLOv4目标检测的系列课程。请持续关注该系列的其它视频课程，包括：
《Windows版YOLOv4目标检测实战：人脸口罩佩戴检测》《Windows版YOLOv4目标检测实战：中国
交通标志识别》《Windows版YOLOv4目标检测：原理与源码解析》
[feedLoadin]
©️2020 CSDN 皮肤主题: 大白设计师: CSDN官方博客返回首页

目录

[INS::INS]

  •  

    Git本地远程仓库的搭建(局域网内也可提交)

    07-09 [readCountW]1万+
  •  

    在U盘里建立git仓库

    08-16 [readCountW]101
  •  

    Git 下载安装及本地配置

    06-01 [readCountW]77
  •  

    使用 U 盘做 Git 仓库的步骤

    12-04 [readCountW]622
  •  

    便携版git——git-portable

    12-17
    立即下载
  •  

    用U盘作为远程git仓库管理代码

    10-07 [readCountW]150
  •  

    Git的安装与使用教程（超详细！！！）

    10-21 [readCountW]3万+
  •  

    把Git Repository建到U盘上去(转载)

    05-07 [readCountW]432
  •  

    将ubuntu安装到U盘上，实现即插即用

    02-14 [readCountW]1万+
  •  

    commons-chunk-plugin 使用方法

    11-01 [readCountW]356
  •  

    webpack多页面，动态生成模板并插入对应的js（代码分析）

    11-06 [readCountW]3188
  •  

    git怎么创建本地版本仓库

    08-27 [readCountW]45
  •  

    使用U盘或在本地电脑作为git远程仓库进行托管

    11-07 [readCountW]73
  •  

    Git--团队开发必备神器

    08-10 [readCountW]4247
  •  

    git 使用详解（8）-- 分支HEAD

    04-17 [readCountW]3万+
  •  

    u盘作为git仓库，完成不同地方的代码同步

    06-24 [readCountW]40
  •  

    如何用u盘当做远程仓库

    08-21 [readCountW]202
  •  

    程序员的兼职技能课

    04-24
  •  

    Windows版YOLOv4目标检测实战：训练自己的数据集

    04-26

实付0元
[pay-time-o] 点击重新获取
[wx][zfb][JD]扫码支付
( ) 钱包余额 0
[pay-help]

抵扣说明：

1.余额是钱包充值的虚拟货币，按照1:1的比例进行支付金额的抵扣。
2.余额无法直接购买下载，可以购买VIP、C币套餐、付费专栏及课程。

[recharge]余额充值
[]
