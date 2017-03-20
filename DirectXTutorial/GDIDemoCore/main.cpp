//#include "stdafx.h"
#include <windows.h>
#include <atldbcli.h>
#include <playsoundapi.h>
#include <time.h>

#pragma comment(lib, "winmm.lib")

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define WINDOW_TITLE L"GDIDemoCore 程序核心框架"
#define WINDOW_CLASS_NAME L"My_CLASS_GDICORE"

HDC g_hdc = NULL;
HDC g_memDC = NULL;
HPEN g_hPen[7] = { 0 };
HBRUSH g_hBrush[7] = { 0 };
int g_iPenStyle[7] = { PS_SOLID, PS_DASH, PS_DOT, PS_DASHDOT, PS_DASHDOTDOT, PS_NULL, PS_INSIDEFRAME };
int g_iBrushStyle[7] = { HS_VERTICAL, HS_HORIZONTAL, HS_CROSS, HS_DIAGCROSS, HS_FDIAGONAL, HS_BDIAGONAL };
HBITMAP g_hBitmap;

VOID Game_Paint(HWND hWnd)
{
	int y = 0;

	for (int i = 0; i <= 6; ++i)
	{
		y = (i + 1) * 70;

		SelectObject(g_hdc, g_hPen[i]);
		MoveToEx(g_hdc, 30, y, NULL);
		LineTo(g_hdc, 100, y);
	}

	int x1 = 120;
	int x2 = 190;
	for (int i = 0; i <= 6; ++i)
	{
		SelectObject(g_hdc, g_hBrush[i]);
		Rectangle(g_hdc, x1, 70, x2, y);
		x1 += 90;
		x2 += 90;
	}

	//绘制文字
	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, L"微软雅黑");
	SelectObject(g_hdc, hFont);
	SetBkMode(g_hdc, TRANSPARENT);
	SetTextColor(g_hdc,RGB(0, 0, 0));
	wchar_t text[] = L"要有最朴素的生活，与最遥远的梦想，即使明日天寒地冻。";
	TextOut(g_hdc, 100, 100, text, wcslen(text));

	SelectObject(g_memDC, g_hBitmap);

	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_memDC, 0, 0, SRCCOPY);
}

BOOL Game_Init(HWND hWnd)
{
	g_hdc = GetDC(hWnd);
	srand((unsigned)time(NULL));

	for (int i = 0; i <= 6; ++i)
	{
		g_hPen[0] = CreatePen(g_iPenStyle[i], 1, RGB(rand() % 256, rand() % 256, rand() % 256));
		if (i==6)
		{
			g_hBrush[i] = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
		}
		else
		{
			g_hBrush[i] = CreateHatchBrush(g_iBrushStyle[i],RGB(rand() % 256, rand() % 256, rand() % 256));
		}
	}

	g_hBitmap = (HBITMAP)::LoadImage(NULL, L"Naruto.bmp", IMAGE_BITMAP, 800, 600, LR_LOADFROMFILE);
	g_memDC = CreateCompatibleDC(g_hdc);

	Game_Paint(hWnd);
	ReleaseDC(hWnd, g_hdc);

	return TRUE;
}

BOOL Game_Cleanup(HWND hWnd)
{
	for (int i = 0; i <= 6; ++i)
	{
		DeleteObject(g_hPen[i]);
		DeleteObject(g_hBrush[i]);
	}

	DeleteObject(g_hBitmap);
	DeleteObject(g_memDC);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	switch (uMessage)
	{
	case WM_PAINT:
		g_hdc = BeginPaint(hWnd, &ps);
		Game_Paint(hWnd);
		EndPaint(hWnd, &ps);
		ValidateRect(hWnd, NULL);
		break;
	case WM_KEYDOWN:
		if (VK_ESCAPE == wParam)
		{
			DestroyWindow(hWnd);
		}
		break;
	case WM_DESTROY:
		Game_Cleanup(hWnd);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMessage, wParam, lParam);
		break;
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbWndExtra = 0;
	wndClass.cbClsExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = (HICON)::LoadImage(NULL, _T("icon.ico"), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wndClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = WINDOW_CLASS_NAME;

	if (!RegisterClassEx(&wndClass))
	{
		return -1;
	}

	HWND hWnd = CreateWindow(WINDOW_CLASS_NAME, WINDOW_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);
	MoveWindow(hWnd, 250, 250, WINDOW_WIDTH, WINDOW_HEIGHT, TRUE);

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	if (!Game_Init(hWnd))
	{
		MessageBox(hWnd, _T("资源初始化失败！"), _T("致命错误"), MB_OK | MB_ICONERROR);
		return -1;
	}

	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	UnregisterClass(WINDOW_CLASS_NAME, wndClass.hInstance);

	return 0;
}