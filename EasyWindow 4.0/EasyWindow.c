#include"EasyWindow.h"

//*******************************************************************************************
//                函数实体
//*******************************************************************************************

//初始化函数
BOOL InitEZWindow()
{
	WNDCLASS wndclass;
	//注册窗口类
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



//创建窗口函数
EZWND CreateEZParentWindowEx(DWORD EZStyle, int x, int y, int Width, int Height, DWORD WinStyle, EZWNDPROC ezWndProc, HMENU hMenu, HWND hParent)
{
	HWND hwndParent;//这是Windows句柄的父窗口
	EZWND ezwndParent;//这是和Win窗口等大的EZ窗口，返回给用户。该窗口摧毁时同时摧毁Win窗口


	if (!(ezwndParent = (EZWND)malloc(sizeof(EZWINDOW))))//申请内存并检查
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

	//我将下面所有赋值为0的语句全部注释掉了，因为这是多余的操作


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
	//ezwndParent->ezParent = NULL;//没有，空。
	ezwndParent->ezRootParent = ezwndParent;//你自己

	//ezwndParent->TopWndExtend->CptMouseWindow = NULL;//没有的，2333
	//ezwndParent->TopWndExtend->FocusWindow = NULL;
	//ezwndParent->TopWndExtend->MouseOnWnd = NULL;
	//ezwndParent->TopWndExtend->CptKbdWindow = NULL;


	ezwndParent->IsTopWindow = TRUE;//是的，是顶层窗口。

	//ezwndParent->FocusState = FALSE;

	ezwndParent->MouseMsgRecv = 1;

	ezwndParent->ShowState = 1;

	//ezwndParent->MouseOn = FALSE;

	//ezwndParent->Update = FALSE;//刚开始，肯定没有更新过。

	ezwndParent->Transparent = 255;//不透明

	ezwndParent->ezWndProc = ezWndProc;

	//ezwndParent->TopWndExtend->FocusWindow = NULL;


	//ezwndParent->IsWinWnd = 0;
	//ezwndParent->hChild = 0;


	//ezwndParent->IsStyleWindow = FALSE;

	//ezwndParent->ezID = 0;

	//大部分内容都初始化完毕了。ezwndParent被作为参数提交给WM_CREATE消息，并在期间将这个指针设置为窗口扩展
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

	if (!(ezWnd = (EZWND)malloc(sizeof(EZWINDOW))))//创建并检查内存
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

	//我将下面所有赋值为0的语句全部注释掉了，因为这是多余的操作

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
	//ezWnd->IsTopWindow = FALSE;//并不是


	//ezWnd->FocusState = 0;
	ezWnd->MouseMsgRecv = 1;
	ezWnd->ShowState = 1;

	//ezWnd->MouseOn = FALSE;

	ezWnd->Update = 1;//刚开始，肯定没有更新过。

	ezWnd->Transparent = 255;//不透明

	ezWnd->ezParent = ezParent;
	ezWnd->ezRootParent = ezParent->ezRootParent;
	ezWnd->ezWndProc = ezWndProc;

	//在EZ父窗口最后追加这个新的子窗口。
	EZAddChild(ezParent, ezWnd);

	fhdc = GetDC(hParent = ezParent->hParent);
	ezWnd->hdc = GetMemDC(fhdc, Width, Height);
	ReleaseDC(hParent, fhdc);


	//ezWnd->IsWinWnd = 0;
	//ezWnd->hChild = 0;


	//ezWnd->IsStyleWindow = FALSE;


	//ezWnd->ezID = 0;

	EZSendMessage(ezWnd, EZWM_CREATE, 0, 0);//发送创建消息

											//只发送一个移动消息
	EZSendMessage(ezWnd, EZWM_SIZE, (WPARAM)NULL, (LPARAM)MAKELPARAM(Width, Height));
	return ezWnd;

}

BOOL EZAddChild(EZWND ezParent, EZWND ezChild)
{
	EZWND ezChildLast;
	//将子窗口追加在ezParent的子窗口链表最后
	if (!IsEZWindow(ezParent->FirstChild))
	{
		ezParent->FirstChild = ezChild;
		return TRUE;
	}

	//子窗口链表第一个不为空
	ezChildLast = ezParent->FirstChild;

	while (1)//循环到链表最后
	{
		if (!IsEZWindow(ezChildLast->NextEZWnd))
		{
			ezChild->LastEZWnd = ezChildLast;
			ezChildLast->NextEZWnd = ezChild;
			return TRUE;
		}
		ezChildLast = ezChildLast->NextEZWnd;//向下移动
	}

}


EZWND CreateEZStyleWindow(EZWND ezParent, TCHAR Title[], DWORD EZStyle, int x, int y, int Width, int Height)
{
	EZWND ezWnd;
	ezWnd = 0;
	//解析属性，检测是否冲突，然后创建窗口。
	if (CHK_ALT_STYLE(EZStyle, EZS_CHILD))
	{
		//子窗口
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
		//主窗口

	}


	((pEZSE)ezWnd->Extend)->hFont = NULL;
	ezWnd->Extend->Title = 0;
	EZSendMessage(ezWnd, EZWM_SETTEXT, Title, lstrlen(Title));


	((pEZSE)ezWnd->Extend)->BackGroundColor = RGB(255, 255, 255);

	((pEZSE)ezWnd->Extend)->ForeGroundColor = RGB(0, 0, 0);

	((pEZSE)ezWnd->Extend)->MouseHold = FALSE;


	((pEZSE)ezWnd->Extend)->IsFontUserControl = -1;


	//最后再设置一遍，防止之前被防野数据机制清零。
	ezWnd->IsStyleWindow = TRUE;
	ezWnd->EZStyle = EZStyle;

	return ezWnd;
}


EZWND CreateEZStyleParentWindow(TCHAR Title[], DWORD EZStyle, int x, int y, int Width, int Height, BOOL bAdjust, EZWNDPROC ezWndProc)
{
	EZWND ezParent = 0, ezWnd = 0;
	//解析属性，检测是否冲突，然后创建窗口。
	if (CHK_ALT_STYLE(EZStyle, EZS_CHILD))
	{

	}
	else
	{
		//主窗口
		//子窗口
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


	//最后再设置一遍，防止之前被防野数据机制清零。
	ezParent->IsStyleWindow = TRUE;
	ezParent->EZStyle = EZStyle;

	EZRepaint(ezParent, 0);

	return ezWnd;
}

//销毁窗口函数
BOOL DestroyEZWindow(EZWND ezWnd)
{

	if (ezWnd->IsTopWindow)
	{
		DestroyWindow(ezWnd->hParent);//这就行了，销毁会在这里完成。这也没什么好说的，不用管链表，清理就是了。
		return TRUE;
	}



	//先保存信息，倒推，消除所有子孙窗口
	if (IsEZWindow(ezWnd->FirstChild))
	{
		DestroyEZWindowWithNext(ezWnd->FirstChild);
	}


	//把你自己清理了
	EZSendMessage(ezWnd, EZWM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);//发送销毁信息

	DeleteMemDC(ezWnd->hdc);//清理DC
	//DeleteMemDC(ezWnd->hdcCopy);//清理DC

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
	//维护一下链表？
	if (ezWnd->LastEZWnd)
	{
		//有上一个
		if (ezWnd->NextEZWnd)
		{
			//还有下一个。
			ezWnd->LastEZWnd->NextEZWnd = ezWnd->NextEZWnd;
			ezWnd->NextEZWnd->LastEZWnd = ezWnd->LastEZWnd;
			//链接好咯
		}
		else
		{
			//下一个是空的
			ezWnd->LastEZWnd->NextEZWnd = NULL;
		}
	}
	else
	{
		//没有上一个？你是顶层窗口？还是第一个？
		if (!(ezWnd->IsTopWindow))
		{
			//不是顶层啊，你是某个链的第一个
			if (ezWnd->NextEZWnd)
			{
				//还有下一个
				ezWnd->ezParent->FirstChild = ezWnd->NextEZWnd;
				ezWnd->NextEZWnd->LastEZWnd = NULL;
			}
			else
			{
				//你没下面一个了，你父窗口没儿子了。
				ezWnd->ezParent->FirstChild = NULL;
			}
		}
	}


	if ((ezWnd->ezParent->EZStyle != 0) && (!(ezWnd->ezParent->EZStyle & EZS_CHILD)) && ezWnd->ezParent->IsTopWindow)
	{
		//父窗口是样式父窗口,绑定删除
		if (ezWnd->ezParent->FirstChild)
		{

			ezWnd->ezParent->FirstChild = 0;//让父窗口误以为自己没有子窗口，就不会递归删除进入死循环了
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

	//回滚，准备清理并列窗口
	if (IsEZWindow(ezWnd->NextEZWnd))
	{
		ezChildLast = ezWnd->NextEZWnd;

		//循环到链表最后
		while (1)
		{
			if (!IsEZWindow(ezChildLast->NextEZWnd))
			{
				break;//最后，跳出。
			}
			ezChildLast = ezChildLast->NextEZWnd;
		}


		//再循环滚回链表最前（一边滚一边删）

		//为了数据，先回滚，然后通过向下的指针找回去，删了
		while (IsEZWindow(ezChildLast->LastEZWnd))//还没到头
		{

			ezChildLast = ezChildLast->LastEZWnd;//回滚

												 //这时，ezChildLast->NextEZWnd就是最后一个。DestroyEZWindow会删了该窗口和这个窗口的子窗口。
			DestroyEZWindow(ezChildLast->NextEZWnd);//删了
		}
		//滚动到头了，DestroyEZWindow会删了该窗口和这个窗口的子窗口。
		DestroyEZWindow(ezChildLast);
		return TRUE;

	}
	else
	{//你就是最后一个并列窗口。
		DestroyEZWindow(ezWnd);//删了
	}
	return TRUE;

}



//窗口设置函数
BOOL MoveEZWindow(EZWND ezWnd, int x, int y, int Width, int Height, BOOL repaint)
{
	HDC fhdc;//临时DC


	if (ezWnd->IsTopWindow)
	{
		MoveWindow(ezWnd->hParent, x, y, Width, Height, FALSE);
		//剩下的工作里面会完成的
		//TODO:真的所有工作都会完成？仔细检查

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
		//枚举所有子窗口并重设px，py
		EZResetChildPxPy(ezWnd);
	}



	//暂时清理DC，然后重新获得一个。
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

BOOL ScrollEZWindow(EZWND ezWnd, int x, int y, BOOL bAdd)//bAdd为TRUE，则累加。否则重置
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

BOOL SetEZWndTransparent(EZWND ezWnd, int Transparent)//设置EZ窗口透明度
{
	ezWnd->Transparent = Transparent;
	return TRUE;
}

BOOL EZCaptureMouse(EZWND ezWnd)
{
	//向顶层父窗口提交要捕获“老鼠”的窗口的句柄。
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
	//向顶层父窗口提交释放“老鼠”申请。

	ReleaseCapture();

	//下面两这句不需要，在Window窗口处理WM_CAPTURECHANGE之间已经处理好了
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





//与窗口绘制有关的函数
BOOL EZRedraw(EZWND ezWnd)//重绘到内存dc,不更新。
{
	//这里，我们需要先确定需要重绘的范围

	/*
	在整个大窗口体系下
	任何和该窗口有范围重复的窗口
	全！部！需！要！重绘！！！！

	所以，我们要向所有窗口，包括子窗口广播消息。任何有重复的窗口，全部需要重绘到内存
	然后进行叠加处理。

	不需要重绘到Windows父窗口。

	*/

	if (!IsEZWindow(ezWnd))
	{
		return FALSE;
	}

	for (EZWND WndNow = ezWnd; WndNow; WndNow = WndNow->ezParent)//向上推，直到父窗口
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
			//正序，所以，先处理自己。
			RECT RectDst, RectAns;

			{
				RectDst.left = cx + LastChild->x + LastChild->ezParent->ScrollX;
				RectDst.right = RectDst.left + LastChild->Width;
				RectDst.top = cy + LastChild->y + LastChild->ezParent->ScrollY;
				RectDst.bottom = RectDst.top + LastChild->Height;
			}

			if (!((IntersectRect(&RectAns, &RectSrc, &RectDst) == 0) || (LastChild->ShowState == 2)))
				//临时处理
				//if (LastChild->ShowState == 1)
			{
				/*HRGN hRgn, OldRgn;*/

				BroadcastProc(LastChild, SEZWM_REDRAW, wp, lp);//处理自己



															   /*hRgn = CreateRectRgn(
															   RectAns.left - (cx + LastChild->x + LastChild->ezParent->ScrollX),
															   RectAns.top - (cy + LastChild->y + LastChild->ezParent->ScrollY),
															   RectAns.right - (cx + LastChild->x + LastChild->ezParent->ScrollX),
															   RectAns.bottom - (cy + LastChild->y + LastChild->ezParent->ScrollY));*/

															   /*OldRgn = SelectObject(LastChild->hdc, hRgn);*/



				RedrawBroadcast(LastChild, wp, lp, cx + LastChild->x + LastChild->ezParent->ScrollX, cy + LastChild->y + LastChild->ezParent->ScrollY, RectAns);//给自己的子窗口发送该消息																															 /*	DeleteObject(SelectObject(LastChild->hdc, OldRgn));*/


				BroadcastProc(LastChild, SEZWM_COPYDC, wp, lp);//处理自己

			}


			//向下滚

		}

	}
	return TRUE;
}

BOOL EZUpdate(EZWND ezWnd, HDC hdc)//将DC更新到窗体，不重绘。第二个参数是DC，如不提供，函数将自动获取。
{
	if (!IsEZWindow(ezWnd))
	{
		return FALSE;
	}

	//只需要把这个窗口（不需要全屏BitBlt）,得到相对父窗口的位置

	////EZBroadcastToAllChild(ezWnd, TRUE, SEZWM_COPYDC, (WPARAM)NULL, (LPARAM)NULL);,


	if (!hdc)//没DC
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



	for (EZWND WndNow = ezWnd; WndNow; WndNow = WndNow->ezParent)//向上推，直到父窗口
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
	else//没DC
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

	EZSendMessage(ezWnd, EZWM_REDRAWFINISH, 0, 0);//自由映射时间到
	return 0;
}




//计时器函数
int SetEZTimer(EZWND ezWnd, int iTimeSpace)
{
	//申请计时器，在顶部窗口的计时器中找到一个空位，写入信息，返回分配的ID。
	int i;
	for (i = 0; i < MAX_EZ_TIMER; i++)
	{
		if (ezWnd->ezRootParent->TopWndExtend->Timer[i].ezWnd == NULL)
		{
			//写入信息
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
	//删除信息
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




//光标函数
BOOL EZCreateCaret(EZWND ezWnd, HBITMAP hBitmap, int nWidth, int nHeight)
{
	return CreateCaret(ezWnd->hParent, hBitmap, nWidth, nHeight);
}

BOOL EZSetCaretPos(EZWND ezWnd, int x, int y)
{
	//TODO:检测是否超出窗口显示边界。
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

//弹出对话框函数

typedef struct tagDlgMaskHookExtend
{
	BOOL bCenter;
	pEZSE OldExtend;
	EZWNDPROC OldProc;
	EZWND Mask, Dialog;

}DlgMaskHookExtend;


EZWND EZDialogBox(EZWND ezParent, int x, int y, int w, int h, DWORD Style, COLORREF MaskClr, EZWNDPROC ezWndProc)
{
	//TODO: 使用最新的消息 EZWM_COVERCHILD 来代替Mask窗口以提高性能
	EZWND Mask;
	EZWND Dialog;
	BOOL bMask, bCenter;
	bMask = Style & EZDLG_MASK;
	bCenter = Style & EZDLG_CENTER;

	//我们需要拦截一些消息，比如父窗口大小改变之类的
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
			EZSendMessage(Mask, EZWM_USER_NOTIFY, 1, 1);//打开（or打开ing）Mask	
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
			EZSendMessage(Mask, EZWM_USER_NOTIFY, 2, Dialog);//设置Dialog
		}
	}
	EZRepaint(ezParent, 0);
}


BOOL EZEndDialog(EZWND ezWnd)
{

	EZWND ezParent = ezWnd->ezParent;

	//注意，传入的是对话框句柄
	//首先还原父窗口的Extend和WndProc,接下来标记Mask渐变，（完成后自删除）鼠标消息透明，并删除生成的Dialog
	DlgMaskHookExtend * NewExt = ezParent->Extend;

	ezParent->Extend = NewExt->OldExtend;
	ezParent->ezWndProc = NewExt->OldProc;
	if (NewExt->Mask)
	{

		EZSendMessage(NewExt->Mask, EZWM_USER_NOTIFY, 1, 0);//关闭Mask
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
		//这个时候，子窗口应该删干净了。Mask的内存也该释放完了

		ezWnd->Extend = NewExt->OldExtend;//恢复成原来的Extend
										  //发送给原来的窗口过程
		ezWnd->ezWndProc = NewExt->OldProc;
		free(NewExt);
		return EZSendMessage(ezWnd, message, wParam, lParam);
	}
	if (message == EZWM_SIZE)
	{
		//处理一下
		MoveEZWindow(NewExt->Mask, 0, 0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0);
		if (NewExt->bCenter)
		{
			MoveEZWindow(NewExt->Dialog,
				(GET_X_LPARAM(lParam) - NewExt->Dialog->Width) >> 1,
				(GET_Y_LPARAM(lParam) - NewExt->Dialog->Height) >> 1, NewExt->Dialog->Width, NewExt->Dialog->Height, 0);
		}
		//EZRepaint(ezWnd, 0);

	}

	ezWnd->Extend = NewExt->OldExtend;//恢复成原来的Extend
									  //发送给原来的窗口过程
									  //如果说这条消息引发了别的消息，而窗口过程钩子没有卸下来，引发的消息还会传到这里，但是这时候Extend已经是卸下来的状态了，就会gg。考虑到EZWM_SIZE很少会引发自身被size（那不就是死循环了吗），所以这里暂时还原窗口过程
	ezWnd->ezWndProc = NewExt->OldProc;
	iRet = NewExt->OldProc(ezWnd, message, wParam, lParam);
	//还原
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
		if (wParam == 0)//设置颜色
		{
			DeleteObject(DlgMaskInfo->hBrush);
			DlgMaskInfo->hBrush = CreateSolidBrush(lParam);
			EZRepaint(ezWnd, 0);
			return 0;
		}
		if (wParam == 1)
		{
			//有关Mask开关
			//	DlgMaskInfo->MaskOpen = lParam;
			if (lParam)//打开Mask
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
			else//关闭Mask
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
			//正在关闭ing
			if (ezWnd->Transparent - 16 <= 0)
			{
				ezWnd->Transparent = 0;
				KillEZTimer(ezWnd, DlgMaskInfo->TimerID);
				DlgMaskInfo->TimerID = -1;
				EZRepaint(ezWnd->ezParent, 0);
				//自删除

				DestroyEZWindow(ezWnd);
				return 0;//避免调用EZRepaint
			}
			else
			{
				ezWnd->Transparent -= 16;
			}
		}

		EZRepaint(ezWnd->ezParent, 0);
		//临时操作
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

//内部函数
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
		ezWnd->hParent = hwnd;//设置Win父窗口

							  //HDC fhdc;
							  //其实，本应该用这个变量.但是？hdc变量现在不正闲着吗？？？干脆就用那个吧，省内存
							  //ezWnd->DrawOnNC = 0;

		hdc = GetDC(hwnd);
		ezWnd->hdc = GetMemDC(hdc, ezWnd->Width, ezWnd->Height);
		// ezWnd->hdcCopy = GetMemDC(hdc, ezWnd->Width, ezWnd->Height);
		ezWnd->TopWndExtend->hdcTop = GetMemDC(hdc, ezWnd->Width, ezWnd->Height);

		ReleaseDC(hwnd, hdc);

		EZSendMessage(ezWnd, EZWM_CREATE, 0, 0);//发送创建消息

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
		//其实，本应该用这个变量.但是？hdc变量现在不正闲着吗？？？干脆就用那个吧，省内存

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
		//老鼠在跑来跑去。
		if (!ezWnd->TopWndExtend->MouseOnWnd)
		{
			//刚刚进来

			//可以进行监测了。
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
		//上面这个不行，因为这个函数还会重复调用ReleaseCapture.

		//把上面这个函数里面的内容拷下来改一改就可以了
		if (ezWnd->TopWndExtend->CptMouseWindow)
		{
			EZSendMessage(ezWnd->TopWndExtend->CptMouseWindow, EZWM_RELEASEMOUSE, 0, 0);
			ezWnd->TopWndExtend->CptMouseWindow = NULL;

		}
		return 0;
	}

	case WM_MOUSELEAVE:
	{
		//呀，老鼠跑了~~~   广播一下，让每个子窗口看看是不是从自己那里溜出去的。
		//先检查自己。
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
		iHT = EZSendMessage(ezWnd, EZWM_WINNCHITTEST, wParam, lParam);//只有主窗口才能收到 EZWM_WINNCHITTEST 消息
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
		//直接无视（我管你呢，反正正常的话,有ERASE就有PAINT）
		return 1;
	}

	case WM_NCPAINT:
	{
		if (!EZSendMessage(ezWnd, EZWM_WINNCDRAW, wParam, lParam))
		{
			//为0，默认
			break;
		}
		return 0;
	}

	case WM_PAINT:
	{
		//重绘区域无效啦！！！
		hdc = BeginPaint(hwnd, &ps);

		//	这里用无效矩形进行了优化
		BitBlt(hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, ezWnd->TopWndExtend->hdcTop, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);
		EndPaint(hwnd, &ps);

		//好了，现在有效了。设置一下。
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
		//返回值为0，交给默认
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
		//绘制总归让人家绘制吧。。。返回值我也不想处理了于是就写成了这样

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
		//先保存信息，倒推，消除所有子孙窗口
		if (IsEZWindow(ezWnd->FirstChild))
		{
			EZWND FirstChildBuf = ezWnd->FirstChild;
			ezWnd->FirstChild = 0;
			DestroyEZWindowWithNext(FirstChildBuf);
		}
		//把你自己清理了
		EZSendMessage(ezWnd, EZWM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);//发送销毁信息
		DeleteMemDC(ezWnd->hdc);//清理DC
	   // DeleteMemDC(ezWnd->hdcCopy);//清理DC
		DeleteMemDC(ezWnd->TopWndExtend->hdcTop);

		free(ezWnd->TopWndExtend);
		//问题是这样的：当Windows开始执行摧毁窗口时，这里会递归往下删除。而客户区子窗口发现父窗口是一个样式窗口后，就又要求父窗口删除且仅删除自己，
		if (ezWnd->IsStyleWindow)
		{
			free(ezWnd->Extend);
		}

		free(ezWnd);
		SetWindowLongPtr(hwnd, 0, NULL);

		break;
	}

	case WM_KILLFOCUS:
		//通知焦点窗口，失去焦点。
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

	//在列表里找到对应的ezWnd，并转发EZWM_TIMER消息

	int i;
	pTWE TopExtend = ((EZWND)GetWindowLongPtr(hwnd, 0))->TopWndExtend;
	for (i = 0; i < MAX_EZ_TIMER; i++)
	{
		if (iTimerID == TopExtend->Timer[i].WinTimerID)
		{
			//计时器ID和记录的相符
			//那么，发送消息啊
			EZSendMessage(TopExtend->Timer[i].ezWnd, EZWM_TIMER, i, 0);


			break;
		}
	}
	return;
}

int ezInsideWndProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	//这里！修改函数，链表枚举方向应该反过来，先找到最后，然后返回来一个个检查。（保证和绘制的枚举顺序相同）
	EZWND ezChildLast;

	ezChildLast = ezWnd->FirstChild;

	if (IsEZWindow(ezChildLast))
	{

		while (ezChildLast->NextEZWnd)
		{
			ezChildLast = ezChildLast->NextEZWnd;
		}

		//滚动到链表顶部，检查是否在子窗口内
		//**************************************************
		while (ezChildLast)
		{
			//检查是否在子窗口内，注意屏幕移动
			//if (PtInEZWnd(ezChildLast, GET_X_LPARAM(lParam) - ezChildLast->ScrollX, GET_Y_LPARAM(lParam) - ezChildLast->ScrollY))
			if ((ezChildLast->MouseMsgRecv != 2) && PtInEZWnd(ezChildLast, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))
			{
				//可以了，在这个窗口
				if (ezInsideWndProc(ezChildLast, message, wParam, MAKELPARAM(GET_X_LPARAM(lParam) - ezChildLast->x - ezChildLast->ScrollX, GET_Y_LPARAM(lParam) - ezChildLast->y - ezChildLast->ScrollY)) != TRANSPARENT)
				{
					return 0;
				}
				//好吧，子窗口说他是透明的》。。。。。继续看会不会落在其他子窗口内
			}

			ezChildLast = ezChildLast->LastEZWnd;
		}
		//**************************************************		

		//已经到最后了，那么，就是在主窗口。直接出去和外面的合并

	}

	{
		//连个子窗口都没。。。那么，就在你这里了。

		//先发送鼠标到来消息
		//更改这里的时候，别忘了把上面相同的一起改了~~~~~
		if (message == EZWM_MOUSEMOVE && ezWnd->MouseOn == FALSE)
		{
			if (ezWnd->ezRootParent->TopWndExtend->MouseOnWnd)
			{
				ezWnd->ezRootParent->TopWndExtend->MouseOnWnd->MouseOn = FALSE;//标记一下。
				EZSendMessage(ezWnd->ezRootParent->TopWndExtend->MouseOnWnd, EZWM_MOUSELEAVE, 0, 0);
			}
			ezWnd->ezRootParent->TopWndExtend->MouseOnWnd = ezWnd;
			ezWnd->MouseOn = TRUE;//标记一下。
			EZSendMessage(ezWnd, EZWM_MOUSECOME, (WPARAM)NULL, (LPARAM)NULL);//老鼠来了
		}

		else if (ezWnd->FocusState == 0 && message == EZWM_LBUTTONDOWN)//你没焦点，消息是左键
		{
			if (IsEZWindow(EZGetTopParentWindow(ezWnd)->TopWndExtend->FocusWindow))
			{
				EZGetTopParentWindow(ezWnd)->TopWndExtend->FocusWindow->FocusState = 0;//你没焦点了。
				EZSendMessage(EZGetTopParentWindow(ezWnd)->TopWndExtend->FocusWindow, EZWM_KILLFOCUS, NULL, NULL);
			}
			ezWnd->FocusState = 1;

			EZGetTopParentWindow(ezWnd)->TopWndExtend->FocusWindow = ezWnd;
			EZSendMessage(ezWnd, EZWM_SETFOCUS, (WPARAM)NULL, (LPARAM)NULL);//你有焦点了！
		}
		return EZSendMessage(ezWnd, message, wParam, lParam);

	}
}



BOOL EZBroadcastToAllChildFunc(EZWND ezWnd, BOOL sequence, int message, WPARAM wp, LPARAM lp)
{
	//注意，这个函数是以深度优先的方式发送消息的。（我懒得再写一个广度优先的，如果以后我写了，删掉这一句话）
	EZWND LastChild;

	LastChild = ezWnd->FirstChild;
	//向回滚，如正序则直接处理，倒叙不处理。
	for (;;)
	{
		if (sequence)//正
		{
			//正序，所以，先处理自己。
			BroadcastProc(LastChild, message, wp, lp);//处理自己

			EZBroadcastToAllChild(LastChild, sequence, message, wp, lp);//给自己的子窗口发送该消息

		}

		if (!IsEZWindow(LastChild->NextEZWnd))//没有下一个
		{
			break;//没有下一个了
		}

		LastChild = LastChild->NextEZWnd;//向下滚

	}

	//如果倒，那么再滚回去
	if (!sequence)
	{
		for (;;)
		{
			//给自己的子窗口发送该消息
			EZBroadcastToAllChild(LastChild, sequence, message, wp, lp);

			BroadcastProc(LastChild, message, wp, lp);//处理自己

			if (!IsEZWindow(LastChild->LastEZWnd))//到头了
			{
				break;
			}

			LastChild = LastChild->LastEZWnd;//回滚

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
		//看看是否是顶层EZ窗口。
		if (!ezWnd->IsTopWindow)
		{
			//不是
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
		//判断是否是顶层窗口
		if (ezWnd->IsTopWindow)
		{
			//画白布一块
			PatBlt(ezWnd->hdc, 0, 0, ezWnd->Width, ezWnd->Height, WHITENESS);
			//PatBlt(ezWnd->hdcWC, 0, 0, ezWnd->Width, ezWnd->Height, WHITENESS);
		}
		else
		{
			X_PSX = ezWnd->x + ezWnd->ezParent->ScrollX;
			Y_PSY = ezWnd->y + ezWnd->ezParent->ScrollY;
			//复制上级窗口的相应部分。
			BitBlt(ezWnd->hdc, 0, 0,
				max(ezWnd->Width, ezWnd->ezParent->Width), max(ezWnd->Height, ezWnd->ezParent->Height),
				ezWnd->ezParent->hdc,
				X_PSX,
				Y_PSY,
				SRCCOPY);
		}

		EZSendMessage(ezWnd, EZWM_TRANSDRAW, (WPARAM)(ezWnd->hdc), (LPARAM)NULL);


		if (ezWnd->Transparent != 255)//如果不是255，混合。是255，那么，别混合了！
		{
			BLENDFUNCTION bf = { 0 };
			/*bf.AlphaFormat = 0;
			bf.BlendFlags = 0;
			bf.BlendOp = AC_SRC_OVER;*/ // 这三个字段全是 0
			bf.SourceConstantAlpha = 255 - ezWnd->Transparent;

			if (!ezWnd->IsTopWindow)
			{
				//这个蠢货函数，不允许超出边界。我们只能手动确保没有超出边界。
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
				//白色
				HDC hParentdc;
				HDC hdcWhite = GetMemDC(hParentdc = GetDC(ezWnd->hParent), ezWnd->Width, ezWnd->Height);

				PatBlt(hdcWhite, 0, 0, ezWnd->Width, ezWnd->Height, WHITENESS);

				AlphaBlend(ezWnd->hdc, 0, 0, ezWnd->Width, ezWnd->Height, hdcWhite, 0, 0, ezWnd->Width, ezWnd->Height, bf);

				DeleteMemDC(hdcWhite);
				ReleaseDC(ezWnd->hParent, hParentdc);
			}
		}

		//混合绘制现在以不透明的方式在DC上，现在从父窗口复制以255-透明度复制到hdcWC

		//发送普通绘制消息，绘制到DC。
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



//判断函数
BOOL PtInEZWnd(EZWND ezWnd, int x, int y)
{
	return (BOOL)((x > ezWnd->x) && (x < (ezWnd->x + ezWnd->Width)) && (y > ezWnd->y) && (y < (ezWnd->y + ezWnd->Height)));
}

BOOL IsEZWindow(EZWND ezWnd)
{
	return (BOOL)(ezWnd != NULL);
}


//*********************************************************************************************************
//                下面这些函数本不属于EZWindow，但EZWindow基于这些函数。这些函数也可以单独使用。
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
//                EZWindow风格扩展头文件 以及宏定义
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
				//按钮被单击了！

				//判断按钮类型
				if (CHK_STYLE(ezWnd->EZStyle & MKDW(11111111, 00000000, 00000000, 00000000), EZBS_PUSHBUTTON))
				{
					//发送控制消息。
					EZSendMessage(ezWnd->ezParent, EZWM_COMMAND, 0, ezWnd);
				}
				else if (CHK_STYLE(ezWnd->EZStyle & MKDW(11111111, 00000000, 00000000, 00000000), EZBS_RADIOBUTTON))
				{
					//没说是AUTO哦，要听父窗口的话。
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
		//创建3个子窗口
		//向上（左？）滚动 -- 扩展0号控件 
		//向下（右？）滚动 -- 扩展1号控件 
		//拖动条 -- 扩展2号控件 

		ezWnd->Extend->hExtend[0] = CreateEZWindow(ezWnd, 0, 0, 0, 0, EZStyle_Scroll_BtnProc);
		ezWnd->Extend->hExtend[0]->ezID = 0;

		ezWnd->Extend->hExtend[1] = CreateEZWindow(ezWnd, 0, 0, 0, 0, EZStyle_Scroll_BtnProc);
		ezWnd->Extend->hExtend[1]->ezID = 1;

		ezWnd->Extend->hExtend[2] = CreateEZWindow(ezWnd, 0, 0, 0, 0, EZStyle_Scroll_BarProc);
		ezWnd->Extend->hExtend[2]->ezID = 2;


		//为了让子窗口可以更方便的判断应该横向还是纵向绘制
		//用两个按钮的Style进行标识

		if ((ezWnd->EZStyle & MKDW(00000000, 00000000, 00000000, 11111111)) == EZS_CHILD_VSCROLL)
		{
			//EZS_CHILD_VSCROLL，竖直
			ezWnd->Extend->hExtend[0]->EZStyle = ezWnd->Extend->hExtend[1]->EZStyle = 0;
		}
		else
		{
			//EZS_CHILD_HSCROLL，水平
			ezWnd->Extend->hExtend[0]->EZStyle = ezWnd->Extend->hExtend[1]->EZStyle = 1;
		}


		//初始化滚动范围，位置
		//滚动最大范围 -- int扩展 0 号
		//滚动位置上 -- int扩展 1 号
		//滚动位置下 -- int扩展 2 号
		ezWnd->Extend->iExtend[0] = 16;//默认16？2333，随便了。
		ezWnd->Extend->iExtend[1] = 0;//默认0
		ezWnd->Extend->iExtend[2] = 1;//默认1

		return 0;
	case EZWM_SIZE:
	{

		if ((ezWnd->EZStyle & MKDW(00000000, 00000000, 00000000, 11111111)) == EZS_CHILD_VSCROLL)
		{	//EZS_CHILD_VSCROLL，竖直

		   //检测竖直方向长度是否大于3倍水平，不然也太不像话了。
			if (!(ezWnd->Height >= ezWnd->Width * 3))
			{
				//滚吧
				MoveEZWindow(ezWnd->Extend->hExtend[0], 0, 0, 0, 0, 0);
				MoveEZWindow(ezWnd->Extend->hExtend[1], 0, 0, 0, 0, 0);
				MoveEZWindow(ezWnd->Extend->hExtend[2], 0, 0, 0, 0, 0);
				return 0;
			}
			//调整位置咯
			MoveEZWindow(ezWnd->Extend->hExtend[0], 0, 0, ezWnd->Width, (ezWnd->Width * 17) / 15, 0);//上面那个按钮
			MoveEZWindow(ezWnd->Extend->hExtend[1], 0, ezWnd->Height - (ezWnd->Width * 17) / 15, ezWnd->Width, (ezWnd->Width * 17) / 15, 0);//下面那个按钮

																																			//滚动条宽度为 MAX(（总宽度 / 最大滚动范围）,最小宽度）

																																			//总宽度是要扣去两边的按钮的哦
			BarLen = ((ezWnd->Height - (ezWnd->Width) * 34.0 / 15.0) / (float)ezWnd->Extend->iExtend[0]);
			MoveEZWindow(ezWnd->Extend->hExtend[2],
				0,
				ceil(ezWnd->Width * 17.0 / 15.0 + (float)BarLen * (float)ezWnd->Extend->iExtend[1]),
				ezWnd->Width,
				BarLen * (float)(ezWnd->Extend->iExtend[2] - ezWnd->Extend->iExtend[1]),
				0);

		}
		else
		{	//EZS_CHILD_HSCROLL，水平

		   //检测水平方向长度是否大于3倍竖直，不然也太不像话了。
			if (!(ezWnd->Width >= ezWnd->Height * 3))
			{
				//滚吧
				MoveEZWindow(ezWnd->Extend->hExtend[0], 0, 0, 0, 0, 0);
				MoveEZWindow(ezWnd->Extend->hExtend[1], 0, 0, 0, 0, 0);
				MoveEZWindow(ezWnd->Extend->hExtend[2], 0, 0, 0, 0, 0);
				return 0;
			}
			//调整位置咯
			MoveEZWindow(ezWnd->Extend->hExtend[0], 0, 0, (ezWnd->Height * 17.0) / 15.0, ezWnd->Height, 0);//左边那个按钮
			MoveEZWindow(ezWnd->Extend->hExtend[1], ezWnd->Width - (ezWnd->Height * 17.0) / 15.0, 0, (ezWnd->Height * 17.0) / 15.0, ezWnd->Height, 0);//右边那个按钮


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
		{	//EZS_CHILD_VSCROLL，竖直
			BarLen = ((ezWnd->Height - (ezWnd->Width) * 34.0 / 15.0) / (float)ezWnd->Extend->iExtend[0]);
			MoveEZWindow(ezWnd->Extend->hExtend[2],
				0,
				ceil(ezWnd->Width * 17.0 / 15.0 + (float)BarLen * (float)ezWnd->Extend->iExtend[1]),
				ezWnd->Width,
				BarLen * (float)(ezWnd->Extend->iExtend[2] - ezWnd->Extend->iExtend[1]),
				0);
		}
		else
		{	//EZS_CHILD_HSCROLL，水平
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
			{	//EZS_CHILD_VSCROLL，竖直
				BarLen = ((ezWnd->Height - (ezWnd->Width) * 34.0 / 15.0) / (float)ezWnd->Extend->iExtend[0]);
				MoveEZWindow(ezWnd->Extend->hExtend[2],
					0,
					ceil(ezWnd->Width * 17.0 / 15.0 + (float)BarLen * (float)ezWnd->Extend->iExtend[1]),
					ezWnd->Width,
					BarLen * (float)(ezWnd->Extend->iExtend[2] - ezWnd->Extend->iExtend[1]),
					0);
			}
			else
			{	//EZS_CHILD_HSCROLL，水平
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
			{	//EZS_CHILD_VSCROLL，竖直
				BarLen = (((float)ezWnd->Height - (float)(ezWnd->Width) * 34.0 / 15.0) / (float)ezWnd->Extend->iExtend[0]);
				iMove = ceil((float)GET_Y_LPARAM(wParam) / BarLen);//参数通过wParam传送过来
			}
			else
			{
				BarLen = (((float)ezWnd->Width - (float)(ezWnd->Height) * 34.0 / 15.0) / ((float)(ezWnd->Extend->iExtend[0])));
				iMove = ceil((float)GET_X_LPARAM(wParam) / BarLen);//参数通过wParam传送过来
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
		//这个窗口嘛，只是普通创建出来的，可惜我们也想用扩展，那自己申请啊~
		ezWnd->Extend = malloc(sizeof(EZSE));

		((pEZSE)ezWnd->Extend)->BackGroundColor = RGB(70, 70, 70);
		((pEZSE)ezWnd->Extend)->ForeGroundColor = RGB(70, 70, 70);

		((pEZSE)ezWnd->Extend)->MouseHold = FALSE;

		((pEZSE)ezWnd->Extend)->hFont = NULL;
		((pEZSE)ezWnd->Extend)->IsFontUserControl = -1;

		ezWnd->Extend->Title = NULL;//这样清理的时候就会跳过清理了，免着free一个野指针。

									//MD，不是样式窗口（假冒的）
		ezWnd->IsStyleWindow = FALSE;
		ezWnd->EZStyle = NULL;

		ezWnd->Extend->iExtend[0] = 0;
		//设置透明度 
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
		//		//这里！设置定时器！

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
		//		//这里！取消定时器！
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
		//绘制箭头
		//就地define.
#define SRLBTN_ARR_W 3
#define SRLBTN_ARR_H 2

		 //准备画笔，画刷。
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
			//竖直
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
			//水平

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
		//绘制透明背景
		SelectObject(wParam, hBrush = CreateSolidBrush(ezWnd->Extend->BackGroundColor));
		PatBlt(wParam, 0, 0, ezWnd->Width, ezWnd->Height, PATCOPY);
		DeleteObject(hBrush);
		return 0;

	case EZWM_TIMER:
		//添加处理
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
		//我们也想用扩展！！！
		ezWnd->Extend = malloc(sizeof(EZSE));

		//不用
		//((pEZSE)ezWnd->Extend)->BackGroundColor = RGB(70, 70, 70);
		//((pEZSE)ezWnd->Extend)->ForeGroundColor = RGB(70, 70, 70);

		((pEZSE)ezWnd->Extend)->MouseHold = FALSE;

		((pEZSE)ezWnd->Extend)->hFont = NULL;
		((pEZSE)ezWnd->Extend)->IsFontUserControl = -1;

		ezWnd->Extend->Title = NULL;//这样清理的时候就会跳过清理了，免着free一个野指针。

									//MD，不是样式窗口（假冒的）
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

			//记录鼠标按下的位置
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
			//别忘了，这是一个滚动条.
			//将鼠标移动的情况告诉父窗口
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
			//用户提交了HFONT，用户负责释放。
			((pEZSE)ezWnd->Extend)->hFont = (HFONT)wParam;
			((pEZSE)ezWnd->Extend)->IsFontUserControl = 0;
		}
		else if (lParam)
		{
			//用户提交了LOGFONT，我们创建并销毁。
			((pEZSE)ezWnd->Extend)->hFont = CreateFontIndirect(lParam);
			((pEZSE)ezWnd->Extend)->IsFontUserControl = 1;
		}
		else
		{
			//都为空，使用默认字体。
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
			lstrcpy(lf.lfFaceName, TEXT("微软雅黑"));
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
		//清理系列内容
		if (ezWnd->Extend)
		{
			//清理字体
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

			//清理标题
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
		//创建子窗口。
		//0号扩展放主编辑窗口
		//1号扩展放竖直滚动条
		//2号水平滚动条
		ezWnd->Extend->iExtend[0] = 0;
		ezWnd->Extend->iExtend[1] = 0;
		ezWnd->Extend->iExtend[2] = 0;
		ezWnd->Extend->hExtend[0] = CreateEZWindow(ezWnd, 0, 0, 0, 0, EZStyle_Edit_InputProc);//主编辑窗口
		ezWnd->Extend->hExtend[1] = CreateEZStyleWindow(ezWnd, TEXT(""), EZS_CHILD | EZS_CHILD_VSCROLL, 0, 0, 0, 0);
		ezWnd->Extend->hExtend[2] = CreateEZStyleWindow(ezWnd, TEXT(""), EZS_CHILD | EZS_CHILD_HSCROLL, 0, 0, 0, 0);

		ezWnd->Extend->iExtend[3] = -1;//无限制
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
		MoveEZWindow(ezWnd->Extend->hExtend[1],//竖直，V
			ezWnd->Width - 15, 0,
			CHK_ALT_STYLE(ezWnd->EZStyle, EZS_VSCROLL) ? (15) : (0),
			CHK_ALT_STYLE(ezWnd->EZStyle, EZS_HSCROLL) ? (ezWnd->Height - 15) : (ezWnd->Height)
			, 0);
		MoveEZWindow(ezWnd->Extend->hExtend[2],//水平，H
			0, ezWnd->Height - 15,
			CHK_ALT_STYLE(ezWnd->EZStyle, EZS_VSCROLL) ? (ezWnd->Width - 15) : (ezWnd->Width),
			CHK_ALT_STYLE(ezWnd->EZStyle, EZS_HSCROLL) ? (15) : (0)
			, 0);
		return 0;
	}

	case EZWM_SETFONT:
		//嘿嘿，骚操作。直接递交给默认处理。
		EZStyle_DefaultProc(ezWnd, message, wParam, lParam);


		//并入下面SETTEXT的部分
	case EZWM_SETTEXT:
		//截下来，计算好大小，然后再提交给默认处理
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
				//绘制当前行，并退出。
				GetTextExtentPoint32(ezWnd->hdc, Text + LastMove, iMove - LastMove, &size);
				yCount += tm.tmHeight;
				xCount = max(xCount, size.cx);
				LineCount++;

				break;
			}
			else if (Text[iMove] == '\r' && Text[iMove + 1] == '\n')
			{
				//windows换行标记，绘制当前行，重新开始。
				GetTextExtentPoint32(ezWnd->hdc, Text + LastMove, iMove - LastMove, &size);
				yCount += tm.tmHeight;
				xCount = max(xCount, size.cx);
				LineCount++;

				LastMove = iMove + 2;
				iMove++;

			}
			else if (Text[iMove] == '\n')
			{
				//Linux换行标记，绘制当前行。
				GetTextExtentPoint32(ezWnd->hdc, Text + LastMove, iMove - LastMove, &size);
				yCount += tm.tmHeight;
				xCount = max(xCount, size.cx);
				LineCount++;

				LastMove = iMove + 1;
			}
			iMove++;//放在for里面我怕我搞混....放到这里就不会了
		}

		//设置滚动		MoveEZWindow(ezWnd->Extend->hExtend[3], 0, 0, max(ezWnd->Extend->hExtend[0]->Width, xCount), max(ezWnd->Extend->hExtend[0]->Height, yCount), 0);





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
			//已经给默认处理提交过了，在前面。
			return 0;
		}
		//else,EZWM_SETTEXT，提交给默认处理。
		break;
	}
	break;

	case EZWM_SCROLLPOSCHANGE:
		//wParam就是偏移像素数
		if (lParam == ezWnd->Extend->hExtend[1])//竖直
		{
			ezWnd->Extend->hExtend[0]->ScrollY = -(int)(wParam);
		}
		else if (lParam == ezWnd->Extend->hExtend[2])//水平
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
		//父窗口的扩展浪费了，干脆我们也一起用父窗口多余的扩展吧
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
				//当前行，并退出。
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
				//windows换行标记，重新开始。
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
				//Linux换行标记
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
			iMove++;//放在for里面我怕我搞混....放到这里就不会了
		}

		if (IsFounded)
		{
			//就在当前行！

			CaretY = yCount;
			IsFounded = FALSE;
			int LastLen, CurrLen;
			LastLen = CurrLen = 0;
			for (iMove = LastMove; iMove <= iMaxLen;)
			{

				if ((Text[iMove] == '\0') || (Text[iMove] == '\r' && Text[iMove + 1] == '\n') || (Text[iMove] == '\n'))
				{
					//没找到，在行末尾
					ezWnd->ezParent->ezParent->Extend->iExtend[2] = iMove;
					break;
				}

				LastLen = CurrLen;
				//GetTextExtentPoint32(ezWnd->hdc, Text + LastMove, iMove - LastMove, &size);
				int CharWidth;
				GetCharWidth32(ezWnd->hdc, Text[iMove], Text[iMove], &CharWidth);
				//CurrLen = size.cx;
				CurrLen += CharWidth;


				//得到旧位置和新位置的平均位置
				if (GET_X_LPARAM(lParam) <= (LastLen + CurrLen) / 2)
				{
					//插入符号在这个字符往前推一个,也就是在LastCurr的位置
					IsFounded = TRUE;
					ezWnd->ezParent->ezParent->Extend->iExtend[2] = iMove;
					break;

				}

				iMove++;//放在for里面我怕我搞混....放到这里就不会了
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
			//文档末尾
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
		//开始绘制，循环检查，找到\r\n，或\n，掐行，输出。

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
				//绘制当前行，并退出。
				TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
				//GetTextExtentPoint32(wParam, Text + LastMove, iMove - LastMove, &size);

				yCount += tm.tmHeight;
				//xCount = max(xCount, size.cx);



				break;
			}
			else if (Text[iMove] == '\r' && Text[iMove + 1] == '\n')
			{
				//windows换行标记，绘制当前行，重新开始。
				TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
				//GetTextExtentPoint32(wParam, Text + LastMove, iMove - LastMove, &size);
				yCount += tm.tmHeight;
				//xCount = max(xCount, size.cx);


				LastMove = iMove + 2;
				iMove++;

			}
			else if (Text[iMove] == '\n')
			{
				//Linux换行标记，绘制当前行。
				TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
				//GetTextExtentPoint32(wParam, Text + LastMove, iMove - LastMove, &size);
				yCount += tm.tmHeight;
				//xCount = max(xCount, size.cx);


				LastMove = iMove + 1;
			}
			iMove++;//放在for里面我怕我搞混....放到这里就不会了
		}

	}
	EZShowCaret(ezWnd);
	//SetPixel(wParam, ezWnd->ezParent->ezParent->Extend->iExtend[0], ezWnd->ezParent->ezParent->Extend->iExtend[1], RGB(255, 0, 0));
	return 0;

	case EZWM_CHAR:

		//这里！解决新加字符问题。
		//每次有EZWM_CHAR，都是通过传递模拟SETTEXT实现修改的。但是SETTEXT可没有处理位移问题。比如当前输入了一个字符，超出了边界之类的问题
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
				EZSendMessage(ezWnd, EZWM_KEYDOWN, 37, 0);//模拟按键

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
				EZSendMessage(ezWnd, EZWM_KEYDOWN, 37, 0);//模拟按键
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
			//换行符，插入\r\n
			//支持多行？
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
			EZSendMessage(ezWnd, EZWM_KEYDOWN, 39, 0);//模拟按键
			pp_CaretPos = ezWnd->ezParent->ezParent->Extend->iExtend[2];
			EZRepaint(ezWnd->ezParent->ezParent, 0);

			wParam = '\n';//出去之后自动插入
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
		EZSendMessage(ezWnd, EZWM_KEYDOWN, 39, 0);//模拟按键
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
				//左
				//看看前面是不到头了，或是 '\n' 或 "\r\n"
				if (ezWnd->ezParent->ezParent->Extend->iExtend[2] == 0)
				{
					//别动了，到头了，不需要任何操作
					return 0;
				}

				//是不是换行？
				if (Text[ezWnd->ezParent->ezParent->Extend->iExtend[2] - 1] == '\n')
				{
					//是哪种换行？
					if (Text[ezWnd->ezParent->ezParent->Extend->iExtend[2] - 2] == '\r')
					{
						ezWnd->ezParent->ezParent->Extend->iExtend[2]--;//额外-1
					}
				}
				//无论是不是换行，都要-1.
				ezWnd->ezParent->ezParent->Extend->iExtend[2]--;
			}
			else if (wParam == 39)
			{
				//右

				//看看后面是不是到尾了，或是'\n' 或 "\r\n"
				if (ezWnd->ezParent->ezParent->Extend->iExtend[2] == ezWnd->ezParent->ezParent->Extend->TitleLen)
				{
					//别动了，到尾了，不需要任何操作
					return 0;
				}
				//是不是换行？
				if (Text[ezWnd->ezParent->ezParent->Extend->iExtend[2]] == '\r')
				{
					if (Text[ezWnd->ezParent->ezParent->Extend->iExtend[2] + 1] == '\n')
					{
						ezWnd->ezParent->ezParent->Extend->iExtend[2]++;//额外+1
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
					//绘制当前行，并退出。
					//GetTextExtentPoint32(ezWnd->hdc, Text + LastMove, iMove - LastMove, &size);
					yCount += tm.tmHeight;


					break;
				}
				else if (Text[iMove] == '\r' && Text[iMove + 1] == '\n')
				{
					//windows换行标记，绘制当前行，重新开始。
					//GetTextExtentPoint32(ezWnd->hdc, Text + LastMove, iMove - LastMove, &size);
					yCount += tm.tmHeight;

					LastMove = iMove + 2;
					iMove++;

				}
				else if (Text[iMove] == '\n')
				{
					//Linux换行标记，绘制当前行。
					//GetTextExtentPoint32(ezWnd->hdc, Text + LastMove, iMove - LastMove, &size);

					LastMove = iMove + 1;
					yCount += tm.tmHeight;
				}
				iMove++;//放在for里面我怕我搞混....放到这里就不会了
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
					//设置滚动条
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
				//有没有上一行？
				//往前递归，跨过一个换行符，到达第二个换行符

				//有种情况，那就是....这个就是换行符（现在在行尾。）需要我们手动检查并避免这种情况

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
							//两个齐了！现在Text[iLineBeginCount]是 '\n'，所以要加一
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
					//没有上一行
					return 0;
				}
				CaretY = ezWnd->ezParent->ezParent->Extend->iExtend[1] - tm.tmHeight;


			}
			else if (wParam == 40)
			{
				//有没有下一行？
				//往后递归，跨过一个换行符，到达第二行行首
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
					//没有下一行
					return 0;
				}
				CaretY = ezWnd->ezParent->ezParent->Extend->iExtend[1] + tm.tmHeight;

			}

			//在新行中，找到与现在位置最近的位置。

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

				//得到旧位置和新位置的平均位置
				if (ezWnd->ezParent->ezParent->Extend->iExtend[0] <= (LastLen + CurrLen) / 2)
				{
					//插入符号在这个字符往前推一个,也就是在LastCurr的位置
					IsFounded = TRUE;
					//有一种情况是特殊的，那就是行首。在行首，两个值都为0.这样的话，iMove不应-1
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
					//没找到，在行末尾
					ezWnd->ezParent->ezParent->Extend->iExtend[2] = iMove;
					break;
				}


				iMove++;//放在for里面我怕我搞混....放到这里就不会了
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
				//设置滚动条
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
			//这里！！创建关闭，缩小，放大按钮！！！
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
			hFont = CreateFont(EZWND_CAP_HEIGHT * 0.6, 0, 0, 0, 550, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("微软雅黑"));
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
		//这是我能想出来的最快的操作了
		return 0;
	}
	//
	//case EZWM_LBUTTONDOWN:
	//	SendMessage(ezWnd->hParent, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);//拖动窗口。
	////	这里开始！！！改成处理EZWM_NCHITTEST，添加EZWM_NCHITTEST的递归？
	//	return 0;


	case EZWM_WINNCHITTEST:
	{
		////检测范围。先问问默认的意见，如果默认说是客户区我们再看是不是标题栏
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
		if ((ezWnd->EZStyle & 0xff) == EZS_OVERLAPPEDWINDOW)//那个0xff是MKDW(00000000,00000000,00000000,11111111)
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
	//这里！！！如果，是子窗口收到了呢？这样肯定不行。修改这里的EZWM_SETFOCUS  EZWM_KILLFOCUS

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
				NCCSParam->rgrc[0].top += 1;//在标题栏上面留出一些，这里先少加一些
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
			//无视
		}
		else
		{
			//发送控制信号
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
			//透明度减少

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
			//无视
		}
		else
		{
			//发送控制信号
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
			//透明度减少

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
			//无视
		}
		else
		{
			//发送控制信号
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
