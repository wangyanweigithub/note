
<!-- vim-markdown-toc GFM -->

* [vue索引 43](#vue索引-43)
	* [vue api 44](#vue-api-44)
		* [api 文档 46](#api-文档-46)
	* [vue官方文档 49](#vue官方文档-49)
		* [基础 51](#基础-51)
		* [计算属性和侦听器 54](#计算属性和侦听器-54)
		* [模板语法 57](#模板语法-57)
		* [Class 与 Style 绑定 60](#class-与-style-绑定-60)
		* [条件渲染 63](#条件渲染-63)
		* [列表渲染 69](#列表渲染-69)
		* [事件处理 97](#事件处理-97)
		* [表单输入绑定 100](#表单输入绑定-100)
		* [组件基础 103](#组件基础-103)
		* [组件注册 106](#组件注册-106)
		* [Prop 109](#prop-109)
		* [自定义事件 112](#自定义事件-112)
		* [插槽 126](#插槽-126)
		* [动态组件 & 异步组件 164](#动态组件--异步组件-164)
		* [处理边界情况 167](#处理边界情况-167)
		* [进入/离开 & 列表过度 180](#进入离开--列表过度-180)
		* [状态过渡 183](#状态过渡-183)
		* [混入 186](#混入-186)
		* [自定义指令 189](#自定义指令-189)
		* [渲染函数 & JSX 192](#渲染函数--jsx-192)
		* [插件 195](#插件-195)
		* [过滤器 198](#过滤器-198)
		* [单文件组件 201](#单文件组件-201)
		* [单元测试 204](#单元测试-204)
		* [TS支持 207](#ts支持-207)
		* [生产环境部署 210](#生产环境部署-210)
		* [路由 213](#路由-213)
		* [状态管理 216](#状态管理-216)
		* [服务端渲染 219](#服务端渲染-219)
		* [安全 222](#安全-222)
		* [深入响应式原理 225](#深入响应式原理-225)
	* [vue官方文档不重要模块 228](#vue官方文档不重要模块-228)
	* [总结 243](#总结-243)

<!-- vim-markdown-toc -->
# vue索引 43
## vue api 44
> vue所有命令,表达式,组件使用语法
### api 文档 46
1. api.md

## vue官方文档 49

### 基础 51
1. 文件: Vue实例.md

### 计算属性和侦听器 54
1. 文件: vue-计算属性和侦听器.md

### 模板语法 57
1. 文件: 模板语法.md

### Class 与 Style 绑定 60
1. 文件: vue_class_style_bind.md

### 条件渲染 63
1. 文件: vue条件渲染.md
2. 用 key 管理可复用的元素
	1. Vue 会尽可能高效地渲染元素，通常会复用已有元素而不是从头开始渲染。
	2. 如果元素上绑定一个key,就可以显示告诉vue这不是同一个元素,不要服用已有的元素,重新渲染一个新的元素.

### 列表渲染 69
1. 文件: vue列表渲染.md
2. v-for:
	- 遍历数组: 解包顺序是(value, index)
	- 遍历所有对象的属性时,解包的对象顺序是(value, key, index), 可以只有一个,两个或三个全要.

3. 属性状态改变,v-for对象的顺序不会改变,即使vue实例的属性改变,如果要改变顺序的话,需要为为元素加一个key attr. 

	为了给 Vue 一个提示，以便它能跟踪每个节点的身份，从而重用和重新排序现有元素(类似与输入元素重用,
只要显式说明不要重用已有元素,加一个key attribute就好,这回使得元素不匹配没法重用已有元素)</font>,

4. <font color=red>任何数据都不会被自动传递到组件里，因为组件有自己独立的作用域</font>。
为了把迭代数据传递到组件里，我们要使用 prop：

```
<my-component
  v-for="(item, index) in items"
  v-bind:item="item"
  v-bind:index="index"
  v-bind:key="item.id"
></my-component>
```

不自动将 item 注入到组件里的原因是，这会使得组件与 v-for 的运作紧密耦合。

5. <font color=green>父子组件之间共享prop都需要将需要共享的prop bind到组件的属性,
这样组件本身就有引用了不同作用域的prop.</font>

### 事件处理 97
1. 文件: vue事件处理.md

### 表单输入绑定 100
1. 文件: 表单输入绑定.md

### 组件基础 103
1. 文件: 组件基础.md

### 组件注册 106
1. 文件: 组件注册.md

### Prop 109
1. 文件: Prop.md

### 自定义事件 112
1. 文件: 自定义事件.md
2. 总结:
	1. 事件名用kebab-case
	2. 自定义组件的 v-model以改变默认v-model绑定到组件的 value属性和input事件.
	3. 原生事件绑定组件使用 .native
		1. 当监听类似与input这样的组件时,组件会经过重构可能改变节点树,
		导致<font color=green>绑定到根元素的事件和要监听的不是同一个元素.</font>

		2. $listeners: 所有作用在这个组件上的监听器组成的对象.


	4. 双向绑定: .sync

### 插槽 126
1. 文件: 插槽.md

2. <font color=red>该插槽跟模板的其它地方一样可以访问相同的实例 property (也就是相同的“作用域”)，
而不能访问 <navigation-link> 的作用域</font>。

3. <font color=green>记住:</font>
	这里的 `url` 会是 undefined，因为其 (指该插槽的) 内容是
	_传递给_ <navigation-link> 的而不是
	在 <navigation-link> 组件*内部*定义的。

4. 作为一条规则，请记住：
    - 父级模板里的所有内容都是在父级作用域中编译的；子模板里的所有内容都是在子作用域中编译的。
	- <font color=green>组件中的内容是传递给组件的,用的是字模板的prop,slot是在父级作用域中渲染好传递给子模板的.</font>

5. 后备内容: 相当于函数默认参数

6. 具有插槽,多个插槽并列,标识具体是那个插槽

4. <font color=green>分析:</font>
	1. slot 使用v-bind是为slot添加了一个属性,这个属性属于slot,slot的所有子节点都可以访问到这个属性

	2. 因为slot的作用域是子组件的作用域,所以user="user",是将子组件作用域中的值赋值到了slot控件的属性上.

	3. 所以父级作用域使用组件时,一般来说都是父级作用域的值,但是渲染slot组件时,可以通过指定slot的名字,
	使用slot的属性值,slot是一个对象,属性是对象的属性.也就是绑定到slot组件属性的值.

	4. 这样可以成立说明js也是传递的引用.slot属性引用了子组件的属性值.

5. 在这个例子中，我们选择将包含所有插槽 prop 的对象命名为 slotProps，但你也可以使用任意你喜欢的名字。
<font color=green>v-slot:default="aa",表明name是default的v-slot的所有属性是aa对象</font>

6. 跟 v-on 和 v-bind 一样，v-slot 也有缩写，即把参数之前的所有内容 (v-slot:) 替换为字符 #。
例如 v-slot:header 可以被重写为 #header：

7. 跟 v-on 和 v-bind 一样，v-slot 也有缩写，即把参数之前的所有内容 (v-slot:) 替换为字符 #。
例如 v-slot:header 可以被重写为 #header：

### 动态组件 & 异步组件 164
1. 文件: 动态组件与异步组件.md

### 处理边界情况 167
1. 文件: 处理边界情况.md

2. ref:
	- 应该是vue里有一个refs属性,所有组件都可以通过在template定义一个ref,
	将自己作用域里的属性注册到父节点的refs,这样所有父组件就可以访问子组件的属性了.

	- ref 是自属性自己定义的,就像是通过bind:name="name"将自己作用域中的属性绑定到节点上,
	使得父节点在渲染这个组件时可以访问子组件的作用域的属性一样, ref相当于子组件上的bind,
	但是它是显示引用(bind 似乎也是引用),并且,注册到了refs对象上.

	- 父节点可以通过this.$refs.name来使用它

### 进入/离开 & 列表过度 180
1. 文件: 进入_离开_列表过度.md

### 状态过渡 183
1. 文件: 状态过度.md

### 混入 186
1. 文件: 混入.md

### 自定义指令 189
1. 文件: 自定义指令.md

### 渲染函数 & JSX 192
1. 文件: 渲染函数与jsx

### 插件 195
1. 文件: 插件.md

### 过滤器 198
1. 文件: 过滤器.md

### 单文件组件 201
1. 文件: 文件组件.md

### 单元测试 204
1. 文件: 单元测试.md

### TS支持 207
1. 文件: TS支持.md

### 生产环境部署 210
1. 文件: 生产环境部署.md

### 路由 213
1. 文件: 路由.md

### 状态管理 216
1. 文件: 状态管理.md

### 服务端渲染 219
1. 文件: 服务端渲染.md

### 安全 222
1. 文件: 安全.md

### 深入响应式原理 225
1. 文件: 深入响应式原理.md

## vue官方文档不重要模块 228
0. 进入/离开 & 列表过渡

1. 深入了解组件

2. 过渡 & 动画

3. 规模化

4. 工具

5. 可复用性 & 组合

6. 内在

## 总结 243
1. summary.md
2. question.md
