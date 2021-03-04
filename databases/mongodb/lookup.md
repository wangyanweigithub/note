### MongoDB中多表关联查询（$lookup）的深入讲解
> 更新时间：2018年12月03日 10:15:31   作者：东山絮柳仔  

	NoSql的多表关联一直是比较复杂的问题，下面这篇文章主要给大家介绍了关于MongoDB中多表关联查询（
$lookup）的相关资料，文中通过示例代码介绍的非常详细，需要的朋友可以参考下

#### 聚合框架
> 聚合框架是MongoDB的高级查询语言，它允许我们通过转换和合并多个文档中的数据来生成新的单个文档中
> 不存在的信息。

1. 聚合管道操作主要包含下面几个部分：
```
命令     功能描述
$project 指定输出文档里的字段.
$match   选择要处理的文档，与fine()类似。
$limit   限制传递给下一步的文档数量。
$skip    跳过一定数量的文档。
$unwind  扩展数组，为每个数组入口生成一个输出文档。
$group   根据key来分组文档。
$sort    排序文档。
$geoNear 选择某个地理位置附近的的文档。
$out     把管道的结果写入某个集合。
$redact  控制特定数据的访问。
$lookup
         多表关联（3.2版本新增）
```

#### $lookup的功能及语法

1. 主要功能是将每个输入待处理的文档，经过$lookup 阶段的处理，输出的新文档中会包含一个新生成的
数组列（户名可根据需要命名新key的名字）。数组列存放的数据是来自被Join 集合的适配文档，如果没有
，集合为空（即为[ ]）

2. 基本语法
```
{
 $lookup:
 {
 from: <collection to join>,
 localField: <field from the input documents>,
 foreignField: <field from the documents of the "from" collection>,
 as: <output array field>
 }
}
```

3. 语法的解释说明

	1. from 
		同一个数据库下等待被Join的集合. 源集合中的match值，如果输入的集合中，某文档没有 localField,
		这个Key（Field），在处理的过程中，会默认为此文档含有 localField：null的键值对。

	2. foreignField
		待Join的集合的match值，如果待Join的集合中，文档没有foreignField值，
		在处理的过程中，会默认为此文档含有 foreignField：null的键值对。
	
	3. as
		为输出文档的新增值命名。如果输入的集合中已存在该值，则会覆盖掉，

	4. （注：null = null 此为真）

4. 其语法功能类似于下面的伪SQL语句：
```
SELECT *, <output array field>
FROM collection
WHERE <output array field> IN (SELECT *
  FROM <collection to join>
  WHERE <foreignField>= <collection.localField>);
```

#### 案例
1. 假设有订单集合，存储的测试数据如下：
```
db.orders.insert([
 { "_id" : 1, "item" : "almonds", "price" : 12, "quantity" : 2 },
 { "_id" : 2, "item" : "pecans", "price" : 20, "quantity" : 1 },
 { "_id" : 3 }
])
```

2. 其中 item 对应数据为商品名称。另外一个就是就是商品库存集合，存储的测试数据如下：

```
db.inventory.insert([
 { "_id" : 1, "sku" : "almonds", description: "product 1", "instock" : 120 },
 { "_id" : 2, "sku" : "bread", description: "product 2", "instock" : 80 },
 { "_id" : 3, "sku" : "cashews", description: "product 3", "instock" : 60 },
 { "_id" : 4, "sku" : "pecans", description: "product 4", "instock" : 70 },
 { "_id" : 5, "sku": null, description: "Incomplete" },
 { "_id" : 6 }
])
```

3. 此集合中的 sku 数据等同于订单集合中的商品名称。

在这种模式设计下，如果要查询订单表对应商品的库存情况，应如何写代码呢？

很明显这需要两个集合Join。

场景简单，不做赘述，直送答案。其语句如下：

```
db.orders.aggregate([
 {
 $lookup:
 {
 from: "inventory",
 localField: "item",
 foreignField: "sku",
 as: "inventory_docs"
 }
 }
])
```

返回的执行结果如下：

```
    {
        "_id" : NumberInt("1"),
        "item" : "almonds",
        "price" : NumberInt("12"),
        "quantity" : NumberInt("2"),
        "inventory_docs" : [
            {
                "_id" : NumberInt("1"),
                "sku" : "almonds",
                "description" : "product 1",
                "instock" : NumberInt("120")
            }
        ]
    }


    {
        "_id" : NumberInt("2"),
        "item" : "pecans",
        "price" : NumberInt("20"),
        "quantity" : NumberInt("1"),
        "inventory_docs" : [
            {
                "_id" : NumberInt("4"),
                "sku" : "pecans",
                "description" : "product 4",
                "instock" : NumberInt("70")
            }
        ]
    }


    {
        "_id" : NumberInt("3"),
        "inventory_docs" : [
            {
                "_id" : NumberInt("5"),
                "sku" : null,
                "description" : "Incomplete"
            },
            {
                "_id" : NumberInt("6")
            }
        ]
    }
```

