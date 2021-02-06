## istream
> 使用isteam对象当做条件时，其效果是检测流的状态。

1. 例子
```
	while (std::cin >> value)
		pass
```

2. 如果流是有效的，即流未遇到错误，那么检测成功。

3. 当遇到**文件结束符**，或遇到一个无效的输入时(例如读入一个和**value的类型**不同的对象), 
istream对象的状态变为无效。处理无效的状态的istream对象会使条件变为假。

4. 文件结束符
	1. windows 系统的文件结束符是**Ctrl+Z**，然后按Enter或Return键。
	2. unix系统和Max OS是**Ctrl+D**
    
