 

一个简单的Web服务器

格雷格·威尔逊

格雷格·威尔逊（Greg Wilson）是Software Carpentry的创始人，这是一门面向科学家和工程师的计算技能
速成课程。他已经在行业和学术界工作了30年，并且是多本关于计算的书籍的作者或编辑，包括2008年Jolt
奖获得者Beautiful Code和《开源应用程序体系结构》的前两卷。Greg于1993年获得爱丁堡大学计算机科学
博士学位。

介绍

在过去的二十年中，网络以无数种方式改变了社会，但其核心变化却很小。大多数系统仍然遵循Tim
Berners-Lee在25年前制定的规则。特别是，大多数Web服务器仍以相同的方式处理当时处理的相同类型的消
息。

本章将探讨他们如何做到这一点。同时，它将探索开发人员如何创建不需要重写即可添加新功能的软件系统
。

背景

Web上几乎每个程序都在称为Internet协议（IP）的一系列通信标准上运行。与我们有关的该家族的成员是
传输控制协议（TCP / IP），它使计算机之间的通信看起来像读写文件。

使用IP的程序通过套接字进行通信。每个插座是点对点通信通道的一端，就像电话是电话呼叫的一端一样。
套接字由标识特定计算机的IP地址和该计算机上的端口号组成。IP地址由四个8位数字组成，例如
174.136.14.108；域名系统（DNS）会将这些数字与符号名称进行匹配，例如aosabook.org，人类更容易记
住。

端口号是介于0到65535之间的数字，可以唯一地标识主机上的套接字。（如果IP地址就像公司的电话号码，
那么端口号就像分机号。）端口0-1023保留供操作系统使用；其他任何人都可以使用其余的端口。

超文本传输​​协议（HTTP）描述了程序可以通过IP交换数据的一种方式。HTTP刻意简单：客户端通过套接字
连接发送请求，指定请求的内容，服务器作为响应发送一些数据（图22.1）。数据可以从磁盘上的文件复制
，由程序动态生成，或者两者的某种混合。

图22.1-HTTP周期

图22.1-HTTP周期

HTTP请求最重要的是它只是文本：任何想要创建一个或解析一个的程序。为了理解，该文本必须具有图22.2
所示的部分。

图22.2-HTTP请求

图22.2-HTTP请求

HTTP方法几乎总是“ GET”（获取信息）或“ POST”（提交表单数据或上传文件）。URL指定客户想要的内容。
它通常是磁盘上文件的路径，例如/research/experiments.html，但是（这是关键部分）完全取决于服务器
来决定如何处理它。HTTP版本通常是“ HTTP / 1.0”或“ HTTP / 1.1”；两者之间的差异对我们来说无关紧要
。

HTTP标头是键/值对，类似于下面显示的三个：

Accept: text/html
Accept-Language: en, fr
If-Modified-Since: 16-May-2005

与哈希表中的键不同，键可以在HTTP标头中出现任意次。这使请求可以执行诸如指定愿意接受多种类型的内
容之类的操作。

最后，请求的主体是与该请求关联的任何其他数据。在通过Web表单提交数据，上传文件等时，将使用此功
能。最后一个标头和正文的开头之间必须有一个空白行，以指示标头的结尾。

一个名为的标头Content-Length告诉服务器在请求正文中期望读取多少字节。

HTTP响应的格式类似于HTTP请求（图22.3）：

图22.3-HTTP响应

图22.3-HTTP响应

版本，标题和正文具有相同的形式和含义。状态代码是一个数字，指示处理请求时发生了什么：200表示“一
切正常”，404表示“未找到”，其他代码具有其他含义。状态短语以人类可读的短语（例如“确定”或“未找到”
）重复该信息。

就本章而言，关于HTTP仅需要了解另外两件事。

首先是它是无状态的：每个请求都是独立处理的，服务器在一个请求和下一个请求之间不记得任何内容。如
果应用程序想要跟踪用户身份之类的信息，它必须自己进行跟踪。

通常的做法是使用cookie，它是服务器发送给客户端的短字符串，客户端随后又返回到服务器。当用户执行
某些功能时，需要在多个请求中保存状态，服务器会创建一个新的cookie，将其存储在数据库中，然后将其
发送到她的浏览器。每次浏览器将cookie发送回去时，服务器都会使用它来查找有关用户正在执行的操作的
信息。

