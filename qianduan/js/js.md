
<!-- vim-markdown-toc GFM -->

* [js]
	* [JavaScript 的组成]
	* [基本语法]
		* [字符串]
		* [数组]
	* [js面向对象编程]
		* [语句]
		* [map和set]
	* [面向对象编程]
		* [property]
		* [js创建对象的方法]
			* [for]

<!-- vim-markdown-toc -->
# js
> JavaScript，通常缩写为 JS，是一种高级的，解释执行的编程语言。
> JavaScript 是一门基于原型、函数先行的语言，是一门多范式的语言，它支持面向对象编程，命令式编程，以及函数式编程。
> 它提供语法来操控文本、数组、日期以及正则表达式等，<font color=red>不支持 I/O</font>，
比如网络、存储和图形等，但这些都可以由它的宿主环境提供支持

## JavaScript 的组成
	- ECMAScript：JavaScript 的语法标准。
	- DOM：JavaScript 操作网页上的元素的 API。
	- BOM：JavaScript 操作浏览器的部分功能的 API。

## 基本语法
1. javascript 中原始类型：还有引用类型
	- undefined:表示未定义，即声明了变量却没有赋值，此类型只有一个值。
	- null:此类型同样只有一个值，表示尚未存在的对象，如果函数／方法要返回对象，当找不到该对象，通常返回的是null。
	- number:数字，包括整数和小数
	- string:单引号或双引号
	- boolean:true和falses ，但值为false,0,null,undefined,NaN或者空字符串时表示假，其他值则表示真。
	- array
	- object

### 字符串
1. 多行字符串:反引号包围(``)

2. 模板字符串
	1. ${name},表示是使用前面name变量的值替换.

3. <font color=red>字符串是不可变的,如果对字符串某个索引赋值,不会有任何错误,但是,也没有任何效果.</font>

4. 函数:骆驼命名法
	- toUperCase
	- toLowerCase: a.toLowerCase();
	- indexOf(string): 返回指定字符串出现的位置
	- substring(start, end): 返回指定索引区间的子串.

### 数组
1. 可以直接改变array.length,来改变length的长度(多出来是undefine, 少的剪切掉)

2. 函数:
	- indexOf类似于string: 找到返回index,没找到返回-1
		array.indexOf('a')

	- slice: 对应于string的substring,截取Array部分元素,返回一个新的Array
		- array.slice(0, 2)
		- array.slice(0)//0 - end
		- array.slice()// copy a new array

	- push, pop
		- push 末尾添加一个新的元素
		- pop 删除末尾的一个元素

	- unshift, shift
		- shift 把Array第一个元素删除
		- unshift 往Array头部添加元素

	- sort
	
	- reverse

	- splice: 从指定的索引开始删除若干元素,然后在从该位置添加若干元素.
	```
	var arr = ['one', 'two', 'three'];
	arr.splice(1, 2, '2', '3') //返回删除的元素

	只删除,不添加
	arr.splice(1, 2)

	只添加不删除
	arr.splice(1, 0, '2', '3')
	```

	- concat: 把当前的Array和另一个Array连接起来,并返回一个新的Array
		- var new_arr = arr.concat([1, 2, 3]);

		- concat没有改变原array,而是返回一个新的Array

		- concat可以接受任意个元素和Array,并且把array拆开,然后全部添加到新的Array里.
		```
		arr.concant(1, 2, [1, 2])
		```

	- join: 把当前Array的每个元素都用指定的字符串连接起来,然后返回连接后的字符串:
	```
	var arr = ['a', 'b', 'c']
	arr.join('-');
	```

3. 多维数组: 
	var arr = [[1, 2, 3],[100, 200, 300]]

4. 由于Array也是对象,它的每个元素的索引被视为对象的属性,因此,for ... in 循环可以直接循环出Array的索引.
## js面向对象编程
1. 例子:
```
var test = {
	getEle: {
		tag: function(tagName){
			return document.getElementByTagName(tagName);
		},
		id: function(idName) {
			return documetn.getElementById(idName);
		}
	},

	setCss: {
		setStyle: function(arr){
			for(var i=0;i<arr.length;i++) {
			  arr[i].style.backgroundColor="red";
			}
		}
	}
}

var divs = test.getEle.tag("div");
test.setCss.setStyle(divs);
```

2. js中的对象像是python中的字典,只不过key可以不加引号. json和python中的字典一样,都要加引号.

3. 如果key不是标准变量名,可以使用中括号访问: a["midd-name"]

4. js的对象是动态类型,可以随时添加新的属性

5. 访问不存在的属性不报错,返回undefined

6. 检测对象是否包含某个属性使用 in:
	'name' in object

7. 要判断一个属性是否是自己包含的而不是继承到的:
	object.hasOwnProperty("namej")

### 语句
1. if else 和c完全一样

2. for 和c完全一样

3. for ... in ...
	for (var i in list) { ... } //和python的不同在于需要括号

4. while 和do .. while 和c一样.

### map和set

