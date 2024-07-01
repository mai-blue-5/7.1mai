/*#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    // 读取图片
    Mat src = imread("D:/photo/34.jpg", IMREAD_COLOR);
   // cvtColor(src, src, COLOR_BGR2GRAY);
    if (src.empty())
    {
        printf("无法读取图片");
        return -1;
    }

    // 转换为HSV颜色空间
    Mat hsv;
    cvtColor(src, hsv, COLOR_BGR2HSV);

    // 定义绿色范围
    Scalar lower_green(40, 40, 40);
    Scalar upper_green(70, 255, 255);

    // 创建掩码
    Mat mask;
    inRange(hsv, lower_green, upper_green, mask);

    // 对原始图像和掩码进行位运算
    Mat result;
    bitwise_and(src, src, result, mask);
    int new_width = 300;
    int new_height = 200;

    // 创建一个新的图片对象
    std::vector<std::vector<Point>> contours;
    findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // 绘制最小外接矩形
    for (size_t i = 0; i < contours.size(); i++)
    {
        Rect bounding_rect = boundingRect(contours[i]);
        rectangle(result, bounding_rect, Scalar(0, 255, 0), 2);
    }
    // 调整图片大小
    resize(src, result, Size(new_width, new_height), 0, 0, INTER_LINEAR);
    // 显示结果
    namedWindow("原图", WINDOW_KEEPRATIO);
    imshow("原图", src);
    namedWindow("掩码", WINDOW_KEEPRATIO);
    imshow("掩码", mask);
    namedWindow("结果", WINDOW_KEEPRATIO);
    imshow("结果", result);

    // 等待按键
    waitKey(0);

    return 0;
}*/
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void Clear_MicroConnected_Areas(cv::Mat src, cv::Mat &dst, double min_area);
Mat IdentifyLeaves(cv::Mat input);
vector<Mat> read_images_in_folder(cv::String pattern);
int main()
{
   /* cv::String pattern = "G:/temp_picture/*.jpg";
    vector<Mat> src = read_images_in_folder(pattern);*/
    Mat src = imread("D:/photo/34.jpg");

    Mat result = IdentifyLeaves(src);
    namedWindow("result", WINDOW_KEEPRATIO);
    imshow("result", result);
    waitKey(0);
    return 0;
}
vector<Mat> read_images_in_folder(cv::String pattern)
{
  vector<cv::String> fn;
  glob(pattern, fn, false);

  vector<Mat> images;
  size_t count = fn.size(); //number of png files in images folder
  for (size_t i = 0; i < count; i++)
  {
    images.push_back(imread(fn[i]));
    imshow("img", imread(fn[i]));
    waitKey(1000);
  }
  return images;
}
void Clear_MicroConnected_Areas(cv::Mat src, cv::Mat &dst, double min_area)
{
    // 备份复制
    dst = src.clone();
    std::vector<std::vector<cv::Point> > contours;  // 创建轮廓容器
    std::vector<cv::Vec4i> 	hierarchy;

    // 寻找轮廓的函数
    cv::findContours(src, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE, cv::Point());

    if (!contours.empty() && !hierarchy.empty())
    {
        std::vector<std::vector<cv::Point> >::const_iterator itc = contours.begin();
        // 遍历所有轮廓
        while (itc != contours.end())
        {
            // 定位当前轮廓所在位置
            cv::Rect rect = cv::boundingRect(cv::Mat(*itc));
            // contourArea函数计算连通区面积
            double area = contourArea(*itc);
            // 若面积小于设置的阈值
            if (area < min_area)
            {
                // 遍历轮廓所在位置所有像素点
                for (int i = rect.y; i < rect.y + rect.height; i++)
                {
                    uchar *output_data = dst.ptr<uchar>(i);
                    for (int j = rect.x; j < rect.x + rect.width; j++)
                    {
                        // 将连通区的值置0
                        if (output_data[j] == 255)
                        {
                            output_data[j] = 0;
                        }
                    }
                }
            }
            itc++;
        }
    }
}

Mat IdentifyLeaves(cv::Mat input)
{
    CV_Assert(input.channels() == 3);
    Mat temp, result, mask, hole;
    int row = input.rows;
    int col = input.cols;

    // 高斯滤波
    GaussianBlur(input, temp, Size(5, 5), 0);

    // 通道拆分
    vector<cv::Mat> c;
    split(temp, c);

    // 绿通道-蓝通道，提取绿色区域
    Mat diff = c[1] - c[2];
    threshold(diff, mask, 0, 255, THRESH_OTSU);

    // 闭运算封口
    cv::Mat element = getStructuringElement(MORPH_ELLIPSE, Size(9, 9));
    cv::morphologyEx(mask, mask, MORPH_CLOSE, element);

    // 孔洞闭合
    hole = 255 - mask;
    Clear_MicroConnected_Areas(hole, hole, row*col / 300);
    mask = 255 - hole;
    Clear_MicroConnected_Areas(mask, mask, row*col / 300);//除噪点

    // 识别区域标记
    result = input.clone();
    result.setTo(Scalar(0, 0, 0), mask == 0);
    return result;
}

