# Dockerfile参考
> 预计阅读时间： 79分钟

Docker可以通过阅读Docker的指令来自动构建映像 Dockerfile。A Dockerfile是一个文本文档，
其中包含用户可以在命令行上调用以组装图像的所有命令。使用docker build 用户可以创建一个
自动构建，该构建连续执行多个命令行指令。

本页描述您可以在中使用的命令Dockerfile。阅读完此页面后，请参考Dockerfile最佳实践以获取
有关技巧的指南。

用法🔗

的搬运工构建命令从一个构建的图像Dockerfile和一个上下文。构建的上下文是指定位置PATH或的
文件集URL。这PATH是本地文件系统上的目录。该URL是一个Git仓库的位置。

上下文是递归处理的。因此，a PATH包括任何子目录，并且a 包括URL存储库及其子模块。此示例
显示了一个使用当前目录作为上下文的构建命令：

$ docker build .

Sending build context to Docker daemon  6.51 MB
...

构建是由Docker守护程序而不是CLI运行的。生成过程要做的第一件事是将整个上下文（递归）发
送到守护程序。在大多数情况下，最好从空目录开始作为上下文，并将Dockerfile保留在该目录中
。仅添加构建Dockerfile所需的文件。

    警告

    不要用你的根目录下，/作为PATH因为它会导致生成到您的硬盘驱动器的全部内容传输到码头
    工人守护进程。

要在构建上下文中使用文件，Dockerfile引用是指指令（例如，COPY指令）中指定的文件。要提高
构建的性能，请通过将.dockerignore文件添加到上下文目录来排除文件和目录。有关如何创
建.dockerignore 文件的信息，请参阅此页面上的文档。

传统上，Dockerfile称为Dockerfile并位于上下文的根中。您可以使用-f标志with docker build
指向文件系统中任何位置的Dockerfile。

$ docker build -f /path/to/a/Dockerfile .

如果构建成功，则可以指定一个存储库和标记，用于在其中存储新图像：

$ docker build -t shykes/myapp .

要在构建后将映像标记到多个存储库中，请在-t运行build命令时添加多个参数：

$ docker build -t shykes/myapp:1.0.2 -t shykes/myapp:latest .

在Docker守护程序运行中的指令之前Dockerfile，它会对进行初步验证，Dockerfile如果语法不正
确，则返回错误：

$ docker build -t test/myapp .

Sending build context to Docker daemon 2.048 kB
Error response from daemon: Unknown instruction: RUNCMD

Docker守护程序以Dockerfile一对一的方式运行指令，如有必要，将每条指令的结果提交到新映像
，然后最终输出新映像的ID。Docker守护程序将自动清理您发送的上下文。

请注意，每条指令都是独立运行的，并会导致创建新映像-因此RUN cd /tmp对下一条指令不会有任
何影响。

Docker将尽可能重用中间映像（缓存），以docker build显着加速该过程。这由Using cache控制
台输出中的消息指示。（有关详细信息，请参阅Dockerfile最佳做法指南：

$ docker build -t svendowideit/ambassador .

Sending build context to Docker daemon 15.36 kB
Step 1/4 : FROM alpine:3.2
 ---> 31f630c65071
Step 2/4 : MAINTAINER SvenDowideit@home.org.au
 ---> Using cache
 ---> 2a1c91448f5f
Step 3/4 : RUN apk update &&      apk add socat &&        rm -r /var/cache/
 ---> Using cache
 ---> 21ed6e7fbb73
Step 4/4 : CMD env | grep _TCP= | (sed 's/.*_PORT_\([0-9]*\)_TCP=tcp:\/\/\(.*\):\(.*\)/socat -t 100000000 TCP4-LISTEN:\1,fork,reuseaddr TCP4:\2:\3 \&/' && echo wait) | sh
 ---> Using cache
 ---> 7ea8aef582cc
Successfully built 7ea8aef582cc

构建缓存仅用于具有本地父链的映像。这意味着这些图像是由以前的版本创建的，或者整个图像链
都已加载docker load。如果您希望使用特定映像的构建缓存，则可以使用--cache-from选项指定
它。指定的图像 --cache-from不需要具有父链，并且可以从其他注册表中提取。

完成构建后，就可以开始研究将存储库推送到其注册表了。

BuildKit 🔗

从版本18.09开始，Docker支持由moby / buildkit 项目提供的用于执行构建的新后端。与旧的实
现相比，BuildKit后端提供了许多好处。例如，BuildKit可以：

  • 检测并跳过执行未使用的构建阶段
  • 并行构建独立构建阶段
  • 两次构建之间仅增量传输构建上下文中的更改文件
  • 在构建上下文中检测并跳过传输未使用的文件
  • 使用具有许多新功能的外部Dockerfile实现
  • 避免与其他API产生副作用（中间图像和容器）
  • 优先考虑构建缓存以进行自动修剪

要使用BuildKit后端，您需要DOCKER_BUILDKIT=1在CLI上设置环境变量，然后再调用docker build
。

要了解基于BuildKit的构建可用的实验性Dockerfile语法，请参阅BuildKit存储库中的文档。

格式🔗

这是的格式Dockerfile：

# Comment
INSTRUCTION arguments

该指令不区分大小写。但是，约定将它们大写，以便更轻松地将它们与参数区分开。

Docker Dockerfile按顺序运行指令。一个Dockerfile 必须用`FROM`指令开始。这可能在解析器指
令，注释和全局范围的 ARG之后。该FROM指令指定要从中构建父图像。FROM只能在一个或多个ARG
指令之后，这些指令声明在中的FROM行中使用的参数Dockerfile。

该码头工人对待线开始以#作为注释，除非该行是一个有效的解析器指令。#一行中其他任何地方的
标记都被视为参数。这允许如下语句：

# Comment
RUN echo 'we are running some # of cool things'

注释中不支持换行符。

解析器指令🔗

解析器指令是可选的，并且会影响Dockerfile处理a 中后续行的方式。解析器指令不会在构建中添
加层，也不会显示为构建步骤。解析器指令以形式写为特殊类型的注释# directive=value。单个
指令只能使用一次。

处理完注释，空行或生成器指令后，Docker不再寻找解析器指令。而是将格式化为解析器指令的任
何内容都视为注释，并且不会尝试验证它是否可能是解析器指令。因此，所有解析器指令必须位于
的最顶部Dockerfile。

解析器指令不区分大小写。但是，约定是小写的。约定还应在任何解析器指令之后包含一个空白行
。解析器伪指令不支持行继续字符。

由于这些规则，以下示例均无效：

由于行继续而无效：

# direc \
tive=value

由于出现两次而无效：

# directive=value1
# directive=value2

FROM ImageName

由于出现在构建器指令之后而被视为注释：

FROM ImageName
# directive=value

由于出现在不是解析器指令的注释之后，因此被视为注释：

# About my dockerfile
# directive=value
FROM ImageName

由于未被识别，未知指令被视为注释。此外，由于在非解析程序指令的注释之后出现，因此已知指
令被视为注释。

# unknowndirective=value
# knowndirective=value

解析器指令中允许非换行空格。因此，以下各行都被相同地对待：

#directive=value
# directive =value
#       directive= value
# directive = value
#         dIrEcTiVe=value

支持以下解析器指令：

  • syntax
  • escape

语法🔗

# syntax=[remote image reference]

例如：

# syntax=docker/dockerfile
# syntax=docker/dockerfile:1.0
# syntax=docker.io/docker/dockerfile:1
# syntax=docker/dockerfile:1.0.0-experimental
# syntax=example.com/user/repo:tag@sha256:abcdef...

仅当使用BuildKit后端时才启用此功能。

语法指令定义用于构建当前Dockerfile的Dockerfile构建器的位置。BuildKit后端允许无缝使用构
建器的外部实现，这些构建器以Docker映像的形式分发并在容器沙箱环境中执行。

自定义Dockerfile实现使您能够：

  • 自动获取错误修正，无需更新守护程序
  • 确保所有用户都使用相同的实现来构建您的Dockerfile
  • 使用最新功能而不更新守护程序
  • 试用新的实验性或第三方功能

官方发布🔗

Docker分发映像的正式版本，这些映像可用于docker/dockerfile在Docker Hub上的存储库下构建
Dockerfile 。有两个发布新图像的渠道：稳定版和实验版。

稳定的通道遵循语义版本控制。例如：

  • docker/dockerfile:1.0.0 -仅允许不可变版本 1.0.0
  • docker/dockerfile:1.0 -允许版本 1.0.*
  • docker/dockerfile:1 -允许版本 1.*.*
  • docker/dockerfile:latest -稳定频道上的最新版本

在发布时，实验频道使用稳定版本中主要和次要组件的增量版本控制。例如：

  • docker/dockerfile:1.0.1-experimental -仅允许不可变版本 1.0.1-experimental
  • docker/dockerfile:1.0-experimental -之后的最新实验版本 1.0
  • docker/dockerfile:experimental -实验频道的最新版本