## 面向对象编程
### property
1. 一个对象根据另外一个对象创建,并且继承这个对象的所有属性,这个对象继承另一个对象,就是说这个对象的原型是另一个对象.

2. propery继承,是因为js中只有对象,没有类,所以一个对象直接根据另一个对象创建,并继承它的所有属性.

3. xiaoming.__proto__ = Student: xiaoming对象就继承了Student的所有属性.

4. 原型继承并不是一个对象根据另一个对象来创建,而是将一个对象的原型指定另一个对象,这样这个对象调用一个属性,在自己的属性中找不到,就会到原型对象中查找.

5. 一个对象可以随时将自己的原型指定到别的对象,只是属性查找的区别.

6. 一般不要使用obj.__proto__去改变一个原型, 而是使用Obejct.create()方法传入一个原型对象.

7. js对每个创建的对象都会设置一个原型,指向它的原型对象.

### js创建对象的方法
1. Object.create()
```
var Student = {
	name: 'Robot',
	height: 1.2
}

xiaoming = Object.create(Student)
xiaoming.name = 'xiaoming'

### 构造函数
1. 通过字面量创建对象:
```
var student={
	name: 'zhangsan',
	age: 18,
	gender: 'male',
	sayHi: function(){
		conson.log("hi, my name is " + this.name);
	}
}
```

2. 构造函数:
	1. eg
	```
	function Student(name, age, gender){
		this.name = name;
		this.age = age;
		this.gender = gender;
		this.sayHi = function(){
			console.log("hi, my name is " + this.name);
		}
	}
	var s1 = new Student('zhang', 18, 'male');
	```

	2. 来看看构造函数与工厂函数的区别：
		- 首先在构造函数内没有创建对象，而是使用 this 关键字，将属性和方法赋给了 this 对象。
		- 构造函数内没有 return 语句，this 属性默认下是构造函数的返回值。
		- 函数名使用的是大写的 Student。
		- 用 new 运算符和类名 Student 创建对象
		- 注意: 如果调用构造函数没有使用new,那它只是一个普通方法,并且this绑定window,将里面的属性全部设置成了全局变量.一定不要普通方法调用构造函数.

		- 函数不在对象里面,函数里有this,函数没有返回值就是一个构造函数.

	3. 构造函数的问题:
		1. 创建多个对象,每个对象都会创建一次sayHi这个函数,每个对象都是独立的. 解决办法使用原型

### 原型/prototype
1. 在js中,每一个<font color=red>函数</font>都有一个prototype属性,指向另一个prototype对象,
	这个prototype对象的所有属性和方法,都会被构造函数实例对象继承.

	```
	function Student(name, age, gender){
		this.name = name;
		this.age = age;
		this.gender = gender;
	}
	Student.prototype.sayHi=function(){
		console.log("hi");
	}
	var s1 = new Student("zhang", 12, "male");
	s1.sayHi();
	var s2 = new Student("li", 12, "male");
	s2.sayHi();
	console.log(s1.sayHi==s2.sayHi) //结果为true
	```

2. 每一个函数都有一个prototype属性,prototype属性默认都有一个constructor属性,指向prototype对象所在的函数.
	也就是说,函数的有一个prototype,prototype的constructor又指向函数对象本身.
	```
	function F(){}
	console.log(F.prototype.contructor==F); //true
	```

3. `__proto__`: 有构造函数创建的实例对象,有一个`__proto__`指针,指向构造函数的prototype
```
function F(){}
var a = new F();
console.log(a.__proto__==F.prototype); //true
```

4. 原型链
> 所有的对象都有原型，而原型也是对象，也就是说原型也有原型，那么如此下去，也就组成了我们的原型链。

	1. 属性搜索原则
		属性搜索原则，也就是属性的查找顺序，在访问对象的成员的时候，会遵循以下原则：

		- 首先从对象实例本身开始找，如果找到了这个属性或者方法，则返回。
		- 如果对象实例本身没有找到，就从它的原型中去找，如果找到了，则返回。
		- 如果对象实例的原型中也没找到，则从它的原型的原型中去找，如果找到了，则返回。
		- 一直按着原型链查找下去，找到就返回，如果在原型链的末端还没有找到的话，那么如果查找的是属性则返回 undefined，
			如果查找的是方法则返回 xxx is not a function。

	2. 更简单的原型语法
		在前面的例子中，我们是使用 xxx.prototype. 然后加上属性名或者方法名来写原型，
		但是每添加一个属性或者方法就写一次显得有点麻烦，因此我们可以用一个包含所有属性和方法的对象字面量来重写整个原型对象：
		```
		function Student(name, age, gender) {
			this.name = name;
			this.age = age;
			this.gender = gender;
		}
		Student.prototype = {
			hobby:"study",
			sayHi:function(){
			console.log("hi");
			}
		}
		var s1 = new Student("wangwu",18,"male");
		console.log(Student.prototype.constructor === Student);//结果为 false
		```
		<font color=red>但是这样写也有一个问题，那就是原型对象丢失了 constructor 成员</font>

	3. 所以为了保持 constructor 成员的指向正确，建议的写法是:
	```
	function Student(name, age, gender) {
		this.name = name;
		this.age = age;
		this.gender = gender;
	}
	Student.prototype = {
		constructor: Student, //手动将 constructor 指向正确的构造函数
		hobby:"study",
		sayHi:function(){
		console.log("hi");
		}
	}
	var s1 = new Student("wangwu",18,"male");
	console.log(Student.prototype.constructor === Student);//结果为 true
	```

	4. 总结:
		1. js使用prototype来定义所有对象都要继承的属性和方法,<font color=green>有点类似与其他语言的类方法,类属性</font>.所有对象都会继承这些属性和方法.
		2. 每个构造函数有一个原型对象(prototype),原型对象包含一个指向构造函数的指针constructor,而实例对象都包含一个指向原型对象的内部指针`__proto__`

## 函数进阶:
### 高级函数
#### call
> call() 方法调用一个函数, 其具有一个指定的 this 值和分别地提供的参数(参数的列表)。语法为：

```
fun.call(thisArg, arg1, arg2, ...)
```

2. 注：

	- thisArg 指的是在 fun 函数中指定的 this 的值。如果指定了 null 或者 undefined 则内部 this 指向 window，
		同时值为原始值(数字，字符串，布尔值)的 this 会指向该原始值的自动包装对象。是一个可选项。
	- arg1, arg2, ...指定的参数列表。也是可选项。
	- 使用调用者提供的 this 值和参数调用该函数的返回值。若该方法没有返回值，则返回 undefined。
	- call() 允许为不同的对象分配和调用属于一个对象的函数/方法。
	- call() 提供新的 this 值给当前调用的函数/方法。你可以使用 call 来实现继承：
		写一个方法，然后让另外一个新的对象来继承它（而不是在新对象中再写一次这个方法）。

3. eg.
	1. 使用 call() 方法调用函数并且指定上下文的 'this'。
	```
	function foods(){
	}
	foods.prototype={
		price: "15", 
		say: funciton(){
		   console.log("my price: "+this.price);
		}
	}

	var apple = new foods();
	orange = {
		price: "10"
	}
	apple.say.call(orange);
	```

	2. 在一个子构造函数中，你可以通过调用父构造函数的 call() 方法来实现继承。在控制台输入如下代码：
	```
	function Father(name, age) {
		this.name = name;
		this.age = age;
	}

	function Son(name, age) {
		Father.call(this, name, age);
		this.hobby = 'study';
	}

	var S1 = new Son('zhangsan', 18);
	S1; // Son {name: "zhangsan", age: 18, hobby: "study"}
	```

#### apply
> apply() 方法与 call() 方法类似，唯一的区别是 call() 方法接受的是参数，apply() 方法接受的是数组。语法为：

```
fun.apply(thisArg, [argsArray])
```

1. 使用 apply() 方法将数组添加到另一个数组。
	1. eg:
	```
	var array = ['a', 'b','c'];
	var nums = [1, 2, 3];
	array.push.apply(array, nums);
	array //["a", "b", "c", 1, 2, 3]
	```
	2. 注：concat() 方法连接数组，不会改变原数组，而是创建一个新数组。
		而使用 push 是接受可变数量的参数的方式来添加元素。使用 apply 则可以连接两个数组

2. 使用 apply() 方法和内置函数。

	1. 例子：
	```
	var numbers = [7, 10, 2, 1, 11, 9];
	var max = Math.max.apply(null, numbers); 
	max; //11

	```

	2. 注：直接使用 max() 方法的写法为： Math.max(7, 10, 2, 1, 11, 9);

#### bind
> bind() 方法创建一个新的函数（称为绑定函数），在调用时设置 this 关键字为提供的值。
	并在调用新函数时，将给定参数列表作为原函数的参数序列的前若干项。
	
1. 语法为：fun.bind(thisArg[, arg1[, arg2[, ...]]])

2. 注：
	- 参数 thisArg ：当绑定函数被调用时，该参数会作为原函数运行时的 this 指向。
	- 当使用 new 操作符调用绑定函数时，该参数无效。
	- 参数：arg1, arg2, ...表示当目标函数被调用时，预先添加到绑定函数的参数列表中的参数。

3. eg
```
var bin = function(){
    console.log(this.x);
}
var foo = {
x:10
}
bin(); // undefined
var func = bin.bind(foo); //创建一个新函数把 'this' 绑定到 foo 对象
func(); // 10
```
4. eg
```
this.num = 6;
var test = {
  num: 66,
  getNum: function() { return this.num; }
};

test.getNum(); // 返回 66

var newTest = test.getNum;
newTest(); // 返回 6, 在这种情况下，"this"指向全局作用域

// 创建一个新函数，将"this"绑定到 test 对象
var bindgetNum = newTest.bind(test);
bindgetNum(); // 返回 66
```

#### for
1. for (i in a) console.log(i)
	- 如果a是数组,i是数组的下标
	- 如果a是对象,i是对象的key.

	代码可以根据i的值取出a所保存的对象.而不是直接取出a中的对象.

