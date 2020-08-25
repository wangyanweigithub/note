
<!-- vim-markdown-toc GFM -->

* [`Waiter` 22](#waiter-22)
	* [`_FirstCompletedWaiter` 38](#_firstcompletedwaiter-38)
	* [`_AsCompletedWaiter`  52](#_ascompletedwaiter--52)
	* [`_AllCompletedWaiter` 67](#_allcompletedwaiter-67)
* [waiter辅助函数](#waiter辅助函数)
	* [`AcquireFutures` 95](#acquirefutures-95)
	* [waiter 安装器 115](#waiter-安装器-115)
	* [`_yield_finished_futures` 118](#_yield_finished_futures-118)
* [对外暴露的api](#对外暴露的api)
	* [`as_completed` 123](#as_completed-123)
* [Future 136](#future-136)
* [Executor 172](#executor-172)
* [process 194](#process-194)
	* [`_CallItem` 195](#_callitem-195)
	* [`_ExceptionWithTraceback` 和 `_RemoteTrackeback` 204](#_exceptionwithtraceback-和-_remotetrackeback-204)
	* [`_ResultItem` 207](#_resultitem-207)
	* [`_WorkItem` 158 214](#_workitem-158-214)
	* [ProcessPoolExecutor 221](#processpoolexecutor-221)

<!-- vim-markdown-toc -->
### `Waiter` 22
> Provides the event that wait() and as_completed() block on.

1. attribute: 
	- event: threading.Event()
	- `finished_futures` = []

2. 用途：将指定的future 添加到finished_future,完成的、异常的、取消的都添加到这个列表里。
	- `self.finished_futures.append(future)`

3. 有三种waiter类型,对应四种情况,AllCompletedWaiter有区分是否已有错误就退出两种情况.
	1. `_AsCompletedWaiter`
	2. `_FirstCompletedWaiter`
	3. `_AllCompletedWaiter`
		- stop_on_exection: 如果设置这个选项,如果有错误,waiter体停止.否则继续等待所有future的结果.

#### `_FirstCompletedWaiter` 38
> Used by wait(return_when=FIRST_COMPLETED).

1. `_Waiter`的子类，没有添加新的方法，但是`add_result`、`add_exception`和`add_cancelled`都多加了一句话：

	``` def add_result(self, future):
			super().add_result(future)
			self.event.set()
	```

2. 它在添加future到finished_future后，还设置了event。是的其他线程wait event可以继续执行。

3. event 的wait阻塞，直到event被设置为true或者wait设置了超时。

#### `_AsCompletedWaiter`  52
> Used by as_completed().

1. attributor:
	- lock

2. 和`_FirstCompletedWaiter`一样，唯一不一样的是先加锁，然后执行`_FirstCompletedWaiter`的操作

3. 代码：
```
def add_result(self, future):
	with self.lock:
		...
```

#### `_AllCompletedWaiter` 67
> Used by wait(return_when=FIRST_EXCEPTION and ALL_COMPLETED).

1. 先加锁,如果设置了stop_on_exection 是true, 如果出错,这只waiter的event,
否则将所有future执行add_result才会设置event.

2. 代码:
```
class _AllCompletedWaiter(_Waiter):
...
	def add_exception(self, future):
		super().add_exception(future)
		if self.stop_on_exception:
			self.event.set()
		else:
			self._decrement_pending_calls()

	def add_result(self, future):
		super().add_result(future)
		self._decrement_pending_calls()

	def _decrement_pending_calls(self):
		with self.lock:
			self.num_pending_calls -= 1
			if not self.num_pending_calls:
				self.event.set()
```

### waiter辅助函数
#### `AcquireFutures` 95
> """A context manager that does an ordered acquire of Future conditions."""

1. 代码:
```
class _AcquireFutures(object):
    """A context manager that does an ordered acquire of Future conditions."""

    def __init__(self, futures):
        self.futures = sorted(futures, key=id)

	def __enter__(self):
		for future in self.futures:
			futures._condition.acquire()

	def __exit__(self):
		for future in self.futures:
			future._condition.release()
```

#### waiter 安装器 115
> 根据传递的return_when参数,选择对应的waiter.给传递进函数的future数组,依次设置waiter: future._waiters.append(waiter)

#### `_yield_finished_futures` 118
1. 删除完成future和future的waiter,防止遗留future强引用.
2. yield fs.pop();
3. 传递给这个函数的fs应该是已经完成的future.

### 对外暴露的api
> 上面的waiter,基本都是用于wait和as_completed两个方法的.

#### `as_completed` 123
1. 根据传递的return_when,获取waiter,finished futures

2. 将已经完成的future,执行yield from _yield_from_finished_futures(finished, waiter, ref_collect=(fs,));

3. 注意,future的使用是引用的.

4. 阻塞在waiter.event.wait(wait_timeout)

5. 如果waiter的wait返回,对waiter.finished重复2: 
执行yield from _yield_finished_futures(finished, waiter, ref_collect=(fs, pending));


### Future 136
> Represents the result of an asynchronous computation.

1. 概述：
	1. Future是一个多线程共享的内存，会产生竞争，所以每次操作共享数据时需要加上锁，这里用的是condition。
	2. 什么时候调用`condition.notify_all`
		- `set_result`
		- `set_exception`
		- `cancel`
		- `set_running_or_notify_cancel`:这个并没有调用notify
			注意，这里也需要notify_all，但是executor调用这个函数之前，应该先调用self.cancel(),
			而cancel函数已经触发了notify_all，所以这里不需要notify_all了。
		
2. attributor
	- `_condition` = threading.Condition()
	- `_state` = PENDING
	- `_result` = None
	- `_exception` = None
	- `_waiters` = []
	- `_done_callbacks` = []

3. method
	1. `_invoke_callbacks`
	2. cancel
	3. cancelled
	4. running
	5. done
	6. `add_done_callback`
	7. result
	8. exception
	9. `set_running_or_notify_cancel`
	10. `set_result`
	11. `set_exception`
	12. `_invoke_callbacks`
	13. `__get_result`

### Executor 172
> This is an abstract base class for concrete asynchronous executors.

1. 没有属性
2. method
	1. `__enter__`: return self

	2. `__exit__`: self.shutdown(wait=True); return False

	3. submit(self, fn, `*args`, `**kwargs`):
        1. Submits a callable to be executed with the given arguments.
        2. Schedules the callable to be executed as fn(*args, **kwargs) and returns a Future instance representing the execution of the callable.

		3. Returns:
            A Future representing the given call.
	4. map(self, fn,`*iterators`, timeout=None, chunksize=1)
		1. return an iterator equivalent to map (fn, iter)

	5. shutdown:
		1. 可以多次调用
		2. 调用后不可以调用其他函数
		3. 如果wait 是True，shutdown will not return until all running futures have finished executing and the resources used by the executor have benn reclaimed.
### process 194
#### `_CallItem` 195
1. attributor
	- `work_id`
	- fn
	- args
	- kwargs

只有这几个属性，没有方法。

#### `_ExceptionWithTraceback` 和 `_RemoteTrackeback` 204
> 简单定义了error时的情况，用到了`__reduce__`

#### `_ResultItem` 207
1. attributor:
	- `work_id`
	- fn
	- args
	- kwargs

####	`_WorkItem` 158 214
1. attributor:
	- future
	- fn
	- args
	- kwargs

#### ProcessPoolExecutor 221
1. attributor:
	- `_max_workers`
	- `_call_queue: multiprocessing.Queue`

		- Make the call queue slightly larger than the number of processes to                                                    
		- prevent the worker processes from idling. But don't make it too big                                                    
		- because futures in the call queue cannot be cancelled. 
	- `_call_queue._ignore_epipe=True`:
		- # Killed worker processes can produce spurious "broken pipe" tracebacks in the queue's own worker thread. 
			But we detect killed processes anyway, so silence the tracebacks.
		- <font color=red>所以设置Queue忽略broken pipe的信号</font>

	- `_result_queue: SimpleQueue`
	- `_work_ids: queue.Queue`
	- `_queue_management_thread`

	- `_processes={}`
	- `_shutdown_thread: False`
	- `_shutdown_lock: threading.Lock()`
	- `_broken`
	- `_queue_count`
	- `_pending_work_items: {}`

