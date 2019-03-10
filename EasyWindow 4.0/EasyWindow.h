
/*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *

*                EasyWindow.h

*                EasyWindow 库源文件 版本  3.0   Copyright (c) 2017 y.h. All rights reserved.

*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  */


//*********************************************************************************************************
//                头文件等预编译指令
//*********************************************************************************************************


#pragma once

#pragma region HeadFiles

#include<Windows.h>
#include<windowsx.h>
#include<math.h>
#include<strsafe.h>
#pragma endregion


#pragma region LinkLibs

#pragma comment(lib,"msimg32.lib")

#pragma endregion

//*********************************************************************************************************
//                常量宏定义
//*********************************************************************************************************


#pragma region EasyWindowMessageDefines
//下面是对EasyWindows消息的定义

//常规消息
#define EZWM_CREATE 1
#define EZWM_DESTROY 2
#define EZWM_SIZE 3
#define EZWM_TIMER 4

#define EZWM_ENABLE 5
#define EZWM_DISABLE 6

#define EZWM_SETFOCUS 7
#define EZWM_KILLFOCUS 8

#define EZWM_ACTIVATE 9
#define EZWM_ACTIVATEAPP 10
#define EZWM_WINNCACTIVATE 11//啊。。这个返回值啥用都没，被忽视了。只不过是个通知。注意，在收到这条消息之前已经调用过DefWndProc了

#define EZWM_CLOSE 12

#define EZCLOSE_WINDOW 0//响应EZWM_CLOSE时，用于声明关闭窗口
#define EZDO_NOT_CLOSE 1//响应EZWM_CLOSE时，声明请勿关闭窗口




//窗口大小消息
#define EZWM_GETMINMAXINFO 13 //如果进行了处理，请返回非零值。

#define EZWM_WINNCCALCSIZE 14//如果进行了处理，返回值是WM_NCCALCSIZE的返回值+1，没处理返回值为0

//绘制有关消息
#define EZWM_DRAW 101//绘制，不透明
#define EZWM_TRANSDRAW 102//透明绘制，该次绘制将以透明形式覆盖。
#define EZWM_COVERCHILD 103//覆盖子窗口绘制，如需透明自己搞（目前）
#define EZWM_MAPDC 104//映射DC，wParam为一个RECT指针，指明了子窗口范围。修改该结构体即可，lparam是需要映射的子窗口。
#define EZWM_REDRAWFINISH 105//该窗口重绘完成（可以自己把ezWnd->hdc复制到别的地方，用于同步窗口内容啊什么的）


#define EZWM_WINNCDRAW 106//Windows父窗口非客户区要重绘

//鼠标消息
#define EZWM_LBUTTONDOWN 201
#define EZWM_LBUTTONUP 202
#define EZWM_RBUTTONDOWN 203
#define EZWM_RBUTTONUP 204
#define EZWM_MOUSEMOVE 205//这个消息如果WPARAM为1，（仅发送到EZ主窗口）说明从Win窗口出去了
#define EZWM_MOUSECOME 206
#define EZWM_MOUSELEAVE 207

#define EZWM_WINNCHITTEST 208//讲道理我觉得这个...也可以放在鼠标？虽然这是一个非客户区消息。只有Win父窗口会收到这个消息
#define EZWM_NCHITTEST 209//


#define EZHTERROR             (-3)
#define EZHTTRANSPARENT       (-2)
#define EZHTNOWHERE           -1
#define EZHTCLIENT            1
#define EZHTCAPTION           2
#define EZHTSYSMENU           3
#define EZHTGROWBOX           4
#define EZHTSIZE              HTGROWBOX
#define EZHTMENU              5
#define EZHTHSCROLL           6
#define EZHTVSCROLL           7
#define EZHTMINBUTTON         8
#define EZHTMAXBUTTON         9
#define EZHTLEFT              10
#define EZHTRIGHT             11
#define EZHTTOP               12
#define EZHTTOPLEFT           13
#define EZHTTOPRIGHT          14
#define EZHTBOTTOM            15
#define EZHTBOTTOMLEFT        16
#define EZHTBOTTOMRIGHT       17
#define EZHTBORDER            18
#define EZHTREDUCE            HTMINBUTTON
#define EZHTZOOM              HTMAXBUTTON
#define EZHTSIZEFIRST         HTLEFT
#define EZHTSIZELAST          HTBOTTOMRIGHT
#if(WINVER >= 0x0400)
#define EZHTOBJECT            19
#define EZHTCLOSE             20
#define EZHTHELP              21
#endif




