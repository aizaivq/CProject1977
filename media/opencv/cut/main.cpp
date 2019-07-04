/*中值滤波：medianBlur函数是非线性滤波
    函数原型：void medianBlur(inputArray src,OutputArray dst,int ksize)
    参数详解：
               inputArray src------填1、 3、或者4通道的Mat类型的图像。当ksize为3或5的时候
                                          图像深度需为CV_8U、 CV_16U、 CV_32F其中之一，而对
                                          于较大孔径尺寸的图片，它只能是CV_8U
                  
               OutputArray dst-----目标图像，函数的输出参数，同源图像同尺寸，同类型
                
               int ksize------孔径的尺寸，参数必须是大于1的奇
*/

/**************新建QT控制台程序 *******************/
//#include <QCoreApplication>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main()
{
    Mat srcImage=imread("test.jpg");//读入原图


Rect rect(100, 20, 300, 200);
Mat cut = srcImage(rect);

    imshow("cut",cut);
    imshow("原图]",srcImage);


    waitKey(0);
    return 0;
}