我们需要了解的关于HTTP的第二件事是URL可以添加参数以提供更多信息。例如，如果我们使用搜索引擎，
则必须指定我们的搜索词。我们可以将它们添加到URL的路径中，但是我们应该做的是向URL添加参数。我们
通过添加“？”来实现网址，后跟由“＆”分隔的“键=值”对。例如，URL http://www.google.ca?q=Python要求
Google搜索与Python相关的页面：键是字母“ q”，值是“ Python”。较长的查询http://www.google.ca/
search?q=Python&amp;client=Firefox告诉Google我们正在使用Firefox，依此类推。我们可以传递想要的
任何参数，但是同样，要由网站上运行的应用程序决定要注意的参数以及如何解释它们。

当然，如果 '？' 和'＆'是特殊字符，必须有一种将其转义的方法，就像必须有一种方法将双引号字符放在
由双引号分隔的字符串中一样。URL编码标准使用'％'表示特殊字符，后跟两位数字代码，并用'+'字符替换
空格。因此，要在Google上搜索“成绩= A +”（带有空格），可以使用URL http://www.google.ca/search?q
=grade+%3D+A%2B。

打开套接字，构造HTTP请求以及解析响应是很乏味的，因此大多数人使用库来完成大部分工作。Python附带
了一个称为的库urllib2（因为它是一个较早的库的替代品urllib），但是它却暴露了许多人根本不想关心
的管道。该请求库是一个更容易使用的替代品urllib2。这是一个使用它从AOSA图书网站下载页面的示例：

import requests
response = requests.get('http://aosabook.org/en/500L/web-server/testpage.html')
print 'status code:', response.status_code
print 'content length:', response.headers['content-length']
print response.text

status code: 200
content length: 61
<html>
  <body>
    <p>Test page.</p>
  </body>
</html>

request.get向服务器发送HTTP GET请求，并返回包含响应的对象。该对象的status_code成员是响应的状态
码；它的content_length成员是响应数据中的字节数，并且text是实际数据（在这种情况下为HTML页面）。

您好，网站

现在，我们准备编写第一个简单的Web服务器。基本思想很简单：

 1. 等待有人连接到我们的服务器并发送HTTP请求；
 2. 解析该请求；
 3. 弄清楚要什么；
 4. 获取该数据（或动态生成）；
 5. 将数据格式化为HTML；和
 6. 把它退回。

从一个应用程序到另一个应用程序的步骤1、2和6是相同的，因此Python标准库有一个名为的模块
BaseHTTPServer，可以为我们完成这些步骤。我们只需要注意以下步骤3-5中的步骤即可：

import BaseHTTPServer

class RequestHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    '''Handle HTTP requests by returning a fixed 'page'.'''

    # Page to send back.
    Page = '''\
<html>
<body>
<p>Hello, web!</p>
</body>
</html>
'''

    # Handle a GET request.
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-Type", "text/html")
        self.send_header("Content-Length", str(len(self.Page)))
        self.end_headers()
        self.wfile.write(self.Page)

#----------------------------------------------------------------------

if __name__ == '__main__':
    serverAddress = ('', 8080)
    server = BaseHTTPServer.HTTPServer(serverAddress, RequestHandler)
    server.serve_forever()

该库的BaseHTTPRequestHandler类负责解析传入的HTTP请求并确定其包含的方法。如果方法是GET，则该类
将调用名为的方法do_GET。我们的类RequestHandler重写此方法以动态生成一个简单的页面：文本存储在类
级别的变量中Page，我们在发送200响应代码后将其发送回客户端，Content-Type标头告诉客户端将我们的
数据解释为HTML，并且页面的长度。（end_headers方法调用将插入空白行，该行将我们的标题与页面本身
分开。）

但这RequestHandler还不是全部：我们仍然需要最后三行才能真正启动服务器运行。这些行的第一行将服务
器的地址定义为元组：空字符串表示“在当前计算机上运行”，而8080是端口。然后，我们
BaseHTTPServer.HTTPServer使用该地址和请求处理程序类的名称作为参数创建一个实例，然后要求它永久
运行（实际上，这意味着直到我们用Control-C杀死它为止）。

