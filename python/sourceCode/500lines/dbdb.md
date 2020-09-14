 

DBDB：狗床数据库

塔维·伯恩斯（Taavi Burns）

作为《对策》中最新的低音（有时是次中音），Taavi试图打破霉菌……有时只是无视它的存在。在他的职业
生涯中，工作场所的多样性确实是正确的：IBM（使用C和Perl），FreshBooks（使用所有功能），
Points.com（使用Python）以及现在的PagerDuty（使用Scala）。除此之外，您可能会发现他和他的儿子一
起玩《我的世界》，或者和他的妻子一起参加跑酷（或攀岩或其他冒险活动），而不是顺便骑上他的
Brompton折叠车。他编织大陆。

介绍

DBDB（狗床数据库）是一个实现简单键/值数据库的Python库。它使您可以将键与值相关联，并将该关联存
储在磁盘上以供以后检索。

DBDB旨在在计算机崩溃和错误情况下保留数据。它还避免了一次将所有数据保存在RAM中，这样您可以存储
比RAM还要多的数据。

记忆

我记得我第一次真正被错误困扰。当我输入完BASIC程序并运行它时，奇怪的闪亮像素出现在屏幕上，并且
该程序提早终止。当我回头看代码时，程序的最后几行不见了。

我妈妈的一位朋友知道如何编程，所以我们打了个电话。与她交谈的几分钟后，我发现了问题所在：程序太
大，并且已经侵占了视频内存。清除屏幕会截断程序，而火花是Applesoft BASIC将程序状态存储在RAM中的
行为的产物。

从那时起，我开始关心内存分配。我了解了指针以及如何使用malloc分配内存。我了解了如何在内存中布置
数据结构。而且我学会了如何更改它们，非常非常小心。

几年后，在阅读有关一种称为Erlang的面向过程的语言时，我了解到它实际上并不需要在进程之间发送数据
来复制数据，因为一切都是不可变的。然后，我在Clojure中发现了不可变的数据结构，并且它确实开始陷
入。

当我在2013年阅读有关CouchDB的信息时，我笑了笑，点了点头，意识到了管理复杂数据变化的结构和机制
。

我了解到您可以设计围绕不变数据构建的系统。

然后，我同意写一本书的章节。

我认为描述CouchDB的核心数据存储概念（据我所知）会很有趣。

在尝试编写二进制树算法以使树就位时，我对事情变得多么复杂感到沮丧。边缘情况的数量以及试图推断树
的一部分变化如何影响其他部分的事实使我的头部受伤。我不知道如何解释所有这些。

记住所学的教训，我偷看了用于更新不可变二叉树的递归算法，事实证明它相对简单。

我再次了解到，对不变的事物进行推理更容易。

因此，开始故事。

为什么有趣？

大多数项目都需要某种数据库。你真的不应该写你自己的东西。即使您只是将JSON写入磁盘，也有很多情况
会困扰您：

  • 如果您的文件系统空间不足怎么办？
  • 如果笔记本电脑的电池在保存时没电了怎么办？
  • 如果您的数据大小超出可用内存怎么办？（对于现代台式计算机上的大多数应用程序来说不太可能……但
    对于移动设备或服务器端Web应用程序来说则不太可能。）

但是，如果您想了解数据库如何处理所有这些问题，那么亲自编写一个是一个好主意。

我们在这里讨论的技术和概念应该适用于遇到故障时需要具有理性，可预测行为的任何问题。

说到失败...

表征故障

数据库的特征通常在于它们与ACID属性的紧密程度：原子性，一致性，隔离性和持久性。

DBDB中的更新是原子性的和持久性的，这两个属性将在本章后面进行介绍。DBDB不提供一致性保证，因为对
存储的数据没有任何约束。隔离同样没有实现。

应用程序代码当然可以施加自己的一致性保证，但是适当的隔离需要事务管理器。我们不会在这里尝试；但
是，您可以在CircleDB一章中了解有关事务管理的更多信息。

