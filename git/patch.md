# git 打补丁方法总结
> https://blog.csdn.net/sinat_20059415/article/details/80598347版权

## 应用场景
1. 有两个git库（同一个git库不同分支可以用cherry-pick），两个git库代码是相关联的，
	要有选择的定期将其中一个git库的修改merge到另外一个库中。

## 方法
1. 准备工作：初始化两个git仓库，创建一个original.txt文件，
内部写有“This is original text”，然后复制到第一个仓库和第二个仓库。
修改第一个仓库中的original.txt文件，添加一行“This is modify”，修改分别提交。

### git diff + git apply
1. 生成patch:  git diff 前一个提交目标提交 > 1.patch

2. 应用patch: git apply 1.patch

3. 可以看到patch已经打上去了，但是修改信息啥的还得自己来写一下，不是很完美。

## git format-patch+ git am 单个patch
1. 使用git format-patch生成补丁

（git format-patch -1 commitid是提取单个commitid对应的patch）

2. 使用git am 应用补丁

（一般建议git am应用补丁前使用git am --abort）

3. 可以发现这种打补丁方式会将提交信息直接都打进去，非常方便。

## git format-patch+ git am 多个patch
1. 生成多个patch(git format-patch commitid会生成这个commit之后的所有patch,不包含该commit)

2. 打补丁(git am *.patch可以打进去所有patch)

3. 有冲突的话解决冲突，然后git add对应文件，git am --resolve。
不能解决想后面再说就git am --skip，具体使用方法参考git am --help

## PS
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
