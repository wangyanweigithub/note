##curd
### 创建
1. insert:
```
post = {"a": 1};

db.blog.insert(post);
```

### 删除
1. remove 
```
db.blog.remove({title: "my blog"})
```
### 更改: update
1. update 更改整个文档:
	1. update接受至少两个参数: 
		- 要更新文档的限定条件
		- 新的文档
			这是整体更新,将原来的文档覆盖,原来的文档丢失

	2. eg
	```
	1. 给文档添加新建:
	post.comments = []

	2. update替换文档
	db.blog.update({title: "my Blog post"}, post)
	```

2. update 修改器:

### 查找
1. find/findOne

