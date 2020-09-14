 

用Python编写的Python解释器

艾莉森·卡普特（Allison Kaptur）

Allison是Dropbox的工程师，她在其中帮助维护了全球最大的Python客户端网络之一。在Dropbox之前，她
是Recurse中心的主持人，该中心是纽约程序员的务虚会。她在北美PyCon大会上谈到了Python的内部原理，
并且喜欢奇怪的错误。她在akaptur.com上博客。

（本章也提供简体中文）。

介绍

Byterun是用Python实现的Python解释器。通过在Byterun上的工作，我感到惊讶和高兴，发现Python解释器
的基本结构很容易适应500行大小的限制。本章将介绍解释器的结构，并为您提供足够的上下文以进一步探
索它。目的不是要解释关于解释器的所有知识，就像许多有趣的编程和计算机科学领域一样，您可以花很多
年时间来深入理解该主题。

Byterun由Ned Batchelder和我本人根据Paul Swartz的著作编写。它的结构类似于Python的主要实现
CPython，因此了解Byterun将帮助您大致理解解释器，尤其是CPython解释器。（如果您不知道使用的是哪
个Python，则可能是CPython。）尽管Byterun的长度很短，但它能够运行大多数简单的Python程序^1。

Python解释器

在开始之前，让我们缩小“ Python解释器”的含义。讨论Python时，可以以多种不同的方式使用“解释器”一
词。有时解释器会引用Python REPL，这是您python在命令行中键入的交互式提示。有时人们或多或少地将“
Python解释器”与“ Python”互换使用，以谈论从头到尾执行Python代码的问题。在本章中，“解释器”的含义
更狭窄：这是执行Python程序的最后一步。

在解释器接管之前，Python将执行其他三个步骤：词法分析，解析和编译。这些步骤共同将程序员的源代码
从文本行转换为包含解释器可以理解的指令的结构化代码对象。解释器的工作是获取这些代码对象并按照说
明进行操作。

您可能会惊讶地发现编译是执行Python代码的第一步。Python通常被称为Ruby或Perl之类的“解释”语言，而
不是诸如C或Rust的“已编译”语言。但是，此术语并不像看起来那样精确。包括Python在内的大多数解释型
语言的确包含编译步骤。Python之所以称为“已解释”，是因为与在已编译语言中相比，编译步骤所做的工作
相对较少（解释程序所做的工作相对较多）。正如我们将在本章后面看到的那样，Python编译器比C编译器
具有更多有关程序行为的信息。

Python Python解释器

Byterun是用Python编写的Python解释器。这可能会让您感到奇怪，但与用C编写C编译器相比，这并不奇怪
。（实际上，广泛使用的C编译器gcc是用C编写的。）几乎可以使用任何语言编写Python解释器。

用Python编写Python解释器既有优点也有缺点。最大的缺点是速度：通过Byterun执行代码比在CPython中执
行解释要慢得多，在CPython中，解释器是用C编写并经过仔细优化的。但是，Byterun最初是作为一种学习
练习而设计的，因此速度对我们而言并不重要。要使用Python最大的优势是，我们可以更容易地实现公正解
释器，而不是其余的Python运行时，尤其是对象系统。例如，当需要创建类时，Byterun可以使用“真实”
Python。另一个优点是Byterun易于理解，部分原因是它是用许多人都易于阅读的高级语言（Python！）编
写的。（我们还排除了Byterun中的解释器优化-再次偏向于使清晰度和简单性优于速度。）

建立一个口译员

在开始查看Byterun的代码之前，我们需要有关解释器结构的一些高级上下文。Python解释器如何工作？

Python解释器是一个虚拟机，这意味着它是模拟物理计算机的软件。这个特定的虚拟机是一个堆栈计算机：
它操纵多个堆栈来执行其操作（与寄存器计算机相反，后者对特定的内存位置进行写入和读取）。

Python解释器是字节码解释器：其输入是称为字节码的指令集。当您编写Python时，词法分析器，解析器和
编译器会生成代码对象，以供解释器进行操作。每个代码对象都包含一组要执行的指令（即字节码）以及解
释器将需要的其他信息。字节码是Python代码的中间表示形式：它以解释器可以理解的方式表示您编写的源
代码。它类似于汇编语言充当C代码和硬件之间的中间表示的方式。

微小的口译员

为了使这个具体，让我们从一个最小的解释器开始。该解释器只能加数字，并且仅能理解三个指令。它可以
执行的所有代码由这三个指令以不同的组合组成。这三个指令是：

  • LOAD_VALUE
  • ADD_TWO_VALUES
  • PRINT_ANSWER

由于我们在本章中不关心词法分析器，解析器和编译器，因此指令集的生成方式无关紧要。您可以想象编写
7 + 5并让编译器发出这三个指令的组合。或者，如果您使用正确的编译器，则可以编写Lisp语法，将其转
换为相同的指令组合。口译员不在乎。重要的是我们的口译员得到了格式正确的指示。

假设

7 + 5

产生以下指令集：

what_to_execute = {
    "instructions": [("LOAD_VALUE", 0),  # the first number
                     ("LOAD_VALUE", 1),  # the second number
                     ("ADD_TWO_VALUES", None),
                     ("PRINT_ANSWER", None)],
    "numbers": [7, 5] }

Python解释器是一台堆栈机，因此它必须操纵堆栈以添加两个数字（图12.1）。解释器将通过执行第一条指
令LOAD_VALUE并将第一个数字压入堆栈开始。接下来，它将第二个数字压入堆栈。对于第三条指令，
ADD_TWO_VALUES它将弹出两个数字，将它们加在一起，然后将结果压入堆栈。最后，它将答案从堆栈弹出并
打印。

图12.1-堆栈机

图12.1-堆栈机

