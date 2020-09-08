# 详解使用python接收邮件
> 版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本声明。
> 本文链接：https://blog.csdn.net/qq_33304418/article/details/82384090

## 使用POP3接收邮件
1. 导入 poplib 并实例化 poplib.POP3 类。标准流程如下所示：

 1. 连接到服务器。
 2. 登陆。
 3. 发出服务请求。
 4. 退出。

                                                    POP3 对象的常用方法
┌──────────┬──────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│       方 │                                                                                描述                          │
│法        │                                                                                                              │
├──────────┼──────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│user      │向服务器发送登录名，并显示服务器的响应，表示服务器正在等待输入该用户的密                                      │
│(login)   │                                                                                                              │
├──────────┼──────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│pass_     │在用户使用 user()登录后，发送 passwd。如果登录失败，则抛出异常                                                │
│(passwd)  │                                                                                                              │
├──────────┼──────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│stat()    │返回邮件的状态，即一个长度为 2 的元组（msg_ct, mbox_siz），分别表示消息的数量和消息的总大小（即字节数）       │
├──────────┼──────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│list      │stat()的扩展，从服务器返回以三元组表示的整个消息列表（rsp, msg_list, rsp_siz），分别为服务器的响应、消息列表、│
│([msgnum])│返回消息的大小。如果给定了 msgnum，则只返回指定消息的数据                                                     │
│          │                                                                                                              │
├──────────┼──────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│retr      │从服务器中得到消息的 msgnum，并设置其“已读”标志。返回一个长度为 3 的元组（rsp, msglines, msgsiz），分别为服务 │
│(msgnum)  │器的响应、消息的 msgnum 的所有行、消息的字节数                                                                │
├──────────┼──────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│dele      │把消息 msgnum 标记为删除，大多数服务器在调用 quit()后执行删除操作                                             │
│(msgnum)  │                                                                                                              │
├──────────┼──────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│quit()    │注销、提交修改（如处理“已读”和“删除”标记等）、解锁邮箱、终止连接，然后退出                                    │
└──────────┴──────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

import包的作用

import poplib

# base64解码
import base64

# 解码邮件信息
from email.parser import Parser

# 解析邮件主题
from email.header import decode_header

# 解析发件人详情，名称及地址
from email.utils import parseaddr

简单适用

  • 返回邮箱中的最新邮件

import poplib
import base64
from email.parser import Parser
from email.header import decode_header
from email.utils import parseaddr

def get_email_content():
        useraccount = '邮箱账号'
        password = '邮箱密码（客户端授权码非登陆密码）'

        # 邮件服务器地址,以下为网易邮箱
        pop3_server = 'pop.163.com'

        # 开始连接到服务器
        server = poplib.POP3(pop3_server)

        # 打开或者关闭调试信息，为打开，会在控制台打印客户端与服务器的交互信息
        server.set_debuglevel(1)

        # 打印POP3服务器的欢迎文字，验证是否正确连接到了邮件服务器
        print(server.getwelcome().decode('utf8'))

        # 开始进行身份验证
        server.user(useraccount)
        server.pass_(password)

        # 返回邮件总数目和占用服务器的空间大小（字节数）， 通过stat()方法即可
        email_num, email_size = server.stat()
        print("消息的数量: {0}, 消息的总大小: {1}".format(email_num, email_size))

        # 使用list()返回所有邮件的编号，默认为字节类型的串
        rsp, msg_list, rsp_siz = server.list()
        print("服务器的响应: {0},\n消息列表： {1},\n返回消息的大小： {2}".format(rsp, msg_list, rsp_siz))

        print('邮件总数： {}'.format(len(msg_list)))

        # 下面单纯获取最新的一封邮件
        total_mail_numbers = len(msg_list)
        rsp, msglines, msgsiz = server.retr(total_mail_numbers)
        #print("服务器的响应: {0},\n原始邮件内容： {1},\n该封邮件所占字节大小： {2}".format(rsp, msglines, msgsiz))

        msg_content = b'\r\n'.join(msglines).decode('gbk')

        msg = Parser().parsestr(text=msg_content)
        print('解码后的邮件信息:\n{}'.format(msg))

        # 关闭与服务器的连接，释放资源
        server.close()

        return msg

