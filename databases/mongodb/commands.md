
<!-- vim-markdown-toc GFM -->

* [全局 32]
* [数据库操作 87]
* [集合 121]
	* [种类 122]
		* [固定集合 123]
		* [GridFS: 存储文件 144]
		* [数据库引用 152]
	* [操作 114 168]
* [用户操作 209]
* [索引 224]
* [curd 352]
	* [添加  353]
	* [更改: update 366]
		* [修改器 395]
			* [数组修改器: 433]
	* [删除 471]
	* [find 484]
		* [高级查询选项 580]
		* [获取一致结果 588]
		* [聚合操作 596]
			* [group  603]
			* [mapreduce 667]
		* [eg  730]
* [附件 826]
	* [函数 827]
	* [命令 831]

<!-- vim-markdown-toc -->
## 全局 32
1. Help查看命令提示
	- help
	- db.help();
	- db.yourColl.help();

2. 查询之前的错误信息和清除
db.getPrevError();
db.resetError();

3. 创建index,document 有附加结构,index是集合来定义的.

4. 子集合可以用.来实现,语言驱动有这个语法糖,可以直接通过.来查找字集合

5. 保留的数据库名:
	- admin
	- local: 这个数据库永远不会复制,用于存储限于本地单台服务器使用的任意集合.
	- config: monogdb 分片时的配置库.

6. save:
> save 是一个shell函数,可以在文档不存在时插入,存在时更新.它只有一个参数,文档.

	1. 如果这个文档含有"_id", save会调用upsert,否则会调用插入.

7. update第四个参数表明是单独更新一个,还是将匹配的文档全部更新.

8. 想要知道多文档更新到底更新了多少个文档,可以运行getLastError命令. 键"n"的值就是要的数字.

9. 数据类型
	- null
	- 布尔: true, false
	- 32位整数
	- 64位整数
	- 64位浮点数
	- 字符串
	- 符号
	- 对象id
	- 日期
	- 正则表达式
	- 代码
	- 二进制数据
	- 最大值
	- 最小值
	- 未定义: undefiled
	- 数组
	- 内嵌文档

10. 安全操作:
	- 插入,删除,更新都是瞬间完成.因为不管服务器是否完成. 离弦之箭
	- 安全版本: 执行完操作之后立即运行getLastError命令,检查是否执行成功.

11. 捕获常规错误
	mongo最新版本和pymongo都是安全版本,会返回执行结果.

	
## 数据库操作 87
1. 查看mongo帮助文档: mongo -h

2. 登陆
```
mongo -umiaobt -p --authenticationDatabase
mongo -umiaobt -p331sharp --authenticationDatabase
```

3. 删除数据库: db.dropDatabase();

4. 获取数据库名称: db.getName();

5. 获取数据库状态: db.stats();

6. 当前db版本: db.version();  

7. 查看当前db的链接机器地址: db.getMongo();

8. 从指定主机上克隆数据库: db.cloneDatabase("127.0.0.1");

9. 从指定的机器上复制指定数据库数据到某个数据库: 
	db.copyDatabase(“mydb”, “temp”, “127.0.0.1”);
	将本机的mydb的数据复制到temp数据库中

10. 修复数据库: db.repairDatabase();  

11. 在MongoDB中频繁的进行数据增删改时，如果记录变了，例如数据大小发生了变化，这时候容易产生一些数据碎片，
	1. 出现碎片引发的结果
		- 一个是索引会出现性能问题
		- 另外一个就是在一定的时间后，所占空间会莫明其妙地增大，
		
	2. 所以要定期把数据库做修复，定期重新做索引，这样会提升MongoDB的稳定性和效率

## 集合 121
### 种类 122
#### 固定集合 123
1.特点
	- 类似环形队列,空间满了以后,删除最早的文档
	- 不能删除文档
	- 更新不得导致文档移动(通常更新意味着尺寸变大)
	- 默认情况下固定集合没有索引,即便是"_id"也没有索引