我们还需要考虑其他系统维护问题。在此实现中，不会回收过时的数据，因此重复的更新（甚至是对同一密
钥的更新）最终将占用所有磁盘空间。（您很快就会发现为什么会这样。）PostgreSQL将此回收称为“真空”
（这使旧的行空间可供重用），而CouchDB则将其称为“压缩”（通过重写数据的“实时”部分）。放入新文件
，然后将其原子地移到旧文件上）。

可以对DBDB进行增强以添加压缩功能，但这只是读者的练习^1。

DBDB的体系结构

DBDB从数据的逻辑结构（在此示例中为二进制树；逻辑层）的逻辑内容中分离出“将其放在磁盘上的某处”（
数据如何在文件中布置；物理层）的关注点。密钥/值存储（密钥a与值的关联foo；公共API）。

许多数据库将逻辑和物理方面分开，因为通常提供每个数据库的替代实现以获得不同的性能特征，例如DB2
的SMS（文件系统中的文件）与DMS（原始块设备）表空间，或者MySQL的替代引擎实现。

发现设计

本书中的大多数章节都描述了程序从开始到完成的过程。但是，这不是我们大多数人与正在处理的代码进行
交互的方式。我们最经常发现别人编写的代码，并弄清楚如何修改或扩展它以执行其他操作。

在本章中，我们将假定DBDB是一个完整的项目，并逐步学习它的工作原理。让我们首先探讨整个项目的结构
。

组织单位

此处按与最终用户的距离订购单位；也就是说，第一个模块是该程序的用户可能最需要了解的模块，而最后
一个模块则应与他们进行很少的交互。

  • tool.py 定义用于从终端窗口浏览数据库的命令行工具。

  • interface.py定义了一个类（DBDB），该类使用具体的BinaryTree实现来实现Python字典API 。这就是
    在Python程序中使用DBDB的方式。

  • logical.py定义逻辑层。它是键/值存储的抽象接口。

      □ LogicalBase提供用于逻辑更新（如get，set和commit）的API，并推迟到一个具体的子类来实现更
        新本身。它还管理存储锁定和取消引用内部节点。

      □ ValueRef是一个Python对象，它引用存储在数据库中的二进制Blob。间接使我们避免将整个数据存
        储一次加载到内存中。

  • binary_tree.py 在逻辑接口下定义了一个具体的二叉树算法。

      □ BinaryTree提供了二叉树的具体实现，以及用于获取，插入和删除键/值对的方法。BinaryTree代
        表一棵不可变的树；更新是通过返回与旧树具有相同结构的新树来执行的。

      □ BinaryNode 在二叉树中实现一个节点。

      □ BinaryNodeRef是专业的ValueRef，知道如何序列化和反序列化BinaryNode。

  • physical.py定义物理层。本Storage类提供持久的，（大部分）追加专用记录存储。

这些模块源于试图给每个班级一个单一责任的过程。换句话说，每个班级只有一个改变的理由。

读取值

我们从最简单的情况开始：从数据库中读取一个值。让我们看看当尝试获取与key fooin 相关的值时会发生
什么example.db：

$ python -m dbdb.tool example.db get foo

这main()将从模块运行功能dbdb.tool：

# dbdb/tool.py
def main(argv):
    if not (4 <= len(argv) <= 5):
        usage()
        return BAD_ARGS
    dbname, verb, key, value = (argv[1:] + [None])[:4]
    if verb not in {'get', 'set', 'delete'}:
        usage()
        return BAD_VERB
    db = dbdb.connect(dbname)          # CONNECT
    try:
        if verb == 'get':
            sys.stdout.write(db[key])  # GET VALUE
        elif verb == 'set':
            db[key] = value
            db.commit()
        else:
            del db[key]
            db.commit()
    except KeyError:
        print("Key not found", file=sys.stderr)
        return BAD_KEY
    return OK

