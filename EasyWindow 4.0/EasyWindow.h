
/*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *

*                EasyWindow.h

*                EasyWindow ��Դ�ļ� �汾  3.0   Copyright (c) 2017 y.h. All rights reserved.

*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  */


//*********************************************************************************************************
//                ͷ�ļ���Ԥ����ָ��
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
//                �����궨��
//*********************************************************************************************************


#pragma region EasyWindowMessageDefines
//�����Ƕ�EasyWindows��Ϣ�Ķ���

//������Ϣ
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
#define EZWM_WINNCACTIVATE 11//����1����ֹ����DefWindowProc��DefWindowProc����Ĭ����ɫ�ڴ����ϻ���NC��

#define EZWM_CLOSE 12

#define EZCLOSE_WINDOW 0//��ӦEZWM_CLOSEʱ�����������رմ���
#define EZDO_NOT_CLOSE 1//��ӦEZWM_CLOSEʱ����������رմ���




//���ڴ�С��Ϣ
#define EZWM_GETMINMAXINFO 13 //��������˴����뷵�ط���ֵ��

#define EZWM_WINNCCALCSIZE 14//��������˴�������ֵ��WM_NCCALCSIZE�ķ���ֵ+1��û������ֵΪ0

//�����й���Ϣ
#define EZWM_DRAW 101//���ƣ���͸��
#define EZWM_TRANSDRAW 102//͸�����ƣ��ôλ��ƽ���͸����ʽ���ǡ�
#define EZWM_COVERCHILD 103//�����Ӵ��ڻ��ƣ�����͸���Լ��㣨Ŀǰ��
#define EZWM_MAPDC 104//ӳ��DC��wParamΪһ��RECTָ�룬ָ�����Ӵ��ڷ�Χ���޸ĸýṹ�弴�ɣ�lparam����Ҫӳ����Ӵ��ڡ�
#define EZWM_REDRAWFINISH 105//�ô����ػ���ɣ������Լ���ezWnd->hdc���Ƶ���ĵط�������ͬ���������ݰ�ʲô�ģ�


#define EZWM_WINNCDRAW 106//Windows�����ڷǿͻ���Ҫ�ػ�

//�����Ϣ
#define EZWM_LBUTTONDOWN 201
#define EZWM_LBUTTONUP 202
#define EZWM_RBUTTONDOWN 203
#define EZWM_RBUTTONUP 204
#define EZWM_MOUSEMOVE 205//�����Ϣ���WPARAMΪ1���������͵�EZ�����ڣ�˵����Win���ڳ�ȥ��
#define EZWM_MOUSECOME 206
#define EZWM_MOUSELEAVE 207

#define EZWM_WINNCHITTEST 208//�������Ҿ������...Ҳ���Է�����ꣿ��Ȼ����һ���ǿͻ�����Ϣ��ֻ��Win�����ڻ��յ������Ϣ
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

//������Ϣ
#define EZWM_CHAR 301
#define EZWM_KEYDOWN 302
#define EZWM_KEYUP 303


//��������Ϣ
#define EZWM_VSCROLL 401
#define EZWM_HSCROLL 402


//����������Ϣ

#define EZWM_SETCOLOR 501//wParam�Ǳ�����ɫ��lParam��ǰ����ɫ������Ϣ�������ػ档
#define EZWM_SETFONT 502//�������壬wParam��lParam��ѡһ�����������ø÷��Ĭ�����塣wParam��HFONT���û������ͷţ���lParam��LOGFONT��EZWnd�����ͷţ���wParam����
#define EZWM_SETTEXT 503//�����ı���wParam���ı���lParam�ǳ��ȣ�Ϊ0���Զ���ȡ����
#define EZWM_SETTEXTALIGN 504//�ı�����ѡ�wParam���ݲ�������ʱ����ΪDrawText�Ļ��Ʋ��������ڽ������Ľ�
#define EZWM_SETMAXTEXT 505//��������ı����ȣ�wParam����󳤶�,-1û�����ơ��Ѿ������˲�����д���

#define EZWM_SETSCROLLRANGE 506//���ù�������Χ��wParam����������Χ��
#define EZWM_SETSCROLLPOS 507//���ù�����λ�ã�wParam lParam�������¹���λ�á�wParam < lParam <= Max 



