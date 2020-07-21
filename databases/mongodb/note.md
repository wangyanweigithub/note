
<!-- vim-markdown-toc GFM -->

* [note]
	* [field在前还是$修改器在前]
	* [查询注意:]
	* [$修改器]
	* [save 和insert]
	* [删除]
* [数据库]
	* [优劣]
	* [全局]
* [最佳实践]

<!-- vim-markdown-toc -->
## note
### field在前还是$修改器在前
1. find只有field和组合条件($and, $or, $not, $in, $nin)可以在作为最外层的key

	1. 单个field多个condition,使用 {"field": {"$command": []}}

	2. 多个field多个condition,使用 {"field1": {}, "field2":{}}

	3. 组合条件的,$and在外: {"$and": [{"name": {"$ne": "wang"}}, {"name": {"$ne": "da"}}]}
		- 注意,组合条件是多个条件组成数组.
		- 可以灵活使用$命令,比如上面可以使用$nin

2. find 和update条件中是key在前还是更改器在前.
	- <font color=red>find查找时,field在条件前面作为key,因为同一个field可能有多个条件,每个条件是一个kv</font>

	- <font color=red>update更改是,更改器在field前面作为key,因为更改时肯定是针对一个field,不可能对同一个field多次改变,field在后面更直观</font>

3. 修改器和查找条件的不同:
	- 修改器一般是一个修改,所以一般修改作为key,而field和新值的字典作为value
	- 查找器有多个必要条件,所以一般多个条件在字典里面,而field作为条件的key.

### 查询注意:
1. find指定列时,除了_id之外,其他的列只能出现fale或者true,不能既有true,又有false.

2. find查询,一个条件内如果相同命令出现两次,第二次覆盖第一次,如果都需要保留,需要使用$and列表.
	- {"name": {"$ne": "wang", "$ne": "da"}},只有第二个$ne起效
		{"$and": [{"name": {"$ne": "wang"}}, {"name": {"$ne": "da"}}]}

	- {"age": {"$gt": 2, "$lt": 23}},两个条件都起效

### $修改器
1. 注意update的修改和替换,替换整个文档的话`_id`也会变,而修改的话`_id`是不变的.

2. 修改器:局部修改:
	- update 第二个参数使用{"$command": {"key": "value"}}
	- 修改器`_id`不能变,

3. 整体文档替换
	- 文档

### save 和insert

### 删除
1. 删除数据库: db.dropDatabase();

2. 删除当前聚集集合: db.coll.drop();

3. 删除集合中的文档,但不删除集合本身,原有索引也会保留下来:
	db.col.remove()

## 数据库
### 优劣
1. 面向文档,自动分割平衡集群数据,数据库负载,自动重排文档

2. MongoDB支持索引，索引放在内存中，能够提升随机读写的性能。如果索引不能完全放在内存，
一旦出现随机读写比较高的时候，就会频繁地进行磁盘交换，MongoDB的性能就会急剧下降

3. MongoDB占用的空间很大，因为它属于典型空间换时间原则的类型。那么它的磁盘空间比普通数据库会浪费一些

### 全局
1. 属性查找:
	- 先找db的属性,如果db.后跟的不是db的属性的话
	- 再找collection
	- collection不方便如此查找的话,使用getCollection破解同名

2. js 中,x.y 和x[y]完全等价

5. monogdb的4M文档要求
	- monogdb 1.8支持的16M文档要求
	- 这个数字是改变的,可以查看文档

6. remove不删除collection,不删除index,不可撤销,不可恢复

7. 删除整个collection使用dropCollection,然后重建index,比一条一条删除省时间

8. update是原子性的,先到先执行,最后更新确定数据最终状态

9. DBRef
```
{"$ref": collections, "$id": id_value}
{"$ref": coll, "$id": id_value, $db":db_name"} 
```
10. mongodb 停止:
	- kill -2: pid SIGINT 保存缓存然后关闭,不要-9, -9直接关闭进程,
		会导致数据库文件损害或者丢失数据

	- kill pid SIGTERM

	- use admin; db.shutdownServer();

11. 本地mongodb不需要auth认证,有特权

12. 创建固定集合 
	db.createCollection({"My": {capped: true, size: 10000, max: 100}});

13. 服务端执行命令的两种方法:
	- db.runCommand()
	- db.$cmd.findOne({"command":1})

14. js中,for (var i in iter) 如果iter是数组,那么i就是数组索引,
	如果iter是字典对象,那么i就是字典的key

## 最佳实践
1. 各个数据库完全独立:
	一个mongodb实例可以承载多个数据库,它们之间可视为完全独立的.每个数据库都有独立的权限控制,即使再磁盘上,
<font color=green>不同的数据库也放置在不同的文件中</font>

2. 将一个应用的所有数据都存储在一个数据库中的做法就很好.多个应用最好使用不同的数据库.

3. 删除:
	remove删除文档很快,但是清除整个集合,直接删除集合然后重建索引会更快.

4. 实践证明,一定要创建查询中用到的所有键的索引.
  一定不要每个键都做索引.