#define EZWM_CAPTUREMOUSE 251
#define EZWM_RELEASEMOUSE 252

//键盘消息
#define EZWM_CHAR 301
#define EZWM_KEYDOWN 302
#define EZWM_KEYUP 303


//滚动条消息
#define EZWM_VSCROLL 401
#define EZWM_HSCROLL 402


//各类设置消息

#define EZWM_SETCOLOR 501//wParam是背景颜色，lParam是前景颜色。该消息会引起重绘。
#define EZWM_SETFONT 502//设置字体，wParam和lParam二选一，都空则设置该风格默认字体。wParam是HFONT（用户申请释放），lParam是LOGFONT（EZWnd申请释放）。wParam优先
#define EZWM_SETTEXT 503//设置文本，wParam是文本。lParam是长度，为0则自动获取长度
#define EZWM_SETTEXTALIGN 504//文本对齐选项，wParam传递参数。临时设置为DrawText的绘制参数。后期将继续改进
#define EZWM_SETMAXTEXT 505//设置最大文本长度，wParam是最大长度,-1没有限制。已经超过了不会进行处理。

#define EZWM_SETSCROLLRANGE 506//设置滚动条范围，wParam是最大滚动范围。
#define EZWM_SETSCROLLPOS 507//设置滚动条位置，wParam lParam，是上下滚动位置。wParam < lParam <= Max 



//控制消息
#define EZWM_COMMAND 601//恩，含义不解释了。比Windows简单一些，lParam是句柄，wParam是通知的附加信息。
#define EZWM_SCROLLPOSCHANGE 602//滚动条位置动了，wParam 是上滚动位置，lParam是句柄。


//通知类消息
#define EZWM_USER_NOTIFY 701//自由使用，自己弄得清楚通知形式就OK
#define EZWM_WINWND_CHECKSTATE 702//要求Win窗口检查显示状态。



//获取信息消息
#define EZWM_GETTEXT 801//获得文本,wParam缓冲区，lParam最大计数



//内置消息
#define SEZWM_COPYDC 2001//将存储DC复制到父窗口。
#define SEZWM_REDRAW 2002//重绘到内存DC


#define SEZWM_KILLFOCUS 2005//有焦点的窗口失去焦点。wP排除检查。


// #define SEZWM_CAPTURE 2005//有一个窗口申请捕获老鼠。已经捕获了的赶紧放掉！

#pragma endregion




#pragma region EZStyleDefines

#define MKDW(a,b,c,d) 0b ## a ## b ## c ## d

#define CHK_STYLE(Style,ChkStyle) (Style ^ ChkStyle)> 0 ? FALSE : TRUE
#define CHK_ALT_STYLE(Style,AltStyle) (((Style) & (AltStyle)) > 0 ? (TRUE) : (FALSE))

//*****可选择属性 - 定义

//                                                      -|-
#define EZS_CHILD          MKDW(00000000,00000000,00000001,00000000)
#define EZS_PARENT         MKDW(00000000,00000000,00000000,00000000)

//                                                     -|-
#define EZS_BORDER         MKDW(00000000,00000000,00000010,00000000)
#define EZS_NO_BORDER      MKDW(00000000,00000000,00000000,00000000)

//                                                    -|-
#define EZS_CAPTION        MKDW(00000000,00000000,00000100,00000000)
#define EZS_NO_CAPTION     MKDW(00000000,00000000,00000000,00000000)



//                                                   -|-
#define EZS_MINIMIZEBOX    MKDW(00000000,00000000,00001000,00000000)//最小化按钮
#define EZS_NO_MINIMIZEBOX MKDW(00000000,00000000,00000000,00000000)

//                                                  -|-
#define EZS_MAXIMIZEBOX    MKDW(00000000,00000000,00010000,00000000)//最大化按钮
#define EZS_NO_MAXIMIZEBOX MKDW(00000000,00000000,00000000,00000000)

//                                                 -|-
#define EZS_CLOSEBOX       MKDW(00000000,00000000,00100000,00000000)//关闭按钮
#define EZS_NO_CLOSEBOX    MKDW(00000000,00000000,00000000,00000000)

//                                                -|-
#define EZS_VSCROLL        MKDW(00000000,00000000,01000000,00000000)//竖直
#define EZS_NO_VSCROLL     MKDW(00000000,00000000,00000000,00000000)

//                                               -|-
#define EZS_HSCROLL        MKDW(00000000,00000000,10000000,00000000)//水平
#define EZS_NO_HSCROLL     MKDW(00000000,00000000,00000000,00000000)


//*****窗口主要属性 - 定义
//子窗口篇
#define EZS_STATIC         MKDW(00000000,00000000,00000000,00000001)
#define EZS_BUTTON         MKDW(00000000,00000000,00000000,00000010)
//这两个可不是带有滚动控件的窗口，这两个就是滚动控件。
#define EZS_CHILD_VSCROLL  MKDW(00000000,00000000,00000000,00000011)//竖直
#define EZS_CHILD_HSCROLL  MKDW(00000000,00000000,00000000,00000100)//水平


#define EZS_EDIT           MKDW(00000000,00000000,00000000,00000101)//编辑框



//父窗口篇
#define EZS_POPUP          MKDW(00000000,00000000,00000000,00000001)
#define EZS_OVERLAPPED     MKDW(00000000,00000000,00000000,00000010)
#define EZS_POPUPWINDOW    MKDW(00000000,00000000,00000000,00000011)
#define EZS_OVERLAPPEDWINDOW MKDW(00000000, 00000000, 00000000, 00000100)

//有关父窗口的其他定义
#define EZWND_CAP_HEIGHT 30


//*****窗口细分属性 - 定义
//***子窗口篇
//按钮篇
#define EZBS_PUSHBUTTON    MKDW(00000000,00000000,00000000,00000000)//默认，什么都不带就是普通按钮
#define EZBS_RADIOBUTTON   MKDW(00000001,00000000,00000000,00000000)

//编辑框篇
//                             -|-
#define EZES_SINGLELINE    MKDW(10000000,00000000,00000000,00000000)
//                             -|-
#define EZES_MULTILINE     MKDW(00000000,00000000,00000000,00000000)//默认
//                              -|-
#define EZES_PASSWORD      MKDW(01000000,00000000,00000000,00000000)//密码编辑框

#pragma endregion


#pragma region DialogStyleDefines
#define EZDLG_MASK 1//在父窗口上渐变显示一个遮罩。如果没有该选项，颜色参数将被忽略

#define EZDLG_CENTER 2//始终居中显示，忽略x,y参数。

#pragma endregion


#pragma region OtherDefines

//各种其他宏定义

//内部文本最大长度
#define MAX_TEXT 16384

#define GET_EXTEND(ezWnd,Extend) (((pEZSE)(ezWnd))->Extend)

//抗锯齿
#define STRETCH 4.0

#pragma endregion


#pragma region EasyWindowClassName

#define EZWindowClass TEXT("EasyWindowClass")

#pragma endregion


//*******************************************************************************************
//                结构体声明
//*******************************************************************************************


typedef struct tagEZWND * EZWND;//对EZWINDOW结构体的定义。EZWND是指向该结构的指针。



