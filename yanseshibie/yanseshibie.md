# 颜色识别

## 1.思路

##### 读取图片——>寻找轮廓——>遍历轮廓——>选择所需轮廓 

​                                                                                      |

​                                                                                      \/

#####  涂黑<—— 闭运算封口 <——提取绿色区域<——预处理 

### 1.读取图片

```c++
imread()
```

### 2.寻找轮廓

```C++ 
findContours（）
```



### 3.遍历轮廓

```C++
    if (!contours.empty() && !hierarchy.empty())

{

......

}

```

### 4.选择所需轮廓

```C++
    if (!contours.empty() && !hierarchy.empty())

{

......

}

```

### 5.预处理 

 高斯滤波处理   

```c++
 GaussianBlur()
```

### 6.提取绿色区域

```c++
    vectorcv::Mat c;

    split(temp, c);

    Mat diff = c[1] - c[2];

    threshold(diff, mask, 0, 255, THRESH_OTSU);

```

### 7.闭运算封口

```c++
cv::Mat element = getStructuringElement(MORPH_ELLIPSE, Size(9, 9));

    cv::morphologyEx(mask, mask, MORPH_CLOSE, element);

```

### 8.涂黑

```c++
    result = input.clone();

    result.setTo(Scalar(0, 0, 0), mask == 0);

```

## 2.代码

```
#include <iostream>
```

```
#include <opencv2/opencv.hpp>
```

```

```

```
using namespace std;
```

```
using namespace cv;
```

```

```

```
void Clear_MicroConnected_Areas(cv::Mat src, cv::Mat &dst, double min_area);
```

```
Mat IdentifyLeaves(cv::Mat input);
```

```

```

```
int main()
```

```
{
```

```
    Mat src = imread("D:/photo/34.jpg");
```

```
    Mat result = IdentifyLeaves(src);
```

```
    namedWindow("result", WINDOW_KEEPRATIO);
```

```
    imshow("result", result);
```

```
    waitKey(0);
```

```
    return 0;
```

```
}
```

```

```

```
void Clear_MicroConnected_Areas(cv::Mat src, cv::Mat &dst, double min_area)
```

```
{
```

```
    // 备份复制
```

```
    dst = src.clone();
```

```
    std::vector<std::vector<cv::Point> > contours;  // 创建轮廓容器
```

```
    std::vector<cv::Vec4i> 	hierarchy;
```

```

```

```
    // 寻找轮廓的函数
```

```
    // 第四个参数CV_RETR_EXTERNAL，表示寻找最外围轮廓
```

```
    // 第五个参数CV_CHAIN_APPROX_NONE，表示保存物体边界上所有连续的轮廓点到contours向量内
```

```
    cv::findContours(src, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE, cv::Point());
```

```

```

```
    if (!contours.empty() && !hierarchy.empty())
```

```
    {
```

```
        std::vector<std::vector<cv::Point> >::const_iterator itc = contours.begin();
```

```
        // 遍历所有轮廓
```

```
        while (itc != contours.end())
```

```
        {
```

```
            // 定位当前轮廓所在位置
```

```
            cv::Rect rect = cv::boundingRect(cv::Mat(*itc));
```

```
            // contourArea函数计算连通区面积
```

```
            double area = contourArea(*itc);
```

```
            // 若面积小于设置的阈值
```

```
            if (area < min_area)
```

```
            {
```

```
                // 遍历轮廓所在位置所有像素点
```

```
                for (int i = rect.y; i < rect.y + rect.height; i++)
```

```
                {
```

```
                    uchar *output_data = dst.ptr<uchar>(i);
```

```
                    for (int j = rect.x; j < rect.x + rect.width; j++)
```

```
                    {
```

```
                        // 将连通区的值置0
```

```
                        if (output_data[j] == 255)
```

```
                        {
```

```
                            output_data[j] = 0;
```

```
                        }
```

```
                    }
```

```
                }
```

```
            }
```

```
            itc++;
```

```
        }
```

```
    }
```

```
}
```

```

```

```
Mat IdentifyLeaves(cv::Mat input)
```

```
{
```

```
    CV_Assert(input.channels() == 3);
```

```
    Mat temp, result, mask, hole;
```

```
    int row = input.rows;
```

```
    int col = input.cols;
```

```

```

```
    // 高斯滤波
```

```
    GaussianBlur(input, temp, Size(5, 5), 0);
```

```

```

```
    // 通道拆分
```

```
    vector<cv::Mat> c;
```

```
    split(temp, c);
```

```

```

```
    // 绿通道-蓝通道，提取绿色区域
```

```
    Mat diff = c[1] - c[2];
```

```
    //Mat diff = c[1];
```

```
    threshold(diff, mask, 0, 255, THRESH_OTSU);
```

```

```

```
    // 闭运算封口
```

```
    cv::Mat element = getStructuringElement(MORPH_ELLIPSE, Size(9, 9));
```

```
    cv::morphologyEx(mask, mask, MORPH_CLOSE, element);
```

```

```

```
    // 孔洞闭合
```

```
    hole = 255 - mask;
```

```
    Clear_MicroConnected_Areas(hole, hole, row*col / 300);
```

```
    mask = 255 - hole;
```

```
    Clear_MicroConnected_Areas(mask, mask, row*col / 300);//除噪点
```

```

```

```
    // 识别区域标记
```

```
    result = input.clone();
```

```
    result.setTo(Scalar(0, 0, 0), mask == 0);
```

```
    return result;
}

```

![71983856193](C:\Users\CUISHU~1\AppData\Local\Temp\1719838561936.png)