//������Ϣ
#define EZWM_COMMAND 601//�������岻�����ˡ���Windows��һЩ��lParam�Ǿ����wParam��֪ͨ�ĸ�����Ϣ��
#define EZWM_SCROLLPOSCHANGE 602//������λ�ö��ˣ�wParam ���Ϲ���λ�ã�lParam�Ǿ����


//֪ͨ����Ϣ
#define EZWM_USER_NOTIFY 701//����ʹ�ã��Լ�Ū�����֪ͨ��ʽ��OK
#define EZWM_WINWND_CHECKSTATE 702//Ҫ��Win���ڼ����ʾ״̬��



//��ȡ��Ϣ��Ϣ
#define EZWM_GETTEXT 801//����ı�,wParam��������lParam������



//������Ϣ
#define SEZWM_COPYDC 2001//���洢DC���Ƶ������ڡ�
#define SEZWM_REDRAW 2002//�ػ浽�ڴ�DC


#define SEZWM_KILLFOCUS 2005//�н���Ĵ���ʧȥ���㡣wP�ų���顣


// #define SEZWM_CAPTURE 2005//��һ���������벶�������Ѿ������˵ĸϽ��ŵ���

#pragma endregion




#pragma region EZStyleDefines

#define MKDW(a,b,c,d) 0b ## a ## b ## c ## d

#define CHK_STYLE(Style,ChkStyle) (Style ^ ChkStyle)> 0 ? FALSE : TRUE
#define CHK_ALT_STYLE(Style,AltStyle) (((Style) & (AltStyle)) > 0 ? (TRUE) : (FALSE))

//*****��ѡ������ - ����

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
#define EZS_MINIMIZEBOX    MKDW(00000000,00000000,00001000,00000000)//��С����ť
#define EZS_NO_MINIMIZEBOX MKDW(00000000,00000000,00000000,00000000)

//                                                  -|-
#define EZS_MAXIMIZEBOX    MKDW(00000000,00000000,00010000,00000000)//��󻯰�ť
#define EZS_NO_MAXIMIZEBOX MKDW(00000000,00000000,00000000,00000000)

//                                                 -|-
#define EZS_CLOSEBOX       MKDW(00000000,00000000,00100000,00000000)//�رհ�ť
#define EZS_NO_CLOSEBOX    MKDW(00000000,00000000,00000000,00000000)

//                                                -|-
#define EZS_VSCROLL        MKDW(00000000,00000000,01000000,00000000)//��ֱ
#define EZS_NO_VSCROLL     MKDW(00000000,00000000,00000000,00000000)

//                                               -|-
#define EZS_HSCROLL        MKDW(00000000,00000000,10000000,00000000)//ˮƽ
#define EZS_NO_HSCROLL     MKDW(00000000,00000000,00000000,00000000)


//*****������Ҫ���� - ����
//�Ӵ���ƪ
#define EZS_STATIC         MKDW(00000000,00000000,00000000,00000001)
#define EZS_BUTTON         MKDW(00000000,00000000,00000000,00000010)
//�������ɲ��Ǵ��й����ؼ��Ĵ��ڣ����������ǹ����ؼ���
#define EZS_CHILD_VSCROLL  MKDW(00000000,00000000,00000000,00000011)//��ֱ
#define EZS_CHILD_HSCROLL  MKDW(00000000,00000000,00000000,00000100)//ˮƽ


#define EZS_EDIT           MKDW(00000000,00000000,00000000,00000101)//�༭��



//������ƪ
#define EZS_POPUP          MKDW(00000000,00000000,00000000,00000001)
#define EZS_OVERLAPPED     MKDW(00000000,00000000,00000000,00000010)
#define EZS_POPUPWINDOW    MKDW(00000000,00000000,00000000,00000011)
#define EZS_OVERLAPPEDWINDOW MKDW(00000000, 00000000, 00000000, 00000100)

//�йظ����ڵ���������
#define EZWND_CAP_HEIGHT 30


//*****����ϸ������ - ����
//***�Ӵ���ƪ
//��ťƪ
#define EZBS_PUSHBUTTON    MKDW(00000000,00000000,00000000,00000000)//Ĭ�ϣ�ʲô������������ͨ��ť
#define EZBS_RADIOBUTTON   MKDW(00000001,00000000,00000000,00000000)

//�༭��ƪ
//                             -|-
#define EZES_SINGLELINE    MKDW(10000000,00000000,00000000,00000000)
//                             -|-
#define EZES_MULTILINE     MKDW(00000000,00000000,00000000,00000000)//Ĭ��

