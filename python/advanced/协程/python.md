## python的几种函数形式
### 普通函数
def function():
	return 1

print(type(function))
>>> function

### 生成器函数
def generator():
	yield 1

print(type(coro()))
>>> generator

### 协程/异步协程
async def coro():
	return 1

print(type(coro()))
>>> coroutine

### 异步生成器
> async_generator

1. 只有async def 定义的协程中出现yield时，函数返回的是异步生成器

2. AsyncGenerator的定义，
它需要实现__aiter__和__anext__两个核心方法，以及asend，athrow，aclose方法。

3. 异步生成器要先asend后返回的是一个生成器，然后接着send,就是生成器的操作了。

4. <font color=green>await 不能用于异步生成器</font>。
async def async_coro():
	yield 1

print(type(async_coro()))

>>> async_generator

## 协程关键字
### yield
#### yield 生成器函数的特点
1. yield, yield from 的函数返回的是一个生成器。

2. 生成器对象需要send和next

3. yield from只针对生成器和iterable

### await #### async def 的特点
1. async def 声明的函数返回一个coroutine

2. coroutine可以通过调用这个协程的send方法给协程发送一个值。

3. 协程对象在最底层还是通过yield 实现的

4. await 和yield 的区别仅仅是await判断了一下 可等待对象的类型。

5. 通过types.coroutine可以是的生成器可以和协程通用。

6. types.coroutine只是将生成器加了一个标志位，这个标志位和原生协程有区别，但是await会执行这种由生成器转换的协程

7. async def声明的函数没有用await的话，和生成器函数的协程是一样的:
	- 都是send(None)来启动协程，然后send发送value到协程中
	- 运行完后都会报StopIteration
	
8. 不同之处：
	- coroutine 不是iterable的，只能调用send，而不能调用next
	
#### await 的特点
1. await只能等待
	- 实现`__await__`的对象
	- 等待原生的coroutine对象
	- 由types.coroutine修饰的生成器对象

2. await语法只能出现在通过async修饰的函数中，否则会报SyntaxError错误

3. await后面的对象需要是一个Awaitable，或者实现了相关的协议。

4. 在协程函数中，可以通过await语法来挂起自身的协程，并等待另一个协程完成直到返回结果


## 其他
### 没有异步的协程:
> 最底层协程没有异步，没有yield交出执行权。

1. 代码

async def a():
    print("this is a")
    return 2

async def b():
    c = await a()
    print("c is ", c)

d = b()
try:
    e = d.send(None)
except:
    pass
﻿
<!-- vim-markdown-toc GFM -->

* [python的几种函数形式]
	* [普通函数]
	* [生成器函数]
	* [协程/异步协程]
	* [异步生成器]
* [协程关键字]
	* [yield]
		* [yield 生成器函数的特点]
	* [await]
		* [async def 的特点]
		* [await 的特点]
* [其他]
	* [没有异步的协程:]

<!-- vim-markdown-toc -->

## python的几种函数形式
### 普通函数
```
def function():
	return 1

print(type(function))

>>> function
```

### 生成器函数
```
def generator():
	yield 1

print(type(coro()))

>>> generator
```

### 协程/异步协程
```
async def coro():
	return 1

print(type(coro()))

>>> coroutine
```

### 异步生成器
> async_generator

1. 只有async def 定义的协程中出现yield时，函数返回的是异步生成器

2. AsyncGenerator的定义，
它需要实现__aiter__和__anext__两个核心方法，以及asend，athrow，aclose方法。

3. 异步生成器要先asend后返回的是一个生成器，然后接着send,就是生成器的操作了。

4. <font color=green>await 不能用于异步生成器</font>。
```
async def async_coro():
	yield 1

print(type(async_coro()))

>>> async_generator
```

## 协程关键字
### yield
#### yield 生成器函数的特点
1. yield, yield from 的函数返回的是一个生成器。

2. 生成器对象需要send和next

3. yield from只针对生成器和iterable

### await
#### async def 的特点
1. async def 声明的函数返回一个coroutine

2. coroutine可以通过调用这个协程的send方法给协程发送一个值。

3. 协程对象在最底层还是通过yield 实现的

4. await 和yield 的区别仅仅是await判断了一下 可等待对象的类型。

5. 通过types.coroutine可以是的生成器可以和协程通用。

6. types.coroutine只是将生成器加了一个标志位，这个标志位和原生协程有区别，但是await会执行这种由生成器转换的协程

7. async def声明的函数没有用await的话，和生成器函数的协程是一样的:
	- 都是send(None)来启动协程，然后send发送value到协程中
	- 运行完后都会报StopIteration
	
8. 不同之处：
	- coroutine 不是iterable的，只能调用send，而不能调用next
	
#### await 的特点
1. await只能等待
	- 实现`__await__`的对象
	- 等待原生的coroutine对象
	- 由types.coroutine修饰的生成器对象

2. await语法只能出现在通过async修饰的函数中，否则会报SyntaxError错误

3. await后面的对象需要是一个Awaitable，或者实现了相关的协议。

4. 在协程函数中，可以通过await语法来挂起自身的协程，并等待另一个协程完成直到返回结果


## 其他
### 没有异步的协程:
> 最底层协程没有异步，没有yield交出执行权。

1. 代码

```
async def a():
    print("this is a")
    return 2

async def b():
    c = await a()
    print("c is ", c)

d = b()
try:
    e = d.send(None)
except:
    pass
```
﻿<!-- vim-markdown-toc GFM -->

* [python 异步和事件驱动模型]
	* [1、协程]
		* [使用方法如下]
	* [2、yield from]
	* [3. yield from 和迭代器]
	* [4、事件循环：]

<!-- vim-markdown-toc -->
# python 异步和事件驱动模型

## 1、协程
>协程 是为非抢占式多任务产生子程序的计算机程序组件，协程允许不同入口点在不同位置暂停或开始执行程序

1. 协程就是你可以暂停执行的函数，可以理解成就像生成器一样。
2. 原来生成器完全就是关于迭代器的，可以比迭代对象节省内存空间，但是当利用生成器“暂停”的部分，
	添加“将东西发送会生成器内”的功能，那么Python突然就有了协程的概念，PEP 342为生成器引入了send()方法，
	使得我们不仅仅可以暂停生成器，还能够传递值到生成器暂停的地方。

	```
	jump = yield index

	相当于

	yield index
	jump = () send传进来的值，注意程序在yield那里中断，也就是1的位置。如果yield后没有变量，则每次返回None
	
	```
		
* 协程的四种状态
  
　协程有四个状态，可以使用inspect.getgeneratorstate(...)函数确定：

　　1、GEN_CREATED # 等待开始执行，生成器初始化后的状态，生成器还未激活

　　2、GEN_RUNNING # 解释器正在执行（只有在多线程应用中才能看到这个状态）

　　3、GEN_SUSPENDED # 在yield表达式处暂停。只用处于这个状态的生成器才可以调用send发送None以外的值。生成器初始化后调用next预激后会处于这个状态

　　4、GEN_CLOSED # 执行结束，抛出异常或者关闭生成器后处于这种状态。

* 用装饰器实现生成器的预激
	
		from functools import wraps

		def coroutinue(func):
		    @wraps(func)
		    def primer(*args, **kwargs):
		        # 把装饰器生成器函数替换成这里的primer函数；调用primer函数时，返回预激后的生成器。
		        gen = func(*args, **kwargs)        # 调用被被装饰函数，获取生成器对象
		        next(gen)  # 预激生成器
		        return gen  # 返回生成器
		    return primer
		
		@coroutinue
		def simple_coro(a):
    		a = yield

		simple_coro(12)  # 已经预激

### 使用方法如下

	@coroutinue
	def simple_coro(a):
	    a = yield

simple_coro(12)  # 已经预激



## 2、yield from
>yield from 是 Python3.3 后新加的语言结构。和其他语言的await关键字类似，它表示：*在生成器 gen 中使用 yield from subgen()时，subgen 会获得控制权，把产出的值传个gen的调用方，即调用方可以直接控制subgen。于此同时，gen会阻塞，等待subgen终止。

* Python3.5中的async/await的原理是由Python 3.3中的yield from 和Python3.4中的asyncio 组合得来。

* yield from iterator 本质上相当于：

	**yield from 相当于下面表达式，所有yield 表达式必须在函数内出现**

		for x in iterator： #注意这里是可迭代对象，包括生成器和和迭代对象
			yield x

* 在for循环中，yield from 会首先调用生成器的\_\_iter\_\_方法，然后会调用返回对象的\_\_next\_\_方法，``__iter__``只调用一次，next每次循环都要调用。

		class B(object):
	
		    def __init__(self):
		        self.i = 0
		
		    def __iter__(self):
		        print("iter")
		        return self
		
		    def __next__(self):
		        print("next")
		        if self.i < 10:
		            self.i += 1
		            return self.i
		        else:
		            raise StopIteration
	
			def gen(a):
	    		yield from a


		iter
		next
		1
		next
		2
		next
		3

* yeild from 可以将生成器串联起来，使返回值可以在调用栈中上下浮动，而不需要对编码进行过多改动。

* yield from 结果会在内部自动捕获StopIteration 异常。这种处理方式与 for 循环处理StopIteration异常的方式一样。 对于yield from 结构来说，解释器不仅会捕获StopIteration异常，还会把value属性的值变成yield from 表达式的值。

* yield from 可用于简化for循环中的yield表达式。
 
		def gen():
		    for c in 'AB':
		        yield c
		    for i in range(1, 3):
		        yield i
		
		list(gen())
		['A', 'B', '1', '2']
可以改写成

		def gen():
		    yield from 'AB'
		    yield from range(1, 3)
		    
		
		list(gen())
		['A', 'B', '1', '2']

* <font color=green>yield from x 表达式对x对象做的第一件事是，调用 iter(x)，获取迭代器。所以要求x是可迭代对象。如果x有\_\_iter\_\_方法会调用这个方法</font>

　　PEP380 的标题是 ”syntax for delegating to subgenerator“(把指责委托给子生成器的句法)。由此我们可以知道，yield from是可以实现嵌套生成器的使用。

　　yield from 的主要功能是打开双向通道，把最外层的调用方与最内层的子生成器连接起来，使两者可以直接发送和产出值，还可以直接传入异常，而不用在中间的协程添加异常处理的代码。


* <font color=red>最内层的生成器的return的返回值，是外层的yield from的返回值</font>

		rv = yield from gen
		gen return的值是rv的值	

**混用yield 和return时，for循环和next都不能获得return的值，return位置是yield循环完之后抛出StopIteratorError的，所以如果renturn的不是这个错误，那么return的值也在异常捕获哪里获取**
	

* yield 详解

: https://segmentfault.com/a/1190000009781688

　　**作者说这是流畅的python的读书笔记**


## 3. yield from 和迭代器
* <font color=green>yield from x 表达式对x对象做的第一件事是，调用 iter(x)，获取迭代器。所以要求x是可迭代对象。如果x有\_\_iter\_\_方法会调用这个方法</font>

* 如果yield from 后面跟的对象是生成器对象，iter返回对象本身，对象的\_\_next\_\_方法执行返回yield的值。如果是可迭代对象，则会先调用iter获得迭代器，然后执行next方法，next方法里面不要再用yield，因为yield from就是直接调用next方法，如果返回值继续用yield，则yield from返回的还是一个生成器对象。

* <font color=green>如果yield from后的对象调用iter返回的对象是一个普通迭代对象，则yield from会调用迭代对象的next方法。如果iter返回的是一个生成器，则yield from会打通到这个生成器的通道，send、next都会传递到这个对象，不会继续调用next方法</font>

		class A(object):
	    def __init__(self):
	        self.index = 0
	
	    def __iter__(self):
	        print('!!!!!')
	        yield self
	        print("?????????????")
	        return self
	
	    def __next__(self):
	        if self.index < 10:
	            self.index += 1
	            return self.index
	        else:
	            raise StopIteration
	
		def gen(a):
		    yield from a
		
		# gentor = gen(A())
		
		for i in gen(A()):
		    print(i)

		!!!!!
		1
		?????????????

	如果把\_\_iter\_\_的前三行注释掉，

		1
		2
		3
		4
		5
		6
		7
		8
		9
		10


## 4、事件循环：
>事件循环是一种等待程序分配事件或消息的编程架构。基本上来说事件循环就是。"当A发生时，就执行B".

　**asyncio是事件循环框架可以进行异步编程**

1. 　asyncio重点解决网络服务中的问题，事件循环在这里将来自套接字socket的I/O已经准备好的读和写作为"当A发生时"（通过selectors模块).除了GUI和I/O,事件循环也经常用于在别的线程或子进程中执行的代码，并将事件循环作为调节机制（例如，合作式多任务）。事件循环对于需要释放GIL的地方很有用



参考文章：

1. https://mp.weixin.qq.com/s?__biz=MzAwNjI5MjAzNw==&mid=2655751983&idx=1&sn=e4c093c6e5d6e4e8281d76db7c67eb23
2. https://juejin.im/entry/56ea295ed342d300546e1e22“

<!-- vim-markdown-toc GFM -->