#pragma region ExtendForStyles
typedef struct tagExtendStyle
{
	PBYTE Title;//每次动态申请，最多不得超过 MAX_TEXT
	int TitleLen;

	//UI相关
	COLORREF BackGroundColor;
	COLORREF ForeGroundColor;

	HFONT hFont;

	DWORD TextAlign;
	int IsFontUserControl;//字体谁负责释放？（-1 之前没字体，0 用户控制，1 EZWnd负责，2 默认字体）


						  //鼠标相关
	int MouseHold;//用于判断鼠标是否按着，比如按钮控件就会用得到。当然用来记录鼠标按的时间长短也没问题

				  //自由使用扩展
	int iExtend[4];//4个整型扩展 程序可以自由根据控件类型使用。
	EZWND hExtend[8];//8个EZWND扩展 程序可以自由根据控件类型使用。
	void* vExtend[4];//4个指针扩展 程序可以自由根据控件类型使用。

}EZSE, * pEZSE;
#pragma endregion


typedef INT(*EZWNDPROC)(EZWND, int, WPARAM, LPARAM);//回调函数定义

#define MAX_EZ_TIMER 64//64个计时器


#pragma region TopWindowExtend
typedef struct tagezTopWindowExtend
{
	EZWND FocusWindow;//拥有输入焦点的窗口
	EZWND CptMouseWindow;//捕获了鼠标的
	EZWND MouseOnWnd;//鼠标在上面的

	EZWND CptKbdWindow;//捕获了键盘消息的

	HDC hdcTop;//顶层窗口申请内存使用，其他窗口而言，这只是一个空的句柄。

			   //	int TimerNum;//当前使用了的计时器数量

	struct ezTimer
	{
		int WinTimerID;

		EZWND ezWnd;
	} Timer[MAX_EZ_TIMER];

}TopWndExt, * pTWE;
#pragma endregion


#pragma region ezWndStruct


#define WND_TIMER_NUM 8
typedef struct tagEZWND
{
	//有关窗口信息
	int x;
	int y;
	int Width;
	int Height;

	//设置这两个变量可以滚动客户区内的子窗口，以及鼠标输入坐标。
	int ScrollX;
	int ScrollY;

	int px;//x坐标（相对于主窗口）
	int py;//y坐标（相对于主窗口）
	RECT VisibleRect;//可见的矩形（相对于主窗口）
	BOOL IsVsbRectNull;
	//有关绘制状态****************************
	BYTE Transparent;//透明度，0是完全透明，255不透明

					 //有关窗口状态****************************
	int FocusState;//输入焦点状态。0-无焦点。1-有焦点 
	int MouseMsgRecv;//鼠标消息接受情况。 1：正常接受，2：透明
	int ShowState;//显示状态（和鼠标无关）1：正常显示，2：透明

				  //有关鼠标****************************
	BOOL MouseOn;//鼠标在该窗口上。

				 //有关绘制****************************
	HDC hdc;//一个绘制空间。始终存在，直到窗口被销毁。窗口绘制就绘制在这个上面。
	//HDC hdcCopy;//用于加速绘制，下一个窗口可以直接复制的DC。

				//BOOL DrawOnNC;//是否覆盖非客户区绘制，换言之绘制的时候是否用GetWindowDC
	BOOL Update;//更新区域，0为无需更新，1为发生了改变，需要更新。

				//有关关联窗口****************************
	EZWNDPROC ezWndProc;//窗口过程
	EZWND ezParent;//EZ父窗口
	EZWND ezRootParent;//EZ祖先窗口

	BOOL IsTopWindow;//用于指示是否是顶层EZ窗口。
	HWND hParent;//祖先Win窗口.

	BOOL IsWinWnd;//是否链接到了一个Win子窗口
	HWND hChild;//子Win窗口
	WNDPROC WinChildProc;

	EZWND FirstChild;//第一个子窗口
	EZWND LastEZWnd;//（和这个窗口）并列的上一个窗口
	EZWND NextEZWnd;//（和这个窗口）并列的下一个窗口


					//顶层窗口专属
	pTWE TopWndExtend;


	//扩展****************************

	int ezID;//用户可以自由使用，用于标识窗口ID。
	int TimerID[WND_TIMER_NUM];//8个应该够用了，普通窗口可以自由使用。带有样式的窗口可能会用来存放Windows计时器ID
	pEZSE Extend;//扩展指针。如果EZ窗口是自创的，那么EZWindow不会使用这个变量。但是假如这个窗口是含有 EZSTYLE 的窗口，这个指针会被EZWindow使用

	DWORD EZStyle;//窗口样式。
	BOOL IsStyleWindow;//是否是带有样式的窗口

#pragma region  ezWndStyleExplaination
					   /*窗口样式注释

					   EZStyle 被定义为了 DWORD 也就是 4个字节，32位。

					   0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0

					   这里是每个字段的含义：

					   最后一个字节    ：256种，表示窗口主要类型，比如，是不是按钮，是不是编辑框等。
					   倒数第二三个字节：16位，表示窗口的可选择属性，比如窗口是否带有边框。
					   倒数第四个字节  ：256种，表示在主要类型下窗口的细分属性，比如按钮窗口是否是单选按钮。
					   */
#pragma endregion
}EZWINDOW;

