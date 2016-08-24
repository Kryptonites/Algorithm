#include <iostream>
#include <string.h>
#include <cxcore.h>
#include <cv.h>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <fstream>


#define _CV_SNAKE_BIG 2.e+38f
#define _CV_SNAKE_IMAGE 1
#define _CV_SNAKE_GRAD  2

#define CV_VALUE 1
#define CV_ARRAY 2

/*F///////////////////////////////////////////////////////////////////////////////////////

//    Name:      icvSnake8uC1R

//    Purpose:

//    Context:

//    Parameters:

//               src - source image,

//               srcStep - its step in bytes,

//               roi - size of ROI,

//               pt - pointer to snake points array

//               n - size of points array,

//               alpha - pointer to coefficient of continuity energy,

//               beta - pointer to coefficient of curvature energy,

//               gamma - pointer to coefficient of image energy,

//               coeffUsage - if CV_VALUE - alpha, beta, gamma point to single value

//                            if CV_MATAY - point to arrays

//               criteria - termination criteria.

//               scheme - image energy scheme

//                         if _CV_SNAKE_IMAGE - image intensity is energy

//                         if _CV_SNAKE_GRAD  - magnitude of gradient is energy

//    Returns:

//F*/


using namespace cv;
using namespace std;

static CVStatus icvSnake8uC1R(
unsigned char *src,   //ԭʼͼ������
int srcStep,         //ÿ�е��ֽ���
CvSize roi,         //ͼ��ߴ�
Point * pt,       //������(���ζ���)
int n,            //������ĸ���
float *alpha,       //ָ�����ָ�룬�������ǵ���ֵ��Ҳ�����������������һ�µ�����
float *beta,        //�µ�ֵ��ͬ��
float *gamma,       //�õ�ֵ��ͬ��
int coeffUsage,   //ȷ�����¦�����������ֵ���Ǹ�����
Size win,       //ÿ����������������С�������С,���Ϊ����
TermCriteria criteria,   //�ݹ������ֹ������׼��
int scheme)         //ȷ��ͼ��������������ѡ��1Ϊ�Ҷȣ�2Ϊ�Ҷ��ݶ�
{

	int i, j, k;

	int neighbors = win.height * win.width;    //��ǰ�������е�ĸ���

	//��ǰ���λ��

	int centerx = win.width >> 1;
	int centery = win.height >> 1;

	float invn;        //n �ĵ���
	int iteration = 0;     //��������
	int converged = 0;      //������־��0Ϊ������

	//����
	float *Econt;    //
	float *Ecurv;   //������������
	float *Eimg;    //ͼ������
	float *E;      //

	//���¦õĸ���
	float _alpha, _beta, _gamma;

	/*#ifdef GRAD_SNAKE */
	float *gradient = NULL;
	uchar *map = NULL;
	int map_width = ((roi.width - 1) >> 3) + 1;
	int map_height = ((roi.height - 1) >> 3) + 1;

	//CvSepFilter pX, pY;

#define WTILE_SIZE 8
#define TILE_SIZE (WTILE_SIZE + 2)       

	short dx[TILE_SIZE*TILE_SIZE], dy[TILE_SIZE*TILE_SIZE];
	Mat _dx = Mat(TILE_SIZE, TILE_SIZE, CV_16SC1, dx);
	Mat _dy = Mat(TILE_SIZE, TILE_SIZE, CV_16SC1, dy);
	Mat _src = Mat(roi.height, roi.width, CV_8UC1, src);

	//�������ĺ�����
	if (src == NULL)
		return CV_StsNullPtr;

	if ((roi.height <= 0) || (roi.width <= 0))
		return CV_StsBadSize;

	if (srcStep < roi.width)
		return CV_StsBadSize;

	if (pt == NULL)
		return CV_StsNullPtr;

	if (n < 3)                         //����������Ҫ����
		return CV_StsBadSize;

	if (alpha == NULL)
		return CV_StsNullPtr;

	if (beta == NULL)
		return CV_StsNullPtr;

	if (gamma == NULL)
		return CV_StsNullPtr;

	if (coeffUsage != CV_VALUE && coeffUsage != CV_ARRAY)
		return CV_StsBadFlag;

	if ((win.height <= 0) || (!(win.height & 1)))   //�����������ڱ���������
		return CV_StsBadSize;

	if ((win.width <= 0) || (!(win.width & 1)))
		return CV_StsBadSize;

	invn = 1 / ((float)n);        //��������n�ĵ���

	if (scheme == _CV_SNAKE_GRAD)
	{

		//ͼ���ݶȴ�Ż�����
		gradient = (float *)malloc(roi.height * roi.width * sizeof(float));

		if (!gradient)
			return CV_StsNoMem;

		//map���ڱ�־��Ӧλ�õķֿ��ͼ�������Ƿ��Ѿ����
		map = (uchar *)malloc(map_width * map_height);

		if (!map)
		{
			free(&gradient);
			return CV_StsNoMem;
		}

		//���map��־
		memset((void *)map, 0, map_width * map_height);
	}

	//���������Ĵ�Ŵ���ȡÿ������������
	Econt = (float *)malloc(neighbors * sizeof(float));
	Ecurv = (float *)malloc(neighbors * sizeof(float));
	Eimg = (float *)malloc(neighbors * sizeof(float));
	E = (float *)malloc(neighbors * sizeof(float));

	//��ʼ����
	while (!converged)    //������־��Чʱ����
	{
		float ave_d = 0;  //���������ƽ������
		int moved = 0;      //��������ʱ�������ƶ�������

		converged = 0;       //��־δ����
		iteration++;        //���µ�������+1

		//���������и����ƽ������
		//�ӵ�0����n-1�ľ����
		for (i = 1; i < n; i++)
		{
			int diffx = pt[i - 1].x - pt[i].x;
			int diffy = pt[i - 1].y - pt[i].y;

			ave_d += sqrt((float)(diffx * diffx + diffy * diffy));
		}

		//�ټ��ϴӵ�n-1����0�ľ���,�γɻ�·����
		ave_d += sqrt((float)((pt[0].x - pt[n - 1].x) *(pt[0].x - pt[n - 1].x) +(pt[0].y - pt[n - 1].y) * (pt[0].y - pt[n - 1].y)));

		//��ƽ�����ó�ƽ������
		ave_d *= invn;

		//����ÿ������������ض�ѭ���������
		for (i = 0; i < n; i++)
		{
			//��ʼ����������
			float maxEcont = 0;
			float maxEcurv = 0;
			float maxEimg = 0;

			float minEcont = _CV_SNAKE_BIG;
			float minEcurv = _CV_SNAKE_BIG;
			float minEimg = _CV_SNAKE_BIG;
			float Emin = _CV_SNAKE_BIG;

			//��ʼ�����κ��������ƫ����
			int offsetx = 0;
			int offsety = 0;

			float tmp;

			//�������������߽磬�Է�������������ROIͼ��ķ�Χ
			int left = MIN(pt[i].x, win.width >> 1);
			int right = MIN(roi.width - 1 - pt[i].x, win.width >> 1);
			int upper = MIN(pt[i].y, win.height >> 1);
			int bottom = MIN(roi.height - 1 - pt[i].y, win.height >> 1);

			//��ʼ��Econt
			maxEcont = 0;
			minEcont = _CV_SNAKE_BIG;

			//�ں����������Χ�ڽ���Econt�ļ���
			for (j = -upper; j <= bottom; j++)
			{
				for (k = -left; k <= right; k++)
				{
					int diffx, diffy;
					float energy;

					//�������㼯����β��Ӵ�����Ӧ����,����������
					if (i == 0)
					{
						diffx = pt[n - 1].x - (pt[i].x + k);
						diffy = pt[n - 1].y - (pt[i].y + j);
					}
					else
						//�������ط���һ�㴦��
					{
						diffx = pt[i - 1].x - (pt[i].x + k);
						diffy = pt[i - 1].y - (pt[i].y + j);
					}

					//�������������ת��Econt������±���ţ�����������ÿ���Econt
					//Econt��ֵ����ƽ������ʹ˵����һ��ľ���Ĳ�ľ���ֵ
					Econt[(j + centery) * win.width + k + centerx] = energy =(float)fabs(ave_d -sqrt((float)(diffx * diffx + diffy * diffy)));

				   //�������������е�Econt�����ֵ����Сֵ
					maxEcont = MAX(maxEcont, energy);
					minEcont = MIN(minEcont, energy);
				}
			}

			//�������������ֵ����Сֵ֮��,�������е�������Econt���б�׼��һ��,�����ֵ��С
			//��ȣ��������еĵ�Econtȫ��ȣ�Econt��һ������Ϊ0
			tmp = maxEcont - minEcont;
			tmp = (tmp == 0) ? 0 : (1 / tmp);

			for (k = 0; k < neighbors; k++)
			{
				Econt[k] = (Econt[k] - minEcont) * tmp;
			}

			//����ÿ���Ecurv
			maxEcurv = 0;
			minEcurv = _CV_SNAKE_BIG;

			for (j = -upper; j <= bottom; j++)
			{
				for (k = -left; k <= right; k++)
				{
					int tx, ty;
					float energy;

					//��һ����Ķ��ײ��
					if (i == 0)
					{
						tx = pt[n - 1].x - 2 * (pt[i].x + k) + pt[i + 1].x;
						ty = pt[n - 1].y - 2 * (pt[i].y + j) + pt[i + 1].y;
					}

					//���һ����Ķ��ײ��
					else if (i == n - 1)
					{
						tx = pt[i - 1].x - 2 * (pt[i].x + k) + pt[0].x;
						ty = pt[i - 1].y - 2 * (pt[i].y + j) + pt[0].y;
					}

					//�����Ķ��ײ��
					else
					{
						tx = pt[i - 1].x - 2 * (pt[i].x + k) + pt[i + 1].x;
						ty = pt[i - 1].y - 2 * (pt[i].y + j) + pt[i + 1].y;
					}

					//ת������Ϊ������ţ���������Ecurv��ֵ,���ײ�ֺ�ȡƽ��
					Ecurv[(j + centery) * win.width + k + centerx] = energy =(float)(tx * tx + ty * ty);

					//ȡ��С��Ecurv������Ecurv
					maxEcurv = MAX(maxEcurv, energy);
					minEcurv = MIN(minEcurv, energy);
				}
			}

			//��Ecurv���б�׼��һ��
			tmp = maxEcurv - minEcurv;
			tmp = (tmp == 0) ? 0 : (1 / tmp);

			for (k = 0; k < neighbors; k++)
			{
				Ecurv[k] = (Ecurv[k] - minEcurv) * tmp;
			}

			//��Eimg
			for (j = -upper; j <= bottom; j++)
			{
				for (k = -left; k <= right; k++)
				{
					float energy;

					//�����ûҶ��ݶ�����
					if (scheme == _CV_SNAKE_GRAD)
					{
						int x = (pt[i].x + k) / WTILE_SIZE;
						int y = (pt[i].y + j) / WTILE_SIZE;

						//���˴���ͼ��������û�л�ȡ����Դ˴���Ӧ��ͼ��ֿ����ͼ�����������
						if (map[y * map_width + x] == 0)
						{
							int l, m;
							//����Ҫ�����ݶ����Ӵ����ͼ����λ��
							int upshift = y ? 1 : 0;
							int leftshift = x ? 1 : 0;

							int bottomshift = MIN(1, roi.height - (y + 1)*WTILE_SIZE);
							int rightshift = MIN(1, roi.width - (x + 1)*WTILE_SIZE);

							//ͼ����λ�ô�С������ԭROI��һ����8�ı���������ͼ�����С��һ��
							Rect g_roi = { x*WTILE_SIZE - leftshift, y*WTILE_SIZE - upshift,leftshift + WTILE_SIZE + rightshift, upshift + WTILE_SIZE + bottomshift };

							Mat _src1;
							//cvGetSubArr(&_src, &_src1, g_roi);  //�õ�ͼ��������
							_src(g_roi).copyTo(_src1);

							//�ֱ��ͼ���X�����Y��������ݶ�����
							//�����ͼ�����ߴ�ΪTILE_SIZE+2�����Ӱ볤Ϊ3��{-3��-2��-1��0��1��2��3}
							//��������������Ϊ16λ������,�ֱ�����_dx,_dy������,����Ϊ10
							//pX.init_deriv(TILE_SIZE + 2, CV_8UC1, CV_16SC1, 1, 0, 3);
							//pY.init_deriv(TILE_SIZE + 2, CV_8UC1, CV_16SC1, 0, 1, 3);
							//pX.process(&_src1, &_dx);
							//pY.process(&_src1, &_dy);

							//����cvSepFilter��ɾ������Sobel���Ӵ���
							//X�����Ϻ�Y�����ϵ�Soble�ݶ����ӣ�������ͼ����ݶ�
							Sobel(_src1, _dx, _src1.depth(), 1, 0, 3);
							Sobel(_src1, _dy, _src1.depth(), 0, 1, 3);
							convertScaleAbs(_dx, _dx);
							convertScaleAbs(_dy, _dy);
							addWeighted(_dx, 0.5, _dy, 0.5, 0, _src1);

							//cvSepFilterҲ������sepFilter2Dȡ����Ч����GaussianBlur���
							//sepFilter2D(_src1, _dx, _src1.depth(), 1, 0);
							//sepFilter2D(_src1, _dy, _src1.depth(), 0, 1);

							//��ֿ������е�ÿ������ݶ�
							for (l = 0; l < WTILE_SIZE + bottomshift; l++)
							{
								for (m = 0; m < WTILE_SIZE + rightshift; m++)
								{
									gradient[(y*WTILE_SIZE + l) * roi.width + x*WTILE_SIZE + m] =
										(float)(dx[(l + upshift) * TILE_SIZE + m + leftshift] *
										dx[(l + upshift) * TILE_SIZE + m + leftshift] +
										dy[(l + upshift) * TILE_SIZE + m + leftshift] *
										dy[(l + upshift) * TILE_SIZE + m + leftshift]);
								}
							}

							//map��Ӧλ����1��ʾ�˴�ͼ�������Ѿ���ȡ
							map[y * map_width + x] = 1;
						}

						//���ݶ�������Ϊͼ������
						Eimg[(j + centery) * win.width + k + centerx] = energy =
							gradient[(pt[i].y + j) * roi.width + pt[i].x + k];
					}
					else
					{
						//�ԻҶ���Ϊͼ������
						Eimg[(j + centery) * win.width + k + centerx] = energy =
							src[(pt[i].y + j) * srcStep + pt[i].x + k];
					}

					//���������������С��ͼ������
					maxEimg = MAX(maxEimg, energy);
					minEimg = MIN(minEimg, energy);
				}
			}

			//Eimg�ı�׼��һ��
			tmp = (maxEimg - minEimg);
			tmp = (tmp == 0) ? 0 : (1 / tmp);

			for (k = 0; k < neighbors; k++)
			{
				Eimg[k] = (minEimg - Eimg[k]) * tmp;
			}

			//����ϵ��
			if (coeffUsage == CV_VALUE)
			{
				_alpha = *alpha;
				_beta = *beta;
				_gamma = *gamma;
			}
			else
			{
				_alpha = alpha[i];
				_beta = beta[i];
				_gamma = gamma[i];
			}

			//���ÿ��������Snake����
			for (k = 0; k < neighbors; k++)
			{
				E[k] = _alpha * Econt[k] + _beta * Ecurv[k] + _gamma * Eimg[k];
			}

			Emin = _CV_SNAKE_BIG;
			//��ȡ��С���������Լ���Ӧ�������е����λ��
			for (j = -upper; j <= bottom; j++)
			{
				for (k = -left; k <= right; k++)
				{
					if (E[(j + centery) * win.width + k + centerx] < Emin)
					{
						Emin = E[(j + centery) * win.width + k + centerx];
						offsetx = k;
						offsety = j;
					}
				}
			}

			//��������㷢���ı䣬��ǵ��ƶ�����
			if (offsetx || offsety)
			{
				pt[i].x += offsetx;
				pt[i].y += offsety;
				moved++;
			}
		}

		//�������������������ɺ����û���ƶ��ĵ��ˣ���������־λ��Ч��ֹͣ����
		converged = (moved == 0);

		//�ﵽ����������ʱ��������־λ��Ч��ֹͣ����
		if ((criteria.type & CV_TERMCRIT_ITER) && (iteration >= criteria.MAX_ITER))
			converged = 1;

		//������Ӧ����ʱ��ֹͣ���������һ����������ͬЧ����
		if ((criteria.type & CV_TERMCRIT_EPS) && (moved <= criteria.epsilon))
			converged = 1;
	}



	//�ͷŸ���������
	free(&Econt);
	free(&Ecurv);
	free(&Eimg);
	free(&E);

	if (scheme == _CV_SNAKE_GRAD)
	{
		free(&gradient);
		free(&map);
	}

	return CV_StsOk;

}

