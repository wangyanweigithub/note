### 新类型
> long long int

1. long long int 并不是 C++11 最先引入的，其实早在 C99，long long int 就已经被纳入 C 标准中，
	所以大部分的编译器早已支持。C++11 的工作则是正式把它纳入标准库，
	规定了一个 long long int 类型至少具备 64 位的比特数。

### noexcept 的修饰和操作
1. C++ 相比于 C 的一大优势就在于 C++ 本身就定义了一套完整的异常处理机制。
	然而在 C++11 之前，几乎没有人去使用在函数名后书写异常声明表达式，
	从 C++11 开始，这套机制被弃用，所以我们不去讨论也不去介绍以前这套机制是如何工作如何使用，你更不应该主动去了解它。

2. C++11 将异常的声明简化为以下两种情况：

    - 函数可能抛出任何异常
    - 函数不能抛出任何异常

	1. 并使用 noexcept 对这两种行为进行限制，例如：

			void may_throw();           // 可能抛出异常
			void no_throw() noexcept;   // 不可能抛出异常

	2. 使用 noexcept 修饰过的函数如果抛出异常，**编译器会使用 std::terminate() 来立即终止程序运行**。

	3. noexcept 还能用作操作符，用于操作一个表达式，当表达式无异常时，返回 true，否则返回 false。

			#include <iostream>

			void may_throw() {
				throw true;
			}
			auto non_block_throw = []{
				may_throw();
			};
			void no_throw() noexcept {
				return;
			}

			auto block_throw = []() noexcept {
				no_throw();
			};

			int main()
			{
				std::cout << std::boolalpha
				<< "may_throw() noexcept? " << noexcept(may_throw()) << std::endl
				<< "no_throw() noexcept? " << noexcept(no_throw()) << std::endl
				<< "lmay_throw() noexcept? " << noexcept(non_block_throw()) << std::endl
				<< "lno_throw() noexcept? " << noexcept(block_throw()) << std::endl;

				return 0;
			}

3. noexcept 修饰完一个函数之后能够起到封锁异常扩散的功效，如果内部产生异常，外部也不会触发。例如：

		try {
			may_throw();
		} catch (...) {
			std::cout << "捕获异常, 来自 my_throw()" << std::endl;
		}

		try {
			non_block_throw();
		} catch (...) {
			std::cout << "捕获异常, 来自 non_block_throw()" << std::endl;
		}

		try {
			block_throw();
		} catch (...) {
			std::cout << "捕获异常, 来自 block_throw()" << std::endl;
		}

	最终输出为：

		捕获异常, 来自 my_throw()
		捕获异常, 来自 non_block_throw()

### 总结
1. noexcept 是最为重要的特性，它的一个功能在于能够阻止异常的扩散传播，
让编译器能最大限度的优化我们的代码。
