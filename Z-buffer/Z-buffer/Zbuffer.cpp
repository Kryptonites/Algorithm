#include <afxwin.h>
#include <cstdio>
#include <iostream>

void CTestView::ZBuffer() // ����m_Paint����

{

	CRect Rect;

	GetClientRect(&Rect);

	CClientDC dc(this);

	dc.SetMapMode(MM_ANISOTROPIC);//�Զ�������ϵ

	dc.SetWindowExt(Rect.Width(), Rect.Height());

	dc.SetViewportExt(Rect.right, -Rect.bottom);//x�ᴹֱ���ң�y�ᴹֱ����

	dc.SetViewportOrg(Rect.right / 2, Rect.bottom / 2);//��Ļ����ΪԲ��  

	CDC MemDC;

	MemDC.CreateCompatibleDC(GetDC());

	MemDC.SetMapMode(MM_ANISOTROPIC);

	MemDC.SetWindowExt(Rect.Width(), Rect.Height());

	MemDC.SetViewportExt(Rect.right, -Rect.bottom);

	MemDC.SetViewportOrg(Rect.right / 2, Rect.bottom / 2);

	CBitmap Bitmap, *OldBitmap;

	Bitmap.LoadBitmap(IDB_BITMAP2);

	OldBitmap = MemDC.SelectObject(&Bitmap);

	MemDC.BitBlt(-Rect.right / 2, -Rect.bottom / 2, Rect.Width(), Rect.Height(), &Picture, -Rect.right / 2, -Rect.bottom / 2, SRCCOPY);

	m_Paint(&MemDC);

	dc.BitBlt(-Rect.right / 2, -Rect.bottom / 2, Rect.right, Rect.bottom, &MemDC, -Rect.right / 2, -Rect.bottom / 2, SRCCOPY);

	MemDC.SelectObject(OldBitmap);

}



void CTestView::OnMenuReset()//�˵�����

{

	// TODO: Add your command handler code here

	m_AngleX = m_AngleY = 0;

	Angle();

	RedrawWindow();

	MessageBox("ʹ�÷������ת�����壡", "��Ϣ", MB_ICONINFORMATION | MB_OK);

}

void CTestView::ReadPoint()//���붥�����,�˴�Ϊ�������8������m_point[8]

{

	//���ö���ε�8������

	m_OldBox.m_Point[0].x = -100; m_OldBox.m_Point[0].y = -100; m_OldBox.m_Point[0].z = +100;

	m_OldBox.m_Point[1].x = +100; m_OldBox.m_Point[1].y = -100; m_OldBox.m_Point[1].z = +100;

	m_OldBox.m_Point[2].x = +100; m_OldBox.m_Point[2].y = -100; m_OldBox.m_Point[2].z = -100;

	m_OldBox.m_Point[3].x = -100; m_OldBox.m_Point[3].y = -100; m_OldBox.m_Point[3].z = -100;

	m_OldBox.m_Point[4].x = -100; m_OldBox.m_Point[4].y = +100; m_OldBox.m_Point[4].z = +100;

	m_OldBox.m_Point[5].x = +100; m_OldBox.m_Point[5].y = +100; m_OldBox.m_Point[5].z = +100;

	m_OldBox.m_Point[6].x = +100; m_OldBox.m_Point[6].y = +100; m_OldBox.m_Point[6].z = -100;

	m_OldBox.m_Point[7].x = -100; m_OldBox.m_Point[7].y = +100; m_OldBox.m_Point[7].z = -100;

}



void CTestView::ReadFace()//���������,������ÿ������4���������ֵ



{

	F[0][0] = 4; F[0][1] = 0; F[0][2] = 1; F[0][3] = 2; F[0][4] = 3;

	F[1][0] = 4; F[1][1] = 0; F[1][2] = 3; F[1][3] = 7; F[1][4] = 4;

	F[2][0] = 4; F[2][1] = 0; F[2][2] = 1; F[2][3] = 5; F[2][4] = 4;

	F[3][0] = 4; F[3][1] = 1; F[3][2] = 2; F[3][3] = 6; F[3][4] = 5;

	F[4][0] = 4; F[4][1] = 4; F[4][2] = 5; F[4][3] = 6; F[4][4] = 7;

	F[5][0] = 4; F[5][1] = 3; F[5][2] = 2; F[5][3] = 6; F[5][4] = 7;

}