CV_IMPL void SnakeImage(
InputArray src, Point* points,
int length, float *alpha,
float *beta, float *gamma,
int coeffUsage, Size win,
CvTermCriteria criteria, int calcGradient)
{
	CV_FUNCNAME("SnakeImage");

	__CV_BEGIN__;

	uchar *data;
	CvSize size;

	int step;

	if (src.channels()!= 1)
		CV_ERROR(CV_BadNumChannels ,"input image has more than one channel");

	if (src.depth() != IPL_DEPTH_8U)
		CV_ERROR(CV_BadDepth, "Unsupported Format");

	const CvArr* src1 = (CvArr*)&src;
	cvGetRawData(src1, &data, &step, &size);

	CV_CALL(icvSnake8uC1R(data, step, size, points, length,
		alpha, beta, gamma, coeffUsage, win, criteria,
		calcGradient ? _CV_SNAKE_GRAD : _CV_SNAKE_IMAGE));

	__CV_END__;

}


//-------------------------����Ӧ�ó���--------------------------------------

//IplImage *image = 0; //ԭʼͼ��
//IplImage *image2 = 0; //ԭʼͼ��copy
using namespace cv;
using namespace std;

Mat srcImage;
Mat grayImage;

int g_nThreshold = 141;
int ialpha = 20;
int ibeta = 20;
int igamma = 20;

