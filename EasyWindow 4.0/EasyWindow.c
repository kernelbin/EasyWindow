#include"EasyWindow.h"

//*******************************************************************************************
//                ����ʵ��
//*******************************************************************************************

//��ʼ������
BOOL InitEZWindow()
{
	WNDCLASS wndclass;
	//ע�ᴰ����
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = EZParentWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = sizeof(LONG_PTR);
	wndclass.hInstance = GetModuleHandle(NULL);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = EZWindowClass;
	if (!RegisterClass(&wndclass))
	{
		return  FALSE;
	}


	return TRUE;
}



//�������ں���
EZWND CreateEZParentWindowEx(DWORD EZStyle, int x, int y, int Width, int Height, DWORD WinStyle, EZWNDPROC ezWndProc, HMENU hMenu, HWND hParent)
{
	HWND hwndParent;//����Windows����ĸ�����
	EZWND ezwndParent;//���Ǻ�Win���ڵȴ��EZ���ڣ����ظ��û����ô��ڴݻ�ʱͬʱ�ݻ�Win����


	if (!(ezwndParent = (EZWND)malloc(sizeof(EZWINDOW))))//�����ڴ沢���
	{
		return (EZWND)0;
	}

	ZeroMemory(ezwndParent, sizeof(EZWINDOW));

	if (!(ezwndParent->TopWndExtend = (pTWE)malloc(sizeof(TopWndExt))))
	{
		free(ezwndParent);
		return (EZWND)0;
	}

	ZeroMemory(ezwndParent->TopWndExtend, sizeof(TopWndExt));

	ezwndParent->EZStyle = EZStyle;

	if (EZStyle != 0L)
	{
		ezwndParent->Extend = (pEZSE)malloc(sizeof(EZSE));
		ZeroMemory(ezwndParent->Extend, sizeof(EZSE));
	}

	//�ҽ��������и�ֵΪ0�����ȫ��ע�͵��ˣ���Ϊ���Ƕ���Ĳ���


	ezwndParent->x = x;
	ezwndParent->y = y;
	ezwndParent->Width = Width;
	ezwndParent->Height = Height;


	//ezwndParent->px = 0;
	//ezwndParent->py = 0;

	//ezwndParent->ScrollX = 0;
	//ezwndParent->ScrollY = 0;


	//ezwndParent->FirstChild = NULL;
	//ezwndParent->LastEZWnd = NULL;
	//ezwndParent->NextEZWnd = NULL;
	//ezwndParent->ezParent = NULL;//û�У��ա�
	ezwndParent->ezRootParent = ezwndParent;//���Լ�

	//ezwndParent->TopWndExtend->CptMouseWindow = NULL;//û�еģ�2333
	//ezwndParent->TopWndExtend->FocusWindow = NULL;
	//ezwndParent->TopWndExtend->MouseOnWnd = NULL;
	//ezwndParent->TopWndExtend->CptKbdWindow = NULL;


	ezwndParent->IsTopWindow = TRUE;//�ǵģ��Ƕ��㴰�ڡ�

	//ezwndParent->FocusState = FALSE;

	ezwndParent->MouseMsgRecv = 1;

	ezwndParent->ShowState = 1;

	//ezwndParent->MouseOn = FALSE;

	//ezwndParent->Update = FALSE;//�տ�ʼ���϶�û�и��¹���

	ezwndParent->Transparent = 255;//��͸��

	ezwndParent->ezWndProc = ezWndProc;

	//ezwndParent->TopWndExtend->FocusWindow = NULL;


	//ezwndParent->IsWinWnd = 0;
	//ezwndParent->hChild = 0;


	//ezwndParent->IsStyleWindow = FALSE;

	//ezwndParent->ezID = 0;

	//�󲿷����ݶ���ʼ������ˡ�ezwndParent����Ϊ�����ύ��WM_CREATE��Ϣ�������ڼ佫���ָ������Ϊ������չ
	hwndParent = CreateWindow(EZWindowClass, TEXT(""), WinStyle, x, y, Width, Height, hParent, hMenu, GetModuleHandle(NULL), ezwndParent);

	//	EZRepaint(ezwndParent, NULL);

	return ezwndParent;
}

EZWND CreateEZWindowEx(EZWND ezParent, DWORD EZStyle, int x, int y, int Width, int Height, EZWNDPROC ezWndProc)
{
	HWND hParent;
	EZWND ezWnd;
	HDC fhdc;
	if (!IsEZWindow(ezParent))
	{
		return (EZWND)0;
	}

	if (!(ezWnd = (EZWND)malloc(sizeof(EZWINDOW))))//����������ڴ�
	{
		return (EZWND)0;
	}
	ZeroMemory(ezWnd, sizeof(EZWINDOW));
	ezWnd->EZStyle = EZStyle;

	if (EZStyle != 0L)
	{
		ezWnd->Extend = (pEZSE)malloc(sizeof(EZSE));
		ZeroMemory(ezWnd->Extend, sizeof(EZSE));
	}

	//�ҽ��������и�ֵΪ0�����ȫ��ע�͵��ˣ���Ϊ���Ƕ���Ĳ���

	//ezWnd->TopWndExtend = NULL;

	ezWnd->x = x;
	ezWnd->y = y;
	ezWnd->Width = Width;
	ezWnd->Height = Height;

	ezWnd->px = ezParent->px + x + ezParent->ScrollX;
	ezWnd->py = ezParent->py + y + ezParent->ScrollY;

	//ezWnd->ScrollX = 0;
	//ezWnd->ScrollY = 0;

	//ezWnd->FirstChild = NULL;
	//ezWnd->LastEZWnd = NULL;
	//ezWnd->NextEZWnd = NULL;

	ezWnd->hParent = ezParent->hParent;
	//ezWnd->IsTopWindow = FALSE;//������


	//ezWnd->FocusState = 0;
	ezWnd->MouseMsgRecv = 1;
	ezWnd->ShowState = 1;

	//ezWnd->MouseOn = FALSE;

	ezWnd->Update = 1;//�տ�ʼ���϶�û�и��¹���

	ezWnd->Transparent = 255;//��͸��

	ezWnd->ezParent = ezParent;
	ezWnd->ezRootParent = ezParent->ezRootParent;
	ezWnd->ezWndProc = ezWndProc;

	//��EZ���������׷������µ��Ӵ��ڡ�
	EZAddChild(ezParent, ezWnd);

	fhdc = GetDC(hParent = ezParent->hParent);
	ezWnd->hdc = GetMemDC(fhdc, Width, Height);
	ReleaseDC(hParent, fhdc);


	//ezWnd->IsWinWnd = 0;
	//ezWnd->hChild = 0;


	//ezWnd->IsStyleWindow = FALSE;


	//ezWnd->ezID = 0;

	EZSendMessage(ezWnd, EZWM_CREATE, 0, 0);//���ʹ�����Ϣ

											//ֻ����һ���ƶ���Ϣ
	EZSendMessage(ezWnd, EZWM_SIZE, (WPARAM)NULL, (LPARAM)MAKELPARAM(Width, Height));
	return ezWnd;

}

BOOL EZAddChild(EZWND ezParent, EZWND ezChild)
{
	EZWND ezChildLast;
	//���Ӵ���׷����ezParent���Ӵ����������
	if (!IsEZWindow(ezParent->FirstChild))
	{
		ezParent->FirstChild = ezChild;
		return TRUE;
	}

	//�Ӵ��������һ����Ϊ��
	ezChildLast = ezParent->FirstChild;

	while (1)//ѭ�����������
	{
		if (!IsEZWindow(ezChildLast->NextEZWnd))
		{
			ezChild->LastEZWnd = ezChildLast;
			ezChildLast->NextEZWnd = ezChild;
			return TRUE;
		}
		ezChildLast = ezChildLast->NextEZWnd;//�����ƶ�
	}

}


EZWND CreateEZStyleWindow(EZWND ezParent, TCHAR Title[], DWORD EZStyle, int x, int y, int Width, int Height)
{
	EZWND ezWnd;
	ezWnd = 0;
	//�������ԣ�����Ƿ��ͻ��Ȼ�󴴽����ڡ�
	if (CHK_ALT_STYLE(EZStyle, EZS_CHILD))
	{
		//�Ӵ���
		switch (EZStyle & MKDW(00000000, 00000000, 00000000, 11111111))
		{
		case EZS_STATIC:
			ezWnd = CreateEZWindowEx(ezParent, EZStyle, x, y, Width, Height, EZStyle_StaticProc);

			break;
		case EZS_BUTTON:
			ezWnd = CreateEZWindowEx(ezParent, EZStyle, x, y, Width, Height, EZStyle_ButtonProc);
			break;
		case EZS_CHILD_HSCROLL:
		case EZS_CHILD_VSCROLL:
			ezWnd = CreateEZWindowEx(ezParent, EZStyle, x, y, Width, Height, EZStyle_ScrollChildProc);
			break;
		case EZS_EDIT:
			ezWnd = CreateEZWindowEx(ezParent, EZStyle, x, y, Width, Height, EZStyle_EditProc);
			break;
		}

	}
	else
	{
		//������

	}


	((pEZSE)ezWnd->Extend)->hFont = NULL;
	ezWnd->Extend->Title = 0;
	EZSendMessage(ezWnd, EZWM_SETTEXT, Title, lstrlen(Title));


	((pEZSE)ezWnd->Extend)->BackGroundColor = RGB(255, 255, 255);

	((pEZSE)ezWnd->Extend)->ForeGroundColor = RGB(0, 0, 0);

	((pEZSE)ezWnd->Extend)->MouseHold = FALSE;


	((pEZSE)ezWnd->Extend)->IsFontUserControl = -1;


	//���������һ�飬��ֹ֮ǰ����Ұ���ݻ������㡣
	ezWnd->IsStyleWindow = TRUE;
	ezWnd->EZStyle = EZStyle;

	return ezWnd;
}


EZWND CreateEZStyleParentWindow(TCHAR Title[], DWORD EZStyle, int x, int y, int Width, int Height, BOOL bAdjust, EZWNDPROC ezWndProc)
{
	EZWND ezParent = 0, ezWnd = 0;
	//�������ԣ�����Ƿ��ͻ��Ȼ�󴴽����ڡ�
	if (CHK_ALT_STYLE(EZStyle, EZS_CHILD))
	{

	}
	else
	{
		//������
		//�Ӵ���
		DWORD WinStyle = WS_VISIBLE | WS_DLGFRAME | WS_POPUP | WS_THICKFRAME;
		switch (EZStyle & MKDW(00000000, 00000000, 00000000, 11111111))
		{


		case EZS_OVERLAPPEDWINDOW:
			WinStyle |= WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
		case EZS_OVERLAPPED:


			ezParent = CreateEZParentWindowEx(EZStyle, x, y, Width, bAdjust ? Height + EZWND_CAP_HEIGHT : Height,
				WinStyle, EZStyle_OverlappedWndProc, 0, 0);
			ezParent->Extend->hExtend[0] = ezWnd = CreateEZWindow(ezParent, 0, EZWND_CAP_HEIGHT, Width, bAdjust ? Height : Height - EZWND_CAP_HEIGHT, ezWndProc);

			break;
		}

	}


	((pEZSE)ezParent->Extend)->hFont = NULL;
	ezParent->Extend->Title = 0;
	EZSendMessage(ezParent, EZWM_SETTEXT, Title, lstrlen(Title));


	((pEZSE)ezParent->Extend)->BackGroundColor = RGB(255, 255, 255);

	((pEZSE)ezParent->Extend)->ForeGroundColor = RGB(0, 0, 0);

	((pEZSE)ezParent->Extend)->MouseHold = FALSE;


	((pEZSE)ezParent->Extend)->IsFontUserControl = -1;


	//���������һ�飬��ֹ֮ǰ����Ұ���ݻ������㡣
	ezParent->IsStyleWindow = TRUE;
	ezParent->EZStyle = EZStyle;

	EZRepaint(ezParent, 0);

	return ezWnd;
}

//���ٴ��ں���
BOOL DestroyEZWindow(EZWND ezWnd)
{

	if (ezWnd->IsTopWindow)
	{
		DestroyWindow(ezWnd->hParent);//������ˣ����ٻ���������ɡ���Ҳûʲô��˵�ģ����ù�������������ˡ�
		return TRUE;
	}



	//�ȱ�����Ϣ�����ƣ������������ﴰ��
	if (IsEZWindow(ezWnd->FirstChild))
	{
		DestroyEZWindowWithNext(ezWnd->FirstChild);
	}


	//�����Լ�������
	EZSendMessage(ezWnd, EZWM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);//����������Ϣ

	DeleteMemDC(ezWnd->hdc);//����DC
	//DeleteMemDC(ezWnd->hdcCopy);//����DC

	if (ezWnd->ezRootParent->TopWndExtend->MouseOnWnd == ezWnd)
	{
		ezWnd->ezRootParent->TopWndExtend->MouseOnWnd = NULL;
	}
	if (ezWnd->ezRootParent->TopWndExtend->FocusWindow == ezWnd)
	{
		ezWnd->ezRootParent->TopWndExtend->FocusWindow = NULL;
	}
	if (ezWnd->ezRootParent->TopWndExtend->CptMouseWindow == ezWnd)
	{
		ezWnd->ezRootParent->TopWndExtend->CptMouseWindow = NULL;
	}
	if (ezWnd->ezRootParent->TopWndExtend->CptKbdWindow == ezWnd)
	{
		ezWnd->ezRootParent->TopWndExtend->CptKbdWindow = NULL;
	}


	if (ezWnd->IsTopWindow)
	{
		DeleteMemDC(ezWnd->TopWndExtend->hdcTop);
		free(ezWnd->TopWndExtend);
	}


	if (ezWnd->IsStyleWindow)
	{
		free(ezWnd->Extend);
	}
	//ά��һ������
	if (ezWnd->LastEZWnd)
	{
		//����һ��
		if (ezWnd->NextEZWnd)
		{
			//������һ����
			ezWnd->LastEZWnd->NextEZWnd = ezWnd->NextEZWnd;
			ezWnd->NextEZWnd->LastEZWnd = ezWnd->LastEZWnd;
			//���Ӻÿ�
		}
		else
		{
			//��һ���ǿյ�
			ezWnd->LastEZWnd->NextEZWnd = NULL;
		}
	}
	else
	{
		//û����һ�������Ƕ��㴰�ڣ����ǵ�һ����
		if (!(ezWnd->IsTopWindow))
		{
			//���Ƕ��㰡������ĳ�����ĵ�һ��
			if (ezWnd->NextEZWnd)
			{
				//������һ��
				ezWnd->ezParent->FirstChild = ezWnd->NextEZWnd;
				ezWnd->NextEZWnd->LastEZWnd = NULL;
			}
			else
			{
				//��û����һ���ˣ��㸸����û�����ˡ�
				ezWnd->ezParent->FirstChild = NULL;
			}
		}
	}


	if ((ezWnd->ezParent->EZStyle != 0) && (!(ezWnd->ezParent->EZStyle & EZS_CHILD)) && ezWnd->ezParent->IsTopWindow)
	{
		//����������ʽ������,��ɾ��
		if (ezWnd->ezParent->FirstChild)
		{

			ezWnd->ezParent->FirstChild = 0;//�ø���������Ϊ�Լ�û���Ӵ��ڣ��Ͳ���ݹ�ɾ��������ѭ����
			DestroyWindow(ezWnd->ezParent->hParent);
		}
	}

	free(ezWnd);
	ezWnd = NULL;
	return TRUE;

}

BOOL DestroyEZWindowWithNext(EZWND ezWnd)
{
	EZWND ezChildLast;

	//�ع���׼�������д���
	if (IsEZWindow(ezWnd->NextEZWnd))
	{
		ezChildLast = ezWnd->NextEZWnd;

		//ѭ�����������
		while (1)
		{
			if (!IsEZWindow(ezChildLast->NextEZWnd))
			{
				break;//���������
			}
			ezChildLast = ezChildLast->NextEZWnd;
		}


		//��ѭ������������ǰ��һ�߹�һ��ɾ��

		//Ϊ�����ݣ��Ȼع���Ȼ��ͨ�����µ�ָ���һ�ȥ��ɾ��
		while (IsEZWindow(ezChildLast->LastEZWnd))//��û��ͷ
		{

			ezChildLast = ezChildLast->LastEZWnd;//�ع�

												 //��ʱ��ezChildLast->NextEZWnd�������һ����DestroyEZWindow��ɾ�˸ô��ں�������ڵ��Ӵ��ڡ�
			DestroyEZWindow(ezChildLast->NextEZWnd);//ɾ��
		}
		//������ͷ�ˣ�DestroyEZWindow��ɾ�˸ô��ں�������ڵ��Ӵ��ڡ�
		DestroyEZWindow(ezChildLast);
		return TRUE;

	}
	else
	{//��������һ�����д��ڡ�
		DestroyEZWindow(ezWnd);//ɾ��
	}
	return TRUE;

}



//�������ú���
BOOL MoveEZWindow(EZWND ezWnd, int x, int y, int Width, int Height, BOOL repaint)
{
	HDC fhdc;//��ʱDC


	if (ezWnd->IsTopWindow)
	{
		MoveWindow(ezWnd->hParent, x, y, Width, Height, FALSE);
		//ʣ�µĹ����������ɵ�
		//TODO:������й���������ɣ���ϸ���

		return TRUE;
	}
	else
	{
		ezWnd->x = x;
		ezWnd->y = y;
		ezWnd->Width = Width;
		ezWnd->Height = Height;
		ezWnd->px = ezWnd->ezParent->px + x + ezWnd->ezParent->ScrollX;
		ezWnd->py = ezWnd->ezParent->py + y + ezWnd->ezParent->ScrollY;
		//ö�������Ӵ��ڲ�����px��py
		EZResetChildPxPy(ezWnd);
	}



	//��ʱ����DC��Ȼ�����»��һ����
	/*DeleteMemDC(ezWnd->hdc);
	DeleteMemDC(ezWnd->hdcCopy);


	ezWnd->hdc = GetMemDC(fhdc, Width, Height);
	ezWnd->hdcCopy = GetMemDC(fhdc, Width, Height);

	if (ezWnd->IsTopWindow)
	{
	DeleteMemDC(ezWnd->TopWndExtend->hdcTop);
	ezWnd->TopWndExtend->hdcTop = GetMemDC(fhdc, Width, Height);
	}

	ReleaseDC(ezWnd->hParent, fhdc);*/
	fhdc = GetDC(ezWnd->hParent);
	AdjustMemDC(ezWnd->hdc, fhdc, Width, Height);
	// AdjustMemDC(ezWnd->hdcCopy, fhdc, Width, Height);

	ReleaseDC(ezWnd->hParent, fhdc);
	//ReleaseDC(ezWnd->hParent, fhdc);

	EZSendMessage(ezWnd, EZWM_SIZE, (WPARAM)NULL, (LPARAM)MAKELPARAM(Width, Height));

	if (repaint)
	{
		if (ezWnd->ezParent)
		{
			EZRepaint(ezWnd->ezParent, NULL);
		}
		else
		{
			EZRepaint(ezWnd, NULL);
		}
	}

	return TRUE;
}

BOOL ScrollEZWindow(EZWND ezWnd, int x, int y, BOOL bAdd)//bAddΪTRUE�����ۼӡ���������
{
	if (bAdd)
	{
		ezWnd->ScrollX += x;
		ezWnd->ScrollY += y;
	}
	else
	{
		ezWnd->ScrollX = x;
		ezWnd->ScrollY = y;
	}
	EZResetChildPxPy(ezWnd);
	EZRepaint(ezWnd, 0);
	return 0;
}
BOOL EZResetChildPxPy(EZWND ezWnd)
{
	if (IsEZWindow(ezWnd->FirstChild))
	{
		for (EZWND EZChild = ezWnd->FirstChild; EZChild; EZChild = EZChild->NextEZWnd)
		{
			EZChild->px = ezWnd->px + EZChild->x + ezWnd->ScrollX;
			EZChild->py = ezWnd->py + EZChild->y + ezWnd->ScrollY;
			EZResetChildPxPy(EZChild);
		}
	}
	return 0;
}

