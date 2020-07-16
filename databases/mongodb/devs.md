## 运维
1. 备份
	- 冷备
	- mongodump / mongorestore  热备/冷备
	- fsync + 写入锁: 写入会阻塞
		```
		use admin;
		db.runCommand({"fsync":1, "lock": 1})


		解锁:
		db.$cmd.sys.unlock.findOne();
		db.currentop();
		```
	- 通过从服务器备份,最好

2. 批量插入
	- 文档的数组插入

	- 多个文档插入比较快

	- 批量导入只对多个文档插入同一个集合有效,不支持多个集合

	- 导入数据使用mongoimport 而不是批量导入.
