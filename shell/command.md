1. linux 替换目录下所有文件中的某个字符串
	1. 将目录/modules下面所有文件中的张三都修改成lisi
		> sed -i "s/zhangsan/lisi/g" `grep zhangsan -rl /modules`
		
	2. 解释： 
		1. -i 表示inplace edit，就地修改文件
		2. -r 表示搜索子目录
		3. -l 表示输出匹配的文件名

2. 所有目录下所有文件中的某一个字符串
	> grep -r str *
