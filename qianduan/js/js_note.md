## string 和array
1. 都是一个数组,string是字符的数组,array可以存储任何值.

2. indexOf

3. str.subString, arr.slice 类似.


## 容器
### 数组
1. 右边插入删除
	- push
	- pop

2. 左边插入删除:
	1. js
		- js: shift 删除开头
		- js: unshift 在开头添加

	2. redis
		- lpush, lpop
		- rpush, rpop

	3. python
		- 没有直接在开头操作的反噶法
		- 可以使用insert

	4. c
		- 全部需要指针操作

	5. c++
		- 

### set
1. js: var a = new Set()

### map
1. js: var a = new Map()

## iterable for
### for in : 循环的是对象的属性, array的index, objec的key
1. for (var i in array) {
	console.log(array[i])
}

### for of //只循环对象本身
1. for (var i of array) {
	
}

2. for of 用于obj,循环的是obj的属性值.

### arr.forEach
1. array.forEach(function(element, index, array) {})

2. set.forEach(function(element, sameelement, set) {})

3. map.forEach(function(value, key, map) {})