#pragma endregion


#pragma region DialogStyleDefines
#define EZDLG_MASK 1//�ڸ������Ͻ�����ʾһ�����֡����û�и�ѡ���ɫ������������

#define EZDLG_CENTER 2//ʼ�վ�����ʾ������x,y������

#pragma endregion


#pragma region OtherDefines

//���������궨��

//�ڲ��ı���󳤶�
#define MAX_TEXT 16384

#define GET_EXTEND(ezWnd,Extend) (((pEZSE)(ezWnd))->Extend)

//�����
#define STRETCH 4.0

#pragma endregion


#pragma region EasyWindowClassName

#define EZWindowClass TEXT("EasyWindowClass")
#define EZWindowShadowClass TEXT("EasyWindowShadowClass")

#pragma endregion


//*******************************************************************************************
//                �ṹ������
//*******************************************************************************************


typedef struct tagEZWND * EZWND;//��EZWINDOW�ṹ��Ķ��塣EZWND��ָ��ýṹ��ָ�롣



#pragma region ExtendForStyles
typedef struct tagExtendStyle
{
	PBYTE Title;//ÿ�ζ�̬���룬��಻�ó��� MAX_TEXT
	int TitleLen;

	//UI���
	COLORREF BackGroundColor;
	COLORREF ForeGroundColor;

	HFONT hFont;

	DWORD TextAlign;
	int IsFontUserControl;//����˭�����ͷţ���-1 ֮ǰû���壬0 �û����ƣ�1 EZWnd����2 Ĭ�����壩


						  //������
	int MouseHold;//�����ж�����Ƿ��ţ����簴ť�ؼ��ͻ��õõ�����Ȼ������¼��갴��ʱ�䳤��Ҳû����

				  //����ʹ����չ
	int iExtend[4];//4��������չ ����������ɸ��ݿؼ�����ʹ�á�
	EZWND hExtend[8];//8��EZWND��չ ����������ɸ��ݿؼ�����ʹ�á�
	void * vExtend[4];//4��ָ����չ ����������ɸ��ݿؼ�����ʹ�á�

}EZSE, *pEZSE;
#pragma endregion


typedef INT(*EZWNDPROC)(EZWND, int, WPARAM, LPARAM);//�ص���������

#define MAX_EZ_TIMER 64//64����ʱ��


#pragma region TopWindowExtend
typedef struct tagezTopWindowExtend
{
	EZWND FocusWindow;//ӵ�����뽹��Ĵ���
	EZWND CptMouseWindow;//����������
	EZWND MouseOnWnd;//����������

	EZWND CptKbdWindow;//�����˼�����Ϣ��

	HDC hdcTop;//���㴰�������ڴ�ʹ�ã��������ڶ��ԣ���ֻ��һ���յľ����

			   //	int TimerNum;//��ǰʹ���˵ļ�ʱ������
	HWND ShadowWindow;

	BOOL bTobShadow;//���㴰����Ӱ�Ƿ���
	int TopShadowTransparent;//���㴰���Լ�����Ӱ͸���ȡ���ָ���Ӵ��ڵ��뿴EZWINDOW�ṹ��
	COLORREF TopShadowColor;//���㴰���Լ�����Ӱ��ɫ

	struct ezTimer
	{
		int WinTimerID;

		EZWND ezWnd;
	} Timer[MAX_EZ_TIMER];

}TopWndExt, *pTWE;
#pragma endregion


#pragma region ezWndStruct


