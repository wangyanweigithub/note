 

具有异步协程的Web爬网程序

杰西·杰鲁·戴维斯（Jesse Jiryu Davis）和吉多·范·罗苏姆（Guido van Rossum）

A. Jesse Jiryu Davis是纽约MongoDB的一名高级工程师。他编写了异步MongoDB Python驱动程序Motor，并
且是MongoDB C驱动程序的首席开发人员和PyMongo团队的成员。他为asyncio和Tornado做出了贡献。他在
http://emptysqua.re上写道。

Guido van Rossum是Python的创建者，Python是网络上和网络外的主要编程语言之一。Python社区将他称为
BDFL（生命仁爱独裁者），直接来自Monty Python短剧。Guido在网络上的主页是http://www.python.org/
~guido/。

介绍

古典计算机科学强调有效的算法，该算法应尽快完成计算。但是许多网络程序花费的时间不是用于计算，而
是用于保持许多缓慢或不常发生的连接。这些程序提出了非常不同的挑战：有效地等待大量网络事件。解决
此问题的一种现代方法是异步I / O或“异步”。

本章介绍了一个简单的Web搜寻器。搜寻器是原型异步应用程序，因为它等待许多响应，但很少进行计算。
它一次可以获取的页面越多，完成的越早。如果它为每个进行中的请求分配一个线程，则随着并发请求数的
增加，它将先耗尽内存或其他与线程相关的资源，然后再耗尽套接字。通过使用异步I / O，它避免了线程
的需要。

我们分三个阶段介绍该示例。首先，我们显示一个异步事件循环并草绘将事件循环与回调一起使用的爬虫：
这非常有效，但是将其扩展到更复杂的问题将导致难以处理的意大利面条式代码。因此，第二，我们证明
Python协程既高效又可扩展。我们使用生成器函数在Python中实现简单的协程。在第三阶段，我们使用
Python标准的“ asyncio”库^1中的功能齐全的协程，并使用异步队列进行协调。

任务

Web搜寻器会查找并下载网站上的所有页面，也许可以对它们进行存档或建立索引。从根URL开始，它将获取
每个页面，解析该页面以查找到看不见的页面的链接，然后将它们添加到队列中。当它获取没有看不见链接
的页面并且队列为空时，它将停止。

我们可以通过同时下载许多页面来加快此过程。搜寻器找到新链接时，它将在单独的套接字上启动对新页面
的同步获取操作。它解析响应到达时的响应，将新链接添加到队列中。可能会出现收益递减的情况，太多的
并发会降低性能，因此我们限制了并发请求的数量，并将其余链接留在队列中，直到一些正在进行的请求完
成为止。

传统方法

我们如何使搜寻器并发？传统上，我们将创建一个线程池。每个线程负责一次通过套接字下载一页。例如，
要从下载页面xkcd.com：

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

默认情况下，套接字操作处于阻塞状态：当线程调用类似connect或的方法时recv，它将暂停直到操作完成
。^2因此，要一次下载许多页面，我们需要许多线程。复杂的应用程序通过将空闲线程保留在线程池中，然
后将其检出以将其重新用于后续任务来摊销线程创建的成本；它与连接池中的套接字相同。

但是，线程价格昂贵，并且操作系统对进程，用户或计算机可能具有的线程数量施加各种硬性限制。在
Jesse的系统上，Python线程消耗大约5万个内存，而启动数万个线程会导致失败。如果我们在并发套接字上
扩展到数以万计的同时操作，则在线程用完之前，线程用完了。每个线程的开销或线程上的系统限制是瓶颈
。

Dan Kegel 在他的有影响力的文章“ C10K问题” ^3中概述了多线程对I / O并发的局限性。他开始，

    是时候让Web服务器同时处理一万个客户端了，您不觉得吗？毕竟，网络现在是一个很大的地方。

凯格尔（Kegel）于1999年创造了“ C10K”一词。现在，一万个连接听起来很精致，但问题只是在大小上有所
改变，而没有改变。当时，为C10K在每个连接中使用线程是不切实际的。现在，上限提高了几个数量级。确
实，我们的玩具网络爬虫可以在线程上正常工作。然而，对于具有数十万个连接的超大型应用程序，上限仍
然存在：有一个限制，大多数系统仍然可以创建套接字，但线程用完了。我们该如何克服呢？

异步

异步I / O框架使用非阻塞套接字在单个线程上执行并发操作。在异步搜寻器中，我们在开始连接到服务器
之前将套接字设置为非阻塞：

sock = socket.socket()
sock.setblocking(False)
try:
    sock.connect(('xkcd.com', 80))
except BlockingIOError:
    pass

令人烦恼的是connect，即使非阻塞套接字正常运行，它也会从抛出异常。此异常复制下面的C函数集的刺激
性行为errno来EINPROGRESS告诉你它已经开始。

现在，我们的搜寻器需要一种方法来知道何时建立连接，以便它可以发送HTTP请求。我们可以继续紧紧地尝
试：

request = 'GET {} HTTP/1.0\r\nHost: xkcd.com\r\n\r\n'.format(url)
encoded = request.encode('ascii')

