## key 33
1. type
2. expire
	- expire/expireat
	- pexpire/pexpireat
3. dump
4. rename/renamenx
5. persist
6. move
7. randomkey
8. dump
9. ttl/pttl
10. del
11. exists
12. keys pattern
## string 49
### set 50
1. set, setnx, mset, msetnx
2. setex, psetex
	- command key time value
3. setbit
	- setbit key offset value(0/1)
4. setrange
	- setrange key offset value(str)
### get 62
1. get, getset, gitbit, getrange, mget
2. getbit
	- getbit key offset
3. getrange
	- getrange key start end
### incr/decr 71
1. incr/ decr
	- command key
2. incrby/incrbyfloat/decrby
	- command key value
### str 78
1. strlen
2. append
## list 82
1. Lindex, Lrange, Linsert, Lset, Lrem, Llen, Ltrim
2. Lpush, Rpush/ Lpop, Rpop/ Lpushx, Rpushx
3. BLpop, BRpop, BRpopLpush
4. RpopLpush
## Hash 91
1. Hset, Hget/ Hmset, Hmget/ Hgetall, Hsetnx
2. Hincrby, Hincrbyfloat
3. Hexists, Hlen, Hdel, Hvals, Hkeys
## set 98
### set 99
1. sunion, sdiff, sinter / sunionstore, sdiffstore, sinterstore
2. smove, sscan
### curd 104
1. sadd, spop, srem
### container 107
1. scard
2. srandmember
3. smembers
4. sismember
## zset 113
### 提示 114
1. rank -> 秩序,索引
2. range ->区间
3. byscore ->以分数
4. rev -> 反序,降序
5. lex ->字典区间
6. 默认排序是以索引:
### 命令详解: 122
1. zadd key score member[{score member}...]
	创建或设置指定key对应的有序集合，根据每个值对应的score来排名，升序。
	例如有命令 zadd key1 10 A 20 B 30 D 40 C;那么真实排名是 A B D C
2. zrem key member
	删除指定key对应的集合中的member元素
3. zcard key
	返回指定key对应的有序集合的元素数量
4. zincrby key increment member
	将指定key对应的集合中的member元素对应的分值递增加 increment
5. zcount key min max
	返回指定key对应的有序集合中，分值在min~max之间的元素个数
6. zrank key member
	返回指定key对应的有序集合中，指定元素member在集合中排名，从0开始切分值是从小到大升序
7. zscore key member
	返回指定key中的集合中指定member元素对应的分值
8. zrange key min max [withscores]
	返回指定key对应的有序集合中，索引在min~max之间的元素信息，如果带上 withscores 属性的话，可以将分值也带出来
9. zrevrank key member
	返回指定key对应的集合中，指定member在其中的排名，注意排名从0开始且按照分值从大到小降序
10. zrevrange key start end [withscores]
	指定key对应的集合中，分值在 start~end之间的降序，加上 withscores 的话可以将分值以及value都显示出来
11. zrangebyscore key start end [withscores]
	同 zrange命令不同的是，zrange命令是索引在start[end范围的查询，而zrangebyscore命令是根据分值在start]end之间的查询且升序展示
12. zrevrangebyscore key max min [withscores]
	同zrangebyscore命令不同的是，zrangebyscores是根据分值从小到大升序展示，而zrevrangebyscore命令是从max到min降序展示
13. zremrangebyrank key start end
	移除指定key对应集合中索引在start~end之间(包括start和end本身)的元素
14. zremrangebyscore by min max
	同zremrangebyrank命令类似，不同的该命令是删除分值在min~max之间的元素
15. zinterstore desk-key key-count key...
	获取指定数量的key的交集。
	例如有 key1:{10:A,20:B,30:C},key2{40:B,50:C,60:D},那么命令 zinterstore key3 2 key1 key2 
	意思就是将key1 key2这两个集合的交集赋给key3，如何获
	取key1与key2的交集呢。 key1中存在 A B C,key2中存在 B C D,那么交集就是 B 和 C，
	且 B与C对应的score也会叠加，即 key3{B:20+40=60,C:30+50=80}
16. zunionstore desk-key key-count key...
	获取指定数量key的并集，例如有 key1{10:A,20:B,30:C},key2{40:B,50:C,60:D},可以看出
	A和D不是key1与key2共有的，但是并集中只要存在就会记录进去，然后B与C是共有的，即并集的
	结果就是 key3{10:A,B:60,D:60,C:80}
### curd 179
1. zadd key score member
2. zrem key member
3. zincrby key increnment member
4. zremrangebyrank key start end (byrank, 以索引为准)
5. zremrangebyscore key min max (byscore, 以分数为准)
6. zremrangebylex (以字典)
### get 187
1. zrank
2. zrevrank
3. zrange (默认以索引排序)
4. zrangebyscore
5. zrangebylex
6. zscore
7. zrevrange: 反:区间:(索引)
8. zrevrangebyscore
### container 197
1. zlexcount, zcard, zrem, zscan, zcount
### 集合 200
1. zunionstore
2. zinterstore
## 连接 204
1. echo, select, ping, auth
## 事务 207
1. multi, watch, unwatch, discard, exec
## 发布订阅: 210
1. subscribe, unsubscribe
3. 正则表达式
	- Psubscribe: 订阅所有指定模式的频道
	- Punsubscribe: 退订所有指定模式的频道
3. publish, pubsub
## 服务器命令 218
### server 219
1. client pause
2. debug object
3. flushdb
4. flushall
5. save: 异步保存到数据硬盘
6. lastsave: 返回最近一次成功将数据保存到硬盘上的时间
7. config get/ config set /config resetstat
8. command
8. command info
9. debug segfault: 让redis崩溃
10. dbsize: key的数量
11. shutdown: 异步保存数据到硬盘,并关闭服务器
12. client kill
15. client getname
15. client setname
18. client list
13. monitor
14. command getkeys
15. command count
16. time
17. info
18. config rewrite
19. bgsave: 在后台异步保存**当前数据库的数据**到磁盘.
### log 244
1. slowlog: 慢日志
2. bgrewriteaof: 异步执行一个AOF文件重写操作.
### 主备 249
1. slaveof: 将当期那服务器转变为指定服务器的从属服务器
2. sync
3. role
### cluster 254
1. cluster slots: 获取集群节点的映射数组