您应该选择最适合自己需求的渠道。如果您只想修正错误，则应使用docker/dockerfile:1.0。如
果您想从实验功能中受益，则应使用实验频道。如果您正在使用实验性频道，则较新的版本可能无
法向后兼容，因此建议使用不可变的完整版本。

有关主版本和每晚发布的功能，请参考源存储库中的描述。

逃脱🔗

# escape=\ (backslash)

要么

# escape=` (backtick)

该escape指令设置用来逃避的字符的字符 Dockerfile。如果未指定，则默认转义字符为\。

转义字符用于转义一行中的字符和转义换行符。这允许一条Dockerfile指令跨越多行。请注意，无
论escapeparser指令是否包含在Dockerfile，都不会在RUN命令中执行转义，除非在行末。

将转义符设置`为尤其有用 Windows，其中\目录路径分隔符为。`与Windows PowerShell一致。

考虑以下示例，该示例将以非显而易见的方式失败 Windows。第二\，在第二行的端部将被解释为
用于换行的逃逸，而不是从第一逸出的目标\。类似地，\假设第三行的末尾实际上被当作一条指令
来处理，则将其视为行的继续。该dockerfile的结果是第二和第三行被视为一条指令：

FROM microsoft/nanoserver
COPY testfile.txt c:\\
RUN dir c:\

结果是：

PS C:\John> docker build -t cmd .
Sending build context to Docker daemon 3.072 kB
Step 1/2 : FROM microsoft/nanoserver
 ---> 22738ff49c6d
Step 2/2 : COPY testfile.txt c:\RUN dir c:
GetFileAttributesEx c:RUN: The system cannot find the file specified.
PS C:\John>

上述解决方案之一是将和和/用作目标。但是，此语法充其量是令人困惑的，因为它对上的路径来
说是不自然的，而在最坏的情况下，由于并非所有支持的命令都作为路径分隔符，因此容易出错。
COPYdirWindowsWindows/

通过添加escape解析器指令，通过对以下Dockerfile文件路径使用自然平台语义，以下操作可以按
预期成功Windows：

# escape=`

FROM microsoft/nanoserver
COPY testfile.txt c:\
RUN dir c:\

结果是：

PS C:\John> docker build -t succeeds --no-cache=true .
Sending build context to Docker daemon 3.072 kB
Step 1/3 : FROM microsoft/nanoserver
 ---> 22738ff49c6d
Step 2/3 : COPY testfile.txt c:\
 ---> 96655de338de
Removing intermediate container 4db9acbb1682
Step 3/3 : RUN dir c:\
 ---> Running in a2c157f842f5
 Volume in drive C has no label.
 Volume Serial Number is 7E6D-E0F7

 Directory of c:\

10/05/2016  05:04 PM             1,894 License.txt
10/05/2016  02:22 PM    <DIR>          Program Files
10/05/2016  02:14 PM    <DIR>          Program Files (x86)
10/28/2016  11:18 AM                62 testfile.txt
10/28/2016  11:20 AM    <DIR>          Users
10/28/2016  11:20 AM    <DIR>          Windows
           2 File(s)          1,956 bytes
           4 Dir(s)  21,259,096,064 bytes free
 ---> 01c7f3bef04f
Removing intermediate container a2c157f842f5
Successfully built 01c7f3bef04f
PS C:\John>

更换环境🔗

环境变量（与声明的ENV声明），也可以在特定指令作为变量用来被解释 Dockerfile。转义也可以
通过将类变量的语法包括在语句中而得到处理。

环境变量Dockerfile用 $variable_name或表示${variable_name}。它们被同等对待，并且大括号
语法通常用于解决变量名没有空格的问题，例如${foo}_bar。

该${variable_name}语法还支持以下一些标准bash 修饰符：

  • ${variable:-word}表示如果variable设置，则结果将是该值。如果variable未设置，则为
    word结果。
  • ${variable:+word}指示如果variable设置了if，则将是word结果，否则结果为空字符串。

在所有情况下，word都可以是任何字符串，包括其他环境变量。

可通过\在变量前添加a来进行转义：例如，\$foo或\${foo}将分别转换为$foo和${foo}文字。

示例（解析的表示形式显示在之后#）：

FROM busybox
ENV foo /bar
WORKDIR ${foo}   # WORKDIR /bar
ADD . $foo       # ADD . /bar
COPY \$foo /quux # COPY $foo /quux

以下变量中的指令列表支持环境变量Dockerfile：

  • ADD
  • COPY
  • ENV
  • EXPOSE
  • FROM
  • LABEL
  • STOPSIGNAL
  • USER
  • VOLUME
  • WORKDIR
  • ONBUILD （当与以上支持的说明之一结合使用时）

在整个指令中，环境变量替换将对每个变量使用相同的值。换句话说，在此示例中：

ENV abc=hello
ENV abc=bye def=$abc
ENV ghi=$abc

将导致def值为hello，而不是bye。但是， ghi将具有的值，bye因为它不是设置abc为的同一指令
的一部分bye。

.dockerignore文件🔗

在docker CLI将上下文发送到docker守护程序之前，它会.dockerignore在上下文的根目录中查找
名为的文件。如果此文件存在，则CLI会修改上下文以排除与其中的模式匹配的文件和目录。这有
助于避免不必要地将大型文件或敏感文件和目录发送到守护程序，并避免使用ADD或将它们添加到
映像中COPY。

CLI将.dockerignore文件解释为以换行符分隔的模式列表，类似于Unix Shell的文件组。为了匹配
，上下文的根被认为是工作目录和根目录。例如，图案 /foo/bar和foo/bar两个排除命名的文件或
目录bar 的foo子目录PATH或位于Git仓库的根URL。都不排除其他任何东西。

如果.dockerignore文件中的一行以第#1列开头，则该行将被视为注释，并且在CLI解释之前将被忽
略。

这是一个示例.dockerignore文件：

# comment
*/temp*
*/*/temp*
temp?

此文件导致以下生成行为：

 规则                                        行为
#       忽略了。
comment
*/temp* 排除名称以temp根的任何直接子目录开头的文件和目录。例如，/somedir/temporary.txt
        排除纯文件，排除目录/somedir/temp。
*/*/    排除temp从根以下两个级别的任何子目录开始的文件和目录。例如，/somedir/subdir/
temp*   temporary.txt被排除。
temp?   排除名称为的一个字符扩展名的根目录中的文件和目录temp。例如，/tempa和/tempb被排
        除。

使用Go的filepath.Match规则进行匹配。预处理步骤使用Go的filepath.Clean除去开头和结尾的空
格，并消除.和..元素。预处理后空白的行将被忽略。 

除了Go的filepath.Match规则外，Docker还支持一个特殊的通配符字符串**，该字符串可匹配任意
数量的目录（包括零个）。例如，**/*.go将排除.go 在所有目录（包括构建上下文的根目录）中
找到的所有结尾的文件。

以!（感叹号）开头的行可用于排除例外。以下是.dockerignore使用此机制的示例文件：

*.md
!README.md

除 README.md上下文外，所有降价文件都排除在外。

!异常规则的位置会影响行为：.dockerignore匹配特定文件的的最后一行确定是包含还是排除该文
件。考虑以下示例：

*.md
!README*.md
README-secret.md

除了以外的自述文件，上下文中没有markdown文件 README-secret.md。

现在考虑以下示例：

*.md
README-secret.md
!README*.md

包括所有自述文件。中线无效，因为 !README*.md匹配README-secret.md并排在最后。

您甚至可以使用该.dockerignore文件排除Dockerfile 和.dockerignore文件。这些文件仍被发送
到守护程序，因为它需要它们来完成其工作。但是ADD和COPY指令不会将它们复制到映像中。

最后，您可能想要指定要包含在上下文中的文件，而不是要排除的文件。为此，请指定*为第一个
模式，然后指定一个或多个!异常模式。

    注意

    由于历史原因，该模式.被忽略。

来自🔗

FROM [--platform=<platform>] <image> [AS <name>]

要么

FROM [--platform=<platform>] <image>[:<tag>] [AS <name>]

要么

FROM [--platform=<platform>] <image>[@<digest>] [AS <name>]

该FROM指令初始化一个新的构建阶段，并为后续指令设置基本映像。因此，有效的Dockerfile必须
从FROM指令开始。该图像可以是任何有效图像- 从公共存储库中拉出图像特别容易启动。 

  • ARG是先于仅指示FROM在Dockerfile。请参阅了解ARG和FROM之间的相互作用。
  • FROM可以一次出现多次Dockerfile以创建多个映像，也可以将一个构建阶段作为对另一个构建
    阶段的依赖。只需在每个新FROM指令之前记录一次提交输出的最后一个图像ID 。每个FROM指
    令清除由先前指令创建的任何状态。
  • 可选的名称可以通过添加给予一个新的构建阶段AS name的 FROM指令。该名称可以在后续版本
    FROM和 COPY --from=<name|index>说明中使用，以引用此阶段中构建的映像。
  • 该tag或digest值是可选的。如果您忽略其中任何一个latest，那么缺省情况下，构建器将采
    用标签。如果构建器找不到该tag值，则返回错误。