BOOL SetEZWndTransparent(EZWND ezWnd, int Transparent)//����EZ����͸����
{
	ezWnd->Transparent = Transparent;
	return TRUE;
}

BOOL EZCaptureMouse(EZWND ezWnd)
{
	//�򶥲㸸�����ύҪ�������󡱵Ĵ��ڵľ����
	if (ezWnd->ezRootParent->TopWndExtend->CptMouseWindow)
	{
		EZSendMessage(ezWnd->ezRootParent->TopWndExtend->CptMouseWindow, EZWM_RELEASEMOUSE, 0, 0);

	}
	ezWnd->ezRootParent->TopWndExtend->CptMouseWindow = ezWnd;
	EZSendMessage(ezWnd->ezRootParent->TopWndExtend->CptMouseWindow, EZWM_CAPTUREMOUSE, 0, 0);
	SetCapture(ezWnd->hParent);
	return TRUE;
}

BOOL EZReleaseMouse(EZWND ezWnd)
{
	//�򶥲㸸�����ύ�ͷš��������롣

	ReleaseCapture();

	//��������䲻��Ҫ����Window���ڴ���WM_CAPTURECHANGE֮���Ѿ��������
	//EZSendMessage(ezWnd->ezRootParent->CptMouseWindow, EZWM_RELEASEMOUSE, 0, 0);
	//ezWnd->ezRootParent->CptMouseWindow = NULL;

	return TRUE;
}


BOOL EZCaptureKeyboard(EZWND ezWnd)
{
	ezWnd->ezRootParent->TopWndExtend->CptKbdWindow = ezWnd;
	return 0;
}

BOOL EZReleaseKeyboard(EZWND ezWnd)
{
	ezWnd->ezRootParent->TopWndExtend->CptKbdWindow = NULL;
	return 0;
}



BOOL SetMouseMsgRecvMode(EZWND ezWnd, int Mode)
{
	ezWnd->MouseMsgRecv = Mode;
	return TRUE;
}

BOOL SetShowState(EZWND ezWnd, int State)
{
	ezWnd->ShowState = State;
	return TRUE;
}





//�봰�ڻ����йصĺ���
BOOL EZRedraw(EZWND ezWnd)//�ػ浽�ڴ�dc,�����¡�
{
	//���������Ҫ��ȷ����Ҫ�ػ�ķ�Χ

	/*
	�������󴰿���ϵ��
	�κκ͸ô����з�Χ�ظ��Ĵ���
	ȫ�������裡Ҫ���ػ棡������

	���ԣ�����Ҫ�����д��ڣ������Ӵ��ڹ㲥��Ϣ���κ����ظ��Ĵ��ڣ�ȫ����Ҫ�ػ浽�ڴ�
	Ȼ����е��Ӵ���

	����Ҫ�ػ浽Windows�����ڡ�

	*/

	if (!IsEZWindow(ezWnd))
	{
		return FALSE;
	}

	for (EZWND WndNow = ezWnd; WndNow; WndNow = WndNow->ezParent)//�����ƣ�ֱ��������
	{
		if (WndNow->ShowState == 2)return 0;
	}

	RECT rect = { ezWnd->px,ezWnd->py  ,ezWnd->px + ezWnd->Width,ezWnd->py + ezWnd->Height };

	BroadcastProc(ezWnd, SEZWM_REDRAW, (WPARAM)NULL, (LPARAM)NULL);
	RedrawBroadcast(ezWnd, 0, 0, ezWnd->px, ezWnd->py, rect);


	BitBlt(ezWnd->ezRootParent->TopWndExtend->hdcTop,
		ezWnd->px,
		ezWnd->py,
		ezWnd->Width, ezWnd->Height,
		ezWnd->hdc,
		0,
		0, SRCCOPY);

	return TRUE;
}

BOOL RedrawBroadcast(EZWND ezWnd, WPARAM wp, LPARAM lp, int cx, int cy, RECT RectSrc)
{

	if (!IsEZWindow(ezWnd))
	{
		return FALSE;
	}

	if (IsEZWindow(ezWnd->FirstChild))
	{

		EZWND LastChild;



		for (LastChild = ezWnd->FirstChild; LastChild; LastChild = LastChild->NextEZWnd)
		{
			//�������ԣ��ȴ����Լ���
			RECT RectDst, RectAns;

			{
				RectDst.left = cx + LastChild->x + LastChild->ezParent->ScrollX;
				RectDst.right = RectDst.left + LastChild->Width;
				RectDst.top = cy + LastChild->y + LastChild->ezParent->ScrollY;
				RectDst.bottom = RectDst.top + LastChild->Height;
			}

			if (!((IntersectRect(&RectAns, &RectSrc, &RectDst) == 0) || (LastChild->ShowState == 2)))
				//��ʱ����
				//if (LastChild->ShowState == 1)
			{
				/*HRGN hRgn, OldRgn;*/

				BroadcastProc(LastChild, SEZWM_REDRAW, wp, lp);//�����Լ�



															   /*hRgn = CreateRectRgn(
															   RectAns.left - (cx + LastChild->x + LastChild->ezParent->ScrollX),
															   RectAns.top - (cy + LastChild->y + LastChild->ezParent->ScrollY),
															   RectAns.right - (cx + LastChild->x + LastChild->ezParent->ScrollX),
															   RectAns.bottom - (cy + LastChild->y + LastChild->ezParent->ScrollY));*/

															   /*OldRgn = SelectObject(LastChild->hdc, hRgn);*/



				RedrawBroadcast(LastChild, wp, lp, cx + LastChild->x + LastChild->ezParent->ScrollX, cy + LastChild->y + LastChild->ezParent->ScrollY, RectAns);//���Լ����Ӵ��ڷ��͸���Ϣ																															 /*	DeleteObject(SelectObject(LastChild->hdc, OldRgn));*/


				BroadcastProc(LastChild, SEZWM_COPYDC, wp, lp);//�����Լ�

			}


			//���¹�

		}

	}
	return TRUE;
}

BOOL EZUpdate(EZWND ezWnd, HDC hdc)//��DC���µ����壬���ػ档�ڶ���������DC���粻�ṩ���������Զ���ȡ��
{
	if (!IsEZWindow(ezWnd))
	{
		return FALSE;
	}

	//ֻ��Ҫ��������ڣ�����Ҫȫ��BitBlt��,�õ���Ը����ڵ�λ��

	////EZBroadcastToAllChild(ezWnd, TRUE, SEZWM_COPYDC, (WPARAM)NULL, (LPARAM)NULL);,


	if (!hdc)//ûDC
	{

		/*if (ezWnd->ezRootParent->DrawOnNC)
		{
		hdc = GetWindowDC(ezWnd->hParent);
		}
		else
		{*/
		hdc = GetDC(ezWnd->hParent);
		/*	}*/

		//BitBlt(hdcg, Countx, County, ezWnd->Width, ezWnd->Height, ezWnd->ezRootParent->hdc, Countx, County, SRCCOPY);
		BitBlt(hdc, ezWnd->px, ezWnd->py, ezWnd->Width, ezWnd->Height, ezWnd->hdc, 0, 0, SRCCOPY);
		ReleaseDC(ezWnd->hParent, hdc);
		hdc = NULL;
	}
	else
	{
		//BitBlt(hdc, Countx, County, ezWnd->Width, ezWnd->Height, ezWnd->ezRootParent->hdc, Countx, County, SRCCOPY);
		BitBlt(hdc, ezWnd->px, ezWnd->py, ezWnd->Width, ezWnd->Height, ezWnd->hdc, 0, 0, SRCCOPY);
	}

	BitBlt(ezWnd->ezRootParent->TopWndExtend->hdcTop,
		ezWnd->px - ezWnd->ScrollX,
		ezWnd->py - ezWnd->ScrollY,
		ezWnd->Width, ezWnd->Height,
		ezWnd->hdc,
		0,
		0, SRCCOPY);

	/*
	RECT rect;
	rect.left = Countx;
	rect.right = Countx + ezWnd->Width;
	rect.top = County;
	rect.bottom = County + ezWnd->Height;

	InvalidateRect(ezWnd->hParent, &rect, 0);
	*/
	return 0;
}

BOOL EZRepaint(EZWND ezWnd, HDC hdc)
{
	if (!ezWnd)
	{
		return FALSE;
	}
	if (ezWnd->ShowState == 2)
	{
		return TRUE;
	}



	for (EZWND WndNow = ezWnd; WndNow; WndNow = WndNow->ezParent)//�����ƣ�ֱ��������
	{
		if (WndNow->ShowState == 2)return 0;
	}


	RECT rect = { ezWnd->px,ezWnd->py  ,ezWnd->px + ezWnd->Width,ezWnd->py + ezWnd->Height };

	BroadcastProc(ezWnd->ezRootParent, SEZWM_REDRAW, (WPARAM)NULL, (LPARAM)NULL);
	BOOL bFound = 0;
	RedrawBroadcast(ezWnd->ezRootParent, 0, 0, 0, 0, rect);


	EZSendMessage(ezWnd->ezRootParent, EZWM_MAPDC, &rect, ezWnd);

	if (hdc)
	{
		//	BitBlt(hdc, ezWnd->px - ezWnd->ScrollX, ezWnd->py - ezWnd->ScrollY, ezWnd->Width, ezWnd->Height, ezWnd->hdc, 0, 0, SRCCOPY);
		BitBlt(hdc, 0, 0, ezWnd->Width, ezWnd->Height, ezWnd->hdc, 0, 0, SRCCOPY);
	}
	else//ûDC
	{
		
		hdc = GetDC(ezWnd->hParent);
		
		StretchBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
			ezWnd->hdc, 0, 0, ezWnd->Width, ezWnd->Height, SRCCOPY);
		

		ReleaseDC(ezWnd->hParent, hdc);
	}


	//	if (EZSendMessage(ezWnd, EZWM_MAPDC, ezWnd->ezRootParent->TopWndExtend->hdcTop, MAKELPARAM(ezWnd->px - ezWnd->ScrollX, ezWnd->py - ezWnd->ScrollY)) == 0)
	{
		//BitBlt(ezWnd->ezRootParent->TopWndExtend->hdcTop, ezWnd->px, ezWnd->py, ezWnd->Width, ezWnd->Height, ezWnd->hdc, 0, 0, SRCCOPY);
		StretchBlt(ezWnd->ezRootParent->TopWndExtend->hdcTop, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
			ezWnd->hdc, 0, 0, ezWnd->Width, ezWnd->Height, SRCCOPY);
	}

	EZSendMessage(ezWnd, EZWM_REDRAWFINISH, 0, 0);//����ӳ��ʱ�䵽
	return 0;
}




//��ʱ������
int SetEZTimer(EZWND ezWnd, int iTimeSpace)
{
	//�����ʱ�����ڶ������ڵļ�ʱ�����ҵ�һ����λ��д����Ϣ�����ط����ID��
	int i;
	for (i = 0; i < MAX_EZ_TIMER; i++)
	{
		if (ezWnd->ezRootParent->TopWndExtend->Timer[i].ezWnd == NULL)
		{
			//д����Ϣ
			ezWnd->ezRootParent->TopWndExtend->Timer[i].ezWnd = ezWnd;

			int IDGet = SetTimer(NULL, 0, iTimeSpace, ezInsideTimerProc);
			KillTimer(0, IDGet);

			ezWnd->ezRootParent->TopWndExtend->Timer[i].WinTimerID = SetTimer(ezWnd->hParent, IDGet, iTimeSpace, ezInsideTimerProc);
			return i;
		}
	}
	return -1;

}

BOOL KillEZTimer(EZWND ezWnd, int TimerID)
{
	//ɾ����Ϣ
	if (ezWnd->ezRootParent->TopWndExtend->Timer[TimerID].WinTimerID == -1)return -1;
	if (ezWnd->ezRootParent->TopWndExtend->Timer[TimerID].ezWnd != NULL)
	{
		KillTimer(ezWnd->hParent, ezWnd->ezRootParent->TopWndExtend->Timer[TimerID].WinTimerID);
		ezWnd->ezRootParent->TopWndExtend->Timer[TimerID].ezWnd = NULL;
		ezWnd->ezRootParent->TopWndExtend->Timer[TimerID].WinTimerID = 0;
		return 0;
	}
	else
	{
		return -1;
	}
}




//��꺯��
BOOL EZCreateCaret(EZWND ezWnd, HBITMAP hBitmap, int nWidth, int nHeight)
{
	return CreateCaret(ezWnd->hParent, hBitmap, nWidth, nHeight);
}

BOOL EZSetCaretPos(EZWND ezWnd, int x, int y)
{
	//TODO:����Ƿ񳬳�������ʾ�߽硣
	return SetCaretPos(ezWnd->px + x, ezWnd->py + y);
}

BOOL EZShowCaret(EZWND ezWnd)
{
	return ShowCaret(ezWnd->hParent);
}

BOOL EZHideCaret(EZWND ezWnd)
{
	return HideCaret(ezWnd->hParent);
}

BOOL EZDestroyCaret()
{
	DestroyCaret();
}

//�����Ի�����

typedef struct tagDlgMaskHookExtend
{
	BOOL bCenter;
	pEZSE OldExtend;
	EZWNDPROC OldProc;
	EZWND Mask, Dialog;

}DlgMaskHookExtend;


EZWND EZDialogBox(EZWND ezParent, int x, int y, int w, int h, DWORD Style, COLORREF MaskClr, EZWNDPROC ezWndProc)
{
	//TODO: ʹ�����µ���Ϣ EZWM_COVERCHILD ������Mask�������������
	EZWND Mask;
	EZWND Dialog;
	BOOL bMask, bCenter;
	bMask = Style & EZDLG_MASK;
	bCenter = Style & EZDLG_CENTER;

	//������Ҫ����һЩ��Ϣ�����縸���ڴ�С�ı�֮���
	DlgMaskHookExtend * DlgMskHkExtend;
	DlgMskHkExtend = malloc(sizeof(DlgMaskHookExtend));
	DlgMskHkExtend->OldExtend = ezParent->Extend;
	DlgMskHkExtend->OldProc = ezParent->ezWndProc;
	DlgMskHkExtend->bCenter = bCenter;
	ezParent->Extend = DlgMskHkExtend;
	ezParent->ezWndProc = EZDlgHookProc;
	DlgMskHkExtend->Mask = 0;
	if (ezParent)
	{

		Mask = CreateEZWindow(ezParent, 0, 0, ezParent->Width, ezParent->Height, EZDialogBoxMask);
		DlgMskHkExtend->Mask = Mask;
		EZSendMessage(Mask, EZWM_USER_NOTIFY, 0, MaskClr);
		if (bMask)
		{
			EZSendMessage(Mask, EZWM_USER_NOTIFY, 1, 1);//�򿪣�or��ing��Mask	
		}
		if (bCenter)
		{
			x = (ezParent->Width - w) / 2;
			y = (ezParent->Height - h) / 2;

		}
		Dialog = CreateEZWindow(ezParent, x, y, w, h, ezWndProc);
		/*if (ezParent->ezRootParent->TopWndExtend->FocusWindow)
		{
			ezParent->ezRootParent->TopWndExtend->FocusWindow = Dialog;
			Dialog->FocusState = 1;
		}*/
		DlgMskHkExtend->Dialog = Dialog;
		if (bMask)
		{
			EZSendMessage(Mask, EZWM_USER_NOTIFY, 2, Dialog);//����Dialog
		}
	}
	EZRepaint(ezParent, 0);
}


BOOL EZEndDialog(EZWND ezWnd)
{

	EZWND ezParent = ezWnd->ezParent;

	//ע�⣬������ǶԻ�����
	//���Ȼ�ԭ�����ڵ�Extend��WndProc,���������Mask���䣬����ɺ���ɾ���������Ϣ͸������ɾ�����ɵ�Dialog
	DlgMaskHookExtend * NewExt = ezParent->Extend;

	ezParent->Extend = NewExt->OldExtend;
	ezParent->ezWndProc = NewExt->OldProc;
	if (NewExt->Mask)
	{

		EZSendMessage(NewExt->Mask, EZWM_USER_NOTIFY, 1, 0);//�ر�Mask
		NewExt->Mask->MouseMsgRecv = 2;
	}
	DestroyEZWindow(ezWnd);
	free(NewExt);
	//EZRepaint(ezParent->ezRootParent, 0);
	InvalidateRect(ezParent->hParent, 0, 1);
	return 0;
}


EZWNDPROC EZDlgHookProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	int iRet;
	DlgMaskHookExtend * NewExt = ezWnd->Extend;

	if (message == EZWM_DESTROY)
	{
		//���ʱ���Ӵ���Ӧ��ɾ�ɾ��ˡ�Mask���ڴ�Ҳ���ͷ�����

		ezWnd->Extend = NewExt->OldExtend;//�ָ���ԭ����Extend
										  //���͸�ԭ���Ĵ��ڹ���
		ezWnd->ezWndProc = NewExt->OldProc;
		free(NewExt);
		return EZSendMessage(ezWnd, message, wParam, lParam);
	}
	if (message == EZWM_SIZE)
	{
		//����һ��
		MoveEZWindow(NewExt->Mask, 0, 0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0);
		if (NewExt->bCenter)
		{
			MoveEZWindow(NewExt->Dialog,
				(GET_X_LPARAM(lParam) - NewExt->Dialog->Width) >> 1,
				(GET_Y_LPARAM(lParam) - NewExt->Dialog->Height) >> 1, NewExt->Dialog->Width, NewExt->Dialog->Height, 0);
		}
		//EZRepaint(ezWnd, 0);

	}

	ezWnd->Extend = NewExt->OldExtend;//�ָ���ԭ����Extend
									  //���͸�ԭ���Ĵ��ڹ���
									  //���˵������Ϣ�����˱����Ϣ�������ڹ��̹���û��ж��������������Ϣ���ᴫ�����������ʱ��Extend�Ѿ���ж������״̬�ˣ��ͻ�gg�����ǵ�EZWM_SIZE���ٻ���������size���ǲ�������ѭ�����𣩣�����������ʱ��ԭ���ڹ���
	ezWnd->ezWndProc = NewExt->OldProc;
	iRet = NewExt->OldProc(ezWnd, message, wParam, lParam);
	//��ԭ
	ezWnd->ezWndProc = EZDlgHookProc;
	ezWnd->Extend = NewExt;
	return iRet;
}