该LOAD_VALUE指令告诉解释器将一个数字压入堆栈，但是仅该指令并没有指定哪个数字。每条指令都需要一
条额外的信息，告诉解释器在哪里找到要加载的编号。因此，我们的指令集包括两部分：指令本身，以及指
令所需的常量列表。（在Python中，我们所谓的“指令”是字节码，下面的“执行什么”对象是代码对象。）

为什么不将数字直接放在说明中？想象一下，如果我们将字符串而不是数字加在一起。我们不想在指令中插
入字符串，因为它们可能会很大。这种设计还意味着我们可以只有一个每个对象，我们需要的副本，例如要
增加7 + 7，"numbers"可能只是[7]。

您可能想知道为什么ADD_TWO_VALUES根本不需要其他说明。确实，对于将两个数字相加的简单情况，该示例
有些虚构。但是，此指令是更复杂程序的构建块。例如，仅使用到目前为止我们已定义的指令，只要正确设
置了这些指令，我们就可以将三个值（或任意数量的值）加在一起。堆栈提供了一种清晰的方式来跟踪解释
器的状态，并且随着我们的前进，它将支持更多的复杂性。

现在让我们开始编写解释器本身。解释器对象具有一个堆栈，我们将用一个列表来表示。该对象还具有描述
如何执行每条指令的方法。例如，对于LOAD_VALUE，解释器会将值压入堆栈。

class Interpreter:
    def __init__(self):
        self.stack = []

    def LOAD_VALUE(self, number):
        self.stack.append(number)

    def PRINT_ANSWER(self):
        answer = self.stack.pop()
        print(answer)

    def ADD_TWO_VALUES(self):
        first_num = self.stack.pop()
        second_num = self.stack.pop()
        total = first_num + second_num
        self.stack.append(total)

这三个功能实现了我们的解释器理解的三个指令。解释器还需要一件：将所有东西捆绑在一起并实际执行的
方法。此方法run_code将what_to_execute上面定义的字典作为参数。它循环遍历每条指令，处理该指令的
参数（如果有），然后在解释器对象上调用相应的方法。

    def run_code(self, what_to_execute):
        instructions = what_to_execute["instructions"]
        numbers = what_to_execute["numbers"]
        for each_step in instructions:
            instruction, argument = each_step
            if instruction == "LOAD_VALUE":
                number = numbers[argument]
                self.LOAD_VALUE(number)
            elif instruction == "ADD_TWO_VALUES":
                self.ADD_TWO_VALUES()
            elif instruction == "PRINT_ANSWER":
                self.PRINT_ANSWER()

为了对其进行测试，我们可以创建对象的实例，然后run_code使用上面定义的添加7 + 5的指令集调用该方
法。

    interpreter = Interpreter()
    interpreter.run_code(what_to_execute)

果然，它会打印出答案：12。

尽管此解释器非常有限，但此过程几乎完全是真正的Python解释器如何加数字的过程。即使在这个小例子中
，也要注意两点。

首先，一些指令需要参数。在真正的Python字节码中，大约一半的指令带有参数。参数与指令打包在一起，
非常类似于我们的示例。请注意，指令的自变量与调用的方法的自变量不同。

其次，请注意的指令ADD_TWO_VALUES不需要任何参数。而是将要加在一起的值从解释器的堆栈中弹出。这是
基于堆栈的解释器的定义功能。

请记住，给定有效的指令集，而无需对我们的解释器进行任何更改，我们可以一次添加两个以上的数字。请
考虑以下说明。您期望发生什么？如果您有一个友好的编译器，可以编写什么代码来生成此指令集？

    what_to_execute = {
        "instructions": [("LOAD_VALUE", 0),
                         ("LOAD_VALUE", 1),
                         ("ADD_TWO_VALUES", None),
                         ("LOAD_VALUE", 2),
                         ("ADD_TWO_VALUES", None),
                         ("PRINT_ANSWER", None)],
        "numbers": [7, 5, 8] }

在这一点上，我们可以开始了解这种结构的可扩展性：我们可以在解释器对象上添加描述更多操作的方法（
只要我们有一个编译器来处理格式正确的指令集即可）。

变数

接下来，让我们向解释器添加变量。变量需要一条指令来存储变量的值STORE_NAME；检索它的指令
LOAD_NAME；以及从变量名到值的映射。现在，我们将忽略名称空间和作用域，因此我们可以将变量映射存
储在解释器对象本身上。最后，what_to_execute除了常量列表之外，我们还必须确保具有变量名称列表。

>>> def s():
...     a = 1
...     b = 2
...     print(a + b)
# a friendly compiler transforms `s` into:
    what_to_execute = {
        "instructions": [("LOAD_VALUE", 0),
                         ("STORE_NAME", 0),
                         ("LOAD_VALUE", 1),
                         ("STORE_NAME", 1),
                         ("LOAD_NAME", 0),
                         ("LOAD_NAME", 1),
                         ("ADD_TWO_VALUES", None),
                         ("PRINT_ANSWER", None)],
        "numbers": [1, 2],
        "names":   ["a", "b"] }

我们的新实现如下。为了跟踪哪些名称绑定到哪些值，我们将向environment该__init__方法添加一个字典
。我们还将添加STORE_NAME和LOAD_NAME。这些方法首先查找有问题的变量名，然后使用字典存储或检索其
值。

指令的参数现在可以表示两种不同的含义：它们可以是“数字”列表的索引，也可以是“名称”列表的索引。解
释器通过检查正在执行的指令来知道应该是哪个。我们将把这种逻辑（以及指令到其参数含义的映射）分解
为一个单独的方法。