while True:
    try:
        sock.send(encoded)
        break  # Done.
    except OSError as e:
        pass

print('sent')

这种方法不仅浪费电力，而且不能有效地等待多个插座上的事件。在远古时代，BSD Unix解决此问题的方法
是selectC函数，它等待事件在无阻塞套接字或其中的一小部分发生。如今与连接的数量巨大的互联网应用
的需求已经导致替代品像poll，然后kqueue在BSD和epollLinux上。这些API与相似select，但在大量连接时
表现良好。

Python 3.4 DefaultSelector使用了select系统上可用的最佳类函数。要注册有关网络I / O的通知，我们
创建一个非阻塞套接字，并使用默认选择器注册它：

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

我们忽略了虚假的错误并调用selector.register，传入套接字的文件描述符和一个常量，该常量表示我们
正在等待的事件。要在建立连接时得到通知，我们通过EVENT_WRITE：即，我们想知道套接字何时是“可写的
”。我们还传递了Python函数，connected以便在该事件发生时运行。这样的函数称为回调。

当选择器接收到I / O通知时，我们将对其进行循环处理：

def loop():
    while True:
        events = selector.select()
        for event_key, event_mask in events:
            callback = event_key.data
            callback()

connected回调存储为event_key.data，一旦连接了非阻塞套接字，我们将检索并执行该回调。

与上面的快速旋转循环不同，select此处的调用会暂停，等待下一个I / O事件。然后，循环运行等待这些
事件的回调。未完成的操作将保持待处理状态，直到事件循环的将来某个时间间隔。

我们已经展示了什么？我们展示了如何开始操作并在操作就绪后执行回调。异步框架建立在我们已经展示的
两个功能（无阻塞套接字和事件循环）的基础上，可以在单个线程上运行并发操作。

我们在这里实现了“并发性”，但是没有实现传统上所谓的“并行性”。也就是说，我们构建了一个很小的系统
，该系统执行重叠的I / O。它可以在其他飞机飞行时开始新的操作。它实际上并没有利用多个内核来并行
执行计算。但是，此系统是针对I / O约束的问题而不是CPU约束的问题而设计的。^4

因此，我们的事件循环在并发I / O方面效率很高，因为它不会将线程资源分配给每个连接。但是在继续之
前，重要的是要纠正常见的误解，即异步比多线程要快。通常不是这样-实际上，在Python中，在服务少量
活动连接时，像我们这样的事件循环比多线程要慢一些。在没有全局解释器锁的运行时中，线程在这种工作
负载下会表现得更好。异步I / O最适合的是具有许多不频繁事件的缓慢或睡眠连接的应用程序。^5

回调编程

到目前为止，我们已经使用可构建的异步框架构建了Web搜寻器？即使是简单的URL提取程序，也很难编写。

我们从尚未获取的URL的全局集合开始，并看到以下URL：

urls_todo = set(['/'])
seen_urls = set(['/'])

该seen_urls集合包括urls_todo完整的URL。两组都用根URL“ /”初始化。

提取页面将需要一系列回调。在connected当一个插座连接回调火灾，并发送GET请求到服务器。但是随后它
必须等待响应，因此它注册了另一个回调。如果在触发该回调时仍无法读取完整的响应，则将再次注册，依
此类推。

让我们将这些回调收集到一个Fetcher对象中。它需要一个URL，一个套接字对象以及一个累积响应字节的位
置：

class Fetcher:
    def __init__(self, url):
        self.response = b''  # Empty array of bytes.
        self.url = url
        self.sock = None

我们首先调用Fetcher.fetch：

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

该fetch方法开始连接套接字。但是请注意，在建立连接之前该方法将返回。它必须将控制权返回给事件循
环以等待连接。要了解原因，请想象我们的整个应用程序的结构如下：

# Begin fetching http://xkcd.com/353/
fetcher = Fetcher('/353/')
fetcher.fetch()

while True:
    events = selector.select()
    for event_key, event_mask in events:
        callback = event_key.data
        callback(event_key, event_mask)

调用时，所有事件通知均在事件循环中处理select。因此fetch必须将控制权交给事件循环，以便程序知道
套接字已连接的时间。只有这样，循环才会运行connected在上述结尾处注册的回调fetch。

这是实现connected：

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

该方法发送一个GET请求。如果send无法立即发送整个消息，则实际的应用程序将检查返回值。但是我们的
要求很小，我们的应用程序也不复杂。它愉快地调用send，然后等待响应。当然，它必须注册另一个回调并
放弃对事件循环的控制。下一个也是最后一个回调，read_response处理服务器的回复：

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

每当选择器看到套接字是“可读的”时，都会执行该回调，这可能意味着两件事：套接字具有数据或已关闭。

回调从套接字请求最多四千字节的数据。如果准备就绪，则chunk包含可用的任何数据。如果还有更多内容
，chunk则为4 KB长，并且套接字保持可读状态，因此事件循环在下一个刻度上再次运行此回调。响应完成
后，服务器已关闭套接字并且chunk为空。

