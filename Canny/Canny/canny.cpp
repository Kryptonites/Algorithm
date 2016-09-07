#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;

int g_nCannyThreshValue = 1;

Mat srcImage, dstImage, grey, edge;

void on_CannyEdge(int, void *);

void main()
{
	srcImage = imread("pic5.jpg", CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);

	namedWindow("原图", 1);
	imshow("原图", srcImage);

	// 将原图像转换为灰度图像  
	cvtColor(srcImage, grey, CV_BGR2GRAY);

	namedWindow("Canny算子边缘检测", 1);

	createTrackbar("阈值", "Canny算子边缘检测", &g_nCannyThreshValue, 50, on_CannyEdge);
	on_CannyEdge(0, 0);

	while (char(waitKey(1)) != 'q'){}
	return;

}

void on_CannyEdge(int, void *)
{
	dstImage = Mat::zeros(srcImage.size(), srcImage.type());
	// 使用 3x3内核来降噪
	blur(grey, grey, Size(3, 3));
	//使用Canny算子
	Canny(grey, edge, g_nCannyThreshValue, g_nCannyThreshValue * 3, 3);
	//使用Canny算子输出的边缘图作为掩码，来将原图srcImage拷到目标图dstImage中
	srcImage.copyTo(dstImage, edge);
	imshow("Canny算子边缘检测", dstImage);

}