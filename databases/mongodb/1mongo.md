返回主页

python运维自动化之路

  • 博客园
  • 首页
  • 新随笔
  • 联系
  • 订阅
  • 管理

随笔 - 655  文章 - 240  评论 - 32

mongodb:常用命令大全

一、MongoDB 数据库常用操作命令

1、Help查看命令提示

help
db.help();
db.yourColl.help();

2、切换/创建数据库

use raykaeso; 当创建一个集合(table)的时候会自动创建当前数据库

3、查询所有数据库

show dbs;

4、删除当前使用数据库

db.dropDatabase();

5、从指定主机上克隆数据库

db.cloneDatabase(“127.0.0.1”); 将指定机器上的数据库的数据克隆到当前数据库

6、从指定的机器上复制指定数据库数据到某个数据库

db.copyDatabase(“mydb”, “temp”, “127.0.0.1”);将本机的mydb的数据复制到temp数据库中

7、修复当前数据库

db.repairDatabase();

8、查看当前使用的数据库

db.getName()/db;

9、显示当前db状态

db.stats();

10、当前db版本

db.version();

11、查看当前db的连接服务器机器地址

db.getMongo();

12、查询之前的错误信息和清除

db.getPrevError();
db.resetError();

二、MongoDB Collection聚集集合

1、创建一个聚集集合（table）

db.createCollection(“collName”, {size: 20, capped: 5, max: 100});//创建成功会显示{“ok”:1}
//判断集合是否为定容量db.collName.isCapped();

2、得到指定名称的聚集集合（table）

db.getCollection(“account”);

3、得到当前db的所有聚集集合

db.getCollectionNames();

4、显示当前db所有聚集索引的状态

db.printCollectionStats();

5、查询当前集合的数据条数

db.yourColl.count();

6、查看当前集合数据空间大小

db.yourColl.dataSize();

7、得到当前聚集集合所在的db

db.yourColl.getDB();

8、得到当前聚集的状态

db.coll.stats();

9、得到聚集集合总大小

db.coll.totalSize();

10、聚集集合储存空间大小

db.coll.storageSize();

11、聚集集合重命名

db.coll.renameCollection(“ray”); 将coll重命名为ray

12、删除当前聚集集合

db.coll.drop();

三、MongoDB用户相关

1、添加一个用户（创建）

db.createUser({user: 'username', pwd: 'xxxx', roles: [{role: 'readWrite', db: 'dbname'}]}); 添加用户、设置密码、是否只读

2、数据库认证、安全模式(登录)

db.auth(“ray”, “123456”);

3、显示当前所有用户

show users;

4、删除用户

db.removeUser(“userName”);

四、MongoDB聚集集合查询

1、查询所有记录

db.userInfo.find();
相当于：select* from userInfo;

默认每页显示20条记录，当显示不下的情况下，可以用it迭代命令查询下一页数据。注意：键入it命令不能带“;”
但是你可以设置每页显示数据的大小，用DBQuery.shellBatchSize= 50;这样每页就显示50条记录了。

2、查询去掉后的当前聚集集合中的某列的重复数据

db.userInfo.distinct(“name”);
会过滤掉name中的相同数据
相当于：select distict name from userInfo;

3、查询age = 22的记录

db.userInfo.find({“age”: 22});
相当于： select * from userInfo where age = 22;

4、条件查询的记录
MongoDB中条件操作符有：
(>) 大于 – $gt
(<) 小于 – $lt (>=) 大于等于 – $gte
(<= ) 小于等于 – $lte db.userInfo.find({age: {$gt: 22}}); 相当于：select * from userInfo where age>22;

db.userInfo.find({age: {$lt: 22}});
相当于：select * from userInfo where age<22; db.userInfo.find({age: {$gte: 25}}); 相当于：select * from userInfo where age >= 25;

db.userInfo.find({age: {$lte: 25}});
相当于：select * from userInfo where age <= 25; 5、and查询 db.userInfo.find({age: {$gte: 23, $lte: 26}}); 相当于：select * from userInfo wher age >=23 and age <=26
db.userInfo.find({name: ‘raykaeso’, age: 22});
相当于：select * from userInfo where name = ‘raykaeso’ and age = ‘22′;

6、字符模糊查询

db.userInfo.find({name: /mongo/});
//相当于%%
[code]select * from userInfo where name like ‘%mongo%';

7、查询指定列数据

db.userInfo.find({}, {name: 1, age: 1});
相当于：select name, age from userInfo;
当然name也可以用true或false

8、按条件查询指定列数据

db.userInfo.find({age: {$gt: 25}}, {name: 1, age: 1});
相当于：select name, age from userInfo where age <25;

9、排序

升序：db.userInfo.find().sort({age: 1});
降序：db.userInfo.find().sort({age: -1});

10、查询前5条数据

db.userInfo.find().limit(5);
相当于：select * from userInfo limit 5;

11、查询10条以后的数据

db.userInfo.find().skip(10);
相当于：select count() from userInfo as total;
select from userInfo limit 10,total;

12、查询在5-10之间的数据

db.userInfo.find().limit(10).skip(5);
可用于分页，limit是pageSize，skip是第几页pageSize
相当于：select from userInfo limit 5,10;

13、or与查询

db.userInfo.find({$or: [{age: 22}, {age: 25}]});
相当于：select * from userInfo where age = 22 or age = 25;

14、查询第一条数据

db.userInfo.findOne();
db.userInfo.find().limit(1);
相当于：select * from userInfo limit 1;

15、查询某个结果集的记录条数

db.userInfo.find({age: {$gte: 25}}).count();
相当于：select count(*) from userInfo where age >= 20;

五、MongoDB索引

1、创建索引

db.userInfo.ensureIndex({name: 1});
db.userInfo.ensureIndex({name: 1, ts: -1});

2、查询当前聚集集合所有索引

db.userInfo.getIndexes();

3、查看总索引记录大小

db.userInfo.totalIndexSize();

4、读取当前集合的所有index信息

db.users.reIndex();

5、删除指定索引

db.users.dropIndex(“name_1″);

6、删除所有索引索引

db.users.dropIndexes();

六、MongoDB修改、添加、删除集合数据

1、添加

db.users.save({name: ‘zhangsan’, age: 25, sex: true});
添加的数据的数据列，没有固定，根据添加的数据为准

2、修改

db.users.update({age: 25}, {$set: {name: ‘changeName’}}, false, true);
相当于：update users set name = ‘changeName’ where age = 25;
db.users.update({name: ‘Lisi’}, {$inc: {age: 50}}, false, true);
相当于：update users set age = age + 50 where name = ‘Lisi';
db.users.update({name: ‘Lisi’}, {$inc: {age: 50}, $set: {name: ‘hoho’}}, false, true);
相当于：update users set age = age + 50, name = ‘hoho’ where name = ‘Lisi';

3、删除

db.users.remove({age: 132});

4、查询修改删除

db.users.findAndModify({
query: {age: {$gte: 25}},
sort: {age: -1},
update: {$set: {name: ‘a2′}, $inc: {age: 2}},
remove: true
});

posted @ 2019-02-26 15:17  活的潇洒80  阅读(...)  评论(...)  编辑  收藏
刷新评论刷新页面返回顶部
Copyright © 2020 活的潇洒80
Powered by .NET Core on Kubernetes