class Interpreter:
    def __init__(self):
        self.stack = []
        self.environment = {}

    def STORE_NAME(self, name):
        val = self.stack.pop()
        self.environment[name] = val

    def LOAD_NAME(self, name):
        val = self.environment[name]
        self.stack.append(val)

    def parse_argument(self, instruction, argument, what_to_execute):
        """ Understand what the argument to each instruction means."""
        numbers = ["LOAD_VALUE"]
        names = ["LOAD_NAME", "STORE_NAME"]

        if instruction in numbers:
            argument = what_to_execute["numbers"][argument]
        elif instruction in names:
            argument = what_to_execute["names"][argument]

        return argument

    def run_code(self, what_to_execute):
        instructions = what_to_execute["instructions"]
        for each_step in instructions:
            instruction, argument = each_step
            argument = self.parse_argument(instruction, argument, what_to_execute)

            if instruction == "LOAD_VALUE":
                self.LOAD_VALUE(argument)
            elif instruction == "ADD_TWO_VALUES":
                self.ADD_TWO_VALUES()
            elif instruction == "PRINT_ANSWER":
                self.PRINT_ANSWER()
            elif instruction == "STORE_NAME":
                self.STORE_NAME(argument)
            elif instruction == "LOAD_NAME":
                self.LOAD_NAME(argument)

即使只有五条指令，该run_code方法也开始变得乏味。如果我们保持这种结构，那么if每条指令都需要一个
语句分支。在这里，我们可以利用Python的动态方法查找。我们将始终定义一个称为的方法FOO来执行称为
的指令FOO，因此我们可以使用Python的getattr函数即时查找该方法，而无需使用big if语句。run_code然
后，该方法如下所示：

    def execute(self, what_to_execute):
        instructions = what_to_execute["instructions"]
        for each_step in instructions:
            instruction, argument = each_step
            argument = self.parse_argument(instruction, argument, what_to_execute)
            bytecode_method = getattr(self, instruction)
            if argument is None:
                bytecode_method()
            else:
                bytecode_method(argument)

真正的Python字节码

在这一点上，我们将放弃玩具指令集，而改用真正的Python字节码。字节码的结构类似于玩具解释器的详细
指令集，不同之处在于它使用一个字节代替长名称来标识每条指令。为了理解这种结构，我们将遍历一个简
短函数的字节码。考虑下面的示例：

>>> def cond():
...     x = 3
...     if x < 5:
...         return 'yes'
...     else:
...         return 'no'
...

Python在运行时公开了其内部组件，我们可以从REPL中直接访问它们。对于功能对象cond，cond.__code__
是与之关联的代码对象，并且cond.__code__.co_code是字节码。在编写Python代码时，几乎没有充分的理
由直接使用这些属性，但是它们的确使我们能够直面各种恶作剧-并且查看内部结构以了解它们。

>>> cond.__code__.co_code  # the bytecode as raw bytes
b'd\x01\x00}\x00\x00|\x00\x00d\x02\x00k\x00\x00r\x16\x00d\x03\x00Sd\x04\x00Sd\x00
   \x00S'
>>> list(cond.__code__.co_code)  # the bytecode as numbers
[100, 1, 0, 125, 0, 0, 124, 0, 0, 100, 2, 0, 107, 0, 0, 114, 22, 0, 100, 3, 0, 83,
 100, 4, 0, 83, 100, 0, 0, 83]

当我们仅打印字节码时，它看起来就难以理解-我们只能说它是一系列字节。幸运的是，我们可以使用一个
强大的工具来理解它：disPython标准库中的模块。

dis是字节码反汇编程序。反汇编程序采用为机器编写的低级代码（例如汇编代码或字节码），并以人类可
读的方式进行打印。当我们运行时dis.dis，它输出已通过的字节码的说明。

>>> dis.dis(cond)
  2           0 LOAD_CONST               1 (3)
              3 STORE_FAST               0 (x)

  3           6 LOAD_FAST                0 (x)
              9 LOAD_CONST               2 (5)
             12 COMPARE_OP               0 (<)
             15 POP_JUMP_IF_FALSE       22

  4          18 LOAD_CONST               3 ('yes')
             21 RETURN_VALUE

  6     >>   22 LOAD_CONST               4 ('no')
             25 RETURN_VALUE
             26 LOAD_CONST               0 (None)
             29 RETURN_VALUE

这是什么意思呢？让我们以第一个指令LOAD_CONST为例。第一列（2）中的数字显示了Python源代码中的行
号。第二列是字节码的索引，告诉我们该LOAD_CONST指令出现在零位置。第三列是指令本身，映射到其可读
名称。第四列（如果存在）是该指令的参数。出现在第五列时，它暗示了该参数的含义。

考虑此字节码的前几个字节：[100，1，0，125，0，0]。这六个字节代表两个带有参数的指令。我们可以使
用dis.opname从字节到可理解的字符串的映射来找出指令100和125映射到：

>>> dis.opname[100]
'LOAD_CONST'
>>> dis.opname[125]
'STORE_FAST'

第二和第三个字节（1，0）是的参数LOAD_CONST，而第五和第六个字节（0，0）是的参数STORE_FAST。就像
在我们的玩具示例中一样，LOAD_CONST需要知道在哪里找到要加载其常量的常量，并且STORE_FAST需要找到
要存储的名称。（Python LOAD_CONST与我们的玩具解释器相同LOAD_VALUE，并且与。LOAD_FAST也相同
LOAD_NAME。）因此，这六个字节代表代码的第一行x = 3。（为什么每个参数都使用两个字节？如果Python
仅使用一个字节来定位常量和名称，而不是两个字节，则与单个代码对象关联的名称/常量只能为256个。使
用两个字节时，最多可以有256个平方，或65,536。）

条件和循环

到目前为止，解释器仅通过单步执行指令就可以执行代码。这是个问题; 通常，我们想多次执行某些指令，
或者在某些条件下跳过它们。为了允许我们在代码中编写循环和if语句，解释器必须能够在指令集中跳来跳
去。从某种意义上说，Python使用GOTO字节码中的语句处理循环和条件！cond再次查看该函数的反汇编：

