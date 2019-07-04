#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
int main( )
{
	// 载入原图
	Mat image=imread("test.jpg");
	//创建窗口
	namedWindow( "双边滤波【原图】" );
	namedWindow( "双边滤波【效果图】");
	//显示原图
	imshow( "双边滤波【原图】", image );
	//进行双边滤波操作
	Mat out;
	bilateralFilter (image, out, 100, 0, 0 );
	//显示效果图
	imshow( "双边滤波【效果图】" ,out );
	waitKey( 0 );    
}