void on_Change(int,void*)
{

//	if (image2) cvReleaseImage(&image2);
//	if (image) cvReleaseImage(&image);

	srcImage = imread("pic1.jpg", CV_LOAD_IMAGE_ANYCOLOR|CV_LOAD_IMAGE_ANYDEPTH); 
	grayImage = imread("pic1.jpg", 0);

	threshold(grayImage, grayImage, g_nThreshold, 255, CV_THRESH_BINARY); //�ָ���ֵ   

//	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours2 = NULL;

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(grayImage, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);//Ѱ�ҳ�ʼ������

	if (contours.empty) return;

	int length = contours.size();

	if (length<10) return;

	contours2 = contours;
	Point* point = new Point[length]; //����������
	CvSeqReader reader;
	CvPoint pt = cvPoint(0, 0);

	cvStartReadSeq(contours2, &reader); 
	for (int i = 0; i < length; i++)
	{
		CV_READ_SEQ_ELEM(pt, reader);
		point[i] = pt;
		point[i] = contours.data[i];
	}

//	cvReleaseMemStorage(&storage);

	//��ʾ��������
	for (int i = 0; i<length; i++)
	{
		int j = (i + 1) % length;
		line(srcImage, point[i], point[j], CV_RGB(0, 0, 255), 1, 8, 0);
	}

	float alpha = ialpha / 100.0f;
	float beta = ibeta / 100.0f;
	float gamma = igamma / 100.0f;

	CvSize size;
	size.width = 3;
	size.height = 3;

	CvTermCriteria criteria;
	criteria.type = CV_TERMCRIT_ITER;
	criteria.max_iter = 1000;
	criteria.epsilon = 0.1;

	SnakeImage(grayImage, point, length, &alpha, &beta, &gamma, CV_VALUE, size, criteria, 0);

	//��ʾ����
	for (int i = 0; i<length; i++)
	{
		int j = (i + 1) % length;
		line(srcImage, point[i], point[j], CV_RGB(0, 255, 0), 1, 8, 0);
	}

	delete[]point;

}



int main(int argc, char* argv[])
{
	namedWindow("Snake�㷨����", WINDOW_KEEPRATIO);

	createTrackbar("Thd", "Snake�㷨����", &g_nThreshold, 255, on_Change);
	createTrackbar("alpha", "Snake�㷨����", &ialpha, 100, on_Change);
	createTrackbar("beta", "Snake�㷨����", &ibeta, 100, on_Change);
	createTrackbar("gamma", "Snake�㷨����", &igamma, 100, on_Change);

//	resizeWindow("Snake�㷨����", 300, 500);

	on_Change(g_nThreshold,0);

	imshow("Snake�㷨����", srcImage);
	
	while (char(waitKey(1)) != 'q'){}
	return 0;


}