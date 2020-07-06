Docker文件
Docker文件
[                    ]
切换导航

  • 家
  • 导游
  • 产品手册
  • 参考
  • 样品

预计阅读时间： 65分钟

Docker运行参考🔗

Docker在隔离的容器中运行进程。容器是在主机上运行的进程。主机可以是本地主机，也可以是远程主机。当操作员执行时docker run，运行的容器进程是独立的，因为它具有自己的文件系统，自
己的网络以及与主机分离的独立的进程树。

本页详细介绍如何docker run在运行时使用命令定义容器的资源。

一般形式🔗

基本docker run命令采用以下形式：

$ docker run [OPTIONS] IMAGE[:TAG|@DIGEST] [COMMAND] [ARG...]

该docker run命令必须指定要从中派生容器的IMAGE。图像开发人员可以定义与以下内容有关的图像默认值：

  • 独立运行或前台运行
  • 容器识别
  • 网络设置
  • CPU和内存的运行时限制

使用，docker run [OPTIONS]操作员可以添加或覆盖开发人员设置的图像默认设置。而且，操作员可以覆盖Docker运行时本身设置的几乎所有默认值。操作员具有覆盖映像和Docker运行时默认值的
能力，这就是为什么run具有比其他任何docker命令更多的选项的原因。

要了解如何解释的类型[OPTIONS]，请参阅选项类型。

    注意

    根据您的Docker系统配置，可能需要在docker run命令前加上sudo。为了避免使用sudo该docker命令，系统管理员可以创建一个Unix组docker，并将其添加用户。有关此配置的更多信息，请参
    阅您操作系统的Docker安装文档。

运营商专有选项🔗

只有操作员（执行的人员docker run）可以设置以下选项。

  • 独立式vs前景式
      □ 分离（-d）
      □ 前景
  • 容器识别
      □ 名称（--name）
      □ PID当量
  • IPC设置（--ipc）
  • 网络设置
  • 重新启动策略（-重新启动）
  • 清理（​​--rm）
  • 资源的运行时限制
  • 运行时特权和Linux功能

独立式vs前景式🔗

启动Docker容器时，必须首先确定是要在后台以“分离”模式还是默认的前台模式运行该容器：

-d=false: Detached mode: Run container in the background, print new container id

分离（-d）🔗

要以分离模式启动容器，请使用-d=true或just -d选项。按照设计，以脱机模式启动的容器会在用于运行容器的根进程退出时退出，除非您也指定了该--rm选项。如果-d与with一起使用 --rm，则
在容器退出或守护程序退出时（以先发生的为准）将容器删除。

不要将service x start命令传递给分离的容器。例如，此命令尝试启动nginx服务。

$ docker run -d -p 80:80 my_image service nginx start

这成功启动nginx了容器内部的服务。但是，它失败了分离容器范例，因为根进程（service nginx start）返回并且分离容器按设计停止。结果，该 nginx服务已启动但无法使用。相反，要启动诸
如nginxWeb服务器之类的进程，请执行以下操作：

$ docker run -d -p 80:80 my_image nginx -g 'daemon off;'

要使用分离的容器进行输入/输出，请使用网络连接或共享卷。这些是必需的，因为容器不再侦听docker run运行所在的命令行。

要重新附加到分离的容器，请使用docker attach命令。

前景🔗

在前台模式下（-d未指定默认值），docker run可以在容器中启动进程并将控制台附加到进程的标准输入，输出和标准错误。它甚至可以假装为TTY（这是大多数命令行可执行文件所期望的）并传
递信号。所有这些都是可配置的：

-a=[]           : Attach to `STDIN`, `STDOUT` and/or `STDERR`
-t              : Allocate a pseudo-tty
--sig-proxy=true: Proxy all received signals to the process (non-TTY mode only)
-i              : Keep STDIN open even if not attached

如果您未指定，-a则Docker将同时附加到stdout和stderr 。您可以指定其中三个标准流（STDIN，STDOUT， STDERR）你想，而不是连接，如：

$ docker run -a stdin -a stdout -i -t ubuntu /bin/bash

对于交互式进程（如shell），必须-i -t一起使用才能为容器进程分配tty。-i -t通常是-it 在后面的示例中看到的。-t当客户端从管道接收其标准输入时，禁止指定，如：

$ echo test | docker run -i busybox cat

    注意

    Linux会特别处理在容器内以PID 1运行的进程：它会忽略具有默认操作的任何信号。其结果是，该进程也不会终止SIGINT或者SIGTERM除非它被编写这样做。

集装箱识别🔗

名称（--name）🔗

操作员可以通过三种方式识别容器：

 标识符类型                                 示例值
UUID长标识符 “ f78375b1c487e03c9438c729345e54db9d20cfa2ac1fc3494b6eb60872e74778”
UUID短标识符 “ f78375b1c487”
名称         “ evil_ptolemy”

UUID标识符来自Docker守护程序。如果未使用该--name选项分配容器名称，则守护程序将为您生成一个随机字符串名称。定义a name是增加容器含义的便捷方法。如果指定name，则可以在Docker网
络中引用容器时使用它。这适用于后台和前台Docker容器。

    注意

    必须链接默认网桥网络上的容器以按名称进行通信。

等效PID🔗

最后，为了帮助自动化，您可以让Docker将容器ID写入您选择的文件中。这类似于某些程序将其进程ID写入文件（您已将它们视为PID文件）的方式类似：

--cidfile="": Write the container ID to the file

图片[：tag] 🔗

虽然不是严格地标识容器的一种方法，但是您可以通过添加image[:tag]到命令来指定要用于运行容器的映像的版本。例如，docker run ubuntu:14.04。

图片[@digest] 🔗

使用v2或更高版本图像格式的图像具有称为摘要的内容可寻址标识符。只要用于生成图像的输入不变，摘要值就可以预测和参考。

以下示例从alpine带有sha256:9cacb71397b640eca97488cf08582ae4e4068513101088e9f96c9814bfda95e0摘要的图像运行容器：

$ docker run alpine@sha256:9cacb71397b640eca97488cf08582ae4e4068513101088e9f96c9814bfda95e0 date

PID设置（--pid）🔗

--pid=""  : Set the PID (Process) Namespace mode for the container,
             'container:<name|id>': joins another container's PID namespace
             'host': use the host's PID namespace inside the container

默认情况下，所有容器都启用了PID名称空间。

PID名称空间提供了进程分离。PID命名空间可删除系统进程的视图，并允许重用进程ID（包括pid 1）。

在某些情况下，您希望容器共享主机的进程名称空间，基本上允许容器内的进程查看系统上的所有进程。例如，您可以使用诸如strace或的调试工具来构建容器gdb，但是要在调试容器中的进程时
使用这些工具。

示例：在容器中运行htop🔗

创建此Dockerfile：