>>> dis.dis(cond)
  2           0 LOAD_CONST               1 (3)
              3 STORE_FAST               0 (x)

  3           6 LOAD_FAST                0 (x)
              9 LOAD_CONST               2 (5)
             12 COMPARE_OP               0 (<)
             15 POP_JUMP_IF_FALSE       22

  4          18 LOAD_CONST               3 ('yes')
             21 RETURN_VALUE

  6     >>   22 LOAD_CONST               4 ('no')
             25 RETURN_VALUE
             26 LOAD_CONST               0 (None)
             29 RETURN_VALUE

条件if x < 5的代码的第3行被编译成四个指令：LOAD_FAST，LOAD_CONST，COMPARE_OP，和
POP_JUMP_IF_FALSE。x < 5生成代码以加载x，加载5并比较这两个值。该指令POP_JUMP_IF_FALSE负责实现
if。该指令将从解释器的栈中弹出最高值。如果该值为true，则什么也不会发生。（该值可以是“真值”，它
不必是文字True对象。）如果该值为false，则解释器将跳转到另一条指令。

落入的指令称为跳转目标，它作为POP_JUMP指令的参数提供。在这里，跳转目标是22。索引22的指令在
LOAD_CONST第6行。（用dis标记跳转目标>>。）如果结果x < 5为False，则解释器将直接跳到第6行（
return "no"），而跳过第4行（return "yes"）。因此，解释器使用跳转指令来有选择地跳过部分指令集。

Python循环也依赖跳跃。在下面的字节码中，请注意，该行while x < 5生成的字节码与几乎相同if x < 10
。在这两种情况下，都将计算比较值，然后POP_JUMP_IF_FALSE控制下一步执行哪个指令。在第4行的末尾（
循环主体的末尾），该指令JUMP_ABSOLUTE始终将解释器发送回循环顶部的指令9。当x <5变为false时，则
将POP_JUMP_IF_FALSE解释器跳过循环结束，转到指令34。

>>> def loop():
...      x = 1
...      while x < 5:
...          x = x + 1
...      return x
...
>>> dis.dis(loop)
  2           0 LOAD_CONST               1 (1)
              3 STORE_FAST               0 (x)

  3           6 SETUP_LOOP              26 (to 35)
        >>    9 LOAD_FAST                0 (x)
             12 LOAD_CONST               2 (5)
             15 COMPARE_OP               0 (<)
             18 POP_JUMP_IF_FALSE       34

  4          21 LOAD_FAST                0 (x)
             24 LOAD_CONST               1 (1)
             27 BINARY_ADD
             28 STORE_FAST               0 (x)
             31 JUMP_ABSOLUTE            9
        >>   34 POP_BLOCK

  5     >>   35 LOAD_FAST                0 (x)
             38 RETURN_VALUE

探索字节码

我鼓励您尝试dis.dis在编写的函数上运行。一些要探索的问题：

  • Python解释器的for循环和while循环之间有什么区别？
  • 如何编写不同的函数来生成相同的字节码？
  • elif工作如何？那么列表理解呢？

镜框

到目前为止，我们已经了解到Python虚拟机是堆栈机。它步进并跳转指令，将值压入和弹出堆栈。但是，我
们的心理模型仍然存在一些差距。在上面的示例中，最后一条指令是RETURN_VALUE，它对应return于代码中
的语句。但是指令返回到哪里？

为了回答这个问题，我们必须增加一层复杂性：框架。框架是一段代码的信息和上下文的集合。当您的
Python代码执行时，即时创建和销毁框架。一个函数的每个调用对应一个框架-因此，尽管每个框架都有一
个与之关联的代码对象，但是一个代码对象可以具有许多框架。如果您有一个函数十次递归调用自己，那么
您将拥有11帧-每个递归级别一个，而从您开始的模块一个。通常，Python程序中的每个作用域都有一个框
架。例如，每个模块，每个函数调用和每个类定义都有一个框架。

框架存在于调用堆栈中，与我们迄今为止讨论的堆栈完全不同。（调用堆栈是您最熟悉的堆栈-您已经在异
常的回溯中看到了该堆栈。回溯中以“ File'program.py'，line 10”开头的每一行都对应一帧我们正在检查
的堆栈（解释器在执行字节码时正在处理的堆栈）将称为数据堆栈。还有第三个堆栈，称为块堆栈。块用于
某些类型的控制流，尤其是循环和异常处理。调用堆栈上的每个帧都有其自己的数据堆栈和块堆栈。

让我们用一个例子来具体说明。假设Python解释器当前正在执行下面标记为3的行。解释器在呼叫的中间，
该呼叫foo又是bar。该图显示了帧的调用堆栈，块堆栈和数据堆栈的示意图。（此代码的编写类似于REPL会
话，因此我们首先定义了所需的功能。）在我们感兴趣的时刻，解释器foo()在底部执行，然后到达主体，
foo然后向上进入bar。

>>> def bar(y):
...     z = y + 3     # <--- (3) ... and the interpreter is here.
...     return z
...
>>> def foo():
...     a = 1
...     b = 2
...     return a + bar(b) # <--- (2) ... which is returning a call to bar ...
...
>>> foo()             # <--- (1) We're in the middle of a call to foo ...
3

图12.2-调用堆栈

图12.2-调用堆栈

此时，解释器位于对的函数调用的中间bar。调用堆栈上有三帧：一帧用于模块级，一帧用于功能foo，一帧
用于bar（图12.2）。一旦bar返回，与之关联的帧就会从调用堆栈中弹出并丢弃。

字节码指令RETURN_VALUE告诉解释器在帧之间传递一个值。首先，它将顶部值从调用堆栈顶部框架的数据堆
栈弹出。然后，它将整个帧从调用堆栈中弹出并丢弃。最后，该值将在下一帧向下推入数据堆栈。

当Ned Batchelder和我在Byterun上工作时，很长时间以来，我们在实现过程中都遇到了重大错误。而不是
在每一帧上都有一个数据堆栈，而是在整个虚拟机上只有一个数据堆栈。我们进行了几十个测试，这些测试
由Python代码的小片段组成，我们通过Byterun和真实的Python解释器进行了运行，以确保两个解释器中都
发生了同样的事情。几乎所有这些测试都通过了。我们唯一无法工作的是发电机。最后，更仔细地阅读
CPython代码，我们意识到了错误^2。将数据堆栈移到每个帧上可以解决此问题。