该parse_links方法（未显示）返回一组URL。我们为每个新URL启动一个新的访存程序，没有并发上限。请
注意带有回调的异步编程的一个不错的功能：我们不需要在共享数据更改周围使用互斥锁，例如，当我们向
添加链接时seen_urls。没有抢先式多任务处理，因此我们不能在代码中的任意点被打断。

我们添加一个全局stopped变量并使用它来控制循环：

stopped = False

def loop():
    while not stopped:
        events = selector.select()
        for event_key, event_mask in events:
            callback = event_key.data
            callback()

下载所有页面后，提取程序将停止全局事件循环，并退出程序。

这个例子使异步的问题很明显：意大利面条代码。我们需要某种方式来表示一系列计算和I / O操作，并计
划多个此类操作以同时运行。但是，如果没有线程，则无法将一系列操作收集到单个函数中：每当一个函数
开始I / O操作时，它将明确保存将来需要的任何状态，然后返回。您有责任考虑并编写此状态保存代码。

让我们解释一下这是什么意思。考虑一下我们使用传统的阻塞套接字在线程上获取URL的简单程度：

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

在一个套接字操作和下一个套接字操作之间，此功能还记得什么状态？它具有套接字，URL和累积response
。在线程上运行的函数使用编程语言的基本功能将此临时状态存储在其堆栈上的局部变量中。该函数还具有
“继续”功能，即在I / O完成后计划执行的代码。运行时通过存储线程的指令指针来记住连续性。您无需考
虑还原这些局部变量以及I / O之后的延续。它内置于语言中。

但是对于基于回调的异步框架，这些语言功能没有帮助。在等待I / O时，函数必须明确保存其状态，因为
该函数会在I / O完成之前返回并丢失其堆栈帧。代替局部变量，我们基于回调的示例存储Fetcher实例sock
并将其response作为selfFetcher实例的属性。代替指令指针，它通过注册回调connected和来存储其继续
read_response。随着应用程序功能的增长，我们在回调之间手动保存的状态的复杂性也在增加。这种繁重
的簿记工作使编码人员容易出现偏头痛。

更糟糕的是，如果回调在计划链中的下一个回调之前引发异常，会发生什么情况？假设我们在parse_links
方法上做得很差，并且抛出了解析某些HTML的异常：

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

堆栈跟踪仅显示事件循环正在运行回调。我们不记得是什么导致了错误。链条的两端都断了：我们忘记了我
们要去的地方以及我们从何而来。这种上下文丢失被称为“堆栈撕裂”，并且在许多情况下使研究人员感到困
惑。堆栈撕裂还阻止我们为回调链安装异常处理程序，即“ try / except”块包装函数调用及其后代树的方
式。^6

因此，除了关于多线程和异步的相对效率的长期争论之外，还有另一种争论是关于哪一个更容易出错：如果
您在同步线程时出错，线程很容易受到数据争用的影响，但是回调方法却难以调试由于堆栈撕裂。

协程

我们以承诺诱使您。可以编写将回调效率与多线程编程经典外观完美结合的异步代码。这种组合是通过称为
“协程”的模式实现的。使用Python 3.4的标准asyncio库和名为“ aiohttp”的包，在协程中获取URL非常直接
^7：

    @asyncio.coroutine
    def fetch(self, url):
        response = yield from self.session.get(url)
        body = yield from response.read()

它也是可扩展的。与每个线程50k的内存和操作系统对线程的严格限制相比，Python协程在Jesse的系统上仅
占用3k的内存。Python可以轻松启动成千上万个协程。

协程的概念可以追溯到计算机科学的早期，它很简单：它是一个可以暂停和恢复的子例程。线程由操作系统
抢占式地执行多任务，而协同程序协同执行多任务：它们选择何时暂停以及下一步运行哪个协程。

协程有很多实现；即使在Python中也有几个。Python 3.4中标准“ asyncio”库中的协程基于生成器，Future
类和“ yield from”语句生成。从Python 3.5开始，协程是该语言本身的本机功能^8。但是，了解协程最初
是使用预先存在的语言工具在Python 3.4中实现的，是解决Python 3.5原生协程的基础。

为了解释Python 3.4的基于生成器的协程，我们将对生成器及其在异步中如何用作协程进行说明，并相信您
会像阅读它一样喜欢阅读它。解释了基于生成器的协程之后，我们将在异步Web搜寻器中使用它们。

Python生成器如何工作

在掌握Python生成器之前，您必须了解常规Python函数的工作方式。通常，当Python函数调用子例程时，该
子例程会保留控制权，直到它返回或引发异常为止。然后控制权返回给调用者：

>>> def foo():
...     bar()
...
>>> def bar():
...     pass

标准的Python解释器是用C编写的。执行Python函数的C函数被巧妙地称为PyEval_EvalFrameEx。它接受一个
Python堆栈框架对象，并在框架上下文中评估Python字节码。这是字节码foo：

>>> import dis
>>> dis.dis(foo)
  2           0 LOAD_GLOBAL              0 (bar)
              3 CALL_FUNCTION            0 (0 positional, 0 keyword pair)
              6 POP_TOP
              7 LOAD_CONST               0 (None)
             10 RETURN_VALUE

