//#include "stdafx.h"
#include <windows.h>
#include <atldbcli.h>
#include <playsoundapi.h>
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#pragma comment(lib, "winmm.lib")

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define WINDOW_TITLE L"������Ŀ��"
#define WINDOW_CLASS_NAME L"My_CLASS"

LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddevice;

void initD3D(HWND hWnd); // sets up and initializes Direct3D
void render_frame();    // renders a single frame
void cleanD3D();       // closes Direct3D and releases memory

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbWndExtra = 0;
	wndClass.cbClsExtra = 0;
	wndClass.hInstance = hInstance;
	//wndClass.hIcon = (HICON)::LoadImage(NULL, _T("icon.ico"), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wndClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	//wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = WINDOW_CLASS_NAME;

	if (!RegisterClassEx(&wndClass))
	{
		return -1;
	}

	hWnd = CreateWindowEx(0, 
		WINDOW_CLASS_NAME, 
		WINDOW_TITLE, 
		WS_POPUP, 
		0, 0, 
		WINDOW_WIDTH, WINDOW_HEIGHT, 
		NULL, NULL, hInstance, NULL);
	//MoveWindow(hWnd, 250, 250, WINDOW_WIDTH, WINDOW_HEIGHT, TRUE);

	ShowWindow(hWnd, iCmdShow);
	//UpdateWindow(hWnd);

	initD3D(hWnd);

	MSG msg = { 0 };
	while (TRUE)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				goto exit;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// run render code
		render_frame();
	}

exit:
	cleanD3D();
	return msg.wParam;
}

void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = FALSE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = WINDOW_WIDTH;
	d3dpp.BackBufferHeight = WINDOW_HEIGHT;

	d3d->CreateDevice(D3DADAPTER_DEFAULT,  //ʹ��Ĭ��ͼ�ο�
		D3DDEVTYPE_HAL, //ʹ��Ӳ��������Ⱦ
		hWnd, 
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
		&d3dpp,
		&d3ddevice);
}

void render_frame()
{
	//���ƻ�����
	d3ddevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);

	//���ƻ�ǰ������
	d3ddevice->BeginScene(); //��ס�Կ����ڴ�

	d3ddevice->EndScene();
	d3ddevice->Present(NULL, NULL, NULL, NULL);
}

void cleanD3D()
{
	d3ddevice->Release();
	d3d->Release();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMessage, wParam, lParam);
}