回顾此错误，我惊讶于Python很少依赖具有不同数据堆栈的每个框架。Python解释器中几乎所有的操作都仔
细地清理了数据堆栈，因此框架共享同一堆栈这一事实无关紧要。在上面的示例中，bar完成执行后，它将
数据堆栈留空。即使foo共享相同的堆栈，值也会降低。但是，对于生成器，一个关键功能是能够暂停一帧
，返回到其他帧，然后稍后返回到生成器帧，并使它处于与离开时完全相同的状态。

字节流

现在，我们对Python解释器有了足够的了解，可以开始研究Byterun。

Byterun中有四种对象：

  • 一个VirtualMachine类，用于管理最高级别的结构，尤其是帧的调用堆栈，并包含指令到操作的映射。
    这是Intepreter上面对象的更复杂的版本。
  • 一Frame堂课每个Frame实例都有一个代码对象，并管理其他一些必要的状态位，尤其是全局和局部名称
    空间，对调用帧的引用以及最后执行的字节码指令。
  • 一个Function类，它将代替实际的Python函数。回想一下，调用函数会在解释器中创建一个新框架。我
    们实现功能，以便控制新框架的创建。
  • 一个Block类，仅包装块的三个属性。（块的细节不是Python解释器的核心，因此我们不会在它们上花
    费很多时间，但是这里包含了它们，以便Byterun可以运行真实的Python代码。）

该VirtualMachine班

VirtualMachine每次运行该程序时，只会创建一个实例，因为我们只有一个Python解释器。VirtualMachine
存储调用堆栈，异常状态以及在帧之间传递值时的返回值。执行代码的入口点是method run_code，该方法
将已编译的代码对象作为参数。首先要设置并运行框架。该框架可以创建其他框架。调用堆栈将随着程序执
行而增长和收缩。当第一帧最终返回时，执行完成。

class VirtualMachineError(Exception):
    pass

class VirtualMachine(object):
    def __init__(self):
        self.frames = []   # The call stack of frames.
        self.frame = None  # The current frame.
        self.return_value = None
        self.last_exception = None

    def run_code(self, code, global_names=None, local_names=None):
        """ An entry point to execute code using the virtual machine."""
        frame = self.make_frame(code, global_names=global_names,
                                local_names=local_names)
        self.run_frame(frame)

该Frame班

接下来，我们将编写Frame对象。框架是没有方法的属性的集合。如上所述，属性包括由编译器创建的代码
对象。本地，全局和内置名称空间；对前一帧的引用；数据堆栈；块堆栈；和最后执行的指令。（我们需要
做一些额外的工作才能到达内置名称空间，因为Python在不同模块中对待该名称空间的方式有所不同；此详
细信息对于虚拟机而言并不重要。）

class Frame(object):
    def __init__(self, code_obj, global_names, local_names, prev_frame):
        self.code_obj = code_obj
        self.global_names = global_names
        self.local_names = local_names
        self.prev_frame = prev_frame
        self.stack = []
        if prev_frame:
            self.builtin_names = prev_frame.builtin_names
        else:
            self.builtin_names = local_names['__builtins__']
            if hasattr(self.builtin_names, '__dict__'):
                self.builtin_names = self.builtin_names.__dict__

        self.last_instruction = 0
        self.block_stack = []

接下来，我们将向虚拟机添加帧操作。框架有三种帮助功能：一种用于创建新框架（负责为新框架整理名称
空间），另一种用于将框架推入和弹出框架堆栈。第四个功能run_frame完成框架的主要工作。我们将很快
回到这一点。

class VirtualMachine(object):
    [... snip ...]

    # Frame manipulation
    def make_frame(self, code, callargs={}, global_names=None, local_names=None):
        if global_names is not None and local_names is not None:
            local_names = global_names
        elif self.frames:
            global_names = self.frame.global_names
            local_names = {}
        else:
            global_names = local_names = {
                '__builtins__': __builtins__,
                '__name__': '__main__',
                '__doc__': None,
                '__package__': None,
            }
        local_names.update(callargs)
        frame = Frame(code, global_names, local_names, self.frame)
        return frame

    def push_frame(self, frame):
        self.frames.append(frame)
        self.frame = frame

    def pop_frame(self):
        self.frames.pop()
        if self.frames:
            self.frame = self.frames[-1]
        else:
            self.frame = None

    def run_frame(self):
        pass
        # we'll come back to this shortly

该Function班

该Function对象的实现有些曲折，并且大多数细节对于理解解释器而言并不关键。需要注意的重要一点是，
调用函数（调用__call__方法）会创建一个新Frame对象并开始运行它。

class Function(object):
    """
    Create a realistic function object, defining the things the interpreter expects.
    """
    __slots__ = [
        'func_code', 'func_name', 'func_defaults', 'func_globals',
        'func_locals', 'func_dict', 'func_closure',
        '__name__', '__dict__', '__doc__',
        '_vm', '_func',
    ]

    def __init__(self, name, code, globs, defaults, closure, vm):
        """You don't need to follow this closely to understand the interpreter."""
        self._vm = vm
        self.func_code = code
        self.func_name = self.__name__ = name or code.co_name
        self.func_defaults = tuple(defaults)
        self.func_globals = globs
        self.func_locals = self._vm.frame.f_locals
        self.__dict__ = {}
        self.func_closure = closure
        self.__doc__ = code.co_consts[0] if code.co_consts else None

        # Sometimes, we need a real Python function.  This is for that.
        kw = {
            'argdefs': self.func_defaults,
        }
        if closure:
            kw['closure'] = tuple(make_cell(0) for _ in closure)
        self._func = types.FunctionType(code, globs, **kw)

    def __call__(self, *args, **kwargs):
        """When calling a Function, make a new frame and run it."""
        callargs = inspect.getcallargs(self._func, *args, **kwargs)
        # Use callargs to provide a mapping of arguments: values to pass into the new 
        # frame.
        frame = self._vm.make_frame(
            self.func_code, callargs, self.func_globals, {}
        )
        return self._vm.run_frame(frame)