如果我们从命令行运行该程序，它将不会显示任何内容：

$ python server.py

但是，如果我们随后http://localhost:8080使用浏览器进入，则会在浏览器中获得此信息：

Hello, web!

这在我们的外壳中：

127.0.0.1 - - [24/Feb/2014 10:26:28] "GET / HTTP/1.1" 200 -
127.0.0.1 - - [24/Feb/2014 10:26:28] "GET /favicon.ico HTTP/1.1" 200 -

第一行很简单：由于我们没有要求特定的文件，因此我们的浏览器要求输入'/'（服务器所服务的根目录）
。之所以出现第二行，是因为我们的浏览器会自动发送第二个请求，请求一个名为的图像文件/favicon.ico
，如果存在，它将在地址栏中显示为图标。

显示值

让我们修改我们的Web服务器，以显示HTTP请求中包含的某些值。（我们在调试时会非常频繁地执行此操作
，因此我们不妨进行一些练习。）为了保持代码的清洁，我们将创建页面与发送页面分开：

class RequestHandler(BaseHTTPServer.BaseHTTPRequestHandler):

    # ...page template...

    def do_GET(self):
        page = self.create_page()
        self.send_page(page)

    def create_page(self):
        # ...fill in...

    def send_page(self, page):
        # ...fill in...

send_page 差不多是我们以前的样子：

    def send_page(self, page):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.send_header("Content-Length", str(len(page)))
        self.end_headers()
        self.wfile.write(page)

我们要显示的页面模板只是一个字符串，其中包含带有某些格式占位符的HTML表：

    Page = '''\
<html>
<body>
<table>
<tr>  <td>Header</td>         <td>Value</td>          </tr>
<tr>  <td>Date and time</td>  <td>{date_time}</td>    </tr>
<tr>  <td>Client host</td>    <td>{client_host}</td>  </tr>
<tr>  <td>Client port</td>    <td>{client_port}s</td> </tr>
<tr>  <td>Command</td>        <td>{command}</td>      </tr>
<tr>  <td>Path</td>           <td>{path}</td>         </tr>
</table>
</body>
</html>
'''

填充此方法的方法是：

    def create_page(self):
        values = {
            'date_time'   : self.date_time_string(),
            'client_host' : self.client_address[0],
            'client_port' : self.client_address[1],
            'command'     : self.command,
            'path'        : self.path
        }
        page = self.Page.format(**values)
        return page

程序的主体没有改变：和以前一样，它创建HTTPServer带有地址的类的实例，并以该请求处理程序作为参数
，然后永远为请求提供服务。如果运行它并从浏览器发送对的请求，则会http://localhost:8080/
something.html得到：

  Date and time  Mon, 24 Feb 2014 17:17:12 GMT
  Client host    127.0.0.1
  Client port    54548
  Command        GET
  Path           /something.html

请注意，即使页面不作为磁盘上的文件存在，我们也不会收到404错误something.html。这是因为Web服务器
只是一个程序，并且在收到请求时可以做任何想做的事情：发回上一个请求中命名的文件，提供随机选择的
Wikipedia页面或进行其他编程。

服务静态页面

显而易见的下一步是从磁盘开始提供页面，而不是即时生成页面。我们将从重写开始do_GET：

    def do_GET(self):
        try:

            # Figure out what exactly is being requested.
            full_path = os.getcwd() + self.path

            # It doesn't exist...
            if not os.path.exists(full_path):
                raise ServerException("'{0}' not found".format(self.path))

            # ...it's a file...
            elif os.path.isfile(full_path):
                self.handle_file(full_path)

            # ...it's something we don't handle.
            else:
                raise ServerException("Unknown object '{0}'".format(self.path))

        # Handle errors.
        except Exception as msg:
            self.handle_error(msg)

此方法假定允许在Web服务器正在运行的目录（使用的目录）中或目录下方提供任何文件os.getcwd。它将它
与URL中提供的路径（库自动放入该路径self.path，并始终以前导“ /”开头）相结合，以获取用户所需文件
的路径。

