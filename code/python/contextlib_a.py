import time
from functools import wraps


def contextmanager(func):
    @wraps(func)
    def helper(*args, **kwargs):
        return _GeneratorContextManager(func,args, kwargs)
    return helper


class _GeneratorContextManager(object):
    def __init__(self, func, args, kwargs):
        self.gen = func(*args, **kwargs)
        self.func, self.args, self.kwds = func, args, kwargs
        doc = getattr(func, "__doc__", None)
        if doc is None:
                doc = type(self).__doc__
        self.__doc__= doc

    def _recreate_cm(self):
        print("_recreate_cm")
        return self.__class__(self.func, self.args, self.kwds)

    def __enter__(self):
        del self.args, self.kwds, self.func
        try:
            return next(self.gen)
        except StopInterupt:
            raise RuntimeError("gene")

    def __exit__(self, type, value, traceback):
        if type is None:
            try:
                next(self.gen)
            except StopIteration:
                return False
            else:
                raise RuntimeError("generator didn't stop")
        else:
            if value is None:
                value = type()
            try:
                self.gen.throw(type, value, traceback)
            except StopIteration:
                pass

    def __call__(self, func):
        print("!!!!!!!!!!!!!!!!!!")
        @wraps(func)
        def inner(*args, **kwargs):
            with self._recreate_cm():
                return func(*args, **kwargs)
        return inner
         

def hello(a, b):
    print(a, b)


@contextmanager
def timeit(title):
    print("1...")
    start = time.time()
    yield hello
    end = time.time()
    print(end - start)


def test():
    with timeit(1) as f:
        print(3)
        f()

timeit(1)(hello)("wang", "yan")

