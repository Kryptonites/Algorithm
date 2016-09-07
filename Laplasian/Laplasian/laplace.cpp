#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;

int g_nLaplacianValue = 0;

Mat srcImage, dstImage, grey, abs_dstImage;

void on_Laplace(int, void *);

void main()
{
	srcImage = imread("pic5.jpg", CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);

	namedWindow("原图", 1);
	imshow("原图", srcImage);

	namedWindow("拉普拉斯算子测试", 1);

	createTrackbar("参数值", "拉普拉斯算子测试", &g_nLaplacianValue, 15, on_Laplace);
	on_Laplace(g_nLaplacianValue, 0);

	while (char(waitKey(1)) != 'q'){}
	return;

}

void on_Laplace(int, void *)
{
	dstImage = Mat::zeros(srcImage.size(), srcImage.type());

	GaussianBlur(srcImage, srcImage, Size(3, 3), 0, 0, BORDER_DEFAULT);
	cvtColor(srcImage, grey, CV_BGR2GRAY);
	Laplacian(grey, dstImage, dstImage.depth(), g_nLaplacianValue * 2 + 1, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(dstImage, abs_dstImage);
	imshow("拉普拉斯算子测试", dstImage);
}