EZWNDPROC EZDialogBoxMask(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	struct tagDialogBoxMaskInfo {
		int MaskOpen;
		int TimerID;
		BOOL MouseHold;
		EZWND Dialog;
		HBRUSH hBrush;
	} *DlgMaskInfo = 0;

	if (message != EZWM_CREATE)
	{
		DlgMaskInfo = ezWnd->Extend;
	}
	switch (message)
	{
	case EZWM_CREATE:
		DlgMaskInfo = ezWnd->Extend = malloc(sizeof(struct tagDialogBoxMaskInfo));
		DlgMaskInfo->hBrush = CreateSolidBrush(RGB(0, 0, 0));
		DlgMaskInfo->MaskOpen = 0;
		DlgMaskInfo->TimerID = -1;
		DlgMaskInfo->Dialog = 0;
		ezWnd->Transparent = 0;
		DlgMaskInfo->MouseHold = 0;
		return 0;

	case EZWM_USER_NOTIFY:
		if (wParam == 0)//������ɫ
		{
			DeleteObject(DlgMaskInfo->hBrush);
			DlgMaskInfo->hBrush = CreateSolidBrush(lParam);
			EZRepaint(ezWnd, 0);
			return 0;
		}
		if (wParam == 1)
		{
			//�й�Mask����
			//	DlgMaskInfo->MaskOpen = lParam;
			if (lParam)//��Mask
			{
				if (DlgMaskInfo->MaskOpen == 0)
				{
					DlgMaskInfo->MaskOpen = 1;
					if (DlgMaskInfo->TimerID == -1)
					{
						DlgMaskInfo->TimerID = SetEZTimer(ezWnd, 40);
					}

				}
			}
			else//�ر�Mask
			{
				if (DlgMaskInfo->MaskOpen == 1)
				{
					DlgMaskInfo->MaskOpen = 0;
					if (DlgMaskInfo->TimerID == -1)
					{
						DlgMaskInfo->TimerID = SetEZTimer(ezWnd, 40);
					}

				}
			}
			return 0;
		}
		if (wParam == 2)
		{
			DlgMaskInfo->Dialog = lParam;
		}
	case EZWM_LBUTTONDOWN:
		DlgMaskInfo->MouseHold = 1;
		EZCaptureMouse(ezWnd);
		return 0;
	case EZWM_LBUTTONUP:
		if (DlgMaskInfo->MouseHold == 1)
		{
			EZReleaseMouse(ezWnd);
			DlgMaskInfo->MouseHold = 0;
			if (DlgMaskInfo->Dialog)
			{
				EZSendMessage(DlgMaskInfo->Dialog, EZWM_COMMAND, 0, ezWnd);
			}
		}

		return 0;
	case EZWM_TIMER:
		if (DlgMaskInfo->MaskOpen)
		{
			if (ezWnd->Transparent + 16 >= 64)
			{
				ezWnd->Transparent = 64;
				KillEZTimer(ezWnd, DlgMaskInfo->TimerID);
				DlgMaskInfo->TimerID = -1;
			}
			else
			{
				ezWnd->Transparent += 16;
			}
		}
		else
		{
			//���ڹر�ing
			if (ezWnd->Transparent - 16 <= 0)
			{
				ezWnd->Transparent = 0;
				KillEZTimer(ezWnd, DlgMaskInfo->TimerID);
				DlgMaskInfo->TimerID = -1;
				EZRepaint(ezWnd->ezParent, 0);
				//��ɾ��

				DestroyEZWindow(ezWnd);
				return 0;//�������EZRepaint
			}
			else
			{
				ezWnd->Transparent -= 16;
			}
		}

		EZRepaint(ezWnd->ezParent, 0);
		//��ʱ����
		/*	if (DlgMaskInfo->Dialog)
		{
		EZUpdate(DlgMaskInfo->Dialog, 0);
		}*/

		return 0;

	case EZWM_TRANSDRAW:
		SelectObject(wParam, DlgMaskInfo->hBrush);
		PatBlt(wParam, 0, 0, ezWnd->Width, ezWnd->Height, PATCOPY);
		return 0;

	case EZWM_DESTROY:
		DeleteObject(DlgMaskInfo->hBrush);

		if (DlgMaskInfo->TimerID != -1)
		{
			KillEZTimer(ezWnd, DlgMaskInfo->TimerID);
			DlgMaskInfo->TimerID = -1;
		}

		free(DlgMaskInfo);

		return 0;
	}
	return 0;
}

//�ڲ�����
LRESULT CALLBACK EZParentWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	EZWND ezWnd = NULL;

	if (message != WM_CREATE)
	{
		ezWnd = ((EZWND)GetWindowLongPtr(hwnd, 0));
		if (ezWnd == NULL)
		{
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	Sleep(2000);
	switch (message)
	{
	
	case WM_CREATE:
	{
		ezWnd = (EZWND)(((LPCREATESTRUCT)lParam)->lpCreateParams);
		SetWindowLongPtr(hwnd, 0, (LONG_PTR)ezWnd);
		ezWnd->hParent = hwnd;//����Win������

							  //HDC fhdc;
							  //��ʵ����Ӧ�����������.���ǣ�hdc�������ڲ��������𣿣����ɴ�����Ǹ��ɣ�ʡ�ڴ�
							  //ezWnd->DrawOnNC = 0;

		hdc = GetDC(hwnd);
		ezWnd->hdc = GetMemDC(hdc, ezWnd->Width, ezWnd->Height);
		// ezWnd->hdcCopy = GetMemDC(hdc, ezWnd->Width, ezWnd->Height);
		ezWnd->TopWndExtend->hdcTop = GetMemDC(hdc, ezWnd->Width, ezWnd->Height);

		ReleaseDC(hwnd, hdc);

		EZSendMessage(ezWnd, EZWM_CREATE, 0, 0);//���ʹ�����Ϣ

		return 0;

	}

	case WM_MOVE:
	{
		ezWnd->x = GET_X_LPARAM(lParam);
		ezWnd->y = GET_Y_LPARAM(lParam);
		return 0;
	}

	case WM_SIZE:
	{
		/*if (ezWnd->DrawOnNC)
		{
		RECT rect;
		GetWindowRect(hwnd, &rect);
		ezWnd->Width = rect.right - rect.left;
		ezWnd->Height = rect.bottom - rect.top;
		}
		else*/
		{
			ezWnd->Width = GET_X_LPARAM(lParam);
			ezWnd->Height = GET_Y_LPARAM(lParam);
		}


		//DeleteMemDC(ezWnd->hdc);
		//DeleteMemDC(ezWnd->hdcCopy);
		//DeleteMemDC(ezWnd->TopWndExtend->hdcTop);

		//HDC fhdc;
		//��ʵ����Ӧ�����������.���ǣ�hdc�������ڲ��������𣿣����ɴ�����Ǹ��ɣ�ʡ�ڴ�

		//	hdc = GetDC(hwnd);
		/*ezWnd->hdc = GetMemDC(hdc, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		ezWnd->hdcCopy = GetMemDC(hdc, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		ezWnd->TopWndExtend->hdcTop = GetMemDC(hdc, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));*/

		//ReleaseDC(hwnd, hdc);
		hdc = GetDC(hwnd);
		AdjustMemDC(ezWnd->hdc, hdc, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		// AdjustMemDC(ezWnd->hdcCopy, hdc, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		AdjustMemDC(ezWnd->TopWndExtend->hdcTop, hdc, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		ReleaseDC(hwnd, hdc);
		EZSendMessage(ezWnd, EZWM_SIZE, wParam, lParam);

		EZRepaint(ezWnd, NULL);

		return 0;

	}

	case WM_LBUTTONDOWN:
	{
		if (IsEZWindow(ezWnd->TopWndExtend->CptMouseWindow))
		{
			ezInsideWndProc(ezWnd->TopWndExtend->CptMouseWindow, EZWM_LBUTTONDOWN, wParam,
				MAKELPARAM(GET_X_LPARAM(lParam) - ezWnd->TopWndExtend->CptMouseWindow->px,
					GET_Y_LPARAM(lParam) - ezWnd->TopWndExtend->CptMouseWindow->py));
			return 0;
		}
		ezInsideWndProc(ezWnd, EZWM_LBUTTONDOWN, wParam,
			MAKELPARAM(GET_X_LPARAM(lParam) - ezWnd->ScrollX, GET_Y_LPARAM(lParam) - ezWnd->ScrollY));
		return 0;

	}

	case WM_LBUTTONUP:
	{
		if (IsEZWindow(ezWnd->TopWndExtend->CptMouseWindow))
		{
			ezInsideWndProc(ezWnd->TopWndExtend->CptMouseWindow, EZWM_LBUTTONUP, wParam,
				MAKELPARAM(GET_X_LPARAM(lParam) - ezWnd->TopWndExtend->CptMouseWindow->px,
					GET_Y_LPARAM(lParam) - ezWnd->TopWndExtend->CptMouseWindow->py));
			return 0;
		}
		ezInsideWndProc(ezWnd, EZWM_LBUTTONUP, wParam,
			MAKELPARAM(GET_X_LPARAM(lParam) - ezWnd->ScrollX, GET_Y_LPARAM(lParam) - ezWnd->ScrollY));
		return 0;
	}

	case WM_RBUTTONDOWN:
	{
		if (IsEZWindow(ezWnd->TopWndExtend->CptMouseWindow))
		{
			ezInsideWndProc(ezWnd->TopWndExtend->CptMouseWindow, EZWM_RBUTTONDOWN, wParam,
				MAKELPARAM(GET_X_LPARAM(lParam) - ezWnd->TopWndExtend->CptMouseWindow->px,
					GET_Y_LPARAM(lParam) - ezWnd->TopWndExtend->CptMouseWindow->py));
			return 0;
		}
		ezInsideWndProc(ezWnd, EZWM_RBUTTONDOWN, wParam,
			MAKELPARAM(GET_X_LPARAM(lParam) - ezWnd->ScrollX, GET_Y_LPARAM(lParam) - ezWnd->ScrollY));
		return 0;

	}

	case WM_RBUTTONUP:
	{
		if (IsEZWindow(ezWnd->TopWndExtend->CptMouseWindow))
		{
			ezInsideWndProc(ezWnd->TopWndExtend->CptMouseWindow, EZWM_RBUTTONUP, wParam,
				MAKELPARAM(GET_X_LPARAM(lParam) - ezWnd->TopWndExtend->CptMouseWindow->px,
					GET_Y_LPARAM(lParam) - ezWnd->TopWndExtend->CptMouseWindow->py));
			return 0;
		}
		ezInsideWndProc(ezWnd, EZWM_RBUTTONUP, wParam,
			MAKELPARAM(GET_X_LPARAM(lParam) - ezWnd->ScrollX, GET_Y_LPARAM(lParam) - ezWnd->ScrollY));
		return 0;

	}

	case WM_MOUSEMOVE:
	{
		//������������ȥ��
		if (!ezWnd->TopWndExtend->MouseOnWnd)
		{
			//�ոս���

			//���Խ��м���ˡ�
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.dwHoverTime = 0;
			tme.hwndTrack = hwnd;
			TrackMouseEvent(&tme);
		}
		if (IsEZWindow(ezWnd->TopWndExtend->CptMouseWindow))
		{
			ezInsideWndProc(ezWnd->TopWndExtend->CptMouseWindow, EZWM_MOUSEMOVE, wParam,
				MAKELPARAM(GET_X_LPARAM(lParam) - ezWnd->TopWndExtend->CptMouseWindow->px,
					GET_Y_LPARAM(lParam) - ezWnd->TopWndExtend->CptMouseWindow->py));
			return 0;
		}
		ezInsideWndProc(ezWnd, EZWM_MOUSEMOVE, wParam,
			MAKELPARAM(GET_X_LPARAM(lParam) - ezWnd->ScrollX, GET_Y_LPARAM(lParam) - ezWnd->ScrollY));
		return 0;

	}

	case WM_CAPTURECHANGED:
	{
		//EZReleaseMouse(((EZWND)GetWindowLongPtr(hwnd, 0))->CptMouseWindow);
		//����������У���Ϊ������������ظ�����ReleaseCapture.

		//���������������������ݿ�������һ�ľͿ�����
		if (ezWnd->TopWndExtend->CptMouseWindow)
		{
			EZSendMessage(ezWnd->TopWndExtend->CptMouseWindow, EZWM_RELEASEMOUSE, 0, 0);
			ezWnd->TopWndExtend->CptMouseWindow = NULL;

		}
		return 0;
	}

	case WM_MOUSELEAVE:
	{
		//ѽ����������~~~   �㲥һ�£���ÿ���Ӵ��ڿ����ǲ��Ǵ��Լ��������ȥ�ġ�
		//�ȼ���Լ���
		if (ezWnd->TopWndExtend->MouseOnWnd)
		{
			ezWnd->TopWndExtend->MouseOnWnd->MouseOn = FALSE;
			EZSendMessage(ezWnd->TopWndExtend->MouseOnWnd, EZWM_MOUSELEAVE, 0, 0);
			ezWnd->TopWndExtend->MouseOnWnd = NULL;
		}
		return 0;
	}

	case WM_NCHITTEST:
	{
		int iHT;
		iHT = EZSendMessage(ezWnd, EZWM_WINNCHITTEST, wParam, lParam);//ֻ�������ڲ����յ� EZWM_WINNCHITTEST ��Ϣ
		if (iHT == 0)
		{
			break;
		}
		if (iHT > 0)
		{
			return iHT;
		}
		if (iHT < 0)
		{
			return iHT + 1;
		}

	}


	case WM_ERASEBKGND:
	{
		//ֱ�����ӣ��ҹ����أ����������Ļ�,��ERASE����PAINT��
		return 1;
	}

	case WM_NCPAINT:
	{
		if (!EZSendMessage(ezWnd, EZWM_WINNCDRAW, wParam, lParam))
		{
			//Ϊ0��Ĭ��
			break;
		}
		return 0;
	}

	case WM_PAINT:
	{
		//�ػ�������Ч��������
		hdc = BeginPaint(hwnd, &ps);

		//	��������Ч���ν������Ż�
		BitBlt(hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, ezWnd->TopWndExtend->hdcTop, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);
		EndPaint(hwnd, &ps);

		//���ˣ�������Ч�ˡ�����һ�¡�
		ezWnd->Update = 0;
		return 0;
		/*}*/

	}

	case WM_GETMINMAXINFO:
	{
		if (EZSendMessage(ezWnd, EZWM_GETMINMAXINFO, wParam, lParam))
		{
			return 0;
		}
		//����ֵΪ0������Ĭ��
		break;
	}

	case WM_NCCALCSIZE:
	{
		int iRet = EZSendMessage(ezWnd, EZWM_WINNCCALCSIZE, wParam, lParam);
		if (iRet == 0)
		{
			break;
		}
		return iRet - 1;
	}

	case WM_ACTIVATE:
	{
		//return EZSendMessage(ezWnd, EZWM_ACTIVATE, wParam, lParam);
		break;
	}

	case WM_ACTIVATEAPP:
	{
		//return EZSendMessage(ezWnd, EZWM_ACTIVATEAPP, wParam, lParam);
		break;
	}

	case WM_NCACTIVATE:
	{

		//MSDN - The DefWindowProc function draws the title bar or icon title in its active colors when the wParam parameter is TRUE and in its inactive colors when wParam is FALSE.
		//�����ܹ����˼һ��ưɡ���������ֵ��Ҳ���봦�������Ǿ�д��������

		int iRet = DefWindowProc(hwnd, message, wParam, lParam);
		EZSendMessage(ezWnd, EZWM_WINNCACTIVATE, wParam, lParam);
		return 1;
	}

	case WM_CLOSE:
		if (EZSendMessage(ezWnd, EZWM_CLOSE, 0, 0) != EZDO_NOT_CLOSE)
		{
			break;
		}
		return 0;

	case WM_DESTROY:
	{
		//�ȱ�����Ϣ�����ƣ������������ﴰ��
		if (IsEZWindow(ezWnd->FirstChild))
		{
			EZWND FirstChildBuf = ezWnd->FirstChild;
			ezWnd->FirstChild = 0;
			DestroyEZWindowWithNext(FirstChildBuf);
		}
		//�����Լ�������
		EZSendMessage(ezWnd, EZWM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);//����������Ϣ
		DeleteMemDC(ezWnd->hdc);//����DC
	   // DeleteMemDC(ezWnd->hdcCopy);//����DC
		DeleteMemDC(ezWnd->TopWndExtend->hdcTop);

		free(ezWnd->TopWndExtend);
		//�����������ģ���Windows��ʼִ�дݻٴ���ʱ�������ݹ�����ɾ�������ͻ����Ӵ��ڷ��ָ�������һ����ʽ���ں󣬾���Ҫ�󸸴���ɾ���ҽ�ɾ���Լ���
		if (ezWnd->IsStyleWindow)
		{
			free(ezWnd->Extend);
		}

		free(ezWnd);
		SetWindowLongPtr(hwnd, 0, NULL);

		break;
	}

	case WM_KILLFOCUS:
		//֪ͨ���㴰�ڣ�ʧȥ���㡣
		if (IsEZWindow(ezWnd->TopWndExtend->FocusWindow))
		{
			ezWnd->ezRootParent->TopWndExtend->FocusWindow->FocusState = 0;

			EZWND Buf = ezWnd->ezRootParent->TopWndExtend->FocusWindow;
			ezWnd->ezRootParent->TopWndExtend->FocusWindow = NULL;

			EZSendMessage(Buf, EZWM_KILLFOCUS, 0, 0);
		}
		return 0;

	case WM_CHAR:
	case WM_KEYDOWN:
	case WM_KEYUP:
	{
		EZWND MsgToSend = NULL;
		if (ezWnd->TopWndExtend->FocusWindow != NULL)
		{
			MsgToSend = ezWnd->TopWndExtend->FocusWindow;
		}
		if (ezWnd->TopWndExtend->CptKbdWindow != NULL)
		{
			MsgToSend = ezWnd->TopWndExtend->CptKbdWindow;
		}

		if (MsgToSend)
		{
			switch (message)
			{
			case WM_CHAR:
				EZSendMessage(MsgToSend, EZWM_CHAR, wParam, lParam);
				return 0;
			case WM_KEYDOWN:
				EZSendMessage(MsgToSend, EZWM_KEYDOWN, wParam, lParam);
				return 0;
			case WM_KEYUP:
				EZSendMessage(MsgToSend, EZWM_KEYUP, wParam, lParam);
				return 0;
			}
		}


	}
	return 0;

	}

	return DefWindowProc(hwnd, message, wParam, lParam);

}

VOID CALLBACK ezInsideTimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{

	//���б����ҵ���Ӧ��ezWnd����ת��EZWM_TIMER��Ϣ

	int i;
	pTWE TopExtend = ((EZWND)GetWindowLongPtr(hwnd, 0))->TopWndExtend;
	for (i = 0; i < MAX_EZ_TIMER; i++)
	{
		if (iTimerID == TopExtend->Timer[i].WinTimerID)
		{
			//��ʱ��ID�ͼ�¼�����
			//��ô��������Ϣ��
			EZSendMessage(TopExtend->Timer[i].ezWnd, EZWM_TIMER, i, 0);


			break;
		}
	}
	return;
}

int ezInsideWndProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	//����޸ĺ���������ö�ٷ���Ӧ�÷����������ҵ����Ȼ�󷵻���һ������顣����֤�ͻ��Ƶ�ö��˳����ͬ��
	EZWND ezChildLast;

	ezChildLast = ezWnd->FirstChild;

	if (IsEZWindow(ezChildLast))
	{

		while (ezChildLast->NextEZWnd)
		{
			ezChildLast = ezChildLast->NextEZWnd;
		}

		//������������������Ƿ����Ӵ�����
		//**************************************************
		while (ezChildLast)
		{
			//����Ƿ����Ӵ����ڣ�ע����Ļ�ƶ�
			//if (PtInEZWnd(ezChildLast, GET_X_LPARAM(lParam) - ezChildLast->ScrollX, GET_Y_LPARAM(lParam) - ezChildLast->ScrollY))
			if ((ezChildLast->MouseMsgRecv != 2) && PtInEZWnd(ezChildLast, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))
			{
				//�����ˣ����������
				if (ezInsideWndProc(ezChildLast, message, wParam, MAKELPARAM(GET_X_LPARAM(lParam) - ezChildLast->x - ezChildLast->ScrollX, GET_Y_LPARAM(lParam) - ezChildLast->y - ezChildLast->ScrollY)) != TRANSPARENT)
				{
					return 0;
				}
				//�ðɣ��Ӵ���˵����͸���ġ������������������᲻�����������Ӵ�����
			}

			ezChildLast = ezChildLast->LastEZWnd;
		}
		//**************************************************		

		//�Ѿ�������ˣ���ô�������������ڡ�ֱ�ӳ�ȥ������ĺϲ�

	}

	{
		//�����Ӵ��ڶ�û��������ô�������������ˡ�

		//�ȷ�����굽����Ϣ
		//���������ʱ�򣬱����˰�������ͬ��һ�����~~~~~
		if (message == EZWM_MOUSEMOVE && ezWnd->MouseOn == FALSE)
		{
			if (ezWnd->ezRootParent->TopWndExtend->MouseOnWnd)
			{
				ezWnd->ezRootParent->TopWndExtend->MouseOnWnd->MouseOn = FALSE;//���һ�¡�
				EZSendMessage(ezWnd->ezRootParent->TopWndExtend->MouseOnWnd, EZWM_MOUSELEAVE, 0, 0);
			}
			ezWnd->ezRootParent->TopWndExtend->MouseOnWnd = ezWnd;
			ezWnd->MouseOn = TRUE;//���һ�¡�
			EZSendMessage(ezWnd, EZWM_MOUSECOME, (WPARAM)NULL, (LPARAM)NULL);//��������
		}

		else if (ezWnd->FocusState == 0 && message == EZWM_LBUTTONDOWN)//��û���㣬��Ϣ�����
		{
			if (IsEZWindow(EZGetTopParentWindow(ezWnd)->TopWndExtend->FocusWindow))
			{
				EZGetTopParentWindow(ezWnd)->TopWndExtend->FocusWindow->FocusState = 0;//��û�����ˡ�
				EZSendMessage(EZGetTopParentWindow(ezWnd)->TopWndExtend->FocusWindow, EZWM_KILLFOCUS, NULL, NULL);
			}
			ezWnd->FocusState = 1;

			EZGetTopParentWindow(ezWnd)->TopWndExtend->FocusWindow = ezWnd;
			EZSendMessage(ezWnd, EZWM_SETFOCUS, (WPARAM)NULL, (LPARAM)NULL);//���н����ˣ�
		}
		return EZSendMessage(ezWnd, message, wParam, lParam);

	}
}



BOOL EZBroadcastToAllChildFunc(EZWND ezWnd, BOOL sequence, int message, WPARAM wp, LPARAM lp)
{
	//ע�⣬�����������������ȵķ�ʽ������Ϣ�ġ�����������дһ��������ȵģ�����Ժ���д�ˣ�ɾ����һ�仰��
	EZWND LastChild;

	LastChild = ezWnd->FirstChild;
	//��ع�����������ֱ�Ӵ������𲻴���
	for (;;)
	{
		if (sequence)//��
		{
			//�������ԣ��ȴ����Լ���
			BroadcastProc(LastChild, message, wp, lp);//�����Լ�

			EZBroadcastToAllChild(LastChild, sequence, message, wp, lp);//���Լ����Ӵ��ڷ��͸���Ϣ

		}

		if (!IsEZWindow(LastChild->NextEZWnd))//û����һ��
		{
			break;//û����һ����
		}

		LastChild = LastChild->NextEZWnd;//���¹�

	}

	//���������ô�ٹ���ȥ
	if (!sequence)
	{
		for (;;)
		{
			//���Լ����Ӵ��ڷ��͸���Ϣ
			EZBroadcastToAllChild(LastChild, sequence, message, wp, lp);

			BroadcastProc(LastChild, message, wp, lp);//�����Լ�

			if (!IsEZWindow(LastChild->LastEZWnd))//��ͷ��
			{
				break;
			}

			LastChild = LastChild->LastEZWnd;//�ع�

		}
	}
	return TRUE;
}

BOOL BroadcastProc(EZWND ezWnd, int Param, WPARAM wP, LPARAM lP)
{
	if (!IsEZWindow(ezWnd))
	{
		return FALSE;
	}

	switch (Param)
	{
	case SEZWM_COPYDC:
	{
		EZSendMessage(ezWnd, EZWM_COVERCHILD, ezWnd->hdc, 0);
		//�����Ƿ��Ƕ���EZ���ڡ�
		if (!ezWnd->IsTopWindow)
		{
			//����
			BitBlt(ezWnd->ezParent->hdc,
				ezWnd->x + ezWnd->ezParent->ScrollX,
				ezWnd->y + ezWnd->ezParent->ScrollY,
				ezWnd->Width, ezWnd->Height,
				ezWnd->hdc,
				0, 0,
				SRCCOPY);
			/* BitBlt(ezWnd->ezParent->hdcCopy,
				 ezWnd->x + ezWnd->ezParent->ScrollX,
				 ezWnd->y + ezWnd->ezParent->ScrollY,
				 ezWnd->Width, ezWnd->Height,
				 ezWnd->hdc,
				 0, 0,
				 SRCCOPY);*/
		}

		return 0;
	}
	case SEZWM_REDRAW:
	{

		int X_PSX;
		int Y_PSY;
		//�ж��Ƿ��Ƕ��㴰��
		if (ezWnd->IsTopWindow)
		{
			//���ײ�һ��
			PatBlt(ezWnd->hdc, 0, 0, ezWnd->Width, ezWnd->Height, WHITENESS);
			//PatBlt(ezWnd->hdcWC, 0, 0, ezWnd->Width, ezWnd->Height, WHITENESS);
		}
		else
		{
			X_PSX = ezWnd->x + ezWnd->ezParent->ScrollX;
			Y_PSY = ezWnd->y + ezWnd->ezParent->ScrollY;
			//�����ϼ����ڵ���Ӧ���֡�
			BitBlt(ezWnd->hdc, 0, 0,
				max(ezWnd->Width, ezWnd->ezParent->Width), max(ezWnd->Height, ezWnd->ezParent->Height),
				ezWnd->ezParent->hdc,
				X_PSX,
				Y_PSY,
				SRCCOPY);
		}

		EZSendMessage(ezWnd, EZWM_TRANSDRAW, (WPARAM)(ezWnd->hdc), (LPARAM)NULL);


		if (ezWnd->Transparent != 255)//�������255����ϡ���255����ô�������ˣ�
		{
			BLENDFUNCTION bf = { 0 };
			/*bf.AlphaFormat = 0;
			bf.BlendFlags = 0;
			bf.BlendOp = AC_SRC_OVER;*/ // �������ֶ�ȫ�� 0
			bf.SourceConstantAlpha = 255 - ezWnd->Transparent;

			if (!ezWnd->IsTopWindow)
			{
				//����������������������߽硣����ֻ���ֶ�ȷ��û�г����߽硣
				int ab_Width, ab_Height;
				ab_Width = min((ezWnd->Width), (ezWnd->ezParent->Width - (X_PSX)));
				ab_Height = min((ezWnd->Height), (ezWnd->ezParent->Height - (Y_PSY)));

				AlphaBlend(ezWnd->hdc,
					0,
					0,
					ab_Width,
					ab_Height,
					ezWnd->ezParent->hdc,
					max(X_PSX, 0),
					max(Y_PSY, 0),
					ab_Width,
					ab_Height,
					bf);
			}
			else
			{
				//��ɫ
				HDC hParentdc;
				HDC hdcWhite = GetMemDC(hParentdc = GetDC(ezWnd->hParent), ezWnd->Width, ezWnd->Height);

				PatBlt(hdcWhite, 0, 0, ezWnd->Width, ezWnd->Height, WHITENESS);

				AlphaBlend(ezWnd->hdc, 0, 0, ezWnd->Width, ezWnd->Height, hdcWhite, 0, 0, ezWnd->Width, ezWnd->Height, bf);

				DeleteMemDC(hdcWhite);
				ReleaseDC(ezWnd->hParent, hParentdc);
			}
		}

		//��ϻ��������Բ�͸���ķ�ʽ��DC�ϣ����ڴӸ����ڸ�����255-͸���ȸ��Ƶ�hdcWC

		//������ͨ������Ϣ�����Ƶ�DC��
		EZSendMessage(ezWnd, EZWM_DRAW, (WPARAM)(ezWnd->hdc), (LPARAM)NULL);
		//BitBlt(ezWnd->hdcCopy, 0, 0, ezWnd->Width, ezWnd->Height, ezWnd->hdc, 0, 0, SRCCOPY);

		/*if (!ezWnd->IsTopWindow)
		{
		BitBlt(ezWnd->ezParent->hdc,
		ezWnd->x + ezWnd->ezParent->ScrollX,
		ezWnd->y + ezWnd->ezParent->ScrollY,
		ezWnd->Width, ezWnd->Height,
		ezWnd->hdc, 0, 0, SRCCOPY);
		}*/



		return 0;
	}


	}
}

int EZWndMessageLoop()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}