#define WND_TIMER_NUM 8
typedef struct tagEZWND
{
	//�йش�����Ϣ
	int x;
	int y;
	int Width;
	int Height;

	//�����������������Թ����ͻ����ڵ��Ӵ��ڣ��Լ�����������ꡣ
	int ScrollX;
	int ScrollY;

	int px;//x���꣨����������ڣ�
	int py;//y���꣨����������ڣ�

	
	float ShadowStrength;//��Ӱǿ��

	BOOL bShadow;//��Ӱ�Ƿ�������������Ӵ��ڶ����Ǳ����ڵ�
	COLORREF ShadowColor;//��Ӱ��ɫ����������Ӵ��ڶ����Ǳ����ڵ�
	BYTE ShadowTransparent;//��Ӱ͸���ȡ���������Ӵ��ڶ����Ǳ����ڵ�
	HDC ShadowDC;//��Ӱ��DC��
	//TODO: �������Ӵ���������Ӱʱ�ſ���

	RECT VisibleRect;//�ɼ��ľ��Σ�����������ڣ�
	BOOL IsVsbRectNull;
	//�йػ���״̬****************************
	BYTE Transparent;//͸���ȣ�0����ȫ͸����255��͸��

					 //�йش���״̬****************************
	int FocusState;//���뽹��״̬��0-�޽��㡣1-�н��� 
	int MouseMsgRecv;//�����Ϣ��������� 1���������ܣ�2��͸��
	int ShowState;//��ʾ״̬��������޹أ�1��������ʾ��2��͸��

				  //�й����****************************
	BOOL MouseOn;//����ڸô����ϡ�

				 //�йػ���****************************
	HDC hdc;//һ�����ƿռ䡣ʼ�մ��ڣ�ֱ�����ڱ����١����ڻ��ƾͻ�����������档
	//HDC hdcCopy;//���ڼ��ٻ��ƣ���һ�����ڿ���ֱ�Ӹ��Ƶ�DC��

				//BOOL DrawOnNC;//�Ƿ񸲸Ƿǿͻ������ƣ�����֮���Ƶ�ʱ���Ƿ���GetWindowDC
	BOOL Update;//��������0Ϊ������£�1Ϊ�����˸ı䣬��Ҫ���¡�

				//�йع�������****************************
	EZWNDPROC ezWndProc;//���ڹ���
	EZWND ezParent;//EZ������
	EZWND ezRootParent;//EZ���ȴ���

	BOOL IsTopWindow;//����ָʾ�Ƿ��Ƕ���EZ���ڡ�
	HWND hParent;//����Win����.

	BOOL IsWinWnd;//�Ƿ����ӵ���һ��Win�Ӵ���
	HWND hChild;//��Win����
	WNDPROC WinChildProc;

	EZWND FirstChild;//��һ���Ӵ���
	EZWND LastEZWnd;//����������ڣ����е���һ������
	EZWND NextEZWnd;//����������ڣ����е���һ������


					//���㴰��ר��
	pTWE TopWndExtend;


	//��չ****************************

	int ezID;//�û���������ʹ�ã����ڱ�ʶ����ID��
	int TimerID[WND_TIMER_NUM];//8��Ӧ�ù����ˣ���ͨ���ڿ�������ʹ�á�������ʽ�Ĵ��ڿ��ܻ��������Windows��ʱ��ID
	pEZSE Extend;//��չָ�롣���EZ�������Դ��ģ���ôEZWindow����ʹ��������������Ǽ�����������Ǻ��� EZSTYLE �Ĵ��ڣ����ָ��ᱻEZWindowʹ��

	DWORD EZStyle;//������ʽ��
	BOOL IsStyleWindow;//�Ƿ��Ǵ�����ʽ�Ĵ���

#pragma region  ezWndStyleExplaination
					   /*������ʽע��

					   EZStyle ������Ϊ�� DWORD Ҳ���� 4���ֽڣ�32λ��

					   0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0

					   ������ÿ���ֶεĺ��壺

					   ���һ���ֽ�    ��256�֣���ʾ������Ҫ���ͣ����磬�ǲ��ǰ�ť���ǲ��Ǳ༭��ȡ�
					   �����ڶ������ֽڣ�16λ����ʾ���ڵĿ�ѡ�����ԣ����細���Ƿ���б߿�
					   �������ĸ��ֽ�  ��256�֣���ʾ����Ҫ�����´��ڵ�ϸ�����ԣ����簴ť�����Ƿ��ǵ�ѡ��ť��
					   */
#pragma endregion
}EZWINDOW;

#pragma endregion

//*******************************************************************************************
//                ��������
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
////�ⲿ��ʽ
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
BOOL EnableShadow(EZWND ezWnd, BOOL bEnable);
BOOL SetShadowStrength(EZWND ezWnd, float Strength);
BOOL SetShadowColor(EZWND ezWnd, COLORREF Color);
BOOL SetShadowTransparent(EZWND ezWnd, int Trans);
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
LRESULT CALLBACK EZShadowWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
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



void GaussianBlurFilter(unsigned char * input, unsigned char * output, int Width, int Height, int Stride, float GaussianSigma);

//�ⲿ��ʽ
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
//                �����궨��
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


