编程沉思录

  • 首页
  • Golang源码剖析
  • 分类10
  • 归档27
  • 标签26
  • 关于

  • 文章目录
  • 站点概览

 1. eventloop 的创建
 2. 网络 IO 事件
 3. 定时器
     1. 异常处理
 4. 事件的等待和处理
 5. 操作系统的适配
 6. 总结
 7. 参考

cyhone

cyhone

我们采集的只是石头，却必须始终展望着未来的大教堂
    
Creative Commons
cyhone wechat
扫码关注公众号【编程沉思录】
第一时间收到文章推送
0%
 
Theme NexT works best with JavaScript enabled

Redis 事件循环器 (AE) 实现剖析

发表于 2019-11-20 更新于 2020-08-20 分类于 Redis 阅读次数：

Redis 作为一个单线程高性能的内存缓存 Server 而被人熟知。作为一个典型的 Reactor 式网络应用，Redis
能够达到如此高的性能，必然要依靠足够可靠的事件循环库。
Redis 内置了一个高性能事件循环器，叫做 AE。其定义和实现可以在 ae*.h/cpp 这些文件中找到。

AE 本身就是 Redis 的一部分，所以整体设计原则就是够用就行。也正因为这个背景，AE 的代码才可以简短
干净，非常适合阅读和学习。

本文将基于 Redis 5.0.6 的源码分析下其事件循环器 (AE) 的实现原理。

同时本人也提供了一个 Redis 注释版，用以辅助理解 Redis 的源码。

 eventloop 的创建

Redis 通过以下接口进行 eventloop 的创建和释放。

aeEventLoop *aeCreateEventLoop(int setsize);
void aeDeleteEventLoop(aeEventLoop *eventLoop);

Redis 通过将对应事件注册到 eventloop 中，然后不断循环检测有无事件触发。目前 eventloop 支持超时事
件和网络 IO 读写事件的注册。

我们可以通过 aeCreateEventLoop 来创建一个 eventloop。可以看到在创建 EventLoop 的时候，必须指定一
个 setsize 的参数。

setsize 参数表示了 eventloop 可以监听的网络事件 fd 的个数（不包含超时事件），如果当前监听的 fd
个数超过了 setsize，eventloop 将不能继续注册。

我们知道，Linux 内核会给每个进程维护一个文件描述符表。而 POSIX 标准对于文件描述符进行了以下约束
：

 1. fd 为 0、1、2 分别表示标准输入、标准输出和错误输出
 2. 每次新打开的 fd，必须使用当前进程中最小可用的文件描述符。

Redis 充分利用了文件描述符的这些特点，来存储每个 fd 对应的事件。

在 Redis 的 eventloop 中，直接用了一个连续数组来存储事件信息:

eventLoop->events = zmalloc(sizeof(aeFileEvent)*setsize);
for (i = 0; i < setsize; i++)
    eventLoop->events[i].mask = AE_NONE;

可以看到数组长度就是 setsize，同时创建之后将每一个 event 的 mask 属性置为 AE_NONE(即是 0)，mask
代表该 fd 注册了哪些事件。

对于 eventLoop->events 数组来说，fd 就是这个数组的下标。
例如，当程序刚刚启动时候，创建监听套接字，按照标准规定，该 fd 的值为 3。此时就直接在 eventLoop->
events 下标为 3 的元素中存放相应 event 数据。

不过也基于文件描述符的这些特点，意味着 events 数组的前三位一定不会有相应的 fd 赋值。

那么，Redis 是如何指定 eventloop 的 setsize 的呢？以下是 Redis 创建 eventloop 的相关代码：

server.el = aeCreateEventLoop(server.maxclients+CONFIG_FDSET_INCR);

其中：

 1. maxclients 代表用户配置的最大连接数，可在启动时由 --maxclients 指定，默认为 10000。
 2. CONFIG_FDSET_INCR 大小为 128。给 Redis 预留一些安全空间。

也正是因为 Redis 利用了 fd 的这个特点，Redis 只能在完全符合 POSIX 标准的系统中工作。其他的例如
Windows 系统，生成的 fd 或者说 HANDLE 更像是个指针，并不符合 POSIX 标准。

 网络 IO 事件

Redis 通过以下接口进行网络 IO 事件的注册和删除。

int aeCreateFileEvent(aeEventLoop *eventLoop, int fd, int mask,
        aeFileProc *proc, void *clientData);

void aeDeleteFileEvent(aeEventLoop *eventLoop, int fd, int mask);

aeCreateFileEvent 表示将某个 fd 的某些事件注册到 eventloop 中。