该foo函数加载bar到其堆栈上并调用它，然后从堆栈中弹出其返回值，加载None到堆栈上，然后返回None。

当PyEval_EvalFrameEx遇到CALL_FUNCTION字节码时，它将创建一个新的Python堆栈框架并进行递归：即，
它PyEval_EvalFrameEx使用新的框架递归调用，该新框架用于执行bar。

至关重要的是要了解Python堆栈帧是在堆内存中分配的！Python解释器是普通的C程序，因此其堆栈框架是
普通的堆栈框架。但是它操纵的Python堆栈框架在堆上。除其他意外之外，这意味着Python堆栈框架可以超
过其函数调用的寿命。要以交互方式查看，请从中保存当前帧bar：

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

图5.1-函数调用

图5.1-函数调用

现在为Python生成器设置了阶段，它们使用相同的构造块—代码对象和堆栈框架—产生了惊人的效果。

这是一个生成器函数：

>>> def gen_fn():
...     result = yield 1
...     print('result of yield: {}'.format(result))
...     result2 = yield 2
...     print('result of 2nd yield: {}'.format(result2))
...     return 'done'
...

当Python编译gen_fn为字节码时，它会看到该yield语句并知道这gen_fn是一个生成器函数，而不是常规函
数。它设置一个标志来记住这一事实：

>>> # The generator flag is bit position 5.
>>> generator_bit = 1 << 5
>>> bool(gen_fn.__code__.co_flags & generator_bit)
True

当您调用生成器函数时，Python会看到generator标志，并且它实际上并未运行该函数。而是创建一个生成
器：

>>> gen = gen_fn()
>>> type(gen)
<class 'generator'>

Python生成器封装了堆栈框架以及对某些代码的引用，这些代码为gen_fn：

>>> gen.gi_code.co_name
'gen_fn'

所有生成器都从调用gen_fn指向此相同的代码。但是每个都有自己的堆栈框架。该堆栈帧不在任何实际堆栈
上，它位于堆内存中等待使用：

图5.2-发电机

图5.2-发电机

框架具有“最后一条指令”指针，即最近执行的指令。首先，最后一条指令指针为-1，表示生成器尚未开始：

>>> gen.gi_frame.f_lasti
-1

当我们调用时send，生成器到达其第一个yield，并暂停。返回值为send1，因为这是gen传递给yield表达式
的内容：

>>> gen.send(None)
1

现在，生成器的指令指针从头开始是3个字节码，部分经过编译的Python的56个字节：

>>> gen.gi_frame.f_lasti
3
>>> len(gen.gi_code.co_code)
56

生成器可以随时通过任何函数恢复，因为生成器的堆栈帧实际上不在堆栈上：它在堆上。它在调用层次结构
中的位置不是固定的，并且不需要遵循常规函数执行的先进先后顺序。它被解放，像云一样自由漂浮。

我们可以将值“ hello”发送到生成器中，它成为yield表达式的结果，并且生成器继续运行直到产生2：

>>> gen.send('hello')
result of yield: hello
2

现在，它的堆栈框架包含局部变量result：

>>> gen.gi_frame.f_locals
{'result': 'hello'}

从中创建的其他生成器gen_fn将具有自己的堆栈框架和局部变量。

当我们send再次调用时，生成器从其第二个继续yield，并通过引发特殊StopIteration异常结束：

>>> gen.send('goodbye')
result of 2nd yield: goodbye
Traceback (most recent call last):
  File "<input>", line 1, in <module>
StopIteration: done

异常有一个值，它是生成器的返回值：string "done"。

用发电机建造协程

因此，生成器可以暂停，并且可以使用一个值恢复，并且它具有返回值。听起来像是构建异步编程模型的好
原始方法，而没有意大利面条回调！我们要构建一个“协程”：一个与程序中其他例程协同调度的例程。我们
的协程将是Python标准“ asyncio”库中的协程的简化版本。与异步中一样，我们将使用生成器，期货和“
yield from”语句。

首先，我们需要一种方法来代表协程正在等待的某些未来结果。精简版：

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

未来最初是“待定”。通过调用来“解决”它set_result。^9

让我们调整提取程序以使用期货和协程。我们写fetch了一个回调：

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

该fetch方法开始连接套接字，然后注册connected要在套接字准备好时执行的回调。现在，我们可以将这两
个步骤合并为一个协程：

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

现在fetch是生成器函数，而不是常规函数，因为它包含一条yield语句。我们创建一个未决的未来，然后让
它暂停fetch直到套接字准备就绪。内部功能on_connected解决了未来。

但是，当未来解决时，是什么恢复了发电机？我们需要一个协程司机。让我们称之为“任务”：

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

# Begin fetching http://xkcd.com/353/
fetcher = Fetcher('/353/')
Task(fetcher.fetch())

loop()

该任务fetch通过发送None到生成器来启动它。然后fetch运行直到产生未来，该任务将捕获为next_future
。连接套接字后，事件循环将运行回调on_connected，该回调可解决将来的问题，即调用step并恢复fetch
。

