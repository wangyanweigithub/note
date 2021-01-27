## 语法
### case 语法
1. case 语法
```
case "变量" in

	"变量1")

		...

		;; #输出两个分号

	"变量2")

		...

	  ;; #输出两个分号

	"变量3")

		...

	   ;; #输出两个分号

	*)

		 ...

	   ;; #输出两个分号

esac
```

## 常用命令
1. linux 替换目录下所有文件中的某个字符串
	1. 将目录/modules下面所有文件中的张三都修改成lisi
		> sed -i "s/zhangsan/lisi/g" `grep zhangsan -rl /modules`
		
	2. 解释： 
		1. -i 表示inplace edit，就地修改文件
		2. -r 表示搜索子目录
		3. -l 表示输出匹配的文件名

2. 所有目录下所有文件中的某一个字符串
	> grep -r str *

### 将liunx程序做成服务
> [将liunx程序做成服务](https://www.cnblogs.com/yaosj/p/6601649.html)

1. 写个小程序
```
#include<stdio.h>
#include<stdlib.h>
 
int main(int argc,char **argv)
{
　　while(1)
　　{
　　　　printf("hello world\n");
　　　　sleep(2);//2s
　　}
}
```

2. gcc编译

```
gcc -o hello hello.c
```

3. 在/etc/init.d/目录下生成hello.sh脚本
		
```
#!/bin/bash
 
SERVERNAME="hello"
 
start()
{
	echo "start $SERVERNAME"
	/home/yao/projects/$SERVERNAME
	echo "start $SERVERNAME ok!"
	exit 0;
}
 
stop()
{
	echo "stop $SERVERNAME"
	killall $SERVERNAME
	echo "stop $SERVERNAME ok!"
}
 
case "$1" in
start)
	start
	;;
stop)
	stop
	;;
restart)
	stop
	start
	;;
*)
	echo "usage: $0 start|stop|restart"
	exit 0;
esac
exit
```

4.更改脚本文件属性
```
chmod +x hello.sh
```

5.运行
```
root@localhost:vwa# service hello.sh start/stop/restart
```