如果不存在，或者它不是文件，则该方法通过引发并捕获异常来报告错误。另一方面，如果路径与文件匹配
，则它将调用名为的帮助程序方法以handle_file读取并返回内容。此方法仅读取文件，并使用我们现有的
文件send_content将其发送回客户端：

    def handle_file(self, full_path):
        try:
            with open(full_path, 'rb') as reader:
                content = reader.read()
            self.send_content(content)
        except IOError as msg:
            msg = "'{0}' cannot be read: {1}".format(self.path, msg)
            self.handle_error(msg)

请注意，我们以二进制模式（“ rb”中的“ b”）打开文件，以便Python不会尝试通过更改看起来像Windows行
尾的字节序列来“帮助”我们。还要注意，在服务时将整个文件读入内存在现实生活中是个坏主意，因为文件
可能是几GB的视频数据。处理这种情况不在本章范围之内。

要结束此类，我们需要编写错误处理方法和错误报告页面的模板：

    Error_Page = """\
        <html>
        <body>
        <h1>Error accessing {path}</h1>
        <p>{msg}</p>
        </body>
        </html>
        """

    def handle_error(self, msg):
        content = self.Error_Page.format(path=self.path, msg=msg)
        self.send_content(content)

该程序有效，但前提是我们看起来不太紧密。问题在于，即使所请求的页面不存在，它也始终返回200的状
态码。是的，在这种情况下发送回的页面包含错误消息，但是由于我们的浏览器无法读取英文，因此它不知
道请求实际上失败了。为了说清楚，我们需要修改handle_error和send_content如下：

    # Handle unknown objects.
    def handle_error(self, msg):
        content = self.Error_Page.format(path=self.path, msg=msg)
        self.send_content(content, 404)

    # Send actual content.
    def send_content(self, content, status=200):
        self.send_response(status)
        self.send_header("Content-type", "text/html")
        self.send_header("Content-Length", str(len(content)))
        self.end_headers()
        self.wfile.write(content)

请注意，ServerException当找不到文件时，我们不会引发错误，而是会生成错误页面。A ServerException
的意思是表示服务器代码中发生内部错误，即我们出错了。handle_error另一方面，当用户遇到问题时（即
向我们发送了不存在的文件的URL），将显示由创建的错误页面。^1个

上市目录

下一步，我们可以教Web服务器在URL中的路径是目录而不是文件时显示目录内容的列表。我们甚至可以更进
一步，让它在该目录中index.html查找要显示的文件，并且仅在该文件不存在的情况下显示目录内容的列表
。

但是do_GET将这些规则构建进去将是一个错误，因为生成的方法将是一长串if控制特殊行为的语句。正确的
解决方案是退后一步并解决一般问题，即弄清楚如何处理URL。这是do_GET方法的重写：

    def do_GET(self):
        try:

            # Figure out what exactly is being requested.
            self.full_path = os.getcwd() + self.path

            # Figure out how to handle it.
            for case in self.Cases:
                handler = case()
                if handler.test(self):
                    handler.act(self)
                    break

        # Handle errors.
        except Exception as msg:
            self.handle_error(msg)

第一步是相同的：找出请求的事物的完整路径。但是，在此之后，代码看起来大不相同。该版本代替了一系
列内联测试，而是循环存储在列表中的一组案例。每个案例都是一个具有两种方法的对象：test，它告诉我
们是否能够处理该请求；以及act，它实际上会采取某些措施。一旦找到合适的情况，我们就让它处理请求
并跳出循环。

这三个案例类重现了先前服务器的行为：

class case_no_file(object):
    '''File or directory does not exist.'''

    def test(self, handler):
        return not os.path.exists(handler.full_path)

    def act(self, handler):
        raise ServerException("'{0}' not found".format(handler.path))


class case_existing_file(object):
    '''File exists.'''

    def test(self, handler):
        return os.path.isfile(handler.full_path)

    def act(self, handler):
        handler.handle_file(handler.full_path)


class case_always_fail(object):
    '''Base case if nothing else worked.'''

    def test(self, handler):
        return True

    def act(self, handler):
        raise ServerException("Unknown object '{0}'".format(handler.path))

这是我们在RequestHandler类顶部构造案例处理程序列表的方式：

class RequestHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    '''
    If the requested path maps to a file, that file is served.
    If anything goes wrong, an error page is constructed.
    '''

    Cases = [case_no_file(),
             case_existing_file(),
             case_always_fail()]

    ...everything else as before...

