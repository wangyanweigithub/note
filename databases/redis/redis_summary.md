## redis 命令总结
### set
> set表示`k->v`的意思.所有具有索引的kv对都有set.

1. string: set key value
2. list:   lset key index value. 索引是下标值
3. hash:   hset key field value. 索引是key,和string类似.

4. set/zset 无序且没有索引,所有set/zset没有set命令.只有sadd,spop(随机),srem

### 删除
1. 总结:
	1. 有序的删除小标: rem
	2. 无序的使用删除key的, del
	3. trim: 剪切切片的用trim

2. 详细 
	1. string: del
	2. list: ltrim, lrem, lrange
	3. hash: hdel
	4. set/zset: srem

### 子元素:
1. 总结:
	1. 获取容器的子元素切片:
		- lrange
		- hvals, hkeys

	2. set无序所有没有切片,只有判断是否包含这个元素:
		- sismember, smembers
	
	3. zset通过值可以有多种获取切片的方法
		- zrane, zrangebyscore, zrangebylex, zrevrange
		- zrevrangebyscore
