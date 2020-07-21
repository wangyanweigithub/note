
<!-- vim-markdown-toc GFM -->

* [mongod]
* [安全和认证]
	* [角色]
* [备份]
* [复制]
	* [主从复制]
* [副本集]
	* [复制原理:]
	* [诊断]
* [分片]

<!-- vim-markdown-toc -->
### mongod
1. mongod的选项可以通过mongod --help来查看
	- dbpath
	- port
	- fork
	- logpath
	- config

2. 关闭mongod,如果mongod在后台运行,关闭方法:
	1. kill pid / kill -2 pid  千万不要-9 pid,会导致文件损坏.
	2. shutdown 命令
		这是管理命令,要在admin数据库下使用
		use admin
		db.shutdownServer();

3. 服务器状态:
	- serverStatus
	- mongostat

### 安全和认证
1. 在认证的上下文中,Mongodb会将普通的数据作为admin数据库处理.
2. admin数据库中的用户被视为超级用户,即管理员
3. 认证之后,管理员可以读写所有数据库,执行特定的管理命令,如listDatabases 和shutdown
4. 开启安全检查之前,一定至少有个管理员账号.

5. 数据库的用户账号以文档的形式存储在system.users集合里面.
6. 在system.users集合中删掉用户账号文档,就可以删除用户.
7. system.users在admin 中,所有数据库的账号都在admin数据库中.他们的_id里包含了他们是哪个数据库的账号.

8. 用户认证时,服务器将认证和连接绑定来跟踪认证,也就是说,如果驱动程序或是工具使用了连接池或是因故障切换到了另一个节点,所有认证用户都必须对每个新连接重新认证.

#### 角色
1. 内置角色（Built-In Roles）
	内置角色是MongoDB预定义的角色，操作的资源是在DB级别上。MongoDB拥有一个SuperUser的角色：root，
拥有最大权限，能够在系统的所有资源上执行任意操作。

2. 数据库用户角色（Database User Roles）：
	- read：授予User只读数据的权限
	- readWrite：授予User读写数据的权限

3. 数据库管理角色（Database Administration Roles）：
	- dbAdmin：在当前dB中执行管理操作
	- dbOwner：在当前DB中执行任意操作
	- userAdmin：在当前DB中管理User

4. 备份和还原角色（Backup and Restoration Roles）：
	- backup
	- restore

5. 跨库角色（All-Database Roles）：
	- readAnyDatabase：授予在所有数据库上读取数据的权限
	- readWriteAnyDatabase：授予在所有数据库上读写数据的权限
	- userAdminAnyDatabase：授予在所有数据库上管理User的权限
	- dbAdminAnyDatabase：授予管理所有数据库的权限

6. userAdminAnyDatabase用户只是拥有了创建用户，删除用户的角色，这并不是最高权限

7. 权限认证是跟着db走的,某个db都有自己的账户密码.

### 备份
1. 备份方法.
	- 冷备: 拷贝/db/data 
		除非服务器做了完整的fsync,还不允许写入,否则运行的mongodb创建的数据目录副本并不安全,可能备份已经损坏.

	- mongodump / mongorestore  客户端查询/客户端插入
		- mongorestore --help
			mongodump -d test -o backup
			mongorestore -d foo --drop backup/test/
			--drop: 在恢复前删除(若存在)

	- fsync + 写入锁: 写入会阻塞
		1. fsync会强制服务器将所有缓冲区写入磁盘,还可以上锁阻止进一步写入.直到释放锁为止.

		2. 
		```
		use admin;
		db.runCommand({"fsync":1, "lock": 1})
		```


		3. 解锁:
		```
		db.$cmd.sys.unlock.findOne();
		db.currentop(); //currentOp是为了确保已经解锁(初次请求解锁会花点时间)
		```

	- 通过从服务器备份,最好
		fsync+锁会阻塞写入操作,唯一不耽误读写还能保证实时快照备份方式就是通过从服务器备份.

2. 数据修复
	1. 最简单方式就是 --repair: mongod --repair 来启动服务

	2. 修复运行的服务器中的数据库: repairDatabase
		use test
		db.repairDatabase()

3. 批量插入
	- 文档的数组插入

	- 多个文档插入比较快

	- 批量导入只对多个文档插入同一个集合有效,不支持多个集合

	- 导入数据使用mongoimport 而不是批量导入.

### 复制
#### 主从复制
1. 主服务器: mongod --master
2. 从服务器: mongod --slave --source master_address
3. 所有从节点都从主节点复制内容,因为从节点没有自己的oplog
4. 从节点太多对主服务器压力很大,不超过12个从节点