//�жϺ���
BOOL PtInEZWnd(EZWND ezWnd, int x, int y)
{
	return (BOOL)((x > ezWnd->x) && (x < (ezWnd->x + ezWnd->Width)) && (y > ezWnd->y) && (y < (ezWnd->y + ezWnd->Height)));
}

BOOL IsEZWindow(EZWND ezWnd)
{
	return (BOOL)(ezWnd != NULL);
}


//*********************************************************************************************************
//                ������Щ������������EZWindow����EZWindow������Щ��������Щ����Ҳ���Ե���ʹ�á�
//*********************************************************************************************************

HDC GetMemDC(HDC hdc, int cx, int cy)
{
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, cx, cy);
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, hBitmap);
	DeleteObject(hBitmap);
	return hdcMem;
}


BOOL DeleteMemDC(HDC hdc)
{
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, 1, 1);
	DeleteObject(SelectObject(hdc, hBitmap));
	DeleteDC(hdc);
	DeleteObject(hBitmap);
	return TRUE;
}

BOOL AdjustMemDC(HDC hdc, HDC hdcCpb, int cx, int cy)
{
	DeleteObject(SelectObject(hdc, CreateCompatibleBitmap(hdcCpb, cx, cy)));
}

//*********************************************************************************************************
//                EZWindow�����չͷ�ļ� �Լ��궨��
//*********************************************************************************************************




EZWNDPROC EZStyle_ButtonProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	HBRUSH hBrush;
	HPEN hPen;
	switch (message)
	{
	case EZWM_CREATE:
		SetEZWndTransparent(ezWnd, 30);
		return 0;

	case EZWM_DRAW:
		rect.left = 0;
		rect.top = 0;
		rect.right = ezWnd->Width;
		rect.bottom = ezWnd->Height;
		SetTextColor(wParam, (COLORREF)(((ezWnd->Extend))->ForeGroundColor));

		SetBkMode(wParam, TRANSPARENT);
		if (((pEZSE)ezWnd->Extend)->hFont)
		{
			SelectObject(wParam, ((pEZSE)ezWnd->Extend)->hFont);
		}

		if (CHK_ALT_STYLE(ezWnd->EZStyle, EZS_BORDER))
		{
			hPen = CreatePen(PS_SOLID, 1, ezWnd->Extend->ForeGroundColor);
			SelectPen(wParam, hPen);
			SelectBrush(wParam, GetStockObject(NULL_BRUSH));
			Rectangle(wParam, 0, 0, ezWnd->Width, ezWnd->Height);
		}
		DrawText(wParam, ((pEZSE)(ezWnd->Extend))->Title, -1, &rect, (ezWnd->Extend->TextAlign == 0) ? (DT_CENTER | DT_VCENTER | DT_SINGLELINE) : (ezWnd->Extend->TextAlign));

		return 0;
	case EZWM_TRANSDRAW:
		hBrush = CreateSolidBrush((COLORREF)(((ezWnd->Extend))->BackGroundColor));
		SelectObject(wParam, hBrush);
		PatBlt(wParam, 0, 0, ezWnd->Width, ezWnd->Height, PATCOPY);

		DeleteObject(hBrush);
		return 0;
	case EZWM_LBUTTONDOWN:
		if ((((ezWnd->Extend))->MouseHold) == FALSE)
		{
			EZCaptureMouse(ezWnd);

			(((ezWnd->Extend))->MouseHold) = TRUE;
			SetEZWndTransparent(ezWnd, 90);
			EZRepaint(ezWnd, NULL);
		}

		return 0;
	case EZWM_LBUTTONUP:
		SetEZWndTransparent(ezWnd, 60);
		EZRepaint(ezWnd, NULL);
		if ((((ezWnd->Extend))->MouseHold) == TRUE)
		{
			EZReleaseMouse(ezWnd);
			if ((LOWORD(lParam) > 0) && (LOWORD(lParam) < ezWnd->Width) && (HIWORD(lParam) > 0) && (HIWORD(lParam) < ezWnd->Height))
			{

				(((ezWnd->Extend))->MouseHold) = FALSE;
				//��ť�������ˣ�

				//�жϰ�ť����
				if (CHK_STYLE(ezWnd->EZStyle & MKDW(11111111, 00000000, 00000000, 00000000), EZBS_PUSHBUTTON))
				{
					//���Ϳ�����Ϣ��
					EZSendMessage(ezWnd->ezParent, EZWM_COMMAND, 0, ezWnd);
				}
				else if (CHK_STYLE(ezWnd->EZStyle & MKDW(11111111, 00000000, 00000000, 00000000), EZBS_RADIOBUTTON))
				{
					//û˵��AUTOŶ��Ҫ�������ڵĻ���
				}
			}

		}
		return 0;
	case EZWM_MOUSECOME:
		SetEZWndTransparent(ezWnd, 60);
		EZRepaint(ezWnd, NULL);
		return 0;

	case EZWM_RELEASEMOUSE:
		(((ezWnd->Extend))->MouseHold) = FALSE;
		return 0;

	case EZWM_MOUSELEAVE:
		SetEZWndTransparent(ezWnd, 30);
		EZRepaint(ezWnd, NULL);
		return 0;

	}
	return EZStyle_DefaultProc(ezWnd, message, wParam, lParam);
}


EZWNDPROC EZStyle_StaticProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	HBRUSH hBrush;
	HPEN hPen;
	switch (message)
	{
	case EZWM_CREATE:
		return 0;
	case EZWM_DRAW:



		rect.left = 0;
		rect.top = 0;
		rect.right = ezWnd->Width;
		rect.bottom = ezWnd->Height;
		SetTextColor(wParam, (COLORREF)(((ezWnd->Extend))->ForeGroundColor));

		SetBkMode(wParam, TRANSPARENT);
		if (((pEZSE)ezWnd->Extend)->hFont)
		{
			SelectObject(wParam, ((pEZSE)ezWnd->Extend)->hFont);
		}
		DrawText(wParam, ezWnd->Extend->Title, -1, &rect, (ezWnd->Extend->TextAlign == 0) ? (DT_WORDBREAK | DT_EDITCONTROL | DT_NOCLIP) : (ezWnd->Extend->TextAlign));

		return 0;
	case EZWM_TRANSDRAW:
		hPen = CreatePen(PS_INSIDEFRAME, 1, (COLORREF)(((ezWnd->Extend))->ForeGroundColor));
		SelectObject(wParam, hPen);


		hBrush = CreateSolidBrush((COLORREF)(((ezWnd->Extend))->BackGroundColor));
		SelectObject(wParam, hBrush);
		if (CHK_ALT_STYLE(ezWnd->EZStyle, EZS_BORDER))
		{
			Rectangle(wParam, 0, 0, ezWnd->Width, ezWnd->Height);
		}
		else
		{
			PatBlt(wParam, 0, 0, ezWnd->Width, ezWnd->Height, PATCOPY);
		}
		DeleteObject(hBrush);

		DeleteObject(hPen);
		return 0;
	}
	return EZStyle_DefaultProc(ezWnd, message, wParam, lParam);
}