目前可注册的事件有三种:

 1. AE_READABLE 可读事件
 2. AE_WRITABLE 可写事件
 3. AE_BARRIER 该事件稍后在 “事件的等待和处理“ 一节详细讲到。

而 mask 就是这几个事件经过或运算后的掩码。

aeCreateFileEvent 在 epoll 的实现中调用了 epoll_ctl 函数。Redis 会根据该事件对应之前的 mask 是否
为 AE_NONE，来决定使用 EPOLL_CTL_ADD 还是 EPOLL_CTL_MOD。

同样的，aeDeleteFileEvent 也使用了 epoll_ctl，Redis 判断用户是否是要完全删除该 fd 上所有事件，来
决定使用 EPOLL_CTL_DEL 还是 EPOLL_CTL_MOD。

 定时器

AE 中最不值得分析的大概就是定时器了。。因为实现的实在是太简单了，甚至可以说是简陋。

Redis 通过以下两个接口进行定时器的注册和取消。

long long aeCreateTimeEvent(aeEventLoop *eventLoop, long long milliseconds,
        aeTimeProc *proc, void *clientData,
        aeEventFinalizerProc *finalizerProc);
int aeDeleteTimeEvent(aeEventLoop *eventLoop, long long id);

在调用 aeCreateTimeEvent 注册超时事件的时候，调用方需要提供两个 callback: aeTimeProc 和
aeEventFinalizerProc。

  • aeTimeProc 很简单，就是当超时事件触发时调用的 callback。有点特殊的是，aeTimeProc 需要返回一
    个 int 值，代表下次该超时事件触发的时间间隔。如果返回 - 1，则说明超时时间不需要再触发了，标
    记为删除即可。
  • finalizerProc 当 timer 被删除的时候，会调用这个 callback

Redis 的定时器其实做的非常简陋，只是一个普通的双向链表，链表也并不是有序的。每次最新的超时事件，
直接插入链表的最头部。
当 AE 要遍历当前时刻的超时事件时，也是直接暴力的从头到尾遍历链表，看看有没有超时的事件。

当时我看到这里源码的时候，还是很震惊的。因为一般来说，定时器都会采用最小堆或者时间轮等有序数据结
构进行存储，
为什么 Redis 的定时器做的这么简陋？

《Redis 的设计与实现》一书中说，在 Redis 3.0 版本中，只使用到了 serverCon 这一个超时事件。
所以这种情况下，也无所谓性能了，虽然是个链表，但其实用起来就只有一个元素，相当于当做一个指针在用
。

虽然还不清楚 5.0.6 版本里面超时事件有没有增多，不过可以肯定的是，目前依然达不到花点时间去优化的
程度。
Redis 在注释里面也说明了这事，并且给出了以后的优化方案：
用 skiplist 代替现有普通链表，查询的时间复杂度将优化为 O(1), 插入的时间复杂度将变成 O(log(N))

 异常处理

虽然定时器做的这么简陋，但是对于一些时间上的异常情况，Redis 还是做了下基本的处理。具体可见如下代
码：

if (now < eventLoop->lastTime) {
        te = eventLoop->timeEventHead;
        while(te) {
                te->when_sec = 0;
                te = te->next;
        }
}

这段代码的意思是，如果当前时刻小于 lastTime, 那意味着时间有可能被调整了。

对于这种情况，Redis 是怎么处理的呢：
直接把所有的事件的超时时间都置为 0, te->when_sec = 0。这样的话，接下来检查有哪些超时时间到期的时
候，所有的超时事件都会被判定为到期。相当于本次遍历把所有超时事件一次性全部激活。

因为 Redis 认为，在这种异常情况下，与其冒着超时事件可能永远无法触发的风险，还不如把事情提前做了
。

还是基于 Redis 够用就行的原则，这个解决方案在 Redis 中显然是被接受的。

但是其实还有更好的做法，比如 libevent 就是通过相对时间的方式进行处理这个问题。为了解决这个几乎不
会出现的异常 case，libevent 也花了大量代码进行处理。

 事件的等待和处理

Redis 中关于处理等待事件的函数有以下两个：

int aeProcessEvents(aeEventLoop *eventLoop, int flags);
void aeMain(aeEventLoop *eventLoop);

aeMain 的实现很简单, 就是我们所说的事件循环了，真的就是个循环：

void aeMain(aeEventLoop *eventLoop) {
    eventLoop->stop = 0;
    while (!eventLoop->stop) {
        if (eventLoop->beforesleep != NULL)
            eventLoop->beforesleep(eventLoop);
        aeProcessEvents(eventLoop, AE_ALL_EVENTS|AE_CALL_AFTER_SLEEP);
    }
}