2. 属性方法
	1. 不像普通集合,固定集合必须在使用前显示创建.
		db.createCollection("my_col", {capped: true, size: 10000, max: 100})
	2. size是字节,max是文档数量上限.当指定max时必须制定size.

	3. 将普通集合转变成为固定集合: convertToCapped
		db.runCommand({convertToCapped: "test", size:10000})

	4. 自然排序: 文档按照插入顺序存储.
		- 反向查找: db.may_col.find().sort({"$natural": -1})

	5. 尾部游标: 只能用于固定集合
		mongodb shell 没有,只能在语言驱动里查找.

#### GridFS: 存储文件 144
1. mongofiles:
	- mongofiles put foo.txt
	- mongofiles list
	- mongofiles get foo.txt
	- mongofiles search foo.txt
	- mongofiles delete foo.txt

#### 数据库引用 152
> 数据库引用,也叫作DBRef,就像是URL,唯一确定到一个文档的引用.它自动加载文档的方式正如网站
URL通过链接自动加载web页面一样.

1. DBRef必选键:
	{"$ref": collection, "$id": id_value} // 唯一确定同一个数据库中某一集合某一文档.

2. 如果想要引用其他数据库的文档,还需要添加一个可选键"$db"
	{"$ref": collection, "$id": id_value, "$db": database}

3. <font color=green>DBRef键的顺序不能改变,第一个必须是"$ref", 接着必须是"$id", 最后是"$db"<font>

4. 引用是以references为key,一个数组为value的对象.value中的key是3中所标记的key.

	note = {"_id": 20, "author": "wang", "references": [{"$ref": collection, "$$id": id}, {xxxx}]}

###	操作 114 168
1. 创建一个聚集集合,固定大小集合（table）
```
//指定数据库大小size，最大存放100个文档，满了，就会mongodb 会删除旧文档。
db.createCollection("human",{"size":1024,capped:true,max:100});
db.createCollection("people");
```
//判断集合是否为定容量db.collName.isCapped();

2. 得到指定名称的聚集集合（table）
	db.getCollection(“account”);

3. 得到当前db的所有聚集集合
	db.getCollectionNames();

4. 显示当前db所有聚集索引的状态
	db.printCollectionStats();

5. 查询当前集合的数据条数
	db.yourColl.count();

6. 查看当前集合数据空间大小
	db.yourColl.dataSize();

7. 得到当前聚集集合所在的db
	db.yourColl.getDB();

8. 得到当前聚集的状态
	db.coll.stats();

9. 得到聚集集合总大小
	db.coll.totalSize();

10. 聚集集合储存空间大小
	db.coll.storageSize();

11. 聚集集合重命名
	db.coll.renameCollection(“ray”); 将coll重命名为ray

12. 删除当前聚集集合: db.coll.drop();

## 用户操作 209
1. 添加一个用户:
	db.createUser({user: 'username', pwd: 'xxxx', roles: [{role: 'readWrite', db: 'dbname'}]}); 添加用户. 设置密码. 是否只读

2. 数据库认证. 安全模式: db.auth(“zs”, “111”);

3. 显示当前所有用户，角色
```
show users;
show roles;
```

4. 删除用户: db.removeUser("zs");  


## 索引 224
1. 创建索引
```
db.people.ensureIndex({name: 1});  
db.people.ensureIndex({name: 1, ts: -1});  //联合索引
db.people.ensureIndex({"name":1},{"unique":true}); //唯一索引
```

2. 查询当前聚集集合所有索引

	db.people.getIndexes();  

3. 查看总索引记录大小

	db.people.totalIndexSize();  

4. 读取当前集合的所有index信息

	db.people.reIndex();  

5. 删除指定索引

	db.people.dropIndex("name_1");

6. 删除所有索引索引

	db.people.dropIndexes();  

7. 实践证明,一定要创建查询中用到的所有键的索引.

8. 单个键的索引方向无所谓,但是多个键需要考虑方向的问题.

