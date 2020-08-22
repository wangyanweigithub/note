
<!-- vim-markdown-toc GFM -->

* [500 lines web craw 翻译笔记 21](#500-lines-web-craw-翻译笔记-21)
	* [网络io的几个阶段  23](#网络io的几个阶段--23)
	* [本文三部分 30](#本文三部分-30)
		* [1. 阻塞socket  37](#1-阻塞socket--37)
		* [2. 非阻塞socket /Async 58](#2-非阻塞socket-async-58)
	* [3. selec、epoll、kqueue 轮询 89](#3-selecepollkqueue-轮询-89)
	* [4 回调 130](#4-回调-130)
	* [5 协同程序（Coroutines） 277](#5-协同程序coroutines-277)
	* [6. Python 生成器如何工作 292](#6-python-生成器如何工作-292)
		* [7. 用生成器搭建协程 394](#7-用生成器搭建协程-394)
		* [10. asyncio 575](#10-asyncio-575)
	* [总结： 578](#总结-578)
		* [异步实现方法： 580](#异步实现方法-580)
		* [Reactor,Pactor 595](#reactorpactor-595)
		* [faq 605](#faq-605)

<!-- vim-markdown-toc -->
# 500 lines web craw 翻译笔记 21

## 网络io的几个阶段  23
1. 阻塞的socket, connect, accept
2. 非阻塞的socket，while 循环检测
3. 非阻塞的soket， select/poll/epoll/kqueue 来轮询,注册函数来回调
4. 非阻塞socket，epoll..轮循，返回future，使用协程来并发大量socket
5. 使用非阻塞的socket，就必定要知道socket的状态什么时候准备好，或者while循环，或者阻塞在select/epoll处等待准备好的socket。这个循环基本上都是select/epoll的事件循环，等待触发的事件，这几个函数又有超时时间，所以也可以执行添加的任务，而不必非要等一个准备好的socket。

## 本文三部分 30
1. 异步事件循环框架，使用回调

2. 使用python生成器来实现携程

3. 使用全功能的asyncio标准库

### 1. 阻塞socket  37

```
def fetch(url):
    sock = socket.socket()
    sock.connect(('xkcd.com', 80))
    request = 'GET {} HTTP/1.0\r\nHost: xkcd.com\r\n\r\n'.format(url)
    sock.send(request.encode('ascii'))
    response = b''
    chunk = sock.recv(4096)
    while chunk:
        response += chunk
        chunk = sock.recv(4096)

    # Page is now downloaded.
    links = parse_links(response)
    q.add(links)
```

这时socket上的accept、connect、recv等操作都是阻塞的，这时只能开线程和进程，会大量消耗系统资源。解决不了C10k问题

### 2. 非阻塞socket /Async 58
	
```
sock = socket.socket()
sock.setblocking(False)
try:
	sock.connect(('xkcd.com', 80))
except BlockingIOError:
	pass

----------

request = 'GET {} HTTP/1.0\r\nHost: xkcd.com\r\n\r\n'.format(url)
encoded = request.encode('ascii')

while True:
	try:
		sock.send(encoded)
		break  # Done.
	except OSError as e:
		pass

print('sent')

```

这时socket的操作不是阻塞的，但是会立即返回，如果socket的操作没有成功，会立即返回异常。
其实这时没有有效利用异步，需要自己不停的检查是否成功。并且不能同时处理多个socket的事件。

unix 最初用select，后来linxu使用epoll，window使用select，BSD 使用kqueue.

## 3. selec、epoll、kqueue 轮询 89

- python3.4 DefaultSelector 会自动取得当前系统最优的轮询事件循环。
- select、epoll、kqueue的接口都差不多

		注册回到函数，取消注册
		from selectors import DefaultSelector, EVENT_WRITE
	
		selector = DefaultSelector()
		
		sock = socket.socket()
		sock.setblocking(False)
		try:
		    sock.connect(('xkcd.com', 80))
		except BlockingIOError:
		    pass
		
		def connected():
		    selector.unregister(sock.fileno())
		    print('connected!')
		
		selector.register(sock.fileno(), EVENT_WRITE, connected)

- 当在loop中节后到I/O通知时，在IOLoop中处理它。

	----------
		def loop():
		    while True:
		        events = selector.select()
		        for event_key, event_mask in events:
		            callback = event_key.data
		            callback()

- 连接的回调函数存储为event_key.data,一旦非阻塞套接字连接，我们将检索并执行它。selector.select是阻塞的，它会一直阻塞到有事件到来，然后调用callback来处理它。<font color=red>这里的callback还是阻塞的，这里的异步是socket是异步，然后可以同时处理大量的socket句柄。</font>

- 注意：<font color=green>**一个异步框架基于两个我们展示的功能：（非阻塞的套接字和事件循环）在单个线程上运行并发操作**</font>

	1. <font color=green>我们这里已经实现了“并发”，但不是传统上称之为”并行“的东西，也就是说，我们构建了一个可以重叠I/O的小型系统，它有能力在其他操作执行时，启动一个新的操作。它实际上没有利用多个内核来并行执行计算，但是这个系统是为了I/O约束问题而设计的，而不是cpu约束问题</font>

	2. <font color=red>这个事件循环在并发I/O方面效率很高，因为它不会为每个连接投入线程资源，但纠正一个常见的错误认识，即异步比多线程更快。通常情况下，它不是的。在事件循环服务少量非常活跃的连接时比多线程慢，在没有全局解释器锁的运行时中，线程在这样的工作负载上执行的更好。**异步I/O最适合的场景是有许多慢或者睡眠的连接和偶发事件的应用程序 </font>**

## 4 回调 130
1. 代码

		urls_todo = set(['/'])
		seen_urls = set(['/'])
	
		class Fetcher:
		    def __init__(self, url):
		        self.response = b''  # Empty array of bytes.
		        self.url = url
		        self.sock = None
	
		`#` Method on Fetcher class.
		    def fetch(self):
		        self.sock = socket.socket()
		        self.sock.setblocking(False)
		        try:
		            self.sock.connect(('xkcd.com', 80))
		        except BlockingIOError:
		            pass
		
		        # Register next callback.
		        selector.register(self.sock.fileno(),
		                          EVENT_WRITE,
		                          self.connected)

	1. The fetch method begins connecting a socket. But notice the method returns before the connection is established. It must return control to the event loop to wait for the connection. To understand why, imagine our whole application was structured so:

2. fetching

		# Begin fetching http://xkcd.com/353/
		fetcher = Fetcher('/353/')
		fetcher.fetch()
		
		while True:
		    events = selector.select()
		    for event_key, event_mask in events:
		        callback = event_key.data
		        callback(event_key, event_mask)

	1. 解释：
		All event notifications are processed in the event loop when it calls select. Hence fetch must hand control to the event loop, so that the program knows when the socket has connected. Only then does the loop run the connected callback, which was registered at the end of fetch above.

	2. 连接实现：Here is the implementation of connected:

		`#` Method on Fetcher class.
	    def connected(self, key, mask):
	        print('connected!')
	        selector.unregister(key.fd)
	        request = 'GET {} HTTP/1.0\r\nHost: xkcd.com\r\n\r\n'.format(self.url)
	        self.sock.send(request.encode('ascii'))
	
	        # Register the next callback.
	        selector.register(key.fd,
	                          EVENT_READ,
	                          self.read_response)

	3. 解释：<font color=red>一个真正的应用程序会检查发送的返回值，以防一次发送没有发送整个消息。但我们的请求很小而且request里会封装这些接口。它发送请求，等待响应。它必须注册另外一个回调函数并将控制权交给事件循环，下个回调函数是read_response处理服务器的回复</font>

3. 最后一个处理函数read_response
	1. 代码

		   ` #` Method on Fetcher class.
		    def read_response(self, key, mask):
		        global stopped
		
		        chunk = self.sock.recv(4096)  # 4k chunk size.
		        if chunk:
		            self.response += chunk
		        else:
		            selector.unregister(key.fd)  # Done reading.
		            links = self.parse_links()
		
		            # Python set-logic:
		            for link in links.difference(seen_urls):
		                urls_todo.add(link)
		                Fetcher(link).fetch()  # <- New Fetcher.
		
		            seen_urls.update(links)
		            urls_todo.remove(self.url)
		            if not urls_todo:
		                stopped = True

	2. 解释：The callback is executed each time the selector sees that the socket is "readable", <font color=red>which could mean two things: the socket has data or it is closed.</font>select默认水平触发。

	3. 注意：
		1. 上面代码socket每次接受4k数据，如果socket数据小于4k则可以将数据读完，如果大于4k，则进入下一个事件循环，会再次将这个socket返回可读，再次读取数据。直到socket关闭也就是可读事件但读取的数据为空，这时才可以人为这次socket数据接收完了，可以解除注册了。</font>

	4. 没有url任务了，设置stopped，停止事件循环

	```
	stopped = False
	
	def loop():
		while not stopped:
			events = selector.select()
			for event_key, event_mask in events:
				callback = event_key.data
				callback()
	```

	5. 总结：这个例子使得async的问题变得简单，你需要某种方式表示一系列的计算和io操作，并调度这些操作并发执行。但是<font color=red>如果没有多线程，这一系列操作不应该在一个函数中：一个函数开始一个I/O操作，它会明确的保存任何在将来需要的状态</font>

	Let us explain what we mean by that. Consider how simply we fetched a URL on a thread with a conventional blocking socket:

4.

	# Blocking version.
	def fetch(url):
	    sock = socket.socket()
	    sock.connect(('xkcd.com', 80))
	    request = 'GET {} HTTP/1.0\r\nHost: xkcd.com\r\n\r\n'.format(url)
	    sock.send(request.encode('ascii'))
	    response = b''
	    chunk = sock.recv(4096)
	    while chunk:
	        response += chunk
	        chunk = sock.recv(4096)
	
	    # Page is now downloaded.
	    links = parse_links(response)
	    q.add(links)

这个函数是怎么记住io状态和io完成后需要调用的指令？socket.recv是一个io操作，不消耗cpu。程序是怎么知道IO是否已经完成，怎么从IO操作返回到程序，怎么知道io返回时需要调用的指令？这些我们都不用关心，这些是系统和底层语言实现的。系统会记住IO的状态和下个指令的指针。

但是使用基于回调的异步框架，这些语言功能没有任何帮助。<font color=green>这说明socket的IO操作，系统同会记住socket的IO状态和下个指令的地址，这样socket IO状态改变时，系统调用会让socket排队获得cpu，继续执行下条指令，就相当于系统在IO状态改变时，回调socket接下来的指令。**这里可能是只有阻塞socket才可以这样**</font>因此基于回调的异步框架在等待IO时，函数必须显式的保存它的状态，因为函数在IO完成之前返回<font color=red>并丢失他的堆栈帧</font>，而阻塞的socket在同一个函数中完成，函数堆栈还存在。

上例中，我们把函数栈中的变量作为自己的属性(socket，response)来代替系统保存的属性，然后将回调函数指针注册到事件循环来代替系统回调的指令指针。<font color=green>这里没有用到了闭包，注册的回调函数能访问到对象的属性是因为对象方法的第一个参数就是self，
c++中的对象方法第一个参数也是对象本身，而所有的方法都在代码区。python里注册回调函数时的self.method其实就相当于method(self),它已经绑定了self对象到第一个参数。偏函数</font>

5. 不足之处，随着回调方法的增加会越来越麻烦，而且链式调用的抛出异常不容易定位。
	
		Traceback (most recent call last):
		  File "loop-with-callbacks.py", line 111, in <module>
		    loop()
		  File "loop-with-callbacks.py", line 106, in loop
		    callback(event_key, event_mask)
		  File "loop-with-callbacks.py", line 51, in read_response
		    links = self.parse_links()
		  File "loop-with-callbacks.py", line 67, in parse_links
		    raise Exception('parse error')
		Exception: parse error
	
	try excetp 也解决不了。

	问题：多线程在于数据竞争，而回调则在于堆栈翻录

## 5 协同程序（Coroutines） 277
通过将高效的回调以经典的多线程外观结合在一起，组合成协同程序的模式。

> 协程：它是一个可以暂停和恢复的子程序。

尽管多线程备操作系统赋予多任务处理的功能，协程也可以协同处理多任务：他们选择合适暂停，以及接下来运行那个协程。

python 3.4 的标准库的协程是基于生成器、Future类和“yield from” 语句来构建的。
python 3.5开始语言内置了协程。

	@asyncio.coroutine
    def fetch(self, url):
        response = yield from self.session.get(url)
        body = yield from response.read()

## 6. Python 生成器如何工作 292

1. 普通函数的调用流程：

	1. 正常python调用函数流程：python 函数调用另一个函数，这个子函数将获得控制权知道它完成退出或者抛出异常为止，然后控制权返回给顶层函数。
	
	2. Python的解释器是C写的，执行Python 函数的C函数叫做PyEval_EvalFrameEx。它会获取一个Python堆栈帧对象并且在帧的上下文中评估Python的字节码。用dis.dis可以看到一个函数的字节码。
	
			>>> def foo():
			...     bar()
			...
			>>> def bar():
			...     pass
	
			>>> import dis
			>>> dis.dis(foo)
	  		2         0 LOAD_GLOBAL              0 (bar)
		              3 CALL_FUNCTION            0 (0 positional, 0 keyword pair)
		              6 POP_TOP
		              7 LOAD_CONST               0 (None)
		             10 RETURN_VALUE
	
		1. foo 函数加载bar到foo本身的栈帧中，并且调用它，然后从栈帧中弹出bar的返回值，然后加载None到栈帧并且返回None。
		
		2. 当PyEval_EvalFrameEx 遇到CALL_FUNCTION字节码时，它会创建一个新的栈帧并且递归调用它。
		
		3. <font color=red>Python堆栈帧被分配到堆内存</font>，Python 解释器是一个普通的C程序，所以它的堆栈帧是正常的堆栈帧，但是它操作的Python堆栈帧是在堆上的。
	
				>>> import inspect
				>>> frame = None
				>>> def foo():
				...     bar()
				...
				>>> def bar():
				...     global frame
				...     frame = inspect.currentframe()
				...
				>>> foo()
				>>> # The frame was executing the code for 'bar'.
				>>> frame.f_code.co_name
				'bar'
				>>> # Its back pointer refers to the frame for 'foo'.
				>>> caller_frame = frame.f_back
				>>> caller_frame.f_code.co_name
				'foo'
	
		<font color=red>这里foo结束后，函数空间中的变量应该都回收了，但是他的栈帧依旧存在。</font>
	
	![Figure1-Function Calls](G:\picture\program_graph\web_asyncio_craw1.jpg)

2. 生成器调用流程：
	1. 
		>它使用和普通函数调用相同的构建块 - 代码对象和堆栈帧 - 以获得奇妙的效果。
	
			>>> def gen_fn():
			...     result = yield 1
			...     print('result of yield: {}'.format(result))
			...     result2 = yield 2
			...     print('result of 2nd yield: {}'.format(result2))
			...     return 'done'
			...     
	
		当Python解释器将gen_fn编译成字节码是，它看到<font color=red>yield</font>语句就知道gen\_fn是一个生成器函数而不是普通函数。它会设置一个标识来记住这个事实。   
	
			>>> # The generator flag is bit position 5.
			>>> generator_bit = 1 << 5
			>>> bool(gen_fn.__code__.co_flags & generator_bit)
			True
		<font color=green>生成器函数的标识是32</font>
	
	2. 
		1. 当你调用一个生成器函数时，Python解释器看到生成器的标志，实际上它没有运行这个函数，相反，它创建了一个生成器。
	
				>>> gen = gen_fn()
				>>> type(gen)
				<class 'generator'>

		2. python 生成器封装了一个堆栈帧和一些gen_fn主体代码的引用
				
				>>> gen.gi_code.co_name
				'gen_fn'
		
		3. 从调用gen_fn生成的所有生成器都指向同样的代码，但是每个生成器都有自己的堆栈帧。这个堆栈帧不在任何实际堆栈上，它在堆内存中等待被使用。

		![Figure2-Generators](G:\picture\program_graph\web_asyncio_craw2.jpg)
		
			<font color=red>堆栈帧中记录着“最后指令”的指针，它是最近执行的指令。在开始时，最后治指令指针是-1，意味着生成器还没有开始</font>
		
				>>> gen.gi_frame.f_lasti
				-1
		
		4. 
			1. a = yield x ，相当于两步：yield x； a=yield。第一步在yield 返回值时会交出控制权。
			2. send(None)相当于next().第一步一定要send(None)或者next，因为如果需要传入值时，第一步肯定是先yield出值那步，那个步骤不需要传入参数，仅仅返回。

		5. 生成器可以在任何时间被任何函数唤醒，因为它不在函数的栈帧上，它在堆上。它在函数调用层次结构上的位置是不固定的，它也不用遵守先进后出的执行顺序，它被解放了。

		6. 当执行完next后，在send一次，这时生成器执行到第二个yield，这时生成器多了一个result的本地变量，说明函数刚执行到赋值给第一个yield那里。
		
		7. 生成器完成时会抛出一个StopIteration异常，这个异常有一个值，它是生成器返回的：string “done”


### 7. 用生成器搭建协程 394
1. future
```
class Future:
	def __init__(self):
		self.result = None
		self._callbacks = []

	def add_done_callback(self, fn):
		self._callbacks.append(fn)

	def set_result(self, result):
		self.result = result
		for fn in self._callbacks:
			fn(self)
```
2. fetch
```
def fetch(self):
	socket = socket.socket()
	socket.setblocking(False)

	try:
		socket.connect(('xkcd.com', 80))
	except BlockingIOError:
		pass

	f = futre()
	
	def on_connected():
		f.set_result(None)
	
	selector.register(sock.fileno(), EVENt_WRITE, on_connected)

	yield f

	selector.unregister(socket.fileno())
	print('connected!')
```

	一般yield future之后，注册的handler会设置future的set_result,这样需要处理future结果的语句会阻塞
	到future的get_result，或者另起一个线程使用notify信号。

3. yield将cpu控制权转让出去，就一定要有人将控制权再次传递到协成中，来唤醒协程。

```
class Task:
	def __init__(self, coro):
		self.coro = coro
		f = Future()
		f.set_result(None)
		self.step(f)

	def setp(self, future):
		try:
			next_future = self.coro.send(future.result)
		except StopIteration:
			return
		next_future.add_done_callback(self.step)

#Begin fetching  454

fetcher = Fetcher('/353/')
Task(fetcher.fetch())

loop()
 ```

	1. <font color=red>这里的step，每次返回的future任务完成时，都会调用step，将这个future的结果通过send传递回协程中，
	协程下面的代码可以认为上一行future是已经有结果的。这样协程中好像是同步一样。</font>

	2. <font color=green>而且每个future都会添加step函数，这样不管返回多少个future，
	都可以使协程往下执行。</font> 

	3. task通过send None 给fetch生成器来启动他，然后fetch运行直到yield 一个future。task捕获future作为next_future.当socket链接成功时，事件循环运行on_connected，来设置future的result，然后future调用step，来重新启动协程fetch。

### 8. 通过yield from 分解协程 470
1. 现在的fetch

```
def fetch():
	...
	socket.send(request.encode('ascii'))
	while True:
		f = Future()

		def on_readable():
			f.set_result(sock.recv(4096))

		selector.register(sock.fileno(), EVENt_READ, on_readable)
		chunk = yield f
		selector.unregister(sock.fileno())
		if chunk:
			self.responese += chunk
		else:
			break;
```

2. 通过yield from 分解协程，是的协程调用分解的子协程就像调用普通函数一样方便。

```
def read(sock):
	f = Future()
	
	def on_readable():
		f.set_result(socket.recv(4096))

	selector.register(sock.fileno(), EVENT_READ, on_readable)
	chunk = yield f
	selector.unregister(sock.fileno())
	return chunk;

def read_all(sock):
	response = []
	chunk = yield from read(sock)
	while chunk:
		response.append(chunk)
		chunk = yield from read(sock)
	return b''.join(response)
	 
class Fetcher:
	def fetch(self):
		...
		sock.send(reqeust.encode("ascii"))
		self.response = yield from read_all(sock)
```
	1. <font color=red>使用yield form 传递委托后，被yield from 使用的生成器的StopIterator Error就已经捕获了，return的值返回给了yield from表达式。只需要捕获最外面的生成器的异常就可以了。就是在task中step的try。用来捕获最外层异常。如果不使用yield from 的话，生成器的return返回值会在Exception中出现</font>

### 9. 为生成器添加iter接口 522
```
class Future:
	def __iter__(self):
		yield self
		return self.result
```

1. 这里解释一下：
	
```
>>> def gen():
...   yield 1
...
>>> def gen_dele():
...   yield from gen()
...
>>> dis.dis(gen_dele)
  2           0 LOAD_GLOBAL              0 (gen)
              2 CALL_FUNCTION            0
              4 GET_YIELD_FROM_ITER
              6 LOAD_CONST               0 (None)
              8 YIELD_FROM
             10 POP_TOP
             12 LOAD_CONST               0 (None)
             14 RETURN_VALUE
>>> type(gen_dele)
<class 'function'>
>>> type(gen_dele())
<class 'generator'>
>>>

```
2. python文档：

```
GET_YIELD_FROM_ITER
如果 "TOS" 是一个 *generator iterator* 或 *coroutine* 对象则保持原样。否则实现 "TOS = iter(TOS)" 。
```
3. 文档：generator iterator -- 生成器迭代器

   *generator* 函数所创建的对象。
   
   每个 "yield" 会临时暂停处理，记住当前位置执行状态（包括局部变量和挂
   起的 try 语句）。当该 *生成器迭代器* 恢复时，它会从离开位置继续执行
  （这与每次调用都从新开始的普通函数差别很大）。

4. 可以看出，一个yield from 调用了GET_YIELD_FROM_ITER,而GET_YIELD_FROM_ITER调用了iter()返回一个generator iterator，所以会先调用future的__iter__方法。

5. 注意一个函数中只要有yield，解释器就会把它解释为一个生成器。而一个类里面有yield，解释器做其余操作，他还是会一个类，调用它会生成一个对象。

6. 所以，yield from future后，先调用future的__iter__,yield future本身，然后task send future的result，future return 结果。

### 10. asyncio 575
1. asyncio.Queue
	Queue.get方法是一个本身就是一个协程，只有queue中没有数据的话，会阻塞，直到queue中有数据，才会返回item。
## 总结： 578

### 异步实现方法： 580
1. python的异步IO主要是通过事件循环来实现的异步阻塞IO，主要用在socket的io上面，普通文件的io依旧会阻塞，

2. linux系统内核有两种AIO，
	1. 一种是通过多线程和进程来模拟实现异步IO的
	2. 另一中真正的的AIO

3. python实现的异步包括两种
	
	1. 通过多线程或者多进程来模拟异步IO，python的cocurrent就是用这种方法来让本身不是异步的任务达到异步的预期，
		tornado使用异步装饰器来通过cocurrent的exector来将同步的任务模拟成异步任务。
	
	2. 通过epoll/select/kqueue 和非阻塞socket来实现socket的异步。
		asyncio、tornado、zeromq中的事件循环就是通过这样来实现异步网络IO的异步的。

### Reactor,Pactor 595


参考：[500 lines craw](http://aosabook.org/en/500L/pages/a-web-crawler-with-asyncio-coroutines.html)


![Figure3.3-Yield From](G:\picture\program_graph\web_asyncio_craw3.jpg)

![Figure3.4-Redirects](G:\picture\program_graph\web_asyncio_craw4.jpg)

### faq 605
