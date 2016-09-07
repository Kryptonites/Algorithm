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

	namedWindow("ԭͼ", 1);
	imshow("ԭͼ", srcImage);

	// ��ԭͼ��ת��Ϊ�Ҷ�ͼ��  
	cvtColor(srcImage, grey, CV_BGR2GRAY);

	namedWindow("Canny���ӱ�Ե���", 1);

	createTrackbar("��ֵ", "Canny���ӱ�Ե���", &g_nCannyThreshValue, 50, on_CannyEdge);
	on_CannyEdge(0, 0);

	while (char(waitKey(1)) != 'q'){}
	return;

}

void on_CannyEdge(int, void *)
{
	dstImage = Mat::zeros(srcImage.size(), srcImage.type());
	// ʹ�� 3x3�ں�������
	blur(grey, grey, Size(3, 3));
	//ʹ��Canny����
	Canny(grey, edge, g_nCannyThreshValue, g_nCannyThreshValue * 3, 3);
	//ʹ��Canny��������ı�Եͼ��Ϊ���룬����ԭͼsrcImage����Ŀ��ͼdstImage��
	srcImage.copyTo(dstImage, edge);
	imshow("Canny���ӱ�Ե���", dstImage);

}