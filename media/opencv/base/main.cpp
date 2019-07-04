#include "cv.h"  
#include "highgui.h"  
#include <stdio.h>  
int main()  
{  
    IplImage *src=0;
    IplImage*dst =0;
    IplImage*dst2=0;  
    float k[9]={  
        1,2,1,
        2,4,2,
        1,2,1};  //高斯卷积3*3的核
 
        for(int i = 0 ; i< 9;i++){
            k[i] = float(k[i]/10);
        }
        CvMat Km;  
        Km = cvMat(3,3,CV_32F,k);  
 
        float k2[25] = {
           1, 2, 3, 2, 1,
           2, 5, 6, 5, 2,  
           3, 6, 8, 6, 3,
           2, 5, 6, 5, 2,  
           1, 2, 3, 2, 1};//高斯卷积5*5的核
            for(int j = 0 ; j<25; j++){
                k2[j] = float(k2[j]/30);
            }
            CvMat Km2;
            Km2 = cvMat(5,5,CV_32F,k2);
            src=cvLoadImage("test.jpg");
            dst=cvCloneImage(src);  //使用cvCloneImage时，dst无需初始化，直接复制
            dst2 = cvCloneImage(src);
            cvNamedWindow("src",CV_WINDOW_AUTOSIZE);  
            cvNamedWindow("filter3*3",CV_WINDOW_AUTOSIZE);
            cvNamedWindow("filter5*5",CV_WINDOW_AUTOSIZE);
 
            cvShowImage("src",src);  
 
            cvFilter2D(src,dst,&Km,cvPoint(-1,-1));  
            cvFilter2D(src, dst2, &Km2,cvPoint(-1,-1));
            //卷积本身公式比较复杂，但是经过舍去高阶小量，简化后成为模板操作
            cvShowImage("filter3*3",dst);  
            cvShowImage("filter5*5", dst2);
            cvWaitKey(0);  
            cvReleaseImage(&src);  
            cvReleaseImage(&dst);  
            cvReleaseImage(&dst2);
            cvDestroyAllWindows();
            return 0;  
}
