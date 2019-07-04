#include<opencv2/opencv.hpp>
#include<opencv2/core/core_c.h>
#include<iostream>
#include<opencv2/imgproc/types_c.h>
#include<opencv2/core/hal/interface.h>
#include<opencv2/imgproc/imgproc_c.h>
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

void cutg(int x,int y,int w,int hh)
{
	printf("cutg h: %d\n",hh);
}

void cuts(unsigned char *clip,unsigned char *data,int x,int y,int w,int h,int width,int height)
{
	int indexu = 0;
	int indexv = 0;
	int indexy = 0;
	int cy = y;
	int cx = 0;
	for(int indexy = 0,cy = y,cx = 0;indexy < w * h;)
	{
		if(cx >= x && cx < x + w)
		{
			clip[indexy] = data[cy * width + cx];
			int indexv = indexy / 4 + w * h;
			int indexu = indexy / 4 + w * h * 5 / 4;
			clip[indexv] = data[width * height + ((cy * width + cx) / 4)];
			clip[indexu] = data[width * height * 5 / 4 + ((cy * width + cx) / 4)];
			indexy++;
		}
		else if(cx >= x + w)
		{
			cy++;
			cx = -1;
		}
		cx++;
	}

}


void cutYuv(unsigned char *tarYuv, unsigned char *srcYuv, int startW,
		int startH, int cutW, int cutH, int srcW, int srcH) 
{
	int i;
	int j = 0;
	int k = 0;
	//分配一段内存，用于存储裁剪后的Y分量 
	unsigned char *tmpY = (unsigned char *)malloc(cutW*cutH);
	//分配一段内存，用于存储裁剪后的UV分量 
	unsigned char *tmpUV = (unsigned char *)malloc(cutW*cutH/2);
	for(i=startH; i<cutH+startH; i++) {
		// 逐行拷贝Y分量，共拷贝cutW*cutH
		memcpy(tmpY+j*cutW, srcYuv+startW+i*srcW, cutW);
		j++;
	}
	for(i=startH/2; i<(cutH+startH)/2; i++) {
		//逐行拷贝UV分量，共拷贝cutW*cutH/2
		memcpy(tmpUV+k*cutW, srcYuv+startW+srcW*srcH+i*srcW, cutW);
		k++;
	}
	//将拷贝好的Y，UV分量拷贝到目标内存中
	memcpy(tarYuv, tmpY, cutW*cutH);
	memcpy(tarYuv+cutW*cutH, tmpUV, cutW*cutH/2);
	free(tmpY);
	free(tmpUV);
}


int main(int a,char**p)
{
	IplImage *yuvimage, *rgbimg, *yimg, *uimg, *vimg, *uuimg, *vvimg;
	int height = 1080;
	int width = 1920;
	FILE * file = fopen("test.yuv","r");
	int x = 0;
	int y = 0;
	int w = 256;
	int h = 256;
	unsigned char yuvbuff[width * height * 3 / 2];
	unsigned char clipbuff[w * h * 3 / 2];
	printf("cut y: %d\n",y);
	fread(yuvbuff,sizeof(char),width * height *3 / 2,file);
	x = 0;
	y = 0;
	w = 256;
	h = 256;
	cuts(clipbuff,yuvbuff,x,y,w,h,width,height);
	Mat yuvMat(height * 3 / 2 ,width,CV_8UC1);
	yuvMat.data = (unsigned char*)yuvbuff;
	Mat resizeMat;
	resize(yuvMat,resizeMat,Size(300,300));
	imshow("yuv", yuvMat);
//	imshow("原图像", resizeMat);
	Mat rgbMat;  
	//rgbMat.create(height  ,width,CV_8UC3);
	rgbMat.create(256  ,256,CV_8UC3);
	//cvtColor(yuvMat, rgbMat, CV_YUV2BGR_YV12); //函数第三个参数需要视YUV图像格式而定
	cvtColor(resizeMat, rgbMat, CV_YUV2BGR_YV12); //函数第三个参数需要视YUV图像格式而定
//	imshow("原图像", resizeMat);
	imshow("原图像", rgbMat);
/*
//	resize(rgbMat,resizeMat,Size(256,256),(0,0),(0,0),INTER_LINEAR);
	fclose(file);	
//	imshow("原图像", rgbMat);
//	imshow("原图像", resizeMat);
	Mat grayMat;
//	cvtColor(rgbMat,grayMat,7);
//	Scalar scalar = mean(grayMat);
	cvtColor(resizeMat,grayMat,7);
	Scalar scalar = mean(grayMat);
	printf("brightness: %f\n",scalar.val[0]);
	Mat matQua;
	Laplacian(grayMat, matQua, CV_16U);
	double meanValue = 0.0;
	meanValue = mean(matQua)[0];
	stringstream meanValueStream;
	string meanValueString;
	meanValueStream << meanValue;
	meanValueStream >> meanValueString;
	printf("qua: %s\n",meanValueString.c_str());

*/
	waitKey();

	return 0;
}