9. 如果包含N个键,则对于前几个键的查询都会有帮助.比如有个索引{"a": 1, "b": 1, "c": 1},实际上有了{"a": 1}, {"a": 1, "b": 1}, {"a": 1, "b": 1, "c": 1}等索引.

但是使用{"b": 1},{"a": 1, "c": 1}等索引不会被优化.最大开头字符串类似.

  只有使用索引前部的查询才能使用该索引.
  添加索引的坏处在于每次插入,更新,删除都要额外操作索引,所有索引尽可能少.每个集合默认最大索引树为64个.

  一定不要每个键都做索引.

10. 内嵌文档建立索引:
	db.blog.ensureIndex({"comments.date": 1})

11. 对内嵌文档的键索引与普通键索引并无差异,两者也可以联合组成复合索引.

12. 索引的名字:
	db.foo.ensureIndex({"a": 1, "b": 1}, {"name": "alphabet"})

	索引名称字符有个数限制,所以特别复杂的索引在创建时一定要使用自定义的名字.

13. unique 唯一索引.如果插入文档没有唯一索引,会保存为null,第二个没有唯一索引的文档会因为重复的null而报错.

14. 删除重复: dropDups 可以保留第一个发现的稳定,而删除接下来的重复值的文档.
	db.people.ensureIndex({"username": 1}, {"unique": true, "dropDups": true})

15. 索引信保存在每个数据库的system.indexs集合中.这是一个保留集合,不能对它进行插入或者删除文档.
操作只能通过ensureIndex和dropIndex进行.

16. 建立索引是阻塞操作,指定background执行是是在后台线程执行,不会阻塞:
	db.people.ensuerIndex({"name:" 1}, {"background": true})

17. 地理空间索引同样有ensureIndex来创建,只不过不是1和-1,而是2d
	db.map.ensureIndex({"gps": 2d})

	- gps 键的值必须是某种形式的一对值:一个包含两个元素的数组或是包含两个键的内嵌文档.
	```
	{"gps": [0, 100]}
	{"gps:": {"x": -30, "y": 30}}
	{"gps:": {"latitude": -180, "longitude": 180}}
	```

	- 键名可以随意,如{"gps": {"foo": 0, "bar": 1}}

	- 默认情况下,地理空间索引值的范围是-180~180,

	- 如果要使用其他的值,使用ensureIndex的选项来指定最大最小值:
		db.star.trek.ensureIndex({"linght-yeras": "2d"}, {"min": -1000, "max": 1000})

18. 地理空间查询以两种形式进行
	- 普通查询(find): 使用了$near
		1. db.map.find({"gps": {"$near": [40, -73]}}).limit(10)
			find, $near不会返回距离,但是如果结果大于4MB,这是唯一的选择.


	- 使用数据库命令
		1. db.runCommand({"geoNear": "map", near: [40, -73], num: 10})
			geoNear 会返回每个文档到查询点的距离

19. 查找指定形状内的文档: 将$near换成$within
	- 矩形: $box
		db.map.find({"gps": {"$within": {"$box": [[10, 30], [15, 30]]}}}) //左下角和右上角

	- 圆形: $center
		db.map.find({"gps": {"$within": {"$center": [[10, 30], 5]}}}) 

20. 复合索引:
	db.ensureIndez({"location": "2d", "desc": 1})






























## curd 352
### 添加  353
1. insert:
```
post = {"a": 1};
db.blog.insert(post);
```

2. save:
```
db.people.save({name: 'zhangsan', age: 18, sex: true});  
添加的数据的数据列，没有固定，根据添加的数据为准
```

