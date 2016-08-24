#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//-----------------------------------【全局变量声明部分】--------------------------------------    
//      描述：全局变量声明    
//---------------------------------------------------------------------------------------------
Mat srcImage, dstImage, greyImage, maskImage;
int g_nFillMode = 1;
int g_nLowDifference=20, g_nUpDifference=20;
int g_nConnectivity = 4;
int g_nNewMaskVal = 255;
bool g_bIsColor = true;
bool g_bUseMask = false;



//-----------------------------------【ShowHelpText( )函数】-----------------------------------    
//      描述：输出一些帮助信息    
//---------------------------------------------------------------------------------------------
static void ShowHelpText()
{
	printf("\n\n\t按键操作说明: \n\n"
	   	   "\t\t鼠标点击图中区域- 进行漫水填充操作\n"
		   "\t\t键盘按键【ESC】- 退出程序\n"
		   "\t\t键盘按键【1】-  切换彩色图/灰度图模式\n"
		   "\t\t键盘按键【2】- 显示/隐藏掩膜窗口\n"
		   "\t\t键盘按键【3】- 恢复原始图像\n"
		   "\t\t键盘按键【4】- 使用空范围的漫水填充\n"
		   "\t\t键盘按键【5】- 使用渐变、固定范围的漫水填充\n"
		   "\t\t键盘按键【6】- 使用渐变、浮动范围的漫水填充\n"
		   "\t\t键盘按键【7】- 操作标志符的低八位使用4位的连接模式\n"
		   "\t\t键盘按键【8】- 操作标志符的低八位使用8位的连接模式\n");
}

//-----------------------------------【onMouse( )函数】--------------------------------------    
//      描述：鼠标消息onMouse回调函数  
//-------------------------------------------------------------------------------------------
static void onMouse(int event, int x, int y,int, void*)
{
	if (event != CV_EVENT_LBUTTONDOWN)
		return;

	//-------------------【<1>调用floodFill函数之前的参数准备部分】--------------- 
	Point seed = Point(x, y);
	int LowDifference = g_nFillMode == 0 ? 0 : g_nLowDifference;//空范围的漫水填充，此值设为0，否则设为全局的g_nLowDifference  
	int UpDifference = g_nFillMode == 0 ? 0 : g_nUpDifference;//空范围的漫水填充，此值设为0，否则设为全局的g_nUpDifference  
	int flags = g_nConnectivity + (g_nNewMaskVal << 8) +(g_nFillMode == 1 ? CV_FLOODFILL_FIXED_RANGE : 0);//标识符的0~7位为g_nConnectivity，8~15位为g_nNewMaskVal左移8位的值，16~23位为CV_FLOODFILL_FIXED_RANGE或者0。  

	//随机生成bgr值  
	int b = (unsigned)theRNG() & 255;//随机返回一个0~255之间的值  
	int g = (unsigned)theRNG() & 255;//随机返回一个0~255之间的值  
	int r = (unsigned)theRNG() & 255;//随机返回一个0~255之间的值  
	Rect ccomp;//定义重绘区域的最小边界矩形区域  

	Scalar newVal = g_bIsColor ? Scalar(b, g, r) : Scalar(r*0.299 + g*0.587 + b*0.114);//在重绘区域像素的新值，若是彩色图模式，取Scalar(b, g, r)；若是灰度图模式，取Scalar(r*0.299 + g*0.587 + b*0.114)  

	Mat dst = g_bIsColor ? dstImage : greyImage;//目标图的赋值  
	int area;

	//--------------------【<2>正式调用floodFill函数】-----------------------------
	if (g_bUseMask)
	{
		threshold(maskImage, maskImage, 1, 128, CV_THRESH_BINARY);
		area=floodFill(dstImage, maskImage, seed, newVal, &ccomp, Scalar(LowDifference, LowDifference, LowDifference), Scalar(UpDifference, UpDifference, UpDifference), flags);
		imshow("mask图", maskImage);

	}
	else
	{
		area = floodFill(dstImage, seed, newVal, &ccomp, Scalar(LowDifference, LowDifference, LowDifference), Scalar(UpDifference, UpDifference, UpDifference), flags);

	}
	imshow("效果图", dstImage);
	cout << area << "个像素被重绘\n";
}

//-----------------------------------【main( )函数】--------------------------------------------    
//                          描述：控制台应用程序的入口函数    
//----------------------------------------------------------------------------------------------
int main()
{
	system("color 04");

	srcImage = imread("pic10.jpg", CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	if (!srcImage.data)
	{
		printf("读取图片错误！");
		return false;
	}

	ShowHelpText();

	srcImage.copyTo(dstImage);
	cvtColor(srcImage, greyImage, CV_RGB2GRAY);
	maskImage.create(srcImage.rows + 1, srcImage.cols + 1, CV_8UC1);//利用srcImage的尺寸来初始化掩膜mask

	namedWindow("效果图", 1);

	createTrackbar("负差最大值", "效果图", &g_nLowDifference, 255, 0);
	createTrackbar("正差最大值", "效果图", &g_nUpDifference, 255, 0);

	setMouseCallback("效果图", onMouse, 0);

	while (1)
	{
		imshow("效果图", g_bIsColor ? dstImage : greyImage);

		int c = waitKey(0);
		
		if ((c & 255) == 27)
		{
			cout << "程序退出";
			break;
		}
		switch ((char)c)
			{
			case'1':
				if (g_bIsColor)
				{
					cout << "切换彩色/灰度模式，当前操作为将【彩色模式】切换为【灰度模式】\n";
					cvtColor(srcImage, greyImage, CV_RGB2GRAY);
					maskImage = Scalar::all(0);
					g_bIsColor = false;
				}
				else
				{
					cout << "切换彩色/灰度模式，当前操作为将【灰度模式】切换为【彩色模式】\n";
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
				cout << "按键“3”被按下，恢复原始图像\n";
				srcImage.copyTo(dstImage);
				cvtColor(dstImage, greyImage, COLOR_BGR2GRAY);
				maskImage = Scalar::all(0);
				break;
			case'4':
				cout << "按键“4”被按下，使用空范围的漫水填充\n";
				g_nFillMode = 0;
				break;
			case'5':
				cout << "按键“5”被按下，使用渐变、固定范围的漫水填充\n";
				g_nFillMode = 1;
				break;
			case '6':
				cout << "按键“6”被按下，使用渐变、浮动范围的漫水填充\n";
				g_nFillMode = 2;
				break;
			case '7':
				cout << "按键“7”被按下，操作标志符的低八位使用4位的连接模式\n";
				g_nConnectivity = 4;
				break;
			case '8':
				cout << "按键“8”被按下，操作标志符的低八位使用8位的连接模式\n";
				g_nConnectivity = 8;
				break;
			}
		}
		
	return 0;
}
