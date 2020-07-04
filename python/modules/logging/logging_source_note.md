## class 继承关系
1. Logger ---> Filter

2. StreamerHandler ---> Handler

## 全局对象
1. root, Logger

```
root = RootLogger(WARNING)
Logger.root = root
Logger.manager = Manager(Logger.root)
```

2. root 是一个RootLogger对象,是一个单例对象

3. Logger 添加了两个类对象.

## 全局函数
1.addLevelName

2. basicConfig: 配置logging源文件中的root单例Logger

```
def basicConfig(**kwargs):
    """
    Do basic configuration for the logging system.

    This function does nothing if the root logger already has handlers
    configured. It is a convenience method intended for use by simple scripts
    to do one-shot configuration of the logging package.

    The default behaviour is to create a StreamHandler which writes to
    sys.stderr, set a formatter using the BASIC_FORMAT format string, and
    add the handler to the root logger.

    A number of optional keyword arguments may be specified, which can alter
    the default behaviour.

    filename  Specifies that a FileHandler be created, using the specified
              filename, rather than a StreamHandler.
    filemode  Specifies the mode to open the file, if filename is specified
              (if filemode is unspecified, it defaults to 'a').
    format    Use the specified format string for the handler.
    datefmt   Use the specified date/time format.
    style     If a format string is specified, use this to specify the
              type of format string (possible values '%', '{', '$', for
              %-formatting, :meth:`str.format` and :class:`string.Template`
              - defaults to '%').
    level     Set the root logger level to the specified level.
    stream    Use the specified stream to initialize the StreamHandler. Note
              that this argument is incompatible with 'filename' - if both
              are present, 'stream' is ignored.
    handlers  If specified, this should be an iterable of already created
              handlers, which will be added to the root handler. Any handler
              in the list which does not have a formatter assigned will be
              assigned the formatter created in this function.

    Note that you could specify a stream created using open(filename, mode)
    rather than passing the filename and mode in. However, it should be
    remembered that StreamHandler does not close its stream (since it may be
    using sys.stdout or sys.stderr), whereas FileHandler closes its stream
    when the handler is closed.

    .. versionchanged:: 3.2
       Added the ``style`` parameter.

    .. versionchanged:: 3.3
       Added the ``handlers`` parameter. A ``ValueError`` is now thrown for
       incompatible arguments (e.g. ``handlers`` specified together with
       ``filename``/``filemode``, or ``filename``/``filemode`` specified
       together with ``stream``, or ``handlers`` specified together with
       ``stream``.
    """
    # Add thread safety in case someone mistakenly calls
    # basicConfig() from multiple threads
    _acquireLock()
    try:
        if len(root.handlers) == 0:
            handlers = kwargs.pop("handlers", None)
            if handlers is None:
                if "stream" in kwargs and "filename" in kwargs:
                    raise ValueError("'stream' and 'filename' should not be "
                                     "specified together")
            else:
                if "stream" in kwargs or "filename" in kwargs:
                    raise ValueError("'stream' or 'filename' should not be "
                                     "specified together with 'handlers'")
            if handlers is None:
                filename = kwargs.pop("filename", None)
                mode = kwargs.pop("filemode", 'a')
                if filename:
                    h = FileHandler(filename, mode)
                else:
                    stream = kwargs.pop("stream", None)
                    h = StreamHandler(stream)
                handlers = [h]
            dfs = kwargs.pop("datefmt", None)
            style = kwargs.pop("style", '%')
            if style not in _STYLES:
                raise ValueError('Style must be one of: %s' % ','.join(
                                 _STYLES.keys()))
            fs = kwargs.pop("format", _STYLES[style][1])
            fmt = Formatter(fs, dfs, style)
            for h in handlers:
                if h.formatter is None:
                    h.setFormatter(fmt)
                root.addHandler(h)
            level = kwargs.pop("level", None)
            if level is not None:
                root.setLevel(level)
            if kwargs:
                keys = ', '.join(kwargs.keys())
                raise ValueError('Unrecognised argument(s): %s' % keys)
    finally:
        _releaseLock()
```

-captureWarnings
-critical
-currentframe
-debug
-disable
-error
-exception
-getLevelName
-getLogRecordFactory
-getLogger
-getLoggerClass
-info
-log
-makeLogRecord
-setLogRecordFactory
-setLoggerClass
-shutdown
-warn
-warning

## Logger
1. Logger.debug/info/ -> self._log -> self.makeRecord(), self(Logger).handle(record)
-> self.callHandler -> for handler in self.handlers ,handler.handle(record)
-> Handler.emit() -> Handler.filter(record), self.emit(record)
		
2. emit是各个Handler类各自实现的自己的emit,关于怎样处理消息的.如StreamHandle

	1. StreamHandler的emit
	```
	def emit(self, record):
			"""
			Emit a record.

			If a formatter is specified, it is used to format the record.
			The record is then written to the stream with a trailing newline.  If
			exception information is present, it is formatted using
			traceback.print_exception and appended to the stream.  If the stream
			has an 'encoding' attribute, it is used to determine how to do the
			output to the stream.
			"""
			try:
				msg = self.format(record)
				stream = self.stream
				stream.write(msg)
				stream.write(self.terminator)
				self.flush()
			except Exception:
				self.handleError(record)
	```
