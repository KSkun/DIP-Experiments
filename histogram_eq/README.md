# 直方图均衡化/规定化

## 直方图均衡化

代码文件：`histogram_equal.py`

使用方法：`python histogram_equal.py lena_gray.png lena_eq.png`

效果：

![Lena原图](lena_gray.png)

![Lena均衡化](lena_eq.png)

## 直方图规定化

代码文件：`histogram_match.py`

使用方法：`python histogram_match.py histogram.txt lena_gray.png lena_match.png`

效果：

测试用例为二值化直方图，映射到灰度值 119 和 177 上。

![Lena原图](lena_gray.png)

![Lena均衡化](lena_match.png)