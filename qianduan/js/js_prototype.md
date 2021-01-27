### js的原型继承

1. 每个实例对象都有一个私有属性(__proto__)指向它的构造函数的原型对象(prototype).
该原型对象也有一个自己的原型对象(__proto__),层层向上指导一个对象的原型对象为null.
根据定义,null没有原型,并作为这个原型连中的最后一个环节.

	- 只有实例对象有(__proto__)
	- 只有构造函数有prototype,指向一个对象

2. 注意:
	1. prototype是一个对象,js的对象就是python的字典.
	2. prototype的__proto__,指向另一个对象,是创建这个这个对象的构造函数的函数的原型.
	3. prototype的原型连就是属性查找时的路径,一个对象的属性查找顺序:
		- 先查这个对象的构造函数赋予的属性
		- 在查这个对象继承的__proto__的属性.这个proto就相当于它的父类的属性了.
		- 然后继续查找这个原型的__proto__指向对象的属性.

	4. 所以,一个构造函数的prototype应该有两部分对象:
		- 直接在prototype上定义的属性,方法/ 类似于python的类方法,而不是对象方法.
		- 一个__proto__, 指向创建创建这个prototype的构造方法的prototype.

	5. 一个构造函数的prototype,除了它的__proto__,指向创建它的构造函数的prototype外,应该是空的.

	6. 所以prototype本身和它的构造函数是没有关系.也不是构造函数指定的属性的数据模型.
	
	7. 继承时,调用父原型的构造方法可以继承在构造函数中赋予的属性,但是父原型上定义的属性没办法继承.所以原型继承分两步:
		- 调用父原型的构造方法
		- 将子原型的__proto__指向父原型的原型.及子构造函数的prototype应该是父构造方法new的对象.

		- 但是如果直接将子构造函数的prototype设置为new 父构造函数, 子构造函数的原型就没有办法设置属于自己的属性.因为子的prototype的命名空间没有了.
		生成的对象的__proto__,直接就是父的protype
		```
		function Student() {
			this.name = "student";
			this.age = 23;
		}

		function Pri() {
			Student.call(this, arguments);
			this.hello = function () { console.log("hello"); }
		}

		Student.prototype.sex = 'F'

		> p1 = new Pri()
		Pri { name: 'student', age: 23, hello: [Function] }

		> s1 = new Student()
		Student { name: 'student', age: 23 }

		> s1.sex
		'F'

		> p1.sex        //因为只调用了父对象的构造函数,而没有继承prototype
		undefined

		> Pri.prototype = new Student()  //将子构造函数的原型设置为父对象,
										 //可以使用父对象的prototype属性.
		Student { name: 'student', age: 23 }

		> p2 = new Pri()
		Student { name: 'student', age: 23, hello: [Function] }

		> p2
		Student { name: 'student', age: 23, hello: [Function] }

		> p2.sex
		'F'
		> p2.__proto__  //但是,子构造函数生成的对象的__proto__直接是父的prototype,
		Student { name: 'student', age: 23 }
		//这样还是可以在子对象的prototype上设置独属于自类原型属性,只不过是将属性设置到了父构造函数的对象上.

		//这样的问题在于Pri的子属性和Pri的__prot__都是一样的,都是Student,这样就无法将Pri的原型prototype定义的属性,遗传下去.

		function fri() {
			Pri.call(this, arguments);
			this.state = 'fri'
		}

		fri.prototype = new Pri()
		> f1.__proto__.__proto__
		Student { name: 'student', age: 23, sex: 'M' }
		> f1.__proto__
		Student { name: 'student', age: 23, hello: [Function] }
		//Pri的原型上的属性丢掉了.所以需要一个空的构造函数F()
		```

3. prototype继承最佳实践:
	1. class 语法糖

	2. 设置空函数F,将子构造函数的prototype设置为new F(), 将F的prototype设置为父prototype,
	然后修正子的构造函数的protype的constructor为子构造函数本身.

4. 为什么要做A.prototype.constructor=A这样的修正:
	1. [资料](https://www.cnblogs.com/SheilaSun/p/4397918.html)

	2. stackflow:
		构造函数属性对内部任何事物都没有任何实际的区别。仅当您的代码明确使用它时，
	它才有用。例如，您可能决定需要每个对象都具有对创建它的实际构造函数的引用。
	如果是这样，那么在设置继承时，需要通过将对象分配给构造函数的原型属性来显式设
	置构造函数属性，如您的示例。
