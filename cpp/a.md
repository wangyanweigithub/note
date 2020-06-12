1. 有些构造函数使用：new 初始化对象成员，这是因为new返回的是一个指针，用在成员是一个指针的情况下，用来复制一个新的new 对象。
	
		class A {
		public:
			int *pointer;
			A(): pointer(new int(1)){}
		}

2. clone 第二个参数只传递一个指针，就可以为子进程创建栈空间。是因为:
	1. 在2.6以前内核中，各个进程的task_struct存放在它们内核栈的尾端，
		这样是为了让那些像x86那样的寄存器较少的硬件体系结构只要通过栈指针就能计算出它的位置，从而避免使用额外的寄存器专门记录。
	2. 由于现在使用slab分配器动态生成task_struct,
		1. 所以只需要在栈底（对于向下增长的栈来说）
		2. 或栈顶（对于像上增长的栈来说）创建一个新的struct_thread_info.这个结构体有task_struct的指针。

	3. 内存中不同的内存页有不同的访问权限和拥有这个页的进程ID。
		
			struct page {
				unsigned long flags; //是否为脏页，是否锁定在内存中...
				atomic_t  _count;
				atomic_t _mapcount;
				unsigned long private; 
				struct addree_space *mapping;
				pgoff_t index;
				struct list_head lru;
				void *virtual;
			}

	4. linux 系统栈是向下增长（高地址向低地址增长），堆是向上增长。但数组内的元素是从低地址像高地址增长的。
	5. clone 第二个参数给定了栈底就可以申请子进程内存栈空间了，它会一直向下增长，直到溢出报核心转储错误。