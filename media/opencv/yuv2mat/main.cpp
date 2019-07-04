#include<opencv2/opencv.hpp>
#include<opencv2/core/core_c.h>
#include<iostream>
#include<opencv2/imgproc/types_c.h>
#include<opencv2/core/hal/interface.h>
#include<opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

#include "cv.h"
#include "highgui.h"

using namespace std;
using namespace cv;
#define CV_MINMAX       32
/*
mainYUuvImage.create( mainHeight * 3 / 2, mainWidth, CV_8UC1);
CopyYUVToImage(mainYUuvImage.data, mainY, mainU, mainV, mainWidth, mainHeight);
cv::Mat mainRgbImage;
cv::cvtColor(mainYUuvImage, mainRgbImage, CV_YUV2BGR_I420);
                 
cv::Mat subYuvImage;
subYuvImage.create(subHeight * 3 / 2,subWidth,  CV_8UC1);
CopyYUVToImage(subYuvImage.data, subY, subU, subV, subWidth, subHeight);
cv::Mat subRgbImage;
cv::cvtColor(subYuvImage, subRgbImage, CV_YUV2BGR_I420);
 
double alpha = 0.5;
cv::Mat dstRgbImage;
 
cv::Mat dstYuvImage;
cv::cvtColor(dstRgbImage, dstYuvImage, CV_BGR2YUV_I420);
 
CopyImageToYUV(mainY, mainU, mainV, dstYuvImage.data,mainWidth, mainHeight);
static void CopyYUVToImage(uchar * dst ,uint8_t *pY, uint8_t *pU , uint8_t *pV,int width, int height)
{
　　uint32_t size = width * height;
　　memcpy(dst, pY, size);
　　memcpy(dst + size, pU, size /4);
　　memcpy(dst + size + size /4, pV, size / 4);
}
 
static void CopyImageToYUV(uint8_t *pY, uint8_t *pU, uint8_t *pV, uchar * src ,int width, int height)
{
　　uint32_t size = width * height;
　　memcpy(pY, src, size);
　　memcpy(pU, src + size, size / 4);
　　memcpy(pV, src + size + size / 4, size / 4);
}
*/


void conv_yuv420_to_mat(Mat &dst, unsigned char* pYUV420, int width, int height)
{
	if (!pYUV420) {
		return;
	}
 
	IplImage *yuvimage, *rgbimg, *yimg, *uimg, *vimg, *uuimg, *vvimg;
 
	int nWidth = width;
	int nHeight = height;
	rgbimg = cvCreateImage(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 3);
	yuvimage = cvCreateImage(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 3);
 
	yimg = cvCreateImageHeader(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 1);
	uimg = cvCreateImageHeader(cvSize(nWidth / 2, nHeight / 2), IPL_DEPTH_8U, 1);
	vimg = cvCreateImageHeader(cvSize(nWidth / 2, nHeight / 2), IPL_DEPTH_8U, 1);
 
	uuimg = cvCreateImage(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 1);
	vvimg = cvCreateImage(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 1);
 
	cvSetData(yimg, pYUV420, nWidth);
	cvSetData(uimg, pYUV420 + nWidth * nHeight, nWidth / 2);
	cvSetData(vimg, pYUV420 + long(nWidth*nHeight*1.25), nWidth / 2);
	cvResize(uimg, uuimg, CV_INTER_LINEAR);
	cvResize(vimg, vvimg, CV_INTER_LINEAR);
 
	cvMerge(yimg, uuimg, vvimg, NULL, yuvimage);
	cvCvtColor(yuvimage, rgbimg, CV_YCrCb2RGB);
 
	cvReleaseImage(&uuimg);
	cvReleaseImage(&vvimg);
	cvReleaseImageHeader(&yimg);
	cvReleaseImageHeader(&uimg);
	cvReleaseImageHeader(&vimg);
 
	cvReleaseImage(&yuvimage);
 
	//dst = Mat(*rgbimg,int(1));
	dst = cvarrToMat(rgbimg, true);
	//rgbimg->
	cvReleaseImage(&rgbimg);
}

int main(int a,char**p)
{
	IplImage *yuvimage, *rgbimg, *yimg, *uimg, *vimg, *uuimg, *vvimg;
	int height = 480;
	int width = 640;
	unsigned char yuvbuff[width * height * 2];
	//unsigned char yuvbuff[width * height];
	Mat yuv;
	yuv.create(height * 2 ,width,CV_8UC1);
	FILE * file = fopen("test.yuv","r");
	fread(yuvbuff,sizeof(unsigned char),width * height *2,file);
	//Mat yuvMat(height ,width,CV_8UC2);
	Mat yuvMat(height,width,CV_8UC2);
	//Mat yuvMat(height ,width,CV_8UC1);
//	yuvMat.create(height ,width,CV_8UC2);
	yuvMat.data = yuvbuff;
       // imshow("原图像", yuvMat);

	//Rect rect(200,200,200,200);
	Rect rect(0,0,640,480);
	Mat cutMat = yuvMat(rect);

	//memcpy(yuvMat.data, yuvbuff, width * height *2*sizeof(unsigned char));
	Mat rgbMat;  
	rgbMat.create(height ,width,CV_8UC3);
	//cvtColor(yuvMat, rgbMat, CV_YUV2BGR_YUYV); //函数第三个参数需要视YUV图像格式而定
	cvtColor(cutMat, rgbMat, CV_YUV2BGR_YUYV); //函数第三个参数需要视YUV图像格式而定
	//cvtColor(yuvMat, rgbMat, CV_YUV2RGB_YUYV); //函数第三个参数需要视YUV图像格式而定
	fclose(file);	
  //      imshow("原图像", yuvMat);
        imshow("原图像", rgbMat);
        waitKey();
	return 0;
}
