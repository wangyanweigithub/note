### 用法
1. tornado finsh和write的区别

    1. self.finish()代表回应生成的终结，并不代表着请求处理逻辑的终结。
		假设你有一个block的逻辑是和回应无关的，那么放在self.finish()的后面可以显著的缩短响应时间。
		所以，如果你确定自己的逻辑需要立即返回，可以在self.finish()后立刻return。Tornado在将这个自由留给了你自己。

    2. 另外一个理由是，在call stack里让顶端的函数去弹出一个非顶端的函数，这个逻辑有点奇怪。唯一能够提供退出的机制就是异常了。
		但是在正常逻辑里面使用异常去实现一个功能，也是很怪的逻辑。

    3. 没错  同理还有self.render/self.write
		我们在所有这种response语句前加return 例如  return self.redirect(‘/’)

    至此，这个问题得到了完美的解决和解答，而我想后者才是更重要的。

2. http post 提交时，如果是key－value格式，即字典格式，tornado接收在self.request.arguments中，若是string格式，即json格式，没有key的话，
	tornado接收在body中，需要自己去body中转化json格式。如果传过来的是key－value的话，body中时用&拼接的key－value。

3. http post 请求有四种方式。对应不同的数据格式。

4. requests
    - get方法的params 参数值时字典格式，requests会自动将参数拼接为正常的get参数方法。
    - post方法，data参数格式应该是字典，json参数后跟字典，但会自动转化为json。       
		
			post(url,data=json.dumps(dic)) == post(url,json=dic) 
