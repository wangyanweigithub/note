
<!-- vim-markdown-toc GFM -->

* [命令](#命令)
* [利用git,使U盘作为仓库](#利用git使u盘作为仓库)
* [git 打补丁方法总结](#git-打补丁方法总结)
	* [应用场景](#应用场景)
	* [方法](#方法)
		* [git diff + git apply](#git-diff--git-apply)
		* [git format-patch+ git am 单个patch](#git-format-patch-git-am-单个patch)
		* [git format-patch+ git am 多个patch](#git-format-patch-git-am-多个patch)
	* [使用git format-patch生成所需要的patch:](#使用git-format-patch生成所需要的patch)
	* [应用patch：](#应用patch)

<!-- vim-markdown-toc -->
## 命令
1. 初始化一个Git仓库，使用 git init命令

2. git status 查看仓库当前的状态。  git diff 查看修改的记录。git log 查看从最近到最远的提交日志。

3. 退回到上一个版本：git reset 命令。
	1. git reset --hard HEAD^  // 在git中，用HEAD表示当前版本，上一个版本是HEAD^,上上一个版本就是^^，

	2. git reset --hard 3628164 //恢复commit id 以3628164开头的版本

4. 使用reset命令后，再使用git log命令发现最新的那个版本已经看不到了，如果还想恢复最新的版本，只要知道那个版本的commit id就可以，
如果找不到commit id，可以使用git reflog命令，这个命令用来记录你的每一次命令。

5. 三个区域
	- 工作区：你在电脑里能看到的目录
	- 版本库：工作区有一个隐藏的目录.git 这个不算工作区，而是git的版本库。
	- 暂存区：git版本库中最重要的就是称为stage（或者叫index）的暂存区，还有git为我们自动创建的第一个分支master以及指向master的一个指针叫HEAD。

6. git add 和git commit
	1. git add命令实际上就是把要提交的所有修改放到暂存区(Stage)

	2. 然后git commit就可以一次性把暂存区的所有修改提交到分支。
	一旦提交后，如果没有对工作区做任何修改，那个工作区就是干净的。

7. 查看工作区和版本库里面最新版本的区别：git diff HEAD  -- readme.txt

8. git checkout -- file丢弃工作区的修改，就是让这个文件回到最后一次 git commit 或是git add时的状态。

9. git reset HEAD file可以把暂存区的修改撤销掉(unstage),重新放回工作区。
	- git reset HEAD readme.txt 

	- git reset命令既可以回退版本，也可以把暂存区的修改回退到工作区。
	当退回的是当前版本时表示把暂存区的修改回退到工作区。

10. eg
```
git init
git add README.md
git commit -m "first commit"
git remote add origin git@github.com:wangyanweigithub/wang_note.git
git push -u origin master
```

11. 创建分支dev，然后切换到dev分支：
	- $git checkout -b dev
    
	- git checkout 命令加上 -b参数表示创建并切换，相当于以下两条命令：
		- git branch dev
		- git checkout dev

12. git branch 命令查看当前分支：
    git branch 命令会列出所有分支，当前分支前面会标一个*号。然后我们就可以在dev分支上正常提交。

13. 当dev分支的工作完成，我们就可以切换回master分支：
    git checkout master

14. 把dev分支的工作成果合并到master分支上：
    $git merge dev
    Updating d12efd8
    Fast-forward
      .......
    git merge 命令用于合并指定分支到当前分支。Fast-forward 信息告诉我们，这次合并时“快进模式”，也就是直接把master指向dev的当前提交，所以合并速度非常快。

15. 并完成后，就可以放心的删除dev分支了：
        $git branch -d dev
        Deleted branch dev (was fec145a).
    
16. 删除后，查看branch，就只剩下master分支了：
    $git branch
    *master

17. Git鼓励大量使用分支：

	- 查看分支：git branch

	- 创建分支：git branch <name>

	- 切换分支：git checkout <name>

	- 创建+切换分支：git checkout -b <name>

	- 合并某分支到当前分支：git merge <name>

	- 删除分支：git branch -d <name>

11. 合并分支冲突，需要手动更改文件，然后提交。

## 利用git,使U盘作为仓库
> [本文链接](https://blog.csdn.net/u010928583/article/details/86496734)

1. U盘作为仓库操作步骤

	- 首先U盘上创建一个repository 名称： fill_backup_repos

		git init –bare  fill_backup_repos  //裸库，没有work目录

	- 电脑文件夹创建一个源项目名称：git_myfill

		git init git_myfill

	- 放入需要同步管理的文件
		git add .    //添加文件、如果有不需要管理的文件，加入 .gitignore文件；

		git commit -m "initialized."   //提交到本地仓库

		git remote add usb F:/fill_backup_repos    //把u盘上的裸库fill_backup添加为远程仓库

		git push usb master

	- 在U盘上的fill_backup 裸仓库里头建立一个本地仓库

		git init  myfill //建一个本地仓库

		git remote add usb F:/fill_backup_repos     //把u盘上的裸库fill_backup_repos添加为远程仓库

		git pull myusb master   //完成代码同步

2. 备注remote相关命令：
	1. 查看remote： git remote
	2. 删除remote: git remote remove

## git 打补丁方法总结
> https://blog.csdn.net/sinat_20059415/article/details/80598347版权

### 应用场景
1. 有两个git库（同一个git库不同分支可以用cherry-pick），两个git库代码是相关联的，
	要有选择的定期将其中一个git库的修改merge到另外一个库中。

### 方法
1. 准备工作：初始化两个git仓库，创建一个original.txt文件，
内部写有“This is original text”，然后复制到第一个仓库和第二个仓库。
修改第一个仓库中的original.txt文件，添加一行“This is modify”，修改分别提交。

#### git diff + git apply
1. 生成patch:  git diff 前一个提交目标提交 > 1.patch

2. 应用patch: git apply 1.patch

3. 可以看到patch已经打上去了，但是修改信息啥的还得自己来写一下，不是很完美。

#### git format-patch+ git am 单个patch
1. 使用git format-patch生成补丁

（git format-patch -1 commitid是提取单个commitid对应的patch）

2. 使用git am 应用补丁

（一般建议git am应用补丁前使用git am --abort）

3. 可以发现这种打补丁方式会将提交信息直接都打进去，非常方便。

#### git format-patch+ git am 多个patch
1. 生成多个patch(git format-patch commitid会生成这个commit之后的所有patch,不包含该commit)

2. 打补丁(git am *.patch可以打进去所有patch)

3. 有冲突的话解决冲突，然后git add对应文件，git am --resolve。
不能解决想后面再说就git am --skip，具体使用方法参考git am --help

### 使用git format-patch生成所需要的patch:
1. 当前分支所有超前master的提交：
	git format-patch -M master

2. 某次提交以后的所有patch:
	git format-patch 4e16 --4e16指的是commit名

3. 从根到指定提交的所有patch:
	git format-patch --root 4e16
	
4. 某两次提交之间的所有patch:
	git format-patch 365a..4e16 --365a和4e16分别对应两次提交的名称

5. 某次提交（含）之前的几次提交：
	git format-patch –n 07fe --n指patch数，07fe对应提交的名称故，单次提交即为：

	git format-patch -1 07fe

6. git format-patch生成的补丁文件默认从1开始顺序编号，并使用对应提交信息中的第一行作为文件名。

7. 如果使用了-- numbered-files选项，则文件名只有编号，不包含提交信息；

8. 如果指定了--stdout选项，可指定输出位置，如当所有patch输出到一个文件；
可指定-o <dir>指定patch的存放目录；

### 应用patch：
1. 先检查patch文件：git apply --stat newpatch.patch

2. 检查能否应用成功：git apply --check newpatch.patch
	打补丁：git am  newpatch.patch
