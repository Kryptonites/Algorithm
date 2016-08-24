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
	namedWindow("FindContours±ßÔµ¼ì²â",WINDOW_KEEPRATIO);

	createTrackbar("¶þÖµÍ¼ãÐÖµ","FindContours±ßÔµ¼ì²â", &g_nThreshold, 255, on_change);
	createTrackbar("ÁÚÓò°ë¾¶", "FindContours±ßÔµ¼ì²â", &g_NeighborValue, 50, on_change);
	createTrackbar("ÑÕÉ«¿Õ¼ä", "FindContours±ßÔµ¼ì²â", &g_ColorValue, 50, on_change);
	createTrackbar("×ø±ê¿Õ¼ä", "FindContours±ßÔµ¼ì²â", &g_CoordinateValue, 50, on_change);

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

	imshow("FindContours±ßÔµ¼ì²â", srcImage);

}