该connect()函数打开数据库文件（可能会创建它，但永远不会覆盖它）并返回以下实例DBDB：

# dbdb/__init__.py
def connect(dbname):
    try:
        f = open(dbname, 'r+b')
    except IOError:
        fd = os.open(dbname, os.O_RDWR | os.O_CREAT)
        f = os.fdopen(fd, 'r+b')
    return DBDB(f)

# dbdb/interface.py
class DBDB(object):

    def __init__(self, f):
        self._storage = Storage(f)
        self._tree = BinaryTree(self._storage)

我们马上就看到DBDB了对的实例的引用Storage，但它也与共享了该引用self._tree。为什么？无法自行
self._tree管理对存储的访问？

在设计中，哪个对象“拥有”资源的问题通常是一个重要的问题，因为它可以提示我们哪些更改可能是不安全
的。在继续前进时，让我们牢记这个问题。

有了DBDB实例后，key通过字典查找（db[key]）完成获取值，这将导致Python解释器调用DBDB.__getitem__
()。

# dbdb/interface.py
class DBDB(object):
# ...
    def __getitem__(self, key):
        self._assert_not_closed()
        return self._tree.get(key)

    def _assert_not_closed(self):
        if self._storage.closed:
            raise ValueError('Database closed.')

__getitem__()通过调用确保数据库仍处于打开状态_assert_not_closed。啊哈！在这里，我们看到至少
DBDB需要直接访问我们的Storage实例的一个原因：这样它可以强制执行先决条件。（您是否同意这种设计
？您能想到我们可以采用的另一种方式吗？）

然后，DBDB 通过调用来检索与key内部相关的值，该值_tree由_tree.get()提供LogicalBase：

# dbdb/logical.py
class LogicalBase(object):
# ...
    def get(self, key):
        if not self._storage.locked:
            self._refresh_tree_ref()
        return self._get(self._follow(self._tree_ref), key)

get()检查我们是否已锁定存储。我们不是100％知道为什么这里可能有一个锁，但是我们可以猜测它可能存
在，以允许编写者序列化对数据的访问。如果存储未锁定怎么办？

# dbdb/logical.py
class LogicalBase(object):
# ...
def _refresh_tree_ref(self):
        self._tree_ref = self.node_ref_class(
            address=self._storage.get_root_address())

_refresh_tree_ref 使用磁盘上当前的内容重置树的数据“视图”，从而使我们能够执行完全最新的读取。

如果在尝试读取时存储被锁定怎么办？这意味着其他一些过程可能正在改变我们现在要读取的数据。我们读
取的数据不太可能是最新的数据状态。这通常称为“脏读”。这种模式允许许多读者访问数据而不必担心阻塞
，但代价是过时一些。

现在，让我们看一下如何实际检索数据：

# dbdb/binary_tree.py
class BinaryTree(LogicalBase):
# ...
    def _get(self, node, key):
        while node is not None:
            if key < node.key:
                node = self._follow(node.left_ref)
            elif node.key < key:
                node = self._follow(node.right_ref)
            else:
                return self._follow(node.value_ref)
        raise KeyError

这是标准的二叉树搜索，遵循对其节点的引用。通过阅读BinaryTree文档，我们知道Nodes和NodeRefs是值
对象：它们是不可变的，其内容永不改变。Node使用相关的键和值以及左右子级创建。这些联系也永远不会
改变。BinaryTree仅当替换根节点时，整个内容的内容才会明显更改。这意味着我们在执行搜索时无需担心
树的内容被更改。

找到关联的值后，将stdout在main()不添加任何新行的情况下将其写入，以准确保留用户的数据。

插入和更新

现在，我们将设置键foo到值bar在example.db：

$ python -m dbdb.tool example.db set foo bar

同样，这main()将从module 运行功能dbdb.tool。既然我们之前已经看过这段代码，我们将仅重点介绍以下
重要部分：