def make_cell(value):
    """Create a real Python closure and grab a cell."""
    # Thanks to Alex Gaynor for help with this bit of twistiness.
    fn = (lambda x: lambda: x)(value)
    return fn.__closure__[0]

接下来，回到该VirtualMachine对象，我们将添加一些用于数据堆栈操作的辅助方法。操纵堆栈的字节码始
终在当前帧的数据堆栈上运行。这将使我们对POP_TOP，LOAD_FAST和所有其他与堆栈相关的指令的实现更具
可读性。

class VirtualMachine(object):
    [... snip ...]

    # Data stack manipulation
    def top(self):
        return self.frame.stack[-1]

    def pop(self):
        return self.frame.stack.pop()

    def push(self, *vals):
        self.frame.stack.extend(vals)

    def popn(self, n):
        """Pop a number of values from the value stack.
        A list of `n` values is returned, the deepest value first.
        """
        if n:
            ret = self.frame.stack[-n:]
            self.frame.stack[-n:] = []
            return ret
        else:
            return []

在开始运行框架之前，我们需要另外两种方法。

第一个是parse_byte_and_args，采用字节码，检查其是否具有参数，如果有则解析参数。此方法还更新框
架的属性last_instruction，即对最后执行的指令的引用。如果没有参数，则一条指令的长度为一个字节，
如果没有参数，则为三个字节；如果没有参数，则为三个字节。最后两个字节是参数。每个指令的参数含义
取决于它是哪个指令。例如，如上所述，对于POP_JUMP_IF_FALSE，指令的参数是跳转目标。对于
BUILD_LIST，它是列表中元素的数量。对于LOAD_CONST，它是常量列表的索引。

一些指令使用简单数字作为参数。对于其他人，虚拟机必须做一些工作才能发现参数的含义。dis标准库中
的模块公开了一个备忘单，解释了哪些参数具有什么含义，这使我们的代码更加紧凑。例如，名单
dis.hasname告诉我们，参数LOAD_NAME，IMPORT_NAME，LOAD_GLOBAL，其他九个指令具有相同的含义：对于
这些指令，则表示索引到代码对象名称的列表。

class VirtualMachine(object):
    [... snip ...]

    def parse_byte_and_args(self):
        f = self.frame
        opoffset = f.last_instruction
        byteCode = f.code_obj.co_code[opoffset]
        f.last_instruction += 1
        byte_name = dis.opname[byteCode]
        if byteCode >= dis.HAVE_ARGUMENT:
            # index into the bytecode
            arg = f.code_obj.co_code[f.last_instruction:f.last_instruction+2]
            f.last_instruction += 2   # advance the instruction pointer
            arg_val = arg[0] + (arg[1] * 256)
            if byteCode in dis.hasconst:   # Look up a constant
                arg = f.code_obj.co_consts[arg_val]
            elif byteCode in dis.hasname:  # Look up a name
                arg = f.code_obj.co_names[arg_val]
            elif byteCode in dis.haslocal: # Look up a local name
                arg = f.code_obj.co_varnames[arg_val]
            elif byteCode in dis.hasjrel:  # Calculate a relative jump
                arg = f.last_instruction + arg_val
            else:
                arg = arg_val
            argument = [arg]
        else:
            argument = []

        return byte_name, argument

下一个方法是dispatch，它查找给定指令的操作并执行它们。在CPython解释器中，这种分派是通过跨越
1500行的巨型switch语句完成的！幸运的是，由于我们正在编写Python，因此我们可以变得更紧凑。我们将
为每个字节名称定义一个方法，然后使用getattr它来查找它。就像上面的玩具解释器一样，如果我们的指
令被命名FOO_BAR，相应的方法将被命名byte_FOO_BAR。目前，我们将这些方法的内容保留为黑框。每个字
节码方法将返回一个None或一个称为why，这是解释程序在某些情况下需要的一种额外状态。各个指令方法
的这些返回值仅用作解释器状态的内部指示符-请勿将其与执行帧的返回值混淆。

class VirtualMachine(object):
    [... snip ...]

    def dispatch(self, byte_name, argument):
        """ Dispatch by bytename to the corresponding methods.
        Exceptions are caught and set on the virtual machine."""

        # When later unwinding the block stack,
        # we need to keep track of why we are doing it.
        why = None
        try:
            bytecode_fn = getattr(self, 'byte_%s' % byte_name, None)
            if bytecode_fn is None:
                if byte_name.startswith('UNARY_'):
                    self.unaryOperator(byte_name[6:])
                elif byte_name.startswith('BINARY_'):
                    self.binaryOperator(byte_name[7:])
                else:
                    raise VirtualMachineError(
                        "unsupported bytecode type: %s" % byte_name
                    )
            else:
                why = bytecode_fn(*argument)
        except:
            # deal with exceptions encountered while executing the op.
            self.last_exception = sys.exc_info()[:2] + (None,)
            why = 'exception'

        return why

    def run_frame(self, frame):
        """Run a frame until it returns (somehow).
        Exceptions are raised, the return value is returned.
        """
        self.push_frame(frame)
        while True:
            byte_name, arguments = self.parse_byte_and_args()

            why = self.dispatch(byte_name, arguments)

            # Deal with any block management we need to do
            while why and frame.block_stack:
                why = self.manage_block_stack(why)

            if why:
                break

        self.pop_frame()

        if why == 'exception':
            exc, val, tb = self.last_exception
            e = exc(val)
            e.__traceback__ = tb
            raise e

        return self.return_value

该Block班

