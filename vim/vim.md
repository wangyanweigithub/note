## 替换
1. 在vim中的123开头的行前面加上一行文字:
	- %s/123/zheshiyiju\r123

		1. -r是回车,-n是换行,这句话的意思是将123替换成为"zheshiyijuxinhua\r123",
			等于是将123替换了,而替换的新语句里面有一个回车,就换行了,换行后开头的又正好是
		被替换的字母,效果和在头部插一行类似.

	- :g/123/normal Ozheshiyijuxinhang
		g匹配到指定文字,normal转换到常规模式,O是在上面插入一行,O后面的字是插入的字.

	- 注意:vim 中的换行是\r,而不是\n.

## 缩写
1. iab
	1. iab不起作用,后来证实是pathogen的问题,注释掉这一行.
	2. 去掉pathogen后,minbuffer没法使用,需要minbuffer打开pathogen

## 折叠
1. 小写一层层,大写递归所有
	- zo/zO: (open)
	- zc/zC: (close)
	- za/zA: (triggle)
	- zr/zm: 一层层
	- zR/zM: 打开/关闭所有的折叠

## 反向引用匹配模式
1. %s/\(.*\)—\(.*\)/\2—\1/g
	\1和\2表示引用第一个括号，第二个括号中的匹配到的内容