### 更改: update 366
1. update 更改整个文档:
	1. update接受至少两个参数: 
		- 查询文档,找出修改的文档
		- 修改器文档,描述对文档做哪些修改

	2. 两种修改方式:

		1. 整体覆盖
		```
		1. 给文档添加新建:
		post.comments = []

		2. update替换文档
		db.blog.update({title: "my Blog post"}, post)
		```

		2. 修改器
			- db.people.update({age: 18}, {$set: {name: 'changeName'}}, false, true); 
			相当于：update people set name = 'changeName' where age = 18;  

			- db.people.update({name: 'zhangs'}, {$inc: {age: 12}}, false, true);  
			相当于：update people set age = age + 12 where name = 'zhangs';  

			- db.people.update({name: 'zhangs'}, {$inc: {age: 12}, $set: {name: 'hoho'}}, false, true);
			相当于：update people set age = age + 12, name = 'hoho' where name = 'zhangs';  

	3. 更新操作是原子性的

#### 修改器 395
1. $set: 用来指定一个值,如果键不存在,则创建它.

2. $unset: 和$set相反.删除某一个字段
	db.a.update({name: "wang"}, {$unset: {"comment": 1}})

3. $inc 增加和$dec减少

4. $upsert
	- 要是没有文档符合更新条件,就会以这个条件和更新文档作为基础创建爱你一个新的文档.
	- 如果找到匹配的文档,则正常更新.

	- 总结: upsert非常方便,不必预置集合,同一套代码既可以创建又可以更新.
		- 更加高效,且是原子性的.

	- update第三个参数表示表示这个是否是upsert
	```
	db.analytics.update({"url": "/blog"}, {"$inc": {"visits": 1}}, true)
	```
5. findAndModify
> 有点慢,因为它要扥带数据库的响应.

	- 对于操作查询以及执行其他需要取值和赋值风格的原子性操作来说十分方便.将一个查询和一个修改原子的完成.
	- 对于那些查询,修改有竞态条件的操作,这个原子操作可以避免这个问题.
	- 如果不用这个方法,而先用find,然后update,需要update之后,调用getLastError,查看上条命令是否生效,生效表示update成功.
	- findAndModify既可以update,也可以remove删除文档.
	- update和remove只能有一个,且必须有一个.匹配不到文档,会返回一个错误.
	- 它一次只能处理一个文档,也不能执行upsert操作.只能更新已有文档.
	- 参数:
		- findAndModify: 集合名
		- query: 查询文档
		- sort
		- update
		- remove
		- new

6. 插入,删除,更新都是瞬间完成.因为不管服务器是否完成.

##### 数组修改器: 433
1. $push: 数组已存在,会在末尾添加一个,否则会创建一个新的数组.
注意,push后面跟一个值就可以,不需要是一个数组,在db中,会自动添加一个中括号.

2. 经常会有种情况,如果一个值不在数组里面就把他加进去.可以在查询文档中用"$ne"来实现.
eg:
```
db.papers.update({"authors cited": {"$ne": "Richid"}}, {$push: {"authors cited": "Richie"}})
```
注: 实际使用时,当没有authors cited这个字段时,会添加成功.但重复运行,会再添加一遍.$ne不能识别出数组里面的值和Richid
一个数组和一个字符串相比,永远不相等.只有当没有这个字段时,还比较合适.

3. $addToSet: 可以避免重复添加.像一个集合一样.

4. $addToSet和$each的组合,可以添加多个不同的值.
```
db.users.update({}, {$addToSet: {"email": {$each: ["joe@php.net", "joe@example.com"]}})
```
	- $each: 后面跟数组,对数组中的每个元素执行一遍外面的命令.

5. $pop
	1. {$pop: {key: 1}}  从数组末尾删除一个元素
	1. {$pop: {key: -1}}  从数组头部删除一个元素

6. 基于特定的条件来删除元素,而不仅仅是位置,使用 $pull
	- $pull 会将所有匹配的部分删除掉,对数组[1, 1, 3, 1],执行pull 1,得到的结果只有一个元素的数组[2]