--platform在FROM引用多平台图像的情况下，可选标志可用于指定图像的平台。例如，linux/
amd64， linux/arm64，或windows/amd64。默认情况下，使用构建请求的目标平台。可以在此标志
的值中使用全局构建参数，例如，自动平台ARG 允许您将阶段强制为本机构建平台（--platform=
$BUILDPLATFORM），并使用它来交叉编译到阶段内部的目标平台。

了解ARG和FROM之间的交互方式🔗

FROM指令支持由ARG 第一指令之前的任何指令声明的变量FROM。

ARG  CODE_VERSION=latest
FROM base:${CODE_VERSION}
CMD  /code/run-app

FROM extras:${CODE_VERSION}
CMD  /code/run-extras

ARG在a之前的声明FROM位于构建阶段之外，因此在a之后的任何指令中都不能使用它FROM。要ARG在
第一次FROM使用声明之前使用默认值，请在ARG构建阶段使用没有值的指令：

ARG VERSION=latest
FROM busybox:$VERSION
ARG VERSION
RUN echo $VERSION > image_version

运行🔗

RUN有2种形式：

  • RUN <command>（shell形式，命令在shell中运行，默认情况下/bin/sh -c在Linux或cmd /S /
    CWindows 上运行）
  • RUN ["executable", "param1", "param2"]（执行表格）

该RUN指令将在当前图像顶部的新层中执行所有命令，并提交结果。生成的提交图像将用于中的下
一步Dockerfile。

分层RUN指令和生成提交符合Docker的核心概念，在这些概念上提交很便宜，并且可以从映像历史
记录的任何位置创建容器，就像源代码控制一样。

在EXEC形式使得能够避免壳串改写（munging），并RUN 使用不包含指定壳可执行基本图像的命令
。

可以使用以下命令更改shell形式的默认shell SHELL。

在shell形式中，您可以使用\（反斜杠）将一条RUN指令继续到下一行。例如，考虑以下两行：

RUN /bin/bash -c 'source $HOME/.bashrc; \
echo $HOME'

它们合起来相当于以下一行：

RUN /bin/bash -c 'source $HOME/.bashrc; echo $HOME'

要使用'/ bin / sh'以外的其他shell，请使用exec形式传入所需的shell。例如：

RUN ["/bin/bash", "-c", "echo hello"]

    注意

    在EXEC形式被解析为一个JSON阵列，这意味着必须使用双引号（“）周围的话不单引号（'）。

与shell表单不同，exec表单不会调用命令shell。这意味着不会发生常规的外壳处理。例如， RUN
[ "echo", "$HOME" ]将不会对进行变量替换$HOME。如果要进行shell处理，则可以使用shell形式
或直接执行shell，例如：RUN [ "sh", "-c", "echo $HOME" ]。当使用exec表单并直接执行shell
时（例如在shell表单中），是由shell进行环境变量扩展，而不是docker。

    注意

    在JSON格式中，必须转义反斜杠。这在Windows中特别有用，在Windows中反斜杠是路径分隔符
    。由于无效的JSON，以下行否则将被视为shell形式，并以意外的方式失败：

    RUN ["c:\windows\system32\tasklist.exe"]

    此示例的正确语法为：

    RUN ["c:\\windows\\system32\\tasklist.exe"]

RUN下一次构建期间，指令缓存不会自动失效。类似指令的缓存 RUN apt-get dist-upgrade -y将
在下一个构建中重用。RUN指令的缓存可以通过使用--no-cache 标志来使无效，例如docker build
--no-cache。

有关更多信息，请参见Dockerfile最佳实践指南。

RUN指令的缓存可以被ADD和COPY指令无效。

已知问题（运行）🔗

  • 问题783是有关使用AUFS文件系统时可能发生的文件权限问题。例如，您在尝试rm文件过程中
    可能会注意到它。

    对于具有最新aufs版本的系统（即，dirperm1可以设置安装选项），docker将尝试通过使用
    options挂载层来尝试自动修复问题dirperm1。有关dirperm1选项的更多详细信息，请参见
    aufs手册页

    如果您的系统不支持dirperm1，则该问题描述了一种解决方法。

CMD 🔗

该CMD指令具有三种形式：

  • CMD ["executable","param1","param2"]（exec形式，这是首选形式）
  • CMD ["param1","param2"]（作为ENTRYPOINT的默认参数）
  • CMD command param1 param2（外壳形式）

CMD指令中只能有一条指令Dockerfile。如果您列出多个，CMD 则只有最后一个CMD才会生效。

a的主要目的CMD是为执行中的容器提供默认值。这些默认值可以包含一个可执行文件，也可以忽略
该可执行文件，在这种情况下，您还必须指定一条ENTRYPOINT 指令。

如果CMD用于为ENTRYPOINT指令提供默认参数，则CMD和ENTRYPOINT指令均应使用JSON数组格式指定
。

    注意

    在EXEC形式被解析为一个JSON阵列，这意味着必须使用双引号（“）周围的话不单引号（'）。

与shell表单不同，exec表单不会调用命令shell。这意味着不会发生常规的外壳处理。例如， CMD
[ "echo", "$HOME" ]将不会对进行变量替换$HOME。如果要进行shell处理，则可以使用shell形式
或直接执行shell，例如：CMD [ "sh", "-c", "echo $HOME" ]。当使用exec表单并直接执行shell
时（例如在shell表单中），是由shell进行环境变量扩展，而不是docker。

当以shell或exec格式使用时，该CMD指令设置运行映像时要执行的命令。

如果您使用的外壳形式CMD，则将在中<command>执行 /bin/sh -c：

FROM ubuntu
CMD echo "This is a test." | wc -

如果要在 <command> 没有外壳的情况下运行，则必须将命令表示为JSON数组，并提供可执行文件
的完整路径。此数组形式是的首选格式CMD。任何其他参数必须在数组中分别表示为字符串：

FROM ubuntu
CMD ["/usr/bin/wc","--help"]

如果您希望容器每次都运行相同的可执行文件，则应考虑ENTRYPOINT与结合使用CMD。请参阅
ENTRYPOINT。

如果用户指定的参数，docker run则它们将覆盖中指定的默认参数CMD。

    注意

    不要RUN与混淆CMD。RUN实际上运行命令并提交结果；CMD在生成时不执行任何操作，但是指定
    映像的预期命令。

标签🔗

LABEL <key>=<value> <key>=<value> <key>=<value> ...

该LABEL指令将元数据添加到图像。A LABEL是键值对。要在LABEL值中包含空格，请像在命令行分
析中一样使用引号和反斜杠。一些用法示例：

LABEL "com.example.vendor"="ACME Incorporated"
LABEL com.example.label-with-value="foo"
LABEL version="1.0"
LABEL description="This text illustrates \
that label-values can span multiple lines."

一幅图像可以有多个标签。您可以在一行上指定多个标签。在Docker 1.10之前的版本中，这减小
了最终映像的大小，但是情况不再如此。您仍然可以通过以下两种方式之一选择在一条指令中指定
多个标签：

LABEL multi.label1="value1" multi.label2="value2" other="value3"

LABEL multi.label1="value1" \
      multi.label2="value2" \
      other="value3"

基本或父图像（该FROM行中的图像）中包含的标签由您的图像继承。如果标签已经存在但具有不同
的值，则最近应用的值将覆盖任何先前设置的值。

要查看图像的标签，请使用docker image inspect命令。您可以使用该--format选项仅显示标签。

docker image inspect --format='' myimage

{
  "com.example.vendor": "ACME Incorporated",
  "com.example.label-with-value": "foo",
  "version": "1.0",
  "description": "This text illustrates that label-values can span multiple lines.",
  "multi.label1": "value1",
  "multi.label2": "value2",
  "other": "value3"
}

维护者（已弃用）🔗

MAINTAINER <name>

该MAINTAINER指令设置生成图像的作者字段。该LABEL指令是此版本的灵活得多，您应该改用它，
因为它可以设置所需的任何元数据，并且可以轻松查看，例如使用docker inspect。要设置与该
MAINTAINER字段对应的标签，可以使用：

LABEL maintainer="SvenDowideit@home.org.au"

这样docker inspect，其他标签就可以显示出来了。

暴露🔗

EXPOSE <port> [<port>/<protocol>...]

该EXPOSE指令通知Docker容器在运行时监听指定的网络端口。您可以指定端口是侦听TCP还是UDP，
如果未指定协议，则默认值为TCP。

该EXPOSE指令实际上并未发布端口。它充当构建映像的人员和运行容器的人员之间的一种文档类型
，有关打算发布哪些端口的信息。要实际运行的容器中时发布的端口，使用-p国旗docker run 发
布和映射一个或多个端口，或者使用-P标记发布所有公开的端口并将它们映射到高阶端口。

