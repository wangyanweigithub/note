## Tornado Web应用程序的结构
1. 通常一个Tornado Web应用包括一个或者多个 RequestHandler子类,一个可以将收到的请求路由到对应的handler的Application对象,和一个启动服务的main()函数.

2. 一个最小的“hello world”示例如下所示：
```
import tornado.ioloop
import tornado.web

class MainHandler(tornado.web.RequestHandler):
    def get(self):
        self.write("Hello, world")

def make_app():
    return tornado.web.Application([
        (r"/", MainHandler),
    ])

if __name__ == "__main__":
    app = make_app()
    app.listen(8888)
    tornado.ioloop.IOLoop.current().start()
```

## Application 对象
1. 这个 Application 对象负责全局配置，包括将请求映射到处理程序的路由表。

2. 路由表是 URLSpec 对象（或元组），每个对象至少包含一个正则表达式和一个处理程序类。顺序很重要；使用第一个匹配规则。如果正则表
达式包含捕获组，则这些组是路径参数并将传递给处理程序的HTTP方法。如果将字典作为 URLSpec ，它提供初始化参数将传递给
RequestHandler.initialize . 最后， URLSpec 可能有一个名称，允许它与 RequestHandler.reverse_url .

3. 例如，在这个片段中，根URL / 映射到 MainHandler 和表单的URL /story/ 后跟一个数字映射到 StoryHandler . 这个数字（作为字符串）
传递给 StoryHandler.get .
```
class MainHandler(RequestHandler):
    def get(self):
        self.write('<a href="%s">link to story 1</a>' %
                   self.reverse_url("story", "1"))

class StoryHandler(RequestHandler):
    def initialize(self, db):
        self.db = db

    def get(self, story_id):
        self.write("this is story %s" % story_id)

app = Application([
    url(r"/", MainHandler),
    url(r"/story/([0-9]+)", StoryHandler, dict(db=db), name="story")
    ])
```

这个 Application 构造函数采用许多关键字参数，这些参数可用于自定义应用程序的行为并启用可选功能；请参见 Application.settings
完整的列表。

### 子类别化 RequestHandler¶
1. Tornado Web应用程序的大部分工作都是在 RequestHandler . 处理程序子类的主要入口点是以正在处理的HTTP方法命名的方法： get() ， 
post() 等。每个处理程序可以定义这些方法中的一个或多个来处理不同的HTTP操作。如上所述，将使用与匹配的路由规则的捕获组相对应的
参数调用这些方法。

2. 在处理程序中，调用诸如 RequestHandler.render 或 RequestHandler.write 做出回应。 
	- render() 加载A Template 并用给定的参数呈现
	- write() 用于非基于模板的输出；它接受字符串、字节和字典（dict将被编码为json）。

3. 许多方法 RequestHandler 在子类中被重写，并在整个应用程序中使用。定义一个 BaseHandler 重写方法的类，例如 write_error 和
get_current_user 然后把你自己的 BaseHandler 而不是 RequestHandler 对于所有特定的处理程序。

处理请求输入¶

请求处理程序可以访问表示当前请求的对象 self.request . 参见类定义 HTTPServerRequest 获取完整的属性列表。

HTML表单使用的格式中的请求数据将为您进行分析，并在以下方法中可用 get_query_argument 和 get_body_argument .

class MyFormHandler(tornado.web.RequestHandler):
    def get(self):
        self.write('<html><body><form action="/myform" method="POST">'
                   '<input type="text" name="message">'
                   '<input type="submit" value="Submit">'
                   '</form></body></html>')

    def post(self):
        self.set_header("Content-Type", "text/plain")
        self.write("You wrote " + self.get_body_argument("message"))

由于HTML表单编码对于参数是单个值还是包含一个元素的列表不明确， RequestHandler 具有不同的方法以允许应用程序指示是否需要列表
。对于列表，使用 get_query_arguments 和 get_body_arguments 而不是它们的单一对应物。

通过表单上载的文件可在 self.request.files ，它映射名称（HTML的名称 <input type="file"> 元素）到文件列表。每个文件都是表单的
字典 {{"filename":..., "content_type":..., "body":...}} . 这个 files 只有使用表单包装器（即 multipart/form-data 内容类型）
；如果未使用此格式，则可以在 self.request.body . 默认情况下，上载的文件在内存中是完全缓冲的；如果您需要处理过大的文件而无法
在内存中舒适地保存，请参阅 stream_request_body 类修饰器。

在演示目录中， file_receiver.py 显示两种接收文件上载的方法。

由于HTML表单编码的特殊性（例如，单参数和复数参数之间的模糊性），Tornado不尝试将表单参数与其他类型的输入统一起来。特别是，我
们不解析JSON请求体。希望使用JSON而不是表单编码的应用程序可能会重写 prepare 分析他们的请求：

def prepare(self):
    if self.request.headers.get("Content-Type", "").startswith("application/json"):
        self.json_args = json.loads(self.request.body)
    else:
        self.json_args = None

重写RequestHandler方法¶

