
<!-- vim-markdown-toc GFM -->

* [socket模型]
* [reactor]
* [proactor]
* [reactor和proactor的区别]
* [更好的proactor]
* [在服务器程序上使用]

<!-- vim-markdown-toc -->
### socket模型
1. 服务器程序，简单的说就是接收网络数据，处理后并返回结果数据。网络模块是其必不可少的部分，
它本质上就是处理socket的五类事件:
	- accept(客户端接入)
	- connect(连接上服务器)
	- read
	- write
	- error
	
2. socket接口有两种工作模式，一种是阻塞模式，一种是非阻塞模式
	- 阻塞模式通常不会用，因为它有两个弊端：
		- 会阻塞线程，要想处理多个连接就必须要一个连接一个线程，这样线程开销大且复杂度高;
		- read时，如果对端链路异常了，协议栈无法正常得到关闭通知，那么就永远阻塞住了。
		
	- 非阻塞模式现今有两类：
		- 一类是同步事件分离器，以select,epoll为代表
		- 一类是异步io,以iocp为代表。
		
		- 在非阻塞模式的实践上，通常会使用reactor和proactor这两种设计模式。


3. 涉及的系统接口

![socket 接口](socket.jpeg)

### reactor
1. reactor由资源、事件处理器、资源管理器、调度器和同步事件分离器构成。

2. 资源是抽象出的能产生某些事件的对象，资源的种类表示着reactor功能的数量，在本文特指socket。

3. 事件处理器是资源产生事件时执行的函数。资源管理器是资源和事件处理器的容器。

4. 调度器是一个过程，调用同步事件分离器检测多个资源产生的事件，然后唤醒对应的事件处理器。

5. 同步事件分离器就是select,epoll等系统函数。

6. reactor基于事件循环(调度器不断执行),能以单线程同步检测处理多路socket的事件。
其输入就是资源和事件处理器，其输出为唤醒事件处理器。libev就是典型的实现。

7. 其优点为：
	- 单线程处理多路socket,规范了一套处理流程。

8. 其缺点为：
	- 使用者的知识度没有减少，需要自己进行accept,read,write和错误判断。
	- 假设为单线程环境，接口都不是线程安全的，在多线程项目中用起来不是很平滑。(与模式无关，libev等实现的缺陷)

### proactor
1. 它由资源、请求、请求完成处理器、资源管理器、调度器和异步处理器构成。

2. 资源同reactor。

3. 请求是对资源的实际操作，如read,write,accept,每个请求对应于一个异步处理器,
	请求不会立刻执行，而是先缓存在请求队列中，由调度器统一处理。
	
4. 请求完成处理器是请求完成后的回调函数。

5. 资源管理器是资源、请求和请求完成处理器的容器。

6. 调度器也是一个过程，从资源管理器中取出请求，调用相应的异步处理器，在异步处理完毕后调用请求完成处理器。

7. 异步处理器为操作系统提供的异步操作函数，如iocp，或者用线程去模拟。它也是
基于事件循环，能以单线程并发处理多路socket的事件。其输入为资源和请求，其输出为请求完成通知。libuv就是典型的实现。

8. 其优点为：

	- 单线程处理多路socket,规范了一套处理流程。
	- 在操作系统支持真异步处理器时，有并发性。
	- 使用者所需知识度低，不需要了解实际的事件处理。

9. 其缺点为：
	- 假设为单线程环境，接口都不是线程安全的，在多线程项目中用起来不是很平滑。(与模式无关，libuv等实现的缺陷)

### reactor和proactor的区别
1. reactor侧重的是告诉使用者a资源发生了b事件，怎么处理你看着办吧。

2. proactor侧重的是告诉使用者，主人你对a资源进行的b请求完成了

3. 以做软件项目为例子，在reactor时，它就是老板，你就是程序员，某天它对你下达一个开发x软件的任务，
然后你就去调研需求，设计编码;

4. 在proactor时，它就是程序员，你就是老板，某天你对它下达一个开发x软件的任务，它开发完毕后告诉你，老板，x软件开发完毕。

5. 从实现细节来看，reactor就是缺少异步处理器，如果内部为每种事件提供一个异步处理器，
将输入的事件侦听变为提交请求，那么reactor就变为proactor了。

### 更好的proactor
1. 我希望所有的请求操作都是线程安全的，请求完成通知不是通过回调函数由内部发起，而是转化为一条消息，
由外部主动获取。这样能更好的整合到项目中。它的接口是这样的：

```
#ifndef SSOCKET_H
#define SSOCKET_H

#define    SS_MSG_ACCEPT 1 //客户端连接消息
#define SS_MSG_OPEN 2 //连接服务器成功消息
#define SS_MSG_DATA 3 //数据消息
#define SS_MSG_CLOSE 4 //对方断开消息
#define SS_MSG_ERROR 5 //错误消息，发生时，socket已经断开
#define SS_MSG_FILE 6 //文件发送成功消息

struct ss_message{
    int type;
    int id;
    void *ud;
    int size; //在type = SS_MSG_ACCEPT时，size为客户端的id;在SS_MSG_DATA时，size为data的大小

    //在SS_MSG_DATA时,data为数据;在SS_MSG_ACCEPT时,data为ip地址字符串;在SS_MSG_ERROR时，data为错误消息;在SS_MSG_FILE时,data为文件路径;
    //SS_MSG_DATA,SS_MSG_ACCEPT,SS_MSG_FILE,data需要外部释放
    char *data;
};

int ss_init();
//获取一条网络消息，return >0有消息
int ss_poll(struct ss_message *m);

//以下接口都是线程安全的
int ss_listen(const char *addr,int port,int bks,void *ud);
int ss_connect(const char *addr,int port,void *ud);
void ss_close(int id);
void ss_bind(int id,void *ud);
void ss_start(int id);
void ss_send(int id,char *data,int n);
int ss_send_file(int id,const char *path);

#endif
```

2. socket句柄不暴露给外部，而是暴露内部的id，内部的id可以映射到实际的socket句柄，这样做更安全，
通过id，内部可以检测该socket是否有效。

3. 由外部循环调用ss_poll来获取请求完成通知。

4. ss_bind可以为id绑定一个用户数据，该id的请求完成消息里会带上该用户数据。

5. ss_start用于开始accept,read这些持续性请求，而不需要一次一次发起。

6. ss_send将要发送的数据送入id的发送队列，内部会保证有序的发送出去。

7. ss_send_file:因为内部有发送队列要缓存数据，所以并不适合文件这种大数据量的需求，
该接口会使用sendfile要处理文件发送，从而避免缓存文件数据。

8. ss_close在关闭时会保证发送队列已经全部发送，否则不会关闭socket.

9. 具体的实现放在github上。

### 在服务器程序上使用
1. 多线程的服务器程序通常是这样的结构：

![]()

2. 网关用于提供入口点，会收到所有的socket消息，在accept时会创建一个agent,
此后该socket的消息就会发送到该agent。agent负责解码网络数据，分解出一个个协议包，然后调用协议包处理器处理该包，
再将处理的结果编码为协议包发送到该socket。因为proactor是单线程的，为了并发，一般有两种方式：

	1. agent解码出协议包后，用线程池来执行包处理逻辑。
	2. agent缓存住网络数据，由线程池来调度agent,以一定的频率来进行解码、包处理。

3. 网关和agent的作用是隔离网络层，进行网络协议和内部协议的转换。agent除了转换协议，
还可以保存中间状态，在有状态的协议中，agent和socket是一一配对的。在无状态的协议中，所有的socket可以共用一个agent。