7. 有两种方法操作和数组中的值:
	1. 通过位置: 数组都是以0开头的,可以讲下标直接作为 键 来选择元素
		db.blog.update({"post": post_id}, {"$inc": {"comments.0.votes": 1}})

	2. 通过定位操作符: 很多情况下不能提前知道下标的值:
		- Mongo提供了定位次操作符"$",用来定位查询文档已经匹配的元素,并进行更新.
			db.blog.update({"comments.autuor": "John"}, {"$set": {"coments.$.author": "Jim"}})

		- 定位符只更新第一个匹配的元素.
		

### 删除 471
1. remove

2. 查询修改删除
```
db.users.findAndModify({
query: {age: {$gte: 25}},
sort: {age: -1},
update: {$set: {name: ‘a2′}, $inc: {age: 2}},
remove: true
});
```

### find 484
1. 查询返回一个数据库游标,游标只有在你需要的时候才会**惰性**的批量返回文档.

2. 查询文档的值必须是常量,意味着不能引用文档中其他键的值.

3. or: 有两种方式进行or查询

	1. $in/$nin: 可以用来查询一个键的多个值.

	2. $or: 更通用一些,用来完成多个键的任意给定值.

4. $not: 原条件句,即可以用在任何其他条件之上.

5. $mod: 会将查询的值除以第一个给定值,若余数等于第二个给定值,则返回该结果:
"""
db.users.find({"id_num": {"$mod": [5, 1]}})
db.users.find({"id_num": {"$not": {"$mod": [5, 1]}}})
"""

6. null: 及匹配null字符,同事匹配没有这个字段的文档.
	- 单独匹配null字符:
		"""db.c.find({"z": {"$in": [null], "$exists": true}})"""

7. 正则:
	- //:包裹正则
	- //i: i忽略大小写

8. 查询数组:
	- 每一个元素都是整个键的值

	- $all: 通过多个元素来匹配数组,这个数组中包含所有$all给定的值,
		```db.food.find({"furit": {"$all":["apple", "banana"]}})```

	- 精确匹配: 要完全一样才匹配,冗余元素和缺少元素都不行,排序不一样都不行.
		```db.food.find({"fruti": ["apple", "banana", "peach"]})```

	- 查询数组指定位置的元素: 使用 key.index语法下标
		```db.food.find({"fruit.2": "peach"})```

	- $size: 查询指定长度的数组:
		```db.food.find({"fruit": {"$size": 3}})```

	- $slice: 返回一个数组的子集合.find的第二个参数,指明返回那些键.可以指明返回数组的那些值.
		```db.blog.posts.findOne(condition, {"comments": {"$slice": 10}})```, 返回前10条
		```db.blog.posts.findOne(condition, {"comments": {"$slice": -10}})``` 返回后10条
		```db.blog.posts.findOne(condition, {"comments": {"$slice": [23, 10]}})``` 偏移值和返回的条数:返回24-33

		- find指明$slice,别的键默认全部返回,而其他指明某个键后,都是默认不返回其他键.

9. 查询内嵌文档
	- 查询整个文档: 查询顺序相关,如果顺序不对也查不出来.
	```
	{
		"name": {
			"first": "Joe",
			"last": "Schemoe"
		},
		"age": 46
	}

	db.people.find({"name": {"first": "Joe", "last": "Schmoe"}})
	```

	- 只针对其键/值对进行查询: 使用.号查询.
	```
	db.people.find({"name.first": "Joe", "name.last": "Schmoe"})
	```

10. 内嵌文档的数组,数组的两种查询都不可以,需要使用$elemMatch.用来部分指定匹配数组中单个内嵌文档的限定条件.
	- $elemMatch 将限定条件进行分组,仅当需要对一个内嵌文档的多个键操作时才会用到.

	```
	db.blog.find({"commets": {"$elemMatch": {"author": "jod", "score": {"$gte": 5}}}})
	```


11. $where:
```
db.foo.find({"$where": function() {
	for (var current in this) {
		for (var other in this){
			if (current != other && this[current] == this[other]) {
				return true;
			}
		}
	}
}})
```
如果函数返回true,文档就作为结果的一部分被返回,如果为false,则不然.

