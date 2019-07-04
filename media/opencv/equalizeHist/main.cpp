#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;
      
    int main(int argc, char *argv[])  
    {  
        Mat image = imread("test.jpg", 1);  
        if (image.empty())  
        {  
            std::cout << "打开图片失败,请检查" << std::endl;  
            return -1;  
        }  
        imshow("原图像", image);  
        Mat imageRGB[3];  
        split(image, imageRGB);  
        for (int i = 0; i < 3; i++)  
        {  
            equalizeHist(imageRGB[i], imageRGB[i]);  
        }  
        merge(imageRGB, 3, image);  
        imshow("直方图均衡化图像增强效果", image);  

    Mat grayMat;
  //cvtColor(dstImage,grayMat,7);
    cvtColor(image,grayMat,7);
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

        
        waitKey();  
        return 0;  
    }  