默认情况下，EXPOSE假定为TCP。您还可以指定UDP：

EXPOSE 80/udp

要同时在TCP和UDP上公开，请包括以下两行：

EXPOSE 80/tcp
EXPOSE 80/udp

在这种情况下，如果-P与配合使用docker run，则该端口仅对TCP公开一次，对于UDP公开一次。请
记住，-P该端口在主机上使用临时的高阶主机端口，因此该端口对于TCP和UDP将是不同的。

无论EXPOSE设置如何，都可以在运行时使用该-p标志覆盖它们。例如

docker run -p 80:80/tcp -p 80:80/udp ...

要在主机系统上设置端口重定向，请参阅使用-P标志。该docker network命令支持创建网络以在容
器之间进行通信，而无需暴露或发布特定端口，因为连接到网络的容器可以通过任何端口相互通信
。有关详细信息，请参见此功能概述。

环保V

ENV <key> <value>
ENV <key>=<value> ...

该ENV指令将环境变量<key>设置为value <value>。该值将在构建阶段中所有后续指令的环境中使
用，并且可以 在很多情况下内联替换。

该ENV指令有两种形式。第一种形式，ENV <key> <value>会将一个变量设置为一个值。第一个空格
之后的整个字符串将被视为<value>-包括空格字符。该值将为其他环境变量解释，因此如果不对引
号字符进行转义，则将其删除。

第二种形式ENV <key>=<value> ...允许一次设置多个变量。请注意，第二种形式在语法中使用等
号（=），而第一种形式则不使用等号（=）。与命令行分析一样，引号和反斜杠可用于在值中包含
空格。

例如：

ENV myName="John Doe" myDog=Rex\ The\ Dog \
    myCat=fluffy

和

ENV myName John Doe
ENV myDog Rex The Dog
ENV myCat fluffy

将在最终图像中产生相同的净结果。

ENV从结果映像运行容器时，使用设置的环境变量将保留。您可以使用查看值docker inspect，并
使用更改它们docker run --env <key>=<value>。

    注意

    环境持久性可能导致意外的副作用。例如，设置ENV DEBIAN_FRONTEND noninteractive可能会
    使基于Debian的映像上的apt-get用户感到困惑。要为单个命令设置值，请使用 RUN <key>=
    <value> <command>。

添加🔗

ADD有两种形式：

ADD [--chown=<user>:<group>] <src>... <dest>
ADD [--chown=<user>:<group>] ["<src>",... "<dest>"]

包含空格的路径需要后一种形式。

    注意

    该--chown功能仅在用于构建Linux容器的Dockerfiles上受支持，而在Windows容器上不起作用
    。由于用户和组所有权概念不会在Linux和Windows之间转换，因此使用/etc/passwd和/etc/
    group将用户名和组名转换为ID的使用限制了此功能仅适用于基于Linux OS的容器。

该ADD指令从中复制新文件，目录或远程文件URL <src> ，并将它们添加到路径中映像的文件系统
中<dest>。

<src>可以指定多个资源，但是如果它们是文件或目录，则将其路径解释为相对于构建上下文源的
路径。

每个都<src>可能包含通配符，并且匹配将使用Go的 filepath.Match规则完成。例如：

要添加所有以“ hom”开头的文件：

ADD hom* /mydir/

在下面的示例中，?被替换为任何单个字符，例如“ home.txt”。

ADD hom?.txt /mydir/

的<dest>是一个绝对路径，或相对于一个路径WORKDIR，到其中的源将在目标容器内进行复制。

下面的示例使用相对路径，并将“ test.txt”添加到<WORKDIR>/relativeDir/：

ADD test.txt relativeDir/

而此示例使用绝对路径，并向其中添加“ test.txt” /absoluteDir/

ADD test.txt /absoluteDir/

添加包含特殊字符（例如[ 和]）的文件或目录时，您需要按照Golang规则转义那些路径，以防止
将它们视为匹配模式。例如，要添加名为的文件arr[0].txt，请使用以下命令；

ADD arr[[]0].txt /mydir/

除非可选--chown标志指定给定的用户名，组名或UID / GID组合以请求对所添加内容的特定所有权
，否则所有新文件和目录的UID和GID均为0 。--chown标志的格式允许用户名和组名字符串或直接
整数UID和GID任意组合。提供不带组名的用户名或不带GID的UID将使用与GID相同的数字UID。如果
提供了用户名或组名，则将使用容器的根文件系统 /etc/passwd和/etc/group文件将名称分别转换
为整数UID或GID。以下示例显示了该--chown标志的有效定义：

ADD --chown=55:mygroup files* /somedir/
ADD --chown=bin files* /somedir/
ADD --chown=1 files* /somedir/
ADD --chown=10:11 files* /somedir/

如果容器根文件系统不包含/etc/passwd或 /etc/group文件，并且在--chown 标志中使用了用户名
或组名，则该构建将在该ADD操作上失败。使用数字ID不需要查找，并且不依赖于容器根文件系统
内容。

如果<src>是远程文件URL，则目标将具有600的权限。如果要检索的远程文件具有HTTP
Last-Modified标头，则该标头中的时间戳将用于设置mtime目标文件上的时间戳。但是，就像在期
间处理的任何其他文件一样ADD，mtime该文件是否已更改以及是否应更新缓存的确定将不包括在内
。

    注意

    如果通过传递DockerfileSTDIN（docker build - < somefile）进行构建，则没有构建上下文
    ，因此Dockerfile 只能包含基于URL的ADD指令。您还可以通过STDIN：（docker build - <
    archive.tar.gz）传递压缩的归档文件，归档文件Dockerfile根目录中的，归档文件的其余部
    分将用作构建的上下文。

如果您的网址文件都使用认证保护，则需要使用RUN wget， RUN curl或作为从容器内使用另一种
工具ADD指令不支持认证。

    注意

    ADD如果Dockerfile中的内容<src>已更改，则第一个遇到的指令将使Dockerfile中所有后续指
    令的缓存无效。这包括使高速缓存中的RUN指令无效。有关更多信息，请参见Dockerfile最佳
    实践指南–利用构建缓存。

ADD 遵守以下规则：

  • 该<src>路径必须是内部语境的构建; 您不能这样做ADD ../something /something，因为a的
    第一步 docker build是将上下文目录（和子目录）发送到docker守护程序。

  • 如果<src>是URL，<dest>并且不以斜杠结尾，则从URL下载文件并将其复制到<dest>。

  • 如果<src>是URL并<dest>以斜杠结尾，则从URL推断文件名，然后将文件下载到 <dest>/
    <filename>。例如，ADD http://example.com/foobar /将创建文件/foobar。该URL必须具有
    非平凡的路径，以便在这种情况下可以发现适当的文件名（http://example.com 将不起作用
    ）。

  • 如果<src>是目录，则将复制目录的整个内容，包括文件系统元数据。

    注意

    该目录本身不被复制，仅被复制其内容。

  • 如果<src>是以公认的压缩格式（身份，gzip，bzip2或xz）作为本地 tar归档文件，则将其解
    压缩为目录。来自远程 URL的资源不会被解压缩。复制或解压缩目录时，其行为与相同tar -x
    ，结果是以下各项的并集：

     1. 目标路径上存在的任何内容
     2. 源代码树的内容，已解决冲突，而选择了“ 2”。在逐个文件的基础上。


        注意

        是否仅将文件内容而不是文件名确定为文件是公认的压缩格式。例如，如果一个空文件恰
        好以.tar.gz该文件结尾，则不会被识别为压缩文件，并且不会生成任何类型的解压缩错
        误消息，而是会将文件简单地复制到目标位置。

  • 如果<src>是任何其他类型的文件，则将其及其元数据一起单独复制。在这种情况下，如果
    <dest>以斜杠结尾/，它将被视为目录，其内容<src>将写入<dest>/base(<src>)。

  • 如果<src>直接或由于使用通配符而指定了多个资源，则该资源<dest>必须是目录，并且必须
    以斜杠结尾/。

  • 如果<dest>不以斜杠结尾，则将其视为常规文件，并将其内容<src>写入<dest>。

  • 如果<dest>不存在，它将与路径中所有缺少的目录一起创建。

复制🔗

COPY有两种形式：

COPY [--chown=<user>:<group>] <src>... <dest>
COPY [--chown=<user>:<group>] ["<src>",... "<dest>"]

包含空格的路径需要后一种形式

    注意

    该--chown功能仅在用于构建Linux容器的Dockerfiles上受支持，而在Windows容器上不起作用
    。由于用户和组所有权概念不会在Linux和Windows之间转换，因此使用/etc/passwd和/etc/
    group将用户名和组名转换为ID的使用限制了此功能仅适用于基于Linux OS的容器。

