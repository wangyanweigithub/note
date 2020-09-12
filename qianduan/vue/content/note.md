## 绑定位置
> 注意: html标签都是key="value"

1. html标签上的内容,属性名不加引号,属性值都是引号括住的字符串,因此vue绑定的几种情况:

### 写入标签中
#### 将vue绑定到属性值:
> 需要写入到html标签内的vue元素都绑定到了一个双引号括住的字符串.
 vif = "message"
 vbind:id="message"

#### vue绑定到标签属性:
> 写入html标签,但是添加了一个标签属性,而不是属性值:

### vue绑定在语句中
1. 类似语言的指令,都是绑定在标签中,属性值是vue的变量.
	1. vfor: <div vfor="todo in todos">
       
### 写在标签包裹的内容中:
1. 写到html标签内容上的可以直接双的那括号包裹{{}}


## question
1. slot 中slot解构,父级作用域为slot bind的属性命名,是否会覆盖子组件的属性值.
        不会: 父级作用域的赋值是将子组件的命名空间起一个别名,它指向的就是slot的所有属性,
	                slot依然要bind需要传递到父作用域空间的值
	                父作用域依然可以使用slot的整个作用域属性,通过<a slot.default="slotprops">, 
	                slotprops其实是slot属性空间的别名.
	
2. () => import() 语法:
3. promise 语法
4. webpack 的require()语法.