# dbdb/tool.py
def main(argv):
    ...
    db = dbdb.connect(dbname)          # CONNECT
    try:
        ...
        elif verb == 'set':
            db[key] = value            # SET VALUE
            db.commit()                # COMMIT
        ...
    except KeyError:
        ...

这次我们设置db[key] = value用于调用的值DBDB.__setitem__()。

# dbdb/interface.py
class DBDB(object):
# ...
    def __setitem__(self, key, value):
        self._assert_not_closed()
        return self._tree.set(key, value)

__setitem__确保数据库仍然打开，然后通过调用在内部存储从key到的关联。value_tree_tree.set()

_tree.set()由提供LogicalBase：

# dbdb/logical.py
class LogicalBase(object):
# ...
    def set(self, key, value):
        if self._storage.lock():
            self._refresh_tree_ref()
        self._tree_ref = self._insert(
            self._follow(self._tree_ref), key, self.value_ref_class(value))

set() 首先检查存储锁：

# dbdb/storage.py
class Storage(object):
    ...
    def lock(self):
        if not self.locked:
            portalocker.lock(self._f, portalocker.LOCK_EX)
            self.locked = True
            return True
        else:
            return False

这里有两件重要的事情要注意：

  • 我们的锁定是由名为portalocker的第三方文件锁定库提供的。
  • lock()返回False数据库是否已被锁定，True否则返回。

返回到_tree.set()，我们现在可以理解为什么它首先检查的返回值lock()：它使我们能够调用
_refresh_tree_ref最新的根节点引用，这样就不会丢失自上次刷新树以来其他进程可能进行的更新。磁盘
。然后，它用包含插入的（或更新的）键/值的新树替换根树节点。

插入或更新树不会改变任何节点，因为会_insert()返回新树。新树与前一棵树共享不变的部分，以节省内
存和执行时间。递归实现是很自然的：

# dbdb/binary_tree.py
class BinaryTree(LogicalBase):
# ...
    def _insert(self, node, key, value_ref):
        if node is None:
            new_node = BinaryNode(
                self.node_ref_class(), key, value_ref, self.node_ref_class(), 1)
        elif key < node.key:
            new_node = BinaryNode.from_node(
                node,
                left_ref=self._insert(
                    self._follow(node.left_ref), key, value_ref))
        elif node.key < key:
            new_node = BinaryNode.from_node(
                node,
                right_ref=self._insert(
                    self._follow(node.right_ref), key, value_ref))
        else:
            new_node = BinaryNode.from_node(node, value_ref=value_ref)
        return self.node_ref_class(referent=new_node)

注意我们如何总是返回一个新节点（用包裹NodeRef）。代替更新节点以指向新的子树，我们创建一个共享
未更改子树的新节点。这就是使此二叉树成为不可变数据结构的原因。

您可能在这里注意到了一些奇怪的事情：我们尚未对磁盘上的任何内容进行任何更改。我们要做的就是通过
移动树节点来操纵对磁盘数据的视图。

实际上为了写这些更改磁盘，我们需要显式调用commit()，这是我们看到的是我们的第二部分set操作在
tool.py本节的开始。

提交涉及写出内存中的所有脏状态，然后保存树的新根节点的磁盘地址。

从API开始：

# dbdb/interface.py
class DBDB(object):
# ...
    def commit(self):
        self._assert_not_closed()
        self._tree.commit()

的实现_tree.commit()来自LogicalBase：

# dbdb/logical.py
class LogicalBase(object)
# ...
    def commit(self):
        self._tree_ref.store(self._storage)
        self._storage.commit_root_address(self._tree_ref.address)

所有人都NodeRef知道如何通过首先要求孩子通过prepare_to_store()以下方式进行序列化来将自身序列化
到磁盘：

# dbdb/logical.py
class ValueRef(object):
# ...
    def store(self, storage):
        if self._referent is not None and not self._address:
            self.prepare_to_store(storage)
            self._address = storage.write(self.referent_to_string(self._referent))

