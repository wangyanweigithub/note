
<!-- vim-markdown-toc GFM -->

* [OpenCV 60](#opencv-60)
	* [core组件进阶 62](#core组件进阶-62)
		* [像素 64](#像素-64)
		* [离散傅里叶变换 88](#离散傅里叶变换-88)
		* [XML/YAML 156](#xmlyaml-156)
	* [数据类型 158](#数据类型-158)
		* [Mat 160](#mat-160)
		* [Matx 170](#matx-170)
		* [Vec 174](#vec-174)
		* [Point 177](#point-177)
		* [Scalar 182](#scalar-182)
		* [Size 187](#size-187)
		* [Rect 192](#rect-192)
		* [其他 209](#其他-209)
	* [模块 214](#模块-214)
		* [Window 216](#window-216)
		* [Trackbar 219](#trackbar-219)
	* [函数 223](#函数-223)
	* [直方图与匹配 239](#直方图与匹配-239)
		* [直方图计算与绘制 241](#直方图计算与绘制-241)
		* [直方图对比 243](#直方图对比-243)
		* [反向投影 245](#反向投影-245)
		* [模板匹配 247](#模板匹配-247)
	* [角点检测 249](#角点检测-249)
		* [Hrris 角点检测 251](#hrris-角点检测-251)
		* [Shi_Tomasi角点检测 253](#shi_tomasi角点检测-253)
		* [亚像素级角点检测 255](#亚像素级角点检测-255)
	* [特征检测与匹配 257](#特征检测与匹配-257)
		* [SURF特征点检测 259](#surf特征点检测-259)
		* [SURF特征提取 261](#surf特征提取-261)
		* [使用FLANN进行特征点匹配 263](#使用flann进行特征点匹配-263)
		* [寻找已知物体 265](#寻找已知物体-265)
		* [ORB特征提取 267](#orb特征提取-267)
	* [图像轮廓与图像分割修复 269](#图像轮廓与图像分割修复-269)
		* [查找并寻找轮廓 271](#查找并寻找轮廓-271)
		* [寻找物体的凸包 273](#寻找物体的凸包-273)
		* [利用多边形将轮廓包围 275](#利用多边形将轮廓包围-275)
		* [图像的矩 277](#图像的矩-277)
		* [分水岭算法 279](#分水岭算法-279)
		* [图像修补 281](#图像修补-281)
	* [图像变换 283](#图像变换-283)
		* [基于OpenCV的边缘检测 285](#基于opencv的边缘检测-285)
		* [霍夫变换 287](#霍夫变换-287)
		* [重映射 289](#重映射-289)
		* [仿射变换 291](#仿射变换-291)
		* [直方图均衡化 293](#直方图均衡化-293)
	* [图像处理/imgproc 295](#图像处理imgproc-295)
		* [平滑处理 297](#平滑处理-297)
		* [滤波 303](#滤波-303)
		* [漫水填充 332](#漫水填充-332)
		* [图像金字塔与图片尺寸缩放 336](#图像金字塔与图片尺寸缩放-336)
		* [阈值化: 343](#阈值化-343)

<!-- vim-markdown-toc -->
# OpenCV 60

## core组件进阶 62

### 像素 64
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

### 离散傅里叶变换 88
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

### XML/YAML 156

## 数据类型 158

### Mat 160
1. 引用计数，浅拷贝
2. 七种创建方法
3. 格式化输出

	- python
	- 逗号
	- 默认
	- Numpy ...

### Matx 170
1. 轻量级Mat，必须在使用前规定好大小
2. 2`*`3的float Matx： Matx23f;

### Vec 174
1. Matex派生类，一维的Matx，和vector相似  

### Point 177
1. typedef Point_<int> Point2i;
2. typedef Point2i Point;
3. typefef Point_<float> Point2f;

### Scalar 182
1. 4个元素
2. typedef Scalar_<double> Scalar;
3. Scalar_是Vec4x的变种

### Size 187
1. typedef Size_<int> Size2i;
2. typedef Size2i Size;
3. 两个属性：width，height

### Rect 192
1. x、y、width、height

2. area()

3. contains(Point)是否在矩形内

4. inside(Rect)判断矩形是否在矩形内

5. tl()/br():返回左上角，右下角坐标

6. rect1& rect2/rect1 | rect2 :与或操作，图形重叠或者扩展

7. rect+point：平移

8. rect+size:缩放

### 其他 209
1. 深度

	- CV_8U、CV_16U、CV_16S\、CV_32F、CV_64F

## 模块 214

### Window 216
1. namedWindow

### Trackbar 219
1. createTrackbar
2. getTrackbarPos

## 函数 223
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

3. 滤波函数：
	- boxblur: 
	```
		void BoxFilter(InputArray src, OutputArray dst, int depth, Size ksize, 
			Point anchor=Point(-1, -1), boolnormalize=true, int borderType=BORDER__DEFAULT)
	```

	- blur: 仅仅比BoxFilter少了一个归一化参数 
	```
		void blur(InputArray src, OutputArray dst, int depth, Size ksize, 
			Point anchor=Point(-1, -1), int borderType=BORDER__DEFAULT)
	```

	- GaussianBlur(): 
	```
		void GaussianBlur(InputArray src, OutputArray dst, Size ksize, 
			double sigmax, double sigmay=0, int borderType=BORDER__DEFAULT)
	```

## 直方图与匹配 239

### 直方图计算与绘制 241

### 直方图对比 243

### 反向投影 245

### 模板匹配 247

## 角点检测 249

### Hrris 角点检测 251

### Shi_Tomasi角点检测 253

### 亚像素级角点检测 255

## 特征检测与匹配 257

### SURF特征点检测 259

### SURF特征提取 261

### 使用FLANN进行特征点匹配 263

### 寻找已知物体 265

### ORB特征提取 267

## 图像轮廓与图像分割修复 269

### 查找并寻找轮廓 271

### 寻找物体的凸包 273

### 利用多边形将轮廓包围 275

### 图像的矩 277

### 分水岭算法 279

### 图像修补 281

## 图像变换 283

### 基于OpenCV的边缘检测 285

### 霍夫变换 287

### 重映射 289

### 仿射变换 291

### 直方图均衡化 293

## 图像处理/imgproc 295

### 平滑处理 297
1. 平滑处理也称模糊处理,是一种简单且使用频率很高的图像处理方法. 

2. 平滑处理最常见用来减少图像上的噪点或者失真.再涉及降低图像分辨率时, 平滑处理是非常好用的方法. 

### 滤波 303

1. 消除图像中的噪声成分叫做图像的平滑化或滤波操作

2. 信号或图像的能量大部分集中在幅度谱的低频和中频段，
而在较高频段，有用的信息经常被噪声淹没。

3. 邻域算子: 利用给定像素周围的像素值决定此像素的最终输出值的一种算子。
	1. 线性邻域滤波: 像素的输出值取决于输入像素的加权和。

4. 线性滤波
	1. 方框滤波: 
		- 邻域算子，加权值全是1。就是周围像素的和。
		- 如果归一化的话，就是周围像素的平均值。和均值滤波一样

	2. 均值滤波
		- 方框滤波的归一化。

	3. 高斯滤波
		- 矩阵的值是高斯函数对应于x，y的结果

		- 矩阵是一个钟形曲线，越远的点对结果的影响越小。

		- sigma影响高斯函数的钟形的半径。

5. 非线性滤波

	1. 中值滤波
	2. 双边滤波

6. 形态学滤波:

	1. 腐蚀和膨胀
	2. 开运算
	3. 闭运算
	4. 形态学梯度
	5. 顶帽
	6. 黑帽

### 漫水填充 332
1. 思想
2. floodFill()

### 图像金字塔与图片尺寸缩放 336

1. 图像金字塔
2. 高斯金字塔
3. 拉普拉斯金字塔
4. 尺寸调整函数: resize()

### 阈值化: 343

1. 固定阈值操作: Threshold()
2. 自适应阈值操作: adaptiveThreshold()