5. 主从复制选项
	1. --only
		在从节点上指定只复制特定某个数据库(默认复制所有数据库)

	2. --slavedelay
		在从节点上,当应用主节点的操作时增加延时(单位是秒),延时从节点.

	3. --fastsyncc
		以主节点的数据快照为基础启动从节点.比完整同步快很多.

	4. --autoresync
		如果从节点与主节点不同步了,则自动同步.

	5. --oplogsize
		主节点oplog的大小(单位是MB)

6. 添加删除源, slave的信息在local中,local的集合不会被复制.
	1. 添加
		./mongod --slave --dbpath /db/data1 --port 27018
		use local
		db.sources.insert({"host": "localhost:27017"})
		
		db.sources.find()
	
	2. 在生产环境下,想更改从节点的配置,改用prod.example.com为源, 则可以用insert和remove来完成.
		db.sources.insert({"host":"prod.example.com:27017"})
		db.sources.remove({"host": "localhost:27017"})

	3. 注意:如果切换的两个主节点有相同的集合,Mongodb会尝试合并,但不能保证正确合并.
		要是使用的是一个从节点对应多个不同的主节点,最好在主节点上使用不同的命名空间

### 副本集
> 自动故障回复功能的主从集群

1. 主从集群和副本集最为明显的区别是副本集没有固定的"主节点": 
	整个集群会选举出一个"主节点", 当其不能工作时则变更到其他节点.

2. 副本集总会有一个活跃节点(primary)和一个或多个备份节点(secondary)

3. 初始化副本集
	1. 不能用localhost地址作为成员,需要找机器的主机名: cat /etc/hostname

	2. 要为每一个服务器创建数据目录:
		mkdir -p ~/dbs/node1 ~/dbs/node2

	3. 需要为每一个副本集起个名字: blort
	
	4. 启动服务器: --replset: 作用是让服务器知晓这个副本集还有别的同伴.
		./mongod --dbpath ~/dbs/node1 --port 10001 --replset blort/morton:10002
	
	5. 添加第二台
		./mongod --dbpath ~/dbs/node2 --port 10002 --replset blort/morton:10001

	6. 添加第三台:两种都可以,只要让服务器知道任何一个服务器就可以
		./mongod --dbpath ~/dbs/node2 --port 10002 --replset blort/morton:10001
		./mongod --dbpath ~/dbs/node2 --port 10002 --replset blort/morton:10001, morton:10002
		
	7. 初始化:
		- 连接一个服务器,执行初始化命令,**初始化命令只能执行一遍**
		```
		./mongo morton:10001/admin
		db.runCommand({"replSetInitiate": {
			"_id": "blort",
			"members": [
				{"_id": 1, "host": "morton:10001"},
				{"_id": 2, "host": "morton:10002"},
				{"_id": 3, "host": "morton:10003"},
			],
		}})
		```
4. 配合会在服务器之间传递:local.system.replset

5. 节点
	- standard: 投票,复制数据,可以成为活跃节点
	- passive: 存储完整数据副本,参与投票,不能成为活跃节点
	- arbiter: 只投票,不复制数据,不能成为活跃节点

6. 标准节点和被动节点之间的区别仅仅是数量的差别:
	- 每个参与者(非仲裁者)有个优先权,优先权为0则表示是被动的,不能成为活跃节点.
	- 优先值不为0,则按照从大到小选出活跃节点
	- 优先值一样的话,则看谁的数据比较新.

7. 在节点配置中修改priority键,来配置成标准节点或者别动节点
```
members.push({
	"id": 3,
	"host": "morton:10003",
	"priority": 40
})
```
8. arbiterOnly键可以指定仲裁节点:
```
members.push({
	"id": 4,
	"host": "morton:10004",
	"arbiterOnly": true
})
```

9. 备份节点会从活跃节点抽取oplog,并执行操作,就像活跃备份系统中的备份服务器一样.
活跃节点也会写操作到自己的本地opog,这样就能成为活跃节点了.
oplog的操作也包括严格递增的序号,通过这个序号来判定数据的时效性.

10. 活跃节点使用心跳来跟踪集群中有多少节点对其可见,如果不够半数,活跃节点会自动降为备份节点.

11. 从节点的读扩展: slaveOkay

12. 用从节点做数据处理:
	1. 用--master启动一个从节点: 对一个节点同时使用了--slave --maser

	2. 可以像普通数据库那样读取,写入,但是它同时在不断从真正的主节点符合数据
	3. 一定要确保不能对正在复制主节点的从节点上的数据库执行写入

	4. 从节点第一次启动时,也不能有正咋被复制的数据库,否则只能更新新的操作,不会同步了.

#### 复制原理:
1. 主节点记录在其上执行的所有操作,
2. 从节点定期轮询主节点获得这些操作.然后对自己的数据副本执行这些操作.
3. oplog:
	- 位置: local.oplog.$main集合
	- oplog每个文档都代表一个主节点上执行的一个操作.
	- 内容: 
		- ts
		- op
		- ns
		- o

	- oplog是固定集合,大小是剩余磁盘的5%的可用空间.
	- --oplogSize 指定oplog的大小.