现在，从表面上看，这使我们的服务器变得更加复杂，而不是更少：文件已从74行增加到99行，并且存在一
个间接级别，没有任何新功能。当我们回到开始本章的任务并尝试教我们的服务器index.html为目录提供页
面时（如果有的话）为该页面提供服务，而如果没有目录的列表则为您带来好处。前者的处理程序是：

class case_directory_index_file(object):
    '''Serve index.html page for a directory.'''

    def index_path(self, handler):
        return os.path.join(handler.full_path, 'index.html')

    def test(self, handler):
        return os.path.isdir(handler.full_path) and \
               os.path.isfile(self.index_path(handler))

    def act(self, handler):
        handler.handle_file(self.index_path(handler))

在这里，helper方法index_path构造了index.html文件的路径。将其放入案件处理程序可以防止主体混乱
RequestHandler。test检查路径是否是包含index.html页面的目录，并act要求主请求处理程序提供该页面
。

唯一需要做的更改RequestHandler是将一个case_directory_index_file对象添加到我们的Cases列表中：

    Cases = [case_no_file(),
             case_existing_file(),
             case_directory_index_file(),
             case_always_fail()]

不包含index.html页面的目录呢？该测试与上面not策略插入的测试相同，但是该act方法呢？应该怎么办？

class case_directory_no_index_file(object):
    '''Serve listing for a directory without an index.html page.'''

    def index_path(self, handler):
        return os.path.join(handler.full_path, 'index.html')

    def test(self, handler):
        return os.path.isdir(handler.full_path) and \
               not os.path.isfile(self.index_path(handler))

    def act(self, handler):
        ???

看来我们已经陷入困境。从逻辑上讲，该act方法应该创建并返回目录列表，但是我们现有的代码不允许这
样做：RequestHandler.do_GETcalls act，但是不期望或处理它的返回值。现在，让我们添加一个方法来
RequestHandler生成目录列表，然后从case handler的中调用它act：

class case_directory_no_index_file(object):
    '''Serve listing for a directory without an index.html page.'''

    # ...index_path and test as above...

    def act(self, handler):
        handler.list_dir(handler.full_path)


class RequestHandler(BaseHTTPServer.BaseHTTPRequestHandler):

    # ...all the other code...

    # How to display a directory listing.
    Listing_Page = '''\
        <html>
        <body>
        <ul>
        {0}
        </ul>
        </body>
        </html>
        '''

    def list_dir(self, full_path):
        try:
            entries = os.listdir(full_path)
            bullets = ['<li>{0}</li>'.format(e)
                for e in entries if not e.startswith('.')]
            page = self.Listing_Page.format('\n'.join(bullets))
            self.send_content(page)
        except OSError as msg:
            msg = "'{0}' cannot be listed: {1}".format(self.path, msg)
            self.handle_error(msg)

CGI协议

当然，大多数人都不想编辑Web服务器的源代码以添加新功能。为了避免不必要的麻烦，服务器始终支持称
为通用网关接口（CGI）的机制，该机制为Web服务器提供了运行外部程序以满足请求的标准方法。

例如，假设我们希望服务器能够在HTML页面中显示本地时间。我们可以在一个只有几行代码的独立程序中完
成此操作：

from datetime import datetime
print '''\
<html>
<body>
<p>Generated {0}</p>
</body>
</html>'''.format(datetime.now())

为了使Web服务器为我们运行该程序，我们添加了此case处理器：

class case_cgi_file(object):
    '''Something runnable.'''

    def test(self, handler):
        return os.path.isfile(handler.full_path) and \
               handler.full_path.endswith('.py')

    def act(self, handler):
        handler.run_cgi(handler.full_path)

测试很简单：文件路径是否以结尾.py？如果是这样，请RequestHandler运行该程序。

    def run_cgi(self, full_path):
        cmd = "python " + full_path
        child_stdin, child_stdout = os.popen2(cmd)
        child_stdin.close()
        data = child_stdout.read()
        child_stdout.close()
        self.send_content(data)

这是非常不安全的：如果有人知道我们服务器上Python文件的路径，我们只是让他们运行它而不必担心它可
以访问哪些数据，它是否包含无限循环或其他任何东西。^2