该COPY指令从中复制新文件或目录<src> ，并将它们添加到路径中容器的文件系统中<dest>。

<src>可以指定多个资源，但是文件和目录的路径将被解释为相对于构建上下文的源。

每个都<src>可能包含通配符，并且匹配将使用Go的 filepath.Match规则完成。例如：

要添加所有以“ hom”开头的文件：

COPY hom* /mydir/

在下面的示例中，?被替换为任何单个字符，例如“ home.txt”。

COPY hom?.txt /mydir/

的<dest>是一个绝对路径，或相对于一个路径WORKDIR，到其中的源将在目标容器内进行复制。

下面的示例使用相对路径，并将“ test.txt”添加到<WORKDIR>/relativeDir/：

COPY test.txt relativeDir/

而此示例使用绝对路径，并向其中添加“ test.txt” /absoluteDir/

COPY test.txt /absoluteDir/

复制包含特殊字符（例如[ 和]）的文件或目录时，您需要按照Golang规则转义那些路径，以防止
将它们视为匹配模式。例如，要添加名为的文件arr[0].txt，请使用以下命令；

ADD arr[[]0].txt /mydir/

除非可选--chown标志指定给定的用户名，组名或UID / GID组合以请求对复制内容的特定所有权，
否则所有新文件和目录的UID和GID均为0 。--chown标志的格式允许用户名和组名字符串或直接整
数UID和GID任意组合。提供不带组名的用户名或不带GID的UID将使用与GID相同的数字UID。如果提
供了用户名或组名，则将使用容器的根文件系统 /etc/passwd和/etc/group文件将名称分别转换为
整数UID或GID。以下示例显示了该--chown标志的有效定义：

COPY --chown=55:mygroup files* /somedir/
COPY --chown=bin files* /somedir/
COPY --chown=1 files* /somedir/
COPY --chown=10:11 files* /somedir/

如果容器根文件系统不包含/etc/passwd或 /etc/group文件，并且在--chown 标志中使用了用户名
或组名，则该构建将在该COPY操作上失败。使用数字ID不需要查找，并且不依赖于容器根文件系统
内容。

    注意

    如果使用STDIN（docker build - < somefile）进行构建，则没有构建上下文，因此COPY无法
    使用。

（可选）COPY接受一个标志--from=<name|index>，该标志可用于将源位置设置为FROM .. AS
<name>将用于代替用户发送的构建上下文的上一个构建阶段（使用创建）。该标志还接受为以FROM
指令开头的所有先前构建阶段分配的数字索引。如果找不到具有指定名称的构建阶段，则尝试使用
具有相同名称的映像代替。

COPY 遵守以下规则：

  • 该<src>路径必须是内部语境的构建; 您不能这样做COPY ../something /something，因为a的
    第一步 docker build是将上下文目录（和子目录）发送到docker守护程序。

  • 如果<src>是目录，则将复制目录的整个内容，包括文件系统元数据。

    注意

    该目录本身不被复制，仅被复制其内容。

  • 如果<src>是任何其他类型的文件，则将其及其元数据一起单独复制。在这种情况下，如果
    <dest>以斜杠结尾/，它将被视为目录，其内容<src>将写入<dest>/base(<src>)。

  • 如果<src>直接或由于使用通配符而指定了多个资源，则该资源<dest>必须是目录，并且必须
    以斜杠结尾/。

  • 如果<dest>不以斜杠结尾，则将其视为常规文件，并将其内容<src>写入<dest>。

  • 如果<dest>不存在，它将与路径中所有缺少的目录一起创建。

    注意

    COPY如果Dockerfile中的内容<src>已更改，则第一个遇到的指令将使Dockerfile中所有后续
    指令的缓存无效。这包括使高速缓存中的RUN指令无效。有关更多信息，请参见Dockerfile最
    佳实践指南–利用构建缓存。

入口点🔗

ENTRYPOINT有两种形式：

在EXEC的形式，这是优选的形式：

ENTRYPOINT ["executable", "param1", "param2"]

该壳形式：

ENTRYPOINT command param1 param2

An ENTRYPOINT允许您配置将作为可执行文件运行的容器。

例如，以下代码以其默认内容启动nginx，监听端口80：

$ docker run -i -t --rm -p 80:80 nginx

命令行参数to docker run <image>将以exec形式附加在所有元素之后ENTRYPOINT，并将覆盖使用
指定的所有元素CMD。这允许将参数传递给入口点，即，docker run <image> -d 将-d参数传递给
入口点。您可以ENTRYPOINT使用该docker run --entrypoint 标志覆盖指令。

所述壳形式防止任何CMD或run被使用命令行参数，但具有的缺点是你ENTRYPOINT将开始作为的子命
令/bin/sh -c，其不通过信号。这意味着可执行文件将不是容器的PID 1-并且将不会接收Unix信号
-因此您的可执行文件将不会SIGTERM从接收到docker stop <container>。

只有中的最后一条ENTRYPOINT指令Dockerfile才会生效。

执行表格ENTRYPOINT示例🔗

您可以使用exec形式的ENTRYPOINT来设置相当稳定的默认命令和参数，然后使用这两种形式的CMD
来设置更可能被更改的其他默认值。

FROM ubuntu
ENTRYPOINT ["top", "-b"]
CMD ["-c"]

运行容器时，可以看到这top是唯一的过程：

$ docker run -it --rm --name test  top -H

top - 08:25:00 up  7:27,  0 users,  load average: 0.00, 0.01, 0.05
Threads:   1 total,   1 running,   0 sleeping,   0 stopped,   0 zombie
%Cpu(s):  0.1 us,  0.1 sy,  0.0 ni, 99.7 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
KiB Mem:   2056668 total,  1616832 used,   439836 free,    99352 buffers
KiB Swap:  1441840 total,        0 used,  1441840 free.  1324440 cached Mem

  PID USER      PR  NI    VIRT    RES    SHR S %CPU %MEM     TIME+ COMMAND
    1 root      20   0   19744   2336   2080 R  0.0  0.1   0:00.04 top

要进一步检查结果，可以使用docker exec：

$ docker exec -it test ps aux

USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root         1  2.6  0.1  19752  2352 ?        Ss+  08:24   0:00 top -b -H
root         7  0.0  0.1  15572  2164 ?        R+   08:25   0:00 ps aux

您也可以使用优雅地请求top关闭docker stop test。

下面Dockerfile显示了使用ENTRYPOINT来在前台运行Apache（即as PID 1）：

FROM debian:stable
RUN apt-get update && apt-get install -y --force-yes apache2
EXPOSE 80 443
VOLUME ["/var/www", "/var/log/apache2", "/etc/apache2"]
ENTRYPOINT ["/usr/sbin/apache2ctl", "-D", "FOREGROUND"]

如果需要为单个可执行文件编写启动脚本，则可以使用exec和gosu 命令确保最终的可执行文件接
收Unix信号：

#!/usr/bin/env bash
set -e

if [ "$1" = 'postgres' ]; then
    chown -R postgres "$PGDATA"

    if [ -z "$(ls -A "$PGDATA")" ]; then
        gosu postgres initdb
    fi

    exec gosu postgres "$@"
fi

exec "$@"

最后，如果您需要在关闭时进行一些额外的清理（或与其他容器通信），或协调多个可执行文件，
则可能需要确保ENTRYPOINT脚本接收Unix信号，将其传递，然后执行一些更多的工作：

#!/bin/sh
# Note: I've written this using sh so it works in the busybox container too

# USE the trap if you need to also do manual cleanup after the service is stopped,
#     or need to start multiple services in the one container
trap "echo TRAPed signal" HUP INT QUIT TERM

# start service in background here
/usr/sbin/apachectl start

echo "[hit enter key to exit] or run 'docker stop <container>'"
read

# stop service and clean up here
echo "stopping apache"
/usr/sbin/apachectl stop

echo "exited $0"

如果使用运行该映像docker run -it --rm -p 80:80 --name test apache，则可以使用docker
exec或来检查容器的进程docker top，然后要求脚本停止Apache：

$ docker exec -it test ps aux

USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root         1  0.1  0.0   4448   692 ?        Ss+  00:42   0:00 /bin/sh /run.sh 123 cmd cmd2
root        19  0.0  0.2  71304  4440 ?        Ss   00:42   0:00 /usr/sbin/apache2 -k start
www-data    20  0.2  0.2 360468  6004 ?        Sl   00:42   0:00 /usr/sbin/apache2 -k start
www-data    21  0.2  0.2 360468  6000 ?        Sl   00:42   0:00 /usr/sbin/apache2 -k start
root        81  0.0  0.1  15572  2140 ?        R+   00:44   0:00 ps aux

$ docker top test

PID                 USER                COMMAND
10035               root                {run.sh} /bin/sh /run.sh 123 cmd cmd2
10054               root                /usr/sbin/apache2 -k start
10055               33                  /usr/sbin/apache2 -k start
10056               33                  /usr/sbin/apache2 -k start

