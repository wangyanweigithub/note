## 协程
### 概述
1. **python的异步IO主要是通过事件循环来实现的异步阻塞IO，主要用在socket的io上面，普通文件的io依旧会阻塞，
linux系统内核有两种AIO，一种是通过多线程和进程来模拟实现异步IO的，另一中真正的的AIO，不能缓存，直接写入磁盘**

2. python是实现的异步也包括两种
	1. 通过多线程或者多进程来模拟异步IO，python的cocurrent就是用这种方法来让本身不是异步的任务达到异步的预期，
	tornado使用异步装饰器来通过cocurrent的exector来将同步的任务模拟成异步任务。
		        
	2. 通过epoll/select/kqueue 和非阻塞socket来实现socket的异步。
	asyncio、tornado、zeromq中的事件循环就是通过这样来实现异步网络IO的异步的。