12. 也可以使用字符串来指定$where查询
```
db.foo.find({"$where:" "this.x + this.y == 10"})
db.foo.find({"$where:" "function() {return this.x + this.y == 10;}"})
```

#### 高级查询选项 580
1. $maxscan: integer / 查询最多扫描的文档数量
2. $min: document    / 查询的开始条件
3. $max: document    / 查询的结束条件
4. $hint: document   / 指定服务器使用那个索引进行查询
5. $explain: boolean / 获取查询执行的细节
6. $snapshot: boolean / 确保查询的结果实在查询执行的那一刻的一致快照.

#### 获取一致结果 588
1. 因为查询文档时,可能因为文档大小改变而导致文档移动或者文档的更改,使得文档顺序内容错乱.
**对查询使用快照可以解决这个问题**

2. 所有返回一组结果的查询实际上都进行了快照.

3. 不一致只在游标等待结果时集合内容被改变的情况下发生.

#### 聚合操作 596
1. count
	db.people.count()

2. distinct: 必须指定集合和键
	db.runCommand({"distinct": "people", "key": "age"})

##### group  603
1. 先选定分组所依据的键,而后Mongodb就会将集合依据选定键值的不同分成若干组.
而后可以通过聚合每一组内的文档,产生一个结果文档.
```
db.runCommand({"group": {
    "ns": "集合名",
    "key": "分组依据的key",
    "initial": {"time": 0} // 返回文档的初始化
    "$reduce": function(doc, perv) { // doc是每个查找的文档,prev是上面返回文档.
        if(doc.time > prev.time){
            prev.price = doc.price;
            prev.time = doc.time;
        }
    },
	"condition": {"day": {"$gt": "2010/08/3"}}
}})
```
	- initial 是返回文档的初始化
	- $reduce是一个函数,类似于python的reduce,只不过有两个参数,第二个参数就是上面初始化的文档
		reduce是对数组中所有的文档执行一遍函数,依次来将多个文档输出一个文档.和group无关.

	- 执行完,返回initial后面的文档.
	- $reduce 必须有.
	- condition: 选择文档的条件.

2. 完成器:
> 完成器用以精简从数据库传到用户的数据.

	- finalize 附带一个函数,在每组结果传递给客户端之前被调用一次. 可以用其修剪结果中的"残枝败叶".

	-eg
	```
	db.runCommand({"group": {
		"ns": "coll",
		"key": {"tags": true},
		"initial": {"tags": {}},
		"$reduce": function(doc, prev) {
			for (i in doc.tags) {
				if (doc.tags[i] in prev.tags) {
					prev.tags[doc[tags[i]]]++;
				} esle {
					prev.tags[doc.tags[i]] = 1;    
				}
			}
		},
		"finalize": function(prev) {
			var mostPopular = 0;
			for (i in prev.tags) {
				if (prev.tags[i]> mostPopular){
					prev.tag = i;
					mostPopular = prev.tags[i];
				}
			}
			delete prev.tags;
		}

	}})
	```

3. 将函数作为分组的键
```
db.posts.group({"ns": "posts", "$keyf": function(x) {return x.category.toLowerCase();}, "initializer": ...})
```

##### mapreduce 667
1. 步骤
	1. 映射(map),将从操作映射到集合中的每个文档. 
		- 这个操作要么"无作为"
		- 要么"产生一些键和X个值"

	2. 中间环节(洗牌,shuffle): 按照键分组,并将产生的键值组成列表放到对应的键中.

	3. 化简(reduce): 把列表中的值化简成为一个单值.这个值被返回.
					 
	4. 然后接着洗牌,直到每个键的列表只有一个值位置.这个值就是最后结果.