用分解协程 yield from

连接套接字后，我们将发送HTTP GET请求并读取服务器响应。这些步骤不再需要分散在回调之间。我们将它
们收集到相同的生成器函数中：

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

该代码从套接字读取整个消息，通常看起来很有用。我们如何将其从fetch子程序中分解出来？现在，著名
的Python 3 yield from登上了舞台。它使一个生成器可以委派给另一个生成器。

要了解如何操作，让我们回到简单的生成器示例：

>>> def gen_fn():
...     result = yield 1
...     print('result of yield: {}'.format(result))
...     result2 = yield 2
...     print('result of 2nd yield: {}'.format(result2))
...     return 'done'
...

要从另一个生成器调用此生成器，请使用以下方法委派给它yield from：

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

该caller发电机的作用就好像它是gen，它是委托给发电机：

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

虽然caller从收益率gen，caller不前进。注意yield from，即使内部生成器gen从一个yield语句前进到下
一个语句，它的指令指针仍位于语句的位置15 。^10从我们外部的角度来看caller，我们无法确定它产生的
值是来自caller还是来自它委托的生成器。从内部gen，我们无法确定值是从内部发送caller还是从外部发
送。该yield from语句是一个无摩擦的通道，值通过该通道流入和流出，gen直到gen完成为止。

协程可以将工作委托给子协程，yield from并接收工作结果。请注意，上面caller打印了“ yield-from：
done的返回值”。当gen完成后，其返回值成为价值yield from的声明caller：

    rv = yield from gen

之前，当我们批评基于回调的异步编程时，我们最直接的抱怨是关于“堆栈撕裂”的：当回调引发异常时，堆
栈跟踪通常是无用的。它仅显示事件循环正在运行回调，而不是为什么。协程如何收费？

>>> def gen_fn():
...     raise Exception('my error')
>>> caller = caller_fn()
>>> caller.send(None)
Traceback (most recent call last):
  File "<input>", line 1, in <module>
  File "<input>", line 3, in caller_fn
  File "<input>", line 2, in gen_fn
Exception: my error

这更有用！堆栈跟踪显示caller_fn委托gen_fn它引发错误。更令人欣慰的是，我们可以将对子协程的调用
包装在异常处理程序中，与常规子例程相同：

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

因此，就像常规子例程一样，我们使用子协程分解逻辑。让我们从提取程序中考虑一些有用的子协程。我们
编写一个read协程以接收一个块：

def read(sock):
    f = Future()

    def on_readable():
        f.set_result(sock.recv(4096))

    selector.register(sock.fileno(), EVENT_READ, on_readable)
    chunk = yield f  # Read one chunk.
    selector.unregister(sock.fileno())
    return chunk

我们read以read_all接收整个消息的协程为基础：

def read_all(sock):
    response = []
    # Read whole response.
    chunk = yield from read(sock)
    while chunk:
        response.append(chunk)
        chunk = yield from read(sock)

    return b''.join(response)

如果以正确的方式斜视，这些yield from语句就会消失，并且这些语句看起来像是常规函数，它们会阻塞I
/ O。但事实上，read和read_all是协同程序。从read暂停read_all直到I / O完成为止。虽然read_all暂停
，ASYNCIO的事件循环做其他工作，并等待其他I / O活动; 一旦事件就绪，将在下一个循环滴答声中
read_all恢复结果read。

在堆栈的根目录，fetch调用read_all：

class Fetcher:
    def fetch(self):
         # ... connection logic from above, then:
        sock.send(request.encode('ascii'))
        self.response = yield from read_all(sock)

奇迹般地，Task类不需要修改。它fetch与前面一样驱动外部协程：

Task(fetcher.fetch())
loop()

当read产生未来时，任务将通过yield from语句通道接收它，就像未来直接从中产生一样fetch。当循环解
析未来时，任务将其结果发送到中fetch，并且接收该值read，就像任务read直接在驱动一样：

图5.3-产量

图5.3-产量

为了完善我们的协程实现，我们会在3月结束：我们的代码yield在等待未来时使用，但是yield from在它委
托给子协程时使用。如果我们yield from在协程暂停时使用，它将更加完善。这样，协程就不必关心它正在
等待什么类型的事情。

我们利用Python在生成器和迭代器之间的深层对应关系。对于调用者而言，推进生成器与推进迭代器相同。
因此，我们通过实现一个特殊的方法使Future类变得可迭代：

    # Method on Future class.
    def __iter__(self):
        # Tell Task to resume me here.
        yield self
        return self.result

未来的__iter__方法是协程，它产生了未来本身。现在，当我们替换这样的代码时：

# f is a Future.
yield f

...有了这个：

# f is a Future.
yield from f

...结果是一样的！驾驶任务从其对的调用中接收未来send，当未来解决后，它将新结果发送回协程。

yield from随处使用的好处是什么？为什么这比等待期货yield和委托协程更好yield from呢？更好是因为
现在，一个方法可以在不影响调用者的情况下自由地更改其实现：它可能是返回将来将解析为值的普通方法
，或者它可能是包含yield from语句并返回值的协程。在这两种情况下，调用方仅需要yield from该方法即
可等待结果。

读者们，我们已经愉快地完成了协程在异步中的阐述。我们凝视着发电机的机械，并勾画出期货和任务的实
施方案。我们概述了asyncio如何实现两全其美：并发I / O比线程更有效，比回调更易懂。当然，真正的异
步比我们的草图复杂得多。真正的框架解决了零拷贝I / O，公平调度，异常处理以及大量其他功能的问题
。

对于一个异步用户，使用协程进行编码比您在这里看到的要简单得多。在上面的代码中，我们从第一条原则
实现了协程，因此您看到了回调，任务和期货。您甚至看到了无阻塞的套接字以及对的调用select。但是，
当需要使用asyncio构建应用程序时，这些都不会出现在您的代码中。如我们所承诺的，您现在可以轻松获
取URL：

    @asyncio.coroutine
    def fetch(self, url):
        response = yield from self.session.get(url)
        body = yield from response.read()

对此博览会感到满意后，我们回到了最初的任务：使用asyncio编写一个异步Web搜寻器。

协程

我们首先描述了我们希望爬虫如何工作。现在是时候用asyncio协程实现它了。

我们的搜寻器将获取第一页，解析其链接，并将其添加到队列中。之后，它会散布在整个网站上，并发获取
页面。但是为了限制客户端和服务器上的负载，我们希望运行一些最大数量的工作线程，而不再需要更多。
每当工作者完成获取页面时，都应立即从队列中拉出下一个链接。我们将经历没有足够工作要做的时期，因
此一些工人必须停下来。但是，当工作人员访问带有新链接的页面时，队列突然增加，任何暂停的工作人员
都应醒来并开始破解。最后，我们的程序一旦完成工作就必须退出。

想象一下，如果工人是线程。我们将如何表达搜寻器的算法？我们可以使用Python标准库中的同步队列^11
。每次将项目放入队列时，队列都会增加其“任务”计数。工作人员线程task_done在完成项目工作后调用。
主线程Queue.join一直阻塞，直到队列中放入的每个项目都被task_done调用匹配，然后退出。

协程与asyncio队列使用完全相同的模式！首先我们导入它^12：

try:
    from asyncio import JoinableQueue as Queue
except ImportError:
    # In Python 3.5, asyncio.JoinableQueue is
    # merged into Queue.
    from asyncio import Queue

我们在爬虫类中收集工人的共享状态，并在其crawl方法中编写主要逻辑。我们从crawl协程开始并运行
asyncio的事件循环，直到crawl完成：

loop = asyncio.get_event_loop()

crawler = crawling.Crawler('http://xkcd.com',
                           max_redirect=10)

loop.run_until_complete(crawler.crawl())

搜寻器以根URL开头，并且max_redirect为获取任何一个URL愿意遵循的重定向次数。它将对(URL,
max_redirect)放入队列。（出于这个原因，请继续关注。）

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

现在，队列中未完成的任务数为1。返回主脚本，我们启动事件循环和crawl方法：

loop.run_until_complete(crawler.crawl())

该crawl协程揭开序幕的工人。它就像一个主线程：它阻塞join直到所有任务完成，而工作人员在后台运行
。

    @asyncio.coroutine
    def crawl(self):
        """Run the crawler until all work is done."""
        workers = [asyncio.Task(self.work())
                   for _ in range(self.max_tasks)]

        # When all work is done, exit.
        yield from self.q.join()
        for w in workers:
            w.cancel()

如果工人是线程，我们可能不希望立即启动它们。为了避免在确定线程是必需的之前创建昂贵的线程，线程
池通常会按需增长。但是协程很便宜，因此我们只需要开始允许的最大数量即可。

有趣的是，我们如何关闭搜寻器。当join未来解决时，辅助任务仍然有效，但已暂停：它们等待更多URL，
但没有一个出现。因此，主协程会在退出前将其取消。否则，当Python解释器关闭并调用所有对象的析构函
数时，活动任务会大声喊叫：

ERROR:asyncio:Task was destroyed but it is pending!

以及如何cancel运作？生成器具有我们尚未向您显示的功能。您可以从外部将异常抛出到生成器中：

>>> gen = gen_fn()
>>> gen.send(None)  # Start the generator as usual.
1
>>> gen.throw(Exception('error'))
Traceback (most recent call last):
  File "<input>", line 3, in <module>
  File "<input>", line 2, in gen_fn
Exception: error

生成器由恢复throw，但是现在引发异常。如果生成器的调用堆栈中没有代码捕获到该异常，则异常会冒泡
回到顶部。因此，要取消任务的协同程序：

    # Method of Task class.
    def cancel(self):
        self.coro.throw(CancelledError)

无论生成器在何处暂停，它都会在某些yield from语句处恢复并引发异常。我们通过任务的step方法处理取
消：

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

现在任务知道它已被取消，因此当它被销毁时，它不会对光的死亡感到愤怒。

一旦crawl取消了工作人员，便退出。事件循环看到协程已完成（我们将在稍后看到），它也将退出：

loop.run_until_complete(crawler.crawl())

该crawl方法包括我们的主要协程必须做的所有事情。是工作程序协程从队列中获取URL，获取它们并解析它
们以获取新链接。每个工人work独立运行协程：

    @asyncio.coroutine
    def work(self):
        while True:
            url, max_redirect = yield from self.q.get()

            # Download page and add new links to self.q.
            yield from self.fetch(url, max_redirect)
            self.q.task_done()

Python看到此代码包含yield from语句，并将其编译为生成器函数。因此，在中crawl，当主协程调用
self.work十次时，它实际上不执行此方法：它仅创建十个生成器对象，并带有对此代码的引用。它将每个
都包装在一个Task中。任务接收生成器产生的每个未来，并send在未来解决时通过调用每个未来的结果来驱
动生成器。因为生成器具有自己的堆栈框架，所以它们使用独立的局部变量和指令指针独立运行。

工人通过队列与同事进行协调。它等待带有以下内容的新URL：

    url, max_redirect = yield from self.q.get()

队列的get方法本身就是协程：它暂停直到有人将项目放入队列中，然后继续并返回该项目。

顺便说一句，这是在主协程程序取消搜寻时在爬网结束时工人暂停的地方。从协程的角度来看，它在循环周
围的最后行程是在yield from升高时结束的CancelledError。

当工作人员获取页面时，它将解析链接并将新链接放入队列，然后调用task_done以减少计数器。最终，工
作人员将获取其URL已经全部获取的页面，并且队列中也没有任何工作。因此，此工作人员要求task_done将
计数器减为零的调用。然后crawl，正在等待队列join方法的取消暂停并完成。

我们承诺要解释为什么队列中的项目是成对的，例如：

# URL to fetch, and the number of redirects left.
('http://xkcd.com/353', 10)

新的URL剩余十个重定向。提取此特定URL会导致重定向到带有斜杠的新位置。我们减少剩余的重定向次数，
并将下一个位置放入队列：

# URL with a trailing slash. Nine redirects left.
('http://xkcd.com/353/', 9)

aiohttp默认情况下，我们使用的软件包将遵循重定向并提供最终响应。但是，我们告诉它不要这样做，并
在搜寻器中处理重定向，因此它可以合并指向相同目的地的重定向路径：如果我们已经看到此URL，它就在
其中，self.seen_urls并且我们已经从另一个路径开始了入口点：

图5.4-重定向

图5.4-重定向

搜寻器获取“ foo”，并看到它重定向到“ baz”，因此将“ baz”添加到队列和中seen_urls。如果它获取的下
一页是“ bar”，它也重定向到“ baz”，则获取程序不会再次排队“ baz”。如果响应是页面而不是重定向，
fetch则将其解析为链接并将新链接放入队列。

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

如果这是多线程代码，则在竞争条件下会很糟糕。例如，工作程序检查链接是否在其中seen_urls，如果不
是，则将其放入队列并将其添加到中seen_urls。如果在两个操作之间中断了链接，则另一个工作程序可能
会从不同的页面解析相同的链接，还要注意它不在中seen_urls，并将其添加到队列中。现在，同一链接在
队列中两次，从而（最多）导致重复的工作和错误的统计信息。

但是，协程仅易受yield from语句中断的影响。这是一个关键区别，它使协程代码比多线程代码更不容易出
现竞争：多线程代码必须通过抓住锁来显式进入关键部分，否则将被中断。默认情况下，Python协程是不可
中断的，只有在显式屈服时才让出控制权。

我们不再需要像基于回调的程序中那样的访存器类。该类是缺少回调的一种解决方法：在等待I / O时，它
们需要一些存储状态的位置，因为它们的局部变量不会在调用之间保留。但是fetch协程可以像常规函数一
样将其状态存储在局部变量中，因此不再需要类。

当fetch完成处理时，它返回给调用者的服务器响应，work。该work方法调用task_done队列，然后从队列中
获取要提取的下一个URL。

当fetch将新链接放入队列时，它将增加未完成任务的数量，并使等待的主协程q.join暂停。但是，如果没
有看不见的链接，而这是队列中的最后一个URL，那么在work调用时task_done，未完成任务的计数将降为零
。该事件停止join，主要协程完成。

协调工人和主要协程的队列代码如下^13所示：

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

主要协程crawl从产生join。因此，当最后一个工作人员将未完成的任务计数减为零时，它表示crawl要恢复
并完成。

旅程快结束了。我们的程序始于对以下内容的调用crawl：

loop.run_until_complete(self.crawler.crawl())

程序如何结束？由于crawl是生成器函数，因此调用它会返回生成器。为了驱动生成器，asyncio将其包装在
一个任务中：

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

任务完成后，它将引发StopError，循环将其用作已正常完成的信号。

那是什么该任务具有称为add_done_callback和的方法result。您可能会认为任务类似于未来。你的直觉是
正确的。我们必须承认有关我们向您隐藏的Task类的详细信息：任务是未来。

class Task(Future):
    """A coroutine wrapped in a Future."""

通常，未来会由其他人解决set_result。但是，当协程停止时，任务会自行解决。请记住，从我们之前对
Python生成器的探索中可以知道，当生成器返回时，它会抛出特殊StopIteration异常：

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

