Docker文件
Docker文件
[                    ]
切换导航

  • 家
  • 导游
  • 产品手册
  • 参考
  • 样品

经常问的问题

预计阅读时间： 4分钟

如果您在这里没有看到您的问题，请随时#docker-compose访问freenode IRC并询问社区。

我可以控制服务启动顺序吗？🔗

是-请参阅控制启动顺序。

为什么我的服务需要10秒钟才能重新创建或停止？🔗

撰写stop尝试，通过发送停止容器SIGTERM。然后，它等待10秒的默认超时。超时后，将a SIGKILL发送到容器以强制杀死它。如果您正在等待此超时，则意味着您的容器在收到SIGTERM信号后不会
关闭。

关于处理容器中信号的过程的问题已经有很多文章了。

要解决此问题，请尝试以下操作：

  • 确保您使用的EXEC形式CMD，并ENTRYPOINT 在你的Dockerfile。

    例如使用["program", "arg1", "arg2"]not "program arg1 arg2"。使用字符串形式会使Docker运行bash无法正确处理信号的进程。Compose始终使用JSON格式，因此如果您覆盖Compose文件中
    的命令或入口点，请不要担心。

  • 如果可以，请修改正在运行的应用程序，为添加一个显式信号处理程序SIGTERM。

  • 将设置为stop_signal应用程序知道如何处理的信号：

    web:
      build: .
      stop_signal: SIGINT

  • 如果您无法修改应用程序，请将应用程序包装在轻量级的初始化系统（如s6）或信号代理（如 dumb-init或 tini）中。这些包装纸均应SIGTERM妥善处理。

如何在同一主机上运行Compose文件的多个副本？🔗

Compose使用项目名称为项目的所有容器和其他资源创建唯一的标识符。要运行一个项目的多个副本，请使用-p命令行选项或COMPOSE_PROJECT_NAME环境变量来设置自定义项目名称。

什么之间的区别up，run以及start？🔗

通常，您需要docker-compose up。使用up启动或重新启动所有定义的服务docker-compose.yml。在默认的“附加”模式下，您会看到来自所有容器的所有日志。在“分离”模式（-d）中，启动容器后
Compose退出，但是容器继续在后台运行。

该docker-compose run命令用于运行“一次性”或“临时”任务。它要求您要运行的服务名称，并且仅启动正在运行的服务所依赖的服务的容器。使用run运行测试或执行管理任务，如删除或添加数据
的数据量的容器。该run命令的行为类似于docker run -ti，它打开了容器的交互式终端，并返回与容器中进程的退出状态匹配的退出状态。

该docker-compose start命令仅对重新启动先前创建但已停止的容器有用。它永远不会创建新的容器。

我可以在撰写文件中使用json而不是yaml吗？🔗

是。Yaml是json的超集，因此任何JSON文件都应为有效的Yaml。要将JSON文件与Compose一起使用，请指定要使用的文件名，例如：

docker-compose -f docker-compose.json up

我应该在COPY/ ADD或卷中包含我的代码吗？🔗

您可以在中使用COPY或ADD指令将代码添加到图像中 Dockerfile。如果您需要将代码与Docker映像一起重定位，例如在将代码发送到另一个环境（生产环境，CI等）时，这很有用。

volume如果要更改代码并立即看到更改，则应使用a ，例如在开发代码且服务器支持热代码重载或实时重载时。

在某些情况下，您可能要同时使用两者。您可以使用来使图像包含代码COPY，并volume在Compose文件中使用来在开发过程中包含来自主机的代码。该卷将覆盖映像的目录内容。

在哪里可以找到示例撰写文件？🔗

github上有很多Compose文件的示例。

撰写文档🔗

  • 安装撰写
  • 开始使用Django
  • 开始使用Rails
  • 开始使用WordPress
  • 命令行参考
  • 撰写文件参考

文档，docs，docker，compose，常见问题
评价此页面：

 
43
 
