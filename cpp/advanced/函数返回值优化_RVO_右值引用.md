## 函数返回值
1. 函数返回值的优化技术: RVO和右值引用
[img](https://blog.csdn.net/dreamvyps/article/details/82467608)
```
#include <iostream>
using namespace std;
class Moveable {
public:
	Moveable():h(new int(3)) {
		cout << "construct " << endl;
	}

	~Moveable() {
		delete h;
	}

	Moveable(const Moveable& m): h(new int(*m.h)) {
		cout << "copy constructor" << endl;
	}

	/* 转移构造
	Moveable(Moveable&& m): h(m.h) {
		cout << "move constructor" << endl;
		m.h = nullptr;
	}
	*/
	int* h;
};

Moveable GetTemp() {
	Moveable tmp;
	return tmp;
}

int main(int argc, char** argv) {
	Moveable a = GetTemp();
	return 0;
}
```

2. 如果正常的编绎，我这边使用的是clang-700.1.81，

	1. 编绎命令：g++ -std=c++11 demo_3.cpp -o demo_3

	2. 执行结果如下：constructor

	3. 即是只有一个构造函数被调用。

	4. 场景1分析：
		- 这个情况下，没有临时变量产生？没有拷贝构造被调用，直接就一个构造函数被调用了。
		- 为啥？这是编绎的优化起作用了，叫作RVO技术，可以自行百度一下。

	5. 场景2：
		1. 如何才能看到拷贝构造被调用呢？即是产生临时变量，这才是正常调用的一个过程。

			编绎命令：g++ -std=c++11 -fno-elide-constructors demo_3.cpp -o demo_3

		2. 执行结果：
			```
			constructor			
			copy constructor			
			copy constructor			
			```

		3. 分析：我们看到，-fno-elide-constructors这个选项关闭了RVO优化，直接出现了两次拷贝构造的调用。
			第一次是return变量tmp传给临时变量。第二次是临时变量转值给变量a

	6. 场景3：我们把转移构造函数的注释打开。代码如下：
	```
	#include <iostream>
	using namespace std;
	class Moveable {
	public:
		Moveable():h(new int(3)) {
			cout << "construct " << endl;
		}

		~Moveable() {
			delete h;
		}

		Moveable(const Moveable& m): h(new int(*m.h)) {
			cout << "copy constructor" << endl;
		}

		Moveable(Moveable&& m): h(m.h) {
			cout << "move constructor" << endl;
			m.h = nullptr;
		}

		int* h;
	};

	Moveable GetTemp() {
		Moveable tmp;
		return tmp;
	}

	int main(int argc, char** argv) {
		Moveable a = GetTemp();
		return 0;
	}
	```

	7. 编绎命令：g++ -std=c++11 -fno-elide-constructors demo_3.cpp -o demo_3
		1. 关闭RVO优化，执行结果：

				constructor			
				move constructor			
				move constructor			
				
			这里是调用了两次转移构造。
			我们知道，第二次转移构造的时候，是从临时变量转移到变量a中，
			因为临时变量是个右值引用，触发的是转移构造，这个可以理解。
			问题是：第一个return tmp的时候，为什么是转移构造？

			tmp不是一个有名变量吗？怎么不是使用拷贝构造函数？
			猜想是不是编绎器做的优化？不要急，我们看看下面这个场景。

	8. 场景4：
	```
	#include <iostream>
	using namespace std;

	class Moveable {
	public:
		Moveable() {
			cout << "constructor" << endl;
		}

		~Moveable() {
		}

		Moveable(const Moveable& m) {
			cout << "copy constructor" << endl;
		}

		Moveable(Moveable&& m) {
			cout << "move constructor" << endl;
		}
	};

	Moveable GetTemp(bool which) {
		Moveable tmp1, tmp2;
		return which ? tmp1 : tmp2;
	}

	int main(int argc, char** argv) {
		Moveable a = GetTemp(true);
		return 0;
	}
	```

		1. 在GetTemp要返回处，加一个多目运算做逻辑判断。执行结果如下：
		```
		constructor	
		constructor	
		copy constructor	
		move constructor	
		```

		2. 分析：这次，return tmp到函数外的时候，就真实的执行了拷贝构造，而不会被编绎器优化。
			所以场景3中的，return tmp被当成右值，其实是编绎器进行的优化。

		3. 场景4中，如果还想让返回出去的临时变量赋值调用转移构造，可以使用std::move()去强制转换。

				即是：return which ? tmp1 : tmp2; 修改为

				return std::move(which ? tmp1 : tmp2)，即是不会调用拷贝构造函数。

	9. 别外，有个stackoverflow讲述了这个问题：

	https://stackoverflow.com/questions/14856344/when-should-stdmove-be-used-on-a-function-return-value

	总结：这里简单总结了，RVO与函数返回值转换成右值引用的问题。希望后续对这些语言特征有更深入理解。
