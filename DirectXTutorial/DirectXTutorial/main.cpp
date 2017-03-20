//#include "stdafx.h"
#include <windows.h>
#include <atldbcli.h>
#include <playsoundapi.h>

#pragma comment(lib, "winmm.lib")

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define WINDOW_TITLE L"³ÌÐòºËÐÄ¿ò¼Ü"
#define WINDOW_CLASS_NAME L"My_CLASS"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
	case WM_PAINT:
		ValidateRect(hWnd, NULL);
		break;
	case WM_KEYDOWN:
		if (VK_ESCAPE == wParam)
		{
			DestroyWindow(hWnd);
		}
		break;
	case WM_DESTROY:
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