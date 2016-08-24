#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <string>
#include <opencv2/core/core_c.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat srcImage;
Mat grayImage;
Mat TempImage;

int g_nThreshold = 130;
int g_NeighborValue = 10;
int g_ColorValue = 10;
int g_CoordinateValue = 10;

void on_change(int, void*);

void main()
{
	namedWindow("FindContours��Ե���",WINDOW_KEEPRATIO);

	createTrackbar("��ֵͼ��ֵ","FindContours��Ե���", &g_nThreshold, 255, on_change);
	createTrackbar("����뾶", "FindContours��Ե���", &g_NeighborValue, 50, on_change);
	createTrackbar("��ɫ�ռ�", "FindContours��Ե���", &g_ColorValue, 50, on_change);
	createTrackbar("����ռ�", "FindContours��Ե���", &g_CoordinateValue, 50, on_change);

	on_change(g_nThreshold, 0);

	while (char(waitKey(1)) != 'q'){}
	return;

}

void on_change(int, void*)
{

	srcImage = imread("pic1.jpg", CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	grayImage = imread("pic1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	TempImage.create(srcImage.size(),srcImage.type());

	bilateralFilter(srcImage, TempImage, g_NeighborValue, g_ColorValue, g_CoordinateValue);
	cvtColor(TempImage, grayImage, CV_RGB2GRAY);
	threshold(grayImage, grayImage, g_nThreshold, 255, CV_THRESH_BINARY);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(grayImage, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++)
	{
		drawContours(srcImage, contours, i, Scalar(0, 255, 0), 2, 8, hierarchy, 0, Point());
	}

	imshow("FindContours��Ե���", srcImage);

}