EZWNDPROC EZStyle_ScrollChildProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{

	double BarLen;
	switch (message)
	{
	case EZWM_CREATE:
		//����3���Ӵ���
		//���ϣ��󣿣����� -- ��չ0�ſؼ� 
		//���£��ң������� -- ��չ1�ſؼ� 
		//�϶��� -- ��չ2�ſؼ� 

		ezWnd->Extend->hExtend[0] = CreateEZWindow(ezWnd, 0, 0, 0, 0, EZStyle_Scroll_BtnProc);
		ezWnd->Extend->hExtend[0]->ezID = 0;

		ezWnd->Extend->hExtend[1] = CreateEZWindow(ezWnd, 0, 0, 0, 0, EZStyle_Scroll_BtnProc);
		ezWnd->Extend->hExtend[1]->ezID = 1;

		ezWnd->Extend->hExtend[2] = CreateEZWindow(ezWnd, 0, 0, 0, 0, EZStyle_Scroll_BarProc);
		ezWnd->Extend->hExtend[2]->ezID = 2;


		//Ϊ�����Ӵ��ڿ��Ը�������ж�Ӧ�ú������������
		//��������ť��Style���б�ʶ

		if ((ezWnd->EZStyle & MKDW(00000000, 00000000, 00000000, 11111111)) == EZS_CHILD_VSCROLL)
		{
			//EZS_CHILD_VSCROLL����ֱ
			ezWnd->Extend->hExtend[0]->EZStyle = ezWnd->Extend->hExtend[1]->EZStyle = 0;
		}
		else
		{
			//EZS_CHILD_HSCROLL��ˮƽ
			ezWnd->Extend->hExtend[0]->EZStyle = ezWnd->Extend->hExtend[1]->EZStyle = 1;
		}


		//��ʼ��������Χ��λ��
		//�������Χ -- int��չ 0 ��
		//����λ���� -- int��չ 1 ��
		//����λ���� -- int��չ 2 ��
		ezWnd->Extend->iExtend[0] = 16;//Ĭ��16��2333������ˡ�
		ezWnd->Extend->iExtend[1] = 0;//Ĭ��0
		ezWnd->Extend->iExtend[2] = 1;//Ĭ��1

		return 0;
	case EZWM_SIZE:
	{

		if ((ezWnd->EZStyle & MKDW(00000000, 00000000, 00000000, 11111111)) == EZS_CHILD_VSCROLL)
		{	//EZS_CHILD_VSCROLL����ֱ

		   //�����ֱ���򳤶��Ƿ����3��ˮƽ����ȻҲ̫�����ˡ�
			if (!(ezWnd->Height >= ezWnd->Width * 3))
			{
				//����
				MoveEZWindow(ezWnd->Extend->hExtend[0], 0, 0, 0, 0, 0);
				MoveEZWindow(ezWnd->Extend->hExtend[1], 0, 0, 0, 0, 0);
				MoveEZWindow(ezWnd->Extend->hExtend[2], 0, 0, 0, 0, 0);
				return 0;
			}
			//����λ�ÿ�
			MoveEZWindow(ezWnd->Extend->hExtend[0], 0, 0, ezWnd->Width, (ezWnd->Width * 17) / 15, 0);//�����Ǹ���ť
			MoveEZWindow(ezWnd->Extend->hExtend[1], 0, ezWnd->Height - (ezWnd->Width * 17) / 15, ezWnd->Width, (ezWnd->Width * 17) / 15, 0);//�����Ǹ���ť

																																			//���������Ϊ MAX(���ܿ�� / ��������Χ��,��С��ȣ�

																																			//�ܿ����Ҫ��ȥ���ߵİ�ť��Ŷ
			BarLen = ((ezWnd->Height - (ezWnd->Width) * 34.0 / 15.0) / (float)ezWnd->Extend->iExtend[0]);
			MoveEZWindow(ezWnd->Extend->hExtend[2],
				0,
				ceil(ezWnd->Width * 17.0 / 15.0 + (float)BarLen * (float)ezWnd->Extend->iExtend[1]),
				ezWnd->Width,
				BarLen * (float)(ezWnd->Extend->iExtend[2] - ezWnd->Extend->iExtend[1]),
				0);

		}
		else
		{	//EZS_CHILD_HSCROLL��ˮƽ

		   //���ˮƽ���򳤶��Ƿ����3����ֱ����ȻҲ̫�����ˡ�
			if (!(ezWnd->Width >= ezWnd->Height * 3))
			{
				//����
				MoveEZWindow(ezWnd->Extend->hExtend[0], 0, 0, 0, 0, 0);
				MoveEZWindow(ezWnd->Extend->hExtend[1], 0, 0, 0, 0, 0);
				MoveEZWindow(ezWnd->Extend->hExtend[2], 0, 0, 0, 0, 0);
				return 0;
			}
			//����λ�ÿ�
			MoveEZWindow(ezWnd->Extend->hExtend[0], 0, 0, (ezWnd->Height * 17.0) / 15.0, ezWnd->Height, 0);//����Ǹ���ť
			MoveEZWindow(ezWnd->Extend->hExtend[1], ezWnd->Width - (ezWnd->Height * 17.0) / 15.0, 0, (ezWnd->Height * 17.0) / 15.0, ezWnd->Height, 0);//�ұ��Ǹ���ť


			BarLen = (((float)ezWnd->Width - (float)(ezWnd->Height) * 34.0 / 15.0) / ((float)(ezWnd->Extend->iExtend[0])));

			MoveEZWindow(ezWnd->Extend->hExtend[2],
				ceil(ezWnd->Height * 17.0 / 15.0 + (float)BarLen * (float)ezWnd->Extend->iExtend[1]),
				0,
				BarLen * (float)(ezWnd->Extend->iExtend[2] - ezWnd->Extend->iExtend[1]),
				ezWnd->Height,
				0);


		}
		return 0;
	}

	case EZWM_SETSCROLLRANGE:
		ezWnd->Extend->iExtend[0] = wParam;
		if (ezWnd->Extend->iExtend[2] > wParam)
		{
			ezWnd->Extend->iExtend[1] = 0;
			ezWnd->Extend->iExtend[2] = 1;
		}
		if ((ezWnd->EZStyle & MKDW(00000000, 00000000, 00000000, 11111111)) == EZS_CHILD_VSCROLL)
		{	//EZS_CHILD_VSCROLL����ֱ
			BarLen = ((ezWnd->Height - (ezWnd->Width) * 34.0 / 15.0) / (float)ezWnd->Extend->iExtend[0]);
			MoveEZWindow(ezWnd->Extend->hExtend[2],
				0,
				ceil(ezWnd->Width * 17.0 / 15.0 + (float)BarLen * (float)ezWnd->Extend->iExtend[1]),
				ezWnd->Width,
				BarLen * (float)(ezWnd->Extend->iExtend[2] - ezWnd->Extend->iExtend[1]),
				0);
		}
		else
		{	//EZS_CHILD_HSCROLL��ˮƽ
			BarLen = (((float)ezWnd->Width - (float)(ezWnd->Height) * 34.0 / 15.0) / ((float)(ezWnd->Extend->iExtend[0])));

			MoveEZWindow(ezWnd->Extend->hExtend[2],
				ceil(ezWnd->Height * 17.0 / 15.0 + (float)BarLen * (float)ezWnd->Extend->iExtend[1]),
				0,
				BarLen * (float)(ezWnd->Extend->iExtend[2] - ezWnd->Extend->iExtend[1]),
				ezWnd->Height,
				0);
		}

		return 0;

	case EZWM_SETSCROLLPOS:

		if (wParam < lParam)
		{
			ezWnd->Extend->iExtend[1] = wParam;
			ezWnd->Extend->iExtend[2] = lParam;
			if ((ezWnd->EZStyle & MKDW(00000000, 00000000, 00000000, 11111111)) == EZS_CHILD_VSCROLL)
			{	//EZS_CHILD_VSCROLL����ֱ
				BarLen = ((ezWnd->Height - (ezWnd->Width) * 34.0 / 15.0) / (float)ezWnd->Extend->iExtend[0]);
				MoveEZWindow(ezWnd->Extend->hExtend[2],
					0,
					ceil(ezWnd->Width * 17.0 / 15.0 + (float)BarLen * (float)ezWnd->Extend->iExtend[1]),
					ezWnd->Width,
					BarLen * (float)(ezWnd->Extend->iExtend[2] - ezWnd->Extend->iExtend[1]),
					0);
			}
			else
			{	//EZS_CHILD_HSCROLL��ˮƽ
				BarLen = (((float)ezWnd->Width - (float)(ezWnd->Height) * 34.0 / 15.0) / ((float)(ezWnd->Extend->iExtend[0])));

				MoveEZWindow(ezWnd->Extend->hExtend[2],
					ceil(ezWnd->Height * 17.0 / 15.0 + (float)BarLen * (float)ezWnd->Extend->iExtend[1]),
					0,
					BarLen * (float)(ezWnd->Extend->iExtend[2] - ezWnd->Extend->iExtend[1]),
					ezWnd->Height,
					0);
			}
			EZSendMessage(ezWnd->ezParent, EZWM_SCROLLPOSCHANGE, wParam, ezWnd);
		}
		EZRepaint(ezWnd, NULL);
		return 0;

	case EZWM_COMMAND:
		if (lParam == ezWnd->Extend->hExtend[0])
		{
			if (ezWnd->Extend->iExtend[1] - (int)(ezWnd->Extend->iExtend[0] / 10) >= 0)
			{
				EZSendMessage(ezWnd, EZWM_SETSCROLLPOS,
					ezWnd->Extend->iExtend[1] - (int)(ezWnd->Extend->iExtend[0] / 10),
					ezWnd->Extend->iExtend[2] - (int)(ezWnd->Extend->iExtend[0] / 10));
			}
			else
			{
				EZSendMessage(ezWnd, EZWM_SETSCROLLPOS,
					0,
					ezWnd->Extend->iExtend[2] - ezWnd->Extend->iExtend[1]);
			}
		}
		if (lParam == ezWnd->Extend->hExtend[1])
		{
			if (ezWnd->Extend->iExtend[2] + (int)(ezWnd->Extend->iExtend[0] / 10) <= ezWnd->Extend->iExtend[0])
			{
				EZSendMessage(ezWnd, EZWM_SETSCROLLPOS,
					ezWnd->Extend->iExtend[1] + (int)(ezWnd->Extend->iExtend[0] / 10),
					ezWnd->Extend->iExtend[2] + (int)(ezWnd->Extend->iExtend[0] / 10));
			}
			else
			{
				EZSendMessage(ezWnd, EZWM_SETSCROLLPOS,
					ezWnd->Extend->iExtend[0] - (ezWnd->Extend->iExtend[2] - ezWnd->Extend->iExtend[1]),
					ezWnd->Extend->iExtend[0]);
			}
		}
		if (lParam == ezWnd->Extend->hExtend[2])
		{
			int iMove;
			if ((ezWnd->EZStyle & MKDW(00000000, 00000000, 00000000, 11111111)) == EZS_CHILD_VSCROLL)
			{	//EZS_CHILD_VSCROLL����ֱ
				BarLen = (((float)ezWnd->Height - (float)(ezWnd->Width) * 34.0 / 15.0) / (float)ezWnd->Extend->iExtend[0]);
				iMove = ceil((float)GET_Y_LPARAM(wParam) / BarLen);//����ͨ��wParam���͹���
			}
			else
			{
				BarLen = (((float)ezWnd->Width - (float)(ezWnd->Height) * 34.0 / 15.0) / ((float)(ezWnd->Extend->iExtend[0])));
				iMove = ceil((float)GET_X_LPARAM(wParam) / BarLen);//����ͨ��wParam���͹���
			}
			if (-iMove > ezWnd->Extend->iExtend[1])
			{
				iMove = -ezWnd->Extend->iExtend[1];
			}
			if (iMove + ezWnd->Extend->iExtend[2] > ezWnd->Extend->iExtend[0])
			{
				iMove = ezWnd->Extend->iExtend[0] - ezWnd->Extend->iExtend[2];
			}

			EZSendMessage(ezWnd, EZWM_SETSCROLLPOS, ezWnd->Extend->iExtend[1] + iMove, ezWnd->Extend->iExtend[2] + iMove);

		}
		return 0;
	}
	return EZStyle_DefaultProc(ezWnd, message, wParam, lParam);
}

EZWNDPROC EZStyle_Scroll_BtnProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	HBRUSH hBrush;
	HPEN hPen;
	POINT pt[7];
	switch (message)
	{
	case EZWM_CREATE:
		//��������ֻ����ͨ���������ģ���ϧ����Ҳ������չ�����Լ����밡~
		ezWnd->Extend = malloc(sizeof(EZSE));

		((pEZSE)ezWnd->Extend)->BackGroundColor = RGB(70, 70, 70);
		((pEZSE)ezWnd->Extend)->ForeGroundColor = RGB(70, 70, 70);

		((pEZSE)ezWnd->Extend)->MouseHold = FALSE;

		((pEZSE)ezWnd->Extend)->hFont = NULL;
		((pEZSE)ezWnd->Extend)->IsFontUserControl = -1;

		ezWnd->Extend->Title = NULL;//���������ʱ��ͻ����������ˣ�����freeһ��Ұָ�롣

									//MD��������ʽ���ڣ���ð�ģ�
		ezWnd->IsStyleWindow = FALSE;
		ezWnd->EZStyle = NULL;

		ezWnd->Extend->iExtend[0] = 0;
		//����͸���� 
		SetEZWndTransparent(ezWnd, 20);
		EZRepaint(ezWnd, NULL);
		return 0;




	case EZWM_LBUTTONDOWN:
		if ((((ezWnd->Extend))->MouseHold) == FALSE)
		{
			EZCaptureMouse(ezWnd);

			(((ezWnd->Extend))->MouseHold) = TRUE;
			SetEZWndTransparent(ezWnd, 200);
			EZRepaint(ezWnd, NULL);
			EZSendMessage(ezWnd->ezParent, EZWM_COMMAND, 0, ezWnd);
			ezWnd->TimerID[0] = SetEZTimer(ezWnd, 80);
		}

		return 0;
	case EZWM_LBUTTONUP:
		SetEZWndTransparent(ezWnd, 40);
		EZRepaint(ezWnd, NULL);
		if ((((ezWnd->Extend))->MouseHold) == TRUE)
		{
			EZReleaseMouse(ezWnd);
			KillEZTimer(ezWnd, ezWnd->TimerID[0]);
			ezWnd->Extend->iExtend[0] = 0;
		}
		return 0;
	case EZWM_MOUSECOME:
		SetEZWndTransparent(ezWnd, 40);
		EZRepaint(ezWnd, NULL);
		return 0;

	case EZWM_RELEASEMOUSE:
		(((ezWnd->Extend))->MouseHold) = FALSE;
		return 0;

	case EZWM_MOUSELEAVE:
		if ((((ezWnd->Extend))->MouseHold) == TRUE)
		{
			ezWnd->Extend->MouseHold = FALSE;
			KillEZTimer(ezWnd, ezWnd->TimerID[0]);
			ezWnd->Extend->iExtend[0] = 0;
		}

		SetEZWndTransparent(ezWnd, 20);
		EZRepaint(ezWnd, NULL);
		return 0;


		//case EZWM_LBUTTONDOWN:
		//	if ((((ezWnd->Extend))->MouseHold) == FALSE)
		//	{
		//		EZCaptureMouse(ezWnd);
		//		(((ezWnd->Extend))->MouseHold) = TRUE;
		//		//ezWnd->TimerID[0] = SetTimer(NULL, 0, 100, ScrollBtnTimerProc);
		//		//������ö�ʱ����

		//		EZSendMessage(ezWnd->ezParent, EZWM_COMMAND, 0, ezWnd);
		//		ezWnd->TimerID[0] = SetEZTimer(ezWnd, 80);

		//		SetEZWndTransparent(ezWnd, 200);
		//		EZRepaint(ezWnd, NULL);
		//	}
		//	return 0;

		//case EZWM_LBUTTONUP:
		//	if ((((ezWnd->Extend))->MouseHold) == TRUE)
		//	{
		//		
		//		(((ezWnd->Extend))->MouseHold) = FALSE;
		//		//���ȡ����ʱ����
		//		KillEZTimer(ezWnd, ezWnd->TimerID[0]);
		//		ezWnd->Extend->iExtend[0] = 0;

		//		SetEZWndTransparent(ezWnd, 40);
		//		EZReleaseMouse(ezWnd);
		//		EZRepaint(ezWnd, NULL);
		//	}
		//	return 0;

		//case EZWM_MOUSECOME:
		//	SetEZWndTransparent(ezWnd, 40);
		//	EZRepaint(ezWnd, NULL);
		//	return 0;

		//case EZWM_MOUSELEAVE:
		//	if ((((ezWnd->Extend))->MouseHold) == TRUE)
		//	{
		//		ezWnd->Extend->MouseHold = FALSE;
		//		KillEZTimer(ezWnd, ezWnd->TimerID[0]);
		//		ezWnd->Extend->iExtend[0] = 0;
		//	}
		//	SetEZWndTransparent(ezWnd, 20);
		//	EZRepaint(ezWnd, NULL);
		//	return 0;

	case EZWM_DRAW:
		//���Ƽ�ͷ
		//�͵�define.
#define SRLBTN_ARR_W 3
#define SRLBTN_ARR_H 2

		 //׼�����ʣ���ˢ��
		hPen = NULL;
		hBrush = NULL;
		if ((((ezWnd->Extend))->MouseHold) == TRUE)
		{
			SelectObject(wParam, GetStockObject(WHITE_PEN));
			SelectObject(wParam, GetStockObject(WHITE_BRUSH));
		}
		else
		{
			SelectObject(wParam, hPen = CreatePen(PS_SOLID, 1, ezWnd->Extend->ForeGroundColor));
			SelectObject(wParam, hBrush = CreateSolidBrush(ezWnd->Extend->ForeGroundColor));
		}

		if (ezWnd->EZStyle == 0)
		{
			//��ֱ
			pt[0].x = ezWnd->Width / 2;
			pt[1].x = ezWnd->Width / 2 + SRLBTN_ARR_W;
			pt[2].x = ezWnd->Width / 2 + SRLBTN_ARR_W;
			pt[3].x = ezWnd->Width / 2;
			pt[4].x = ezWnd->Width / 2 - SRLBTN_ARR_W;
			pt[5].x = ezWnd->Width / 2 - SRLBTN_ARR_W;
			pt[6].x = ezWnd->Width / 2;
			if (ezWnd->ezID == 0)
			{

				pt[0].y = ezWnd->Height / 2 - SRLBTN_ARR_H;

				pt[1].y = ezWnd->Height / 2 - SRLBTN_ARR_H + SRLBTN_ARR_W;

				pt[2].y = ezWnd->Height / 2 + SRLBTN_ARR_W;

				pt[3].y = ezWnd->Height / 2;

				pt[4].y = ezWnd->Height / 2 + SRLBTN_ARR_W;

				pt[5].y = ezWnd->Height / 2 - SRLBTN_ARR_H + SRLBTN_ARR_W;

				pt[6].y = ezWnd->Height / 2 - SRLBTN_ARR_H;
			}
			else
			{

				pt[0].y = ezWnd->Height / 2 + SRLBTN_ARR_H;

				pt[1].y = ezWnd->Height / 2 + SRLBTN_ARR_H - SRLBTN_ARR_W;

				pt[2].y = ezWnd->Height / 2 - SRLBTN_ARR_W;

				pt[3].y = ezWnd->Height / 2;

				pt[4].y = ezWnd->Height / 2 - SRLBTN_ARR_W;

				pt[5].y = ezWnd->Height / 2 + SRLBTN_ARR_H - SRLBTN_ARR_W;

				pt[6].y = ezWnd->Height / 2 + SRLBTN_ARR_H;
			}


		}
		else
		{
			//ˮƽ

			pt[0].y = ezWnd->Height / 2;
			pt[1].y = ezWnd->Height / 2 + SRLBTN_ARR_W;
			pt[2].y = ezWnd->Height / 2 + SRLBTN_ARR_W;
			pt[3].y = ezWnd->Height / 2;
			pt[4].y = ezWnd->Height / 2 - SRLBTN_ARR_W;
			pt[5].y = ezWnd->Height / 2 - SRLBTN_ARR_W;
			pt[6].y = ezWnd->Height / 2;
			if (ezWnd->ezID == 0)
			{

				pt[0].x = ezWnd->Width / 2 - SRLBTN_ARR_H;

				pt[1].x = ezWnd->Width / 2 - SRLBTN_ARR_H + SRLBTN_ARR_W;

				pt[2].x = ezWnd->Width / 2 + SRLBTN_ARR_W;

				pt[3].x = ezWnd->Width / 2;

				pt[4].x = ezWnd->Width / 2 + SRLBTN_ARR_W;

				pt[5].x = ezWnd->Width / 2 - SRLBTN_ARR_H + SRLBTN_ARR_W;

				pt[6].x = ezWnd->Width / 2 - SRLBTN_ARR_H;
			}
			else
			{

				pt[0].x = ezWnd->Width / 2 + SRLBTN_ARR_H;

				pt[1].x = ezWnd->Width / 2 + SRLBTN_ARR_H - SRLBTN_ARR_W;

				pt[2].x = ezWnd->Width / 2 - SRLBTN_ARR_W;

				pt[3].x = ezWnd->Width / 2;

				pt[4].x = ezWnd->Width / 2 - SRLBTN_ARR_W;

				pt[5].x = ezWnd->Width / 2 + SRLBTN_ARR_H - SRLBTN_ARR_W;

				pt[6].x = ezWnd->Width / 2 + SRLBTN_ARR_H;
			}

		}

		Polygon((HDC)wParam, &pt, 6);


		if ((((ezWnd->Extend))->MouseHold) == FALSE)
		{
			DeleteObject(hPen);
			DeleteObject(hBrush);
		}
		return 0;

	case EZWM_TRANSDRAW:
		//����͸������
		SelectObject(wParam, hBrush = CreateSolidBrush(ezWnd->Extend->BackGroundColor));
		PatBlt(wParam, 0, 0, ezWnd->Width, ezWnd->Height, PATCOPY);
		DeleteObject(hBrush);
		return 0;

	case EZWM_TIMER:
		//��Ӵ���
		if (ezWnd->Extend->iExtend[0] < 3)
		{
			ezWnd->Extend->iExtend[0]++;
		}
		else
		{
			EZSendMessage(ezWnd->ezParent, EZWM_COMMAND, 0, ezWnd);
		}

		return 0;

	case EZWM_DESTROY:
		free(ezWnd->Extend);
		return 0;

	}
	return EZStyle_DefaultProc(ezWnd, message, wParam, lParam);
}

EZWNDPROC EZStyle_Scroll_BarProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	HBRUSH hBrush;
	switch (message)
	{
	case EZWM_CREATE:
		//����Ҳ������չ������
		ezWnd->Extend = malloc(sizeof(EZSE));

		//����
		//((pEZSE)ezWnd->Extend)->BackGroundColor = RGB(70, 70, 70);
		//((pEZSE)ezWnd->Extend)->ForeGroundColor = RGB(70, 70, 70);

		((pEZSE)ezWnd->Extend)->MouseHold = FALSE;

		((pEZSE)ezWnd->Extend)->hFont = NULL;
		((pEZSE)ezWnd->Extend)->IsFontUserControl = -1;

		ezWnd->Extend->Title = NULL;//���������ʱ��ͻ����������ˣ�����freeһ��Ұָ�롣

									//MD��������ʽ���ڣ���ð�ģ�
		ezWnd->IsStyleWindow = FALSE;
		ezWnd->EZStyle = NULL;



		SetEZWndTransparent(ezWnd, 50);
		return 0;

	case EZWM_TRANSDRAW:
		hBrush = CreateSolidBrush(RGB(36, 36, 36));

		SelectObject(wParam, hBrush);
		PatBlt(wParam, 0, 0, ezWnd->Width, ezWnd->Height, PATCOPY);

		DeleteObject(hBrush);
		return 0;
	case EZWM_LBUTTONDOWN:
		if (ezWnd->Extend->MouseHold == FALSE)
		{
			ezWnd->Extend->MouseHold = TRUE;

			EZCaptureMouse(ezWnd);

			//��¼��갴�µ�λ��
			ezWnd->Extend->iExtend[0] = GET_X_LPARAM(lParam);
			ezWnd->Extend->iExtend[1] = GET_Y_LPARAM(lParam);

			SetEZWndTransparent(ezWnd, 180);
			EZRepaint(ezWnd, NULL);
		}
		return 0;
	case EZWM_LBUTTONUP:
		if (ezWnd->Extend->MouseHold == TRUE)
		{
			ezWnd->Extend->MouseHold = FALSE;
			EZReleaseMouse(ezWnd);
			SetEZWndTransparent(ezWnd, 95);
			EZRepaint(ezWnd, NULL);
		}

	case EZWM_MOUSEMOVE:
		if (ezWnd->Extend->MouseHold == TRUE)
		{
			//�����ˣ�����һ��������.
			//������ƶ���������߸�����
			EZSendMessage(ezWnd->ezParent, EZWM_COMMAND,
				MAKELPARAM(GET_X_LPARAM(lParam) - ezWnd->Extend->iExtend[0], GET_Y_LPARAM(lParam) - ezWnd->Extend->iExtend[1]),
				ezWnd);
			EZRepaint(ezWnd, NULL);
		}
		return 0;
	case EZWM_MOUSECOME:
		SetEZWndTransparent(ezWnd, 95);
		EZRepaint(ezWnd, NULL);
		return 0;
	case EZWM_MOUSELEAVE:
		/*if (ezWnd->Extend->MouseHold == TRUE)
		{
		EZReleaseMouse(ezWnd);
		}*/

		SetEZWndTransparent(ezWnd, 50);
		EZRepaint(ezWnd, NULL);
		return 0;
	case EZWM_RELEASEMOUSE:
		(((ezWnd->Extend))->MouseHold) = FALSE;
		return 0;

	case EZWM_DESTROY:
		free(ezWnd->Extend);
		return 0;
	}
}