#pragma endregion

//*******************************************************************************************
//                函数声明
//*******************************************************************************************

#pragma region EZWindowFunctionDefines

//static BOOL InitEZWindow();
//static EZWND CreateEZParentWindowEx(DWORD EZStyle, int x, int y, int Width, int Height, DWORD WinStyle, EZWNDPROC ezWndProc, HMENU hMenu, HWND hParent);
//static EZWND CreateEZWindowEx(EZWND ezParent, DWORD EZStyle, int x, int y, int Width, int Height, EZWNDPROC ezWndProc);
//static BOOL DestroyEZWindow(EZWND ezWnd);
//static BOOL DestroyEZWindowWithNext(EZWND ezWnd);
//static BOOL MoveEZWindow(EZWND ezWnd, int x, int y, int Width, int Height, BOOL repaint);
//static BOOL EZResetChildPxPy(EZWND ezWnd);
//static BOOL ScrollEZWindow(EZWND ezWnd, int x, int y, BOOL bAdd);
//static BOOL EZCaptureMouse(EZWND ezWnd);
//static BOOL EZReleaseMouse(EZWND ezWnd);
//static BOOL SetMouseMsgRecvMode(EZWND ezWnd, int Mode);
//static BOOL SetShowState(EZWND ezWnd, int State);
//static BOOL EZRedraw(EZWND ezWnd);
//static BOOL EZUpdate(EZWND ezWnd, HDC hdc);
//static BOOL EZRepaint(EZWND ezWnd, HDC hdc);
//static BOOL EZAddChild(EZWND ezParent, EZWND ezChild);
//static EZWND CreateEZStyleWindow(EZWND ezParent, TCHAR Title[], DWORD EZStyle, int x, int y, int Width, int Height);
//static EZWND CreateEZStyleParentWindow(TCHAR Title[], DWORD EZStyle, int x, int y, int Width, int Height, BOOL bAdjust, EZWNDPROC ezWndProc);
//static int SetEZTimer(EZWND ezWnd, int iTimeSpace);
//static BOOL KillEZTimer(EZWND ezWnd, int TimerID);
//static BOOL EZCreateCaret(EZWND ezWnd, HBITMAP hBitmap, int nWidth, int nHeight);
//static BOOL EZSetCaretPos(EZWND ezWnd, int x, int y);
//static BOOL EZShowCaret(EZWND ezWnd);
//static BOOL EZHideCaret(EZWND ezWnd);
//static BOOL EZDestroyCaret();
//static EZWND EZDialogBox(EZWND ezParent, int x, int y, int w, int h, DWORD Style, COLORREF MaskClr, EZWNDPROC ezWndProc);
//static BOOL EZEndDialog(EZWND ezWnd);
//static EZWNDPROC EZDlgHookProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
//static EZWNDPROC EZDialogBoxMask(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
//static LRESULT CALLBACK EZParentWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
//static VOID CALLBACK ezInsideTimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
//static int ezInsideWndProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
//static BOOL PtInEZWnd(EZWND ezWnd, int x, int y);
//static BOOL IsEZWindow(EZWND ezWnd);
//static BOOL EZBroadcastToAllChildFunc(EZWND ezWnd, BOOL sequence, int message, WPARAM wp, LPARAM lp);
//static BOOL BroadcastProc(EZWND ezWnd, int Param, WPARAM wP, LPARAM lP);
////static int EZSendMessage(EZWND ezWnd, int message, WPARAM wP, LPARAM lP);
//static BOOL SetEZWndTransparent(EZWND ezWnd, int Transparent);
//
//static BOOL RedrawBroadcast(EZWND ezWnd, WPARAM wp, LPARAM lp, int cx, int cy, RECT RectSrc);
//static int EZWndMessageLoop();
//
//
//
////外部样式
//static EZWNDPROC EZStyle_ButtonProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
//static EZWNDPROC EZStyle_StaticProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
//static EZWNDPROC EZStyle_ScrollChildProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
//static EZWNDPROC EZStyle_Scroll_BtnProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
//static EZWNDPROC EZStyle_Scroll_BarProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
//static EZWNDPROC EZStyle_DefaultProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
//static EZWNDPROC EZStyle_EditProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
//static EZWNDPROC EZStyle_Edit_InputProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
//static EZWNDPROC EZStyle_Edit_InputChildProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
//static EZWNDPROC EZStyle_OverlappedWndProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
//
//static EZWNDPROC EZStyle_WndMinProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
//
//static EZWNDPROC EZStyle_WndMaxProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
//
//static EZWNDPROC EZStyle_WndCloseProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);