$ /usr/bin/time docker stop test

test
real    0m 0.27s
user    0m 0.03s
sys     0m 0.03s

    注意

    您可以使用覆盖ENTRYPOINT设置--entrypoint，但这只能将二进制文件设置为exec（sh -c将
    不使用）。

    注意

    在EXEC形式被解析为一个JSON阵列，这意味着必须使用双引号（“）周围的话不单引号（'）。

与shell表单不同，exec表单不会调用命令shell。这意味着不会发生常规的外壳处理。例如，
ENTRYPOINT [ "echo", "$HOME" ]将不会对进行变量替换$HOME。如果要进行shell处理，则可以使
用shell形式或直接执行shell，例如：ENTRYPOINT [ "sh", "-c", "echo $HOME" ]。当使用exec
表单并直接执行shell时（例如在shell表单中），是由shell进行环境变量扩展，而不是docker。

外壳形式ENTRYPOINT示例example

您可以为指定一个纯字符串ENTRYPOINT，它将在中执行/bin/sh -c。这种形式将使用外壳处理来替
代外壳环境变量，并且将忽略任何CMD或docker run命令行参数。为了确保能够正确docker stop发
出任何长期运行的ENTRYPOINT可执行文件信号，您需要记住以以下命令启动它exec：

FROM ubuntu
ENTRYPOINT exec top -b

运行此图像时，您将看到单个PID 1过程：

$ docker run -it --rm --name test top

Mem: 1704520K used, 352148K free, 0K shrd, 0K buff, 140368121167873K cached
CPU:   5% usr   0% sys   0% nic  94% idle   0% io   0% irq   0% sirq
Load average: 0.08 0.03 0.05 2/98 6
  PID  PPID USER     STAT   VSZ %VSZ %CPU COMMAND
    1     0 root     R     3164   0%   0% top -b

干净地退出docker stop：

$ /usr/bin/time docker stop test

test
real    0m 0.20s
user    0m 0.02s
sys     0m 0.04s

如果您忘记添加exec到您的开头ENTRYPOINT：

FROM ubuntu
ENTRYPOINT top -b
CMD --ignored-param1

然后，您可以运行它（为下一步命名）：

$ docker run -it --name test top --ignored-param2

Mem: 1704184K used, 352484K free, 0K shrd, 0K buff, 140621524238337K cached
CPU:   9% usr   2% sys   0% nic  88% idle   0% io   0% irq   0% sirq
Load average: 0.01 0.02 0.05 2/101 7
  PID  PPID USER     STAT   VSZ %VSZ %CPU COMMAND
    1     0 root     S     3168   0%   0% /bin/sh -c top -b cmd cmd2
    7     1 root     R     3164   0%   0% top -b

从输出中可以看到top，指定ENTRYPOINT的不是PID 1。

如果随后运行docker stop test，容器将不会干净退出- 超时后将stop强制命令发送a SIGKILL：

$ docker exec -it test ps aux

PID   USER     COMMAND
    1 root     /bin/sh -c top -b cmd cmd2
    7 root     top -b
    8 root     ps aux

$ /usr/bin/time docker stop test

test
real    0m 10.19s
user    0m 0.04s
sys     0m 0.03s

了解CMD和ENTRYPOINT如何相互作用🔗

无论CMD和ENTRYPOINT指令定义运行的容器中时什么命令得到执行。很少有规则描述他们的合作。

 1. Dockerfile应该指定CMD或ENTRYPOINT命令中的至少一个。

 2. ENTRYPOINT 使用容器作为可执行文件时应定义。

 3. CMD应该用作定义ENTRYPOINT命令默认值或在容器中执行临时命令的方式。

 4. CMD 当使用其他参数运行容器时，将被覆盖。

下表显示了针对不同ENTRYPOINT/ CMD组合执行的命令：

                     没有入口点          ENTRYPOINT      ENTRYPOINT [“ exec_entry”，“
                                    exec_entry p1_entry           p1_entry”]
没有CMD           错误，不允许      / bin / sh -c        exec_entry p1_entry
                                    exec_entry p1_entry
CMD [“ exec_cmd”  exec_cmd p1_cmd   / bin / sh -c        exec_entry p1_entry exec_cmd
，“ p1_cmd”]                        exec_entry p1_entry  p1_cmd
CMD [“ p1_cmd”，“ p1_cmd p2_cmd     / bin / sh -c        exec_entry p1_entry p1_cmd
p2_cmd”]                            exec_entry p1_entry  p2_cmd
CMD exec_cmd      / bin / sh -c     / bin / sh -c        exec_entry p1_entry / bin /
p1_cmd            exec_cmd p1_cmd   exec_entry p1_entry  sh -c exec_cmd p1_cmd

    注意

    如果CMD从基本图像定义，则设置ENTRYPOINT将重置CMD为空值。在这种情况下，CMD必须在当
    前图像中定义一个值。

音量🔗

VOLUME ["/data"]

该VOLUME指令创建具有指定名称的安装点，并将其标记为保存来自本机主机或其他容器的外部安装
的卷。该值可以是JSON数组，也可以是VOLUME ["/var/log/"]具有多个参数的纯字符串，例如
VOLUME /var/log或VOLUME /var/log /var/db。有关通过Docker客户端的更多信息/示例和安装说
明，请参阅通过Volumes共享目录。

该docker run命令使用基本映像内指定位置上存在的任何数据初始化新创建的卷。例如，考虑以下
Dockerfile片段：

FROM ubuntu
RUN mkdir /myvol
RUN echo "hello world" > /myvol/greeting
VOLUME /myvol

该Dockerfile生成一个映像，该映像导致docker run在处创建一个新的挂载点/myvol并将该
greeting文件复制到新创建的卷中。

有关指定卷的注意事项🔗

关于。中的卷，请注意以下几点Dockerfile。

  • 基于Windows的容器上的卷：使用基于Windows的容器时，容器内的卷的目的地必须是以下之一
    ：

      □ 不存在或空目录
      □ 除以下以外的驱动器 C:
  • 从Dockerfile中更改卷：如果在声明了卷之后有任何构建步骤更改了卷中的数据，则这些更改
    将被丢弃。

  • JSON格式：列表被解析为JSON数组。您必须用双引号（"）而不是单引号（'）括住单词。

  • 主机目录是在容器运行时声明的：主机目录（挂载点）从本质上说是依赖于主机的。这是为了
    保留图像的可移植性，因为不能保证给定的主机目录在所有主机上都可用。因此，您无法从
    Dockerfile中挂载主机目录。该VOLUME指令不支持指定host-dir 参数。创建或运行容器时，
    必须指定安装点。

用户🔗

USER <user>[:<group>]

要么

USER <UID>[:<GID>]

该USER指令设置运行映像时以及用于任何映像时使用的用户名（或UID）和可选的用户组（或GID）
RUN，CMD以及在 ENTRYPOINT后面跟随的指令Dockerfile。

    请注意，在为用户指定组时，用户将仅具有指定的组成员身份。任何其他已配置的组成员身份
    将被忽略。

    警告

    当用户没有主要组时，该映像（或后续说明）将与该root组一起运行。

    在Windows上，如果不是内置帐户，则必须首先创建用户。这可以通过net user作为
    Dockerfile的一部分调用的命令来完成。

FROM microsoft/windowsservercore
# Create Windows user in the container
RUN net user /add patrick
# Set it for subsequent commands
USER patrick

WORKDIR 🔗

WORKDIR /path/to/workdir

该WORKDIR指令集的工作目录对任何RUN，CMD， ENTRYPOINT，COPY和ADD它后面的说明Dockerfile
。如果WORKDIR不存在，即使以后的任何Dockerfile指令中都没有使用它，也将创建它。

该WORKDIR指令可以在中多次使用Dockerfile。如果提供了相对路径，则它将相对于上一条WORKDIR
指令的路径。例如：

WORKDIR /a
WORKDIR b
WORKDIR c
RUN pwd

最终的输出pwd命令这Dockerfile将是/a/b/c。

该WORKDIR指令可以解析先前使用设置的环境变量 ENV。您只能使用在中显式设置的环境变量
Dockerfile。例如：

ENV DIRPATH /path
WORKDIR $DIRPATH/$DIRNAME
RUN pwd

最终pwd命令的输出Dockerfile将是 /path/$DIRNAME

ARG 🔗

ARG <name>[=<default value>]

该ARG指令定义了一个变量，用户可以在构建时docker build使用带有--build-arg <varname>=
<value> 标志的命令将其传递给构建器。如果用户指定了Dockerfile中未定义的构建参数，则该构
建会输出警告。

[Warning] One or more build-args [foo] were not consumed.

Dockerfile可能包含一个或多个ARG指令。例如，以下是有效的Dockerfile：

FROM busybox
ARG user1
ARG buildno
# ...

    警告：

    不建议使用构建时变量来传递诸如github密钥，用户凭据等机密。构建时变量值对于使用该
    docker history命令的图像的任何用户都是可见的。

    请参阅“使用BuildKit生成映像” 部分，以了解有关在生成映像时使用机密的安全方法。