self._tree_ref在这种情况下，in LogicalBase实际上是BinaryNodeRef（的子类ValueRef），因此具体的
实现prepare_to_store()是：

# dbdb/binary_tree.py
class BinaryNodeRef(ValueRef):
    def prepare_to_store(self, storage):
        if self._referent:
            self._referent.store_refs(storage)

的BinaryNode问题，_referent，要求其裁判来存储自己：

# dbdb/binary_tree.py
class BinaryNode(object):
# ...
    def store_refs(self, storage):
        self.value_ref.store(storage)
        self.left_ref.store(storage)
        self.right_ref.store(storage)

对于任何NodeRef未写入的更改（即no _address），这将一直向下递归。

现在，我们再次使用ValueRef的store方法备份堆栈。的最后一步store()是序列化此节点并保存其存储地址
：

# dbdb/logical.py
class ValueRef(object):
# ...
    def store(self, storage):
        if self._referent is not None and not self._address:
            self.prepare_to_store(storage)
            self._address = storage.write(self.referent_to_string(self._referent))

此时NodeRef的_referent都保证有可用自己所有的裁判的地址，所以我们通过创建一个字节字符串，它表示
此节点连载之：

# dbdb/binary_tree.py
class BinaryNodeRef(ValueRef):
# ...
    @staticmethod
    def referent_to_string(referent):
        return pickle.dumps({
            'left': referent.left_ref.address,
            'key': referent.key,
            'value': referent.value_ref.address,
            'right': referent.right_ref.address,
            'length': referent.length,
        })

从store()技术上讲，更新地址在技术上是的突变ValueRef。因为它对用户可见的值没有影响，所以我们可
以认为它是不可变的。

一旦store()根目录_tree_ref完成（在中LogicalBase.commit()），我们就知道所有数据都已写入磁盘。现
在，我们可以通过调用以下命令来提交根地址：

# dbdb/physical.py
class Storage(object):
# ...
    def commit_root_address(self, root_address):
        self.lock()
        self._f.flush()
        self._seek_superblock()
        self._write_integer(root_address)
        self._f.flush()
        self.unlock()

我们确保清除文件句柄（以便操作系统知道我们希望将所有数据保存到SSD等稳定的存储中）并写出根节点
的地址。我们知道最后一次写入是原子的，因为我们将磁盘地址存储在扇区边界上。这是文件中的第一件事
，因此无论扇区大小如何，都是如此，并且磁盘硬件保证了单扇区磁盘写入是原子的。

由于根节点地址具有旧值或新值（从不旧，也不新），因此其他进程可以从数据库中读取而不会获得锁定。
外部进程可能会看到旧树或新树，但永远不会看到两者的混合。这样，提交是原子的。

因为我们在写入根节点地址之前将新数据写入磁盘并调用fsyncsyscall ^2，所以无法提交未提交的数据。
相反，一旦根节点地址已更新，我们知道它引用的所有数据也都在磁盘上。这样，提交也是持久的。

大功告成！

NodeRefs如何节省内存

为避免将整个树结构同时保留在内存中，当从磁盘中读取逻辑节点时，其左，右子级（及其值）的磁盘地址
将被加载到内存中。访问子级及其值需要一个额外的函数调用NodeRef.get()来取消引用（“真正获取”）数
据。

我们只需要构建一个NodeRef地址：

+---------+
| NodeRef |
| ------- |
| addr=3  |
| get()   |
+---------+

调用get()它将返回具体节点，以及该节点的引用作为NodeRefs：

+---------+     +---------+     +---------+
| NodeRef |     | Node    |     | NodeRef |
| ------- |     | ------- | +-> | ------- |
| addr=3  |     | key=A   | |   | addr=1  |
| get() ------> | value=B | |   +---------+
+---------+     | left  ----+
                | right ----+   +---------+
                +---------+ |   | NodeRef |
                            +-> | ------- |
                                | addr=2  |
                                +---------+

