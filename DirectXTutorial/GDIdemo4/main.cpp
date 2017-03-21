//#include "stdafx.h"
#include <windows.h>
#include <atldbcli.h>
#include <playsoundapi.h>
#include <time.h>

#pragma comment(lib, "winmm.lib")

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define WINDOW_TITLE L"GDIDemo4 ³ÌÐòºËÐÄ¿ò¼Ü"
#define WINDOW_CLASS_NAME L"My_CLASS_GDI_DEMO4"

HDC g_hdc = NULL;
HDC g_memDC = NULL;
HBITMAP g_hBitmapBk, g_hSprite1, g_hSprite2;

VOID Game_Paint(HWND hWnd)
{
	// »­±³¾°
	SelectObject(g_memDC, g_hBitmapBk);
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_memDC, 0, 0, SRCCOPY);

	//Í¸Ã÷ÕÚÕÖ·¨
	SelectObject(g_memDC, g_hSprite1);
	BitBlt(g_hdc, 50, WINDOW_HEIGHT - 579, 320, 579, g_memDC, 320, 0, SRCAND);
	BitBlt(g_hdc, 50, WINDOW_HEIGHT - 579, 320, 579, g_memDC, 0, 0, SRCPAINT);

	SelectObject(g_memDC, g_hSprite2);
	BitBlt(g_hdc, 450, WINDOW_HEIGHT - 584, 400, 584, g_memDC, 400, 0, SRCAND);
	BitBlt(g_hdc, 450, WINDOW_HEIGHT - 584, 400, 584, g_memDC, 0, 0, SRCPAINT);
}

BOOL Game_Init(HWND hWnd)
{
	g_hdc = GetDC(hWnd);

	g_hBitmapBk = (HBITMAP)::LoadImage(NULL, L"bg.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
	g_hSprite1 = (HBITMAP)::LoadImage(NULL, L"character1.bmp", IMAGE_BITMAP, 640, 579, LR_LOADFROMFILE);
	g_hSprite2 = (HBITMAP)::LoadImage(NULL, L"character2.bmp", IMAGE_BITMAP, 800, 584, LR_LOADFROMFILE);
	g_memDC = CreateCompatibleDC(g_hdc);

	Game_Paint(hWnd);
	ReleaseDC(hWnd, g_hdc);

	return TRUE;
}

BOOL Game_Cleanup(HWND hWnd)
{
	DeleteObject(g_hBitmapBk);
	DeleteObject(g_hSprite1);
	DeleteObject(g_hSprite2);
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
		MessageBox(hWnd, _T("×ÊÔ´³õÊ¼»¯Ê§°Ü£¡"), _T("ÖÂÃü´íÎó"), MB_OK | MB_ICONERROR);
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