除了 get()/post() /等，某些其他方法 RequestHandler 在必要时被子类覆盖。对于每个请求，都会发生以下调用序列：

 1. 一个新的 RequestHandler 对象是根据每个请求创建的。

 2. initialize() 使用来自的初始化参数调用 Application 配置。 initialize 通常只应保存传递给成员变量的参数；它可能不会生成任
    何输出或调用方法，如 send_error .

 3. prepare() 被称为。这在所有处理程序子类共享的基类中最有用，如 prepare 无论使用哪种HTTP方法都会调用。 prepare 可能产生输
    出；如果它调用 finish （或） redirect 等），此处停止处理。

 4. 其中一个HTTP方法称为： get() ， post() ， put() 等等。如果URL正则表达式包含捕获组，则将它们作为参数传递给此方法。

 5. 请求完成后， on_finish() 被称为。这通常是在 get() 或者返回另一个HTTP方法。

设计为重写的所有方法在 RequestHandler 文档。一些最常见的重写方法包括：

  • write_error -输出用于错误页的HTML。

  • on_connection_close -当客户端断开连接时调用；应用程序可以选择检测此情况并停止进一步处理。请注意，不能保证可以及时检测到
    闭合连接。

  • get_current_user -见用户身份验证 .

  • get_user_locale -收益 Locale 对象以供当前用户使用。

  • set_default_headers -可用于在响应上设置其他头（例如自定义 Server 标题）。

错误处理¶

如果处理程序引发异常，Tornado将调用 RequestHandler.write_error 生成错误页。 tornado.web.HTTPError 可用于生成指定的状态代码
；所有其他异常都返回500状态。

默认错误页包括调试模式下的堆栈跟踪和错误的一行描述（例如“500:内部服务器错误”），否则。要生成自定义错误页，请重写
RequestHandler.write_error （可能在所有处理程序共享的基类中）。这种方法可以通过如下方法正常产生输出 write 和 render . 如果
错误是由异常引起的，则 exc_info Triple将作为关键字参数传递（请注意，此异常不保证是中的当前异常 sys.exc_info 如此 
write_error 必须使用例如 traceback.format_exception 而不是 traceback.format_exc ）

还可以从常规处理程序方法生成错误页，而不是 write_error 通过呼叫 set_status ，写一个响应，然后返回。特殊例外
tornado.web.Finish 可以在不调用的情况下引发以终止处理程序 write_error 在简单返回不方便的情况下。

对于404个错误，请使用 default_handler_class Application setting . 此处理程序应重写 prepare 而不是像 get() 所以它适用于任何
HTTP方法。它应该产生上面描述的错误页：或者通过 HTTPError(404) 压倒一切 write_error 或呼叫 self.set_status(404) 直接产生反应
prepare() .

重定向¶

在Tornado中，有两种主要方法可以重定向请求： RequestHandler.redirect 并与 RedirectHandler .

你可以使用 self.redirect() 在一个 RequestHandler 方法将用户重定向到其他位置。还有一个可选参数 permanent 它可以用来表示重定
向被认为是永久的。默认值为 permanent 是 False ，生成一个 302 Found HTTP响应代码，适用于成功后重定向用户 POST 请求。如果 
permanent 是 True , the 301 Moved Permanently 使用HTTP响应代码，这对于以SEO友好的方式重定向到页面的规范URL非常有用。

RedirectHandler 允许您直接在 Application 路由表。例如，要配置单个静态重定向，请执行以下操作：

app = tornado.web.Application([
    url(r"/app", tornado.web.RedirectHandler,
        dict(url="http://itunes.apple.com/my-app-id")),
    ])

RedirectHandler 还支持正则表达式替换。以下规则重定向所有以开头的请求 /pictures/ 前缀 /photos/ 取而代之的是：

app = tornado.web.Application([
    url(r"/photos/(.*)", MyPhotoHandler),
    url(r"/pictures/(.*)", tornado.web.RedirectHandler,
        dict(url=r"/photos/{0}")),
    ])

不像 RequestHandler.redirect ， RedirectHandler 默认情况下使用永久重定向。这是因为路由表在运行时不会更改，并且假定是永久的
，而在处理程序中找到的重定向可能是其他可能更改的逻辑的结果。使用 RedirectHandler ，添加 permanent=False 到 RedirectHandler
初始化参数。

异步处理程序¶

某些处理方法（包括 prepare() and the HTTP verb methods get()/post() /etc）可以重写为协程以使处理程序异步。

例如，这里有一个使用协程的简单处理程序：

class MainHandler(tornado.web.RequestHandler):
    async def get(self):
        http = tornado.httpclient.AsyncHTTPClient()
        response = await http.fetch("http://friendfeed-api.com/v2/feed/bret")
        json = tornado.escape.json_decode(response.body)
        self.write("Fetched " + str(len(json["entries"])) + " entries "
                   "from the FriendFeed API")

有关更高级的异步示例，请查看 chat example application ，它使用 long polling . 长轮询的用户可能希望覆盖 on_connection_close
() 在客户端关闭连接后进行清理（但请参阅该方法的docstring以了解警告）。