BOOL InitEZWindow();
EZWND CreateEZParentWindowEx(DWORD EZStyle, int x, int y, int Width, int Height, DWORD WinStyle, EZWNDPROC ezWndProc, HMENU hMenu, HWND hParent);
EZWND CreateEZWindowEx(EZWND ezParent, DWORD EZStyle, int x, int y, int Width, int Height, EZWNDPROC ezWndProc);
BOOL DestroyEZWindow(EZWND ezWnd);
BOOL DestroyEZWindowWithNext(EZWND ezWnd);
BOOL MoveEZWindow(EZWND ezWnd, int x, int y, int Width, int Height, BOOL repaint);
BOOL EZResetChildPxPy(EZWND ezWnd);
BOOL ScrollEZWindow(EZWND ezWnd, int x, int y, BOOL bAdd);
BOOL EZCaptureMouse(EZWND ezWnd);
BOOL EZReleaseMouse(EZWND ezWnd);
BOOL EZCaptureKeyboard(EZWND ezWnd);
BOOL EZReleaseKeyboard(EZWND ezWnd);
BOOL SetMouseMsgRecvMode(EZWND ezWnd, int Mode);
BOOL SetShowState(EZWND ezWnd, int State);
BOOL EZRedraw(EZWND ezWnd);
BOOL EZUpdate(EZWND ezWnd, HDC hdc);
BOOL EZRepaint(EZWND ezWnd, HDC hdc);
BOOL EZAddChild(EZWND ezParent, EZWND ezChild);
EZWND CreateEZStyleWindow(EZWND ezParent, TCHAR Title[], DWORD EZStyle, int x, int y, int Width, int Height);
EZWND CreateEZStyleParentWindow(TCHAR Title[], DWORD EZStyle, int x, int y, int Width, int Height, BOOL bAdjust, EZWNDPROC ezWndProc);
int SetEZTimer(EZWND ezWnd, int iTimeSpace);
BOOL KillEZTimer(EZWND ezWnd, int TimerID); BOOL EZCreateCaret(EZWND ezWnd, HBITMAP hBitmap, int nWidth, int nHeight);
BOOL EZSetCaretPos(EZWND ezWnd, int x, int y);
BOOL EZShowCaret(EZWND ezWnd);
BOOL EZHideCaret(EZWND ezWnd);
BOOL EZDestroyCaret();
EZWND EZDialogBox(EZWND ezParent, int x, int y, int w, int h, DWORD Style, COLORREF MaskClr, EZWNDPROC ezWndProc);
BOOL EZEndDialog(EZWND ezWnd);
EZWNDPROC EZDlgHookProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
EZWNDPROC EZDialogBoxMask(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EZParentWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
VOID CALLBACK ezInsideTimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
int ezInsideWndProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
BOOL PtInEZWnd(EZWND ezWnd, int x, int y);
BOOL IsEZWindow(EZWND ezWnd);
BOOL EZBroadcastToAllChildFunc(EZWND ezWnd, BOOL sequence, int message, WPARAM wp, LPARAM lp);
BOOL BroadcastProc(EZWND ezWnd, int Param, WPARAM wP, LPARAM lP);
//int EZSendMessage(EZWND ezWnd, int message, WPARAM wP, LPARAM lP);
BOOL SetEZWndTransparent(EZWND ezWnd, int Transparent);

BOOL RedrawBroadcast(EZWND ezWnd, WPARAM wp, LPARAM lp, int cx, int cy, RECT RectSrc);
int EZWndMessageLoop();



//外部样式
EZWNDPROC EZStyle_ButtonProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
EZWNDPROC EZStyle_StaticProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
EZWNDPROC EZStyle_ScrollChildProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
EZWNDPROC EZStyle_Scroll_BtnProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
EZWNDPROC EZStyle_Scroll_BarProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
EZWNDPROC EZStyle_DefaultProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
EZWNDPROC EZStyle_EditProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
EZWNDPROC EZStyle_Edit_InputProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
EZWNDPROC EZStyle_Edit_InputChildProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
EZWNDPROC EZStyle_OverlappedWndProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);

