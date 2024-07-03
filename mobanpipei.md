# 模板匹配

## 1.思路

截取模板----->模板匹配----->画框

### 1.手动截取模板

读模板templateImages.push_back(cv::imread("D:/photo/books/11.png", cv::IMREAD_COLOR));

```
    templateImages.push_back(cv::imread("D:/photo/books/22.png", cv::IMREAD_COLOR));
```

```
    templateImages.push_back(cv::imread("D:/photo/books/44.png", cv::IMREAD_COLOR));
```

```
    templateImages.push_back(cv::imread("D:/photo/books/55.png", cv::IMREAD_COLOR));
```

```
    templateImages.push_back(cv::imread("D:/photo/books/77.png", cv::IMREAD_COLOR));
```

```
    templateImages.push_back(cv::imread("D:/photo/books/88.png", cv::IMREAD_COLOR));
```

```
    templateImages.push_back(cv::imread("D:/photo/books/CC.png", cv::IMREAD_COLOR));
    templateImages.push_back(cv::imread("D:/photo/books/xiegang.png", cv::IMREAD_COLOR));
    templateImages.push_back(cv::imread("D:/photo/books/dian.png", cv::IMREAD_COLOR));
```

添加字签

```
 // 字符标签
    std::vector<std::string> labels = {"1", "2","4","5","7","8","C","xg","dian"};
```

![72001696978](C:\Users\CUISHU~1\AppData\Local\Temp\1720016969786.png)

![72001698101](C:\Users\CUISHU~1\AppData\Local\Temp\1720016981013.png)![72001698883](C:\Users\CUISHU~1\AppData\Local\Temp\1720016988839.png

![72001701249](C:\Users\CUISHU~1\AppData\Local\Temp\1720017012492.png)





### 2.模板匹配

使用模板匹配函数matchTemplate();

```
    for (size_t i = 0; i < templateImages.size(); i++) {
```

```
        cv::Mat templImage = templateImages[i];
```

```
        cv::Mat resultImage;
```

```
        cv::matchTemplate(srcImage, templImage, resultImage, cv::TM_CCOEFF_NORMED);
```

```
        double minVal, maxVal;
```

```
        cv::Point minLoc, maxLoc;
```

```
        cv::Point matchLoc;
```

```
        double threshold = 0.5; // 可根据实际情况调整
```

```
        while (true) {
```

```
            cv::minMaxLoc(resultImage, &minVal, &maxVal, &minLoc, &maxLoc);
```

```
                if(maxVal >= threshold){
```

```
                    matchLoc = maxLoc;
```

```
        cv::rectangle(srcImage, matchLoc, cv::Point(matchLoc.x + templImage.cols, matchLoc.y + templImage.rows), cv::Scalar(0, 255, 0), 2);
```

```
        cv::putText(srcImage, labels[i], matchLoc, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 200), 2);
```

```
        cv::floodFill(resultImage, maxLoc, cv::Scalar(0), 0, cv::Scalar(0.1), cv::Scalar(1.0));
```

```
            }
```

```
            else {
```

```
                break;
```

```
            }
        }
    }
```

### 3.画框

```c++
  cv::rectangle(srcImage, matchLoc, cv::Point(matchLoc.x + templImage.cols, matchLoc.y + templImage.rows), cv::Scalar(0, 255, 0), 2);

 cv::putText(srcImage, labels[i], matchLoc, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 200), 2);

 cv::floodFill(resultImage, maxLoc, cv::Scalar(0), 0, cv::Scalar(0.1), cv::Scalar(1.0));

```

![72001706338](C:\Users\CUISHU~1\AppData\Local\Temp\1720017063385.png)

## 2.代码

```
cv::Mat srcImage = cv::imread("D:/photo/books/moban5.jpg", cv::IMREAD_COLOR);
```

```
    cv::Mat src = cv::imread("D:/photo/books/moban5.jpg", cv::IMREAD_COLOR);
```

```
    // 模板图像列表，每个模板对应一个字符
```

```
    std::vector<cv::Mat> templateImages;
```

```
    templateImages.push_back(cv::imread("D:/photo/books/11.png", cv::IMREAD_COLOR));
```

```
    templateImages.push_back(cv::imread("D:/photo/books/22.png", cv::IMREAD_COLOR));
```

```
    templateImages.push_back(cv::imread("D:/photo/books/44.png", cv::IMREAD_COLOR));
```

```
    templateImages.push_back(cv::imread("D:/photo/books/55.png", cv::IMREAD_COLOR));
```

```
    templateImages.push_back(cv::imread("D:/photo/books/77.png", cv::IMREAD_COLOR));
```

```
    templateImages.push_back(cv::imread("D:/photo/books/88.png", cv::IMREAD_COLOR));
```

```
    templateImages.push_back(cv::imread("D:/photo/books/CC.png", cv::IMREAD_COLOR));
```

```
    templateImages.push_back(cv::imread("D:/photo/books/xiegang.png", cv::IMREAD_COLOR));
```

```
    templateImages.push_back(cv::imread("D:/photo/books/dian.png", cv::IMREAD_COLOR));
```

```
    // 添加更多模板图像...
```

```

```

```
    // 字符标签
```

```
    std::vector<std::string> labels = {"1", "2","4","5","7","8","C","xg","dian"};
```

```
    // 添加更多字符标签...
```

```

```

```
    for (size_t i = 0; i < templateImages.size(); i++) {
```

```
        cv::Mat templImage = templateImages[i];
```

```
        cv::Mat resultImage;
```

```
        cv::matchTemplate(srcImage, templImage, resultImage, cv::TM_CCOEFF_NORMED);
```

```
        double minVal, maxVal;
```

```
        cv::Point minLoc, maxLoc;
```

```
        cv::Point matchLoc;
```

```
        double threshold = 0.5; // 可根据实际情况调整
```

```
        while (true) {
```

```
            cv::minMaxLoc(resultImage, &minVal, &maxVal, &minLoc, &maxLoc);
```

```
                if(maxVal >= threshold){
```

```
                    matchLoc = maxLoc;
```

```
        cv::rectangle(srcImage, matchLoc, cv::Point(matchLoc.x + templImage.cols, matchLoc.y + templImage.rows), cv::Scalar(0, 255, 0), 2);
```

```
        cv::putText(srcImage, labels[i], matchLoc, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 200), 2);
```

```
        cv::floodFill(resultImage, maxLoc, cv::Scalar(0), 0, cv::Scalar(0.1), cv::Scalar(1.0));
```

```
            }
```

```
            else {
```

```
                break;
```

```
            }
        }
    }
    }
```