* [A Web Crawler With asyncio Coroutines 18](#a-web-crawler-with-asyncio-coroutines-18)
	* [Introduction 25](#introduction-25)
	* [The Task 32](#the-task-32)
	* [The Traditional Approach 38](#the-traditional-approach-38)
	* [Async 69](#async-69)
	* [Programming With Callbacks 148](#programming-with-callbacks-148)
	* [Coroutines 314](#coroutines-314)
	* [How Python Generators Work 333](#how-python-generators-work-333)
	* [Building Coroutines With Generators 481](#building-coroutines-with-generators-481)
	* [Factoring Coroutines With `yield from` 577](#factoring-coroutines-with-yield-from-577)
	* [Coordinating Coroutines 790](#coordinating-coroutines-790)
	* [Conclusion 1100](#conclusion-1100)

<!-- vim-markdown-toc -->
# A Web Crawler With asyncio Coroutines 18
> author: A. Jesse Jiryu Davis and Guido van Rossum

1. _A. Jesse Jiryu Davis is a staff engineer at MongoDB in New York. He wrote Motor, the async MongoDB Python driver, and he is the lead developer of the MongoDB C Driver and a member of the PyMongo team. He contributes to asyncio and Tornado. He writes at [http://emptysqua.re](http://emptysqua.re)._

2. _Guido van Rossum is the creator of Python, one of the major programming languages on and off the web. The Python community refers to him as the BDFL (Benevolent Dictator For Life), a title straight from a Monty Python skit.  Guido's home on the web is [http://www.python.org/~guido/](http://www.python.org/~guido/)._

## Introduction 25
1. Classical computer science emphasizes efficient algorithms that complete computations as quickly as possible. But many networked programs spend their time not computing, but holding open many connections that are slow, or have infrequent events. These programs present a very different challenge: to wait for a huge number of network events efficiently. A contemporary approach to this problem is asynchronous I/O, or "async".

2. This chapter presents a simple web crawler. The crawler is an archetypal async application because it waits for many responses, but does little computation. The more pages it can fetch at once, the sooner it completes. If it devotes a thread to each in-flight request, then as the number of concurrent requests rises it will run out of memory or other thread-related resource before it runs out of sockets. It avoids the need for threads by using asynchronous I/O.

We present the example in three stages. First, we show an async event loop and sketch a crawler that uses the event loop with callbacks: it is very efficient, but extending it to more complex problems would lead to unmanageable spaghetti code. Second, therefore, we show that Python coroutines are both efficient and extensible. We implement simple coroutines in Python using generator functions. In the third stage, we use the full-featured coroutines from Python's standard "asyncio" library[^16], and coordinate them using an async queue.

## The Task 32

A web crawler finds and downloads all pages on a website, perhaps to archive or index them. Beginning with a root URL, it fetches each page, parses it for links to unseen pages, and adds these to a queue. It stops when it fetches a page with no unseen links and the queue is empty.

We can hasten this process by downloading many pages concurrently. As the crawler finds new links, it launches simultaneous fetch operations for the new pages on separate sockets. It parses responses as they arrive, adding new links to the queue. There may come some point of diminishing returns where too much concurrency degrades performance, so we cap the number of concurrent requests, and leave the remaining links in the queue until some in-flight requests complete.

## The Traditional Approach 38

How do we make the crawler concurrent? Traditionally we would create a thread pool. Each thread would be in charge of downloading one page at a time over a socket. For example, to download a page from `xkcd.com`:

```python
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

By default, socket operations are *blocking*: when the thread calls a method like `connect` or `recv`, it pauses until the operation completes.[^15] Consequently to download many pages at once, we need many threads. A sophisticated application amortizes the cost of thread-creation by keeping idle threads in a thread pool, then checking them out to reuse them for subsequent tasks; it does the same with sockets in a connection pool.

And yet, threads are expensive, and operating systems enforce a variety of hard caps on the number of threads a process, user, or machine may have. On Jesse's system, a Python thread costs around 50k of memory, and starting tens of thousands of threads causes failures. If we scale up to tens of thousands of simultaneous operations on concurrent sockets, we run out of threads before we run out of sockets. Per-thread overhead or system limits on threads are the bottleneck.

In his influential article "The C10K problem"[^8], Dan Kegel outlines the limitations of multithreading for I/O concurrency. He begins,

> It's time for web servers to handle ten thousand clients simultaneously, don't you think? After all, the web is a big place now.

Kegel coined the term "C10K" in 1999. Ten thousand connections sounds dainty now, but the problem has changed only in size, not in kind. Back then, using a thread per connection for C10K was impractical. Now the cap is orders of magnitude higher. Indeed, our toy web crawler would work just fine with threads. Yet for very large scale applications, with hundreds of thousands of connections, the cap remains: there is a limit beyond which most systems can still create sockets, but have run out of threads. How can we overcome this?

## Async 69

Asynchronous I/O frameworks do concurrent operations on a single thread using
*non-blocking* sockets. In our async crawler, we set the socket non-blocking
before we begin to connect to the server:

```python
sock = socket.socket()
sock.setblocking(False)
try:
    sock.connect(('xkcd.com', 80))
except BlockingIOError:
    pass
```

Irritatingly, a non-blocking socket throws an exception from `connect`, even when it is working normally. This exception replicates the irritating behavior of the underlying C function, which sets `errno` to `EINPROGRESS` to tell you it has begun.

Now our crawler needs a way to know when the connection is established, so it can send the HTTP request. We could simply keep trying in a tight loop:

```python
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

This method not only wastes electricity, but it cannot efficiently await events on *multiple* sockets. In ancient times, BSD Unix's solution to this problem was `select`, a C function that waits for an event to occur on a non-blocking socket or a small array of them. Nowadays the demand for Internet applications with huge numbers of connections has led to replacements like `poll`, then `kqueue` on BSD and `epoll` on Linux. These APIs are similar to `select`, but perform well with very large numbers of connections.

Python 3.4's `DefaultSelector` uses the best `select`-like function available on your system. To register for notifications about network I/O, we create a non-blocking socket and register it with the default selector:

```python
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
```

We disregard the spurious error and call `selector.register`, passing in the socket's file descriptor and a constant that expresses what event we are waiting for. To be notified when the connection is established, we pass `EVENT_WRITE`: that is, we want to know when the socket is "writable". We also pass a Python function, `connected`, to run when that event occurs. Such a function is known as a *callback*.

We process I/O notifications as the selector receives them, in a loop:

```python
def loop():
    while True:
        events = selector.select()
        for event_key, event_mask in events:
            callback = event_key.data
            callback()
```

The `connected` callback is stored as `event_key.data`, which we retrieve and execute once the non-blocking socket is connected.

Unlike in our fast-spinning loop above, the call to `select` here pauses, awaiting the next I/O events. Then the loop runs callbacks that are waiting for these events. Operations that have not completed remain pending until some future tick of the event loop.

What have we demonstrated already? We showed how to begin an operation and execute a callback when the operation is ready. An async *framework* builds on the two features we have shown&mdash;non-blocking sockets and the event loop&mdash;to run concurrent operations on a single thread.

We have achieved "concurrency" here, but not what is traditionally called "parallelism". That is, we built a tiny system that does overlapping I/O. It is capable of beginning new operations while others are in flight. It does not actually utilize multiple cores to execute computation in parallel. But then, this system is designed for I/O-bound problems, not CPU-bound ones.[^14]

So our event loop is efficient at concurrent I/O because it does not devote thread resources to each connection. But before we proceed, it is important to correct a common misapprehension that async is *faster* than multithreading. Often it is not&mdash;indeed, in Python, an event loop like ours is moderately slower than multithreading at serving a small number of very active connections. In a runtime without a global interpreter lock, threads would perform even better on such a workload. What asynchronous I/O is right for, is applications with many slow or sleepy connections with infrequent events.[^11]<latex>[^bayer]</latex>

## Programming With Callbacks 148

With the runty async framework we have built so far, how can we build a web crawler? Even a simple URL-fetcher is painful to write.

We begin with global sets of the URLs we have yet to fetch, and the URLs we have seen:

```python
urls_todo = set(['/'])
seen_urls = set(['/'])
```

The `seen_urls` set includes `urls_todo` plus completed URLs. The two sets are initialized with the root URL "/".

Fetching a page will require a series of callbacks. The `connected` callback fires when a socket is connected, and sends a GET request to the server. But then it must await a response, so it registers another callback. If, when that callback fires, it cannot read the full response yet, it registers again, and so on.

Let us collect these callbacks into a `Fetcher` object. It needs a URL, a socket object, and a place to accumulate the response bytes:

```python
class Fetcher:
    def __init__(self, url):
        self.response = b''  # Empty array of bytes.
        self.url = url
        self.sock = None
```

We begin by calling `Fetcher.fetch`:

```python
    # Method on Fetcher class.
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
```

The `fetch` method begins connecting a socket. But notice the method returns before the connection is established. It must return control to the event loop to wait for the connection. To understand why, imagine our whole application was structured so:

```python
# Begin fetching http://xkcd.com/353/ 194
fetcher = Fetcher('/353/')
fetcher.fetch()

while True:
    events = selector.select()
    for event_key, event_mask in events:
        callback = event_key.data
        callback(event_key, event_mask)
```

All event notifications are processed in the event loop when it calls `select`. Hence `fetch` must hand control to the event loop, so that the program knows when the socket has connected. Only then does the loop run the `connected` callback, which was registered at the end of `fetch` above.

Here is the implementation of `connected`:

```python
    # Method on Fetcher class.
    def connected(self, key, mask):
        print('connected!')
        selector.unregister(key.fd)
        request = 'GET {} HTTP/1.0\r\nHost: xkcd.com\r\n\r\n'.format(self.url)
        self.sock.send(request.encode('ascii'))
        
        # Register the next callback.
        selector.register(key.fd,
                          EVENT_READ,
                          self.read_response)
```

The method sends a GET request. A real application would check the return value of `send` in case the whole message cannot be sent at once. But our request is small and our application unsophisticated. It blithely calls `send`, then waits for a response. Of course, it must register yet another callback and relinquish control to the event loop. The next and final callback, `read_response`, processes the server's reply:

```python
    # Method on Fetcher class.
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
```

The callback is executed each time the selector sees that the socket is "readable", which could mean two things: the socket has data or it is closed.

The callback asks for up to four kilobytes of data from the socket. If less is ready, `chunk` contains whatever data is available. If there is more, `chunk` is four kilobytes long and the socket remains readable, so the event loop runs this callback again on the next tick. When the response is complete, the server has closed the socket and `chunk` is empty.

The `parse_links` method, not shown, returns a set of URLs. We start a new fetcher for each new URL, with no concurrency cap. Note a nice feature of async programming with callbacks: we need no mutex around changes to shared data, such as when we add links to `seen_urls`. There is no preemptive multitasking, so we cannot be interrupted at arbitrary points in our code.

We add a global `stopped` variable and use it to control the loop:

```python
stopped = False

def loop():
    while not stopped:
        events = selector.select()
        for event_key, event_mask in events:
            callback = event_key.data
            callback()
```

Once all pages are downloaded the fetcher stops the global event loop and the program exits.

This example makes async's problem plain: spaghetti code. We need some way to express a series of computations and I/O operations, and schedule multiple such series of operations to run concurrently. But without threads, a series of operations cannot be collected into a single function: whenever a function begins an I/O operation, it explicitly saves whatever state will be needed in the future, then returns. You are responsible for thinking about and writing this state-saving code.

Let us explain what we mean by that. Consider how simply we fetched a URL on a thread with a conventional blocking socket:

```python
# Blocking version. 274
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

What state does this function remember between one socket operation and the next? It has the socket, a URL, and the accumulating `response`.  A function that runs on a thread uses basic features of the programming language to store this temporary state in local variables, on its stack. The function also has a "continuation"&mdash;that is, the code it plans to execute after I/O completes. The runtime remembers the continuation by storing the thread's instruction pointer. You need not think about restoring these local variables and the continuation after I/O. It is built in to the language.

But with a callback-based async framework, these language features are no help. While waiting for I/O, a function must save its state explicitly, because the function returns and loses its stack frame before I/O completes. In lieu of local variables, our callback-based example stores `sock` and `response` as attributes of `self`, the Fetcher instance. In lieu of the instruction pointer, it stores its continuation by registering the callbacks `connected` and `read_response`. As the application's features grow, so does the complexity of the state we manually save across callbacks. Such onerous bookkeeping makes the coder prone to migraines.

Even worse, what happens if a callback throws an exception, before it schedules the next callback in the chain? Say we did a poor job on the `parse_links` method and it throws an exception parsing some HTML:

```
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
```

The stack trace shows only that the event loop was running a callback. We do not remember what led to the error. The chain is broken on both ends: we forgot where we were going and whence we came. This loss of context is called "stack ripping", and in many cases it confounds the investigator. Stack ripping also prevents us from installing an exception handler for a chain of callbacks, the way a "try / except" block wraps a function call and its tree of descendents.[^7]

So, even apart from the long debate about the relative efficiencies of multithreading and async, there is this other debate regarding which is more error-prone: threads are susceptible to data races if you make a mistake synchronizing them, but callbacks are stubborn to debug due to stack ripping. 

## Coroutines 314

We entice you with a promise. It is possible to write asynchronous code that combines the efficiency of callbacks with the classic good looks of multithreaded programming. This combination is achieved with a pattern called "coroutines". Using Python 3.4's standard asyncio library, and a package called "aiohttp", fetching a URL in a coroutine is very direct[^10]:

```python
    @asyncio.coroutine
    def fetch(self, url):
        response = yield from self.session.get(url)
        body = yield from response.read()
```

It is also scalable. Compared to the 50k of memory per thread and the operating system's hard limits on threads, a Python coroutine takes barely 3k of memory on Jesse's system. Python can easily start hundreds of thousands of coroutines.

The concept of a coroutine, dating to the elder days of computer science, is simple: it is a subroutine that can be paused and resumed. Whereas threads are preemptively multitasked by the operating system, coroutines multitask cooperatively: they choose when to pause, and which coroutine to run next.

There are many implementations of coroutines; even in Python there are several. The coroutines in the standard "asyncio" library in Python 3.4 are built upon generators, a Future class, and the "yield from" statement. Starting in Python 3.5, coroutines are a native feature of the language itself[^17]; however, understanding coroutines as they were first implemented in Python 3.4, using pre-existing language facilities, is the foundation to tackle Python 3.5's native coroutines.

To explain Python 3.4's generator-based coroutines, we will engage in an exposition of generators and how they are used as coroutines in asyncio, and trust you will enjoy reading it as much as we enjoyed writing it. Once we have explained generator-based coroutines, we shall use them in our async web crawler.

## How Python Generators Work 333

Before you grasp Python generators, you have to understand how regular Python functions work. Normally, when a Python function calls a subroutine, the subroutine retains control until it returns, or throws an exception. Then control returns to the caller:

```python
>>> def foo():
...     bar()
...
>>> def bar():
...     pass
```

The standard Python interpreter is written in C. The C function that executes a Python function is called, mellifluously, `PyEval_EvalFrameEx`. It takes a Python stack frame object and evaluates Python bytecode in the context of the frame. Here is the bytecode for `foo`:

```python
>>> import dis
>>> dis.dis(foo)
  2           0 LOAD_GLOBAL              0 (bar)
              3 CALL_FUNCTION            0 (0 positional, 0 keyword pair)
              6 POP_TOP
              7 LOAD_CONST               0 (None)
             10 RETURN_VALUE
```

The `foo` function loads `bar` onto its stack and calls it, then pops its return value from the stack, loads `None` onto the stack, and returns `None`.

When `PyEval_EvalFrameEx` encounters the `CALL_FUNCTION` bytecode, it creates a new Python stack frame and recurses: that is, it calls `PyEval_EvalFrameEx` recursively with the new frame, which is used to execute `bar`.

It is crucial to understand that Python stack frames are allocated in heap memory! The Python interpreter is a normal C program, so its stack frames are normal stack frames. But the *Python* stack frames it manipulates are on the heap. Among other surprises, this means a Python stack frame can outlive its function call. To see this interactively, save the current frame from within `bar`:

```python
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
```

\aosafigure[240pt]{crawler-images/function-calls.png}{Function Calls}{500l.crawler.functioncalls}

The stage is now set for Python generators, which use the same building blocks&mdash;code objects and stack frames&mdash;to marvelous effect.

This is a generator function:

```python
>>> def gen_fn():
...     result = yield 1
...     print('result of yield: {}'.format(result))
...     result2 = yield 2
...     print('result of 2nd yield: {}'.format(result2))
...     return 'done'
...     
```

When Python compiles `gen_fn` to bytecode, it sees the `yield` statement and knows that `gen_fn` is a generator function, not a regular one. It sets a flag to remember this fact:

```python
>>> # The generator flag is bit position 5.
>>> generator_bit = 1 << 5
>>> bool(gen_fn.__code__.co_flags & generator_bit)
True
```

When you call a generator function, Python sees the generator flag, and it does not actually run the function. Instead, it creates a generator:

```python
>>> gen = gen_fn()
>>> type(gen)
<class 'generator'>
```

A Python generator encapsulates a stack frame plus a reference to some code, the body of `gen_fn`:

```python
>>> gen.gi_code.co_name
'gen_fn'
```

All generators from calls to `gen_fn` point to this same code. But each has its own stack frame. This stack frame is not on any actual stack, it sits in heap memory waiting to be used:

\aosafigure[240pt]{crawler-images/generator.png}{Generators}{500l.crawler.generators}

The frame has a "last instruction" pointer, the instruction it executed most recently. In the beginning, the last instruction pointer is -1, meaning the generator has not begun:

```python
>>> gen.gi_frame.f_lasti
-1
```

When we call `send`, the generator reaches its first `yield`, and pauses. The return value of `send` is 1, since that is what `gen` passes to the `yield` expression:

```python
>>> gen.send(None)
1
```

The generator's instruction pointer is now 3 bytecodes from the start, part way through the 56 bytes of compiled Python:

```python
>>> gen.gi_frame.f_lasti
3
>>> len(gen.gi_code.co_code)
56
```

The generator can be resumed at any time, from any function, because its stack frame is not actually on the stack: it is on the heap. Its position in the call hierarchy is not fixed, and it need not obey the first-in, last-out order of execution that regular functions do. It is liberated, floating free like a cloud.

We can send the value "hello" into the generator and it becomes the result of the `yield` expression, and the generator continues until it yields 2:

```python
>>> gen.send('hello')
result of yield: hello
2
```

Its stack frame now contains the local variable `result`:

```python
>>> gen.gi_frame.f_locals
{'result': 'hello'}
```

Other generators created from `gen_fn` will have their own stack frames and local variables.

When we call `send` again, the generator continues from its second `yield`, and finishes by raising the special `StopIteration` exception:

```python
>>> gen.send('goodbye')
result of 2nd yield: goodbye
Traceback (most recent call last):
  File "<input>", line 1, in <module>
StopIteration: done
```

The exception has a value, which is the return value of the generator: the string `"done"`.

## Building Coroutines With Generators 481

So a generator can pause, and it can be resumed with a value, and it has a return value. Sounds like a good primitive upon which to build an async programming model, without spaghetti callbacks! We want to build a "coroutine": a routine that is cooperatively scheduled with other routines in the program. Our coroutines will be a simplified version of those in Python's standard "asyncio" library. As in asyncio, we will use generators, futures, and the "yield from" statement.

First we need a way to represent some future result that a coroutine is waiting for. A stripped-down version:

```python
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

A future is initially "pending". It is "resolved" by a call to `set_result`.[^12]

Let us adapt our fetcher to use futures and coroutines. We wrote `fetch` with a callback:

```python
class Fetcher:
    def fetch(self):
        self.sock = socket.socket()
        self.sock.setblocking(False)
        try:
            self.sock.connect(('xkcd.com', 80))
        except BlockingIOError:
            pass
        selector.register(self.sock.fileno(),
                          EVENT_WRITE,
                          self.connected)

    def connected(self, key, mask):
        print('connected!')
        # And so on....
```

The `fetch` method begins connecting a socket, then registers the callback, `connected`, to be executed when the socket is ready. Now we can combine these two steps into one coroutine:

```python
    def fetch(self):
        sock = socket.socket()
        sock.setblocking(False)
        try:
            sock.connect(('xkcd.com', 80))
        except BlockingIOError:
            pass

        f = Future()

        def on_connected():
            f.set_result(None)

        selector.register(sock.fileno(),
                          EVENT_WRITE,
                          on_connected)
        yield f
        selector.unregister(sock.fileno())
        print('connected!')
```

Now `fetch` is a generator function, rather than a regular one, because it contains a `yield` statement. We create a pending future, then yield it to pause `fetch` until the socket is ready. The inner function `on_connected` resolves the future.

But when the future resolves, what resumes the generator? We need a coroutine *driver*. Let us call it "task":

```python
class Task:
    def __init__(self, coro):
        self.coro = coro
        f = Future()
        f.set_result(None)
        self.step(f)

    def step(self, future):
        try:
            next_future = self.coro.send(future.result)
        except StopIteration:
            return

        next_future.add_done_callback(self.step)

# Begin fetching http://xkcd.com/353/ 568
fetcher = Fetcher('/353/')
Task(fetcher.fetch())

loop()
```

The task starts the `fetch` generator by sending `None` into it. Then `fetch` runs until it yields a future, which the task captures as `next_future`. When the socket is connected, the event loop runs the callback `on_connected`, which resolves the future, which calls `step`, which resumes `fetch`.

## Factoring Coroutines With `yield from` 577

Once the socket is connected, we send the HTTP GET request and read the server response. These steps need no longer be scattered among callbacks; we gather them into the same generator function:

```python
    def fetch(self):
        # ... connection logic from above, then:
        sock.send(request.encode('ascii'))

        while True:
            f = Future()

            def on_readable():
                f.set_result(sock.recv(4096))

            selector.register(sock.fileno(),
                              EVENT_READ,
                              on_readable)
            chunk = yield f
            selector.unregister(sock.fileno())
            if chunk:
                self.response += chunk
            else:
                # Done reading.
                break
```

This code, which reads a whole message from a socket, seems generally useful. How can we factor it from `fetch` into a subroutine? Now Python 3's celebrated `yield from` takes the stage. It lets one generator *delegate* to another.

To see how, let us return to our simple generator example:

```python
>>> def gen_fn():
...     result = yield 1
...     print('result of yield: {}'.format(result))
...     result2 = yield 2
...     print('result of 2nd yield: {}'.format(result2))
...     return 'done'
...     
```

To call this generator from another generator, delegate to it with `yield from`:

```python
>>> # Generator function:
>>> def caller_fn():
...     gen = gen_fn()
...     rv = yield from gen
...     print('return value of yield-from: {}'
...           .format(rv))
...
>>> # Make a generator from the
>>> # generator function.
>>> caller = caller_fn()
```

The `caller` generator acts as if it were `gen`, the generator it is delegating to:

```python
>>> caller.send(None)
1
>>> caller.gi_frame.f_lasti
15
>>> caller.send('hello')
result of yield: hello
2
>>> caller.gi_frame.f_lasti  # Hasn't advanced.
15
>>> caller.send('goodbye')
result of 2nd yield: goodbye
return value of yield-from: done
Traceback (most recent call last):
  File "<input>", line 1, in <module>
StopIteration
```

While `caller` yields from `gen`, `caller` does not advance. Notice that its instruction pointer remains at 15, the site of its `yield from` statement, even while the inner generator `gen` advances from one `yield` statement to the next.[^13] From our perspective outside `caller`, we cannot tell if the values it yields are from `caller` or from the generator it delegates to. And from inside `gen`, we cannot tell if values are sent in from `caller` or from outside it. The `yield from` statement is a frictionless channel, through which values flow in and out of `gen` until `gen` completes.

A coroutine can delegate work to a sub-coroutine with `yield from` and receive the result of the work. Notice, above, that `caller` printed "return value of yield-from: done". When `gen` completed, its return value became the value of the `yield from` statement in `caller`:

```python
    rv = yield from gen
```

Earlier, when we criticized callback-based async programming, our most strident complaint was about "stack ripping": when a callback throws an exception, the stack trace is typically useless. It only shows that the event loop was running the callback, not *why*. How do coroutines fare?

```python
>>> def gen_fn():
...     raise Exception('my error')
>>> caller = caller_fn()
>>> caller.send(None)
Traceback (most recent call last):
  File "<input>", line 1, in <module>
  File "<input>", line 3, in caller_fn
  File "<input>", line 2, in gen_fn
Exception: my error
```

This is much more useful! The stack trace shows `caller_fn` was delegating to `gen_fn` when it threw the error. Even more comforting, we can wrap the call to a sub-coroutine in an exception handler, the same is with normal subroutines:

```python
>>> def gen_fn():
...     yield 1
...     raise Exception('uh oh')
...
>>> def caller_fn():
...     try:
...         yield from gen_fn()
...     except Exception as exc:
...         print('caught {}'.format(exc))
...
>>> caller = caller_fn()
>>> caller.send(None)
1
>>> caller.send('hello')
caught uh oh
```

So we factor logic with sub-coroutines just like with regular subroutines. Let us factor some useful sub-coroutines from our fetcher. We write a `read` coroutine to receive one chunk:

```python
def read(sock):
    f = Future()

    def on_readable():
        f.set_result(sock.recv(4096))

    selector.register(sock.fileno(), EVENT_READ, on_readable)
    chunk = yield f  # Read one chunk.
    selector.unregister(sock.fileno())
    return chunk
```

We build on `read` with a `read_all` coroutine that receives a whole message:

```python
def read_all(sock):
    response = []
    # Read whole response.
    chunk = yield from read(sock)
    while chunk:
        response.append(chunk)
        chunk = yield from read(sock)

    return b''.join(response)
```

If you squint the right way, the `yield from` statements disappear and these look like conventional functions doing blocking I/O. But in fact, `read` and `read_all` are coroutines. Yielding from `read` pauses `read_all` until the I/O completes. While `read_all` is paused, asyncio's event loop does other work and awaits other I/O events; `read_all` is resumed with the result of `read` on the next loop tick once its event is ready.

At the stack's root, `fetch` calls `read_all`:

```python
class Fetcher:
    def fetch(self):
		 # ... connection logic from above, then:
        sock.send(request.encode('ascii'))
        self.response = yield from read_all(sock)
```

Miraculously, the Task class needs no modification. It drives the outer `fetch` coroutine just the same as before:

```python
Task(fetcher.fetch())
loop()
```

When `read` yields a future, the task receives it through the channel of `yield from` statements, precisely as if the future were yielded directly from `fetch`. When the loop resolves a future, the task sends its result into `fetch`, and the value is received by `read`, exactly as if the task were driving `read` directly:

\aosafigure[240pt]{crawler-images/yield-from.png}{Yield From}{500l.crawler.yieldfrom}

To perfect our coroutine implementation, we polish out one mar: our code uses `yield` when it waits for a future, but `yield from` when it delegates to a sub-coroutine. It would be more refined if we used `yield from` whenever a coroutine pauses. Then a coroutine need not concern itself with what type of thing it awaits.

We take advantage of the deep correspondence in Python between generators and iterators. Advancing a generator is, to the caller, the same as advancing an iterator. So we make our Future class iterable by implementing a special method:

```python
    # Method on Future class.
    def __iter__(self):
        # Tell Task to resume me here.
        yield self
        return self.result
```

The future's `__iter__` method is a coroutine that yields the future itself. Now when we replace code like this:

```python
# f is a Future. 762
yield f
```

...with this:

```python
# f is a Future. 769
yield from f
```

...the outcome is the same! The driving Task receives the future from its call to `send`, and when the future is resolved it sends the new result back into the coroutine.

What is the advantage of using `yield from` everywhere? Why is that better than waiting for futures with `yield` and delegating to sub-coroutines with `yield from`? It is better because now, a method can freely change its implementation without affecting the caller: it might be a normal method that returns a future that will *resolve* to a value, or it might be a coroutine that contains `yield from` statements and *returns* a value. In either case, the caller need only `yield from` the method in order to wait for the result.

Gentle reader, we have reached the end of our enjoyable exposition of coroutines in asyncio. We peered into the machinery of generators, and sketched an implementation of futures and tasks. We outlined how asyncio attains the best of both worlds: concurrent I/O that is more efficient than threads and more legible than callbacks. Of course, the real asyncio is much more sophisticated than our sketch. The real framework addresses zero-copy I/O, fair scheduling, exception handling, and an abundance of other features.

To an asyncio user, coding with coroutines is much simpler than you saw here. In the code above we implemented coroutines from first principles, so you saw callbacks, tasks, and futures. You even saw non-blocking sockets and the call to ``select``. But when it comes time to build an application with asyncio, none of this appears in your code. As we promised, you can now sleekly fetch a URL:

```python
    @asyncio.coroutine
    def fetch(self, url):
        response = yield from self.session.get(url)
        body = yield from response.read()
```

Satisfied with this exposition, we return to our original assignment: to write an async web crawler, using asyncio.

## Coordinating Coroutines 790

We began by describing how we want our crawler to work. Now it is time to implement it with asyncio coroutines.

Our crawler will fetch the first page, parse its links, and add them to a queue. After this it fans out across the website, fetching pages concurrently. But to limit load on the client and server, we want some maximum number of workers to run, and no more. Whenever a worker finishes fetching a page, it should immediately pull the next link from the queue. We will pass through periods when there is not enough work to go around, so some workers must pause. But when a worker hits a page rich with new links, then the queue suddenly grows and any paused workers should wake and get cracking. Finally, our program must quit once its work is done.

Imagine if the workers were threads. How would we express the crawler's algorithm? We could use a synchronized queue[^5] from the Python standard library. Each time an item is put in the queue, the queue increments its count of "tasks". Worker threads call `task_done` after completing work on an item. The main thread blocks on `Queue.join` until each item put in the queue is matched by a `task_done` call, then it exits.

Coroutines use the exact same pattern with an asyncio queue! First we import it[^6]:

```python
try:
    from asyncio import JoinableQueue as Queue
except ImportError:
    # In Python 3.5, asyncio.JoinableQueue is
    # merged into Queue.
    from asyncio import Queue
```

We collect the workers' shared state in a crawler class, and write the main logic in its `crawl` method. We start `crawl` on a coroutine and run asyncio's event loop until `crawl` finishes:

```python
loop = asyncio.get_event_loop()

crawler = crawling.Crawler('http://xkcd.com',
                           max_redirect=10)

loop.run_until_complete(crawler.crawl())
```

The crawler begins with a root URL and `max_redirect`, the number of redirects it is willing to follow to fetch any one URL. It puts the pair `(URL, max_redirect)` in the queue. (For the reason why, stay tuned.)

```python
class Crawler:
    def __init__(self, root_url, max_redirect):
        self.max_tasks = 10
        self.max_redirect = max_redirect
        self.q = Queue()
        self.seen_urls = set()
        
        # aiohttp's ClientSession does connection pooling and
        # HTTP keep-alives for us.
        self.session = aiohttp.ClientSession(loop=loop)
        
        # Put (URL, max_redirect) in the queue.
        self.q.put((root_url, self.max_redirect))
```

The number of unfinished tasks in the queue is now one. Back in our main script, we launch the event loop and the `crawl` method:

```python
loop.run_until_complete(crawler.crawl())
```

The `crawl` coroutine kicks off the workers. It is like a main thread: it blocks on `join` until all tasks are finished, while the workers run in the background.

```python
    @asyncio.coroutine
    def crawl(self):
        """Run the crawler until all work is done."""
        workers = [asyncio.Task(self.work())
                   for _ in range(self.max_tasks)]

        # When all work is done, exit.
        yield from self.q.join()
        for w in workers:
            w.cancel()
```

If the workers were threads we might not wish to start them all at once. To avoid creating expensive threads until it is certain they are necessary, a thread pool typically grows on demand. But coroutines are cheap, so we simply start the maximum number allowed.

It is interesting to note how we shut down the crawler. When the `join` future resolves, the worker tasks are alive but suspended: they wait for more URLs but none come. So, the main coroutine cancels them before exiting. Otherwise, as the Python interpreter shuts down and calls all objects' destructors, living tasks cry out:

```
ERROR:asyncio:Task was destroyed but it is pending!
```

And how does `cancel` work? Generators have a feature we have not yet shown you. You can throw an exception into a generator from outside:

```python
>>> gen = gen_fn()
>>> gen.send(None)  # Start the generator as usual.
1
>>> gen.throw(Exception('error'))
Traceback (most recent call last):
  File "<input>", line 3, in <module>
  File "<input>", line 2, in gen_fn
Exception: error
```

The generator is resumed by `throw`, but it is now raising an exception. If no code in the generator's call stack catches it, the exception bubbles back up to the top. So to cancel a task's coroutine:

```python
    # Method of Task class.
    def cancel(self):
        self.coro.throw(CancelledError)
```

Wherever the generator is paused, at some `yield from` statement, it resumes and throws an exception. We handle cancellation in the task's `step` method:

```python
    # Method of Task class.
    def step(self, future):
        try:
            next_future = self.coro.send(future.result)
        except CancelledError:
            self.cancelled = True
            return
        except StopIteration:
            return

        next_future.add_done_callback(self.step)
```

Now the task knows it is cancelled, so when it is destroyed it does not rage against the dying of the light.

Once `crawl` has canceled the workers, it exits. The event loop sees that the coroutine is complete (we shall see how later), and it too exits:

```python
loop.run_until_complete(crawler.crawl())
```

The `crawl` method comprises all that our main coroutine must do. It is the worker coroutines that get URLs from the queue, fetch them, and parse them for new links. Each worker runs the `work` coroutine independently:

```python
    @asyncio.coroutine
    def work(self):
        while True:
            url, max_redirect = yield from self.q.get()

            # Download page and add new links to self.q.
            yield from self.fetch(url, max_redirect)
            self.q.task_done()
```

Python sees that this code contains `yield from` statements, and compiles it into a generator function. So in `crawl`, when the main coroutine calls `self.work` ten times, it does not actually execute this method: it only creates ten generator objects with references to this code. It wraps each in a Task. The Task receives each future the generator yields, and drives the generator by calling `send` with each future's result when the future resolves. Because the generators have their own stack frames, they run independently, with separate local variables and instruction pointers.

The worker coordinates with its fellows via the queue. It waits for new URLs with:

```python
    url, max_redirect = yield from self.q.get()
```

The queue's `get` method is itself a coroutine: it pauses until someone puts an item in the queue, then resumes and returns the item.

Incidentally, this is where the worker will be paused at the end of the crawl, when the main coroutine cancels it. From the coroutine's perspective, its last trip around the loop ends when `yield from` raises a `CancelledError`.

When a worker fetches a page it parses the links and puts new ones in the queue, then calls `task_done` to decrement the counter. Eventually, a worker fetches a page whose URLs have all been fetched already, and there is also no work left in the queue. Thus this worker's call to `task_done` decrements the counter to zero. Then `crawl`, which is waiting for the queue's `join` method, is unpaused and finishes.

We promised to explain why the items in the queue are pairs, like:

```python
# URL to fetch, and the number of redirects left. 942
('http://xkcd.com/353', 10)
```

New URLs have ten redirects remaining. Fetching this particular URL results in a redirect to a new location with a trailing slash. We decrement the number of redirects remaining, and put the next location in the queue:

```python
# URL with a trailing slash. Nine redirects left. 949
('http://xkcd.com/353/', 9)
```

The `aiohttp` package we use would follow redirects by default and give us the final response. We tell it not to, however, and handle redirects in the crawler, so it can coalesce redirect paths that lead to the same destination: if we have already seen this URL, it is in ``self.seen_urls`` and we have already started on this path from a different entry point:

\aosafigure[240pt]{crawler-images/redirects.png}{Redirects}{500l.crawler.redirects}

The crawler fetches "foo" and sees it redirects to "baz", so it adds "baz" to
the queue and to ``seen_urls``. If the next page it fetches is "bar", which
also redirects to "baz", the fetcher does not enqueue "baz" again. If the
response is a page, rather than a redirect, `fetch` parses it for links and
puts new ones in the queue.

```python
    @asyncio.coroutine
    def fetch(self, url, max_redirect):
        # Handle redirects ourselves.
        response = yield from self.session.get(
            url, allow_redirects=False)

        try:
            if is_redirect(response):
                if max_redirect > 0:
                    next_url = response.headers['location']
                    if next_url in self.seen_urls:
                        # We have been down this path before.
                        return
    
                    # Remember we have seen this URL.
                    self.seen_urls.add(next_url)
                    
                    # Follow the redirect. One less redirect remains.
                    self.q.put_nowait((next_url, max_redirect - 1))
    	     else:
    	         links = yield from self.parse_links(response)
    	         # Python set-logic:
    	         for link in links.difference(self.seen_urls):
                    self.q.put_nowait((link, self.max_redirect))
                self.seen_urls.update(links)
        finally:
            # Return connection to pool.
            yield from response.release()
```

If this were multithreaded code, it would be lousy with race conditions. For example, the worker checks if a link is in `seen_urls`, and if not the worker puts it in the queue and adds it to `seen_urls`. If it were interrupted between the two operations, then another worker might parse the same link from a different page, also observe that it is not in `seen_urls`, and also add it to the queue. Now that same link is in the queue twice, leading (at best) to duplicated work and wrong statistics.

However, a coroutine is only vulnerable to interruption at `yield from` statements. This is a key difference that makes coroutine code far less prone to races than multithreaded code: multithreaded code must enter a critical section explicitly, by grabbing a lock, otherwise it is interruptible. A Python coroutine is uninterruptible by default, and only cedes control when it explicitly yields.

We no longer need a fetcher class like we had in the callback-based program. That class was a workaround for a deficiency of callbacks: they need some place to store state while waiting for I/O, since their local variables are not preserved across calls. But the `fetch` coroutine can store its state in local variables like a regular function does, so there is no more need for a class.

When `fetch` finishes processing the server response it returns to the caller, `work`. The `work` method calls `task_done` on the queue and then gets the next URL from the queue to be fetched.

When `fetch` puts new links in the queue it increments the count of unfinished tasks and keeps the main coroutine, which is waiting for `q.join`, paused. If, however, there are no unseen links and this was the last URL in the queue, then when `work` calls `task_done` the count of unfinished tasks falls to zero. That event unpauses `join` and the main coroutine completes.

The queue code that coordinates the workers and the main coroutine is like this[^9]:

```python
class Queue:
    def __init__(self):
        self._join_future = Future()
        self._unfinished_tasks = 0
        # ... other initialization ...
    
    def put_nowait(self, item):
        self._unfinished_tasks += 1
        # ... store the item ...

    def task_done(self):
        self._unfinished_tasks -= 1
        if self._unfinished_tasks == 0:
            self._join_future.set_result(None)

    @asyncio.coroutine
    def join(self):
        if self._unfinished_tasks > 0:
            yield from self._join_future
```

The main coroutine, `crawl`, yields from `join`. So when the last worker decrements the count of unfinished tasks to zero, it signals `crawl` to resume, and finish.

The ride is almost over. Our program began with the call to `crawl`:

```python
loop.run_until_complete(self.crawler.crawl())
```

How does the program end? Since `crawl` is a generator function, calling it returns a generator. To drive the generator, asyncio wraps it in a task:

```python
class EventLoop:
    def run_until_complete(self, coro):
        """Run until the coroutine is done."""
        task = Task(coro)
        task.add_done_callback(stop_callback)
        try:
            self.run_forever()
        except StopError:
            pass

class StopError(BaseException):
    """Raised to stop the event loop."""

def stop_callback(future):
    raise StopError
```

When the task completes, it raises `StopError `, which the loop uses as a signal that it has arrived at normal completion.

But what's this? The task has methods called `add_done_callback` and `result`? You might think that a task resembles a future. Your instinct is correct. We must admit a detail about the Task class we hid from you: a task is a future.

```python
class Task(Future):
    """A coroutine wrapped in a Future."""
```

Normally a future is resolved by someone else calling `set_result` on it. But a task resolves *itself* when its coroutine stops. Remember from our earlier exploration of Python generators that when a generator returns, it throws the special `StopIteration` exception:

```python
    # Method of class Task.
    def step(self, future):
        try:
            next_future = self.coro.send(future.result)
        except CancelledError:
            self.cancelled = True
            return
        except StopIteration as exc:

            # Task resolves itself with coro's return
            # value.
            self.set_result(exc.value)
            return

        next_future.add_done_callback(self.step)
```

So when the event loop calls `task.add_done_callback(stop_callback)`, it prepares to be stopped by the task. Here is `run_until_complete` again:

```python
    # Method of event loop.
    def run_until_complete(self, coro):
        task = Task(coro)
        task.add_done_callback(stop_callback)
        try:
            self.run_forever()
        except StopError:
            pass
```

When the task catches `StopIteration` and resolves itself, the callback raises `StopError` from within the loop. The loop stops and the call stack is unwound to `run_until_complete`. Our program is finished.

## Conclusion 1100

Increasingly often, modern programs are I/O-bound instead of CPU-bound. For such programs, Python threads are the worst of both worlds: the global interpreter lock prevents them from actually executing computations in parallel, and preemptive switching makes them prone to races. Async is often the right pattern. But as callback-based async code grows, it tends to become a dishevelled mess. Coroutines are a tidy alternative. They factor naturally into subroutines, with sane exception handling and stack traces.

If we squint so that the `yield from` statements blur, a coroutine looks like a thread doing traditional blocking I/O. We can even coordinate coroutines with classic patterns from multi-threaded programming. There is no need for reinvention. Thus, compared to callbacks, coroutines are an inviting idiom to the coder experienced with multithreading.

But when we open our eyes and focus on the `yield from` statements, we see they mark points when the coroutine cedes control and allows others to run. Unlike threads, coroutines display where our code can be interrupted and where it cannot. In his illuminating essay "Unyielding"[^4], Glyph Lefkowitz writes, "Threads make local reasoning difficult, and local reasoning is perhaps the most important thing in software development." Explicitly yielding, however, makes it possible to "understand the behavior (and thereby, the correctness) of a routine by examining the routine itself rather than examining the entire system."

This chapter was written during a renaissance in the history of Python and async. Generator-based coroutines, whose devising you have just learned, were released in the "asyncio" module with Python 3.4 in March 2014. In September 2015, Python 3.5 was released with coroutines built in to the language itself. These native coroutinesare declared with the new syntax "async def", and instead of "yield from", they use the new "await" keyword to delegate to a coroutine or wait for a Future.

Despite these advances, the core ideas remain. Python's new native coroutines will be syntactically distinct from generators but work very similarly; indeed, they will share an implementation within the Python interpreter. Task, Future, and the event loop will continue to play their roles in asyncio.

Now that you know how asyncio coroutines work, you can largely forget the details. The machinery is tucked behind a dapper interface. But your grasp of the fundamentals empowers you to code correctly and efficiently in modern async environments.

[^4]: [https://glyph.twistedmatrix.com/2014/02/unyielding.html](https://glyph.twistedmatrix.com/2014/02/unyielding.html)

[^5]: [https://docs.python.org/3/library/queue.html](https://docs.python.org/3/library/queue.html)

[^6]: [https://docs.python.org/3/library/asyncio-sync.html](https://docs.python.org/3/library/asyncio-sync.html)

[^7]: For a complex solution to this problem, see [http://www.tornadoweb.org/en/stable/stack_context.html](http://www.tornadoweb.org/en/stable/stack_context.html)

[^8]: [http://www.kegel.com/c10k.html](http://www.kegel.com/c10k.html)

[^9]: The actual `asyncio.Queue` implementation uses an `asyncio.Event` in place of the Future shown here. The difference is an Event can be reset, whereas a Future cannot transition from resolved back to pending.

[^10]: The `@asyncio.coroutine` decorator is not magical. In fact, if it decorates a generator function and the `PYTHONASYNCIODEBUG` environment variable is not set, the decorator does practically nothing. It just sets an attribute, `_is_coroutine`, for the convenience of other parts of the framework. It is possible to use asyncio with bare generators not decorated with `@asyncio.coroutine` at all.

<latex>
[^11]: Jesse listed indications and contraindications for using async in "What Is Async, How Does It Work, And When Should I Use It?", available at pyvideo.org. 
[^bayer]: Mike Bayer compared the throughput of asyncio and multithreading for different workloads in his "Asynchronous Python and Databases": http://techspot.zzzeek.org/2015/02/15/asynchronous-python-and-databases/
</latex>
<markdown>
[^11]: Jesse listed indications and contraindications for using async in ["What Is Async, How Does It Work, And When Should I Use It?":](http://pyvideo.org/video/2565/what-is-async-how-does-it-work-and-when-should). Mike Bayer compared the throughput of asyncio and multithreading for different workloads in ["Asynchronous Python and Databases":](http://techspot.zzzeek.org/2015/02/15/asynchronous-python-and-databases/)
</markdown>

[^12]: This future has many deficiencies. For example, once this future is resolved, a coroutine that yields it should resume immediately instead of pausing, but with our code it does not. See asyncio's Future class for a complete implementation.

[^13]: In fact, this is exactly how "yield from" works in CPython. A function increments its instruction pointer before executing each statement. But after the outer generator executes "yield from", it subtracts 1 from its instruction pointer to keep itself pinned at the "yield from" statement. Then it yields to *its* caller. The cycle repeats until the inner generator throws `StopIteration`, at which point the outer generator finally allows itself to advance to the next instruction.

[^14]: Python's global interpreter lock prohibits running Python code in parallel in one process anyway. Parallelizing CPU-bound algorithms in Python requires multiple processes, or writing the parallel portions of the code in C. But that is a topic for another day.

[^15]: Even calls to `send` can block, if the recipient is slow to acknowledge outstanding messages and the system's buffer of outgoing data is full.

<markdown>
[^16]: Guido introduced the standard asyncio library, called "Tulip" then, at [PyCon 2013](http://pyvideo.org/video/1667/keynote).
</markdown>
<latex>
[^16]: Guido introduced the standard asyncio library, called "Tulip" then, at PyCon 2013.
</latex>

[^17]: Python 3.5's built-in coroutines are described in [PEP 492](https://www.python.org/dev/peps/pep-0492/), "Coroutines with async and await syntax."
## 协程
### 概述
1. **python的异步IO主要是通过事件循环来实现的异步阻塞IO，主要用在socket的io上面，普通文件的io依旧会阻塞，
linux系统内核有两种AIO，一种是通过多线程和进程来模拟实现异步IO的，另一中真正的的AIO，不能缓存，直接写入磁盘**

2. python是实现的异步也包括两种
	1. 通过多线程或者多进程来模拟异步IO，python的cocurrent就是用这种方法来让本身不是异步的任务达到异步的预期，
	tornado使用异步装饰器来通过cocurrent的exector来将同步的任务模拟成异步任务。
		        
	2. 通过epoll/select/kqueue 和非阻塞socket来实现socket的异步。
	asyncio、tornado、zeromq中的事件循环就是通过这样来实现异步网络IO的异步的。
1. 理论铺垫：阻塞IO、非阻塞IO、IO多路复用/事件驱动IO(单线程高并发原理)、异步IO
 
2.  完全来自：http://www.cnblogs.com/alex3714/articles/5876749.html

3.  同步IO和异步IO，阻塞IO和非阻塞IO分别是什么，到底有什么区别？不同的人在不同的上下文下给出的答案是不同的。
所以先限定一下本文的上下文。
	本文讨论的背景是Linux环境下的network IO。

### 概念说明
1. 在进行解释之前，首先要说明几个概念：
	- 用户空间和内核空间
	- 进程切换
	- 进程的阻塞
	- 文件描述符
	- 缓存 I/O

### 用户空间与内核空间
1. 现在操作系统都是采用虚拟存储器，那么对32位操作系统而言，它的寻址空间（虚拟存储空间）为4G（2的32次方）。
操作系统的核心是内核，独立于普通的应用程序，可以访问受保护的内存空间，也有访问底层硬件设备的所有权限。
为了保证用户进程不能直接操作内核（kernel），保证内核的安全，操心系统将虚拟空间划分为两部分，一部分为内核空间，
一部分为用户空间。针对linux操作系统而言，将最高的1G字节（从虚拟地址0xC0000000到0xFFFFFFFF），供内核使用，
称为内核空间，而将较低的3G字节（从虚拟地址0x00000000到0xBFFFFFFF），供各个进程使用，称为用户空间。

### 进程切换
1. 为了控制进程的执行，内核必须有能力挂起正在CPU上运行的进程，并恢复以前挂起的某个进程的执行。
这种行为被称为进程切换。因此可以说，任何进程都是在操作系统内核的支持下运行的，是与内核紧密相关的。

2. 从一个进程的运行转到另一个进程上运行，这个过程中经过下面这些变化：
	1. 保存处理机上下文，包括程序计数器和其他寄存器。
	2. 更新PCB信息。
	3. 把进程的PCB移入相应的队列，如就绪、在某事件阻塞等队列。
	4. 选择另一个进程执行，并更新其PCB。
	5. 更新内存管理的数据结构。
	6. 恢复处理机上下文。

3. 总而言之就是很耗资源，具体的可以参考这篇文章：进程切换

4. 注：进程控制块（Processing Control Block），是操作系统核心中一种数据结构，主要表示进程状态。
其作用是使一个在多道程序环境下不能独立运行的程序（含数据），成为一个能独立运行的基本单位或与其它进程并发执行的进程。
或者说，OS是根据PCB来对并发执行的进程进行控制和管理的。 PCB通常是系统内存占用区中的一个连续存区，
它存放着操作系统用于描述进程情况及控制进程运行所需的全部信息
  
### 进程的阻塞

 正在执行的进程，由于期待的某些事件未发生，如请求系统资源失败、等待某种操作的完成、新数据尚未到达或无新工作做等，则由系统自
 动执行阻塞原语(Block)，使自己由运行状态变为阻塞状态。可见，进程的阻塞是进程自身的一种主动行为，也因此只有处于运行态的进程
 （获得CPU），才可能将其转为阻塞状态。当进程进入阻塞状态，是不占用CPU资源的。

 文件描述符fd

 文件描述符（File descriptor）是计算机科学中的一个术语，是一个用于表述指向文件的引用的抽象化概念。

 文件描述符在形式上是一个非负整数。实际上，它是一个索引值，指向内核为每一个进程所维护的该进程打开文件的记录表。当程序打开一
 个现有文件或者创建一个新文件时，内核向进程返回一个文件描述符。在程序设计中，一些涉及底层的程序编写往往会围绕着文件描述符展
 开。但是文件描述符这一概念往往只适用于UNIX、Linux这样的操作系统。

  

 缓存 I/O

 缓存 I/O 又被称作标准 I/O，大多数文件系统的默认 I/O 操作都是缓存 I/O。在 Linux 的缓存 I/O 机制中，操作系统会将 I/O 的数据
 缓存在文件系统的页缓存（ page cache ）中，也就是说，数据会先被拷贝到操作系统内核的缓冲区中，然后才会从操作系统内核的缓冲区
 拷贝到应用程序的地址空间。

 缓存 I/O 的缺点：
 数据在传输过程中需要在应用程序地址空间和内核进行多次数据拷贝操作，这些数据拷贝操作所带来的 CPU 以及内存开销是非常大的。

  

 二 IO模式

 刚才说了，对于一次IO访问（以read举例），数据会先被拷贝到操作系统内核的缓冲区中，然后才会从操作系统内核的缓冲区拷贝到应用程
 序的地址空间。所以说，当一个read操作发生时，它会经历两个阶段：
 1. 等待数据准备 (Waiting for the data to be ready)
 2. 将数据从内核拷贝到进程中 (Copying the data from the kernel to the process)

 正式因为这两个阶段，linux系统产生了下面五种网络模式的方案。
 - 阻塞 I/O（blocking IO）
 - 非阻塞 I/O（nonblocking IO）
 - I/O 多路复用（ IO multiplexing）
 - 信号驱动 I/O（ signal driven IO）
 - 异步 I/O（asynchronous IO）

 注：由于signal driven IO在实际中并不常用，所以我这只提及剩下的四种IO Model。

  

 阻塞 I/O（blocking IO）

 在linux中，默认情况下所有的socket都是blocking，一个典型的读操作流程大概是这样：

  [720333-201]

  

 当用户进程调用了recvfrom这个系统调用，kernel就开始了IO的第一个阶段：准备数据（对于网络IO来说，很多时候数据在一开始还没有到
 达。比如，还没有收到一个完整的UDP包。这个时候kernel就要等待足够的数据到来）。这个过程需要等待，也就是说数据被拷贝到操作系
 统内核的缓冲区中是需要一个过程的。而在用户进程这边，整个进程会被阻塞（当然，是进程自己选择的阻塞）。当kernel一直等到数据准
 备好了，它就会将数据从kernel中拷贝到用户内存，然后kernel返回结果，用户进程才解除block的状态，重新运行起来。

     所以，blocking IO的特点就是在IO执行的两个阶段都被block了。

 非阻塞 I/O（nonblocking IO）

 linux下，可以通过设置socket使其变为non-blocking。当对一个non-blocking socket执行读操作时，流程是这个样子：

 [720333-201]

  

 当用户进程发出read操作时，如果kernel中的数据还没有准备好，那么它并不会block用户进程，而是立刻返回一个error。从用户进程角度
 讲，它发起一个read操作后，并不需要等待，而是马上就得到了一个结果。用户进程判断结果是一个error时，它就知道数据还没有准备好
 ，于是它可以再次发送read操作。一旦kernel中的数据准备好了，并且又再次收到了用户进程的system call，那么它马上就将数据拷贝到
 了用户内存，然后返回。

     所以，nonblocking IO的特点是用户进程需要不断的主动询问kernel数据好了没有。

 I/O 多路复用（ IO multiplexing）

 IO multiplexing就是我们说的select，poll，epoll，有些地方也称这种IO方式为event driven IO。select/epoll的好处就在于单个
 process就可以同时处理多个网络连接的IO。它的基本原理就是select，poll，epoll这个function会不断的轮询所负责的所有socket，当某
 个socket有数据到达了，就通知用户进程。

 [720333-201]

  

 当用户进程调用了select，那么整个进程会被block，而同时，kernel会“监视”所有select负责的socket，当任何一个socket中的数据准备
 好了，select就会返回。这个时候用户进程再调用read操作，将数据从kernel拷贝到用户进程。

     所以，I/O 多路复用的特点是通过一种机制一个进程能同时等待多个文件描述符，而这些文件描述符（套接字描述符）其中的任意一个
     进入读就绪状态，select()函数就可以返回。

 这个图和blocking IO的图其实并没有太大的不同，事实上，还更差一些。因为这里需要使用两个system call (select 和 recvfrom)，而
 blocking IO只调用了一个system call (recvfrom)。但是，用select的优势在于它可以同时处理多个connection。

 所以，如果处理的连接数不是很高的话，使用select/epoll的web server不一定比使用multi-threading + blocking IO的web server性能
 更好，可能延迟还更大。select/epoll的优势并不是对于单个连接能处理得更快，而是在于能处理更多的连接。）

 在IO multiplexing Model中，实际中，对于每一个socket，一般都设置成为non-blocking，但是，如上图所示，整个用户的process其实是
 一直被block的。只不过process是被select这个函数block，而不是被socket IO给block。

  

 异步 I/O（asynchronous IO）

 inux下的asynchronous IO其实用得很少。先看一下它的流程：

 [720333-201]

 用户进程发起read操作之后，立刻就可以开始去做其它的事。而另一方面，从kernel的角度，当它受到一个asynchronous read之后，首先
 它会立刻返回，所以不会对用户进程产生任何block。然后，kernel会等待数据准备完成，然后将数据拷贝到用户内存，当这一切都完成之
 后，kernel会给用户进程发送一个signal，告诉它read操作完成了。

  

 总结

 blocking和non-blocking的区别

 调用blocking IO会一直block住对应的进程直到操作完成，而non-blocking IO在kernel还准备数据的情况下会立刻返回。

 synchronous IO和asynchronous IO的区别

 在说明synchronous IO和asynchronous IO的区别之前，需要先给出两者的定义。POSIX的定义是这样子的：
 - A synchronous I/O operation causes the requesting process to be blocked until that I/O operation completes;
 - An asynchronous I/O operation does not cause the requesting process to be blocked;

 两者的区别就在于synchronous IO做”IO operation”的时候会将process阻塞。按照这个定义，之前所述的blocking IO，non-blocking IO
 ，IO multiplexing都属于synchronous IO。

 有人会说，non-blocking IO并没有被block啊。这里有个非常“狡猾”的地方，定义中所指的”IO operation”是指真实的IO操作，就是例子中
 的recvfrom这个system call。non-blocking IO在执行recvfrom这个system call的时候，如果kernel的数据没有准备好，这时候不会block
 进程。但是，当kernel中数据准备好的时候，recvfrom会将数据从kernel拷贝到用户内存中，这个时候进程是被block了，在这段时间内，
 进程是被block的。

 而asynchronous IO则不一样，当进程发起IO 操作之后，就直接返回再也不理睬了，直到kernel发送一个信号，告诉进程说IO完成。在这整
 个过程中，进程完全没有被block。

  

 各个IO Model的比较如图所示：

 [720333-201]

  

 通过上面的图片，可以发现non-blocking IO和asynchronous IO的区别还是很明显的。在non-blocking IO中，虽然进程大部分时间都不会
 被block，但是它仍然要求进程去主动的check，并且当数据准备完成以后，也需要进程主动的再次调用recvfrom来将数据拷贝到用户内存。
 而asynchronous IO则完全不同。它就像是用户进程将整个IO操作交给了他人（kernel）完成，然后他人做完后发信号通知。在此期间，用
 户进程不需要去检查IO操作的状态，也不需要主动的去拷贝数据。

  

 三 I/O 多路复用之select、poll、epoll详解

 select，poll，epoll都是IO多路复用的机制。I/O多路复用就是通过一种机制，一个进程可以监视多个描述符，一旦某个描述符就绪（一般
 是读就绪或者写就绪），能够通知程序进行相应的读写操作。但select，poll，epoll本质上都是同步I/O，因为他们都需要在读写事件就绪
 后自己负责进行读写，也就是说这个读写过程是阻塞的，而异步I/O则无需自己负责进行读写，异步I/O的实现会负责把数据从内核拷贝到用
 户空间。（这里啰嗦下）

 select
 
 1 select(rlist, wlist, xlist, timeout=None)

 select 函数监视的文件描述符分3类，分别是writefds、readfds、和exceptfds。调用后select函数会阻塞，直到有描述副就绪（有数据可
 读、可写、或者有except），或者超时（timeout指定等待时间，如果立即返回设为null即可），函数返回。当select函数返回后，可以通
 过遍历fdset，来找到就绪的描述符。

 select目前几乎在所有的平台上支持，其良好跨平台支持也是它的一个优点。select的一个缺点在于单个进程能够监视的文件描述符的数量
 存在最大限制，在Linux上一般为1024，可以通过修改宏定义甚至重新编译内核的方式提升这一限制，但是这样也会造成效率的降低。

 poll
 
 1 int poll (struct pollfd *fds, unsigned int nfds, int timeout);

 不同与select使用三个位图来表示三个fdset的方式，poll使用一个 pollfd的指针实现。

 struct pollfd {
     int fd; /* file descriptor */
     short events; /* requested events to watch */
     short revents; /* returned events witnessed */
 };
 
 pollfd结构包含了要监视的event和发生的event，不再使用select“参数-值”传递的方式。同时，pollfd并没有最大数量限制（但是数量过
 大后性能也是会下降）。和select函数一样，poll返回后，需要轮询pollfd来获取就绪的描述符。

     从上面看，select和poll都需要在返回后，通过遍历文件描述符来获取已经就绪的socket。事实上，同时连接的大量客户端在一时刻可
     能只有很少的处于就绪状态，因此随着监视的描述符数量的增长，其效率也会线性下降。

 　　

 epoll

 epoll是在2.6内核中提出的，是之前的select和poll的增强版本。相对于select和poll来说，epoll更加灵活，没有描述符限制。epoll使用
 一个文件描述符管理多个描述符，将用户关系的文件描述符的事件存放到内核的一个事件表中，这样在用户空间和内核空间的copy只需一次
 。

 一 epoll操作过程

 epoll操作过程需要三个接口，分别如下：

  
 
 1 int epoll_create(int size)；//创建一个epoll的句柄，size用来告诉内核这个监听的数目一共有多大
 2 int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)；
 3 int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);

 1. int epoll_create(int size);
 创建一个epoll的句柄，size用来告诉内核这个监听的数目一共有多大，这个参数不同于select()中的第一个参数，给出最大监听的fd+1的
 值，参数size并不是限制了epoll所能监听的描述符最大个数，只是对内核初始分配内部数据结构的一个建议。
 当创建好epoll句柄后，它就会占用一个fd值，在linux下如果查看/proc/进程id/fd/，是能够看到这个fd的，所以在使用完epoll后，必须
 调用close()关闭，否则可能导致fd被耗尽。

 2. int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)；
 函数是对指定描述符fd执行op操作。
 - epfd：是epoll_create()的返回值。
 - op：表示op操作，用三个宏来表示：添加EPOLL_CTL_ADD，删除EPOLL_CTL_DEL，修改EPOLL_CTL_MOD。分别添加、删除和修改对fd的监听
 事件。
 - fd：是需要监听的fd（文件描述符）
 - epoll_event：是告诉内核需要监听什么事

 3. int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
 等待epfd上的io事件，最多返回maxevents个事件。
 参数events用来从内核得到事件的集合，maxevents告之内核这个events有多大，这个maxevents的值不能大于创建epoll_create()时的size
 ，参数timeout是超时时间（毫秒，0会立即返回，-1将不确定，也有说法说是永久阻塞）。该函数返回需要处理的事件数目，如返回0表示
 已超时。

  

 　　

 [Contracted][ExpandedBl]
 
 #_*_coding:utf-8_*_
 __author__ = 'Alex Li'

 import socket, logging
 import select, errno

 logger = logging.getLogger("network-server")

 def InitLog():
     logger.setLevel(logging.DEBUG)

     fh = logging.FileHandler("network-server.log")
     fh.setLevel(logging.DEBUG)
     ch = logging.StreamHandler()
     ch.setLevel(logging.ERROR)

     formatter = logging.Formatter("%(asctime)s - %(name)s - %(levelname)s - %(message)s")
     ch.setFormatter(formatter)
     fh.setFormatter(formatter)

     logger.addHandler(fh)
     logger.addHandler(ch)


 if __name__ == "__main__":
     InitLog()

     try:
         # 创建 TCP socket 作为监听 socket
         listen_fd = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
     except socket.error as  msg:
         logger.error("create socket failed")

     try:
         # 设置 SO_REUSEADDR 选项
         listen_fd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
     except socket.error as  msg:
         logger.error("setsocketopt SO_REUSEADDR failed")

     try:
         # 进行 bind -- 此处未指定 ip 地址，即 bind 了全部网卡 ip 上
         listen_fd.bind(('', 2003))
     except socket.error as  msg:
         logger.error("bind failed")

     try:
         # 设置 listen 的 backlog 数
         listen_fd.listen(10)
     except socket.error as  msg:
         logger.error(msg)

     try:
         # 创建 epoll 句柄
         epoll_fd = select.epoll()
         # 向 epoll 句柄中注册 监听 socket 的 可读 事件
         epoll_fd.register(listen_fd.fileno(), select.EPOLLIN)
     except select.error as  msg:
         logger.error(msg)

     connections = {}
     addresses = {}
     datalist = {}
     while True:
         # epoll 进行 fd 扫描的地方 -- 未指定超时时间则为阻塞等待
         epoll_list = epoll_fd.poll()

         for fd, events in epoll_list:
             # 若为监听 fd 被激活
             if fd == listen_fd.fileno():
                 # 进行 accept -- 获得连接上来 client 的 ip 和 port，以及 socket 句柄
                 conn, addr = listen_fd.accept()
                 logger.debug("accept connection from %s, %d, fd = %d" % (addr[0], addr[1], conn.fileno()))
                 # 将连接 socket 设置为 非阻塞
                 conn.setblocking(0)
                 # 向 epoll 句柄中注册 连接 socket 的 可读 事件
                 epoll_fd.register(conn.fileno(), select.EPOLLIN | select.EPOLLET)
                 # 将 conn 和 addr 信息分别保存起来
                 connections[conn.fileno()] = conn
                 addresses[conn.fileno()] = addr
             elif select.EPOLLIN & events:
                 # 有 可读 事件激活
                 datas = ''
                 while True:
                     try:
                         # 从激活 fd 上 recv 10 字节数据
                         data = connections[fd].recv(10)
                         # 若当前没有接收到数据，并且之前的累计数据也没有
                         if not data and not datas:
                             # 从 epoll 句柄中移除该 连接 fd
                             epoll_fd.unregister(fd)
                             # server 侧主动关闭该 连接 fd
                             connections[fd].close()
                             logger.debug("%s, %d closed" % (addresses[fd][0], addresses[fd][1]))
                             break
                         else:
                             # 将接收到的数据拼接保存在 datas 中
                             datas += data
                     except socket.error as  msg:
                         # 在 非阻塞 socket 上进行 recv 需要处理 读穿 的情况
                         # 这里实际上是利用 读穿 出 异常 的方式跳到这里进行后续处理
                         if msg.errno == errno.EAGAIN:
                             logger.debug("%s receive %s" % (fd, datas))
                             # 将已接收数据保存起来
                             datalist[fd] = datas
                             # 更新 epoll 句柄中连接d 注册事件为 可写
                             epoll_fd.modify(fd, select.EPOLLET | select.EPOLLOUT)
                             break
                         else:
                             # 出错处理
                             epoll_fd.unregister(fd)
                             connections[fd].close()
                             logger.error(msg)
                             break
             elif select.EPOLLHUP & events:
                 # 有 HUP 事件激活
                 epoll_fd.unregister(fd)
                 connections[fd].close()
                 logger.debug("%s, %d closed" % (addresses[fd][0], addresses[fd][1]))
             elif select.EPOLLOUT & events:
                 # 有 可写 事件激活
                 sendLen = 0
                 # 通过 while 循环确保将 buf 中的数据全部发送出去
                 while True:
                     # 将之前收到的数据发回 client -- 通过 sendLen 来控制发送位置
                     sendLen += connections[fd].send(datalist[fd][sendLen:])
                     # 在全部发送完毕后退出 while 循环
                     if sendLen == len(datalist[fd]):
                         break
                 # 更新 epoll 句柄中连接 fd 注册事件为 可读
                 epoll_fd.modify(fd, select.EPOLLIN | select.EPOLLET)
             else:
                 # 其他 epoll 事件不进行处理
                 continue

 epoll socket echo server

 epoll socket echo server
﻿
<!-- vim-markdown-toc GFM -->

* [Python PEP 492 中文翻译——协程与async/await语法 26]
	* [PEP 492：协程与async/await语法 40]
		* [摘要 41]
		* [API设计和实现的备注 51]
		* [理论和目标 58]
		* [4. 详细内容 71]
		* [5 await表达式 116]
		* [6. 新的操作符优先级列表 163]
		* [异步上下文管理器和“async with” 191]
		* [8. 异步迭代器和“async for” 261]
		* [9. 为什么是StopAsyncIteration？ 390]
		* [协程对象 426]
		* [协程对象的方法 445]
		* [调试特性 451]
		* [新的标准库函数 471]
		* [新的抽象基类 482]
	* [词汇表 503]
		* [向后兼容性 551]
		* [asyncio 555]
		* [asyncio迁移策略 563]

<!-- vim-markdown-toc -->
# Python PEP 492 中文翻译——协程与async/await语法 26
原文标题：PEP 0492 -- Coroutines with async and await syntax
原文链接：https://www.python.org/dev/peps/pep-0492/
生效于：Python 3.5
翻译参照版本：05-May-2015
翻译最后修改：2015年8月22日
翻译出处：http://www.cnblogs.com/animalize/p/4738941.html

1. 用几句话说明这个PEP：
	1. 把协程的概念从生成器独立出来，并为之添加了新语句（async/await）。
	2. 但是在CPython的内部实现，协程仍然是一个生成器。
	3. 增加了异步迭代器（async for），异步迭代器的__aiter__、__anext__函数是协程，可以将程序挂起。
	4. 增加了异步上下文管理器（async with），异步上下文管理器的__aenter__、__aexit__函数是协程，可以将程序挂起。

## PEP 492：协程与async/await语法 40
### 摘要 41
1. 不断增多的Internet连接程序刺激了对响应性、伸缩性代码的需求。这个PEP的目标在于：制订显式的异步/并发语法，比传统的Python方法更易用、更丰富。
我们准备把协程（协同程序）的概念独立出来，并为其使用新的语法。最终目标是建立一个通用、易学的异步编程的构思模型，并尽量与同步编程的风格相似。

2. 这个PEP假设异步任务被一个事件循环器（类似于标准库里的 asyncio.events.AbstractEventLoop）管理和调度。
然而我们并不会依赖某个事件循环器的具体实现方法，从本质上说只与此相关：
采用yield作为给调度器的信号，表示协程将会挂起、等待一个异步事件（如IO）的完成。

3. 在这个异步编程不断增长的时期，我们相信这些改变将会使Python保持一定的竞争性，就像许多其它编程语言已经、将要进行的改变那样。

### API设计和实现的备注 51
1. 根据Python 3.5 Beta期间的反馈，进行了重新设计，明确地把协程从生成器里独立出来了。
	协程现在是原生的，有明确的独立类型，而不是作为生成器的一种特殊形式。

2. 这个改变，主要是为了解决在Tornado里使用协程出现的一些问题。
【译注：在Tornado 4.3已经可以使用新的async/await语句，详见此链接】

### 理论和目标 58
1. 在以前，我们可以用生成器实现协程（PEP 342），后来又对其进行了改进，引入了yield from语法（PEP 380）。但仍有一些缺点：

	- 协程和普通生成器使用相同的语法，所以很容易把它们搞混，初学者更是如此。
	- 一个函数是否是一个协程，取决于它里面是否出现了yield或yield from语句。这并不明显，容易在重构函数的时候搞乱，导致出错。
	- 异步调用被yield语法限制了，我们不能获得、使用更多的语法特性，比如with和for。

2. 这个PEP把协程从生成器独立出来，成为Python的一个原生事物。这会消除协程和生成器之间的混淆，方便编写不依赖特定库的协程代码。
	也为linter和IDE进行代码静态分析提供了机会。**【译注：在CPython内部，原生协程仍然是基于生成器实现的。】**

	使用原生协程和相应的新语法，我们可以在异步编程时使用上下文管理器（context manager）和迭代器。如下文所示，
新的async with语句可以在进入、离开运行上下文（runtime context）时进行异步调用，而async for语句可以在迭代时进行异步调用。

### 4. 详细内容 71
> 请理解Python现有的协程（见PEP 342和PEP 380），这次改变的动机来自于asyncio框架（PEP 3156）和Confunctions提案（PEP 3152，此PEP已经被废弃）。
由此，在本文中，我们使用“原生协程”指用新语法声明的协程。“生成器实现的协程”指用传统方法实现的协程。“协程”则用在两个都可以使用的地方。

1. 新的协程声明语法
	使用以下语法声明原生协程：

	```
	async def read_data(db):
		pass
	```

2. <font color=red>协程语法的关键点：</font>
	- async def函数必定是协程，即使里面不含有await语句。
	- 如果在async函数里面使用yield或yield from语句，会引发SyntaxError异常。
	- 在CPython内部，引入两个新的代码对象标识（code object flags）：
		1. CO_COROUTINE表示这是原生协程。（由新语法定义）
		2. CO_ITERABLE_COROUTINE表示这是用生成器实现的协程，但是和原生协程兼容。（用装饰器types.coroutine()装饰过的生成器协程）

	- 调用一个普通生成器，返回一个生成器对象（generator object）；相应的，调用一个协程返回一个协程对象（coroutine object）。
	- 协程不再抛出StopIteration异常，因为抛出的StopIteration异常会被包装（wrap）成一个RuntimeError异常。
		（在Python 3.5，对于普通生成器要想这样需要进行future import，见PEP 479）。

	- 如果一个协程从未await等待就被垃圾收集器销毁了，会引发一个RuntimeWarning异常（见“调试特性”）。
	- 更多请参考“协程对象”一节。

3. types.coroutine()
	1. types模块添加了一个新函数coroutine(fn)，使用它，“生成器实现的协程”和“原生协程”之间可以进行互操作。
		【译注：这是个装饰器，能把现有代码的“用生成器实现的协程”转化为与“原生协程”兼容的形式】

		```
		@types.coroutine
		def process_data(db):
			data = yield from read_data(db)
			...
		```

	2. <font color=red>coroutine(fn)函数给生成器的代码对象(code object)设置CO_ITERABLE_COROUTINE标识，使它返回一个协程对象。</font>
		如果fn不是一个生成器函数，它什么也不做。如果fn是一个生成器函数，则会被一个awaitable代理对象(proxy object)包装(wrapped)，
		详见下文的“定义awaitable对象”。

	3. 注意， types.coroutine()不会设置CO_COROUTINE标识，只有用新语法定义的原生协程才会有这个标识。
	【译注： @types.coroutine装饰器仅给生成器函数设置一个CO_ITERABLE_COROUTINE标识，除此之外什么也不做。
		但是如果生成器函数没有这个标识，await语句不会接受它的对象作为参数。】

###  5 await表达式 116

1. 新的await表达式用于获得协程执行的结果：

```
async def read_data(db):
	data = await db.fetch('SELECT ...')
	...

```

2. <font color=red>await和yield from类似，它挂起`read_data`的执行，直到db.fetch执行完毕并返回结果。</font>

3. <font color=red>以CPython内部，await使用了yield from的实现，但加入了一个额外步骤——验证它的参数类型。
	await只接受awaitable对象，awaitable对象是以下的其中一个：</font>

	- 一个原生协程对象（由一个原生协程函数返回）
	- 用装饰器types.coroutine()装饰的一个“生成器实现的协程”对象
	- 一个有__await__方法的对象（__await__方法返回的一个迭代器）

4. 每个yield from调用链条都会追溯到一个最终的yield语句，这是Future实现的基本机制。
	在Python内部，由于协程是生成器的一种特殊形式，<font color=green>所以每个await最终会被await调用链条上的某个yield语句挂起。</font>
	（详情请参考PEP 3156）【译注：Future对象用来表示在未来完成的某项任务。】

5. 为了让协程也有这样的行为，添加了一个新的魔术方法__await__。
	【译注：一系列递归调用必终结于某个return具体结果的语句；一个yield from调用链条必终结于某个yield语句；
	类似的，一个await调用链条必终结于某个有__await__方法的对象。】
	例如，在asyncio模块，要想在await语句里使用Future对象，唯一的修改是给

	```
	asyncio.Future加一行：__await__ = __iter__
	```

6. 在本文中，有`__await__`方法的对象被称为Future-like对象。
【译注：协程会被await语句挂起，直到await语句右边的Future-like对象的`__await__`执行完毕、返回结果。】

7. 另外，请注意`__aiter__`方法（见下文）不能被用于此目的。那是另一套东西，这样做的话，
	类似于callable对象使用`__iter__`代替`__call__`。
	【译注：意思是`__await__`和`__aiter__`的关系有点像callable对象里__call__和__iter__的关系】

9. 在CPython C API，有`tp_as_async.am_await`函数的对象，该函数返回一个迭代器（类似`__await__`方法）

12. 引发异常：
	1. 如果在async def函数之外使用await语句，会引发SyntaxError异常。这和在def函数之外使用yield语句一样。
	2. 如果await右边不是一个awaitable对象，会引发TypeError异常。
	3. 如果`__await__`返回的不是一个迭代器，则引发TypeError异常。

### 6. 新的操作符优先级列表 163
1. await语句和yield、yield from的一个区别是：await语句多数情况下不需要被圆括号包围。

2. await表达式使用示例

	- 有效用法：
	```
			   表达式                        被解析为
	if await fut: pass           if (await fut): pass
	if await fut + 1: pass       if (await fut) + 1: pass
	pair = await fut, 'spam'     pair = (await fut), 'spam'
	with await fut, open(): pass with (await fut), open(): pass
	await foo()['spam'].baz()()  await ( foo()['spam'].baz()() )
	return await coro()          return ( await coro() )
	res = await coro() ** 2      res = (await coro()) ** 2
	func(a1=await coro(), a2=0)  func(a1=(await coro()), a2=0)
	await foo() + await bar()    (await foo()) + (await bar())
	-await foo()                 -(await foo())

	```

	- 无效用法：
	```
		  表达式             应该写为
	await await coro() await (await coro())
	await -coro()      await (-coro())
	```

### 异步上下文管理器和“async with” 191
> 异步上下文管理器（asynchronous context manager），可以在它的enter和exit方法里挂起、调用异步代码。
为此，我们设计了一套方案，添加了两个新的魔术方法：`__aenter__`和`__aexit__`，它们必须返回一个awaitable。

1. 异步上下文管理器的一个示例：

```
class AsyncContextManager:
	async def __aenter__(self):
		await log('entering context')

	async def __aexit__(self, exc_type, exc, tb):
		await log('exiting context')

```

2. 新语法
	1. 采纳了一个异步上下文管理器的新语法：

	```
	async with EXPR as VAR:
		BLOCK

	```

	2. 在语义上等同于：
	```
	mgr = (EXPR)
	aexit = type(mgr).__aexit__
	aenter = type(mgr).__aenter__(mgr)
	exc = True

	VAR = await aenter
	try:
		BLOCK
	except:
		if not await aexit(mgr, *sys.exc_info()):
			raise
	else:
		await aexit(mgr, None, None, None)
	```

3. 和普通的with语句一样，可以在单个async with语句里指定多个上下文管理器。
	在使用async with时，如果上下文管理器没有`__aenter__`和`__aexit__`方法，则会引发错误。
	在async def函数之外使用async with则会引发SyntaxError异常。

4. 示例
	有了异步上下文管理器，协程很容易实现对数据库处理的恰当管理。
	```
	async def commit(session, data):
		...

		async with session.transaction():
			...
			await session.update(data)
			...

	```

	再比如，加锁时看着更简洁：
	```
	async with lock:
		...

	而不是：

	with (yield from lock):
		...
	```

### 8. 异步迭代器和“async for” 261
> 异步迭代器可以在它的iter实现里挂起、调用异步代码，也可以在它的`__next__`方法里挂起、调用异步代码。要支持异步迭代，需要：

1. 对象必须实现一个`__aiter__`方法（或者，如果使用CPython C API，需要定义`tp_as_async.am_aiter`），
	返回一个异步迭代器对象，这个异步迭代器对象在每次迭代时会返回一个awaitable。

2. 一个异步迭代器必须实现一个`__anext__`方法（或者，如果使用CPython C API，需要定义`tp_as_async.am_anext`），在每次迭代时返回一个awaitable。

3. 要停止迭代，`__anext__`必须抛出一个StopAsyncIteration异常。

4. 异步迭代的一个示例：
```
class AsyncIterable:
	async def __aiter__(self):
		return self

	async def __anext__(self):
		data = await self.fetch_data()
		if data:
			return data
		else:
			raise StopAsyncIteration

	async def fetch_data(self):
		...
```

5. 新语法
	1. 采纳了一个迭代异步迭代器的新语法：
	```
	async for TARGET in ITER:
		BLOCK
	else:
		BLOCK2

	```

	2. 在语义上等同于：
	```
	iter = (ITER)
	iter = await type(iter).__aiter__(iter)
	running = True
	while running:
		try:
			TARGET = await type(iter).__anext__(iter)
		except StopAsyncIteration:
			running = False
		else:
			BLOCK
	else:
		BLOCK2
	```

	3. 如果async for的迭代器不支持`__aiter__`方法，则引发TypeError异常。如果在async def函数外使用async for，则引发SyntaxError异常。

6. 和普通的for语句一样，async for有一个可选的else分句。
	1. 示例1
		有了异步迭代，我们可以在迭代时异步缓冲（buffer）数据：
		```
		async for data in cursor:
			...

		```
	2. Cursor是一个异步迭代器，可以从数据库预读4行数据并缓存。见以下代码：
	```
	# 【译注：此代码已被修改，望更易理解】
	class Cursor:
		def __init__(self):
			self.buffer = collections.deque()

		async def _prefetch(self):
			row1, row2, row3, row4 = await fetch_from_db()
			self.buffer.append(row1)
			self.buffer.append(row2)
			self.buffer.append(row3)
			self.buffer.append(row4)

		async def __aiter__(self):
			return self

		async def __anext__(self):
			if not self.buffer:
				self.buffer = await self._prefetch()
				if not self.buffer:
					raise StopAsyncIteration
			return self.buffer.popleft()

	```

	然后，可以这样使用Cursor类：
	```
	async for row in Cursor():
		print(row)

	与下述代码相同：

	i = await Cursor().__aiter__()
	while True:
		try:
			row = await i.__anext__()
		except StopAsyncIteration:
			break
		else:
			print(row)

	```

3. 示例2
	这是一个便利类，用于把普通的迭代对象转变为一个异步迭代对象。虽然这个类没什么实际用处，但它演示了普通迭代器和异步迭代器的关系：
	```
	class AsyncIteratorWrapper:
		def __init__(self, obj):
			self._it = iter(obj)

		async def __aiter__(self):
			return self

		async def __anext__(self):
			try:
				value = next(self._it)
			except StopIteration:
				raise StopAsyncIteration
			return value

	async for letter in AsyncIteratorWrapper("abc"):
		print(letter)

	```

### 9. 为什么是StopAsyncIteration？ 390
> 在CPython内部，协程的实现仍然是基于生成器的。所以，在PEP 479生效之前【译注：将在Python 3.7正式生效，
> 在3.5、3.6需要from __future__ import `generator_stop`】，以下两个代码是完全一样的，
> 最终都是给外部代码抛出一个StopIteration('spam')异常：

```
def g1():
    yield from fut
    return 'spam'

和

def g2():
    yield from fut
    raise StopIteration('spam')
```


由于PEP 479已被正式采纳，并作用于协程，以下代码的StopIteration会被包装（wrapp）成一个RuntimeError。
```
async def a1():
	await fut
	raise StopIteration('spam')

```

所以，要想通知外部代码迭代已经结束，抛出一个StopIteration异常的方法不行了。因此，添加了一个新的内置异常StopAsyncIteration，用于表示迭代结束。

此外，根据PEP 479，协程抛出的所有StopIteration异常都会被包装成RuntimeError异常。

【译注：如果函数生成器内部的代码出现StopIteration异常、且未被捕获，则外部代码会误认为生成器已经迭代结束。
为了消除这样的误会，PEP 479的规定，Python会把生成器内部抛出的StopIteration包装成RuntimeError。

在以后，如果想主动结束一个函数生成器的迭代，用return语句即可（这时函数生成器仍然会给外部代码抛出一个StopIteration异常），
	而不是以前的使用raise StopIteration语句（这样的话，StopIteration会被包装成一个RuntimeError）。】

### 协程对象 426
> 和生成器的不同之处这一小节只对原生协程有效（用async def语法定义的、有CO_COROUTINE标识的）。对于asyncio模块里现有的“基于生成器的协程”，仍然保持不变。

1. 为了在概念上把协程和生成器区分开来，做了以下规定：
	1. 原生协程对象不实现`__iter__`和`__next__`方法，因此，不能对其进行迭代（如for...in循环），
	也不能传递给iter()，list()，tuple()及其它内置函数。如果尝试对其使用
	`__iter__`或`__next__`方法，会引发TypeError异常。

	2. 未装饰的生成器不能yield from一个原生协程，这样做会引发TypeError异常。

	3. “基于生成器的协程”在经过 @asyncio.coroutine装饰后，可以yield from原生协程对象。

	4. 对于原生协程对象和原生协程函数，调用inspect.isgenerator()和inspect.isgeneratorfunction()会返回False。

	5. 【译注： @asyncio.coroutine装饰器，在Python 3.4，用于把一个函数装饰为一个协程。有些函数并不是生成器函数（不含yield或yield from语句），
	也可以用@asyncio.coroutine装饰为一个协程。

	6. 在Python 3.5中， @asyncio.coroutine也会有 @types.coroutine的效果——使函数的对象可以被await语句接受。】

### 协程对象的方法 445
1. 在CPython内部，协程是基于生成器实现的，因此它们有共同的代码。像生成器对象那样，协程也有throw()，send()和close()方法。
	对于协程，StopIteration和GeneratorExit起着同样的作用（虽然PEP 479已经应用于协程）。详见PEP 342、PEP 380，以及Python文档。

2. 对于协程，send()，throw()方法用于往Future-like对象发送内容、抛出异常。

### 调试特性 451
1. 新手在使用协程时可能忘记使用yield from语句，比如：
```
@asyncio.coroutine
def useful():
    asyncio.sleep(1) # 前面忘写yield from，所以程序在这里不会挂起1秒
```

2. 在asyncio里，对于此类错误，有一个特定的调试方法。装饰器 @coroutine用一个特定的对象包装（wrap）所有函数，
	这个对象有一个析构函数（destructor）用于记录警告信息。无论何时，一旦被装饰过的生成器被垃圾回收，
	会生成一个详细的记录信息（具体哪个函数、回收时的stack trace等等）。
	包装对象提供一个__repr__方法用来输出关于生成器的详细信息。

3. 唯一的问题是如何启用这些调试工具，由于这些调试工具在生产模式里什么也不做，比如 @coroutine必须是在系统变量PYTHONASYNCIODEBUG出现时才具有调试功能。
	这时可以给asyncio程序进行如下设置：EventLoop.set_debug(true)，这时使用另一套调试工具，对 @coroutine的行为没有影响。

4. 根据本文，协程是原生的，已经在概念上和生成器进行了区分。
	一个从未await的协程会抛出一个RuntimeWarning，除此之外，给sys模块增加了两个新函数`set_coroutine_wrapper`和`get_coroutine_wrapper`，
	它们会为asyncio和其它框架启用高级调试工具，比如显示协程在何处被创建、协程在何处被垃圾回收的详细stack trace。

### 新的标准库函数 471
	- types.coroutine(gen) 详见types.coroutine()一节。
	- inspect.iscoroutine(obj) 如果obj是原生协程对象，返回True。
	- inspect.iscoroutinefunction(obj) 如果obj是原生协程函数，返回True。
	- inspect.isawaitable(obj) 如果obj是awaitable返回True。
	- inspect.getcoroutinestate(coro) 返回原生协程对象的当前状态（inspect.getfgeneratorstate(gen)的镜像）。
	- inspect.getcoroutinelocals(coro) 返回一个原生协程对象的局部变量的映射【译注：变量名->值】（inspect.getgeneratorlocals(gen) 的镜像）。
	- sys.`set_coroutine_wrapper`(wrapper) 允许拦截原生协程对象的创建。wrapper必须是一个接受一个参数callable（一个协程对象），或者是None。None会重置（reset）这个
	wrapper。如果再次调用，新的wrapper会取代旧的。这个函数是线程专有的（thread-specific）。详见“调度特性”一节。
	- sys.`get_coroutine_wrapper`() 返回当前的包装对象(wrapper object)。如果没有则返回None。这个函数是线程专有的（thread-specific）。详见“调度特性”一节。

### 新的抽象基类 482
为了能更好的与现有框架（如Tornado）和其它编译器（如Cython）相整合，增加了两个新的抽象基类（Abstract Base Classes）：

1. collections.abc.Awaitable，Future-like类的抽象基类，实现__await__方法。
2. collections.abc.Coroutine，协程对象的抽象基类，实现send(value)，throw(type, exc, tb)，close()和`__await__`()方法。

3. 注意，“基于生成器的协程”（有`CO_ITERABLE_COROUTINE`标识）并不实现`__await__`方法，
	因此它们不是collections.abc.Coroutine和collections.abc.Awaitable的实例：
	```
	@types.coroutine
	def gencoro():
		yield

	assert not isinstance(gencoro(), collections.abc.Coroutine)
	assert inspect.isawaitable(gencoro())
	```
4. 为了更容易地对异步迭代进行调试，又增加了两个抽象基类：

	1. collections.abc.AsyncIterable --用于测试`__aiter__`方法。
	2. collections.abc.AsyncIterator --用于测试`__aiter__`和`__anext__`方法。

## 词汇表 503
1. 原生协程函数 Native coroutine function

    由async def定义的协程函数，可以使用await和return value语句。见“新的协程声明语法”一节。

2. 原生协程 Native coroutine

    原生协程函数返回的对象。见“await表达式”一节。

3. 基于生成器的协程函数 Generator-based coroutine function

    基于生成器语法的协程，最常见的是用 @asyncio.coroutine装饰过的函数。

4. 基于生成器的协程 Generator-based coroutine

    基于生成器的协程函数返回的对象。

5. 协程 Coroutine

    “原生协程”和“基于生成器的协程”都是协程。

6. 协程对象 Coroutine object

    “原生协程对象”和“基于生成器的协程对象”都是协程对象。

7. Future-like对象 Future-like object

    一个有`__await__`方法的对象，或一个有tp_as_async->am_await函数的C语言对象，它们返回一个迭代器。
Future-like对象可以在协程里被一条await语句消费（consume）。

    协程会被await语句挂起，直到await语句右边的Future-like对象的`__await__`执行完毕、返回结果。见“await表达式”一节。

8. Awaitable

    一个Future-like对象或一个协程对象。见“await表达式”一节。

9. 异步上下文管理器 Asynchronous context manager

    有`__aenter__`和`__aexit__`方法的对象，可以被async with语句使用。见“异步上下文管理器和‘async with’”一节。

10. 可异步迭代对象 Asynchronous iterable

    有`__aiter__`方法的对象，该方法返回一个异步迭代器对象。可以被async for语句使用。见“异步迭代器和‘async for’”一节。

11. 异步迭代器 Asynchronous iterator

    有`__anext__`方法的对象。见“异步迭代器和‘async for’”一节。

### 向后兼容性 551

本PEP保持100%向后兼容。

### asyncio 555
1. asyncio模块已经可以使用新语法，并经过测试，100%与async/await兼容。现有的使用asyncio的代码在使用新语法时可以保持不变。
为此，对asyncio模块主要做了如下修改：

	1. 在 @asyncio.coroutine装饰器内部，调用types.coroutine为函数设置一个`CO_ITERABLE_COROUTINE`标识。
	2. 给asyncio.Future类添加一行代码： `__await__` = `__iter__`。
	3. 把async()函数改名为`ensure_future`()，以防该函数名和新关键字冲突。

### asyncio迁移策略 563
1. 由于未经装饰的生成器不能yield from原生协程对象（详见“和生成器的不同之处”一节），因此在使用新语法前，
请确保所有“基于生成器的协程”都被 @asyncio.coroutine装饰器装饰。

2. 启用关键字的计划
	async和await在CPython 3.5、3.6里暂时不是正式的关键字，在CPython 3.7它们将变成正式的关键字。如果不这样，恐怕对现有代码的迁移造成困难。
【译注：在某些现有代码里，可能使用了async和await作为变量名/函数名。然而Python不允许把关键字当作变量名/函数名，所以3.5、3.6给程序员留了一些迁移时间。】
﻿
<!-- vim-markdown-toc GFM -->

* [Python 3.5中async/await的工作机制]
	* [概述]
	* [Python中协程(coroutine)的历史]
	* [获取生成器]
	* [总结]
	* [总结]
	* [总结]
	* [我对未来的愿景]
	* [总结]

<!-- vim-markdown-toc -->
## Python 3.5中async/await的工作机制
> [翻译] Python 3.5中async/await的工作机制 [翻译链接](https://www.cnblogs.com/harelion/p/8496360.html)

### 概述
	身为Python核心开发组的成员，我对于这门语言的各种细节充满好奇。尽管我很清楚自己不可能对这门语言做到全知全能，
但哪怕是为了能够解决各种issue和参与常规的语言设计工作，我也觉得有必要试着接触和理解Python的内核，
弄清楚在底层它是怎么工作的。

	话虽如此，直到最近我才理解了Python3.5中async/await的工作机制。
在此之前，对于async/await语法，我只知道Python3.3中的yield from和Python3.4中的asyncio让这个新语
法得以在Python3.5中实现。由于日常工作中没有接触多少网络编程--asyncio的主要应用领域，
虽然它可以做的远不止于此--我对async/await并没有关注太多。以代码来说，我知道：

```
yield from iterator

(大体)等价于:

from x in iterator:
    yield x
```

而且我知道asyncio是个事件循环的框架，支持异步编程，还有这些术语所表示的(基本)意义。但未曾真正的深入研究async/await语法，分析从最基础的指令到实现代码语法功能
的过程，我觉得并没有理解Python中的异步编程，这一点甚至让我心烦意乱。因此我决定花点时间弄明白这个语法的工作机制。鉴于我听到许多人说他们也不理解异步编程的工作
机制，我写出了这篇论文(是的，这篇博文耗费时间之长，字数之多，让我妻子把它叫做论文)。

由于我希望对这个语法的工作机制有一个完整的理解，这篇论文中会出现涉及CPython的底层技术细节。如果你不关心这些细节，或者无法通过这篇文章完全理解这些细节--限于篇
幅，我不可能详细解释CPython的每个细节，否则这篇文章就要变成一本书了(例如，如果你不知道代码对象具有标识位，那就别在意代码对象是什么，这不是这篇文章的重点)--那
也没什么关系。在每个章节的最后，我都添加了一个概念明确的小结，因此如果你对某个章节的内容不感兴趣，那么可以跳过前面的长篇大论，直接阅读结论。

### Python中协程(coroutine)的历史

根据维基百科，“协程是将多个低优先级的任务转换成统一类型的子任务，以实现在多个节点之间停止或唤醒程序运行的程序模块”。这句专业论述翻译成通俗易懂的话就是，“协程
就是可以人为暂停执行的函数”。如果你觉得，“这听起来像是生成器(generators)”，那么你是对的。

生成器的概念在Python2.2时的PEP 255中(由于实现了遍历器的协议，生成器也被成为生成器遍历器)第一次被引入。主要受到了Icon语言的影响，生成器允许用户创建一个特殊的
遍历器，在生成下一个值时，不会占用额外的内存，并且实现方式非常简单(当然，在自定义类中实现__iter__()和__next__()方法也可以达到不存储遍历器中所有值的效果，但也
带来了额外的工作量)。举例来说，如果你想实现自己的range()函数，最直接的方式是创建一个整数数组：

def eager_range(up_to):
    """创建一个从0到变量up_to的数组，不包括up_to"""
    sequence = []
    index = []
    while index < up_to:
        sequence.append(index)
        index += 1
    return sequence

简单直白，但这个函数的问题是，如果你需要的序列很大，比如0到一百万，你必须创建一个包含了所有整数的长度是一百万的数组。如果使用生成器，你就可以毫不费力的创建一
个从0到上限前一个整数的生成器。所占用的内存也只是每次生成的一个整数。

def lazy_range(up_to):
    """一个从0到变量up_to，不包括up_to的生成器"""
    index = 0
    while index < up_to:
        yield index
        index += 1

函数可以在遇到yield表达式时暂停执行--尽管yield直到Python2.5才出现--然后在下次被调用时继续执行，这种特性对于节约内存使用有意义深远，可以用于实现无限长度的序列
。

也许你已经注意到了，生成器所操作的都是遍历器。多一种更好的创建遍历器的语法的确不错(当你为一个对象定义__iter__()方法作为生成器时，也会收到类似的提升)，但如果
我们把生成器的“暂停”功能拿出来，再加上“把事物传进去”的功能，Python就有了自己的协程功能(暂且把这个当成Python的一个概念，真正的Python中的协程会在后面详细讨论)
。Python 2.5中引入了把对象传进一个被暂停的生成器的功能，这要归功于PEP 342。抛开与本文无关的内容不看，PEP 342引入了生成器的send()方法。这样就不光可以暂停生成
器，更可以在生成器停止时给它传回一个值。在上文range()函数的基础上更近一步，你可以让函数产生的序列前进或后退：

def jumping_range(up_to):
    """一个从0到变量up_to，不包括up_to的生成器
    传入生成器的值会让序列产生对应的位移
    """
    index = 0
    while index < up_to:
        jump = yield index
        if jump is not None:
            jump = 1
        index += jump

if __name__ == '__main__':
    iterator = jumping_range(5)
    print(next(iterator))  # 0
    print(iterator.send(2))  # 2
    print(next(iterator))  # 3
    print(iterator.send(-1))  # 2
    for x in iterator:
        print(x)  # 3, 4

直到Python 3.3中PEP 380引入yield from之前，生成器都没有太大的变化。严格的说，yield from让用户可以轻松便捷的从遍历器(生成器最常见的应用场景)里提取每一个值，进
而重构生成器。

def lazy_range(up_to):
    """一个从0到变量up_to，不包括up_to的生成器"""
    index = 0
    def gratuitous_refactor():
        nonlocal index
        while index < up_to:
            yield index
            index += 1
    yield from gratuitous_refactor()

同样出于简化重构操作的目的，yield from也支持将生成器串连起来，这样再不同的调用栈之间传递值时，不需要对原有代码做太大的改动。

def bottom():
    """返回yield表达式来允许值通过调用栈进行传递"""
    return (yield 42)

def middle():
    return (yield from bottom())

def top():
    return (yield from middle())

### 获取生成器
gen = top()
value = next(gen)
print(value)  # Prints '42'

try:
    value = gen.send(value * 2)
except StopIteration as exc:
    print("Error!")  # Prints 'Error!'
    value = exc.value
print(value)  # Prints '84'

### 总结

Python2.2引入的生成器使代码的执行可以被暂停。而在Python2.5中引入的允许传值给被暂停的生成器的功能，则让Python中协程的概念成为可能。在Python3.3中引入的yield
from让重构和连接生成器变得更加简单。

事件循环是什么？

如果你想理解async/await语法，那么理解事件循环的定义，知道它如何支持的异步编程，是不可或缺的基础知识。如果你曾经做过GUI编程--包括网页前端工作--那么你已经接触
过事件循环了。但在Python的语言体系中，异步编程的概念还是第一次出现，所以如果不知道事件循环是什么，也情有可原。

让我们回到维基百科，事件循环是“在程序中等待、分发事件或消息的编程结构”。简而言之，事件循环的作用是，“当A发生后，执行B”。最简单的例子可能是每个浏览器中都有的
JavaScript事件循环，当你点击网页某处("当A发生后")，点击事件被传递给JavaScript的事件循环，然后事件循环检查网页上该位置是否有注册了处理这次点击事件的onclick回
调函数("执行B")。如果注册了回调函数，那么回调函数就会接收点击事件的详细信息，被调用执行。事件循环会不停的收集发生的事件，循环已注册的事件操作来找到对应的操作
，因此被称为“循环”。

Python标准库中的asyncio库可以提供事件循环。asyncio在网络编程里的一个重要应用场景，就是以连接到socket的I/O准备好读/写(通过selector模块实现)事件作为事件循环中
的“当A发生后”事件。除了GUI和I/O，事件循环也经常在执行多线程或多进程代码时充当调度器(例如协同式多任务处理)。如果你知道Python中的GIL(General Interpreter Lock)
，事件循环在规避GIL限制方面也有很大的作用。

### 总结

事件循环提供了一个让你实现“当事件A发生后，执行事件B”功能的循环。简单来说，事件循环监视事件的发生，如果发生的是事件循环关心(“注册”过)的事件，那么事件循环会执
行所有被关联到该事件的操作。在Python3.4中加入标准库的asyncio使Python也有了事件循环。

async和await是怎么工作的

在Python3.4中的工作方式

在Python3.3推动生成器的发展和Python3.5中事件循环以asyncio的形式出现之间，Python3.4以并发编程的形式实现了异步编程。从本质上说，异步编程就是无法预知执行时间的
计算机程序(也就是异步，而非同步)。并发编程的代码即使运行在同一个线程中，执行时也互不干扰(并发不是并行)。例如，以下Python3.4的代码中，并发两个异步的函数调用，
每秒递减计数，互不干扰。
```
import asyncio

# Borrowed from http://curio.readthedocs.org/en/latest/tutorial.html.

def countdown(number, n):
    while n > 0:
        print('T-minus', n, '({})'.format(number))
        yield from asyncio.sleep(1)
        n -= 1

loop = asyncio.get_event_loop()
tasks = [
    asyncio.ensure_future(countdown('A', 2)),
    asyncio.ensure_future(countdown('B', 3))
]
loop.run_until_complete(asyncio.wait(tasks))
loop.close()
```

在Python3.4中，asyncio.coroutine装饰器被用于修饰使用asyncio库并且作为协程在它的事件循环中运行的函数。这是Python中第一次出现明确的协程定义：一种实装了PEP 342
中添加给生成器的方法，基类是抽象类collections.abc.Coroutine的对象。这个定义让那些原本没有异步执行意图的生成器也带上了协程的特征。而为了解决这种混淆，asyncio
规定所有作为协程执行的函数都需要以asyncio.coroutine装饰器进行修饰。

有了这样一个明确的协程的定义(同时符合生成器的接口规范)，你可以使用yield from将任何asyncio.Future对象传入事件循环，在等待事件发生时暂停程序执行(future对象是
asyncio中的一种对象，此处不再详述)。future对象进入事件循环后就处于事件循环的监控之下，一旦future对象完成了自身任务，事件循环就会唤醒原本被暂停的协程继续执行
，future对象的返回结果则通过send()方法由事件循环传递给协程。

以上文代码为例，事件循环启动了两个调用call()函数的协程，运行到某个协程中包含yield from和asyncio.sleep()语句处，这条语句将一个asyncio.Future对象返回事件循环，
暂停协程的执行。这时事件循环会为future对象等待一秒(并监控其他程序，例如另外一个协程)，一秒后事件循环唤醒传出了future对象的被暂停的countdown()协程继续执行，并
把future对象的执行结果归还给原协程。这个循环过程会持续到countdown()协程结束执行，事件循环中没有被监控的事件为止。稍后我会用一个完整的例子详细解释协程/事件循
环结构的工作流程，但首先，我要解释一下async和await是如何工作的。

从yield from到Python3.5中的await

在Python3.4中，一个用于异步执行的协程代码会被标记成以下形式：
```
# 这种写法在Python3.5中同样有效
@asyncio.coroutine
def py34_coro():
    yield from stuff()
```

Python3.5也添加了一个作用和asyncio.coroutine相同，用于修饰协程函数的装饰器types.coroutine。也可以使用async def语法定义协程函数，但是这样定义的协程函数中不能
使用yield语句，只允许使用return或await语句返回数据。

```
async def py35_coro():
    await stuff()
```

对同一个协程概念添加的不同语法，是为了规范协程的定义。这些陆续补充的语法使协程从抽象的接口变成了具体的对象类型，让普通的生成器和协程用的生成器有了明显的区别(
inspect.iscoroutine()方法的判断标准则比async还要严格)。

另外，除了async，Python3.5也引入了await语法(只能在async def定义的函数中使用)。虽然await的使用场景与yield from类似，但是await接收的对象不同。作为由于协程而产
生的语法，await接收协程对象简直理所当然。但是当你对某个对象使用await语法时，技术上说，这个对象必须是可等待对象(awaitable object)：一种定义了__await__()方法
(返回非协程本身的遍历器)的对象。协程本身也被视作可等待对象(体现在Python语言设计中，就是collections.abc.Coroutine继承了collections.abc.Awaitable抽象类)。可等
待对象的定义沿用了Python中将大多数语法结构在底层转换成方法调用的传统设计思想，例如a + b等价于a.__add__(b)或b.__radd__(a)。

那么在编译器层面，yield from和await的运行机制有什么区别(例如types.coroutine修饰的生成器和async def语法定义的函数)呢？让我们看看上面两个例子在Python3.5环境下
执行时的字节码细节有什么不同，py34_coro()执行时的字节码是：

```
In [31]: dis.dis(py34_coro)
  3           0 LOAD_GLOBAL              0 (stuff)
              3 CALL_FUNCTION            0 (0 positional, 0 keyword pair)
              6 GET_YIELD_FROM_ITER
              7 LOAD_CONST               0 (None)
             10 YIELD_FROM
             11 POP_TOP
             12 LOAD_CONST               0 (None)
             15 RETURN_VALUE
```

py35_coro()执行时的字节码是：

```
In [33]: dis.dis(py35_coro)
  2           0 LOAD_GLOBAL              0 (stuff)
              3 CALL_FUNCTION            0 (0 positional, 0 keyword pair)
              6 GET_AWAITABLE
              7 LOAD_CONST               0 (None)
             10 YIELD_FROM
             11 POP_TOP
             12 LOAD_CONST               0 (None)
             15 RETURN_VALUE
```

除了py34_coro代码中多了一行装饰器而导致的行号不同，两组字节码的区别集中在GET_YIELD_FROM_ITER操作符和GET_AWAITABLE操作符。两个函数都是以协程的语法声明的。对于
GET_YIELD_FROM_ITER，编译器只检查参数是否生成器或者协程，如果不是，就调用iter()函数遍历参数(types.coroutine装饰器修饰了生成器，让代码对象在C代码层面附带了
CO_ITERABLE_COROUTINE标识，因此yield from语句可以在协程中接收协程对象)。

GET_AWAITABLE则是另外一番光景。虽然同GET_YIELD_FROM_ITER操作符一样，字节码也接收协程对象，但它不会接收没有协程标记的生成器。而且，正如前文所述，字节码不止接
收协程对象，也可以接收可等待对象。这样，yield from语句和await语句都可以实现协程概念，但一个接收的是普通的生成器，另一个是可等待对象。

也许你会好奇，为什么基于async的协程和基于生成器的协程在暂停时接收的对象会不同？这种设计的主要目的是让用户不至于混淆两种类型的协程实现，或者不小心弄错类似的
API的参数类型，甚而影响Python最重要的特性的使用体验。例如生成器继承了协程的API，在需要协程时很容易犯使用了普通的生成器的错误。生成器的使用场景不限于通过协程
实现流程控制的情况，因此很容易的辨别普通生成器和协程也非常重要。可是，Python不是需要预编译的静态语言，在使用基于生成器的协程时编译器只能做到在运行时进行检查
。换句话说，就算使用了types.coroutine装饰器，编译器也无法确定生成器会充当本职工作还是扮演协程的角色(记住，即使代码中明明白白使用了types.coroutine装饰器，依然
有在之前的代码中类似types = spam这样的语句存在的可能)，编译器会根据已知的信息，在不同的上下文环境下调用不同的操作符。

对于基于生成器的协程和async定义的协程的区别，我的一个非常重要的观点是，只有基于生成器的协程可以真正的暂停程序执行，并把外部对象传入事件循环。当你使用事件循环
相关的函数，如asyncio.sleep()时，这些函数与事件循环的交互所用的是框架内部的API，事件循环究竟如何变化，并不需要用户操心，因此也许你很少看到这种关注底层概念的
说法。我们大多数人其实并不需要真正实现一个事件循环，而只需要使用async这样的语法来通过事件循环实现某个功能。但如果你像我一样，好奇为什么我们不能使用async协程
实现类似asnycio.sleep()的功能，那么答案就在这里。

### 总结

让我们总结一下这两个相似的术语，使用async def可以定义协程，使用types.coroutine装饰器可以将一个生成器--返回一个不是协程本身的遍历器--声明为协程。await语句只能
用于可等待对象(await不能作用于普通的生成器)，除此之外就和yield from的功能基本相同。async函数定义的协程中一定会有return语句--包括每个Python函数都有的默认返回
语句return None--和/或await语句(不能使用yield语句)。对async函数所添加的限制，是为了保证用户不会混淆它和基于生成器的协程，两者的期望用途差别很大。

请把async/await视为异步编程的API

David Bzazley的Python Brasil 2015 keynote让我发现自己忽略了一件很重要的事。在那个演讲中，David指出，async/await其实是一种异步编程的API(他在Twitter上对我说过
同样的话)。我想David的意思是，我们不应该把async/await当成asnycio的一种别名，而应该利用async/await，让asyncio成为异步编程的通用框架。

David对将async/await作为异步编程API的想法深信不疑，甚至在他的curio项目中实现了自己的事件循环。这也侧面证明了Python中async/await作为异步编程语法的作用(不像其
他集成了事件循环的语言那样，用户需要自己实现事件循环和底层细节)。async/await语法让像curio这样的项目可以进行不同的底层操作(asyncio使用future对象与事件循环进行
交互，而curio使用元祖对象)，还让它们可以有不同的侧重和性能优化(为了更广泛的适用性，asyncio实现了完整的传输和协议层框架，而相对简单的curio则需要用户实现那些框
架，但也因此获得了更快的运行速度)。

看完了Python中异步编程的(简略)历史，很容易得出async/await == asyncio的结论。我想说的是，asyncio导致了Python3.4中异步编程的出现，并且对Python3.5中async/await
的产生居功至伟，但是，async/await的灵活的设计，甚至到了可以不使用asyncio的地步，也不需要为了应用asyncio框架而修改架构。简而言之，async/await语法延续了Python
在保证实用性的同时尽可能的让设计灵活的传统。

一个例子

看到这里，你的脑子里应该已经装满了各种新术语和新概念，但对于这些新事物如何实现异步编程却仍一知半解。为了加深理解，以下是一个(略显做作的)异步编程的例子，包括
完整的从事件循环到相关业务函数的代码。在这个例子中，协程的用途是实现独立的火箭发射倒计时器，产生的效果是同步进行的倒计时。这是通过异步编程而实现的函数并发，
程序执行是有三个协程运行在在同一个线程中，却可以彼此互不干扰。

```
import datetime
import heapq
import types
import time


class Task:

    """Represent how long a coroutine should wait before starting again.

    Comparison operators are implemented for use by heapq. Two-item
    tuples unfortunately don't work because when the datetime.datetime
    instances are equal, comparison falls to the coroutine and they don't
    implement comparison methods, triggering an exception.

    Think of this as being like asyncio.Task/curio.Task.
    """

    def __init__(self, wait_until, coro):
        self.coro = coro
        self.waiting_until = wait_until

    def __eq__(self, other):
        return self.waiting_until == other.waiting_until

    def __lt__(self, other):
        return self.waiting_until < other.waiting_until


class SleepingLoop:

    """An event loop focused on delaying execution of coroutines.

    Think of this as being like asyncio.BaseEventLoop/curio.Kernel.
    """

    def __init__(self, *coros):
        self._new = coros
        self._waiting = []

    def run_until_complete(self):
        # Start all the coroutines.
        for coro in self._new:
            wait_for = coro.send(None)
            heapq.heappush(self._waiting, Task(wait_for, coro))
        # Keep running until there is no more work to do.
        while self._waiting:
            now = datetime.datetime.now()
            # Get the coroutine with the soonest resumption time.
            task = heapq.heappop(self._waiting)
            if now < task.waiting_until:
                # We're ahead of schedule; wait until it's time to resume.
                delta = task.waiting_until - now
                time.sleep(delta.total_seconds())
                now = datetime.datetime.now()
            try:
                # It's time to resume the coroutine.
                wait_until = task.coro.send(now)
                heapq.heappush(self._waiting, Task(wait_until, task.coro))
            except StopIteration:
                # The coroutine is done.
                pass


@types.coroutine
def sleep(seconds):
    """Pause a coroutine for the specified number of seconds.

    Think of this as being like asyncio.sleep()/curio.sleep().
    """
    now = datetime.datetime.now()
    wait_until = now + datetime.timedelta(seconds=seconds)
    # Make all coroutines on the call stack pause; the need to use `yield`
    # necessitates this be generator-based and not an async-based coroutine.
    actual = yield wait_until
    # Resume the execution stack, sending back how long we actually waited.
    return actual - now


async def countdown(label, length, *, delay=0):
    """Countdown a launch for `length` seconds, waiting `delay` seconds.

    This is what a user would typically write.
    """
    print(label, 'waiting', delay, 'seconds before starting countdown')
    delta = await sleep(delay)
    print(label, 'starting after waiting', delta)
    while length:
        print(label, 'T-minus', length)
        waited = await sleep(1)
        length -= 1
    print(label, 'lift-off!')


def main():
    """Start the event loop, counting down 3 separate launches.

    This is what a user would typically write.
    """
    loop = SleepingLoop(countdown('A', 5), countdown('B', 3, delay=2),
                        countdown('C', 4, delay=1))
    start = datetime.datetime.now()
    loop.run_until_complete()
    print('Total elapsed time is', datetime.datetime.now() - start)


if __name__ == '__main__':
    main()
```

正如前文所说，这个例子是有意为之，如果在Python3.5下运行，你会发现虽然三个协程在同一线程中互不干扰，但总运行时间是5秒左右。你可以把Task，SleepingLoop和sleep()
看成asyncio和curio这样生成事件循环的框架提供的接口函数，对普通用户来说，只有countdown()和main()函数才需要关注。到此为止，你应该已经明白，async，await语句，甚
至整个异步编程，都不是完全无法理解的魔术，async/await只是Python为了让异步编程更简便易用而添加的API。

### 我对未来的愿景

我已经理解了Python中的异步编程，我想把它用到所有地方！这个精巧高效的概念完全可以替代原本线程的作用。问题是，Python3.5和async/await都是面世不久的新事物，这就
意味着支持异步编程的库数量不会太多。例如，要发送HTTP请求，你要么手动构造HTTP请求对象(麻烦透顶)，然后用一个类似aiohttp的框架把HTTP放进另外的事件循环(对于
aiohttp，是asyncio)开始操作；要么就等着哪天出现一个像hyper这样的项目对HTTP这类I/O进行抽象，让你可以使用任意的I/O库(遗憾的是，到目前为止hyper只支持HTTP/2)。

我的个人观点是希望像hyper这样的项目可以继续发展，分离从I/O获取二进制数据和解析二进制数据的逻辑。Python中大部分的I/O库都是包揽进行I/O操作和处理从I/O接收的数据
，因此对操作分离进行抽象意义重大。Python标准库的http包也存在同样的问题，有处理I/O的连接对象，却没有HTTP解析器。而如果你希望requests库支持异步编程，那么你可能
要失望了，因为requests从设计上就是同步编程。拥有异步编程能力让Python社区有机会弥补Python语言中没有多层网络栈抽象的缺点。现在Python的优势是可以像运行同步代码
那样运行异步代码，因此填补异步编程空白的工具，可以应用在同步异步两种场景中。

我还希望Python可以增加async协程对yield语句的支持。这可能需要一个新的关键字(也许是anticipate?)，但只使用async语法就不能实现事件循环的情况实在不尽人意。幸运的
是，在这一点上我不是一个人，PEP 492的作者与我观点相同，我觉得这个愿望很有可能成为现实。

### 总结
	总而言之，async和await出现的目的就是为了协程，顺便支持可等待对象，也可以把普通生成器转换成协程。
所有这些都是为了实现并发操作，来提升Python中的异步编程体验。

	相比使用多线程的编程体验，协程功能强大并且更为易用--只用了包括注释在内的不到100行代码就实现了一个
完整的异步编程实例--兼具良好的适用性和运行效率(curio的FAQ里说它的运行速度比twisted快30-40%，比gevent慢10-15%，
别忘了，在Python2+版本中，Twisted用的内存更少而且调试比Go简单，想想我们可以做到什么程度！)。
	能在Python 3中看到async/await的引入，我非常高兴，并且期待Python社区接纳这个新语法，
希望有更多的库和框架支持async/await语法，让所有的Python开发者都可以从异步编程中受益。
class Task(object):
    def __init__(self, coro):
        self.coro = coro
        f = Future()
        f.set_result(None)
        self.step(f)

    def step(self, future):
        try:
            next_future = self.coro.send(future.result)
        except StopInteration:
            return
        next_future.add_done_callback(self.step)
﻿
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
