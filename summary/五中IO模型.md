## 一、DMA 控制器功能

1. 能向CPU发出系统保持(HOLD)信号，提出总线接管请求；
 
2. 当CPU发出允许接管信号后，负责对总线的控制，进入DMA方式;
 
3. 能对存储器寻址及能修改地址指针，实现对内存的读写；
 
4. 能决定本次DMA传送的字节数，判断DMA传送是否借宿。
 
5. 发出DMA结束信号，使CPU恢复正常工作状态。

## 二、读写
1. 当一个read操作发生时，它会经历两个阶段：
	- 通过read系统调用向内核发起读请求。
	- 内核向硬件发送读指令，并等待读就绪。
	- DMA把将要读取的数据复制到描述符所指向的内核缓存区中。
	- 内核将数据从内核缓存区拷贝到用户进程空间中。

## 三、IO操作
1. 同步IO
    > 当用户发出IO请求操作之后，内核会去查看要读取的数据是否就绪，如果数据没有就绪，就一直等待。
    > 需要通过用户线程或者内核不断地去轮询数据是否就绪，当数据就绪时，再将数据从内核拷贝到用户空间。 

2. 异步IO
    > 只有IO请求操作的发出是由用户线程来进行的，IO操作的两个阶段都是由内核自动完成，然后发
    > 送通知告知用户线程IO操作已经完成。也就是说在异步IO中，不会对用户线程产生任何阻塞。 

3. 阻塞IO
    > 当用户线程发起一个IO请求操作（以读请求操作为例），内核查看要读取的数据还没就绪，当前线
    > 程被挂起，阻塞等待结果返回。 

4. 非阻塞IO
    > 如果数据没有就绪，则会返回一个标志信息告知用户线程当前要读的数据没有就绪。当前线程在拿到
    > 此次请求结果的过程中，可以做其它事情。

## 四、五种I/O 模型
### 同步阻塞I/O模型
![阻塞IO模型](G:\picture\program_graph\阻塞IO模型.png)

### 同步非阻塞I/O模型
![非阻塞IO模型](G:\picture\program_graph\非阻塞IO模型.png)

### I/O复用模型
![阻塞IO模型](G:\picture\program_graph\IO复用模型.png)

### 信号驱动I/O模型
![阻塞IO模型](G:\picture\program_graph\信号驱动IO模型.png)

[信号驱动例子](https://blog.csdn.net/mcheaven/article/details/44303753)

### 异步I/O模型
![阻塞IO模型](G:\picture\program_graph\异步IO模型.png)

### 总结
1. 信号驱动I/O和IO多路复用有些相似，都是返回准备好读写的描述符，然后在这个描述符上可以进行阻塞的读写。不同是select是多路复用，而信号驱动是通过注册信号处理程序。
2. 异步IO是真正的异步完成了读，当拷贝数据拷贝到了用户空间后，才返回成功指示，这时已经完成了读了。

## 五、 AIO
1. aiocb结构

        struct aiocb 
        { 
          int aio_fildes;                    /* File desriptor.  */                        涉及到的文件句柄，用户填写 
          int aio_lio_opcode;           /* Operation to be performed.  */   执行操作，此位aio_read()会填写 
          int aio_reqprio;                 /* Request priority offset.  */          优先权等级 
          volatile void *aio_buf;       /* Location of buffer.  */                 buffer位置，用户填写 
          size_t aio_nbytes;            /* Length of transfer.  */                 buffer长度，用户填写 
          struct sigevent aio_sigevent; /* Signal number and value.  */  可以定义信号和call back的过程

          /* Internal members.  */ 
          struct aiocb *__next_prio; 
          int __abs_prio; 
          int __policy; 
          int __error_code; 
          __ssize_t __return_value;

          #ifndef __USE_FILE_OFFSET64 
          __off_t aio_offset;           /* File offset.  */                              操作的文件偏移 
          char __pad[sizeof (__off64_t) - sizeof (__off_t)]; 
          #else 
          __off64_t aio_offset;         /* File offset.  */ 
          #endif 
          char __unused[32]; 
        };

        
        struct sigevent {
           int          sigev_notify; /* 通知方法 */
           int          sigev_signo;  /* 通知信号 */
           union sigval sigev_value;  /* 传递的数据*/
           void       (*sigev_notify_function) (union sigval);/* 用线程的函数 */
           void        *sigev_notify_attributes;/* 通知线程的属性

        };
                
2. 必须验证每次读操作的结果的要求下，这个例子给出了两种方案
    1. wait_aio，即执行了aio_read()后，循环检查aio_return返回，这样的方式相比阻塞型的read()没有多少优势
    2. call_back,即执行aio_read()的同时，定义该操作响应信号和回调函数。而程序可以继续执行下去。
        当本次read()操作有结果时，自动执行call_back_handler函数。 （推荐）
  
### 参考链接
[AIO](https://blog.csdn.net/gufenglei/article/details/5692181)