12
 
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
              ○ docker app status
              ○ docker app uninstall
              ○ docker app upgrade
              ○ docker app validate
              ○ docker app version
          ☆ docker attach
          ☆ docker build
          ☆ docker builder
              ○ docker builder
              ○ docker builder build
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
          ☆ docker checkpoint
              ○ docker checkpoint
              ○ docker checkpoint create
              ○ docker checkpoint ls
              ○ docker checkpoint rm
          ☆ docker cluster
              ○ docker cluster
              ○ docker cluster backup
              ○ docker cluster create
              ○ docker cluster inspect
              ○ docker cluster ls
              ○ docker cluster restore
              ○ docker cluster rm
              ○ docker cluster update
              ○ docker cluster version
          ☆ docker commit
          ☆ docker config
              ○ docker config
              ○ docker config create
              ○ docker config inspect
              ○ docker config ls
              ○ docker config rm
          ☆ docker container
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
          ☆ docker context
              ○ docker context
              ○ docker context create
              ○ docker context export
              ○ docker context import
              ○ docker context inspect
              ○ docker context ls
              ○ docker context rm
              ○ docker context update
              ○ docker context use
          ☆ docker cp
          ☆ docker create
          ☆ docker diff
          ☆ docker events
          ☆ docker exec
          ☆ docker export
          ☆ docker history
          ☆ docker image
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
          ☆ docker images
          ☆ docker import
          ☆ docker info
          ☆ docker inspect
          ☆ docker kill
          ☆ docker load
          ☆ docker login
          ☆ docker logout
          ☆ docker logs
          ☆ docker manifest
              ○ docker manifest
              ○ docker manifest annotate
              ○ docker manifest create
              ○ docker manifest inspect
              ○ docker manifest push
          ☆ docker network
              ○ docker network
              ○ docker network connect
              ○ docker network create
              ○ docker network disconnect
              ○ docker network inspect
              ○ docker network ls
              ○ docker network prune
              ○ docker network rm
          ☆ docker node
              ○ docker node
              ○ docker node demote
              ○ docker node inspect
              ○ docker node ls
              ○ docker node promote
              ○ docker node ps
              ○ docker node rm
              ○ docker node update
          ☆ docker pause
          ☆ docker plugin
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
          ☆ docker port
          ☆ docker ps
          ☆ docker pull
          ☆ docker push
          ☆ docker registry
              ○ docker registry
              ○ docker registry events
              ○ docker registry history
              ○ docker registry info
              ○ docker registry inspect
              ○ docker registry joblogs
              ○ docker registry ls
              ○ docker registry rmi
          ☆ docker rename
          ☆ docker restart
          ☆ docker rm
          ☆ docker rmi
          ☆ docker run
          ☆ docker save
          ☆ docker search
          ☆ docker secret
              ○ docker secret
              ○ docker secret create
              ○ docker secret inspect
              ○ docker secret ls
              ○ docker secret rm
          ☆ docker service
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
          ☆ docker stack
              ○ docker stack
              ○ docker stack deploy
              ○ docker stack ps
              ○ docker stack rm
              ○ docker stack services
          ☆ docker start
          ☆ docker stats
          ☆ docker stop
          ☆ docker swarm
              ○ docker swarm
              ○ docker swarm ca
              ○ docker swarm init
              ○ docker swarm join-token
              ○ docker swarm join
              ○ docker swarm leave
              ○ docker swarm unlock-key
              ○ docker swarm unlock
              ○ docker swarm update
          ☆ docker system
              ○ docker system
              ○ docker system df
              ○ docker system events
              ○ docker system info
              ○ docker system prune
          ☆ docker tag
          ☆ docker top
          ☆ docker trust
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
          ☆ docker unpause
          ☆ docker update
          ☆ docker version
          ☆ docker volume
              ○ docker volume create
              ○ docker volume inspect
              ○ docker volume ls
              ○ docker volume prune
              ○ docker volume rm
          ☆ docker wait
      □ Docker Compose CLI reference
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
      □ Daemon CLI (dockerd)
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
      □ 版本3
      □ 版本2
      □ 版本1
      □ 关于版本和升级
      □ 经常问的问题
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

  • 我可以控制服务启动顺序吗？
  • 为什么我的服务需要10秒钟才能重新创建或停止？
  • 如何在同一主机上运行Compose文件的多个副本？
  • 启动，运行和启动之间有什么区别？
  • 我可以在撰写文件中使用json而不是yaml吗？
  • 我应该在COPY / ADD或卷中包含代码吗？
  • 在哪里可以找到示例撰写文件？
  • 撰写文件

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

[ipv]
Google 翻译

原文

提供更好的翻译建议
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
[ipv]
