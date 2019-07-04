#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main()
{
    Mat srcImage=imread("test.jpg");//读入原图

    namedWindow("中值滤波[原图]");
    namedWindow("中值滤波[效果图]");

    imshow("中值滤波[原图]",srcImage);

    //进行中值滤波操作
    Mat dstImage;
    medianBlur(srcImage,dstImage,7);
    //显示效果图
    imshow("中值滤波[效果图]",dstImage);




    Mat grayMat;
  //cvtColor(dstImage,grayMat,7);
    cvtColor(srcImage,grayMat,7);
    Scalar scalar = mean(grayMat);
    Mat matQua;
    Laplacian(grayMat, matQua, CV_16U);
    double meanValue = 0.0;
    meanValue = mean(matQua)[0];
    stringstream meanValueStream;
    string meanValueString;
    meanValueStream << meanValue;
    meanValueStream >> meanValueString;
    printf("qua: %s\n",meanValueString.c_str());






    waitKey(0);




    return 0;
}