默认值🔗

的ARG指令可以可选地包括一个默认值：

FROM busybox
ARG user1=someuser
ARG buildno=1
# ...

如果ARG指令具有缺省值，并且在构建时未传递任何值，那么构建器将使用缺省值。

范围🔗

一个ARG变量定义进入从在其上在限定的线效果Dockerfile不从参数对命令行或其他地方使用。例
如，考虑以下Dockerfile：

FROM busybox
USER ${user:-some_user}
ARG user
USER $user
# ...

用户通过调用以下命令来构建此文件：

$ docker build --build-arg user=what_user .

USER在第2行，at some_user在user变量3上定义为。USER在第4 行，at 定义为what_useras user
，并且what_user在命令行上传递了值。在通过ARG指令对其进行定义之前，对变量的任何使用都会
导致一个空字符串。

的ARG指令在它被定义的构建阶段结束推移的范围进行。要在多个阶段使用arg，每个阶段都必须包
含ARG指令。

FROM busybox
ARG SETTINGS
RUN ./run/setup $SETTINGS

FROM busybox
ARG SETTINGS
RUN ./run/other $SETTINGS

使用ARG变量🔗

您可以使用ARG或ENV指令指定该RUN指令可用的变量。使用ENV指令定义的环境变量始终会覆盖ARG
同名指令。考虑此Dockerfile和ENVand ARG指令。

FROM ubuntu
ARG CONT_IMG_VER
ENV CONT_IMG_VER v1.0.0
RUN echo $CONT_IMG_VER

然后，假定此映像是使用以下命令构建的：

$ docker build --build-arg CONT_IMG_VER=v2.0.1 .

在这种情况下，该RUN指令将使用v1.0.0而不是ARG用户传递的设置：v2.0.1此行为类似于Shell脚
本，其中局部作用域的变量从其定义的角度覆盖作为参数传递或从环境继承的变量。

使用上面的示例，但使用不同的ENV规范，可以在ARG和ENV指令之间创建更有用的交互：

FROM ubuntu
ARG CONT_IMG_VER
ENV CONT_IMG_VER ${CONT_IMG_VER:-v1.0.0}
RUN echo $CONT_IMG_VER

与ARG指令不同，ENV值始终保留在生成的映像中。考虑不带--build-arg标志的Docker构建：

$ docker build .

使用此Dockerfile示例，CONT_IMG_VER它仍然保留在映像中，但其值将是指令v1.0.0第3行中的默
认设置ENV。

在此示例中，变量扩展技术使您可以从命令行传递参数，并利用ENV指令将其保留在最终映像中。
仅有限的一组Dockerfile指令支持变量扩展。

预定义的ARGs🔗

Docker具有一组预定义ARG变量，您可以在不使用ARGDockerfile中相应指令的情况下使用它们。

  • HTTP_PROXY
  • http_proxy
  • HTTPS_PROXY
  • https_proxy
  • FTP_PROXY
  • ftp_proxy
  • NO_PROXY
  • no_proxy

要使用这些，只需使用以下标志在命令行中传递它们：

--build-arg <varname>=<value>

默认情况下，这些预定义变量从的输出中排除 docker history。排除它们可以减少意外泄露
HTTP_PROXY变量中的敏感身份验证信息的风险。

例如，考虑使用以下命令构建以下Dockerfile --build-arg HTTP_PROXY=http://
user:pass@proxy.lon.example.com

FROM ubuntu
RUN echo "Hello World"

在这种情况下，HTTP_PROXY变量的值在中不可用， docker history也不被缓存。如果要更改位置
，并且代理服务器已更改为http://user:pass@proxy.sfo.example.com，则后续的构建不会导致高
速缓存未命中。

如果您需要覆盖此行为，则可以通过ARG 在Dockerfile中添加如下语句来做到这一点：

FROM ubuntu
ARG HTTP_PROXY
RUN echo "Hello World"

构建此Dockerfile时，将HTTP_PROXY保留在中 docker history，并且更改其值会使构建缓存无效
。

全球范围内的自动平台ARGs🔗

仅当使用BuildKit后端时，此功能才可用。

Docker ARG在执行构建的节点的平台（构建平台）和结果映像的平台（目标平台）上用信息预定义
了一组变量。可以使用--platform标志on 来指定目标平台docker build。

以下ARG变量是自动设置的：

  • TARGETPLATFORM-构建结果的平台。例如linux/amd64，linux/arm/v7，windows/amd64。
  • TARGETOS -TARGETPLATFORM的OS组件
  • TARGETARCH -TARGETPLATFORM的体系结构组件
  • TARGETVARIANT -TARGETPLATFORM的变体组件
  • BUILDPLATFORM -执行构建的节点的平台。
  • BUILDOS -BUILDPLATFORM的OS组件
  • BUILDARCH -BUILDPLATFORM的体系结构组件
  • BUILDVARIANT -BUILDPLATFORM的变体组件

这些参数在全局范围内定义，因此在构建阶段或您的RUN命令中不会自动提供。为了在构建阶段公
开这些参数之一，请重新定义它而没有价值。

例如：

FROM alpine
ARG TARGETPLATFORM
RUN echo "I'm building for $TARGETPLATFORM"

对构建缓存的影响🔗

ARG变量不会像ENV变量那样持久化到生成的映像中。但是，ARG变量确实以类似的方式影响构建缓
存。如果Dockerfile定义了一个ARG其值不同于先前构建的变量，则首次使用时会发生“缓存未命中
”，而不是其定义。特别是，RUN一条指令之后的所有指令都隐式地ARG使用该ARG变量（作为环境变
量），因此可能导致高速缓存未命中。ARG除非。中有匹配的ARG语句，否则所有预定义变量均免于
缓存Dockerfile。

例如，考虑以下两个Dockerfile：

FROM ubuntu
ARG CONT_IMG_VER
RUN echo $CONT_IMG_VER

FROM ubuntu
ARG CONT_IMG_VER
RUN echo hello

如果--build-arg CONT_IMG_VER=<value>在命令行上指定，则在两种情况下，第2行上的指定都不
会导致高速缓存未命中。第3行确实会导致缓存未命中。ARG CONT_IMG_VER导致RUN行被标识为与正
在运行的CONT_IMG_VER=<value>echo hello 相同，因此，如果进行了<value> 更改，则会遇到缓
存未命中的情况。

考虑在同一命令行下的另一个示例：

FROM ubuntu
ARG CONT_IMG_VER
ENV CONT_IMG_VER $CONT_IMG_VER
RUN echo $CONT_IMG_VER

在此示例中，高速缓存未命中发生在第3行。之所以发生未命中，是因为该变量在ENV引用中的值引
用了该ARG变量，并且该变量通过命令行进行了更改。在此示例中，该ENV 命令使图像包含该值。

如果一条ENV指令覆盖ARG了同名指令，例如Dockerfile：

FROM ubuntu
ARG CONT_IMG_VER
ENV CONT_IMG_VER hello
RUN echo $CONT_IMG_VER

第3行不会导致缓存未命中，因为的值CONT_IMG_VER是一个常量（hello）。结果，RUN（第4行）使
用的环境变量和值在两次构建之间不会更改。

已建🔗

ONBUILD <INSTRUCTION>

当该图像用作另一个构建的基础时，该ONBUILD指令会在图像上添加要在以后执行的触发指令。触
发器将在下游构建的上下文中执行，就好像它已FROM在下游指令之后立即插入 Dockerfile。

任何构建指令都可以注册为触发器。

如果要构建的图像将用作构建其他图像的基础，例如应用程序构建环境或可以使用用户特定配置自
定义的守护程序，则此功能很有用。

例如，如果您的图像是可重用的Python应用程序构建器，则将需要在特定目录中添加应用程序源代
码，并且此后可能需要调用构建脚本。你不能只是打电话ADD和RUN现在，因为你还没有访问应用程
序的源代码，这将是为每个应用程序生成不同的。您可以简单地为应用程序开发人员提供
Dockerfile可复制粘贴到其应用程序中的样板，但这效率低下，容易出错且难以更新，因为它与特
定于应用程序的代码混合在一起。

解决方案是用于ONBUILD注册预先的指令，以便在下一个构建阶段中稍后运行。

运作方式如下：

 1. 当遇到ONBUILD指令时，构建器将触发器添加到正在构建的图像的元数据中。该指令不会影响
    当前版本。
 2. 在构建结束时，所有触发器的列表都存储在映像清单的key下OnBuild。可以使用docker
    inspect命令检查它们。
 3. 稍后，可以使用该FROM指令将该图像用作新版本的基础。作为处理FROM指令的一部分，下游构
    建器将查找ONBUILD触发器，并按照注册时的顺序执行它们。如果任何触发器失败，则该FROM
    指令将中止，进而导致构建失败。如果所有触发器都成功，则FROM指令完成，并且构建照常继
    续。
 4. 执行完触发器后，将从最终图像中清除触发器。换句话说，它们不是“孙子代”版本所继承的。

