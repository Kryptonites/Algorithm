#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//-----------------------------------��ȫ�ֱ����������֡�--------------------------------------    
//      ������ȫ�ֱ�������    
//---------------------------------------------------------------------------------------------
Mat srcImage, dstImage, greyImage, maskImage;
int g_nFillMode = 1;
int g_nLowDifference=20, g_nUpDifference=20;
int g_nConnectivity = 4;
int g_nNewMaskVal = 255;
bool g_bIsColor = true;
bool g_bUseMask = false;



//-----------------------------------��ShowHelpText( )������-----------------------------------    
//      ���������һЩ������Ϣ    
//---------------------------------------------------------------------------------------------
static void ShowHelpText()
{
	printf("\n\n\t��������˵��: \n\n"
	   	   "\t\t�����ͼ������- ������ˮ������\n"
		   "\t\t���̰�����ESC��- �˳�����\n"
		   "\t\t���̰�����1��-  �л���ɫͼ/�Ҷ�ͼģʽ\n"
		   "\t\t���̰�����2��- ��ʾ/������Ĥ����\n"
		   "\t\t���̰�����3��- �ָ�ԭʼͼ��\n"
		   "\t\t���̰�����4��- ʹ�ÿշ�Χ����ˮ���\n"
		   "\t\t���̰�����5��- ʹ�ý��䡢�̶���Χ����ˮ���\n"
		   "\t\t���̰�����6��- ʹ�ý��䡢������Χ����ˮ���\n"
		   "\t\t���̰�����7��- ������־���ĵͰ�λʹ��4λ������ģʽ\n"
		   "\t\t���̰�����8��- ������־���ĵͰ�λʹ��8λ������ģʽ\n");
}

//-----------------------------------��onMouse( )������--------------------------------------    
//      �����������ϢonMouse�ص�����  
//-------------------------------------------------------------------------------------------
static void onMouse(int event, int x, int y,int, void*)
{
	if (event != CV_EVENT_LBUTTONDOWN)
		return;

	//-------------------��<1>����floodFill����֮ǰ�Ĳ���׼�����֡�--------------- 
	Point seed = Point(x, y);
	int LowDifference = g_nFillMode == 0 ? 0 : g_nLowDifference;//�շ�Χ����ˮ��䣬��ֵ��Ϊ0��������Ϊȫ�ֵ�g_nLowDifference  
	int UpDifference = g_nFillMode == 0 ? 0 : g_nUpDifference;//�շ�Χ����ˮ��䣬��ֵ��Ϊ0��������Ϊȫ�ֵ�g_nUpDifference  
	int flags = g_nConnectivity + (g_nNewMaskVal << 8) +(g_nFillMode == 1 ? CV_FLOODFILL_FIXED_RANGE : 0);//��ʶ����0~7λΪg_nConnectivity��8~15λΪg_nNewMaskVal����8λ��ֵ��16~23λΪCV_FLOODFILL_FIXED_RANGE����0��  

	//�������bgrֵ  
	int b = (unsigned)theRNG() & 255;//�������һ��0~255֮���ֵ  
	int g = (unsigned)theRNG() & 255;//�������һ��0~255֮���ֵ  
	int r = (unsigned)theRNG() & 255;//�������һ��0~255֮���ֵ  
	Rect ccomp;//�����ػ��������С�߽��������  

	Scalar newVal = g_bIsColor ? Scalar(b, g, r) : Scalar(r*0.299 + g*0.587 + b*0.114);//���ػ��������ص���ֵ�����ǲ�ɫͼģʽ��ȡScalar(b, g, r)�����ǻҶ�ͼģʽ��ȡScalar(r*0.299 + g*0.587 + b*0.114)  

	Mat dst = g_bIsColor ? dstImage : greyImage;//Ŀ��ͼ�ĸ�ֵ  
	int area;

	//--------------------��<2>��ʽ����floodFill������-----------------------------
	if (g_bUseMask)
	{
		threshold(maskImage, maskImage, 1, 128, CV_THRESH_BINARY);
		area=floodFill(dstImage, maskImage, seed, newVal, &ccomp, Scalar(LowDifference, LowDifference, LowDifference), Scalar(UpDifference, UpDifference, UpDifference), flags);
		imshow("maskͼ", maskImage);

	}
	else
	{
		area = floodFill(dstImage, seed, newVal, &ccomp, Scalar(LowDifference, LowDifference, LowDifference), Scalar(UpDifference, UpDifference, UpDifference), flags);

	}
	imshow("Ч��ͼ", dstImage);
	cout << area << "�����ر��ػ�\n";
}