EZWNDPROC EZStyle_DefaultProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case EZWM_SETCOLOR:
	{
		if (ezWnd->Extend)
		{
			ezWnd->Extend->BackGroundColor = (COLORREF)wParam;
			ezWnd->Extend->ForeGroundColor = (COLORREF)lParam;

			EZRepaint(ezWnd, NULL);

		}
	}
	return 0;
	case EZWM_SETFONT:
	{

		if (((pEZSE)ezWnd->Extend)->hFont)
		{
			switch (((pEZSE)ezWnd->Extend)->IsFontUserControl)
			{
			case 1:
			case 2:
				DeleteObject(((pEZSE)ezWnd->Extend)->hFont);
				break;
			}
			((pEZSE)ezWnd->Extend)->hFont = NULL;
		}

		if (wParam)
		{
			//�û��ύ��HFONT���û������ͷš�
			((pEZSE)ezWnd->Extend)->hFont = (HFONT)wParam;
			((pEZSE)ezWnd->Extend)->IsFontUserControl = 0;
		}
		else if (lParam)
		{
			//�û��ύ��LOGFONT�����Ǵ��������١�
			((pEZSE)ezWnd->Extend)->hFont = CreateFontIndirect(lParam);
			((pEZSE)ezWnd->Extend)->IsFontUserControl = 1;
		}
		else
		{
			//��Ϊ�գ�ʹ��Ĭ�����塣
			LOGFONT lf;
			lf.lfHeight = 19;
			lf.lfWidth = 0;
			lf.lfEscapement = 0;
			lf.lfOrientation = 0;
			lf.lfWeight = 0;//1
			lf.lfItalic = 0;//1
			lf.lfUnderline = 0;//1
			lf.lfStrikeOut = 0;//1
			lf.lfCharSet = DEFAULT_CHARSET;
			lf.lfOutPrecision = 0;
			lf.lfClipPrecision = 0;
			lf.lfQuality = 0;
			lf.lfPitchAndFamily = 0;
			lstrcpy(lf.lfFaceName, TEXT("΢���ź�"));
			((pEZSE)ezWnd->Extend)->hFont = CreateFontIndirect(&lf);
			((pEZSE)ezWnd->Extend)->IsFontUserControl = 2;
		}
		return 0;
	}

	case EZWM_SETTEXT:
	{
		int TitleLen;
		if (lParam == 0)
		{
			TitleLen = lstrlen(wParam);
		}
		else
		{
			TitleLen = lParam;
		}

		if ((TitleLen) > MAX_TEXT - 2)
		{
			return FALSE;
		}
		if (ezWnd->Extend)
		{
			if (((pEZSE)(ezWnd->Extend))->Title)
			{
				free(((pEZSE)(ezWnd->Extend))->Title);
			}
#ifdef UNICODE
			((pEZSE)ezWnd->Extend)->Title = malloc(TitleLen * 2 + 4);
			ezWnd->Extend->TitleLen = TitleLen;
			lstrcpyn(((pEZSE)ezWnd->Extend)->Title, wParam, TitleLen + 1);
			((pEZSE)ezWnd->Extend)->Title[TitleLen * 2 + 2] = '\0';
			((pEZSE)ezWnd->Extend)->Title[TitleLen * 2 + 3] = '\0';
#else
			((pEZSE)ezWnd->Extend)->Title = (PBYTE)malloc(TitleLen + 2);
			strcpy_s((char *)((pEZSE)ezWnd->Extend)->Title, TitleLen + 2, wParam);
			((pEZSE)ezWnd->Extend)->Title[TitleLen] = '\0';
			((pEZSE)ezWnd->Extend)->Title[TitleLen + 1] = '\0';
#endif

			return TRUE;

		}

	}
	break;
	case EZWM_DESTROY:
	{
		//����ϵ������
		if (ezWnd->Extend)
		{
			//��������
			if (((pEZSE)ezWnd->Extend)->hFont)
			{
				switch (((pEZSE)ezWnd->Extend)->IsFontUserControl)
				{
				case 1:
				case 2:
					DeleteObject(((pEZSE)ezWnd->Extend)->hFont);
					break;
				}
				((pEZSE)ezWnd->Extend)->hFont = NULL;
			}

			//�������
			if (((pEZSE)ezWnd->Extend)->Title)
			{
				free(((pEZSE)(ezWnd->Extend))->Title);
			}
		}
		return 0;
	}

	case EZWM_SETTEXTALIGN:
	{
		ezWnd->Extend->TextAlign = wParam;
		return 0;
	}

	}
	return 0;
}




EZWNDPROC EZStyle_EditProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	int iMove, LastMove;
	int iMaxLen;
	int LineCount;
	int xCount, yCount;
	SIZE size;
	TCHAR *Text;

	switch (message)
	{
	case EZWM_CREATE:
		//�����Ӵ��ڡ�
		//0����չ�����༭����
		//1����չ����ֱ������
		//2��ˮƽ������
		ezWnd->Extend->iExtend[0] = 0;
		ezWnd->Extend->iExtend[1] = 0;
		ezWnd->Extend->iExtend[2] = 0;
		ezWnd->Extend->hExtend[0] = CreateEZWindow(ezWnd, 0, 0, 0, 0, EZStyle_Edit_InputProc);//���༭����
		ezWnd->Extend->hExtend[1] = CreateEZStyleWindow(ezWnd, TEXT(""), EZS_CHILD | EZS_CHILD_VSCROLL, 0, 0, 0, 0);
		ezWnd->Extend->hExtend[2] = CreateEZStyleWindow(ezWnd, TEXT(""), EZS_CHILD | EZS_CHILD_HSCROLL, 0, 0, 0, 0);

		ezWnd->Extend->iExtend[3] = -1;//������
		ezWnd->Extend->BackGroundColor = RGB(255, 255, 255);
		SetEZWndTransparent(ezWnd, 0);

		return 0;

	case EZWM_SETMAXTEXT:
		ezWnd->Extend->iExtend[3] = wParam;
		return 0;

	case EZWM_SIZE:
	{
		MoveEZWindow(ezWnd->Extend->hExtend[0],
			0, 0,
			CHK_ALT_STYLE(ezWnd->EZStyle, EZS_VSCROLL) ? ezWnd->Width - 15 : ezWnd->Width,
			CHK_ALT_STYLE(ezWnd->EZStyle, EZS_HSCROLL) ? ezWnd->Height - 15 : ezWnd->Height,
			0);
		MoveEZWindow(ezWnd->Extend->hExtend[1],//��ֱ��V
			ezWnd->Width - 15, 0,
			CHK_ALT_STYLE(ezWnd->EZStyle, EZS_VSCROLL) ? (15) : (0),
			CHK_ALT_STYLE(ezWnd->EZStyle, EZS_HSCROLL) ? (ezWnd->Height - 15) : (ezWnd->Height)
			, 0);
		MoveEZWindow(ezWnd->Extend->hExtend[2],//ˮƽ��H
			0, ezWnd->Height - 15,
			CHK_ALT_STYLE(ezWnd->EZStyle, EZS_VSCROLL) ? (ezWnd->Width - 15) : (ezWnd->Width),
			CHK_ALT_STYLE(ezWnd->EZStyle, EZS_HSCROLL) ? (15) : (0)
			, 0);
		return 0;
	}

	case EZWM_SETFONT:
		//�ٺ٣�ɧ������ֱ�ӵݽ���Ĭ�ϴ���
		EZStyle_DefaultProc(ezWnd, message, wParam, lParam);


		//��������SETTEXT�Ĳ���
	case EZWM_SETTEXT:
		//������������ô�С��Ȼ�����ύ��Ĭ�ϴ���
	{

		if (message == EZWM_SETTEXT)
		{
			Text = (TCHAR *)wParam;
			if (lParam == 0)
			{
				iMaxLen = lstrlen(Text);
			}
			else
			{
				iMaxLen = lParam;
			}

		}
		else
		{//EZWM_SETFONT
			Text = ezWnd->Extend->Title;
			iMaxLen = ezWnd->Extend->TitleLen;
		}

		LastMove = 0;
		xCount = yCount = 0;
		LineCount = 0;

		if (ezWnd->Extend->hFont)
		{
			SelectObject(ezWnd->hdc, ezWnd->Extend->hFont);
		}


		TEXTMETRIC tm;
		GetTextMetrics(ezWnd->hdc, &tm);

		for (iMove = 0; iMove <= iMaxLen;)
		{

			if (Text[iMove] == '\0')
			{
				//���Ƶ�ǰ�У����˳���
				GetTextExtentPoint32(ezWnd->hdc, Text + LastMove, iMove - LastMove, &size);
				yCount += tm.tmHeight;
				xCount = max(xCount, size.cx);
				LineCount++;

				break;
			}
			else if (Text[iMove] == '\r' && Text[iMove + 1] == '\n')
			{
				//windows���б�ǣ����Ƶ�ǰ�У����¿�ʼ��
				GetTextExtentPoint32(ezWnd->hdc, Text + LastMove, iMove - LastMove, &size);
				yCount += tm.tmHeight;
				xCount = max(xCount, size.cx);
				LineCount++;

				LastMove = iMove + 2;
				iMove++;

			}
			else if (Text[iMove] == '\n')
			{
				//Linux���б�ǣ����Ƶ�ǰ�С�
				GetTextExtentPoint32(ezWnd->hdc, Text + LastMove, iMove - LastMove, &size);
				yCount += tm.tmHeight;
				xCount = max(xCount, size.cx);
				LineCount++;

				LastMove = iMove + 1;
			}
			iMove++;//����for���������Ҹ��....�ŵ�����Ͳ�����
		}

		//���ù���		MoveEZWindow(ezWnd->Extend->hExtend[3], 0, 0, max(ezWnd->Extend->hExtend[0]->Width, xCount), max(ezWnd->Extend->hExtend[0]->Height, yCount), 0);





		EZRepaint(ezWnd, 0);
		EZSendMessage(ezWnd->Extend->hExtend[1], EZWM_SETSCROLLRANGE, max(ezWnd->Extend->hExtend[0]->Height, yCount), 0);
		EZSendMessage(ezWnd->Extend->hExtend[2], EZWM_SETSCROLLRANGE, max(ezWnd->Extend->hExtend[0]->Width, xCount), 0);


		if (ezWnd->Extend->iExtend[1] > ezWnd->Extend->hExtend[0]->Height)
		{
			EZSendMessage(ezWnd->Extend->hExtend[1], EZWM_SETSCROLLPOS, ezWnd->Extend->iExtend[1] - ezWnd->Extend->hExtend[0]->Height,
				ezWnd->Extend->iExtend[1]);
		}
		else
		{
			EZSendMessage(ezWnd->Extend->hExtend[1], EZWM_SETSCROLLPOS, 0,
				ezWnd->Extend->hExtend[0]->Height);
		}

		if (ezWnd->Extend->iExtend[0] > ezWnd->Extend->hExtend[0]->Width)
		{
			EZSendMessage(ezWnd->Extend->hExtend[2], EZWM_SETSCROLLPOS, ezWnd->Extend->iExtend[0] - ezWnd->Extend->hExtend[0]->Width,
				ezWnd->Extend->iExtend[0]);
		}
		else
		{
			EZSendMessage(ezWnd->Extend->hExtend[2], EZWM_SETSCROLLPOS, 0,
				ezWnd->Extend->hExtend[0]->Width);
		}


		MoveEZWindow(ezWnd->Extend->hExtend[3], 0, 0, max(ezWnd->Extend->hExtend[0]->Width, xCount), max(ezWnd->Extend->hExtend[0]->Height, yCount), 0);


		if (message == EZWM_SETFONT)
		{
			//�Ѿ���Ĭ�ϴ����ύ���ˣ���ǰ�档
			return 0;
		}
		//else,EZWM_SETTEXT���ύ��Ĭ�ϴ���
		break;
	}
	break;

	case EZWM_SCROLLPOSCHANGE:
		//wParam����ƫ��������
		if (lParam == ezWnd->Extend->hExtend[1])//��ֱ
		{
			ezWnd->Extend->hExtend[0]->ScrollY = -(int)(wParam);
		}
		else if (lParam == ezWnd->Extend->hExtend[2])//ˮƽ
		{
			ezWnd->Extend->hExtend[0]->ScrollX = -((int)wParam);
		}
		EZRepaint(ezWnd, NULL);
		return 0;
	case EZWM_TRANSDRAW:
	{
		HBRUSH hBrush;
		hBrush = CreateSolidBrush(ezWnd->Extend->BackGroundColor);
		PatBlt(wParam, 0, 0, ezWnd->Width, ezWnd->Height, PATCOPY);
		DeleteObject(hBrush);
	}
	return 0;

	}
	return EZStyle_DefaultProc(ezWnd, message, wParam, lParam);
}

EZWNDPROC EZStyle_Edit_InputProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case EZWM_CREATE:
		//�����ڵ���չ�˷��ˣ��ɴ�����Ҳһ���ø����ڶ������չ��
		ezWnd->ezParent->Extend->hExtend[3] = CreateEZWindow(ezWnd, 0, 0, 0, 0, EZStyle_Edit_InputChildProc);
		return 0;
	case EZWM_SIZE:
		MoveEZWindow(ezWnd->ezParent->Extend->hExtend[3], 0, 0, ezWnd->Width, ezWnd->Height, 0);
		return 0;
	}
}