运行结果

+OK Welcome to coremail Mail Pop3 Server (163coms[b62aaa251425b4be4eaec4ab4744cf47s])
*cmd* 'USER 自己的邮箱'
*cmd* 'PASS 自己客户端的授权码(非登陆密码)'
*cmd* 'STAT'
*stat* [b'+OK', b'39', b'490135']
消息的数量: 39, 消息的总大小: 490135
*cmd* 'LIST'
服务器的响应: b'+OK 39 490135',
消息列表： [b'1 2365', b'2 2386', b'3 1364', b'4 1362', b'5 1364', b'6 1362', b'7 1341', b'8 1347', b'9 1374', b'10 1363', b'11 1363', b'12 1347', b'13 1363', b'14 1363', b'
15 1363', b'16 1420', b'17 1284', b'18 1345', b'19 1345', b'20 1349', b'21 5362', b'22 24923', b'23 70894', b'24 71219', b'25 29885', b'26 71565', b'27 8300', b'28 6126', b'29 7433', b'30 5734', b'31 8281', b'32 6121', b'33 7588', b'34 5784', b'35 7474', b'36 6667', b'37 71201', b'38 43956', b'39 2452'],
返回消息的大小： 349
邮件总数： 39
*cmd* 'RETR 39'
解码后的邮件信息:
Received: from qq.com (unknown [59.36.132.97])
        by mx41 (Coremail) with SMTP id W8CowEC5x2JkSI1beCFHNA--.45886S3;
        Mon, 03 Sep 2018 22:42:45 +0800 (CST)
DKIM-Signature: v=1; a=rsa-sha256; c=relaxed/relaxed; d=qq.com; s=s201512;
        t=1535985764; bh=6AIchfdyGOdxI/B5YbmgaxBND01pFaF48qXJpHro9Lw=;
        h=From:To:Subject:Mime-Version:Content-Type:Content-Transfer-Encoding:Date:Message-ID;
        b=lZXzZMCcQGKA0zGo8v+p7PTD7UeoTYzLwPuW6+WnnTA+r0+eQ19GWR53W4LtGhjDB
         0PeZAKCxDGX4jojqNm5EOkRq01WIcG58iNWjQbrWZm6pL5NWYGPKuR3Q348akJN/hx
         6F7/yvOjYxLnW+XbBCs6JZYYb0M4qspkdiWeYIq8=
X-QQ-FEAT: nUNo11GR11/+vQOYd+CTwW2wqkDWMZghzyfKgQ980f37FHXJIMUj1C60MISWI
        oh3noD9cnqVZGzbgaGQOy0yK/2EEOz3g7SkI9Adsx0hP/5tNTi3iz+o3GVllTAaBqDI8xqv
        CSuZvheXF4azceYiolXS3bnWX3TkZAmQBA/NqRpr5cENWNZEmmeRajzaRxgqseMSxRwS0J5
        dqWp4bFGdeZkHwS7IFKTAlK62Q/JvajwyP+NWdLuQmTZ7dNjE0h1SEQ9KlSYzg234zvdQIq
        9gIg==
X-QQ-SSF: 00010000000000F000000000000000Y
X-HAS-ATTACH: no
X-QQ-BUSINESS-ORIGIN: 2
X-Originating-IP: 223.104.44.44
X-QQ-STYLE:
X-QQ-mid: webmail158t1535985763t3377746
From: "=?gb18030?B?qHum7qnfqFSp19K7?=" <xxxxxxx@qq.com>
To: "=?gb18030?B?MTg3MTM4MjM2NzE=?=" <xxxxxxxxxx@163.com>
Subject: test
Mime-Version: 1.0
Content-Type: multipart/alternative;
        boundary="----=_NextPart_5B8D4863_0B8A9D40_22D04493"
Content-Transfer-Encoding: 8Bit
Date: Mon, 3 Sep 2018 22:42:43 +0800
X-Priority: 3
Message-ID: <tencent_D22DA8DEC2917C5D088C3E8106480F45F406@qq.com>
X-QQ-MIME: TCMime 1.0 by Tencent
X-Mailer: QQMail 2.x
X-QQ-Mailer: QQMail 2.x
X-QQ-SENDSIZE: 520
Received: from qq.com (unknown [127.0.0.1])
        by smtp.qq.com (ESMTP) with SMTP
        id ; Mon, 03 Sep 2018 22:42:43 +0800 (CST)
Feedback-ID: webmail:qq.com:bgweb:bgweb3
X-CM-TRANSID: W8CowEC5x2JkSI1beCFHNA--.45886S3
Authentication-Results: mx41; spf=pass smtp.mail=xxxxxxxxx@qq.com; dki
        m=pass header.i=@qq.com
X-Coremail-Antispam: 1Uf129KBjDUn29KB7ZKAUJUUUUU529EdanIXcx71UUUUU7v73
        VFW2AGmfu7bjvjm3AaLaJ3UbIYCTnIWIevJa73UjIFyTuYvjxUq1v3UUUUU

This is a multi-part message in MIME format.

------=_NextPart_5B8D4863_0B8A9D40_22D04493
Content-Type: text/plain;
        charset="gb18030"
Content-Transfer-Encoding: base64

SGVsbG8gV29yZA==

------=_NextPart_5B8D4863_0B8A9D40_22D04493
Content-Type: text/html;
        charset="gb18030"
Content-Transfer-Encoding: base64

PGRpdj48Zm9udCBmYWNlPSLLzszlIiBzaXplPSIyIj5IZWxsbyBXb3JkPC9mb250PjwvZGl2Pg==

------=_NextPart_5B8D4863_0B8A9D40_22D04493--

解析邮件主题 

# 用来解析邮件主题
from email.header import decode_header

def parser_subject(msg):
        subject = msg['Subject']
        value, charset = decode_header(subject)[0]
        if charset:
                value = value.decode(charset)
        print('邮件主题： {0}'.format(value))
        return value

解析邮件来源

# 用来解析邮件来源
from email.utils import parseaddr

def parser_address(msg):
        hdr, addr = parseaddr(msg['From'])
        # name 发送人邮箱名称， addr 发送人邮箱地址
        name, charset = decode_header(hdr)[0]
        if charset:
                name = name.decode(charset)
        print('发送人邮箱名称: {0}，发送人邮箱地址: {1}'.format(name, addr))

解析邮件内容

  • 正文信息是被base64编码后的串，需要获取编码格式进行解码

def parser_content(msg):
        content = msg.get_payload()

        # 文本信息
        content_charset = content[0].get_content_charset() # 获取编码格式
        text = content[0].as_string().split('base64')[-1]
        text_content = base64.b64decode(text).decode(content_charset) # base64解码

        # 添加了HTML代码的信息
        content_charset = content[1].get_content_charset()
        text = content[1].as_string().split('base64')[-1]
        html_content = base64.b64decode(text).decode(content_charset)

        print('文本信息: {0}\n添加了HTML代码的信息: {1}'.format(text_content, html_content))

运行 

if __name__ == '__main__':
    # 返回解码的邮件详情
    msg = get_email_content()
    # 解析邮件主题
    parser_subject(msg)
    # 解析发件人详情
    parser_address(msg)
    # 解析内容
    parser_content(msg)

Note：

  • 以上获取的是163邮箱
  • 客户端授权码需要登陆163邮箱进行设置
  • 内容是自己运用过程中的记录