//-----------------------------------��main( )������--------------------------------------------    
//                          ����������̨Ӧ�ó������ں���    
//----------------------------------------------------------------------------------------------
int main()
{
	system("color 04");

	srcImage = imread("pic10.jpg", CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	if (!srcImage.data)
	{
		printf("��ȡͼƬ����");
		return false;
	}

	ShowHelpText();

	srcImage.copyTo(dstImage);
	cvtColor(srcImage, greyImage, CV_RGB2GRAY);
	maskImage.create(srcImage.rows + 1, srcImage.cols + 1, CV_8UC1);//����srcImage�ĳߴ�����ʼ����Ĥmask

	namedWindow("Ч��ͼ", 1);

	createTrackbar("�������ֵ", "Ч��ͼ", &g_nLowDifference, 255, 0);
	createTrackbar("�������ֵ", "Ч��ͼ", &g_nUpDifference, 255, 0);

	setMouseCallback("Ч��ͼ", onMouse, 0);

	while (1)
	{
		imshow("Ч��ͼ", g_bIsColor ? dstImage : greyImage);

		int c = waitKey(0);
		
		if ((c & 255) == 27)
		{
			cout << "�����˳�";
			break;
		}
		switch ((char)c)
			{
			case'1':
				if (g_bIsColor)
				{
					cout << "�л���ɫ/�Ҷ�ģʽ����ǰ����Ϊ������ɫģʽ���л�Ϊ���Ҷ�ģʽ��\n";
					cvtColor(srcImage, greyImage, CV_RGB2GRAY);
					maskImage = Scalar::all(0);
					g_bIsColor = false;
				}
				else
				{
					cout << "�л���ɫ/�Ҷ�ģʽ����ǰ����Ϊ�����Ҷ�ģʽ���л�Ϊ����ɫģʽ��\n";
					srcImage.copyTo(dstImage);
					maskImage = Scalar::all(0);
					g_bIsColor = true;
				}
				break;
			case'2':
				if (g_bUseMask)
				{
					destroyWindow("mask");
					g_bUseMask = false;
				}
				else
				{
					namedWindow("mask", 0);
					maskImage = Scalar::all(0);
					imshow("mask", maskImage);
					g_bUseMask = true;
				}
				break;
			case'3':
				cout << "������3�������£��ָ�ԭʼͼ��\n";
				srcImage.copyTo(dstImage);
				cvtColor(dstImage, greyImage, COLOR_BGR2GRAY);
				maskImage = Scalar::all(0);
				break;
			case'4':
				cout << "������4�������£�ʹ�ÿշ�Χ����ˮ���\n";
				g_nFillMode = 0;
				break;
			case'5':
				cout << "������5�������£�ʹ�ý��䡢�̶���Χ����ˮ���\n";
				g_nFillMode = 1;
				break;
			case '6':
				cout << "������6�������£�ʹ�ý��䡢������Χ����ˮ���\n";
				g_nFillMode = 2;
				break;
			case '7':
				cout << "������7�������£�������־���ĵͰ�λʹ��4λ������ģʽ\n";
				g_nConnectivity = 4;
				break;
			case '8':
				cout << "������8�������£�������־���ĵͰ�λʹ��8λ������ģʽ\n";
				g_nConnectivity = 8;
				break;
			}
		}
		
	return 0;
}
