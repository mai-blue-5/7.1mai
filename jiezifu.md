# 截字符

## 1.思路

预处理----->边缘检测----->画框

### 1.预处理

滤波，形态学开、闭操作，膨胀，腐蚀

```
medianBlur(result, result, 5);
```

```
        Mat mask,canny;
```

```
        Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
```

```
        morphologyEx(result, mask, MORPH_GRADIENT, kernel);//形态学梯度
```

```
        Canny(mask, canny, 100, 120);
```

```
        Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
```

```
        dilate(canny, kernel, element);//膨胀//预处理
```

```
        Canny(kernel, canny, 100, 120);
        bitwise_not(canny,canny);
        erode(canny, canny, element);//腐蚀
```



### 2.边缘检测canny

效果：

![71992890573](C:\Users\CUISHU~1\AppData\Local\Temp\1719928905738.png)

### 3.画框

```
for (size_t i = 0; i < contours.size(); ++i) {
```

```
            Rect rect = boundingRect(contours[i]);
```

```
            double area = contourArea(contours[i]);
```

```
                 if (area>200&&area<1000) {
```

```
                    rectangle(result, rect, Scalar(0, 255, 0), 2);
                 }
        }
```

效果：![71992893958](C:\Users\CUISHU~1\AppData\Local\Temp\1719928939581.png)![71992894773](C:\Users\CUISHU~1\AppData\Local\Temp\1719928947730.png)

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
vector<Mat> read_images_in_folder(cv::String pattern);
```

```
int main()
```

```
{
```

```
    cv::String pattern = "D:/photo/books/*.jpg"; // the pattern to match the image file names
```

```
      vector<cv::String> fn;
```

```
      glob(pattern, fn, false);
```

```
      for (size_t i = 0; i < fn.size(); i++)
```

```
      {
```

```
          Mat src = imread(fn[i]); // read the image
```

```
          if (src.empty()) // check if the image is successfully loaded
```

```
          {
```

```
              cout << "Could not read the image: " << fn[i] << endl;
```

```
              continue;
```

```
          }
```

```
         Mat result = IdentifyLeaves(src);
```

```
        medianBlur(result, result, 5);
```

```
        Mat mask,canny;
```

```
        Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
```

```
        morphologyEx(result, mask, MORPH_GRADIENT, kernel);//形态学梯度
```

```
        Canny(mask, canny, 100, 120);
```

```
        Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
```

```
        dilate(canny, kernel, element);//膨胀//预处理
```

```
        Canny(kernel, canny, 100, 120);
```

```
        bitwise_not(canny,canny);
```

```
        erode(canny, canny, element);//腐蚀
```

```
        vector<vector<Point>> contours;
```

```
        findContours(canny, contours, RETR_CCOMP, CHAIN_APPROX_NONE);
```

```
        // Draw bounding boxes around the contours
```

```
        for (size_t i = 0; i < contours.size(); ++i) {
```

```
            Rect rect = boundingRect(contours[i]);
```

```
            double area = contourArea(contours[i]);
```

```
                 if (area>200&&area<1000) {
```

```
                    rectangle(result, rect, Scalar(0, 255, 0), 2);
```

```
                 }
```

```
        }
```

```
    namedWindow("result", WINDOW_KEEPRATIO);
```

```
    imshow("result", result);
```

```
    namedWindow("1", WINDOW_KEEPRATIO);
```

```
    imshow("1", canny);
```

```
    waitKey(0);
```

```
      }
```

```
    return 0;
```

```
}
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
```

```
}


```