
<!-- vim-markdown-toc GFM -->

* [redis 源码 12](#redis-源码-12)
* [redis 命令 15](#redis-命令-15)
* [redis 运维 17](#redis-运维-17)
* [redis 使用 19](#redis-使用-19)
	* [最佳实践 20](#最佳实践-20)
	* [分布式锁 21](#分布式锁-21)
	* [数据类型对应的方法](#数据类型对应的方法)

<!-- vim-markdown-toc -->
## redis 源码 12
1. 源码解读: redis_source.md

## redis 命令 15
1. 文件: commands.md

## redis 运维 17

## redis 使用 19
### 最佳实践 20
### 分布式锁 21

### 数据类型对应的方法
1. list:
	- 有序,所以每个index对应一个确定的值

2. set:
	- 无序,所以不能使用index
	- 包含关系,可以使用inter/diff/union
	- 添加只能使用sadd
	- 用来确定是否包含:sismember/smembers

3. hash:
	- 无序,不能使用index
	- 有`key->value`的映射,每个值都有对应的name,所以可以使用set,是Hset,Hget

4. zset
	- 无序,不能使用index
	- 有`key->value`,取值的时候可以通过score来排序,过滤

	