EZWNDPROC EZStyle_WndMinProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);

EZWNDPROC EZStyle_WndMaxProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);

EZWNDPROC EZStyle_WndCloseProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);


#pragma endregion


#pragma region OtherFunctions

HDC GetMemDC(HDC hdc, int cx, int cy);

BOOL DeleteMemDC(HDC hdc);

BOOL AdjustMemDC(HDC hdc, HDC hdcCpb, int cx, int cy);

#pragma endregion

//*******************************************************************************************
//                函数宏定义
//*******************************************************************************************

#pragma region EZWindowFunctions

 //#define CreateEZParentWindow(x, y, Width, Height,  ezWndProc, hMenu, hParent) CreateEZParentWindow(x, y, Width, Height,  WS_CHILD|WS_VISIBLE, ezWndProc, hMenu, hParent) 

#define CreateEZWindow(ezParent, x,y, Width, Height, ezWndProc) CreateEZWindowEx(ezParent,0L, x,y, Width, Height, ezWndProc)

#define CreateEZParentWindow(x, y, Width, Height,WinStyle, ezWndProc, hMenu, hParent) CreateEZParentWindowEx(0L, x, y, Width, Height,WinStyle, ezWndProc, hMenu, hParent)



#define EZBroadcastToAllChild(ezWnd, sequence, message, wp,lp) if(IsEZWindow(ezWnd->FirstChild)){ EZBroadcastToAllChildFunc(ezWnd, sequence, message, wp,lp); }

 //#define EZRepaint(ezWnd,hdc) {  EZRedraw(ezWnd);   EZUpdate(ezWnd, hdc);}

#define EZGetTopParentWindow(ezWnd) (ezWnd->ezRootParent)


#define EZSendMessage(ezWnd, message, wP, lP) (ezWnd)->ezWndProc(ezWnd, message, wP, lP)


 //#define EZRepaint(ezWnd,hdc) {  EZRedraw(ezWnd);   EZUpdate(ezWnd, hdc);}

#pragma endregion