void CTestView::ReadColor()//������ɫ����, �����������������ɫ



{

	RGB[0] = RGB(255, 255, 0);

	RGB[1] = RGB(0, 255, 255);

	RGB[2] = RGB(0, 0, 255);

	RGB[3] = RGB(0, 255, 0);

	RGB[4] = RGB(255, 0, 0);

	RGB[5] = RGB(255, 0, 255);

}



bool CTestView::m_Paint(CDC* mdc)//��ɫ����,��һ��ѭ����������������,ÿ�ζ�����Ƚ��бȽ�,��ȴ��(�����ӵ����)�ᱣ����ǰ��.��һ��ƽ��ʱ����Ч�߱�����㷨(�õ���Ͱ����Ч�߱�).

{

	Sign(mdc);

	Angle();

	for (int i = 0; i <= 400; i++)

		for (int j = 0; j <= 400; j++)

			ZB[i][j] = -500;

	for (Face = 0; Face<6; Face++)

	{

		int TotalEdge = F[Face][0];

		for (int edge = 1; edge <= TotalEdge; edge++)//��ѭ��

		{

			int PointNumber = F[Face][edge];//��Ķ����;

			Point[edge - 1] = m_Box.m_Point[PointNumber];

		}

		CreatBucket();//����Ͱ���

		Et();//���ڽ����߱� 

		PolygonFill(mdc);//�������

	}

	return true;

}



void CTestView::Angle()//��ת�ǶȺ���,�˴����㽫�������8������ֱ���X��,Y����ת����µ������,����������ֵ,����<�����ͼ��ѧʵ���̳�VC++ �����>��ĸı�����ϵ���෴��һ������,����������ϵ����˶�:����ѵ���Y��ת��A��,���൱�ڰ�����ϵ��Y��ת��-A��.�˺�����,��X����ת,�൱��һ��Բ׶,x����, (y,z)��һ��Բ��ת��һ���Ƕ�,��ֱ������µ�(y��,z��).������TmpPointֵ���Կ���,��AngleX=0ʱ,�µ�����Ϊ(x,y,z),��ΪY������;��AngleX=90ʱ, �µ�����Ϊ(x,z,-y),��Ϊ-Z������(��ȡx=0,y=1,z=0��).��X������ۿ�,����ת�Ƕ�Ϊ��ʱ,����˳ʱ��ת��.���뿴�������沿��ʱ,�ɰ����ϼ�,��ʱ�Ƕȼ�10��,����ʱ����ת,�൱�ڰ���������ĵ�����,Ҳ�Ϳ���������ĵ�.��ǰ��ͼ��ʾ.



{//��Boxx������תm_AngleX��,y������תm_AngleY��

	for (int i = 0; i<8; i++)//Box��8������

	{

		MyPoint TmpPoint;

		TmpPoint.x = m_OldBox.m_Point[i].x;

		TmpPoint.y = ROUND(m_OldBox.m_Point[i].y*cos(m_AngleX*PI / 180) + m_OldBox.m_Point[i].z*sin(m_AngleX*PI / 180));

		TmpPoint.z = ROUND(-m_OldBox.m_Point[i].y*sin(m_AngleX*PI / 180) + m_OldBox.m_Point[i].z*cos(m_AngleX*PI / 180));

		m_Box.m_Point[i].x = ROUND(-TmpPoint.z*sin(m_AngleY*PI / 180) + TmpPoint.x*cos(m_AngleY*PI / 180));

		m_Box.m_Point[i].y = TmpPoint.y;

		m_Box.m_Point[i].z = TmpPoint.z*cos(m_AngleY*PI / 180) + TmpPoint.x*sin(m_AngleY*PI / 180);

	}

}



void CTestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)//���̿��ƺ���

{

	switch (nChar)

	{

	case VK_UP:

		m_AngleX += 10;

		break;

	case VK_DOWN:

		m_AngleX -= 10;

		break;

	case VK_LEFT:

		m_AngleY += 10;

		break;

	case VK_RIGHT:

		m_AngleY -= 10;

		break;

	default:

		break;

	}

	if (m_AngleX >= 360)//�ǶȲ��ܴ���360,Ҳ����С��0

	{

		m_AngleX -= 360;

	}

	else if (m_AngleX<0)

	{

		m_AngleX = 360 + m_AngleX;

	}

	if (m_AngleY >= 360)

	{

		m_AngleY -= 360;

	}

	else if (m_AngleY<0)

	{

		m_AngleY = 360 + m_AngleY;

	}

	ZBuffer();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);

}

void CTestView::Sign(CDC* mdc) //���ͼ��

{

	CPoint p1[4];

	CString c[7];

	c[0] = "���棺";

	c[1] = "���棺";

	c[2] = "ǰ�棺";

	c[3] = "���棺";

	c[4] = "���棺";

	c[5] = "���棺";

	c[6] = "�����о�����Ȩ����";

	mdc->SetBkColor(RGB(0, 0, 0));

	mdc->SetTextColor(RGB(255, 255, 255));

	mdc->TextOut(-500, 300, c[6]);

	CBrush MyBrush, *OldBrush;

	for (int i = 0; i<6; i++)

	{

		MyBrush.CreateSolidBrush(RGB[i]);

		OldBrush = mdc->SelectObject(&MyBrush);

		mdc->TextOut(-500, 280 - i * 20, c[i]);

		p1[0].x = -450; p1[0].y = 280 - i * 20;

		p1[1].x = -430; p1[1].y = 280 - i * 20;

		p1[2].x = -430; p1[2].y = 265 - i * 20;

		p1[3].x = -450; p1[3].y = 265 - i * 20;

		mdc->Polygon(p1, 4);

		mdc->SelectObject(OldBrush);

		MyBrush.DeleteObject();

	}

}



void CTestView::PolygonFill(CDC* mdc)//��������

{

	//����ֱ��deep����

	double  CurDeep = 0.0;//��ǰɨ���ߵ����

	double  DeepStep = 0.0;//��ǰɨ��������x��������Ȳ���

	double  A = 0.0;//ƽ�淽��ϵ��A

	double  B = 0.0;//ƽ�淽��ϵ��B

	double  C = 0.0;//ƽ�淽��ϵ��C

	double  D = 0.0;//ƽ�淽��ϵ��D   

	//���㵱ǰ�淽��Ax+By+Cz��D=0��ϵ��,(A,B,C)��Ϊƽ��ķ�����.��ƽ��������ΪM,N,P,������ΪAB*AC(���).

	A = (Point[1].y - Point[2].y)*(Point[1].z - Point[3].z) - (Point[1].y - Point[3].y)*(Point[1].z - Point[2].z);

	B = (Point[1].x - Point[3].x)*(Point[1].z - Point[2].z) - (Point[1].z - Point[3].z)*(Point[1].x - Point[2].x);

	C = (Point[1].x - Point[2].x)*(Point[1].y - Point[3].y) - (Point[1].x - Point[3].x)*(Point[1].y - Point[2].y);

	D = -A*Point[1].x - B*Point[1].y - C*Point[1].z;

	//����curDeep;��x=xMin��ʼ���㣬��ʱ���yi,�൱��yi����,����X,Zƽ�����ߵ�б��һ��,����ÿ�ε�X����1ʱ,ZҲ����Ӧ�ؼ��ϴ�б��ֵ,�������ܱ�֤����Ȼ�ڸ�ֱ��(����ά��ֱ��)��.

	DeepStep = -A / C;

	HeadE = NULL;

	for (CurrentB = HeadB; CurrentB != NULL; CurrentB = CurrentB->next)//��������Ͱ���

	{

		for (CurrentE = CurrentB->p; CurrentE != NULL; CurrentE = CurrentE->next)//����Ͱ������ǰ�ı߽��         

		{

			Edge *TEdge = new Edge;

			TEdge->x = CurrentE->x;

			TEdge->yMax = CurrentE->yMax;

			TEdge->k = CurrentE->k;

			TEdge->next = NULL;

			AddAet(TEdge);//���ñ߲�����ʱAet��

		}

		AetOrder();//ʹ��Aet����x������˳����    

		T1 = HeadE;//����ymax����ɨ����ı߽��

		if (T1 == NULL)

		{

			return;

		}

		while (CurrentB->ScanLine >= T1->yMax)//�����ý�㣬Aet��ָ�����

		{

			T1 = T1->next;

			HeadE = T1;

			if (HeadE == NULL)

			{

				return;

			}

		}

		if (T1->next != NULL)

		{

			T2 = T1;

			T1 = T2->next;

		}

		while (T1 != NULL)

		{

			if (CurrentB->ScanLine >= T1->yMax)//����һ�����

			{

				T2->next = T1->next;

				T1->next = NULL;

				T1 = T2->next;

			}

			else

			{

				T2 = T1;

				T1 = T2->next;

			}

		}

		BOOL In = false;//����һ��BOOL����In����ʼֵΪ��,�൱��ÿ�ζ�2����,�����һ���߶�.

		double xb, xe;//ɨ���ߵ������յ�

		for (T1 = HeadE; T1 != NULL; T1 = T1->next)//���ɨ���ߺͶ�����ཻ������

		{

			if (In == false)

			{

				xb = T1->x;

				CurDeep = -(xb*A + CurrentB->ScanLine*B + D) / C;//z=-(Ax+By+D)/C

				In = true;//ÿ����һ�����,��Inֵȡ��һ��

			}

			else//���InֵΪ�棬�����ӵ�ǰ����xֵ��ʼ����һ����xֵ����������

			{

				xe = T1->x;

				for (double x = xb; x <= xe; x++)

				{

					if (CurDeep >= ZB[ROUND(x) + 200][CurrentB->ScanLine + 200])//����µĲ��������ȴ���ԭ����������,��Ϊ����ÿ���涼����øú���,���Ժ����������и���ȸ���ĵ�,��Ḳ����ǰ�ĵ����ɫ.

					{

						ZB[ROUND(x) + 200][CurrentB->ScanLine + 200] = CurDeep;//xy�����������±걣��һ�£���200

						mdc->SetPixel(ROUND(x), CurrentB->ScanLine, RGB[Face]);

					}

					CurDeep += DeepStep;

				}

				In = false;

			}

		}

		for (T1 = HeadE; T1 != NULL; T1 = T1->next)//��������

		{

			T1->x = T1->x + T1->k;//x=x+1/k            

		}

	}

	delete HeadB;

	delete CurrentB;

	delete CurrentE;

	delete HeadE;

}

//*****************��Ч�߱��㷨��ʼ***********************

void CTestView::CreatBucket()//��ʼ��Ͱ���

{

	int ScanMin, ScanMax;//ȷ��ɨ���ߵ���Сֵ�����ֵ

	ScanMax = ScanMin = Point[0].y;

	for (int i = 1; i<Number; i++)

	{

		if (Point[i].y<ScanMin)

		{

			ScanMin = Point[i].y;//ɨ���ߵ���Сֵ         

		}

		if (Point[i].y>ScanMax)

		{

			ScanMax = Point[i].y;//ɨ���ߵ����ֵ

		}

	}

	for (i = ScanMin; i <= ScanMax; i++)//����Ͱ���

	{

		if (ScanMin == i)

		{

			HeadB = new Bucket;//����Ͱ��ͷ���

			CurrentB = HeadB;//CurrentBΪBucket��ǰ���ָ��

			CurrentB->ScanLine = ScanMin;

			CurrentB->p = NULL;//û�����ӱ�����

			CurrentB->next = NULL;

		}

		else//����Ͱ���������

		{

			CurrentB->next = new Bucket;//�½�һ��Ͱ���

			CurrentB = CurrentB->next;//ʹCurrentBָ���½���Ͱ���

			CurrentB->ScanLine = i;

			CurrentB->p = NULL;//û�����ӱ�����

			CurrentB->next = NULL;

		}

	}

}