撇开这一点，核心思想很简单：

 1. 在子进程中运行程序。
 2. 捕获子流程发送到标准输出的所有内容。
 3. 将其发送回发出请求的客户端。

完整的CGI协议要比这丰富得多（特别是它允许URL中的参数，服务器将这些参数传递给正在运行的程序），
但是这些细节不会影响系统的整体体系结构。

...再一次变得很纠结。RequestHandler最初有一种方法，handle_file用于处理内容。现在，我们以
list_dir和的形式添加了两种特殊情况run_cgi。这三种方法并不真正属于它们所在的位置，因为它们主要
由其他人使用。

解决方法很简单：为我们所有的案例处理程序创建一个父类，并在（且仅当）两个或多个处理程序共享其他
方法时，才将其他方法移至该类。完成后，RequestHandler该类如下所示：

class RequestHandler(BaseHTTPServer.BaseHTTPRequestHandler):

    Cases = [case_no_file(),
             case_cgi_file(),
             case_existing_file(),
             case_directory_index_file(),
             case_directory_no_index_file(),
             case_always_fail()]

    # How to display an error.
    Error_Page = """\
        <html>
        <body>
        <h1>Error accessing {path}</h1>
        <p>{msg}</p>
        </body>
        </html>
        """

    # Classify and handle request.
    def do_GET(self):
        try:

            # Figure out what exactly is being requested.
            self.full_path = os.getcwd() + self.path

            # Figure out how to handle it.
            for case in self.Cases:
                if case.test(self):
                    case.act(self)
                    break

        # Handle errors.
        except Exception as msg:
            self.handle_error(msg)

    # Handle unknown objects.
    def handle_error(self, msg):
        content = self.Error_Page.format(path=self.path, msg=msg)
        self.send_content(content, 404)

    # Send actual content.
    def send_content(self, content, status=200):
        self.send_response(status)
        self.send_header("Content-type", "text/html")
        self.send_header("Content-Length", str(len(content)))
        self.end_headers()
        self.wfile.write(content)

而我们的案例处理程序的父类是：

class base_case(object):
    '''Parent for case handlers.'''

    def handle_file(self, handler, full_path):
        try:
            with open(full_path, 'rb') as reader:
                content = reader.read()
            handler.send_content(content)
        except IOError as msg:
            msg = "'{0}' cannot be read: {1}".format(full_path, msg)
            handler.handle_error(msg)

    def index_path(self, handler):
        return os.path.join(handler.full_path, 'index.html')

    def test(self, handler):
        assert False, 'Not implemented.'

    def act(self, handler):
        assert False, 'Not implemented.'

现有文件的处理程序（只是随机选择一个示例）是：

class case_existing_file(base_case):
    '''File exists.'''

    def test(self, handler):
        return os.path.isfile(handler.full_path)

    def act(self, handler):
        self.handle_file(handler, handler.full_path)

讨论区

我们原始代码和重构版本之间的差异反映了两个重要的想法。首先是将类视为相关服务的集合。
RequestHandler并且base_case不做决定或采取行动；它们提供了其他类可以用来执行这些操作的工具。

第二个是可扩展性：人们可以通过编写外部CGI程序或添加案例处理程序类来向我们的Web服务器添加新功能
。后者确实需要单行更改RequestHandler（以将案例处理程序插入Cases列表中），但是我们可以通过让Web
服务器读取配置文件并从中加载处理程序类来摆脱这种情况。在这两种情况下，它们都可以忽略大多数底层
细节，就像BaseHTTPRequestHandler该类的作者允许我们忽略处理套接字连接和解析HTTP请求的细节一样。

这些想法通常是有用的。看看是否可以找到在自己的项目中使用它们的方法。

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

 1. handle_error在本章中，我们将多次使用，包括几种404不适合使用状态代码的情况。在继续阅读时，
    请尝试考虑如何扩展该程序，以便可以在每种情况下轻松提供状态响应代码。↩

 2. 我们的代码还使用了popen2库函数，为了支持该subprocess模块，该函数已被弃用。但是，popen2在此
    示例中使用的分心工具较少。↩

Google 翻译

原文

提供更好的翻译建议
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
