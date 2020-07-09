## python 配置文件格式
### json格式
1. 格式:

2. python 用法
	1. json.load 和json.loads的区别
		- load 是从文件读取,加载成为dict
		- loads是从程序内存读取,加载成为dict

	2. json.dump和json.dumps的区别
		- dump 将字典转化为string,然后写入文件
		- dumps 将字典转化为string.

	3. 
### yaml格式
1. 格式:

2. python 用法
	1. yaml.load(data, Loader=yaml.FullLoader)

	2. yaml.load_all(data, Loader)
		如果yaml有多个文档内容的话,可以使用"---"分隔,load_all返回一个生成器,生成的每个对象都是一个分隔开的内容.

	3. yaml.dump会将python中的数据写入到yaml文件.

3. 注意:
	1. yaml load时需要指定一个一个Loader,否则会有警告

	2. yaml.dump输出到yaml文件并不是标准的yaml文件,要输出标准的yaml文件,使用ruamel的yaml.
	```
	from ruamel import yaml
	...
	yaml.dump(config_data, file, Dumper=yaml.RoundTripDumper)
	```

### python的标准库ConfigParser
1. 格式:

2. python 用法

3. 注意:
