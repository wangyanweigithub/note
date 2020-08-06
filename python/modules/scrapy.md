
<!-- vim-markdown-toc GFM -->

* [1. scrapy 架构图 19]
* [2. Spiders 23]
* [3. Item 34]
		* [Selectors 选择器 37]
		* [Selector有四个基本的方法 40]
		* [提取item 47]
* [4. 命令行工具 51]
* [2. Spiders 90]
* [3. Item 103]
		* [Selectors 选择器 106]
		* [Selector有四个基本的方法 110]
		* [提取item 117]
* [4. 命令行工具 120]

<!-- vim-markdown-toc -->
## 1. scrapy 架构图 19
![架构图1]()
![架构图2]()

## 2. Spiders 23
1. 处理网页信息的方法：必须继承与scrapy.Spider,且定义三个属性、方法

	1. 	name：爬虫名字，必须唯一
	2. 	start_urls：起始url
	3. 	parse：解析页面的方法

2. 可选的属性：
	1. allowed_domains：应该是检测域名


## 3. Item 34
1. 就像是ORM一样，保存从网页提取的信息结构。

#### Selectors 选择器 37
>从网页中提取数据有很多方法。Scrapy使用了一种基于 XPath 和 CSS 表达式机制: Scrapy Selectors

#### Selector有四个基本的方法 40

- xpath(): 传入xpath表达式，返回该表达式所对应的所有节点的selector list列表 。
- css(): 传入CSS表达式，返回该表达式所对应的所有节点的selector list列表.
- extract(): 序列化该节点为unicode字符串并返回list。
- re(): 根据传入的正则表达式对数据进行提取，返回unicode字符串list列表。

#### 提取item 47
1. 每个 response.xpath() 调用返回selector组成的list，因此我们可以拼接更多
的 .xpath() 来进一步获取某个节点

## 4. 命令行工具 51
- feed输出：scrapy crawl name -o out.json

小型项目输出json，大型项目自己写Item Pipeline 输出到数据库

命令执行流程：

>Scrapy为Spider的 start_urls 属性中的每个URL创建了 scrapy.Request 对象，并将 parse 方法作为回调函数(callback)赋值给了Request。

>Request对象经过调度，执行生成 scrapy.http.Response 对象并送回给spider parse() 方法。

- 无参数scrapy，输出scrapy支持的命令和提示
> scrapy

- 创建项目
> scrapy startproject myporject

- 控制项目

	1. 全局命令：
	
			startproject
			settings
			runspider
			shell
			fetch
			view
			version

	2. 项目命令：

			crawl
			check
			list
			edit
			parse
			genspider
			deploy

## 2. Spiders 90
处理网页信息的方法：必须继承与scrapy.Spider,且定义三个属性、方法

1. 	name：爬虫名字，必须唯一
2. 	start_urls：起始url
3. 	parse：解析页面的方法

可选的属性：

1. allowed_domains：应该是检测域名
2. 


## 3. Item 103
1. 就像是ORM一样，保存从网页提取的信息结构。

#### Selectors 选择器 106
>从网页中提取数据有很多方法。Scrapy使用了一种基于 XPath 和 CSS 表达式机制: Scrapy Selectors

#### Selector有四个基本的方法 110
- xpath(): 传入xpath表达式，返回该表达式所对应的所有节点的selector list列表 。
- css(): 传入CSS表达式，返回该表达式所对应的所有节点的selector list列表.
- extract(): 序列化该节点为unicode字符串并返回list。
- re(): 根据传入的正则表达式对数据进行提取，返回unicode字符串list列表。

#### 提取item 117
每个 response.xpath() 调用返回selector组成的list，因此我们可以拼接更多的 .xpath() 来进一步获取某个节点

## 4. 命令行工具 120

- feed输出：scrapy crawl name -o out.json

小型项目输出json，大型项目自己写Item Pipeline 输出到数据库

命令执行流程：

>Scrapy为Spider的 start_urls 属性中的每个URL创建了 scrapy.Request 对象，并将 parse 方法作为回调函数(callback)赋值给了Request。

>Request对象经过调度，执行生成 scrapy.http.Response 对象并送回给spider parse() 方法。

- 无参数scrapy，输出scrapy支持的命令和提示
> scrapy

- 创建项目
> scrapy startproject myporject

- 控制项目

	1. 全局命令：
	
			startproject
			settings
			runspider
			shell
			fetch
			view
			version

	2. 项目命令：

			crawl
			check
			list
			edit
			parse
			genspider
			deploy
			bench
