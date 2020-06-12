## threading.event
1. Python threading模块提供Event对象用于线程间通信。它提供了一组、拆除、等待用于线程间通信的其他方法。
2. event它是沟通中最简单的一个过程之中，一个线程产生一个信号号。
3. Python 通过threading.Event()产生一个event对象。
	- event对象维护一个内部标志（标志初始值为False），通过set()将其置为True。
	- wait(timeout)则用于堵塞线程直至Flag被set（或者超时，可选的），
	- isSet()用于查询标志位是否为True，
	- Clear()则用于清除标志位（使之为False）。