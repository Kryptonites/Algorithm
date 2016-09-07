#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;

int g_nSobelValue = 0;

Mat srcImage, dstImage;
Mat X_Grad, Y_Grad;
Mat abs_Xgrad, abs_Ygrad;

void on_Sobel(int, void *);

void main()
{
	srcImage = imread("pic5.jpg", CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	namedWindow("原图", 1);
	imshow("原图", srcImage);

	namedWindow("Sobel算子测试", 1);
	createTrackbar("内核尺寸", "Sobel算子测试", &g_nSobelValue, 5, on_Sobel);
	on_Sobel(g_nSobelValue, 0);

	while (char(waitKey(1)) != 'q'){}
	return;
}

void on_Sobel(int, void *)
{
	dstImage = Mat::zeros(srcImage.size(), dstImage.type());

	Sobel(srcImage, X_Grad, srcImage.depth(), 1, 0, (g_nSobelValue * 2 + 1), 1, 1, BORDER_DEFAULT);
	convertScaleAbs(X_Grad, X_Grad);
	namedWindow("X方向变换", 1);
	imshow("X方向变换", X_Grad);
	Sobel(srcImage, Y_Grad, srcImage.depth(), 0, 1, (g_nSobelValue * 2 + 1), 1, 1, BORDER_DEFAULT);
	convertScaleAbs(Y_Grad, Y_Grad);
	namedWindow("Y方向变换", 1);
	imshow("Y方向变换", Y_Grad);
	addWeighted(X_Grad, 0.5, Y_Grad, 0.5, 0, dstImage);
	imshow("Sobel算子测试", dstImage);
}