而 aeProcessEvents 代表处理一次事件循环，那么 aeProcessEvents 都做了那些事情呢？

 1. 取出最近的一次超时事件。
 2. 计算该超时事件还有多久才可以触发。
 3. 等待网络事件触发或者超时。
 4. 处理触发的各个事件，包括网络事件和超时事件

为什么要取出最近的一次超时事件？这是因为对于 epoll_wait 来说，必须要指定一个超时时间。以下是
epoll_wait 的定义：

int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);

timeout 参数单位是毫秒，如果指定了大于 0 的超时时间，则在这段时间内即使如果没有网络 IO 事件触发
，epoll_wait 到了指定时间也会返回。
如果超时时间指定为 - 1，则 epoll_wait 将会一直阻塞等待，直到网络事件触发。

epoll_wait 的超时时间一定要指定为最近超时事件的时间间隔，这样可以防止万一这段时间没有网络事件触
发，超时事件也可以正常的响应。

同时，eventloop 还有两个 callback: beforesleep 和 aftersleep，分别会在 epoll_wait 之前和之后调用
。

接着，我们看下 Redis 是怎么处理已触发的网络事件的：
一般情况下，Redis 会先处理读事件 (AE_READABLE)，再处理写事件 (AE_WRITABLE)。
这个顺序安排其实也算是一点小优化，先读后写可以让一个请求的处理和回包都是在同一次循环里面，使得请
求可以尽快地回包，

前面讲到，网络 IO 事件注册的时候，除了正常的读写事件外，还可以注册一个 AE_BARRIER 事件，这个事件
就是会影响到先读后写的处理顺序。
如果某个 fd 的 mask 包含了 AE_BARRIER，那它的处理顺序会是先写后读。

针对这个场景，redis 举的例子是，如果在 beforesleep 回调中进行了 fsync 动作，然后需要把结果快速回
复给 client。这个情况下就需要用到 AE_BARRIER 事件，用来翻转处理事件顺序了。

 操作系统的适配

Redis 不仅支持 Linux 下的 epoll，还支持其他的 IO 复用方式，目前支持如下四种：

 1. epoll：支持 Linux 系统
 2. kqueue：支持 FreeBSD 系统 (如 macOS)
 3. select
 4. evport: 支持 Solaris

几个 IO 复用方式使用的判断顺序如下:

#ifdef HAVE_EVPORT
#include "ae_evport.c"
#else
    #ifdef HAVE_EPOLL
    #include "ae_epoll.c"
    #else
        #ifdef HAVE_KQUEUE
        #include "ae_kqueue.c"
        #else
        #include "ae_select.c"
        #endif
    #endif
#endif

这个顺序其实也代表了四种 IO 复用方式的性能高低。

对于每种 IO 复用方式，只要实现以下 8 个接口就可以正常对接 Redis 了：

int aeApiCreate(aeEventLoop *eventLoop);
void aeApiDelEvent(aeEventLoop *eventLoop, int fd, int delmask);
void aeApiResize(aeEventLoop *eventLoop, int setsize);
void aeApiFree(aeEventLoop *eventLoop);
int aeApiAddEvent(aeEventLoop *eventLoop, int fd, int mask);
void aeApiDelEvent(aeEventLoop *eventLoop, int fd, int delmask);
int aeApiPoll(aeEventLoop *eventLoop, struct timeval *tvp);
char *aeApiName(void);

在这个 8 个接口下面，其实底层并没有做太多的优化，只是简单的对原有 API 封装而已。

 总结

与其他通用型的事件循环库 (如 libevent) 不一样的是，Redis 的事件循环库不用考虑太多的用户侧因素：

 1. 不用考虑 ABI 兼容。因为 AE 本身就和 Redis 一起编译，所以无需像 libevent 一样考虑库的升级问题
    。
 2. 不支持 Windows 系统，只支持 unix like 的系统
 3. 指定了监听 fd 的个数的上限，默认支持 10000 个客户端连接。

 参考

  • 《Redis 的设计与实现》

如果你在阅读过程中发现本文有错误或者存疑之处，请在下方评论区或者通过公众号进行留言，作者会尽快回
复和解答。感谢您的支持与帮助。
cyhone wechat

  • 本文作者： cyhone
  • 本文链接： https://www.cyhone.com/articles/analysis-of-redis-ae/
  • 版权声明：本博客所有文章除特别声明外，均采用 BY-NC-SA 许可协议。转载请注明出处！

源码 Redis 网络编程 TCP Reactor
Elastic-Filebeat 实现原理剖析
FileBeat-Log 相关配置指南
豫ICP备18011028号
© 2015 – 2021 cyhone
由 Hexo & NexT.Gemini 强力驱动
