//#include "stdafx.h"
#include <windows.h>
#include <atldbcli.h>
#include <playsoundapi.h>
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#pragma comment(lib, "winmm.lib")

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define WINDOW_TITLE L"程序核心框架"
#define WINDOW_CLASS_NAME L"My_CLASS"

LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddevice;
LPDIRECT3DVERTEXBUFFER9 v_buffer;

void initD3D(HWND hWnd); // sets up and initializes Direct3D
void initGraphics();
void render_frame();    // renders a single frame
void cleanD3D();       // closes Direct3D and releases memory

struct CUSTOMVERTEX
{
	float x, y, z, rhw;
	DWORD color;
};

#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

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

	d3d->CreateDevice(D3DADAPTER_DEFAULT,  //使用默认图形卡
		D3DDEVTYPE_HAL, //使用硬件处理渲染
		hWnd, 
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
		&d3dpp,
		&d3ddevice);

	initGraphics();
}

void initGraphics()
{
	CUSTOMVERTEX vertices[] = {
		{400.0f, 62.5f, 0.5f, 1.0f, D3DCOLOR_XRGB(0,0,255)},
		{650.0f, 500.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0,255,0)},
		{150.0f, 500.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255,0,0)}
	};

	d3ddevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
		0, CUSTOMFVF, D3DPOOL_MANAGED, &v_buffer, nullptr);
	VOID *pData;
	v_buffer->Lock(0, 0, &pData, 0);
	memcpy(pData, vertices, sizeof(vertices));
	v_buffer->Unlock();
}

void render_frame()
{
	//类似画背景
	d3ddevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//类似画前景内容
	d3ddevice->BeginScene(); //锁住显卡的内存

	d3ddevice->SetFVF(CUSTOMFVF);
	d3ddevice->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	d3ddevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	d3ddevice->EndScene();
	d3ddevice->Present(NULL, NULL, NULL, NULL);
}

void cleanD3D()
{
	v_buffer->Release();
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