例如，您可以添加以下内容：

ONBUILD ADD . /app/src
ONBUILD RUN /usr/local/bin/python-build --dir /app/src

    警告

    不允许ONBUILD使用链接说明ONBUILD ONBUILD。

    警告

    该ONBUILD指令可能不会触发FROM或MAINTAINER指令。

停止信号🔗

STOPSIGNAL signal

该STOPSIGNAL指令设置将被发送到容器退出的系统调用信号。该信号可以是与内核syscall表中的
位置匹配的有效无符号数字（例如9），也可以是格式为SIGNAME的信号名称（例如SIGKILL）。

健康检查🔗

该HEALTHCHECK指令有两种形式：

  • HEALTHCHECK [OPTIONS] CMD command （通过在容器内部运行命令来检查容器的运行状况）
  • HEALTHCHECK NONE （禁用从基本映像继承的任何运行状况检查）

该HEALTHCHECK指令告诉Docker如何测试容器以检查其是否仍在工作。这样可以检测到诸如Web服务
器陷入无限循环并且无法处理新连接的情况，即使服务器进程仍在运行。

指定容器的运行状况检查后，除了其正常状态外，它还具有运行状况。此状态最初为starting。只
要运行状况检查通过，它将变为healthy（以前处于任何状态）。在一定数量的连续失败之后，它
变为unhealthy。

之前可能出现的选项CMD是：

  • --interval=DURATION（默认值：30s）
  • --timeout=DURATION（默认值：30s）
  • --start-period=DURATION（默认值：0s）
  • --retries=N（默认值：3）

运行状况检查将首先在启动容器后间隔几秒钟运行一次，然后在每次之前的检查完成后间隔几秒钟
再次运行。

如果单次检查花费的时间超过超时秒数，则认为检查失败。

对于要考虑的容器，需要重试连续进行的运行状况检查失败unhealthy。

开始时间段为需要时间进行引导的容器提供了初始化时间。在此期间内的探针故障将不计入最大重
试次数。但是，如果运行状况检查在启动期间成功，则认为该容器已启动，并且所有连续失败将计
入最大重试次数。

HEALTHCHECKDockerfile中只能有一条指令。如果您列出多个，则只有最后一个HEALTHCHECK才会生
效。

CMD关键字后面的命令可以是shell命令（例如HEALTHCHECK CMD /bin/check-running）或exec数组
（与其他Dockerfile命令一样；ENTRYPOINT有关详细信息，请参见例如）。

命令的退出状态指示容器的健康状态。可能的值为：

  • 0：成功-容器健康且可以使用
  • 1：不健康-容器无法正常工作
  • 2：保留-请勿使用此退出代码

例如，要每五分钟检查一次，以便网络服务器能够在三秒钟内为站点的主页提供服务：

HEALTHCHECK --interval=5m --timeout=3s \
  CMD curl -f http://localhost/ || exit 1

为了帮助调试失败的探针，该命令在stdout或stderr上写入的任何输出文本（UTF-8编码）都将存
储在运行状况中，并可以通过查询 docker inspect。此类输出应保持简短（当前仅存储前4096个
字节）。

容器的健康状态发生更改时，将health_status生成具有新状态的事件。

该HEALTHCHECK功能已在Docker 1.12中添加。

SHELL 🔗

SHELL ["executable", "parameters"]

该SHELL指令允许覆盖用于命令的shell形式的默认shell 。在Linux上["/bin/sh", "-c"]，默认
shell是，在Windows 上，默认shell 是["cmd", "/S", "/C"]。该SHELL指令必须以Docker格式的
JSON形式编写。

该SHELL指令在Windows上特别有用，在Windows上有两个常用且完全不同的本机shell：cmd和
powershell，以及可用的替代shell包括sh。

该SHELL说明可以出现多次。每个SHELL指令将覆盖所有先前的SHELL指令，并影响所有后续的指令
。例如：

FROM microsoft/windowsservercore

# Executed as cmd /S /C echo default
RUN echo default

# Executed as cmd /S /C powershell -command Write-Host default
RUN powershell -command Write-Host default

# Executed as powershell -command Write-Host hello
SHELL ["powershell", "-command"]
RUN Write-Host hello

# Executed as cmd /S /C echo hello
SHELL ["cmd", "/S", "/C"]
RUN echo hello

以下说明可以通过影响SHELL指令时，壳他们的形式在一个Dockerfile使用：RUN，CMD和
ENTRYPOINT。

以下示例是Windows上常见的模式，可通过使用SHELL指令进行精简：

RUN powershell -command Execute-MyCmdlet -param1 "c:\foo.txt"

docker调用的命令将是：

cmd /S /C powershell -command Execute-MyCmdlet -param1 "c:\foo.txt"

这效率低下有两个原因。首先，有一个不必要的cmd.exe命令处理器（也称为Shell）被调用。其次
，shell 形式的每条RUN指令都需要在命令前加上前缀。powershell -command

为了使其更有效，可以采用两种机制之一。一种是使用RUN命令的JSON形式，例如：

RUN ["powershell", "-command", "Execute-MyCmdlet", "-param1 \"c:\\foo.txt\""]

尽管JSON形式是明确的，并且不使用不必要的cmd.exe，但它确实需要通过双引号和转义来实现更
多的详细信息。另一种机制是使用SHELL指令和外壳程序形式，使Windows用户的语法更自然，尤其
是与escapeparser指令结合使用时：

# escape=`

FROM microsoft/nanoserver
SHELL ["powershell","-command"]
RUN New-Item -ItemType Directory C:\Example
ADD Execute-MyCmdlet.ps1 c:\example\
RUN c:\example\Execute-MyCmdlet -sample 'hello world'

导致：

PS E:\docker\build\shell> docker build -t shell .
Sending build context to Docker daemon 4.096 kB
Step 1/5 : FROM microsoft/nanoserver
 ---> 22738ff49c6d
Step 2/5 : SHELL powershell -command
 ---> Running in 6fcdb6855ae2
 ---> 6331462d4300
Removing intermediate container 6fcdb6855ae2
Step 3/5 : RUN New-Item -ItemType Directory C:\Example
 ---> Running in d0eef8386e97


    Directory: C:\


Mode                LastWriteTime         Length Name
----                -------------         ------ ----
d-----       10/28/2016  11:26 AM                Example


 ---> 3f2fbf1395d9
Removing intermediate container d0eef8386e97
Step 4/5 : ADD Execute-MyCmdlet.ps1 c:\example\
 ---> a955b2621c31
Removing intermediate container b825593d39fc
Step 5/5 : RUN c:\example\Execute-MyCmdlet 'hello world'
 ---> Running in be6d8e63fe75
hello world
 ---> 8e559e9bf424
Removing intermediate container be6d8e63fe75
Successfully built 8e559e9bf424
PS E:\docker\build\shell>

该SHELL指令还可用于修改外壳的操作方式。例如，SHELL cmd /S /C /V:ON|OFF在Windows上使用
，可以修改延迟的环境变量扩展语义。

如果SHELL需要备用shell，例如，和其他zsh，该指令也可以在Linux上使用。cshtcsh

该SHELL功能已在Docker 1.12中添加。

外部实施功能🔗

仅当使用BuildKit后端时，此功能才可用。

Docker构建支持实验性功能，例如缓存安装，构建机密和ssh转发，这些功能通过使用带有语法指
令的构建器的外部实现启用。要了解这些功能，请参阅BuildKit信息库中的文档。

Dockerfile示例examples

在下面，您可以看到Dockerfile语法的一些示例。

# Nginx
#
# VERSION               0.0.1

FROM      ubuntu
LABEL Description="This image is used to start the foobar executable" Vendor="ACME Products" Version="1.0"
RUN apt-get update && apt-get install -y inotify-tools nginx apache2 openssh-server

# Firefox over VNC
#
# VERSION               0.3

FROM ubuntu

# Install vnc, xvfb in order to create a 'fake' display and firefox
RUN apt-get update && apt-get install -y x11vnc xvfb firefox
RUN mkdir ~/.vnc
# Setup a password
RUN x11vnc -storepasswd 1234 ~/.vnc/passwd
# Autostart firefox (might not be the best way, but it does the trick)
RUN bash -c 'echo "firefox" >> /.bashrc'

EXPOSE 5900
CMD    ["x11vnc", "-forever", "-usepw", "-create"]

# Multiple images example
#
# VERSION               0.1

FROM ubuntu
RUN echo foo > bar
# Will output something like ===> 907ad6c2736f

FROM ubuntu
RUN echo moo > oink
# Will output something like ===> 695d7793cbe4

# You'll now have two images, 907ad6c2736f with /bar, and 695d7793cbe4 with
# /oink.