void CTestView::Et()//����߱�

{

	for (int i = 0; i<Number; i++)//����ÿ������

	{

		CurrentB = HeadB;//��Ͱ�����ͷ��㿪ʼѭ��

		int j = i + 1;//�ߵĵڶ������㣬Point[i]��Point[j]���ɱ�

		if (j == Number) j = 0;//��֤����εıպ�

		if (Point[j].y>Point[i].y)//����ɨ���ߵ��Ϸ�

		{

			while (CurrentB->ScanLine != Point[i].y)//��Ͱ��Ѱ�Ҹñߵ�yMin

			{

				CurrentB = CurrentB->next;//��Ͱ��Ѱ�Ҹñߵ�yMin

			}

			E[i].x = Point[i].x;//����Aet���ֵ

			E[i].yMax = Point[j].y;

			E[i].k = double((Point[j].x - Point[i].x)) / (Point[j].y - Point[i].y);//����1/k       

			E[i].next = NULL;

			CurrentE = CurrentB->p;//���Ͱ�����ӱ߱�ĵ�ַ

			if (CurrentE == NULL)//��ǰͰ�����û�����ӱ߽��

			{

				CurrentE = &E[i];//���ߵ���ʼ��ַ

				CurrentB->p = CurrentE;//��һ���߽��ֱ�����ӵ���Ӧ��Ͱ��

			}

			else

			{

				while (CurrentE->next != NULL)//�����ǰ�������б߽��

				{

					CurrentE = CurrentE->next;//�ƶ�ָ�뵽��ǰ�ߵ����һ���߽��               

				}

				CurrentE->next = &E[i];//�ѵ�ǰ�߽���ȥ

			}

		}

		if (Point[j].y<Point[i].y)//����ɨ���ߵ��·�

		{

			while (CurrentB->ScanLine != Point[j].y)

			{

				CurrentB = CurrentB->next;

			}

			E[i].x = Point[j].x;

			E[i].yMax = Point[i].y;

			E[i].k = double((Point[i].x - Point[j].x)) / (Point[i].y - Point[j].y);

			E[i].next = NULL;

			CurrentE = CurrentB->p;

			if (CurrentE == NULL)

			{

				CurrentE = &E[i];

				CurrentB->p = CurrentE;

			}

			else

			{

				while (CurrentE->next != NULL)

				{

					CurrentE = CurrentE->next;

				}

				CurrentE->next = &E[i];

			}

		}

	}

	CurrentB = NULL;

	CurrentE = NULL;

}



void CTestView::AddAet(Edge *NewEdge)//�߲���Aet��

{

	T1 = HeadE;

	if (T1 == NULL)//Aet��Ϊ��,��Aet����ΪTEdge

	{

		T1 = NewEdge;

		HeadE = T1;

	}

	else

	{

		while (T1->next != NULL)//Aet��Ϊ��,��TEdge���ڸñ�֮��

		{

			T1 = T1->next;

		}

		T1->next = NewEdge;

	}

}



void CTestView::AetOrder()//��Aet���������

{

	T1 = HeadE;

	if (T1 == NULL)

	{

		return;

	}

	if (T1->next == NULL)//�����Aet��û������Aet��

	{

		return;//Ͱ���ֻ��һ���ߣ�����Ҫ����

	}

	else

	{

		if (T1->next->x<T1->x)//Aet��xֵ����

		{

			T2 = T1->next;

			T1->next = T2->next;

			T2->next = T1;

			HeadE = T2;

		}

		T2 = HeadE;

		T1 = HeadE->next;

		while (T1->next != NULL)//���������Ƚ�������Aet���xֵ,��������

		{

			if (T1->next->x<T1->x)

			{

				T2->next = T1->next;

				T1->next = T1->next->next;

				T2->next->next = T1;

				T2 = T2->next;

			}

			else

			{

				T2 = T1;

				T1 = T1->next;

			}

		}

	}

}