2. 找出集合所有的键
	1. 注意:
		- map使用emit返回要处理的值. 

		- emit会给MapReduce一个键(类似与前面group所使用的键)和一个值.

		- this 就是当前映射的文档的引用

		- 我们想为每个键单独计数,所以为文档的每一个键调用一次emit.

	2. 步骤
		1. map

		```
		map = function(){
			for (var key in this) {
				emit(key, {count: 1});
			}
		}
		```

		2. 由1产生了许许多多的{count: 1}的文档,每一个都与集合中的一个键相关.
			这种由一个或多个{count: 1}文档组成的数组,会传递给reduce函数.

		3. reduce有两个参数:
			- key, 也就是emit返回的第一个值
			- 数组,有一个后者多个对应于键的{count: 1} 文档组成
			
			```
			reduce = function(key, emits) {
				toral = 0;
				for (var i in emits) {
					total += emits[i].count;
				}
				return {"count": total};
			}
			```

		4. reduce 一定要能被反复调用,不论是映射环节还是前一个简化环节.
			- 所以reduce 返回的文档必须能作为reduce的第二个参数的一个元素.
			- 因为调用时,reduce第二个参数不一定就是初始的列表,可能有其他的组合方式
			- <font color=green>reduce应该能处理emit文档和其他reduce结果的各种组合.</font>

		5. 最终:
		```
		mr = db.a.mapReduce(map, reduce, {'out': result_of_a})	
		```

3. 网页分类


#### eg  730
1. db.people.find();  
相当于：select* from people;

	- 默认每页显示20条记录，当显示不下的情况下，可以用it迭代命令查询下一页数据。

	- 注意：键入it命令不能带“;”

	- 但是你可以设置每页显示数据的大小，用DBQuery.shellBatchSize= 50;这样每页就显示50条记录了。

2. 查询去掉后的当前聚集集合中的某列的重复数据
```
db.people.distinct("name");  
相当于：select distict name from people;
```

3. 查询age >= 23 并且 age <= 26
```
db.people.find({age: {$gte: 23, $lte: 26}});  
```

4. 查询name中包含 mongo的数据
```
db.people.find({name: /mongo/});  
相当于：select * from people where name like '%mongo%';
```

5. 查询name中以mongo开头的
```
db.people.find({name: /^mongo/});  
```

相当于：select * from people where name like ‘mongo%’;

6. 查询指定列name. age数据
```
db.people.find({}, {name: 1, age: 1});  
```

	相当于：select name, age from people;
当然name也可以用true或false,

	当用ture的情况下和name:1效果一样，如果用false就是排除name，显示name以外的列信息。

7. 查询指定列name. age数据, age > 18

	db.people.find({age: {$gt: 18}}, {name: 1, age: 1});  
	相当于：select name, age from people where age >18;

8. 按照年龄排序

	- 升序：db.people.find().sort({age: 1});  
	- 降序：db.people.find().sort({age: -1});  

9. 查询name = zhangsan, age = 18的数据

	db.people.find({name: 'zhangsan', age: 18});  
	相当于：select * from people where name = 'zhangsan' and age = '18';

10. 查询前5条数据

	db.people.find().limit(5);  
	相当于：select * from people Limit 5;

11. 查询10条以后的数据

	db.people.find().skip(10);  
	相当于：select * from people where id not in (select id from people limit 10);


12. 查询在5-10之间的数据

	db.people.find().limit(10).skip(5);  
	可用于分页，limit是pageSize，skip是第几页*pageSize

13. or与查询

	db.people.find({$or: [{age: 18}, {age: 18}]});  
	相当于：select * from people where age = 18 or age = 18;

14. 查询第一条数据

	db.people.findOne();  
	相当于：select * from people limit 1;
	db.people.find().limit(1);

15. 查询某个结果集的记录条数

	db.people.find({age: {$gte: 18}}).count();  
	相当于：select count(*) from people where age >= 20;

16. 求总数

	db.people.find({sex: {$exists: true}}).count();
	相当于：select count(sex) from people;

## 附件 826
### 函数 827
1. findAndModify
2. help
3. save
### 命令 831