4. 分析查询语句和结果，回扣$lookup定义，可以将上面的处理过程，描述如下：

	从集合order中逐个获取文档处理，拿到一个文档后，会根据localField 值遍历被 Join的 inventory集合
（from: "inventory"），看inventory集合文档中 foreignField值是否与之相等。如果相等，就把符合条
件的inventory文档  整体内嵌到聚合框架新生成的文档中，并且新key 统一命名为 inventory_docs。考虑
到符合条件的文档不唯一，这个Key对应的Value是个数组形式。原集合中Key对应的值为Null值或不存在时
，需特别小心。

#### 说明
	在以下的说明中，为描述方便，将 from对应的集合定义为被join集合；待聚合的表成为源表；将
localField 和 foreignField 对应的Key 定义比较列。

1. 因客户端工具显示的问题，上面示例中查询结果重Int 类型值都自动显示为了 NumberInt("")。这个
NumberInt标注，请忽略，不影响我们的功能测试。

2. 这个示例中，一共输出了三个文档，在没有再次聚合（$match）的条件下，这个输出文档数量是以输入
文档的数量来决定的（由order来决定），而不是以被Join的集合（inventory）文档数量决定。

3. 在此需要特别强调的是输出的第三个文档。在源库中原文档没有要比较的列（即item值不存在，既不是
Null值，也不是值为空），此时和被Join 集合比较，如果被Join集合中比较列也恰好为NUll 或不存在的值
，此时，判断相等，即会把被Join集合中比较列为NUll 或值不存在文档吸收进来。

4. 假设源表（order）中比较列为某一个值，而此值在待比较表（inventory）的所有文档中都不存在，那
么查询结果会是什么样子呢？

order 集合在现有数据的基础上，再被insert 进一笔测试数据，这个订单的商品为 Start，在库存商品中
根本没有此数据。
```
db.orders.insert({"_id" : 4, "item" : "Start", "price" : 2000, "quantity" : 1 })
```

order集合的文档数量由之前的3个增长为4个。

再次执行查询
```

db.orders.aggregate([
 {
 $lookup:
 {
 from: "inventory",
 localField: "item",
 foreignField: "sku",
 as: "inventory_docs"
 }
 }
])
```

此时查看结果

```
    {
        "_id" : NumberInt("1"),
        "item" : "almonds",
        "price" : NumberInt("12"),
        "quantity" : NumberInt("2"),
        "inventory_docs" : [
            {
                "_id" : NumberInt("1"),
                "sku" : "almonds",
                "description" : "product 1",
                "instock" : NumberInt("120")
            }
        ]
    }

    {
        "_id" : NumberInt("2"),
        "item" : "pecans",
        "price" : NumberInt("20"),
        "quantity" : NumberInt("1"),
        "inventory_docs" : [
            {
                "_id" : NumberInt("4"),
                "sku" : "pecans",
                "description" : "product 4",
                "instock" : NumberInt("70")
            }
        ]
    }


    {
        "_id" : NumberInt("3"),
        "inventory_docs" : [
            {
                "_id" : NumberInt("5"),
                "sku" : null,
                "description" : "Incomplete"
            },
            {
                "_id" : NumberInt("6")
            }
        ]
    }


    {
        "_id" : NumberInt("4"),
        "item" : "Start",
        "price" : NumberInt("2000"),
        "quantity" : NumberInt("1"),
        "inventory_docs" : [ ]
    }
```

查询出的结果也由之前的3个变成了4个。比较特别的是第四个文档，其新增列为 "inventory_docs" : [ ]
，即值为空。所以，此时，实现的功能非常像关系型数据库的 left join。

那么，可不可以只筛选出新增列为空的文档呢？

即我们查询出，比较列的条件下，刷选出只在A集合中，而不在集合B的文档呢？就像关系数据库中量表Join
的 left join on a.key =b.key where b.key is null .

答案是可以的。

其实回到聚合框架上来，再次聚合一下就可以了，来一次$match就可以了。

执行的语句调整一下就OK了。