FROM alpine:latest
RUN apk add --update htop && rm -rf /var/cache/apk/*
CMD ["htop"]

构建Dockerfile并将映像标记为myhtop：

$ docker build -t myhtop .

使用以下命令htop在容器内运行：

$ docker run -it --rm --pid=host myhtop

加入另一个容器的pid名称空间可用于调试该容器。

例子🔗

启动运行Redis服务器的容器：

$ docker run --name my-redis -d redis

通过运行另一个包含strace的容器来调试redis容器：

$ docker run -it --pid=container:my-redis my_strace_docker_image bash
$ strace -p 1

UTS设置（--uts）🔗

--uts=""  : Set the UTS namespace mode for the container,
       'host': use the host's UTS namespace inside the container

UTS名称空间用于设置主机名和对该名称空间中正在运行的进程可见的域。默认情况下，所有容器（包括带有的容器）--network=host都具有自己的UTS命名空间。该host设置将导致容器使用与主机
相同的UTS命名空间。请注意， --hostname和--domainname在hostUTS模式下无效。

如果希望容器的主机名随着主机的主机名更改而更改，则可能希望与主机共享UTS名称空间。一个更高级的用例是从容器更改主机的主机名。

IPC设置（--ipc）🔗

--ipc="MODE"  : Set the IPC mode for the container

接受以下值：

          值                               描述
””                     使用守护程序的默认值。
“没有”                 自己的私有IPC名称空间，未安装/ dev / shm。
“私人的”               自己的私有IPC名称空间。
“可共享”               自己的私有IPC名称空间，可以与其他容器共享。
“容器：<_name-or-ID_>” 加入另一个（“可共享”）容器的IPC名称空间。
“主办”                 使用主机系统的IPC名称空间。

如果未指定，则使用守护程序默认值，它可以是"private" 或"shareable"，具体取决于守护程序的版本和配置。

IPC（POSIX / SysV IPC）名称空间提供命名共享内存段，信号量和消息队列的分隔。

共享内存段用于以内存速度加速进程间通信，而不是通过管道或网络堆栈。数据库和科学计算和金融服务行业的定制应用程序（通常是C / OpenMPI，C ++ /使用Boost库）通常使用共享内存。如果
将这些类型的应用程序分解为多个容器，则可能需要共享容器的IPC机制，并使用"shareable"主（即“捐赠者”）容器和"container:<donor-name-or-ID>"其他容器的模式。

网络设置🔗

--dns=[]           : Set custom dns servers for the container
--network="bridge" : Connect a container to a network
                      'bridge': create a network stack on the default Docker bridge
                      'none': no networking
                      'container:<name|id>': reuse another container's network stack
                      'host': use the Docker host network stack
                      '<network-name>|<network-id>': connect to a user-defined network
--network-alias=[] : Add network-scoped alias for the container
--add-host=""      : Add a line to /etc/hosts (host:IP)
--mac-address=""   : Sets the container's Ethernet device's MAC address
--ip=""            : Sets the container's Ethernet device's IPv4 address
--ip6=""           : Sets the container's Ethernet device's IPv6 address
--link-local-ip=[] : Sets one or more container's Ethernet device's link local IPv4/IPv6 addresses

默认情况下，所有容器都启用了联网功能，并且它们可以建立任何传出连接。操作员可以完全禁用网络，docker run --network none从而禁用所有传入和传出网络。在这样的情况下，您将通过文
件或执行I / O STDIN和STDOUT唯一的。

发布端口和链接到其他容器仅适用于默认端口（桥）。链接功能是旧版功能。与链接相比，您应该始终偏爱使用Docker网络驱动程序。

默认情况下，您的容器将使用与主机相同的DNS服务器，但是您可以使用来覆盖它--dns。

默认情况下，MAC地址是使用分配给容器的IP地址生成的。您可以通过使用--mac-address参数（格式12:34:56:78:9a:bc：）提供MAC地址来显式设置容器的MAC地址。请注意，Docker不会检查手动
指定的MAC地址是否唯一。

支持的网络：

      网络                                   描述
没有             容器中没有网络。
网桥（默认）     通过veth接口将容器连接到桥。
主办             使用容器内主机的网络堆栈。
容器：<名称| id> 使用通过其名称或id指定的另一个容器的网络堆栈。
网络             将容器连接到用户创建的网络（使用docker network create命令）

网络：无

有了网络，none容器将无法访问任何外部路由。容器仍将 loopback在容器中启用接口，但它没有任何通往外部流量的路由。

网桥

将网络设置为bridge容器后，将使用docker的默认网络设置。在主机上设置了一个网桥（俗称） docker0，并将veth为该容器创建一对接口。该veth对的一侧将保留在连接到网桥的主机上，而该对
的另一侧将除了loopback接口之外，还将放置在容器的命名空间内。将为网桥的网络上的容器分配一个IP地址，流量将通过该网桥路由到容器。

默认情况下，容器可以通过其IP地址进行通信。要通过名称进行通信，必须将它们链接起来。

网络：主机

将网络设置为host容器后，容器将共享主机的网络堆栈，并且主机的所有接口将对容器可用。容器的主机名将与主机系统上的主机名匹配。请注意，--mac-address在hostnetmode中无效。即使在
host 网络模式下，默认情况下，容器也有其自己的UTS名称空间。这样 --hostname，--domainname在host网络模式下是允许的，并且只会更改容器内的主机名和域名。类似--hostname的
--add-host，--dns，--dns-search，和 --dns-option选项可以在使用host网络模式。这些选项将更新 /etc/hosts或/etc/resolv.conf在容器内部。没有变化的，以制作 /etc/hosts，并/etc/
resolv.conf在主机上。

与默认bridge模式相比，该host模式提供了明显更好的网络性能，因为它使用了主机的本机网络堆栈，而网桥则必须通过docker守护程序进行一级虚拟化。当容器的网络性能至关重要时，建议以这
种模式运行容器，例如生产负载平衡器或高性能Web服务器。

    注意

    --network="host" 使容器可以完全访问本地系统服务（例如D-bus），因此被认为是不安全的。

网络：容器

将网络设置为container一个容器后，将共享另一个容器的网络堆栈。另一个容器的名称必须以的格式提供--network container:<name|id>。请注意，--add-host --hostname --dns --dns-search
--dns-option和--mac-address在container网络模式下无效，并且--publish --publish-all --expose在container网络模式下也无效。

使用Redis绑定运行Redis容器，localhost然后运行redis-cli命令并通过localhost接口连接到Redis服务器的示例。

$ docker run -d --name redis example/redis --bind 127.0.0.1
$ # use the redis container's network stack to access localhost
$ docker run --rm -it --network container:redis example/redis-cli -h 127.0.0.1

用户定义的网络

您可以使用Docker网络驱动程序或外部网络驱动程序插件创建网络。您可以将多个容器连接到同一网络。连接到用户定义的网络后，这些容器可以仅使用另一个容器的IP地址或名称轻松进行通信。

对于overlay支持多主机连接的网络或自定义插件，连接到同一多主机网络但从不同引擎启动的容器也可以这种方式进行通信。

以下示例使用内置bridge网络驱动程序并在创建的网络中运行容器来创建网络

$ docker network create -d bridge my-net
$ docker run --network=my-net -itd --name=container3 busybox

管理/ etc / hosts🔗

您的容器将在/etc/hosts其中定义容器本身的主机名以及localhost其他一些常见内容的行中。该 --add-host标志可用于向添加其他行/etc/hosts。

$ docker run -it --add-host db-static:86.75.30.9 ubuntu cat /etc/hosts

172.17.0.22     09d03f76bf2c
fe00::0         ip6-localnet
ff00::0         ip6-mcastprefix
ff02::1         ip6-allnodes
ff02::2         ip6-allrouters
127.0.0.1       localhost
::1                 localhost ip6-localhost ip6-loopback
86.75.30.9      db-static

如果某个容器已连接到默认网桥网络以及linked 其他容器，则该容器的/etc/hosts文件将使用链接的容器的名称进行更新。

    注意

    由于Docker可以实时更新容器的/etc/hosts文件，因此在某些情况下，容器内的进程最终可能会读取空/etc/hosts文件或不完整文件。在大多数情况下，再次尝试读取应该可以解决该问题。

重新启动策略（-重新启动）🔗

使用--restartDocker run上的标志，您可以指定重启策略，以指定如何在退出时重启容器。

当重新启动策略是活性的容器上，它将被显示为任一Up 或Restarting在docker ps。docker events查看生效的重启策略也很有用。

Docker支持以下重启策略：

        政策                                                                                  结果
没有                 退出时不要自动重启容器。这是默认值。
失败 [：max-retries] 仅当容器以非零退出状态退出时才重新启动。（可选）限制Docker守护进程尝试重新启动的重试次数。
总是                 无论退出状态如何，请始终重新启动容器。当您始终指定时，Docker守护程序将尝试无限期重启容器。无论容器的当前状态如何，该容器还将始终在守护程序启动时启动。
除非停止             无论退出状态如何（包括守护程序启动时），无论退出状态如何，都应始终重新启动容器，除非容器在停止Docker守护程序之前处于停止状态。

在每次重新启动之前，添加一个不断增加的延迟（从100毫秒开始，是以前的延迟的两倍），以防止服务器泛滥。这意味着守护程序将等待100毫秒，然后等待200毫秒，400、800、1600等，直到达
到on-failure限制，或者您docker stop 或您docker rm -f的容器为止。

如果成功重启容器（容器已启动并运行至少10秒钟），则延迟将重置为其默认值100毫秒。

您可以指定使用失败策略时Docker尝试重新启动容器的最大次数。默认是Docker将永远尝试重新启动容器。可以通过获取容器的（尝试的）重新启动次数docker inspect。例如，获取容器“
my-container”的重新启动次数；

$ docker inspect -f "{{ .RestartCount }}" my-container
# 2


或者，获取上一次（重新）启动容器的时间；

$ docker inspect -f "{{ .State.StartedAt }}" my-container
# 2015-03-04T23:47:07.691840179Z


将--restart（重新启动策略）与--rm（清除）标志结合使用会导致错误。容器重新启动时，连接的客户端将断开连接。请参阅本页后面有关使用--rm（清除）标志的示例。

例子🔗

$ docker run --restart=always redis

这将以始终redis具有重启策略的方式运行该容器，以便如果该容器退出，Docker将对其进行重启。

$ docker run --restart=on-failure:10 redis

这将以失败时redis重启策略运行容器，并且最大重启计数为10。如果容器连续以非零退出状态退出超过10次，则Docker将中止尝试重启容器的操作。提供最大重启限制仅对失败策略有效。redis

退出状态🔗

的退出代码docker run提供有关为何容器无法运行或为何退出的信息。当docker run使用非零代码退出时，退出代码遵循chroot标准，请参见以下内容：

如果错误是Docker守护程序本身导致的，则为 125

$ docker run --foo busybox; echo $?

flag provided but not defined: --foo
See 'docker run --help'.
125

126如果无法调用所包含的命令

$ docker run busybox /etc; echo $?

docker: Error response from daemon: Container command '/etc' could not be invoked.
126

127如果找不到所包含的命令

$ docker run busybox foo; echo $?

docker: Error response from daemon: Container command 'foo' not found or does not exist.
127

否则退出所包含命令的代码

$ docker run busybox /bin/sh -c 'exit 3'; echo $?

3

清理（​​--rm）🔗

默认情况下，即使容器退出后，容器的文件系统也会保留。这使调试容易得多（因为您可以检查最终状态），并且默认情况下保留所有数据。但是，如果您正在运行短期前台进程，那么这些容器文
件系统确实会堆积起来。相反，如果您希望Docker 在容器退出时自动清理容器并删除文件系统，则可以添加--rm标志：

--rm=false: Automatically remove the container when it exits

    注意

    如果设置该--rm标志，则在删除容器时，Docker也将删除与该容器关联的匿名卷。这类似于运行docker rm -v my-container。仅删除没有名称的指定卷。例如，运行时：

    docker run --rm -v /foo -v awesome:/bar busybox top

    的音量/foo将被删除，但的音量不会被移除/bar。--volumes-from将通过相同的逻辑删除通过继承的卷：如果使用名称指定了原始卷，则不会删除该卷。

安全配置🔗

                 选项                                          描述
--security-opt="label=user:USER"        设置容器的标签用户
--security-opt="label=role:ROLE"        设置容器的标签角色
--security-opt="label=type:TYPE"        设置容器的标签类型
--security-opt="label=level:LEVEL"      设置容器的标签级别
--security-opt="label=disable"          关闭容器的标签限制
--security-opt="apparmor=PROFILE"       设置要应用于容器的apparmor配置文件
--security-opt="no-new-privileges:true" 禁止容器进程获取新特权
--security-opt="seccomp=unconfined"     关闭容器的seccomp限制
--security-opt="seccomp=profile.json"   白名单的syscalls seccomp Json文件用作seccomp筛选器

您可以通过指定--security-opt标志来覆盖每个容器的默认标签方案。在以下命令中指定级别允许您在容器之间共享相同的内容。

$ docker run --security-opt label=level:s0:c100,c200 -it fedora bash

    注意

    当前不支持MLS标签的自动翻译。

要禁用此容器的安全标签而不是使用该--privileged标志运行，请使用以下命令：

$ docker run --security-opt label=disable -it fedora bash

如果要对容器内的进程采用更严格的安全策略，则可以为容器指定其他类型。您可以通过执行以下命令来运行仅允许在Apache端口上侦听的容器：

$ docker run --security-opt label=type:svirt_apache_t -it centos bash

    注意

    您将必须编写定义svirt_apache_t类型的策略。

如果要阻止容器进程获取其他特权，可以执行以下命令：

$ docker run --security-opt no-new-privileges -it centos bash

这意味着发出诸如su或的特权的命令sudo将不再起作用。它还会导致在删除特权后，稍后再应用任何seccomp过滤器，这可能意味着您可以使用一组限制性更强的过滤器。有关更多详细信息，请参
见内核文档。

指定一个初始化过程🔗

您可以使用该--init标志指示应将初始化进程用作容器中的PID 1。指定初始化过程可确保在创建的容器内执行初始化系统的通常职责，例如获取僵尸进程。

使用的默认初始化进程是docker-init在Docker守护进程的系统路径中找到的第一个可执行文件。docker-init默认安装中包含的此二进制文件由tini支持。

指定自定义cgroup🔗

使用该--cgroup-parent标志，您可以传递特定的cgroup来运行容器。这允许您自己创建和管理cgroup。您可以为这些cgroup定义自定义资源，并将容器放在公共父组下。

资源约束运行🔗

操作员还可以调整容器的性能参数：

        选项                                                                                   描述
-m， --memory=""      内存限制（格式：）<number>[<unit>]。数字是一个正整数。单位可以是一个b，k，m，或g。最小为4M。
--memory-swap=""      总内存限制（内存+交换，格式：）<number>[<unit>]。数字是一个正整数。单位可以是一个b，k，m，或g。
--memory-reservation= 内存软限制（格式：）<number>[<unit>]。数字是一个正整数。单位可以是一个b，k，m，或g。
""
--kernel-memory=""    内核内存限制（格式：）<number>[<unit>]。数字是一个正整数。单位可以是一个b，k，m，或g。最小为4M。
-c， --cpu-shares=0   CPU份额（相对重量）
--cpus=0.000          CPU数量。数字是小数。0.000表示没有限制。
--cpu-period=0        限制CPU CFS（完全公平的调度程序）期限
--cpuset-cpus=""      允许执行的CPU（0-3，0,1）
--cpuset-mems=""      允许执行的内存节点（MEM）（0-3，0,1）。仅在NUMA系统上有效。
--cpu-quota=0         限制CPU CFS（完全公平的调度程序）配额
--cpu-rt-period=0     Limit the CPU real-time period. In microseconds. Requires parent cgroups be set and cannot be higher than parent. Also check rtprio ulimits.
--cpu-rt-runtime=0    Limit the CPU real-time runtime. In microseconds. Requires parent cgroups be set and cannot be higher than parent. Also check rtprio ulimits.
--blkio-weight=0      Block IO weight (relative weight) accepts a weight value between 10 and 1000.
--blkio-weight-device Block IO weight (relative device weight, format: DEVICE_NAME:WEIGHT)
=""
--device-read-bps=""  Limit read rate from a device (format: <device-path>:<number>[<unit>]). Number is a positive integer. Unit can be one of kb, mb, or gb.
--device-write-bps="" Limit write rate to a device (format: <device-path>:<number>[<unit>]). Number is a positive integer. Unit can be one of kb, mb, or gb.
--device-read-iops="" Limit read rate (IO per second) from a device (format: <device-path>:<number>). Number is a positive integer.
--device-write-iops=  Limit write rate (IO per second) to a device (format: <device-path>:<number>). Number is a positive integer.
""
--oom-kill-disable=   Whether to disable OOM Killer for the container or not.
false
--oom-score-adj=0     Tune container’s OOM preferences (-1000 to 1000)
--memory-swappiness=  Tune a container’s memory swappiness behavior. Accepts an integer between 0 and 100.
""
--shm-size=""         Size of /dev/shm. The format is <number><unit>. number must be greater than 0. Unit is optional and can be b (bytes), k (kilobytes), m (megabytes), or
                      g (gigabytes). If you omit the unit, the system uses bytes. If you omit the size entirely, the system uses 64m.

User memory constraints🔗

We have four ways to set user memory usage:

           Option                                                                                 Result
memory=inf, memory-swap=inf   There is no memory limit for the container. The container can use as much memory as needed.
(default)
memory=L<inf, memory-swap=inf (specify memory and set memory-swap as -1) The container is not allowed to use more than L bytes of memory, but can use as much swap as is
                              needed (if the host supports swap memory).
memory=L<inf, memory-swap=2*L (specify memory without memory-swap) The container is not allowed to use more than L bytes of memory, swap plus memory usage is double of
                              that.
memory=L<inf, memory-swap=S   (specify both memory and memory-swap) The container is not allowed to use more than L bytes of memory, swap plus memory usage is limited by S.
<inf, L<=S

Examples:

$ docker run -it ubuntu:14.04 /bin/bash

We set nothing about memory, this means the processes in the container can use as much memory and swap memory as they need.

$ docker run -it -m 300M --memory-swap -1 ubuntu:14.04 /bin/bash

We set memory limit and disabled swap memory limit, this means the processes in the container can use 300M memory and as much swap memory as they need (if the host supports
swap memory).

$ docker run -it -m 300M ubuntu:14.04 /bin/bash

We set memory limit only, this means the processes in the container can use 300M memory and 300M swap memory, by default, the total virtual memory size (--memory-swap) will
be set as double of memory, in this case, memory + swap would be 2*300M, so processes can use 300M swap memory as well.

$ docker run -it -m 300M --memory-swap 1G ubuntu:14.04 /bin/bash

We set both memory and swap memory, so the processes in the container can use 300M memory and 700M swap memory.

Memory reservation is a kind of memory soft limit that allows for greater sharing of memory. Under normal circumstances, containers can use as much of the memory as needed
and are constrained only by the hard limits set with the -m/--memory option. When memory reservation is set, Docker detects memory contention or low memory and forces
containers to restrict their consumption to a reservation limit.

Always set the memory reservation value below the hard limit, otherwise the hard limit takes precedence. A reservation of 0 is the same as setting no reservation. By
default (without reservation set), memory reservation is the same as the hard memory limit.

Memory reservation is a soft-limit feature and does not guarantee the limit won’t be exceeded. Instead, the feature attempts to ensure that, when memory is heavily
contended for, memory is allocated based on the reservation hints/setup.

The following example limits the memory (-m) to 500M and sets the memory reservation to 200M.

$ docker run -it -m 500M --memory-reservation 200M ubuntu:14.04 /bin/bash

Under this configuration, when the container consumes memory more than 200M and less than 500M, the next system memory reclaim attempts to shrink container memory below
200M.

The following example set memory reservation to 1G without a hard memory limit.

$ docker run -it --memory-reservation 1G ubuntu:14.04 /bin/bash

The container can use as much memory as it needs. The memory reservation setting ensures the container doesn’t consume too much memory for long time, because every memory
reclaim shrinks the container’s consumption to the reservation.

By default, kernel kills processes in a container if an out-of-memory (OOM) error occurs. To change this behaviour, use the --oom-kill-disable option. Only disable the OOM
killer on containers where you have also set the -m/--memory option. If the -m flag is not set, this can result in the host running out of memory and require killing the
host’s system processes to free memory.

The following example limits the memory to 100M and disables the OOM killer for this container:

$ docker run -it -m 100M --oom-kill-disable ubuntu:14.04 /bin/bash

The following example, illustrates a dangerous way to use the flag:

$ docker run -it --oom-kill-disable ubuntu:14.04 /bin/bash

The container has unlimited memory which can cause the host to run out memory and require killing system processes to free memory. The --oom-score-adj parameter can be
changed to select the priority of which containers will be killed when the system is out of memory, with negative scores making them less likely to be killed, and positive
scores more likely.

Kernel memory constraints🔗

Kernel memory is fundamentally different than user memory as kernel memory can’t be swapped out. The inability to swap makes it possible for the container to block system
services by consuming too much kernel memory. Kernel memory includes：

  • stack pages
  • slab pages
  • sockets memory pressure
  • tcp memory pressure

You can setup kernel memory limit to constrain these kinds of memory. For example, every process consumes some stack pages. By limiting kernel memory, you can prevent new
processes from being created when the kernel memory usage is too high.

Kernel memory is never completely independent of user memory. Instead, you limit kernel memory in the context of the user memory limit. Assume “U” is the user memory limit
and “K” the kernel limit. There are three possible ways to set limits:

   Option                                                                                Result
U != 0, K =
inf          This is the standard memory limitation mechanism already present before using kernel memory. Kernel memory is completely ignored.
(default)
U != 0, K <  Kernel memory is a subset of the user memory. This setup is useful in deployments where the total amount of memory per-cgroup is overcommitted. Overcommitting
U            kernel memory limits is definitely not recommended, since the box can still run out of non-reclaimable memory. In this case, you can configure K so that the
             sum of all groups is never greater than the total memory. Then, freely set U at the expense of the system's service quality.
U != 0, K >  Since kernel memory charges are also fed to the user counter and reclamation is triggered for the container for both kinds of memory. This configuration gives
U            the admin a unified view of memory. It is also useful for people who just want to track kernel memory usage.

Examples:

$ docker run -it -m 500M --kernel-memory 50M ubuntu:14.04 /bin/bash

We set memory and kernel memory, so the processes in the container can use 500M memory in total, in this 500M memory, it can be 50M kernel memory tops.

$ docker run -it --kernel-memory 50M ubuntu:14.04 /bin/bash

We set kernel memory without -m, so the processes in the container can use as much memory as they want, but they can only use 50M kernel memory.

Swappiness constraint🔗

By default, a container’s kernel can swap out a percentage of anonymous pages. To set this percentage for a container, specify a --memory-swappiness value between 0 and
100. A value of 0 turns off anonymous page swapping. A value of 100 sets all anonymous pages as swappable. By default, if you are not using --memory-swappiness, memory
swappiness value will be inherited from the parent.

For example, you can set:

$ docker run -it --memory-swappiness=0 ubuntu:14.04 /bin/bash

Setting the --memory-swappiness option is helpful when you want to retain the container’s working set and to avoid swapping performance penalties.

CPU share constraint🔗

By default, all containers get the same proportion of CPU cycles. This proportion can be modified by changing the container’s CPU share weighting relative to the weighting
of all other running containers.

To modify the proportion from the default of 1024, use the -c or --cpu-shares flag to set the weighting to 2 or higher. If 0 is set, the system will ignore the value and
use the default of 1024.

The proportion will only apply when CPU-intensive processes are running. When tasks in one container are idle, other containers can use the left-over CPU time. The actual
amount of CPU time will vary depending on the number of containers running on the system.

For example, consider three containers, one has a cpu-share of 1024 and two others have a cpu-share setting of 512. When processes in all three containers attempt to use
100% of CPU, the first container would receive 50% of the total CPU time. If you add a fourth container with a cpu-share of 1024, the first container only gets 33% of the
CPU. The remaining containers receive 16.5%, 16.5% and 33% of the CPU.

On a multi-core system, the shares of CPU time are distributed over all CPU cores. Even if a container is limited to less than 100% of CPU time, it can use 100% of each
individual CPU core.

For example, consider a system with more than three cores. If you start one container {C0} with -c=512 running one process, and another container {C1} with -c=1024 running
two processes, this can result in the following division of CPU shares:

PID    container        CPU     CPU share
100    {C0}             0       100% of CPU0
101    {C1}             1       100% of CPU1
102    {C1}             2       100% of CPU2

CPU period constraint🔗

The default CPU CFS (Completely Fair Scheduler) period is 100ms. We can use --cpu-period to set the period of CPUs to limit the container’s CPU usage. And usually
--cpu-period should work with --cpu-quota.

Examples:

$ docker run -it --cpu-period=50000 --cpu-quota=25000 ubuntu:14.04 /bin/bash

If there is 1 CPU, this means the container can get 50% CPU worth of run-time every 50ms.

In addition to use --cpu-period and --cpu-quota for setting CPU period constraints, it is possible to specify --cpus with a float number to achieve the same purpose. For
example, if there is 1 CPU, then --cpus=0.5 will achieve the same result as setting --cpu-period=50000 and --cpu-quota=25000 (50% CPU).

The default value for --cpus is 0.000, which means there is no limit.

For more information, see the CFS documentation on bandwidth limiting.

Cpuset constraint🔗

We can set cpus in which to allow execution for containers.

Examples:

$ docker run -it --cpuset-cpus="1,3" ubuntu:14.04 /bin/bash

This means processes in container can be executed on cpu 1 and cpu 3.

$ docker run -it --cpuset-cpus="0-2" ubuntu:14.04 /bin/bash

This means processes in container can be executed on cpu 0, cpu 1 and cpu 2.

We can set mems in which to allow execution for containers. Only effective on NUMA systems.

Examples:

$ docker run -it --cpuset-mems="1,3" ubuntu:14.04 /bin/bash

This example restricts the processes in the container to only use memory from memory nodes 1 and 3.

$ docker run -it --cpuset-mems="0-2" ubuntu:14.04 /bin/bash

This example restricts the processes in the container to only use memory from memory nodes 0, 1 and 2.

CPU quota constraint🔗

The --cpu-quota flag limits the container’s CPU usage. The default 0 value allows the container to take 100% of a CPU resource (1 CPU). The CFS (Completely Fair Scheduler)
handles resource allocation for executing processes and is default Linux Scheduler used by the kernel. Set this value to 50000 to limit the container to 50% of a CPU
resource. For multiple CPUs, adjust the --cpu-quota as necessary. For more information, see the CFS documentation on bandwidth limiting.

Block IO bandwidth (Blkio) constraint🔗

By default, all containers get the same proportion of block IO bandwidth (blkio). This proportion is 500. To modify this proportion, change the container’s blkio weight
relative to the weighting of all other running containers using the --blkio-weight flag.

    Note:

    The blkio weight setting is only available for direct IO. Buffered IO is not currently supported.

The --blkio-weight flag can set the weighting to a value between 10 to 1000. For example, the commands below create two containers with different blkio weight:

$ docker run -it --name c1 --blkio-weight 300 ubuntu:14.04 /bin/bash
$ docker run -it --name c2 --blkio-weight 600 ubuntu:14.04 /bin/bash

If you do block IO in the two containers at the same time, by, for example:

$ time dd if=/mnt/zerofile of=test.out bs=1M count=1024 oflag=direct

You’ll find that the proportion of time is the same as the proportion of blkio weights of the two containers.

The --blkio-weight-device="DEVICE_NAME:WEIGHT" flag sets a specific device weight. The DEVICE_NAME:WEIGHT is a string containing a colon-separated device name and weight.
For example, to set /dev/sda device weight to 200:

$ docker run -it \
    --blkio-weight-device "/dev/sda:200" \
    ubuntu

If you specify both the --blkio-weight and --blkio-weight-device, Docker uses the --blkio-weight as the default weight and uses --blkio-weight-device to override this
default with a new value on a specific device. The following example uses a default weight of 300 and overrides this default on /dev/sda setting that weight to 200:

$ docker run -it \
    --blkio-weight 300 \
    --blkio-weight-device "/dev/sda:200" \
    ubuntu

The --device-read-bps flag limits the read rate (bytes per second) from a device. For example, this command creates a container and limits the read rate to 1mb per second
from /dev/sda:

$ docker run -it --device-read-bps /dev/sda:1mb ubuntu

The --device-write-bps flag limits the write rate (bytes per second) to a device. For example, this command creates a container and limits the write rate to 1mb per second
for /dev/sda:

$ docker run -it --device-write-bps /dev/sda:1mb ubuntu

Both flags take limits in the <device-path>:<limit>[unit] format. Both read and write rates must be a positive integer. You can specify the rate in kb (kilobytes), mb
(megabytes), or gb (gigabytes).

The --device-read-iops flag limits read rate (IO per second) from a device. For example, this command creates a container and limits the read rate to 1000 IO per second
from /dev/sda:

$ docker run -ti --device-read-iops /dev/sda:1000 ubuntu

The --device-write-iops flag limits write rate (IO per second) to a device. For example, this command creates a container and limits the write rate to 1000 IO per second to
/dev/sda:

$ docker run -ti --device-write-iops /dev/sda:1000 ubuntu

Both flags take limits in the <device-path>:<limit> format. Both read and write rates must be a positive integer.

Additional groups🔗

--group-add: Add additional groups to run as

By default, the docker container process runs with the supplementary groups looked up for the specified user. If one wants to add more to that list of groups, then one can
use this flag:

$ docker run --rm --group-add audio --group-add nogroup --group-add 777 busybox id

uid=0(root) gid=0(root) groups=10(wheel),29(audio),99(nogroup),777

Runtime privilege and Linux capabilities🔗

   Option                                     Description
--cap-add    Add Linux capabilities
--cap-drop   Drop Linux capabilities
--privileged Give extended privileges to this container
--device=[]  Allows you to run devices inside the container without the --privileged flag.

By default, Docker containers are “unprivileged” and cannot, for example, run a Docker daemon inside a Docker container. This is because by default a container is not
allowed to access any devices, but a “privileged” container is given access to all devices (see the documentation on cgroups devices).

When the operator executes docker run --privileged, Docker will enable access to all devices on the host as well as set some configuration in AppArmor or SELinux to allow
the container nearly all the same access to the host as processes running outside containers on the host. Additional information about running with --privileged is
available on the Docker Blog.

If you want to limit access to a specific device or devices you can use the --device flag. It allows you to specify one or more devices that will be accessible within the
container.

$ docker run --device=/dev/snd:/dev/snd ...

By default, the container will be able to read, write, and mknod these devices. This can be overridden using a third :rwm set of options to each --device flag:

$ docker run --device=/dev/sda:/dev/xvdc --rm -it ubuntu fdisk  /dev/xvdc

Command (m for help): q
$ docker run --device=/dev/sda:/dev/xvdc:r --rm -it ubuntu fdisk  /dev/xvdc
You will not be able to write the partition table.

Command (m for help): q

$ docker run --device=/dev/sda:/dev/xvdc:w --rm -it ubuntu fdisk  /dev/xvdc
    crash....

$ docker run --device=/dev/sda:/dev/xvdc:m --rm -it ubuntu fdisk  /dev/xvdc
fdisk: unable to open /dev/xvdc: Operation not permitted

In addition to --privileged, the operator can have fine grain control over the capabilities using --cap-add and --cap-drop. By default, Docker has a default list of
capabilities that are kept. The following table lists the Linux capability options which are allowed by default and can be dropped.

 Capability Key                                                     Capability Description
SETPCAP          Modify process capabilities.
MKNOD            Create special files using mknod(2).
AUDIT_WRITE      Write records to kernel auditing log.
CHOWN            Make arbitrary changes to file UIDs and GIDs (see chown(2)).
NET_RAW          Use RAW and PACKET sockets.
DAC_OVERRIDE     Bypass file read, write, and execute permission checks.
FOWNER           Bypass permission checks on operations that normally require the file system UID of the process to match the UID of the file.
FSETID           Don’t clear set-user-ID and set-group-ID permission bits when a file is modified.
KILL             Bypass permission checks for sending signals.
SETGID           Make arbitrary manipulations of process GIDs and supplementary GID list.
SETUID           Make arbitrary manipulations of process UIDs.
NET_BIND_SERVICE Bind a socket to internet domain privileged ports (port numbers less than 1024).
SYS_CHROOT       Use chroot(2), change root directory.
SETFCAP          Set file capabilities.

The next table shows the capabilities which are not granted by default and may be added.

Capability Key                                              Capability Description
SYS_MODULE      Load and unload kernel modules.
SYS_RAWIO       Perform I/O port operations (iopl(2) and ioperm(2)).
SYS_PACCT       Use acct(2), switch process accounting on or off.
SYS_ADMIN       Perform a range of system administration operations.
SYS_NICE        Raise process nice value (nice(2), setpriority(2)) and change the nice value for arbitrary processes.
SYS_RESOURCE    Override resource Limits.
SYS_TIME        Set system clock (settimeofday(2), stime(2), adjtimex(2)); set real-time (hardware) clock.
SYS_TTY_CONFIG  Use vhangup(2); employ various privileged ioctl(2) operations on virtual terminals.
AUDIT_CONTROL   Enable and disable kernel auditing; change auditing filter rules; retrieve auditing status and filtering rules.
MAC_ADMIN       Allow MAC configuration or state changes. Implemented for the Smack LSM.
MAC_OVERRIDE    Override Mandatory Access Control (MAC). Implemented for the Smack Linux Security Module (LSM).
NET_ADMIN       Perform various network-related operations.
SYSLOG          Perform privileged syslog(2) operations.
DAC_READ_SEARCH Bypass file read permission checks and directory read and execute permission checks.
LINUX_IMMUTABLE Set the FS_APPEND_FL and FS_IMMUTABLE_FL i-node flags.
NET_BROADCAST   Make socket broadcasts, and listen to multicasts.
IPC_LOCK        Lock memory (mlock(2), mlockall(2), mmap(2), shmctl(2)).
IPC_OWNER       Bypass permission checks for operations on System V IPC objects.
SYS_PTRACE      Trace arbitrary processes using ptrace(2).
SYS_BOOT        Use reboot(2) and kexec_load(2), reboot and load a new kernel for later execution.
LEASE           Establish leases on arbitrary files (see fcntl(2)).
WAKE_ALARM      Trigger something that will wake up the system.
BLOCK_SUSPEND   Employ features that can block system suspend.

Further reference information is available on the capabilities(7) - Linux man page

Both flags support the value ALL, so if the operator wants to have all capabilities but MKNOD they could use:

$ docker run --cap-add=ALL --cap-drop=MKNOD ...

For interacting with the network stack, instead of using --privileged they should use --cap-add=NET_ADMIN to modify the network interfaces.

$ docker run -it --rm  ubuntu:14.04 ip link add dummy0 type dummy

RTNETLINK answers: Operation not permitted

$ docker run -it --rm --cap-add=NET_ADMIN ubuntu:14.04 ip link add dummy0 type dummy

To mount a FUSE based filesystem, you need to combine both --cap-add and --device:

$ docker run --rm -it --cap-add SYS_ADMIN sshfs sshfs sven@10.10.10.20:/home/sven /mnt

fuse: failed to open /dev/fuse: Operation not permitted

$ docker run --rm -it --device /dev/fuse sshfs sshfs sven@10.10.10.20:/home/sven /mnt

fusermount: mount failed: Operation not permitted

$ docker run --rm -it --cap-add SYS_ADMIN --device /dev/fuse sshfs

# sshfs sven@10.10.10.20:/home/sven /mnt
The authenticity of host '10.10.10.20 (10.10.10.20)' can't be established.
ECDSA key fingerprint is 25:34:85:75:25:b0:17:46:05:19:04:93:b5:dd:5f:c6.
Are you sure you want to continue connecting (yes/no)? yes
sven@10.10.10.20's password:

root@30aa0cfaf1b5:/# ls -la /mnt/src/docker

total 1516
drwxrwxr-x 1 1000 1000   4096 Dec  4 06:08 .
drwxrwxr-x 1 1000 1000   4096 Dec  4 11:46 ..
-rw-rw-r-- 1 1000 1000     16 Oct  8 00:09 .dockerignore
-rwxrwxr-x 1 1000 1000    464 Oct  8 00:09 .drone.yml
drwxrwxr-x 1 1000 1000   4096 Dec  4 06:11 .git
-rw-rw-r-- 1 1000 1000    461 Dec  4 06:08 .gitignore
....

The default seccomp profile will adjust to the selected capabilities, in order to allow use of facilities allowed by the capabilities, so you should not have to adjust
this, since Docker 1.12. In Docker 1.10 and 1.11 this did not happen and it may be necessary to use a custom seccomp profile or use --security-opt seccomp=unconfined when
adding capabilities.

Logging drivers (--log-driver)🔗

The container can have a different logging driver than the Docker daemon. Use the --log-driver=VALUE with the docker run command to configure the container’s logging
driver. The following options are supported:

 Driver                                                            Description
none      Disables any logging for the container. docker logs won’t be available with this driver.
json-file Default logging driver for Docker. Writes JSON messages to file. No logging options are supported for this driver.
syslog    Syslog logging driver for Docker. Writes log messages to syslog.
journald  Journald logging driver for Docker. Writes log messages to journald.
gelf      Graylog Extended Log Format (GELF) logging driver for Docker. Writes log messages to a GELF endpoint likeGraylog or Logstash.
fluentd   Fluentd logging driver for Docker. Writes log messages to fluentd (forward input).
awslogs   Amazon CloudWatch Logs logging driver for Docker. Writes log messages to Amazon CloudWatch Logs
splunk    Splunk logging driver for Docker. Writes log messages to splunk using Event Http Collector.

The docker logs command is available only for the json-file and journald logging drivers. For detailed information on working with logging drivers, see Configure logging
drivers.

Overriding Dockerfile image defaults🔗

When a developer builds an image from a Dockerfile or when she commits it, the developer can set a number of default parameters that take effect when the image starts up as
a container.

Four of the Dockerfile commands cannot be overridden at runtime: FROM, MAINTAINER, RUN, and ADD. Everything else has a corresponding override in docker run. We’ll go
through what the developer might have set in each Dockerfile instruction and how the operator can override that setting.

  • CMD (Default Command or Options)
  • ENTRYPOINT (Default Command to Execute at Runtime)
  • EXPOSE (Incoming Ports)
  • ENV (Environment Variables)
  • HEALTHCHECK
  • VOLUME (Shared Filesystems)
  • USER
  • WORKDIR

CMD (default command or options)🔗

Recall the optional COMMAND in the Docker commandline:

$ docker run [OPTIONS] IMAGE[:TAG|@DIGEST] [COMMAND] [ARG...]

This command is optional because the person who created the IMAGE may have already provided a default COMMAND using the Dockerfile CMD instruction. As the operator (the
person running a container from the image), you can override that CMD instruction just by specifying a new COMMAND.

If the image also specifies an ENTRYPOINT then the CMD or COMMAND get appended as arguments to the ENTRYPOINT.

ENTRYPOINT (default command to execute at runtime)🔗

    --entrypoint="": Overwrite the default entrypoint set by the image

The ENTRYPOINT of an image is similar to a COMMAND because it specifies what executable to run when the container starts, but it is (purposely) more difficult to override.
The ENTRYPOINT gives a container its default nature or behavior, so that when you set an ENTRYPOINT you can run the container as if it were that binary, complete with
default options, and you can pass in more options via the COMMAND. But, sometimes an operator may want to run something else inside the container, so you can override the
default ENTRYPOINT at runtime by using a string to specify the new ENTRYPOINT. Here is an example of how to run a shell in a container that has been set up to automatically
run something else (like /usr/bin/redis-server):

$ docker run -it --entrypoint /bin/bash example/redis

or two examples of how to pass more parameters to that ENTRYPOINT:

$ docker run -it --entrypoint /bin/bash example/redis -c ls -l
$ docker run -it --entrypoint /usr/bin/redis-cli example/redis --help

You can reset a containers entrypoint by passing an empty string, for example:

$ docker run -it --entrypoint="" mysql bash

    Note

    Passing --entrypoint will clear out any default command set on the image (i.e. any CMD instruction in the Dockerfile used to build it).

EXPOSE (incoming ports)🔗

The following run command options work with container networking:

--expose=[]: Expose a port or a range of ports inside the container.
             These are additional to those exposed by the `EXPOSE` instruction
-P         : Publish all exposed ports to the host interfaces
-p=[]      : Publish a container's port or a range of ports to the host
               format: ip:hostPort:containerPort | ip::containerPort | hostPort:containerPort | containerPort
               Both hostPort and containerPort can be specified as a
               range of ports. When specifying ranges for both, the
               number of container ports in the range must match the
               number of host ports in the range, for example:
                   -p 1234-1236:1234-1236/tcp

               When specifying a range for hostPort only, the
               containerPort must not be a range.  In this case the
               container port is published somewhere within the
               specified hostPort range. (e.g., `-p 1234-1236:1234/tcp`)

               (use 'docker port' to see the actual mapping)

--link=""  : Add link to another container (<name or id>:alias or <name or id>)

With the exception of the EXPOSE directive, an image developer hasn’t got much control over networking. The EXPOSE instruction defines the initial incoming ports that
provide services. These ports are available to processes inside the container. An operator can use the --expose option to add to the exposed ports.

To expose a container’s internal port, an operator can start the container with the -P or -p flag. The exposed port is accessible on the host and the ports are available to
any client that can reach the host.

The -P option publishes all the ports to the host interfaces. Docker binds each exposed port to a random port on the host. The range of ports are within an ephemeral port
range defined by /proc/sys/net/ipv4/ip_local_port_range. Use the -p flag to explicitly map a single port or range of ports.

The port number inside the container (where the service listens) does not need to match the port number exposed on the outside of the container (where clients connect). For
example, inside the container an HTTP service is listening on port 80 (and so the image developer specifies EXPOSE 80 in the Dockerfile). At runtime, the port might be
bound to 42800 on the host. To find the mapping between the host ports and the exposed ports, use docker port.

If the operator uses --link when starting a new client container in the default bridge network, then the client container can access the exposed port via a private
networking interface. If --link is used when starting a container in a user-defined network as described in Networking overview, it will provide a named alias for the
container being linked to.

ENV (environment variables)🔗

Docker automatically sets some environment variables when creating a Linux container. Docker does not set any environment variables when creating a Windows container.

The following environment variables are set for Linux containers:

Variable                                                Value
HOME     Set based on the value of USER
HOSTNAME The hostname associated with the container
PATH     Includes popular directories, such as /usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
TERM     xterm if the container is allocated a pseudo-TTY

Additionally, the operator can set any environment variable in the container by using one or more -e flags, even overriding those mentioned above, or already defined by the
developer with a Dockerfile ENV. If the operator names an environment variable without specifying a value, then the current value of the named variable is propagated into
the container’s environment:

$ export today=Wednesday
$ docker run -e "deep=purple" -e today --rm alpine env

PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
HOSTNAME=d2219b854598
deep=purple
today=Wednesday
HOME=/root

PS C:\> docker run --rm -e "foo=bar" microsoft/nanoserver cmd /s /c set
ALLUSERSPROFILE=C:\ProgramData
APPDATA=C:\Users\ContainerAdministrator\AppData\Roaming
CommonProgramFiles=C:\Program Files\Common Files
CommonProgramFiles(x86)=C:\Program Files (x86)\Common Files
CommonProgramW6432=C:\Program Files\Common Files
COMPUTERNAME=C2FAEFCC8253
ComSpec=C:\Windows\system32\cmd.exe
foo=bar
LOCALAPPDATA=C:\Users\ContainerAdministrator\AppData\Local
NUMBER_OF_PROCESSORS=8
OS=Windows_NT
Path=C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Users\ContainerAdministrator\AppData\Local\Microsoft\WindowsApps
PATHEXT=.COM;.EXE;.BAT;.CMD
PROCESSOR_ARCHITECTURE=AMD64
PROCESSOR_IDENTIFIER=Intel64 Family 6 Model 62 Stepping 4, GenuineIntel
PROCESSOR_LEVEL=6
PROCESSOR_REVISION=3e04
ProgramData=C:\ProgramData
ProgramFiles=C:\Program Files
ProgramFiles(x86)=C:\Program Files (x86)
ProgramW6432=C:\Program Files
PROMPT=$P$G
PUBLIC=C:\Users\Public
SystemDrive=C:
SystemRoot=C:\Windows
TEMP=C:\Users\ContainerAdministrator\AppData\Local\Temp
TMP=C:\Users\ContainerAdministrator\AppData\Local\Temp
USERDOMAIN=User Manager
USERNAME=ContainerAdministrator
USERPROFILE=C:\Users\ContainerAdministrator
windir=C:\Windows

Similarly the operator can set the HOSTNAME (Linux) or COMPUTERNAME (Windows) with -h.

HEALTHCHECK🔗

  --health-cmd            Command to run to check health
  --health-interval       Time between running the check
  --health-retries        Consecutive failures needed to report unhealthy
  --health-timeout        Maximum time to allow one check to run
  --health-start-period   Start period for the container to initialize before starting health-retries countdown
  --no-healthcheck        Disable any container-specified HEALTHCHECK

Example:

$ docker run --name=test -d \
    --health-cmd='stat /etc/passwd || exit 1' \
    --health-interval=2s \
    busybox sleep 1d
$ sleep 2; docker inspect --format='{{.State.Health.Status}}' test
healthy
$ docker exec test rm /etc/passwd
$ sleep 2; docker inspect --format='{{json .State.Health}}' test
{
  "Status": "unhealthy",
  "FailingStreak": 3,
  "Log": [
    {
      "Start": "2016-05-25T17:22:04.635478668Z",
      "End": "2016-05-25T17:22:04.7272552Z",
      "ExitCode": 0,
      "Output": "  File: /etc/passwd\n  Size: 334       \tBlocks: 8          IO Block: 4096   regular file\nDevice: 32h/50d\tInode: 12          Links: 1\nAccess: (0664/-rw-rw-r--)  Uid: (    0/    root)   Gid: (    0/    root)\nAccess: 2015-12-05 22:05:32.000000000\nModify: 2015..."
    },
    {
      "Start": "2016-05-25T17:22:06.732900633Z",
      "End": "2016-05-25T17:22:06.822168935Z",
      "ExitCode": 0,
      "Output": "  File: /etc/passwd\n  Size: 334       \tBlocks: 8          IO Block: 4096   regular file\nDevice: 32h/50d\tInode: 12          Links: 1\nAccess: (0664/-rw-rw-r--)  Uid: (    0/    root)   Gid: (    0/    root)\nAccess: 2015-12-05 22:05:32.000000000\nModify: 2015..."
    },
    {
      "Start": "2016-05-25T17:22:08.823956535Z",
      "End": "2016-05-25T17:22:08.897359124Z",
      "ExitCode": 1,
      "Output": "stat: can't stat '/etc/passwd': No such file or directory\n"
    },
    {
      "Start": "2016-05-25T17:22:10.898802931Z",
      "End": "2016-05-25T17:22:10.969631866Z",
      "ExitCode": 1,
      "Output": "stat: can't stat '/etc/passwd': No such file or directory\n"
    },
    {
      "Start": "2016-05-25T17:22:12.971033523Z",
      "End": "2016-05-25T17:22:13.082015516Z",
      "ExitCode": 1,
      "Output": "stat: can't stat '/etc/passwd': No such file or directory\n"
    }
  ]
}


The health status is also displayed in the docker ps output.

TMPFS (mount tmpfs filesystems)🔗

--tmpfs=[]: Create a tmpfs mount with: container-dir[:<options>],
            where the options are identical to the Linux
            'mount -t tmpfs -o' command.

The example below mounts an empty tmpfs into the container with the rw, noexec, nosuid, and size=65536k options.

$ docker run -d --tmpfs /run:rw,noexec,nosuid,size=65536k my_image

VOLUME (shared filesystems)🔗

-v, --volume=[host-src:]container-dest[:<options>]: Bind mount a volume.
The comma-delimited `options` are [rw|ro], [z|Z],
[[r]shared|[r]slave|[r]private], and [nocopy].
The 'host-src' is an absolute path or a name value.

If neither 'rw' or 'ro' is specified then the volume is mounted in
read-write mode.

The `nocopy` mode is used to disable automatically copying the requested volume
path in the container to the volume storage location.
For named volumes, `copy` is the default mode. Copy modes are not supported
for bind-mounted volumes.

--volumes-from="": Mount all volumes from the given container(s)

    Note

    When using systemd to manage the Docker daemon’s start and stop, in the systemd unit file there is an option to control mount propagation for the Docker daemon itself,
    called MountFlags. The value of this setting may cause Docker to not see mount propagation changes made on the mount point. For example, if this value is slave, you may
    not be able to use the shared or rshared propagation on a volume.

The volumes commands are complex enough to have their own documentation in section Use volumes. A developer can define one or more VOLUME’s associated with an image, but
only the operator can give access from one container to another (or from a container to a volume mounted on the host).

The container-dest must always be an absolute path such as /src/docs. The host-src can either be an absolute path or a name value. If you supply an absolute path for the
host-dir, Docker bind-mounts to the path you specify. If you supply a name, Docker creates a named volume by that name.

A name value must start with an alphanumeric character, followed by a-z0-9, _ (underscore), . (period) or - (hyphen). An absolute path starts with a / (forward slash).

For example, you can specify either /foo or foo for a host-src value. If you supply the /foo value, Docker creates a bind mount. If you supply the foo specification, Docker
creates a named volume.

USER🔗

root (id = 0) is the default user within a container. The image developer can create additional users. Those users are accessible by name. When passing a numeric ID, the
user does not have to exist in the container.

开发人员可以设置默认用户，以使用Dockerfile USER指令运行第一个进程。启动容器时，操作员可以USER通过传递-u选项来覆盖指令。

-u="", --user="": Sets the username or UID used and optionally the groupname or GID for the specified command.

The followings examples are all valid:
--user=[ user | user:group | uid | uid:gid | user:gid | uid:group ]

    注意：如果您传递数字uid，则它必须在0-2147483647的范围内。

WORKDIR 🔗

在容器中运行二进制文件的默认工作目录是根目录（/），但是开发人员可以使用Dockerfile WORKDIR命令设置其他默认目录。操作员可以使用以下方法覆盖它：

-w="": Working directory inside the container

码头工人，运行，配置，运行时
评价此页面：

 
409
 
124
 
一世
 

  • 家
  • 导游
  • 产品手册
  • 参考
  • 样品

  • 命令行参考
      □ Docker CLI（码头工人）
          ☆ Docker运行参考
          ☆ 使用Docker命令行
          ☆ 泊坞窗（基本命令）
          ☆ 码头工人应用
              ○ 码头工人应用
              ○ docker应用程序捆绑
              ○ Docker应用完成
              ○ docker应用程序初始化
              ○ 码头工人应用检查
              ○ docker应用安装
              ○ docker应用列表
              ○ 码头工人应用程序合并
              ○ 泊坞窗应用程序拉
              ○ docker应用推送
              ○ docker应用渲染
              ○ docker应用拆分
              ○ Docker应用状态
              ○ docker应用卸载
              ○ 码头工人应用程序升级
              ○ 码头工人应用程序验证
              ○ Docker应用程式版本
          ☆ 码头工人附加
          ☆ 码头工人
          ☆ 码头工人
              ○ 码头工人
              ○ 码头工人建造
              ○ docker builder prune
          ☆ docker buildx
              ○ docker buildx
              ○ docker buildx bake
              ○ docker buildx build
              ○ docker buildx create
              ○ docker buildx imagetools
              ○ docker buildx imagetools create
              ○ docker buildx imagetools inspect
              ○ docker buildx inspect
              ○ docker buildx ls
              ○ docker buildx rm
              ○ docker buildx stop
              ○ docker buildx use
              ○ docker buildx version
          ☆ 码头检查站
              ○ docker checkpoint
              ○ docker checkpoint create
              ○ docker checkpoint ls
              ○ docker checkpoint rm
          ☆ 码头工人集群
              ○ docker cluster
              ○ docker cluster backup
              ○ docker cluster create
              ○ docker cluster inspect
              ○ docker cluster ls
              ○ docker cluster restore
              ○ docker cluster rm
              ○ docker cluster update
              ○ docker cluster version
          ☆ 码头工人提交
          ☆ docker配置
              ○ docker config
              ○ docker config create
              ○ docker config inspect
              ○ docker config ls
              ○ docker config rm
          ☆ 码头集装箱
              ○ docker container
              ○ docker container attach
              ○ docker container commit
              ○ docker container cp
              ○ docker container create
              ○ docker container diff
              ○ docker container exec
              ○ docker container export
              ○ docker container inspect
              ○ docker container kill
              ○ docker container logs
              ○ docker container ls
              ○ docker container pause
              ○ docker container port
              ○ docker container prune
              ○ docker container rename
              ○ docker container restart
              ○ docker container rm
              ○ docker container run
              ○ docker container start
              ○ docker container stats
              ○ docker container stop
              ○ docker container top
              ○ docker container unpause
              ○ docker container update
              ○ docker container wait
          ☆ 码头工人上下文
              ○ docker context
              ○ docker context create
              ○ docker context export
              ○ docker context import
              ○ docker context inspect
              ○ docker context ls
              ○ docker context rm
              ○ docker context update
              ○ docker context use
          ☆ 码头工人cp
          ☆ 码头工人创建
          ☆ docker差异
          ☆ 码头工人事件
          ☆ 码头工人
          ☆ 码头工人出口
          ☆ 码头工人的历史
          ☆ 码头工人形象
              ○ docker image
              ○ docker image build
              ○ docker image history
              ○ docker image import
              ○ docker image inspect
              ○ docker image load
              ○ docker image ls
              ○ docker image prune
              ○ docker image pull
              ○ docker image push
              ○ docker image rm
              ○ docker image save
              ○ docker image tag
          ☆ 码头工人图像
          ☆ 码头工人进口
          ☆ 码头工人信息
          ☆ 码头工人检查
          ☆ 码头工人杀
          ☆ 码头工人负载
          ☆ 码头工人登录
          ☆ 码头工人登出
          ☆ 码头工人日志
          ☆ 码头工人清单
              ○ docker manifest
              ○ docker manifest annotate
              ○ docker manifest create
              ○ docker manifest inspect
              ○ docker manifest push
          ☆ 码头工人网络
              ○ docker network
              ○ docker network connect
              ○ docker network create
              ○ docker network disconnect
              ○ docker network inspect
              ○ docker network ls
              ○ docker network prune
              ○ docker network rm
          ☆ 码头工人节点
              ○ docker node
              ○ docker node demote
              ○ docker node inspect
              ○ docker node ls
              ○ docker node promote
              ○ docker node ps
              ○ docker node rm
              ○ docker node update
          ☆ 码头工人暂停
          ☆ 码头工人插件
              ○ docker plugin
              ○ docker plugin create
              ○ docker plugin disable
              ○ docker plugin enable
              ○ docker plugin inspect
              ○ docker plugin install
              ○ docker plugin ls
              ○ docker plugin rm
              ○ docker plugin set
              ○ docker plugin upgrade
          ☆ 码头工人端口
          ☆ 码头工人ps
          ☆ 码头工人拉
          ☆ 码头工人推
          ☆ 码头工人注册表
              ○ docker registry
              ○ docker registry events
              ○ docker registry history
              ○ docker registry info
              ○ docker registry inspect
              ○ docker registry joblogs
              ○ docker registry ls
              ○ docker registry rmi
          ☆ 码头工人重命名
          ☆ docker重启
          ☆ 码头工人
          ☆ 码头工人rmi
          ☆ docker运行
          ☆ 码头工人保存
          ☆ 码头工人搜索
          ☆ 码头工人的秘密
              ○ docker secret
              ○ docker secret create
              ○ docker secret inspect
              ○ docker secret ls
              ○ docker secret rm
          ☆ 码头工人服务
              ○ docker service
              ○ docker service create
              ○ docker service inspect
              ○ docker service logs
              ○ docker service ls
              ○ docker service ps
              ○ docker service rollback
              ○ docker service rm
              ○ docker service scale
              ○ docker service update
          ☆ 码头工人堆栈
              ○ docker stack
              ○ docker stack deploy
              ○ docker stack ps
              ○ docker stack rm
              ○ docker stack services
          ☆ 码头工人开始
          ☆ 码头工人统计
          ☆ 码头工人停止
          ☆ 码头工人
              ○ docker swarm
              ○ docker swarm ca
              ○ docker swarm init
              ○ docker swarm join-token
              ○ docker swarm join
              ○ docker swarm leave
              ○ docker swarm unlock-key
              ○ docker swarm unlock
              ○ docker swarm update
          ☆ 码头工人系统
              ○ docker system
              ○ docker system df
              ○ docker system events
              ○ docker system info
              ○ docker system prune
          ☆ 码头工人标签
          ☆ 码头工人
          ☆ 码头工人信任
              ○ docker trust
              ○ docker trust inspect
              ○ docker trust key
              ○ docker trust key generate
              ○ docker trust key load
              ○ docker trust revoke
              ○ docker trust sign
              ○ docker trust signer
              ○ docker trust signer add
              ○ docker trust signer remove
          ☆ 码头工人取消暂停
          ☆ 码头工人更新
          ☆ 码头工人版本
          ☆ 码头工人卷
              ○ docker volume create
              ○ docker volume inspect
              ○ docker volume ls
              ○ docker volume prune
              ○ docker volume rm
          ☆ 码头工人等待
      □ Docker Compose CLI参考
          ☆ Overview of docker-compose CLI
          ☆ CLI environment variables
          ☆ Command-line completion
          ☆ docker-compose build
          ☆ docker-compose config
          ☆ docker-compose create
          ☆ docker-compose down
          ☆ docker-compose events
          ☆ docker-compose exec
          ☆ docker-compose help
          ☆ docker-compose kill
          ☆ docker-compose logs
          ☆ docker-compose pause
          ☆ docker-compose port
          ☆ docker-compose ps
          ☆ docker-compose pull
          ☆ docker-compose push
          ☆ docker-compose restart
          ☆ docker-compose rm
          ☆ docker-compose run
          ☆ docker-compose scale
          ☆ docker-compose start
          ☆ docker-compose stop
          ☆ docker-compose top
          ☆ docker-compose unpause
          ☆ docker-compose up
      □ 守护程序CLI（dockerd）
  • API参考
      □ Docker Engine API
          ☆ Overview
          ☆ SDKs
          ☆ SDK examples
          ☆ v1.40 reference (latest)
          ☆ API reference by version
              ○ Version history overview
              ○ v1.40 reference
              ○ v1.39 reference
              ○ v1.38 reference
              ○ v1.37 reference
              ○ v1.36 reference
              ○ v1.35 reference
              ○ v1.34 reference
              ○ v1.33 reference
              ○ v1.32 Reference
              ○ v1.31 Reference
              ○ v1.30 Reference
              ○ v1.29 Reference
              ○ v1.28 reference
              ○ v1.27 reference
              ○ v1.26 reference
              ○ v1.25 reference
              ○ v1.24 reference
              ○ v1.23 reference
              ○ v1.22 reference
              ○ v1.21 reference
              ○ v1.20 reference
              ○ v1.19 reference
              ○ v1.18 reference
      □ Registry API
  • Dockerfile参考
  • 撰写文件参考
      □ Version 3
      □ Version 2
      □ Version 1
      □ About versions and upgrading
      □ Frequently asked questions
  • 驱动程序和规格
      □ Registry image manifests
          ☆ Image manifest v 2, schema 1
          ☆ Image manifest v 2, schema 2
          ☆ Update deprecated schema v1 images
      □ Registry token authorization
          ☆ Docker Registry token authentication
          ☆ Token authentication implementation
          ☆ Oauth2 token authentication
          ☆ Token scope documentation
          ☆ Token authentication specification
      □ Registry storage drivers
          ☆ Storage driver overview
          ☆ Aliyun OSS storage driver
          ☆ Filesystem storage driver
          ☆ GCS storage driver
          ☆ In-memory storage driver
          ☆ Microsoft Azure storage driver
          ☆ S3 storage driver
          ☆ Swift storage driver
  • 词汇表

  • 编辑这个页面
  • 请求文档更改
  • 
    [ ]

在本页面：

  • 一般形式
  • 运营商专有选项
  • 独立式vs前景式
      □ 分离（-d）
      □ 前景
  • 容器识别
      □ 名称（--name）
      □ PID当量
      □ 图片[：tag]
      □ 图片[@digest]
  • PID设置（--pid）
      □ 示例：在容器中运行htop
      □ 例
  • UTS设置（--uts）
  • IPC设置（--ipc）
  • 网络设置
      □ 管理/ etc / hosts
  • 重新启动策略（-重新启动）
      □ 例子
  • 退出状态
  • 清理（​​--rm）
  • 安全配置
  • 指定初始化过程
  • 指定自定义cgroup
  • 资源的运行时限制
      □ 用户内存限制
      □ 内核内存限制
      □ 湿度约束
      □ CPU份额限制
      □ CPU周期限制
      □ Cpuset约束
      □ CPU配额限制
      □ 块IO带宽（Blkio）约束
  • 其他组
  • 运行时特权和Linux功能
  • 日志驱动程序（--log-driver）
  • 覆盖Dockerfile映像默认值
      □ CMD（默认命令或选项）
      □ ENTRYPOINT（在运行时执行的默认命令）
      □ 暴露（入港）
      □ ENV（环境变量）
      □ 健康检查
      □ TMPFS（挂载tmpfs文件系统）
      □ VOLUME（共享文件系统）
      □ 用户
      □ 工作目录

  • 为什么选择Docker？
  • 什么是容器？

  • 产品展示
  • Docker桌面
  • Docker中心
  • 特征
  • 容器运行时
  • 开发者工具
  • Kubernetes

  • 开发者
  • 用例
  • 玩Docker
  • 社区
  • 开源的
  • Docker队长

  • 公司
  • 关于我们
  • 博客
  • 顾客
  • 伙伴
  • 编辑部
  • 招贤纳士
  • 联系我们

  • 状态
  • 安全
  • 法律
  • 联系

版权所有©2013-2020 Docker Inc.保留所有权利。

  • 推特
  • 优酷
  • 的GitHub
  • 领英
  • 脸书
  • 幻灯片分享
  • Reddit

Google 翻译

原文

提供更好的翻译建议
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
[ipv]
