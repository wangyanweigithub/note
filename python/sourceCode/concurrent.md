
<!-- vim-markdown-toc GFM -->

	* [`_AcquireFutures`]
	* [`_Waiter` 48]
	* [`_FirstCompletedWaiter` 76]
	* [`_AsCompletedWaiter`  90]
	* [`_AllCompletedWaiter` 105]
* [Executor 108]
* [Future 131]
* [process 167]
	* [`_CallItem` 168]
	* [`_ExceptionWithTraceback` 和 `_RemoteTrackeback` 177]
	* [`_ResultItem` 180]
	* [`_WorkItem` 158 187]
	* [ProcessPoolExecutor 194]

<!-- vim-markdown-toc -->

### `_AcquireFutures`

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

### `_Waiter` 48
> Provides the event that wait() and as_completed() block on.

1. attribute: 
	- event: threading.Event()
	- `finished_futures` = []

2. 用途：将指定的future 添加到finished_future,完成的、异常的、取消的都添加到这个列表里。
	- `self.finished_futures.append(future)`

3. code:
```
class _Waiter(object):
	"""Provides the event that wait() and as_completed() block on."""
	def __init__(self):
		self.event = threading.Event()
		self.finished_futures = []

	def add_result(self, future):
		self.finished_futures.append(future)

	def add_exception(self, future):
		self.finished_futures.append(future)

	def add_cancelled(self, future):
		self.finished_futures.append(future)
```

### `_FirstCompletedWaiter` 76
> Used by wait(return_when=FIRST_COMPLETED).

1. `_Waiter`的子类，没有添加新的方法，但是`add_result`、`add_exception`和`add_cancelled`都多加了一句话：

	``` def add_result(self, future):
			super().add_result(future)
			self.event.set()
	```

2. 它在添加future到finished_future后，还设置了event。是的其他线程wait event可以继续执行。

3. event 的wait阻塞，直到event被设置为true或者wait设置了超时。

### `_AsCompletedWaiter`  90
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

### `_AllCompletedWaiter` 105
> Used by wait(return_when=FIRST_EXCEPTION and ALL_COMPLETED).

## Executor 108
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

## Future 131
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

## process 167
### `_CallItem` 168
1. attributor
	- `work_id`
	- fn
	- args
	- kwargs

只有这几个属性，没有方法。

### `_ExceptionWithTraceback` 和 `_RemoteTrackeback` 177
> 简单定义了error时的情况，用到了`__reduce__`

### `_ResultItem` 180
1. attributor:
	- `work_id`
	- fn
	- args
	- kwargs

###	`_WorkItem` 158 187
1. attributor:
	- future
	- fn
	- args
	- kwargs

### ProcessPoolExecutor 194
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