4. 同步:
	1. 从节点第一次启动,会对主节点进行完整同步
	2. 同步完成后,从节点开始查询主节点的oplog并执行这些操作.
	3. 从节点跟不上同步时,复制就会停下,从节点需要重新做完成的同步:
		1. {"resync": 1}命令手动执行重新同步
		2. 也可以再启动从节点时使用 --autoresync选项让其自动重新同步.
	4. 重新同步代价高昂,所以尽量避免,方法就是配置足够大的oplog

5. local 数据库存放副本集信息
	- master 机器: db.slaves.find()
		
	- slave: db.sources.find()
		{"_id": xx, "host": xx .....syncedTo:{"t": 1234332534534,"i": 1}, "localLogTs": {"t": 0, "i": 0}}

6. 阻塞复制
	1. getLastError的"w"参数来确保数据的同步性.
	2. 运行getLastError 会进入阻塞,直到M个服务器复制了最新的写入操作为止.
		db.runCommand({getLastError: 1, w: N})

#### 诊断
1. 连接主节点:
	db.printReplicationInfo

2. 连接从节点
	db.printSlaveReplicationInfo

3. 变更oplog大小:
	1. 停掉主节点,删除local数据库文件,用新的 --oplogSize重启
		1. rm /data/db/local.*
		2. ./monmod --master --oplogSize size
		3. 所有从节点得用 --autoresync 重启,否则就需要手动重新同步.

4. 复制认证
	1. 如果使用了认证,主从节点都需要在local数据库添加用户,每个节点的用户名和口令都是相同的.
	2. local数据库用户类似于admin用户,都可以读写整个服务器.
	3. 从节点链接主节点时,会使用存储在local.system.users中的用户进行认证.
		- 最先尝试"repl"用户
		- 没有repl,则用local.system.users中的第一个可用用户.

### 分片
1. 片键选择:
	
2. 建立分片
	1. 首先启动配置服务器
		mkdir -p ~/dbs/config
		./mongod --dbpath ~/dbs/config --port 20000

	2. 启动mongos,不需要数据库位置,但需要指明配置服务器位置
		./mongos --port 30000 --configdb localhost:20000

	3. 添加片
		1. 启动一个mongo实例
			mkdir -p ~/dbs/shard1
			./mongod --dbpath ~/dbs/shard1 --port 10000

		2. 连接到mongos, 添加一个mongodb服务器到分片
			./mongo localhost:30000/admin
			db.runCommand({addshard:"localhost:10000", allowLocal:true})

3. 切分数据:
	1. mongodb不会将存储的每一条数据都直接发布,得先在数据库和集合的级别将分片功能打开.
		db.runCommand({"enablesharding": "foo"})// 对数据库启用sharding
		db.runCommand({"shardcollection": "foo.bar", "key": {"_id": 1}})// 对结合启用sharding,配置key

4. 生产配置
	- 多个配置服务器
	- 多个mongos 服务器
	- 每个片都是副本集
	- 正确设置w, getLastError, insert都有这个参数.

5. 健壮的配置
	1. 建立多个配置服务器
		mkdir -p ~/dbs/config1 ~/dbs/config2 ~/dbs/config3
		./mongod --dbpath ~/dbs/config1 --port 20001
		./mongod --dbpath ~/dbs/config2 --port 20002
		./mongod --dbpath ~/dbs/config3 --port 20003

	2. 启动mongos时,连接这三个配置服务器
		./mongos --configdb localhost:20001, localhost:20002, localhost:20003

	3. 配置服务器使用的是两部提交,而不是异步复制来维护集群配置的不同副本.这样能保证集群的一致性.

6. 多个mongos

7. 健壮的片
	1. 生产环境中,每个片都是副本集.
	2. 用addshard命令就可以将副本集作为片添加.添加时只要指定副本集的名字和种子就好.
		db.runCommand({"addshard": "foo/prox.example.com:27017"}) 
		添加副本集任一个服务器都可以,如果这个实例挂了,shared知道副本集中其他实例.

8. 管理分片
> 分片信息主要存放在config数据库上,连接到mongos就可以访问.

	1. 连接到mongos
	2. use config
	3. 片:
		db.shards.find()
	4. dbs:
		db.databases.find()
	5. 块
		db.chunks.find()

9. 分片命令
	1. 获得概要:
		db.printShardingStatus()
	2. 删除片
		db.runCommand({"removeshard": "localhost:10000"})
	3. 1.6版本中,如果删除的数据库的大本营(基片),必须手动移动数据库
		db.runCommand({"moveprimary": "test", "to": "localhost:100001"})
		