因此，当事件循环调用时task.add_done_callback(stop_callback)，它准备被任务停止。这是
run_until_complete一次：

    # Method of event loop.
    def run_until_complete(self, coro):
        task = Task(coro)
        task.add_done_callback(stop_callback)
        try:
            self.run_forever()
        except StopError:
            pass

当任务捕获StopIteration并自行解决时，回调StopError从循环内引发。循环停止，调用堆栈退回到
run_until_complete。我们的程序完成了。

结论

现代程序越来越经常受I / O约束，而不是受CPU约束。对于此类程序，Python线程是两全其美的方法：全局
解释器锁阻止它们实际并行执行计算，而抢占式切换使它们易于出现竞争。异步通常是正确的模式。但是随
着基于回调的异步代码的增长，它往往会变得混乱不堪。协程是一个整洁的选择。它们自然地包含在子例程
中，并具有健全的异常处理和堆栈跟踪功能。

如果我们斜视以使yield from语句模糊，那么协程看起来就像是执行传统阻塞I / O的线程。我们甚至可以
将协程与来自多线程编程的经典模式进行协调。无需重新发明。因此，与回调相比，协程对于使用多线程的
程序员是一个诱人的习惯用法。

但是，当我们睁开眼睛并专注于这些yield from语句时，我们看到它们在协程让步并允许其他人运行时标记
了点。与线程不同，协程会显示我们的代码可以在何处中断和无法在何处中断。Glyph Lefkowitz 在他的启
发性文章“不屈不挠” ^14中写道：“线程使本地推理变得困难，本地推理可能是软件开发中最重要的事情。”
但是，显式屈服可以“通过检查例程本身而不是检查整个系统来了解例程的行为（从而，正确性）”。

本章是在Python和异步技术的复兴中撰写的。您刚刚学会了设计的基于生成器的协程，已于2014年3月在
Python 3.4的“异步”模块中发布。2015年9月，Python 3.5随语言本身内置的协程一起发布。这些原生协程
以新语法“ async def”声明，而不是“ yield from”，而是使用新的“ await”关键字委派给协程或等待
Future。

尽管取得了这些进步，但核心思想仍然存在。Python的新本机协程在语法上将与生成器不同，但工作方式非
常相似。实际上，他们将在Python解释器中共享一个实现。任务，将来和事件循环将继续在异步中发挥作用
。

既然您知道了异步协程的工作原理，就可以在很大程度上忘记细节。机器被塞在一个精巧的接口后面。但是
，您掌握了基础知识后，便可以在现代异步环境中正确有效地进行编码。

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

 1. Guido在PyCon 2013上引入了标准异步库，称为“郁金香” 。↩

 2. send如果收件人无法及时确认未完成的消息，并且系统的传出数据缓冲区已满，则即使呼叫也会阻塞。
    ↩

 3. http://www.kegel.com/c10k.html ↩

 4. Python的全局解释器锁始终禁止在一个进程中并行运行Python代码。用Python并行化受CPU约束的算法
    需要多个进程，或者用C编写代码的并行部分。但这是另一天的话题。↩

 5. Jesse在“什么是异步，它如何工作以及何时应使用它？”中列出了使用异步的适应症和禁忌症：。迈克·
    拜尔相比ASYNCIO的吞吐量，而且多线程在不同的工作负载“异步Python和数据库”：↩

 6. 对于复杂的解决这个问题，看http://www.tornadoweb.org/en/stable/stack_context.html ↩

 7. 该@asyncio.coroutine装饰是没那么神奇。实际上，如果它装饰生成器函数并且PYTHONASYNCIODEBUG未
    设置环境变量，则装饰器几乎不执行任何操作。_is_coroutine为了方便框架的其他部分，它仅设置了
    属性。可以将asyncio与根本没有装饰的裸机一起使用@asyncio.coroutine。↩

 8. PEP 492 “具有异步和等待语法的协程”中描述了Python 3.5的内置协程。↩

 9. 这个未来有许多缺陷。例如，一旦解决了这个未来，产生协程的协程应立即恢复而不是暂停，但是对于
    我们的代码则不能。有关完整的实现，请参见asyncio的Future类。↩

10. 实际上，这正是CPython中“收益”的工作方式。函数在执行每个语句之前会递增其指令指针。但是，在
    外部生成器执行“ yield from”之后，它会从其指令指针中减去1，以保持自身固定在“ yield from”语
    句中。然后，它产生给它的调用者。循环重复直到内部生成器抛出为止StopIteration，此时外部生成
    器最终允许自己前进到下一条指令。↩

11. https://docs.python.org/3/library/queue.html ↩

12. https://docs.python.org/3/library/asyncio-sync.html ↩

13. 实际asyncio.Queue实现使用asyncio.Event此处显示的Future代替。不同之处在于可以重置事件，而将
    来不能从已解决过渡到待处理。↩

14. https://glyph.twistedmatrix.com/2014/02/unyielding.html ↩

Google 翻译

原文

提供更好的翻译建议
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