EZWNDPROC EZStyle_Edit_InputChildProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	int iMove, LastMove;
	int iMaxLen;
	int xCount, yCount;
	int CaretX, CaretY;
	SIZE size;
	TCHAR *Text;
	TEXTMETRIC tm;
	TCHAR TextBuffer[MAX_TEXT];
	switch (message)
	{
	case EZWM_CREATE:
		return 0;
	case EZWM_LBUTTONDOWN:
	{

		int LineCount;
		Text = ezWnd->ezParent->ezParent->Extend->Title;
		iMaxLen = ezWnd->ezParent->ezParent->Extend->TitleLen;

		LineCount = 0;
		LastMove = 0;
		xCount = yCount = 0;


		BOOL IsFounded;
		IsFounded = FALSE;
		if (ezWnd->ezParent->ezParent->Extend->hFont)
		{
			SelectObject(ezWnd->hdc, ezWnd->ezParent->ezParent->Extend->hFont);
		}
		GetTextMetrics(ezWnd->hdc, &tm);

		for (iMove = 0; iMove <= iMaxLen;)
		{
			if (Text[iMove] == '\0')
			{
				//��ǰ�У����˳���
				//GetTextExtentPoint32(ezWnd->hdc, Text + LastMove, iMove - LastMove, &size);
				yCount += tm.tmHeight;
				if (yCount >= GET_Y_LPARAM(lParam))
				{
					IsFounded = TRUE;
					yCount -= tm.tmHeight;
					break;
				}
				break;
			}
			else if (Text[iMove] == '\r' && Text[iMove + 1] == '\n')
			{
				//windows���б�ǣ����¿�ʼ��
				//GetTextExtentPoint32(ezWnd->hdc, Text + LastMove, iMove - LastMove, &size);
				yCount += tm.tmHeight;
				if (yCount >= GET_Y_LPARAM(lParam))
				{
					IsFounded = TRUE;
					yCount -= tm.tmHeight;
					break;
				}
				LineCount++;
				LastMove = iMove + 2;
				iMove++;

			}
			else if (Text[iMove] == '\n')
			{
				//Linux���б��
				//GetTextExtentPoint32(ezWnd->hdc, Text + LastMove, iMove - LastMove, &size);
				yCount += tm.tmHeight;
				if (yCount >= GET_Y_LPARAM(lParam))
				{
					IsFounded = TRUE;
					yCount -= tm.tmHeight;
					break;

				}
				LineCount++;
				LastMove = iMove + 1;
			}
			iMove++;//����for���������Ҹ��....�ŵ�����Ͳ�����
		}

		if (IsFounded)
		{
			//���ڵ�ǰ�У�

			CaretY = yCount;
			IsFounded = FALSE;
			int LastLen, CurrLen;
			LastLen = CurrLen = 0;
			for (iMove = LastMove; iMove <= iMaxLen;)
			{

				if ((Text[iMove] == '\0') || (Text[iMove] == '\r' && Text[iMove + 1] == '\n') || (Text[iMove] == '\n'))
				{
					//û�ҵ�������ĩβ
					ezWnd->ezParent->ezParent->Extend->iExtend[2] = iMove;
					break;
				}

				LastLen = CurrLen;
				//GetTextExtentPoint32(ezWnd->hdc, Text + LastMove, iMove - LastMove, &size);
				int CharWidth;
				GetCharWidth32(ezWnd->hdc, Text[iMove], Text[iMove], &CharWidth);
				//CurrLen = size.cx;
				CurrLen += CharWidth;


				//�õ���λ�ú���λ�õ�ƽ��λ��
				if (GET_X_LPARAM(lParam) <= (LastLen + CurrLen) / 2)
				{
					//�������������ַ���ǰ��һ��,Ҳ������LastCurr��λ��
					IsFounded = TRUE;
					ezWnd->ezParent->ezParent->Extend->iExtend[2] = iMove;
					break;

				}

				iMove++;//����for���������Ҹ��....�ŵ�����Ͳ�����
			}

			if (IsFounded)
			{
				CaretX = LastLen;
			}
			else
			{
				CaretX = CurrLen;
			}
		}
		else
		{
			//�ĵ�ĩβ
			CaretY = yCount;
		}

		ezWnd->ezParent->ezParent->Extend->iExtend[0] = CaretX;

		ezWnd->ezParent->ezParent->Extend->iExtend[1] = CaretY;

		EZHideCaret(ezWnd);
		EZSetCaretPos(ezWnd, ezWnd->ezParent->ezParent->Extend->iExtend[0], ezWnd->ezParent->ezParent->Extend->iExtend[1]);
		EZRepaint(ezWnd->ezParent->ezParent, NULL);
		EZShowCaret(ezWnd);
	}
	return 0;
	case EZWM_DRAW:
	{
		EZHideCaret(ezWnd);
		//��ʼ���ƣ�ѭ����飬�ҵ�\r\n����\n�����У������

		Text = ezWnd->ezParent->ezParent->Extend->Title;
		if (!Text)
		{
			return 0;
		}
		iMaxLen = ezWnd->ezParent->ezParent->Extend->TitleLen;
		LastMove = 0;
		xCount = yCount = 0;

		if (ezWnd->ezParent->ezParent->Extend->hFont)
		{
			SelectObject(wParam, ezWnd->ezParent->ezParent->Extend->hFont);
		}
		SetBkMode(wParam, TRANSPARENT);
		SetTextColor(wParam, ezWnd->ezParent->ezParent->Extend->ForeGroundColor);
		GetTextMetrics(wParam, &tm);
		for (iMove = 0; iMove <= iMaxLen;)
		{

			if (Text[iMove] == '\0')
			{
				//���Ƶ�ǰ�У����˳���
				TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
				//GetTextExtentPoint32(wParam, Text + LastMove, iMove - LastMove, &size);

				yCount += tm.tmHeight;
				//xCount = max(xCount, size.cx);



				break;
			}
			else if (Text[iMove] == '\r' && Text[iMove + 1] == '\n')
			{
				//windows���б�ǣ����Ƶ�ǰ�У����¿�ʼ��
				TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
				//GetTextExtentPoint32(wParam, Text + LastMove, iMove - LastMove, &size);
				yCount += tm.tmHeight;
				//xCount = max(xCount, size.cx);


				LastMove = iMove + 2;
				iMove++;

			}
			else if (Text[iMove] == '\n')
			{
				//Linux���б�ǣ����Ƶ�ǰ�С�
				TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
				//GetTextExtentPoint32(wParam, Text + LastMove, iMove - LastMove, &size);
				yCount += tm.tmHeight;
				//xCount = max(xCount, size.cx);


				LastMove = iMove + 1;
			}
			iMove++;//����for���������Ҹ��....�ŵ�����Ͳ�����
		}

	}
	EZShowCaret(ezWnd);
	//SetPixel(wParam, ezWnd->ezParent->ezParent->Extend->iExtend[0], ezWnd->ezParent->ezParent->Extend->iExtend[1], RGB(255, 0, 0));
	return 0;

	case EZWM_CHAR:

		//�������¼��ַ����⡣
		//ÿ����EZWM_CHAR������ͨ������ģ��SETTEXTʵ���޸ĵġ�����SETTEXT��û�д���λ�����⡣���統ǰ������һ���ַ��������˱߽�֮�������
		EZHideCaret(ezWnd);

		int pp_CaretPos = ezWnd->ezParent->ezParent->Extend->iExtend[2];
		TCHAR * pTitle = ezWnd->ezParent->ezParent->Extend->Title;
		if (wParam == '\b')
		{
			if (pp_CaretPos == 0)
			{
				EZShowCaret(ezWnd);
				return 0;
			}
			if (pTitle[pp_CaretPos - 1] == '\n' && pTitle[pp_CaretPos - 2] == '\r')
			{
				EZSendMessage(ezWnd, EZWM_KEYDOWN, 37, 0);//ģ�ⰴ��

				pp_CaretPos = ezWnd->ezParent->ezParent->Extend->iExtend[2];

				lstrcpyn(TextBuffer, pTitle, pp_CaretPos + 1);

				lstrcpyn(TextBuffer + pp_CaretPos,
					pTitle + pp_CaretPos + 2,
					ezWnd->ezParent->ezParent->Extend->TitleLen - pp_CaretPos - 1);
				EZSendMessage(ezWnd->ezParent->ezParent, EZWM_SETTEXT, TextBuffer, 0);
				pTitle = ezWnd->ezParent->ezParent->Extend->Title;

				EZRepaint(ezWnd->ezParent->ezParent, 0);

			}
			else
			{
				pp_CaretPos = ezWnd->ezParent->ezParent->Extend->iExtend[2];

				lstrcpyn(TextBuffer, pTitle, pp_CaretPos + 1);

				lstrcpyn(TextBuffer + pp_CaretPos - 1,
					pTitle + pp_CaretPos,
					ezWnd->ezParent->ezParent->Extend->TitleLen - pp_CaretPos + 1);
				EZSendMessage(ezWnd->ezParent->ezParent, EZWM_SETTEXT, TextBuffer, 0);
				EZSendMessage(ezWnd, EZWM_KEYDOWN, 37, 0);//ģ�ⰴ��
				pp_CaretPos = ezWnd->ezParent->ezParent->Extend->iExtend[2];
				EZRepaint(ezWnd->ezParent->ezParent, 0);
			}

			EZShowCaret(ezWnd);
			return 0;
		}
		if (wParam == '\r')
		{
			if (ezWnd->ezParent->ezParent->Extend->iExtend[3] != -1)
			{
				if (ezWnd->ezParent->ezParent->Extend->TitleLen + 2 > ezWnd->ezParent->ezParent->Extend->iExtend[3])
				{
					EZShowCaret(ezWnd);
					return 0;
				}
			}
			//���з�������\r\n
			//֧�ֶ��У�
			if (CHK_ALT_STYLE(ezWnd->ezParent->ezParent->EZStyle, EZES_SINGLELINE))
			{
				EZSendMessage(ezWnd->ezParent->ezParent->ezParent, EZWM_COMMAND, 0, ezWnd->ezParent->ezParent);
				EZShowCaret(ezWnd);
				return 0;
			}
			lstrcpyn(TextBuffer, (TCHAR *)ezWnd->ezParent->ezParent->Extend->Title, pp_CaretPos + 1);

			TextBuffer[pp_CaretPos] = wParam;

			lstrcpyn(TextBuffer + pp_CaretPos + 1,
				pTitle + pp_CaretPos,
				ezWnd->ezParent->ezParent->Extend->TitleLen - pp_CaretPos + 1);
			//ezWnd->ezParent->ezParent->Extend->iExtend[2]++;
			EZSendMessage(ezWnd->ezParent->ezParent, EZWM_SETTEXT, TextBuffer, 0);
			pTitle = ezWnd->ezParent->ezParent->Extend->Title;
			EZSendMessage(ezWnd, EZWM_KEYDOWN, 39, 0);//ģ�ⰴ��
			pp_CaretPos = ezWnd->ezParent->ezParent->Extend->iExtend[2];
			EZRepaint(ezWnd->ezParent->ezParent, 0);

			wParam = '\n';//��ȥ֮���Զ�����
		}

		if (ezWnd->ezParent->ezParent->Extend->iExtend[3] != -1)
		{
			if (ezWnd->ezParent->ezParent->Extend->TitleLen + 1 > ezWnd->ezParent->ezParent->Extend->iExtend[3])
			{
				EZShowCaret(ezWnd);
				return 0;
			}
		}

		lstrcpyn(TextBuffer, (TCHAR *)ezWnd->ezParent->ezParent->Extend->Title, ezWnd->ezParent->ezParent->Extend->iExtend[2] + 1);

		TextBuffer[pp_CaretPos] = wParam;

		lstrcpyn(TextBuffer + pp_CaretPos + 1,
			pTitle + pp_CaretPos,
			ezWnd->ezParent->ezParent->Extend->TitleLen - pp_CaretPos + 1);
		//ezWnd->ezParent->ezParent->Extend->iExtend[2]++;
		EZSendMessage(ezWnd->ezParent->ezParent, EZWM_SETTEXT, TextBuffer, 0);
		//	pTitle = ezWnd->ezParent->ezParent->Extend->Title;
		EZSendMessage(ezWnd, EZWM_KEYDOWN, 39, 0);//ģ�ⰴ��
												  //pp_CaretPos = ezWnd->ezParent->ezParent->Extend->iExtend[2];
		EZRepaint(ezWnd->ezParent->ezParent, 0);
		EZShowCaret(ezWnd);
		return 0;
	case EZWM_KEYDOWN:


		Text = ezWnd->ezParent->ezParent->Extend->Title;
		iMaxLen = ezWnd->ezParent->ezParent->Extend->TitleLen;
		LastMove = 0;
		xCount = yCount = 0;
		if (ezWnd->ezParent->ezParent->Extend->hFont)
		{
			SelectObject(ezWnd->hdc, ezWnd->ezParent->ezParent->Extend->hFont);
		}

		GetTextMetrics(ezWnd->hdc, &tm);

		if (wParam == 37 || wParam == 39)
		{
			if (wParam == 37)
			{
				//��
				//����ǰ���ǲ���ͷ�ˣ����� '\n' �� "\r\n"
				if (ezWnd->ezParent->ezParent->Extend->iExtend[2] == 0)
				{
					//���ˣ���ͷ�ˣ�����Ҫ�κβ���
					return 0;
				}

				//�ǲ��ǻ��У�
				if (Text[ezWnd->ezParent->ezParent->Extend->iExtend[2] - 1] == '\n')
				{
					//�����ֻ��У�
					if (Text[ezWnd->ezParent->ezParent->Extend->iExtend[2] - 2] == '\r')
					{
						ezWnd->ezParent->ezParent->Extend->iExtend[2]--;//����-1
					}
				}
				//�����ǲ��ǻ��У���Ҫ-1.
				ezWnd->ezParent->ezParent->Extend->iExtend[2]--;
			}
			else if (wParam == 39)
			{
				//��

				//���������ǲ��ǵ�β�ˣ�����'\n' �� "\r\n"
				if (ezWnd->ezParent->ezParent->Extend->iExtend[2] == ezWnd->ezParent->ezParent->Extend->TitleLen)
				{
					//���ˣ���β�ˣ�����Ҫ�κβ���
					return 0;
				}
				//�ǲ��ǻ��У�
				if (Text[ezWnd->ezParent->ezParent->Extend->iExtend[2]] == '\r')
				{
					if (Text[ezWnd->ezParent->ezParent->Extend->iExtend[2] + 1] == '\n')
					{
						ezWnd->ezParent->ezParent->Extend->iExtend[2]++;//����+1
					}
				}
				ezWnd->ezParent->ezParent->Extend->iExtend[2]++;
			}

			for (iMove = 0; iMove <= iMaxLen;)
			{
				if (iMove == ezWnd->ezParent->ezParent->Extend->iExtend[2])
				{

					GetTextExtentPoint32(ezWnd->hdc, Text + LastMove, iMove - LastMove, &size);
					//yCount += size.cy;

					break;
				}
				if (Text[iMove] == '\0')
				{
					//���Ƶ�ǰ�У����˳���
					//GetTextExtentPoint32(ezWnd->hdc, Text + LastMove, iMove - LastMove, &size);
					yCount += tm.tmHeight;


					break;
				}
				else if (Text[iMove] == '\r' && Text[iMove + 1] == '\n')
				{
					//windows���б�ǣ����Ƶ�ǰ�У����¿�ʼ��
					//GetTextExtentPoint32(ezWnd->hdc, Text + LastMove, iMove - LastMove, &size);
					yCount += tm.tmHeight;

					LastMove = iMove + 2;
					iMove++;

				}
				else if (Text[iMove] == '\n')
				{
					//Linux���б�ǣ����Ƶ�ǰ�С�
					//GetTextExtentPoint32(ezWnd->hdc, Text + LastMove, iMove - LastMove, &size);

					LastMove = iMove + 1;
					yCount += tm.tmHeight;
				}
				iMove++;//����for���������Ҹ��....�ŵ�����Ͳ�����
			}

			//if (ezWnd->FocusState == 1)
			{
				ezWnd->ezParent->ezParent->Extend->iExtend[0] = size.cx;
				ezWnd->ezParent->ezParent->Extend->iExtend[1] = yCount;


				EZHideCaret(ezWnd);

				if (ezWnd->ezParent->ezParent->Extend->iExtend[0] + ezWnd->ezParent->ScrollX < 0)
				{
					ezWnd->ezParent->ScrollX -= (ezWnd->ezParent->ezParent->Extend->iExtend[0]);
				}
				if (ezWnd->ezParent->ezParent->Extend->iExtend[1] + ezWnd->ezParent->ScrollY < 0)
				{
					ezWnd->ezParent->ScrollY -= (ezWnd->ezParent->ezParent->Extend->iExtend[1]);
				}

				if ((ezWnd->ezParent->ezParent->Extend->iExtend[0] + ezWnd->ezParent->ScrollX > ezWnd->ezParent->Width) && wParam == 39)
				{
					ezWnd->ezParent->ScrollX -= (ezWnd->ezParent->ezParent->Extend->iExtend[0] + ezWnd->ezParent->ScrollX - ezWnd->ezParent->Width);
					EZSendMessage(ezWnd->ezParent->ezParent->Extend->hExtend[2],
						EZWM_SETSCROLLPOS,
						ezWnd->ezParent->ezParent->Extend->hExtend[2]->Extend->iExtend[1] - (ezWnd->ezParent->ezParent->Extend->iExtend[0] + ezWnd->ezParent->ScrollX - ezWnd->ezParent->Width),
						ezWnd->ezParent->ezParent->Extend->hExtend[2]->Extend->iExtend[2] - (ezWnd->ezParent->ezParent->Extend->iExtend[0] + ezWnd->ezParent->ScrollX - ezWnd->ezParent->Width));
				}
				if (ezWnd->ezParent->ezParent->Extend->iExtend[1] + ezWnd->ezParent->ScrollY + tm.tmHeight > ezWnd->ezParent->Height)
				{
					ezWnd->ezParent->ScrollY -= (ezWnd->ezParent->ezParent->Extend->iExtend[1] + ezWnd->ezParent->ScrollY + tm.tmHeight - ezWnd->ezParent->Height);
					//���ù�����
					/*	EZSendMessage(ezWnd->ezParent->ezParent->Extend->hExtend[1], EZWM_SETSCROLLPOS, ezWnd->ezParent->ezParent->Extend->iExtend[1] - ezWnd->ezParent->ezParent->Extend->hExtend[0]->Height + tm.tmHeight,
					ezWnd->ezParent->ezParent->Extend->iExtend[1] + tm.tmHeight);*/
				}



				EZSetCaretPos(ezWnd, ezWnd->ezParent->ezParent->Extend->iExtend[0], ezWnd->ezParent->ezParent->Extend->iExtend[1]);

				EZRepaint(ezWnd->ezParent->ezParent, 0);
				EZShowCaret(ezWnd);
			}


		}
		else if (wParam == 38 || wParam == 40)
		{
			int iLineBeginCount;
			int iLineCross;
			iLineCross = 0;
			iLineBeginCount = ezWnd->ezParent->ezParent->Extend->iExtend[2];

			if (wParam == 38)
			{
				//��û����һ�У�
				//��ǰ�ݹ飬���һ�����з�������ڶ������з�

				//����������Ǿ���....������ǻ��з�����������β������Ҫ�����ֶ���鲢�����������

				if (Text[iLineBeginCount] == '\n')
				{

					if (Text[iLineBeginCount - 1] == '\r')
					{
						iLineBeginCount--;
					}
					iLineBeginCount--;
				}

				for (; iLineBeginCount > 0; iLineBeginCount--)
				{
					if (Text[iLineBeginCount] == '\n')
					{
						if (iLineCross == 1)
						{
							//�������ˣ�����Text[iLineBeginCount]�� '\n'������Ҫ��һ
							iLineBeginCount++;
							iLineCross++;
							break;
						}

						if (Text[iLineBeginCount - 1] == '\r')
						{
							iLineBeginCount--;
						}
						iLineCross++;
					}
				}
				if (iLineCross == 0)
				{
					//û����һ��
					return 0;
				}
				CaretY = ezWnd->ezParent->ezParent->Extend->iExtend[1] - tm.tmHeight;


			}
			else if (wParam == 40)
			{
				//��û����һ�У�
				//����ݹ飬���һ�����з�������ڶ�������
				int LineCrossRec;
				for (; iLineBeginCount < iMaxLen; iLineBeginCount++)
				{
					if (Text[iLineBeginCount] == '\n')
					{
						iLineBeginCount++;
						iLineCross++;
						break;

					}
					else if ((Text[iLineBeginCount] == '\r') && (Text[iLineBeginCount + 1] == '\n'))
					{

						iLineBeginCount += 2;
						iLineCross++;
						break;
					}
				}
				if (iLineCross == 0)
				{
					//û����һ��
					return 0;
				}
				CaretY = ezWnd->ezParent->ezParent->Extend->iExtend[1] + tm.tmHeight;

			}

			//�������У��ҵ�������λ�������λ�á�

			int iFindCount;

			int LastLen, CurrLen;
			LastLen = CurrLen = 0;




			BOOL IsFounded;
			IsFounded = FALSE;

			for (iMove = iLineBeginCount; iMove <= iMaxLen;)
			{

				LastLen = CurrLen;
				GetTextExtentPoint32(ezWnd->hdc, Text + iLineBeginCount, iMove - iLineBeginCount, &size);
				CurrLen = size.cx;

				//�õ���λ�ú���λ�õ�ƽ��λ��
				if (ezWnd->ezParent->ezParent->Extend->iExtend[0] <= (LastLen + CurrLen) / 2)
				{
					//�������������ַ���ǰ��һ��,Ҳ������LastCurr��λ��
					IsFounded = TRUE;
					//��һ�����������ģ��Ǿ������ס������ף�����ֵ��Ϊ0.�����Ļ���iMove��Ӧ-1
					if (iMove == iLineBeginCount)
					{
						ezWnd->ezParent->ezParent->Extend->iExtend[2] = iMove;
						break;
					}
					ezWnd->ezParent->ezParent->Extend->iExtend[2] = iMove - 1;
					break;

				}

				if ((Text[iMove] == '\0') || (Text[iMove] == '\r' && Text[iMove + 1] == '\n') || (Text[iMove] == '\n'))
				{
					//û�ҵ�������ĩβ
					ezWnd->ezParent->ezParent->Extend->iExtend[2] = iMove;
					break;
				}


				iMove++;//����for���������Ҹ��....�ŵ�����Ͳ�����
			}

			GetTextExtentPoint32(ezWnd->hdc, Text + iLineBeginCount, iMove - iLineBeginCount, &size);
			CurrLen = size.cx;
			if (IsFounded)
			{
				CaretX = LastLen;
			}
			else
			{
				CaretX = CurrLen;
			}





			ezWnd->ezParent->ezParent->Extend->iExtend[0] = CaretX;

			ezWnd->ezParent->ezParent->Extend->iExtend[1] = CaretY;

			if (ezWnd->ezParent->ezParent->Extend->iExtend[0] + ezWnd->ezParent->ScrollX < 0)
			{
				ezWnd->ezParent->ScrollX -= (ezWnd->ezParent->ezParent->Extend->iExtend[0]);
			}
			if (ezWnd->ezParent->ezParent->Extend->iExtend[1] + ezWnd->ezParent->ScrollY < 0)
			{
				ezWnd->ezParent->ScrollY -= (ezWnd->ezParent->ezParent->Extend->iExtend[1]);
			}


			if (ezWnd->ezParent->ezParent->Extend->iExtend[0] + ezWnd->ezParent->ScrollX > ezWnd->ezParent->Width)
			{
				ezWnd->ezParent->ScrollX -= (ezWnd->ezParent->ezParent->Extend->iExtend[0] + ezWnd->ezParent->ScrollX - ezWnd->ezParent->Width);
			}
			if (ezWnd->ezParent->ezParent->Extend->iExtend[1] + ezWnd->ezParent->ScrollY + tm.tmHeight > ezWnd->ezParent->Height)
			{
				ezWnd->ezParent->ScrollY -= (ezWnd->ezParent->ezParent->Extend->iExtend[1] + ezWnd->ezParent->ScrollY + tm.tmHeight - ezWnd->ezParent->Height);
				//���ù�����
				/*EZSendMessage(ezWnd->ezParent->ezParent->Extend->hExtend[1], EZWM_SETSCROLLPOS, ezWnd->ezParent->ezParent->Extend->iExtend[1] - ezWnd->ezParent->ezParent->Extend->hExtend[0]->Height + tm.tmHeight,
				ezWnd->ezParent->ezParent->Extend->iExtend[1]+ tm.tmHeight);*/
			}

			//if (ezWnd->FocusState == 1)
			{
				//EZHideCaret(ezWnd);
				EZSetCaretPos(ezWnd, ezWnd->ezParent->ezParent->Extend->iExtend[0], ezWnd->ezParent->ezParent->Extend->iExtend[1]);
				//EZShowCaret(ezWnd);
			}
			EZRepaint(ezWnd->ezParent->ezParent, NULL);




		}


		return 0;

	case EZWM_SETFOCUS:

		if (ezWnd->ezParent->ezParent->Extend->hFont)
		{
			SelectObject(ezWnd->hdc, ezWnd->ezParent->ezParent->Extend->hFont);
		}

		GetTextMetrics(ezWnd->hdc, &tm);

		EZCreateCaret(ezWnd, NULL, 2, tm.tmHeight);

		EZShowCaret(ezWnd);
		return 0;

	case EZWM_KILLFOCUS:
		if (CHK_ALT_STYLE(ezWnd->ezParent->ezParent->EZStyle, EZES_SINGLELINE))
		{
			EZSendMessage(ezWnd->ezParent->ezParent->ezParent, EZWM_COMMAND, 0, ezWnd->ezParent->ezParent);
		}
		EZHideCaret(ezWnd);
		EZDestroyCaret();
		EZRepaint(ezWnd->ezParent->ezParent, NULL);
		return 0;


	}

}

