//#include "stdafx.h"
#include <windows.h>
#include <atldbcli.h>
#include <playsoundapi.h>
#include <time.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msimg32.lib")

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define WINDOW_TITLE L"GDIDemo7 ³ÌÐòºËÐÄ¿ò¼Ü"
#define WINDOW_CLASS_NAME L"My_CLASS_GDI_DEMO7"

HDC g_hdc = NULL;
HDC g_memDC = NULL;
HDC g_bufDC = NULL;

HBITMAP g_hBitmapBk;
HBITMAP g_hBitmapSprite;

int g_index = 0;
int g_x = 0;

VOID Game_Paint(HWND hWnd)
{
	if (g_index < 0 || g_index >=8)
	{
		g_index = 0;
	}

	if (g_x >= WINDOW_WIDTH)
	{
		g_x = 0;
	}

	SelectObject(g_bufDC, g_hBitmapBk);
	BitBlt(g_memDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bufDC, 0, 0, SRCCOPY);

	SelectObject(g_bufDC, g_hBitmapSprite);
	TransparentBlt(g_memDC, g_x, WINDOW_HEIGHT - 158, 60, 108, g_bufDC, g_index * 60, 0, 60, 108, RGB(255, 0, 0));

	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_memDC, 0, 0, SRCCOPY);

	++g_index;
	g_x += 3;
}

BOOL Game_Init(HWND hWnd)
{
	g_hdc = GetDC(hWnd);

	g_hBitmapBk = (HBITMAP)::LoadImage(NULL, L"bg.bmp", IMAGE_BITMAP, 798, 600, LR_LOADFROMFILE);
	g_hBitmapSprite = (HBITMAP)::LoadImage(NULL, L"goright.bmp", IMAGE_BITMAP, 480, 108, LR_LOADFROMFILE);

	g_memDC = CreateCompatibleDC(g_hdc);
	g_bufDC = CreateCompatibleDC(g_hdc);
	HBITMAP bmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(g_memDC, bmp);


	return TRUE;
}

BOOL Game_Cleanup(HWND hWnd)
{
	DeleteObject(g_hBitmapBk);
	DeleteObject(g_hBitmapSprite);
	DeleteObject(g_memDC);
	DeleteObject(g_bufDC);
	ReleaseDC(hWnd, g_hdc);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
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
		MessageBox(hWnd, _T("×ÊÔ´³õÊ¼»¯Ê§°Ü£¡"), _T("ÖÂÃü´íÎó"), MB_OK | MB_ICONERROR);
		return -1;
	}

	static DWORD g_lastTick = 0;
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			DWORD dwNow = ::GetTickCount();
			if (dwNow - g_lastTick >= 100)
			{
				Game_Paint(hWnd);
				g_lastTick = dwNow;
			}
		}
	}

	UnregisterClass(WINDOW_CLASS_NAME, wndClass.hInstance);

	return 0;
}