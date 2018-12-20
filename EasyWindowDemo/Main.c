#include<Windows.h>
#include"EasyWindow.h"

#define WIDTH 930
#define HEIGHT 570

EZWND MainWnd;
EZWNDPROC MainProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
EZWNDPROC ChildProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	InitEZWindow();
	MainWnd = CreateEZStyleParentWindow(TEXT("EasyWindow Demo"), EZS_PARENT | EZS_OVERLAPPEDWINDOW,
		(GetSystemMetrics(SM_CXSCREEN) - WIDTH) / 2, (GetSystemMetrics(SM_CYSCREEN) - HEIGHT) / 2, WIDTH, HEIGHT, 1, MainProc);
	return EZWndMessageLoop();
}

EZWNDPROC MainProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	static EZWND ChildWnd;
	switch (message)
	{
	case EZWM_CREATE:
	{
		ChildWnd = CreateEZWindow(ezWnd, ezWnd->Width / 2 + 30, 30, ezWnd->Width / 2 - 60, ezWnd->Height - 60, ChildProc);
		EnableShadow(ezWnd, 1);
		SetShadowTransparent(ezWnd, 128);
		SetShadowStrength(ezWnd, 4);
		return 0;
	}
	case EZWM_DRAW:
	{
		RECT rect = { 0,0,ezWnd->Width / 2,ezWnd->Height };
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


EZWNDPROC ChildProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case EZWM_DRAW:
	{
		RECT rect = { 0,0,ezWnd->Width,ezWnd->Height };

		DrawText(wParam, TEXT("ChildWindow"), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		return 0;
	}
	}
	return 0;
}