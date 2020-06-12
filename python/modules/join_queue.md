﻿### joinableQueue
JoinableQueue比Queue多了个task_done()和join()函数。都是用来进行多线程编程的，都是生产者消费者模型。

1. task_done()是用在get()后，告诉queue我已经消费完这个item了,让queue的join等待的那个变量减一。
```
 self.all_tasks_done.acquire()
 try:
	 while self.unfinished_tasks:    # unfinished_tasks保存了当前队列中的数目
		 self.all_tasks_done.wait()
 finally:
	 self.all_tasks_done.release()
```
	1. 上面代码里有个while循环，unfinished\_tasks存储了当前队列里的数据数目，如果unfinished\_tasks不为0，
		将会一直调用all\_tasks\_done.wait()阻塞住。只要unfinished\_tasks不是0，join就可以一直工作下去。 
		而task\_done()正是减小了unfinished_tasks。
	
	2. 不妨再看看task_done()的实现代码： 
	```
	 self.all_tasks_done.acquire()
	 try:
		 unfinished = self.unfinished_tasks - 1
		 if unfinished <= 0:
			 if unfinished < 0:
				 raise ValueError('task_done() called too many times')
			 self.all_tasks_done.notify_all()
		 self.unfinished_tasks = unfinished
	 finally:
		 self.all_tasks_done.release()

	```

	注意: 表明每次执行了task_done()，unfinished_tasks就减1，直到unfinished_tasks为0。 

	3. <font color=red>如果没有调用task_done而调用了queue的join，会导致主线程一直等不到join返回</font>

2. 原文链接：[JoinableQueue 用于Python多进程生产者消费者问题的注意事项](https://blog.csdn.net/heavendai/article/details/24380325)