EZWNDPROC EZStyle_OverlappedWndProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case EZWM_CREATE:
		if ((ezWnd->EZStyle & MKDW(00000000, 00000000, 00000000, 11111111)) == EZS_OVERLAPPEDWINDOW)
		{
			//����������رգ���С���Ŵ�ť������
			int BtnLen = floor(EZWND_CAP_HEIGHT * 1.618);
			ezWnd->Extend->hExtend[1] = CreateEZWindow(ezWnd, ezWnd->Width - BtnLen, 0, BtnLen, EZWND_CAP_HEIGHT, EZStyle_WndCloseProc);
			ezWnd->Extend->hExtend[2] = CreateEZWindow(ezWnd, ezWnd->Width - (BtnLen << 1), 0, BtnLen, EZWND_CAP_HEIGHT, EZStyle_WndMaxProc);
			ezWnd->Extend->hExtend[3] = CreateEZWindow(ezWnd, ezWnd->Width - BtnLen * 3, 0, BtnLen, EZWND_CAP_HEIGHT, EZStyle_WndMinProc);
			//ezWnd->DrawOnNC = 1;
			//MARGINS margins = { 0,10,0,0 };
			//DwmExtendFrameIntoClientArea(ezWnd->hParent, &margins);
			////Sleep(0);
		}
		return 0;

	case EZWM_SETFONT:

		if (lParam)
		{
			int OldHeight = ((LOGFONT *)lParam)->lfHeight;
			((LOGFONT *)lParam)->lfHeight = EZWND_CAP_HEIGHT * 0.6;
			EZStyle_DefaultProc(ezWnd, message, wParam, lParam);
			((LOGFONT *)lParam)->lfHeight = OldHeight;
		}
		else
		{
			EZStyle_DefaultProc(ezWnd, message, wParam, lParam);
		}
		return 0;
	case EZWM_DRAW:

	{
		TCHAR * pTitle;
		pTitle = ((pEZSE)ezWnd->Extend)->Title;
		if (((pEZSE)ezWnd->Extend)->hFont)
		{
			SelectObject(wParam, ((pEZSE)ezWnd->Extend)->hFont);
			TextOut(wParam, 0, 0, pTitle, lstrlen(pTitle));
		}
		else
		{
			HFONT OldFont, hFont;
			hFont = CreateFont(EZWND_CAP_HEIGHT * 0.6, 0, 0, 0, 550, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("΢���ź�"));
			OldFont = SelectObject(wParam, hFont);
			TextOut(wParam, EZWND_CAP_HEIGHT >> 1, EZWND_CAP_HEIGHT * 0.2, pTitle, lstrlen(pTitle));
			DeleteObject(SelectObject(wParam, OldFont));
		}

		return 0;
	}


	case EZWM_WINNCDRAW:
	case EZWM_WINNCACTIVATE:
	case EZWM_KILLFOCUS:
	{
		HDC hdc = GetDC(ezWnd->hParent);
		BitBlt(hdc, 0, 0, ezWnd->Width, ezWnd->Height, ezWnd->TopWndExtend->hdcTop, 0, 0, SRCCOPY);
		ReleaseDC(ezWnd->hParent, hdc);
		//������������������Ĳ�����
		return 0;
	}
	//
	//case EZWM_LBUTTONDOWN:
	//	SendMessage(ezWnd->hParent, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);//�϶����ڡ�
	////	���￪ʼ�������ĳɴ���EZWM_NCHITTEST�����EZWM_NCHITTEST�ĵݹ飿
	//	return 0;


	case EZWM_WINNCHITTEST:
	{
		////��ⷶΧ��������Ĭ�ϵ���������Ĭ��˵�ǿͻ��������ٿ��ǲ��Ǳ�����
		int iHTDef = DefWindowProc(ezWnd->hParent, WM_NCHITTEST, wParam, lParam);
		if (iHTDef != HTCLIENT)
		{
			if (iHTDef <= 0)
			{
				iHTDef--;
			}
			return iHTDef;
		}
		RECT rect;
		POINT pt;


		SetRect(&rect, 0, 0, ezWnd->Width, EZWND_CAP_HEIGHT);
		if ((ezWnd->EZStyle & 0xff) == EZS_OVERLAPPEDWINDOW)//�Ǹ�0xff��MKDW(00000000,00000000,00000000,11111111)
		{
			rect.right -= 3 * floor(EZWND_CAP_HEIGHT * 1.618);
		}
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);
		ScreenToClient(ezWnd->hParent, &pt);
		if (PtInRect(&rect, pt))
		{
			if (!IsZoomed(ezWnd->hParent))
			{
				if (pt.y <= 3)
				{
					if (pt.x <= 3)
					{
						return EZHTTOPLEFT;
					}
					return EZHTTOP;
				}
			}

			return EZHTCAPTION;
		}
		if ((ezWnd->EZStyle & 0xff) == EZS_OVERLAPPED)
		{
			return EZHTCLIENT;
		}
		return 0;
	}

	case EZWM_SIZE:

		if (ezWnd->Extend->hExtend[0])
		{
			MoveEZWindow(ezWnd->Extend->hExtend[0], 0, EZWND_CAP_HEIGHT, ezWnd->Width, ezWnd->Height - EZWND_CAP_HEIGHT, 0);
		}

		if ((ezWnd->EZStyle & MKDW(00000000, 00000000, 00000000, 11111111)) == EZS_OVERLAPPEDWINDOW)
		{
			int BtnLen = floor(EZWND_CAP_HEIGHT * 1.618);

			MoveEZWindow(ezWnd->Extend->hExtend[1], ezWnd->Width - BtnLen, 0, BtnLen, EZWND_CAP_HEIGHT, 0);
			MoveEZWindow(ezWnd->Extend->hExtend[2], ezWnd->Width - (BtnLen << 1), 0, BtnLen, EZWND_CAP_HEIGHT, 0);
			MoveEZWindow(ezWnd->Extend->hExtend[3], ezWnd->Width - BtnLen * 3, 0, BtnLen, EZWND_CAP_HEIGHT, 0);

		}
		return 0;
	case EZWM_GETMINMAXINFO:
	{
		PMINMAXINFO pMMInfo;
		RECT rect;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

		pMMInfo = lParam;


		AdjustWindowRect(&rect, GetWindowLong(ezWnd->hParent, GWL_STYLE), 0);

		pMMInfo->ptMaxPosition.x = rect.left;// -(xborder << 1);
		pMMInfo->ptMaxPosition.y = rect.top;// -(yborder << 1);
		pMMInfo->ptMaxSize.x = rect.right - rect.left;// +(xborder << 2);
		pMMInfo->ptMaxSize.y = rect.bottom - rect.top;// +(yborder << 2);
		return 1;
	}

	//case EZWM_SETFOCUS:
	//case EZWM_KILLFOCUS:
	//	EZRepaint(ezWnd, 0);
	//���������������Ӵ����յ����أ������϶����С��޸������EZWM_SETFOCUS  EZWM_KILLFOCUS

	//return 0;


	case EZWM_WINNCCALCSIZE:
	{
		if (wParam)
		{

			if (IsZoomed(ezWnd->hParent))
			{
				return 0;
			}
			else
			{

				NCCALCSIZE_PARAMS * NCCSParam;

				NCCSParam = (NCCALCSIZE_PARAMS *)lParam;
				//RECT rect = { 0};

				AdjustWindowRect(&(NCCSParam->rgrc[0]), GetWindowLong(ezWnd->hParent, GWL_STYLE), 0);

				NCCSParam->rgrc[0].left += 4;
				NCCSParam->rgrc[0].top += 1;//�ڱ�������������һЩ���������ټ�һЩ
				NCCSParam->rgrc[0].right -= 4;
				NCCSParam->rgrc[0].bottom -= 4;

				return 0;
			}

		}
		//	InvalidateRect(hwnd, 0, 0);
		return 1;
	}

	case EZWM_CLOSE:
		return EZSendMessage(ezWnd->Extend->hExtend[0], EZWM_CLOSE, wParam, lParam);



	}
	return EZStyle_DefaultProc(ezWnd, message, wParam, lParam);
}


EZWNDPROC EZStyle_WndMinProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	struct EZWndMinBtnInfo
	{
		BOOL MouseHold;
		int IDTimer;
	}*pInfo;

	switch (message)
	{
	case EZWM_CREATE:
		ezWnd->Extend = malloc(sizeof(struct EZWndMinBtnInfo));
		pInfo = ezWnd->Extend;
		pInfo->IDTimer = -1;
		pInfo->MouseHold = 0;
		ezWnd->Transparent = 0;
		return 0;
	case EZWM_MOUSECOME:

		pInfo = ezWnd->Extend;
		if (pInfo->IDTimer == -1)
		{
			pInfo->IDTimer = SetEZTimer(ezWnd, 32);
		}

		return 0;

	case EZWM_MOUSELEAVE:
		pInfo = ezWnd->Extend;
		if (pInfo->IDTimer == -1)
		{
			pInfo->IDTimer = SetEZTimer(ezWnd, 32);
		}
		return 0;

	case EZWM_LBUTTONDOWN:
		pInfo = ezWnd->Extend;
		if (pInfo->MouseHold == 0)
		{
			pInfo->MouseHold = 1;
			EZCaptureMouse(ezWnd);
		}
		return 0;

	case EZWM_LBUTTONUP:
		pInfo = ezWnd->Extend;
		if (pInfo->MouseHold == 0)
		{
			//����
		}
		else
		{
			//���Ϳ����ź�
			ShowWindow(ezWnd->hParent, SW_MINIMIZE);
			pInfo->MouseHold = 0;
			EZReleaseMouse(ezWnd);
		}
		return 0;
	case EZWM_RELEASEMOUSE:
		pInfo = ezWnd->Extend;
		pInfo->MouseHold = FALSE;
		return 0;
	case EZWM_TIMER:
		pInfo = ezWnd->Extend;
		if (ezWnd->MouseOn)
		{
			//͸���ȼ���

			if ((int)(ezWnd->Transparent) + 12 >= 48)
			{
				ezWnd->Transparent = 48;
				KillEZTimer(ezWnd, pInfo->IDTimer);
				pInfo->IDTimer = -1;
			}
			else
			{
				ezWnd->Transparent += 12;
			}
		}
		else
		{

			if ((int)(ezWnd->Transparent) - 6 <= 0)
			{
				ezWnd->Transparent = 0;
				KillEZTimer(ezWnd, pInfo->IDTimer);
				pInfo->IDTimer = -1;
			}
			else
			{
				ezWnd->Transparent -= 6;
			}
		}

		EZRepaint(ezWnd, 0);
		return 0;

	case EZWM_TRANSDRAW:
		PatBlt(wParam, 0, 0, ezWnd->Width, ezWnd->Height, BLACKNESS);
		return 0;

	case EZWM_DRAW:
		MoveToEx(wParam, ezWnd->Width * (1 - 0.618), ezWnd->Height / 2, NULL);
		LineTo(wParam, ezWnd->Width * 0.618, ezWnd->Height / 2);
		return 0;
	case EZWM_DESTROY:
		free(ezWnd->Extend);
		return 0;
	}
}

EZWNDPROC EZStyle_WndMaxProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	struct EZWndMaxBtnInfo
	{
		BOOL MouseHold;
		int IDTimer;
	}*pInfo;

	switch (message)
	{
	case EZWM_CREATE:
		ezWnd->Extend = malloc(sizeof(struct EZWndMaxBtnInfo));
		pInfo = ezWnd->Extend;
		pInfo->IDTimer = -1;
		pInfo->MouseHold = 0;
		ezWnd->Transparent = 0;
		return 0;
	case EZWM_MOUSECOME:

		pInfo = ezWnd->Extend;
		if (pInfo->IDTimer == -1)
		{
			pInfo->IDTimer = SetEZTimer(ezWnd, 32);
		}

		return 0;

	case EZWM_MOUSELEAVE:
		pInfo = ezWnd->Extend;
		if (pInfo->IDTimer == -1)
		{
			pInfo->IDTimer = SetEZTimer(ezWnd, 32);
		}
		return 0;

	case EZWM_LBUTTONDOWN:
		pInfo = ezWnd->Extend;
		if (pInfo->MouseHold == 0)
		{
			pInfo->MouseHold = 1;
			EZCaptureMouse(ezWnd);
		}
		return 0;

	case EZWM_LBUTTONUP:
		pInfo = ezWnd->Extend;
		if (pInfo->MouseHold == 0)
		{
			//����
		}
		else
		{
			//���Ϳ����ź�
			if (IsZoomed(ezWnd->hParent))
			{
				ShowWindow(ezWnd->hParent, SW_RESTORE);
			}
			else
			{
				ShowWindow(ezWnd->hParent, SW_MAXIMIZE);
			}
			pInfo->MouseHold = 0;
			EZReleaseMouse(ezWnd);
		}
		return 0;
	case EZWM_RELEASEMOUSE:
		pInfo = ezWnd->Extend;
		pInfo->MouseHold = FALSE;
		return 0;
	case EZWM_TIMER:
		pInfo = ezWnd->Extend;
		if (ezWnd->MouseOn)
		{
			//͸���ȼ���

			if ((int)(ezWnd->Transparent) + 12 >= 48)
			{
				ezWnd->Transparent = 48;
				KillEZTimer(ezWnd, pInfo->IDTimer);
				pInfo->IDTimer = -1;
			}
			else
			{
				ezWnd->Transparent += 12;
			}
		}
		else
		{

			if ((int)(ezWnd->Transparent) - 6 <= 0)
			{
				ezWnd->Transparent = 0;
				KillEZTimer(ezWnd, pInfo->IDTimer);
				pInfo->IDTimer = -1;
			}
			else
			{
				ezWnd->Transparent -= 6;
			}
		}

		EZRepaint(ezWnd, 0);
		return 0;

	case EZWM_TRANSDRAW:
		PatBlt(wParam, 0, 0, ezWnd->Width, ezWnd->Height, BLACKNESS);
		return 0;

	case EZWM_DRAW:

	{
		double Length = floor(ezWnd->Width *((1 - 0.618) / 2));
		int LeftPt, RightPt, TopPt, BottomPt;
		LeftPt = floor((ezWnd->Width - Length) / 2);
		RightPt = ceil((ezWnd->Width + Length) / 2);
		TopPt = floor((ezWnd->Height - Length) / 2);
		BottomPt = ceil((ezWnd->Height + Length) / 2);

		SelectObject(wParam, GetStockObject(NULL_BRUSH));
		SelectObject(wParam, GetStockObject(BLACK_PEN));
		Rectangle(wParam, LeftPt, TopPt, RightPt, BottomPt);

		return 0;
	}

	case EZWM_DESTROY:
		free(ezWnd->Extend);
		return 0;
	}
}


EZWNDPROC EZStyle_WndCloseProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	struct EZWndCloseBtnInfo
	{
		BOOL MouseHold;
		int IDTimer;
		HBRUSH hBrush;
		HDC hMemDC;
	}*pInfo;

	switch (message)
	{
	case EZWM_CREATE:
		ezWnd->Extend = malloc(sizeof(struct EZWndCloseBtnInfo));
		pInfo = ezWnd->Extend;
		pInfo->hBrush = CreateSolidBrush(RGB(200, 0, 0));
		pInfo->IDTimer = -1;
		pInfo->MouseHold = 0;
		pInfo->hMemDC = GetMemDC(ezWnd->hdc, ezWnd->Width * STRETCH, ezWnd->Height* STRETCH);
		ezWnd->Transparent = 0;
		return 0;
	case EZWM_SIZE:
		pInfo = ezWnd->Extend;

		DeleteMemDC(pInfo->hMemDC);
		pInfo->hMemDC = GetMemDC(ezWnd->hdc, ezWnd->Width* STRETCH, ezWnd->Height* STRETCH);
		return 0;
	case EZWM_MOUSECOME:

		pInfo = ezWnd->Extend;
		if (pInfo->IDTimer == -1)
		{
			pInfo->IDTimer = SetEZTimer(ezWnd, 32);
		}

		return 0;

	case EZWM_MOUSELEAVE:
		pInfo = ezWnd->Extend;
		if (pInfo->IDTimer == -1)
		{
			pInfo->IDTimer = SetEZTimer(ezWnd, 32);
		}
		return 0;

	case EZWM_LBUTTONDOWN:
		pInfo = ezWnd->Extend;
		if (pInfo->MouseHold == 0)
		{
			pInfo->MouseHold = 1;
			EZCaptureMouse(ezWnd);
		}
		return 0;

	case EZWM_LBUTTONUP:
		pInfo = ezWnd->Extend;
		if (pInfo->MouseHold == 0)
		{
			//����
		}
		else
		{
			//���Ϳ����ź�
			if (EZSendMessage(ezWnd->ezParent->Extend->hExtend[0], EZWM_CLOSE, 0, 0) == EZCLOSE_WINDOW)
			{
				DestroyEZWindow(ezWnd->ezParent);
			}
			pInfo->MouseHold = 0;
			EZReleaseMouse(ezWnd);
		}

		return 0;
	case EZWM_RELEASEMOUSE:
		pInfo = ezWnd->Extend;
		pInfo->MouseHold = FALSE;
		return 0;

	case EZWM_TIMER:
		pInfo = ezWnd->Extend;
		if (ezWnd->MouseOn)
		{
			if ((int)(ezWnd->Transparent) + 64 >= 255)
			{
				ezWnd->Transparent = 255;
				KillEZTimer(ezWnd, pInfo->IDTimer);
				pInfo->IDTimer = -1;
			}
			else
			{
				ezWnd->Transparent += 64;
			}
		}
		else
		{

			if ((int)(ezWnd->Transparent) - 32 <= 0)
			{
				ezWnd->Transparent = 0;
				KillEZTimer(ezWnd, pInfo->IDTimer);
				pInfo->IDTimer = -1;
			}
			else
			{
				ezWnd->Transparent -= 32;
			}
		}

		EZRepaint(ezWnd, 0);
		return 0;

	case EZWM_TRANSDRAW:
		pInfo = ezWnd->Extend;
		SelectObject(wParam, pInfo->hBrush);
		PatBlt(wParam, 0, 0, ezWnd->Width, ezWnd->Height, PATCOPY);
		return 0;
	case EZWM_DRAW:
	{
		HPEN hOldPen;
		//hPen = CreatePen(PS_SOLID, STRETCH, RGB(0, 0, 0));
		pInfo = ezWnd->Extend;
		SetStretchBltMode(wParam, HALFTONE);
		SetStretchBltMode(pInfo->hMemDC, HALFTONE);
		StretchBlt(pInfo->hMemDC, 0, 0, ezWnd->Width* STRETCH, ezWnd->Height* STRETCH, wParam, 0, 0, ezWnd->Width, ezWnd->Height, SRCCOPY);

		hOldPen = SelectObject(pInfo->hMemDC, CreatePen(PS_SOLID, STRETCH, RGB(ezWnd->Transparent, ezWnd->Transparent, ezWnd->Transparent)));

		int Length = floor(ezWnd->Width *(2 * 0.618 - 1));
		int LeftPt, RightPt;
		LeftPt = ceil(ezWnd->Width* STRETCH *(1 - 0.618));
		RightPt = floor(ezWnd->Width* STRETCH *(0.618));
		MoveToEx(pInfo->hMemDC, LeftPt, STRETCH * (ezWnd->Height - Length) / 2, 0);
		LineTo(pInfo->hMemDC, RightPt, STRETCH *(ezWnd->Height + Length) / 2);

		MoveToEx(pInfo->hMemDC, RightPt, STRETCH *(ezWnd->Height - Length) / 2, 0);
		LineTo(pInfo->hMemDC, LeftPt, STRETCH *(ezWnd->Height + Length) / 2);

		DeleteObject(SelectObject(pInfo->hMemDC, hOldPen));

		StretchBlt(wParam, 0, 0, ezWnd->Width, ezWnd->Height, pInfo->hMemDC, 0, 0, ezWnd->Width* STRETCH, ezWnd->Height* STRETCH, SRCCOPY);
		return 0;
	}

	case EZWM_DESTROY:
		pInfo = ezWnd->Extend;
		DeleteObject(pInfo->hBrush);
		DeleteMemDC(pInfo->hMemDC);
		free(ezWnd->Extend);
		return 0;
	}


}
