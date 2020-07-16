
<!-- vim-markdown-toc GFM -->

* [Python操作MongoDB]
	* [集合操作：]
		* [增]
		* [查：]
		* [查询关键字：]
		* [查询条件操作符：]
		* [数据排序+跳跃+范围：]
		* [改]
		* [`$`修改器及`$`特殊用法：]
		* [删]
	* [发布订阅]

<!-- vim-markdown-toc -->
# Python操作MongoDB
```
import pymongo
myclient=pymongo.MongoClient(host='127.0.0.1',port=27017)   #指定主机和端口号创建客户端
mydb=myclient['dbtest']#数据库使用
mycol=mydb['t1']#表（集合）使用
```

## 集合操作：
### 增         
1. insert_one()----单条插入
	db.collection.insert_one({})

2. insert_many()----多条插入
	db.collection.insert_many([{},{}...])

### 查：         
1. find()----查询符合条件的所有数据
    db.collection.find(查询条件)

2. find_one()----查询符合跳进的第一条数据
	db.collection.find_one(查询条件)

### 查询关键字：             
1. $and----并列查询
	db.collection.find({'$and':[{},{}...]})

2. $or----或条件查询
	db.collection.find({'$or':[{},{}...]})

3. $in----范围查询
	db.collection.find({field:{'$in':['',''...]}})

4. $all----子集查询
	db.collection.find({field:{'$all':['',''...]}})

### 查询条件操作符：             
1. $lt----小于
	db.collection.find({field:{'$lt':value}})

2. $gt----大于
	db.collection.find({field:{'$gt':value}})

3. $eq----等于
	db.collection.find({field:{'$eq':value}})

4. $lte----小于等于
	db.collection.find({field:{'$lte':value}})

5. $gte----大于等于
	db.collection.find({field:{'$gte':value}})

6. $ne----不等于
	db.collection.find({field:{'$ne':value}})

### 数据排序+跳跃+范围： 

1. sort(filed,pymongo.ASCENDING/pymongo.DESCENDING)----对查询结果升序/降序排序
	db.collection.find({}).sort()

2. skip(num)----对查询结果进行跳跃取值
	db.collection.find({}).skip()

3. limit(num)----对查询结果进行范围截取
	db.collection.find({}).limit()
		
4. 优先级：sort>skip>limit,与使用时的顺序无关
	db.collection.find({}).sort().skip().limit()
	db.collection.find({}).limit().sort().skip()
	db.collection.find({}).skip().sort().limit()

### 改         
1. update_one()----修改查询结果的第一条数据
    db.colleciton.update_one({查询条件},{$修改器:{修改值}})

2. update_many()----修改查询结果所有数据
    db.colleciton.update_many({查询条件},{$修改器:{修改值}})

### `$`修改器及`$`特殊用法：             

1. $set----修改某个字段的值
        db.colleciton.update_one({'name':'c','age':20},{'$set':{'hobby':['swim,dump','sing']}})

2. $unset---删除字段
        db.colleciton.update_one({'$and':[{'name':'c'},{'age':20}]},{"$unset":{'hobby':[1,2]}})

3. $inc----引用增加（先引用 后增加）
        db.colleciton.update_many({'name':{'$in':['d','e','f']}},{'$inc':{'age':2}})

4. 针对数组操作：
    1. $push----在Array的最后一个位置中增加一个数据
		db.colleciton.update_one({'name':'b'},{'$push':{'hobby':['swim','sing']}})

    2. $pushAll----在Array的最后一个位置中增加多个数据
        mycol.update({'name':'c'},{'$pushAll':{'hobby':['sing','scrapy']}})#实际测试无法使用，报错：Unknown modifier: $pushAll

    3. $pull ----删除Array中的指定单个元素
        .update_one({'hobby':'run'},{'$pull':{'hobby':'eat'}})

    4. $pullAll ----删除Array中的指定多个元素
        mycol.update_many({'name':'b'},{'$pullAll':{'hobby':['swim','play']}})#实际测试每次只删除一个元素

    5. $pop----删除Array中的第一个或者最后一个元素 正数是倒序删除 负数是正序删除
        db.colleciton.update_many({'hobby':'run'},{'$pop':{'hobby':1}})

    6. $----存储当前(Array)符合条件的元素下标索引 ，只能存储最外层的 索引位置
        db.colleciton.update_many({'hobby':'run'},{'$set:{'hobby.$':'swim'}})

### 删         
1. delete_one()----删除查询结果的第一条数据
	db.colleciton.delete_one(查询条件)

2. delete_many()----删除查询结果的所有数据
    db.colleciton.delete_many(查询条件)

## 发布订阅
```
sub = db.pubsub()
sub.Psubscribe()
```
