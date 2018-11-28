#include<Windows.h>
#include"EasyWindow.h"



#define WIDTH 930
#define HEIGHT 570

EZWND MainWnd;

EZWNDPROC MainProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	InitEZWindow();
	MainWnd = CreateEZStyleParentWindow(TEXT("EasyWindow Demo"), EZS_PARENT | EZS_OVERLAPPEDWINDOW,
		(GetSystemMetrics(SM_CXSCREEN) - WIDTH) / 2, (GetSystemMetrics(SM_CYSCREEN) - HEIGHT) / 2, WIDTH, HEIGHT, 1, MainProc);
	return EZWndMessageLoop();
}

EZWNDPROC MainProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case EZWM_DRAW:
	{
		RECT rect = { 0,0,ezWnd->Width,ezWnd->Height };
		DrawText(wParam, TEXT("Hello, EasyWindow!!"), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		return 0;
	}
	case EZWM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}
	return 0;
}