当未提交对树的更改时，它们存在于内存中，并且具有从根到更改的叶的引用。更改尚未保存到磁盘，因此
更改的节点包含具体的键和值，并且没有磁盘地址。进行写入的过程可以看到未提交的更改，并且可以在发
出提交之前进行更多更改，因为NodeRef.get()如果有一个，它将返回未提交的值。通过API访问的已提交和
未提交数据之间没有区别。所有更新将自动显示给其他读者，因为直到将新的根节点地址写入磁盘后，更改
才可见。并发更新被磁盘上的锁定文件阻止。该锁在第一次更新时获取，并在提交后释放。

读者练习

DBDB允许许多进程一次读取同一数据库而不会阻塞；折衷是读者有时可以检索陈旧的数据。如果我们需要能
够连续读取一些数据怎么办？一个常见的用例是读取一个值，然后根据该值对其进行更新。您将如何编写方
法DBDB来执行此操作？为了提供此功能，您将需要进行哪些权衡？

通过替换中的字符串BinaryTree，可以完全更改用于更新数据存储的算法interface.py。数据存储倾向于使
用更复杂的搜索树类型，例如B树，B +树等，以提高性能。虽然需要平衡的二叉树（而这不是）
Ø （升ØG2（n ））Ø（升ØG2（ñ））随机节点读取以查找值，例如，B +树需要少得多 Ø （升ØG32（n ））
Ø（升ØG32（ñ））因为每个节点拆分了32种方式，而不仅仅是2种。这在实践中有很大的不同，因为从40亿
个条目中查找 升ØG2（232）= 32升ØG2（232）=32 至 升ØG32（232）≈ 6.4升ØG32（232）≈6.4查找。每次
查找都是随机访问，这对于具有旋转盘片的硬盘来说非常昂贵。SSD有助于延迟，但是I / O的节省仍然存在
。

默认情况下，存储值，ValueRef该值期望将字节作为值（直接传递给Storage）。二叉树节点本身只是的子
类ValueRef。通过json或msgpack存储更丰富的数据只需编写自己的并将其设置为即可value_ref_class。
BinaryNodeRef是使用pickle序列化数据的示例。

数据库压缩是另一个有趣的练习。压缩可以通过对树进行中间位数的遍历来完成，然后随便写出。最好将树
节点全部放在一起，因为遍历它们是为了查找任何数据。至少在压缩之后，将尽可能多的中间节点打包到磁
盘扇区中可以提高读取性能。如果您尝试自己实现此功能，则此处会有一些细微之处（例如，内存使用情况
）。请记住：始终在性能提升之前和之后进行基准测试！您通常会对结果感到惊讶。

模式与原则

测试接口，而不是实现。作为开发DBDB的一部分，我编写了许多测试，描述了我希望如何使用它。最初的测
试是针对数据库的内存版本运行的，然后我将DBDB扩展为可持久存储到磁盘，甚至后来添加了NodeRefs的概
念。大多数测试无需更改，这使我确信一切仍在进行。

尊重单一责任原则。班级最多应该有一个改变的理由。对于DBDB，这并不是严格的情况，但是有多种扩展途
径，只需要本地化更改即可。在添加功能时进行重构很高兴！

摘要

DBDB是一个简单的数据库，可以提供简单的保证，但是事情仍然变得很匆忙。对于管理这种复杂性，我所做
的最重要的事情是实现了一个具有可变数据结构的表面上可变的对象。我鼓励您下次在遇到棘手的问题时考
虑使用此技术，该问题似乎比您能跟踪的更多。

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

 1. 附加功能：您可以保证压缩的树结构是平衡的吗？这有助于随着时间的推移保持性能。↩

 2. 调用fsync文件描述符要求操作系统和硬盘驱动器（或SSD）立即写入所有缓冲的数据。为了提高性能，
    操作系统和驱动器通常不会立即编写所有内容。↩

Google 翻译

原文

提供更好的翻译建议
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