在为每个字节码指令实现方法之前，我们将简要讨论块。块用于某些类型的流控制，特别是异常处理和循环
。该块负责确保操作完成后数据堆栈处于适当的状态。例如，在循环中，循环运行时，特殊的迭代器对象保
留在堆栈上，但在循环结束时会弹出。解释器必须跟踪循环是继续还是结束。

为了跟踪这些额外的信息，解释器设置一个标志以指示其状态。我们实现这个标志作为一个变量叫why，它
可以是None或字符串中的一个"continue"，"break"，"exception"，或"return"。这表明应该对块堆栈和数
据堆栈进行哪种操作。回到迭代器示例，如果块堆栈的顶部是一个loop块且why代码为continue，则迭代器
对象应保留在数据堆栈上，但是如果why代码为break，则应将其弹出。

块操作的确切细节相当复杂，我们不会在此花费更多的时间，但是鼓励感兴趣的读者仔细看看。

Block = collections.namedtuple("Block", "type, handler, stack_height")

class VirtualMachine(object):
    [... snip ...]

    # Block stack manipulation
    def push_block(self, b_type, handler=None):
        stack_height = len(self.frame.stack)
        self.frame.block_stack.append(Block(b_type, handler, stack_height))

    def pop_block(self):
        return self.frame.block_stack.pop()

    def unwind_block(self, block):
        """Unwind the values on the data stack corresponding to a given block."""
        if block.type == 'except-handler':
            # The exception itself is on the stack as type, value, and traceback.
            offset = 3
        else:
            offset = 0

        while len(self.frame.stack) > block.level + offset:
            self.pop()

        if block.type == 'except-handler':
            traceback, value, exctype = self.popn(3)
            self.last_exception = exctype, value, traceback

    def manage_block_stack(self, why):
        """ """
        frame = self.frame
        block = frame.block_stack[-1]
        if block.type == 'loop' and why == 'continue':
            self.jump(self.return_value)
            why = None
            return why

        self.pop_block()
        self.unwind_block(block)

        if block.type == 'loop' and why == 'break':
            why = None
            self.jump(block.handler)
            return why

        if (block.type in ['setup-except', 'finally'] and why == 'exception'):
            self.push_block('except-handler')
            exctype, value, tb = self.last_exception
            self.push(tb, value, exctype)
            self.push(tb, value, exctype) # yes, twice
            why = None
            self.jump(block.handler)
            return why

        elif block.type == 'finally':
            if why in ('return', 'continue'):
                self.push(self.return_value)

            self.push(why)

            why = None
            self.jump(block.handler)
            return why
        return why

说明

剩下的就是实现许多方法来进行指示。实际的指令是解释器中最不有趣的部分，因此在这里我们只展示了少
数几个，但是完整的实现在GitHub上可用。（这里包含足够的指令来执行我们在上面反汇编的所有代码示例
。）

class VirtualMachine(object):
    [... snip ...]

    ## Stack manipulation

    def byte_LOAD_CONST(self, const):
        self.push(const)

    def byte_POP_TOP(self):
        self.pop()

    ## Names
    def byte_LOAD_NAME(self, name):
        frame = self.frame
        if name in frame.f_locals:
            val = frame.f_locals[name]
        elif name in frame.f_globals:
            val = frame.f_globals[name]
        elif name in frame.f_builtins:
            val = frame.f_builtins[name]
        else:
            raise NameError("name '%s' is not defined" % name)
        self.push(val)

    def byte_STORE_NAME(self, name):
        self.frame.f_locals[name] = self.pop()

    def byte_LOAD_FAST(self, name):
        if name in self.frame.f_locals:
            val = self.frame.f_locals[name]
        else:
            raise UnboundLocalError(
                "local variable '%s' referenced before assignment" % name
            )
        self.push(val)

    def byte_STORE_FAST(self, name):
        self.frame.f_locals[name] = self.pop()

    def byte_LOAD_GLOBAL(self, name):
        f = self.frame
        if name in f.f_globals:
            val = f.f_globals[name]
        elif name in f.f_builtins:
            val = f.f_builtins[name]
        else:
            raise NameError("global name '%s' is not defined" % name)
        self.push(val)

    ## Operators

    BINARY_OPERATORS = {
        'POWER':    pow,
        'MULTIPLY': operator.mul,
        'FLOOR_DIVIDE': operator.floordiv,
        'TRUE_DIVIDE':  operator.truediv,
        'MODULO':   operator.mod,
        'ADD':      operator.add,
        'SUBTRACT': operator.sub,
        'SUBSCR':   operator.getitem,
        'LSHIFT':   operator.lshift,
        'RSHIFT':   operator.rshift,
        'AND':      operator.and_,
        'XOR':      operator.xor,
        'OR':       operator.or_,
    }

    def binaryOperator(self, op):
        x, y = self.popn(2)
        self.push(self.BINARY_OPERATORS[op](x, y))

    COMPARE_OPERATORS = [
        operator.lt,
        operator.le,
        operator.eq,
        operator.ne,
        operator.gt,
        operator.ge,
        lambda x, y: x in y,
        lambda x, y: x not in y,
        lambda x, y: x is y,
        lambda x, y: x is not y,
        lambda x, y: issubclass(x, Exception) and issubclass(x, y),
    ]

    def byte_COMPARE_OP(self, opnum):
        x, y = self.popn(2)
        self.push(self.COMPARE_OPERATORS[opnum](x, y))

    ## Attributes and indexing

    def byte_LOAD_ATTR(self, attr):
        obj = self.pop()
        val = getattr(obj, attr)
        self.push(val)

    def byte_STORE_ATTR(self, name):
        val, obj = self.popn(2)
        setattr(obj, name, val)

    ## Building

    def byte_BUILD_LIST(self, count):
        elts = self.popn(count)
        self.push(elts)

    def byte_BUILD_MAP(self, size):
        self.push({})

    def byte_STORE_MAP(self):
        the_map, val, key = self.popn(3)
        the_map[key] = val
        self.push(the_map)

    def byte_LIST_APPEND(self, count):
        val = self.pop()
        the_list = self.frame.stack[-count] # peek
        the_list.append(val)

    ## Jumps

    def byte_JUMP_FORWARD(self, jump):
        self.jump(jump)

    def byte_JUMP_ABSOLUTE(self, jump):
        self.jump(jump)

    def byte_POP_JUMP_IF_TRUE(self, jump):
        val = self.pop()
        if val:
            self.jump(jump)

    def byte_POP_JUMP_IF_FALSE(self, jump):
        val = self.pop()
        if not val:
            self.jump(jump)

    ## Blocks

    def byte_SETUP_LOOP(self, dest):
        self.push_block('loop', dest)

    def byte_GET_ITER(self):
        self.push(iter(self.pop()))

    def byte_FOR_ITER(self, jump):
        iterobj = self.top()
        try:
            v = next(iterobj)
            self.push(v)
        except StopIteration:
            self.pop()
            self.jump(jump)

    def byte_BREAK_LOOP(self):
        return 'break'

    def byte_POP_BLOCK(self):
        self.pop_block()

    ## Functions

    def byte_MAKE_FUNCTION(self, argc):
        name = self.pop()
        code = self.pop()
        defaults = self.popn(argc)
        globs = self.frame.f_globals
        fn = Function(name, code, globs, defaults, None, self)
        self.push(fn)

    def byte_CALL_FUNCTION(self, arg):
        lenKw, lenPos = divmod(arg, 256) # KWargs not supported here
        posargs = self.popn(lenPos)

        func = self.pop()
        frame = self.frame
        retval = func(*posargs)
        self.push(retval)

    def byte_RETURN_VALUE(self):
        self.return_value = self.pop()
        return "return"

