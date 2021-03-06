
<!-- vim-markdown-toc GFM -->

* [OpenCV](#opencv)
	* [core组件进阶](#core组件进阶)
		* [像素](#像素)
		* [离散傅里叶变换](#离散傅里叶变换)
		* [XML/YAML](#xmlyaml)
	* [数据类型](#数据类型)
		* [Mat](#mat)
		* [Matx](#matx)
		* [Vec](#vec)
		* [Point](#point)
		* [Scalar](#scalar)
		* [Size](#size)
		* [Rect](#rect)
		* [其他](#其他)
	* [模块](#模块)
		* [Window](#window)
		* [Trackbar](#trackbar)
	* [函数](#函数)
	* [直方图与匹配](#直方图与匹配)
		* [直方图计算与绘制](#直方图计算与绘制)
		* [直方图对比](#直方图对比)
		* [反向投影](#反向投影)
		* [模板匹配](#模板匹配)
	* [角点检测](#角点检测)
		* [Hrris 角点检测](#hrris-角点检测)
		* [Shi_Tomasi角点检测](#shi_tomasi角点检测)
		* [亚像素级角点检测](#亚像素级角点检测)
	* [特征检测与匹配](#特征检测与匹配)
		* [SURF特征点检测](#surf特征点检测)
		* [SURF特征提取](#surf特征提取)
		* [使用FLANN进行特征点匹配](#使用flann进行特征点匹配)
		* [寻找已知物体](#寻找已知物体)
		* [ORB特征提取](#orb特征提取)
	* [图像轮廓与图像分割修复](#图像轮廓与图像分割修复)
		* [查找并寻找轮廓](#查找并寻找轮廓)
		* [寻找物体的凸包](#寻找物体的凸包)
		* [利用多边形将轮廓包围](#利用多边形将轮廓包围)
		* [图像的矩](#图像的矩)
		* [分水岭算法](#分水岭算法)
		* [图像修补](#图像修补)
	* [图像变换](#图像变换)
		* [基于OpenCV的边缘检测](#基于opencv的边缘检测)
		* [霍夫变换](#霍夫变换)
		* [重映射](#重映射)
		* [仿射变换](#仿射变换)
		* [直方图均衡化](#直方图均衡化)
	* [图像处理/imgproc](#图像处理imgproc)
		* [平滑处理](#平滑处理)
		* [滤波](#滤波)
		* [漫水填充](#漫水填充)
		* [图像金字塔与图片尺寸缩放](#图像金字塔与图片尺寸缩放)
		* [阈值化:](#阈值化)

<!-- vim-markdown-toc -->
# OpenCV

## core组件进阶

### 像素
1. 函数
	- LUT
	- 计时函数
	- 访问图像三类方法
	- 分离颜色通道
		- split()

	- 多通道混合
		- merge

	- addWeighted()

2. 存储
3. 颜色空间
4. ROI
5. 对比度，亮度: ```g(i,j) = a*f(i, j) + b```

	- a: 对比度，所有值乘以一个数，数值相差更加的大，对比出来

	- b：亮度，所有点都更亮，

	- 图像的亮度是通过数值来表现。

### 离散傅里叶变换
1. 原理
	对于数字图像这种离散的信号，频率大小表示信号变换的剧烈程度或者说信号变化的快慢。

2. 频率越大，变换越剧烈，频率越小，信号越平缓，

3. 对应到的图像中，高频信号往往是图像中的边缘信号和噪声信号，而低频信号包含图像变化频繁的图像轮廓及背景灯信号。
	- 高频：变换剧烈的灰度分量，例如边界
	- 低频：变换缓慢的灰度分量，例如一片大海

4. 函数
	- dft()

	- getOptimalDFTSize()

	- copyMakeBorder()

	- magnitude()

	- log

	- normalize()

5. 步骤
	1. 载入原始图像
	2. 将图像扩大到合适尺寸

		- getOptimalDFTSize(): 返回给定向量尺寸的最有尺寸
		- copyMakeBorder(): 扩充图像函数

	3. 为傅里叶变换的结果(实部和虚部)分配存储空间
	4. 进行离散傅里叶变换
	5. 将复数转换为幅值

		- 将分离出来的频率和相位转换为幅值,幅值对应的是图像的颜色索引.越高越亮.
		- 频率越高,转换来的幅值越低,所以低频率的点像素高
		- dft转换来的图,低频的在4个角.高频的在中心.

	6. 进行对数尺度缩放

		- dft转换的幅值过高,在图像中基本没有显示.取对数,来减小幅值

	7. 剪切和重分布幅度图象限

		- 这样将低频转换到中心,显示的清楚

	8. 归一化

		- 还是为了显示，幅值依然能然超过可显示范围，这里将幅值转换为0-1之间,方便在图像中显示.

	9. 显示效果图

6. 能量

	- 高频: 中间, 幅值小, 能量低, 黑

	- 低频: 4角, 幅值大, 能量高, 白

7. 式子

	- 幅值=norimalize(magnitude(mat, dstmat))

8. 滤波器

	- 低通滤波器：只保留低频，会使得图像模糊

	- 高频滤波器：只保留高频，会使得图像细节增强

### XML/YAML

## 数据类型

### Mat
1. 引用计数，浅拷贝
2. 七种创建方法
3. 格式化输出

	- python
	- 逗号
	- 默认
	- Numpy ...

### Matx
1. 轻量级Mat，必须在使用前规定好大小
2.```2*3``` 的float Matx： Matx23f;

### Vec
1. Matex派生类，一维的Matx，和vector相似  

### Point
1. typedef Point_<int> Point2i;
2. typedef Point2i Point;
3. typefef Point_<float> Point2f;

### Scalar
1. 4个元素
2. typedef Scalar_<double> Scalar;
3. Scalar_是Vec4x的变种

### Size
1. typedef Size_<int> Size2i;
2. typedef Size2i Size;
3. 两个属性：width，height

### Rect
1. x、y、width、height

2. area()

3. contains(Point)是否在矩形内

4. inside(Rect)判断矩形是否在矩形内

5. tl()/br():返回左上角，右下角坐标

6. rect1& rect2/rect1 | rect2 :与或操作，图形重叠或者扩展

7. rect+point：平移

8. rect+size:缩放

### 其他
1. 深度

	- CV_8U、CV_16U、CV_16S\、CV_32F、CV_64F

## 模块

### Window
1. namedWindow

### Trackbar
1. createTrackbar
2. getTrackbarPos

## 函数
1. 基本函数

	- cvtColor
	- imread
	- imshow
	- imwrite

2. 绘制函数

	- DrawEllipse
	- DrawFilledCircle
	- DrawPolygon
	- DrawLine
	- main

3. 滤波函数
	- 方框滤波:
	```
	void boxFilter(InputArray src, OutputArray dst, int ddepth, Size ksize, 
		Point anchor=Point(-1, -1), boolnormalize=true, int borderType=BORDER_DEFAULT)
	```

	- 均值滤波:
	```
	void blur(InputArray src, OutputArray dst, int ddepth, Size ksize, 
		Point anchor=Point(-1, -1), int borderType=BORDER_DEFAULT)
	```

	- 高斯滤波:
	```
	void GaussianBlur(InputArray src, OutputArray dst, Size ksize, 
		double sigmax, double sigmay=0, int borderType=BORDER_DEFAULT)
	```

	- 中值滤波:
	```
	void medianBlur(InputArray src, OutArray dst, int ksize)//ksize必须是大于1的奇数
	```

	- 双边滤波器:
	```
	void bilateralFilter(InputArray src, OutputArray dst, int d, double sigmaColor,
		double sigmaSpace, int borderType=BORDER_DEFAULT)
	```

4. 图形变换
	- 腐蚀

	- 膨胀

	- 开运算

	- 闭运算

	- 顶帽

	- 黑帽

	- 填充
		1. 
		```
		int floodFill(InputOutputArray image, Point seedPoint, Scalar newVal, Rect* rect=0,
			Scalar loDiff=Scalar(), Scalar upDiff=Scalar(), int flags=4)
		```

		2. 
		```
		int floodFill(InputOutputArray image, InputOutputArray mask, Point seedPoint, 
			Scalar newVal, Rect* rect=0, Scalar loDiff=Scalar(), Scalar upDiff=Scalar(), 
			int flags=4)

		```



## 直方图与匹配

### 直方图计算与绘制

### 直方图对比

### 反向投影

### 模板匹配

## 角点检测

### Hrris 角点检测

### Shi_Tomasi角点检测

### 亚像素级角点检测

## 特征检测与匹配

### SURF特征点检测

### SURF特征提取

### 使用FLANN进行特征点匹配

### 寻找已知物体

### ORB特征提取

## 图像轮廓与图像分割修复

### 查找并寻找轮廓

### 寻找物体的凸包

### 利用多边形将轮廓包围

### 图像的矩

### 分水岭算法

### 图像修补

## 图像变换

### 基于OpenCV的边缘检测

### 霍夫变换

### 重映射

### 仿射变换

### 直方图均衡化

## 图像处理/imgproc

### 平滑处理

- 平滑处理也称模糊处理,是一种简单且使用频率很高的图像处理方法. 

- 平滑处理最常见用来减少图像上的噪点或者失真.再涉及降低图像分辨率时, 平滑处理是非常好用的方法. 

### 滤波
1. 消除图像中的噪声成分叫做图像的平滑化或滤波操作

2. 信号或图像的能量大部分集中在幅度谱的低频和中频段，而在较高频段，有用的信息经常被噪声淹没。

3. 邻域算子: 利用给定像素周围的像素值决定此像素的最终输出值的一种算子.
	1. 线性邻域滤波: 像素的输出值取决于输入像素的加权和.

3. 线性滤波
	1. 方框滤波
		1. 线性邻域滤波的值全是1,就是周围像素的的和
		2. 如果归一化,就是周围像素的平均值.

	2. 均值滤波: 归一化的方框滤波

	3. 高斯滤波: 高斯函数就是正态分布的曲线.一个钟形.sigma决定了钟的半径.

		- 因为是钟形,离中心越远的点对中心点的影响越小

		- 当离中心足够远时,高斯函数值无线接近0

		- 算子核的值就是高斯函数对应与x,y的值.

4. 非线性滤波
	1. 中值滤波: 
		1. 用像素点邻域灰度值的中值来代替该像素点的灰度值,在去除脉冲噪声,椒盐噪声的同
		时又能保留图像的边缘细节.

		2. 步骤:
			- 按强度值大小排列像素点
			- 选择排序像素集的中间值作为点[i,j]的新值
			- 注: 一般采用奇数点的邻域来计算中值,但像素点为偶数时,中值为中间两点的平均值.

	2. 双边滤波
		1. 比高斯滤波多了一个高斯方差sigma - d

		2. 在边缘附近,离得较远的像素不会对边缘上的像素值影响太多.

		3. 边滤波，不光考虑了像素在空间中位置远近程度的影响，还考虑了像素亮度相近程度的影响。

		4. 当远离边界时，即颜色十分相近，颜色权基本一样时，类似于高斯滤波，这样变可平滑处
		理图像。当处在边界时（所谓边界，就是颜色反差极大的地方），边界上的点互相颜色相近，
		会取极大的权值，而边界外的的点，颜色距离很远，权值取的很小（甚重可以忽略不计），
		这样就保护了边缘。

5. 形态学滤波: 基于形状的一系列图像处理操作

	1. 腐蚀和膨胀
		- 用途:
			消除噪声,分割出独立图像元素,在图像中链接相邻元素,寻找图像明显极大值区域或
			极小值区域,求出图像的梯度.

		- 腐蚀和膨胀是对白色部分(高亮部分)而言的,不是黑色部分.

		- 膨胀: 就是求局部最大值的操作.

		- 从数学角度来说,膨胀和腐蚀操作就是讲图像(或图像的一部分区域称之为A)与核(B)进行卷积.

		- 问题：如果卷积核的步长为1的话，应该在白点之后的所有像素都成为白点？
			如果步长不为1的话，dst的尺寸缩小，相当于了池化。?

		- answer: 这里这个算法应该不能就地转换(in-place),每一个生成的像素都是写道新的目标图上
		的,原图的像素值没有改变.也就不会出现最大值一直向后传播的过程.

	2. 开运算: 先腐蚀，然后膨胀
		1. dst = open(src, element) = dilate(erode(src, element))

		2. 用来消除小物体，在纤细点处分离物体，并且平滑较大物体的边界的同时不明显改变其面积。

		3. 结果是放大了裂缝或者局部低亮度区域

		4. 先腐蚀,小空洞都变黑了,消除了小物体,再膨胀,边缘都变白了.

	3. 闭运算: 先膨胀，后腐蚀
		1. dst = close(src, element) = erode(dilate(src, element))

		2. 排除小型黑洞

	4. 形态学梯度: 膨胀 - 腐蚀
		1. dst = morph - grad(src, element) = dilate(src, element) - erode(src, element)

		2. 膨胀的白 - 腐蚀的黑 = 未变的部分减去为0, 增添的白减去增添的黑,留下的白边就是边界的白.

		3. 对二值图像将团块(blob)的边缘突出出来,可以用来保留物体的边缘轮廓.

	5. 顶帽: 原图 减去 开运算
		1. dst = tophat(src, element) = src - open(src, element)
	
		2. 从原图减去开运算的裂缝或低亮度区域,得到的效果图突出了比原图轮廓周围的区域更明亮的区域,
		这一操作和选择的核的大小有关.

		3. 往往用来分离比临近点亮一些的斑块. 以暗边包裹白的中心,这样留下的是白色的中心.

		4. 在一幅图像具有大幅的背景,而微小物品比较有规律的情况下,可以使用顶帽运算进行背景提取.


	6. 黑帽: 闭运算减去原图
		1. dst = blackhat(src, element) = close(src, element) - src

		2. 突出了效果图比原来轮廓周围的区域更黑的区域,和核的大小有关.

		3. 用来分离临近点暗一些的斑块.有非常完美的轮廓.

### 漫水填充
1. 思想
	用特定的颜色填充连通区域(连通区域应该是像素值一样或者近似的空间区域,遇到颜色值相差过
大的阈值结束?)通过设置连通像素的上下限以及连通方式来达到不同的填充效果的方法

2. 用途:
	- 标记或分离图像的一部分
	- 从输入图像获取掩码区域

3. PhotoShop 魔术棒:
	floodFill是查找和种子点连通的颜色相同的点,魔术棒选择工具是查找和种子点连通的颜色相近的点,
	把和初始种子像素颜色相近的点压进栈做为新的种子.

4. 连通性用像素值的接近程度来衡量.

5. floodFill()

### 图像金字塔与图片尺寸缩放
> 主要用于图像分割，是一种以多分辨率来解释图像的有效但概念简单的结构。

1. 金字塔
	1. 高斯金字塔: 用来向下采样，主要的图像金字塔

	2. 拉普拉斯金字塔: 用来从金字塔底层图像重建上层未采样图像，在数字图像处理中也即是预测残差，
		可以对图像进行最大程度的还原，配合高斯金字塔一起使用。

2. 向下采样: pyrDown
	- 高斯核卷积，然后删除所有偶数行和偶数列，得到图像变为原来的四分之一。

3. 向上采样: pryUp
	- 图像先在每个维度上扩大为原来的两倍,新增的行(偶数行)以0填充
	- 使用先前同样的内核(乘以4)与放大后的图像卷积,获得"新增像素"的近似值

4. 普拉斯金字塔
	1. 第i层的定义``` L[i] = G[i] - UP(G[i+1]*g[5*5]) ```

	2. G[i]是第i层的图像,UP()操作是讲原图像中位置为(x,y)的像素映射到目标图像的(2x+1, 2y+1)
	位置,即向上取样.*表示卷积,g[5*5]是5*5的高斯内核

5. 普拉斯运算:```L[i] = G[i] - PyrUp(G[i+1])```

4. 尺寸调整函数: resize()

### 阈值化:

1. 固定阈值操作: Threshold()
2. 自适应阈值操作: adaptiveThreshold()

