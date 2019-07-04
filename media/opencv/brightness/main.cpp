#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;
#define CV_MINMAX       32

double getAvg(Mat img)  
{
 Mat gray;
    cvtColor(img, gray, 7);  
 Scalar scalar = mean(gray);  
    return scalar.val[0];
}  

int main(int a,char**p)
{

	Mat img = imread("test.jpg");
	double b = getAvg(img);
	printf("brightness: %f\n",b);
	return 0;
}