动态键入：编译器不知道的内容

您可能已经听说过的一件事是Python是一种“动态”语言-特别是它是“动态类型”的。至此，我们所做的工作
为该说明提供了一些启示。

在这种情况下，“动态”意味着的一件事是在运行时完成了许多工作。前面我们看到，Python编译器没有太多
有关代码实际作用的信息。例如，考虑mod下面的short函数。mod接受两个参数，并以第二个为模返回第一
个。在字节码中，我们看到变量a和b被加载，然后字节码BINARY_MODULO本身执行模运算。

>>> def mod(a, b):
...    return a % b
>>> dis.dis(mod)
  2           0 LOAD_FAST                0 (a)
              3 LOAD_FAST                1 (b)
              6 BINARY_MODULO
              7 RETURN_VALUE
>>> mod(19, 5)
4

计算19 %5的结果为4-不足为奇。如果我们用不同的参数调用它会怎样？

>>> mod("by%sde", "teco")
'bytecode'

刚才发生了什么？您可能之前已经看过这种语法，但是在不同的上下文中：

>>> print("by%sde" % "teco")
bytecode

使用符号%格式化用于打印的字符串意味着调用指令BINARY_MODULO。指令执行时，该指令将堆栈中的前两个
值一起修改-不管它们是字符串，整数还是您自己定义的类的实例。字节码是在编译函数时生成的（实际上
是在定义函数时），并且相同的字节码用于不同类型的参数。

Python编译器对字节码的影响了解得很少。由解释器确定BINARY_MODULO要操作的对象的类型，并为该类型
执行正确的操作。这就是为什么Python被描述为动态类型的原因：在实际运行该函数之前，您不知道该函数
的参数类型。相比之下，在使用静态类型化的语言时，程序员会先告诉编译器参数将是哪种类型（或者，编
译器会自行找出它们）。

编译器的无知是优化Python或静态分析它的挑战之一–仅查看字节码，而没有实际运行代码，您不知道每条
指令会做什么！实际上，您可以定义一个实现该__mod__方法的类，如果%在对象上使用Python，则将调用该
方法。因此BINARY_MODULO完全可以运行任何代码！

只看下面的代码，第一次的计算a % b似乎很浪费。

def mod(a,b):
    a % b
    return a %b

不幸的是，无法对这种代码进行静态分析（您可以在不运行该代码的情况下执行该操作），无法确定第一个
代码a % b实际上什么都不做。调用__mod__与%可能会写一个文件，或者相互作用与程序的其他部分，或做
字面上其他任何在Python的可能。当您不知道函数的功能时，很难对其进行优化！他们在Russell Power和
Alex Rubinsteyn的出色论文“我们能使解释的Python速度有多快？”中指出：“在通常不存在类型信息的情况
下，每条指令都必须视为INVOKE_ARBITRARY_METHOD。

结论

Byterun是一个紧凑的Python解释器，比CPython更易于理解。Byterun复制了CPython的主要结构细节：基于
堆栈的解释器，对称为字节码的指令集进行操作。它单步执行或跳过这些指令，并推入和弹出一堆数据。解
释器在调用函数和生成器并从中返回时在帧之间创建，销毁和跳转。Byterun也具有真正的解释器的局限性
：因为Python使用动态类型，所以解释器必须在运行时认真工作才能确定程序的正确行为。

我鼓励您分解自己的程序并使用Byterun运行它们。您将很快遇到该较短版本的Byterun无法实现的说明。完
整的实现可以在https://github.com/nedbat/byterun上找到，或者通过仔细阅读真正的CPython解释器
ceval.c，可以自己实现！

致谢

感谢Ned Batchelder发起该项目并指导我的贡献，感谢Michael Arntzenius帮助调试代码和编辑散文，感谢
Leta Montopoli进行编辑，并感谢整个Recurse Center社区的支持和关注。任何错误都是我自己的。

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

 1. 本章基于Python 3.5或更早版本产生的字节码，因为Python 3.6中的字节码规范有所更改。↩

 2. 感谢Michael Arntzenius对这个错误的见解。↩

Google 翻译

原文

提供更好的翻译建议
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
