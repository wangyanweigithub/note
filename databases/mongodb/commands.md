
<!-- vim-markdown-toc GFM -->

* [全局 18]
* [MongoDB数据库操作 56]
* [MongoDB集合操作 90]
* [MongoBD用户操作 131]
* [索引 146]
* [curd 174]
	* [添加  175]
	* [更改: update 188]
		* [修改器]
	* [删除 213]
	* [find 226]
		* [eg  227]

<!-- vim-markdown-toc -->

## 全局 18
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

6. 数据类型
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

## MongoDB数据库操作 56
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

## MongoDB集合操作 90
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

## MongoBD用户操作 131
1. 添加一个用户:
	db.createUser({user: 'username', pwd: 'xxxx', roles: [{role: 'readWrite', db: 'dbname'}]}); 添加用户. 设置密码. 是否只读

2. 数据库认证. 安全模式: db.auth(“zs”, “111”);

3. 显示当前所有用户，角色
```
show users;
show roles;
```

4. 删除用户: db.removeUser("zs");  


## 索引 146
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

## curd 174
### 添加  175
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

### 更改: update 188
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

#### 修改器
1. $set: 用来指定一个之,如果键不存在,则创建它.

2. $unset: 和$set相反.删除某一个字段

3. $inc 增加和$dec减少

4. 数组修改器:
	- $push: 数组已存在,会在末尾添加一个,否则会创建一个新的数组.

	- $pop

### 删除 213
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

### find 226
#### eg  227
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