```
db.orders.aggregate([
 {
 $lookup:
 {
 from: "inventory",
 localField: "item",
 foreignField: "sku",
 as: "inventory_docs"
 }
 },
 { $match : {"inventory_docs" : [ ]} }
])

执行结果为

    {
        "_id" : NumberInt("4"),
        "item" : "Start",
        "price" : NumberInt("2000"),
        "quantity" : NumberInt("1"),
        "inventory_docs" : [ ]
    }
```

可以看出执行结果只有一个文档。这个文档表明的含义是：订单中有这个商品，但是库存中没有这个商品。

（$look只是聚合框架的一个stage，在其前前后后，都可以嵌入到其他的聚合管道的命令，例如
$match.$group等。下面的说明5，也可以说明一二）

5. 以上的比较列都是单一的Key/Value,如果复杂一点，如果比较的列是数组，我们又该如何关联呢？

我们接下来再来测试一把。将之前集合order 、inventory 插入的数据清空。

插入此场景下的新数据，向order中插入的数据，如下：

```
db.orders.insert({ "_id" : 1, "item" : "MON1003", "price" : 350, "quantity" : 2, "specs" :[ "27 inch", "Retina display", "1920x1080" ], "type" : "Monitor" })

specs 对应的value是数组格式。

向集合inventory 新插入的数据如下：

db.inventory.insert({ "_id" : 1, "sku" : "MON1003", "type" : "Monitor", "instock" : 120,"size" : "27 inch", "resolution" : "1920x1080" })

db.inventory.insert({ "_id" : 2, "sku" : "MON1012", "type" : "Monitor", "instock" : 85,"size" : "23 inch", "resolution" : "1280x800" })

db.inventory.insert({ "_id" : 3, "sku" : "MON1031", "type" : "Monitor", "instock" : 60,"size" : "23 inch", "display_type" : "LED" })
```

查询的语句如下：

```
db.orders.aggregate([
 {
 $unwind: "$specs"
 },
 {
 $lookup:
  {
  from: "inventory",
  localField: "specs",
  foreignField: "size",
  as: "inventory_docs"
 }
 },
 {
 $match: { "inventory_docs": { $ne: [] } }
 }
])
```

查询显示结果如下：

```
    {
        "_id" : NumberInt("1"),
        "item" : "MON1003",
        "price" : NumberInt("350"),
        "quantity" : NumberInt("2"),
        "specs" : "27 inch",
        "type" : "Monitor",
        "inventory_docs" : [
            {
                "_id" : NumberInt("1"),
                "sku" : "MON1003",
                "type" : "Monitor",
                "instock" : NumberInt("120"),
                "size" : "27 inch",
                "resolution" : "1920x1080"
            }
        ]
    }
```

仔细看啊，输出文档中的 specs 对应的数据变成了字符串类型（原集合为数组）。是什么发挥了如此神奇
功效？？？，请看黑板，请将目光集中在

```
{
 $unwind: "$specs"
 }

还有个小问题，大家猜一下，如果查询语句中没有

{
 $match: { "inventory_docs": { $ne: [] } }
 }

结果会是什么样呢？即查看语句修改为：

db.orders.aggregate([
 {
 $unwind: "$specs"
 },
 {
 $lookup:
  {
  from: "inventory",
  localField: "specs",
  foreignField: "size",
  as: "inventory_docs"
 }
 }
])
```

大家猜猜看！

大家猜猜看！

大家猜猜看！

呵呵...此时的结果是：

```
    文档1
    {
        "_id" : NumberInt("1"),
        "item" : "MON1003",
        "price" : NumberInt("350"),
        "quantity" : NumberInt("2"),
        "specs" : "27 inch",
        "type" : "Monitor",
        "inventory_docs" : [
            {
                "_id" : NumberInt("1"),
                "sku" : "MON1003",
                "type" : "Monitor",
                "instock" : NumberInt("120"),
                "size" : "27 inch",
                "resolution" : "1920x1080"
            }
        ]
    }

    文档2
    {
        "_id" : NumberInt("1"),
        "item" : "MON1003",
        "price" : NumberInt("350"),
        "quantity" : NumberInt("2"),
        "specs" : "Retina display",
        "type" : "Monitor",
        "inventory_docs" : [ ]
    }

    文档3

    {
        "_id" : NumberInt("1"),
        "item" : "MON1003",
        "price" : NumberInt("350"),
        "quantity" : NumberInt("2"),
        "specs" : "1920x1080",
        "type" : "Monitor",
        "inventory_docs" : [ ]
    }
```

你推算出正确结果了吗？

谢谢！！！

希望以上的讲解和演示能对大家学习$lookup有所帮助。

注：以上案例数据参考MongoDB官方网站，大家也可访问官网获取更多、更全的相关知识。
