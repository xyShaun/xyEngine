#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include <d2d1.h>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

const TCHAR* szWndClassName = _T("MainWnd");
const TCHAR* szWndCaption = _T("Hello, xyEngine!");
int nClientWidth = 800;
int nClientHeight = 600;

HWND hMainWnd;
ID2D1Factory* pD2DFactory = nullptr;
ID2D1HwndRenderTarget* pRenderTarget = nullptr;
ID2D1SolidColorBrush* pRedBrush = nullptr;
ID2D1SolidColorBrush* pGreenBrush = nullptr;

#ifndef ReleaseCom
#define ReleaseCom(x) { if(x){ x->Release(); x = 0; } }
#endif

HRESULT CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);

	return hr;
}

HRESULT CreateDeviceResources()
{
	HRESULT hr = S_OK;
	if (pRenderTarget != nullptr)
	{
		return hr;
	}

	RECT rc;
	GetClientRect(hMainWnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	hr = pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hMainWnd, size),
		&pRenderTarget);

	if (SUCCEEDED(hr))
	{
		hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
	}

	return hr;
}

void DestroyDeviceResources()
{
	ReleaseCom(pRenderTarget);
	ReleaseCom(pRedBrush);
	ReleaseCom(pGreenBrush);
}

HRESULT OnRender()
{
	HRESULT hr = S_OK;
	hr = CreateDeviceResources();
	if (FAILED(hr))
	{
		DestroyDeviceResources();
		return hr;
	}

	pRenderTarget->BeginDraw();

	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	D2D1_SIZE_F rtSize = pRenderTarget->GetSize();

	// Draw two rectangles.
	D2D1_RECT_F rectangle1 = D2D1::RectF(
		rtSize.width / 2 - 50.0f,
		rtSize.height / 2 - 50.0f,
		rtSize.width / 2 + 50.0f,
		rtSize.height / 2 + 50.0f
	);

	D2D1_RECT_F rectangle2 = D2D1::RectF(
		rtSize.width / 2 - 100.0f,
		rtSize.height / 2 - 100.0f,
		rtSize.width / 2 + 100.0f,
		rtSize.height / 2 + 100.0f
	);

	// Draw a filled rectangle.
	pRenderTarget->FillRectangle(&rectangle1, pRedBrush);

	// Draw the outline of a rectangle.
	pRenderTarget->DrawRectangle(&rectangle2, pGreenBrush);

	hr = pRenderTarget->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DestroyDeviceResources();
	}

	return hr;
}

void OnResize(UINT width, UINT height)
{
	if (pRenderTarget != nullptr)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
	{
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);
		OnResize(width, height);

		return 0;
	}
	case WM_DISPLAYCHANGE:
	{
		InvalidateRect(hWnd, nullptr, FALSE);

		return 0;
	}
	case WM_PAINT:
	{
		OnRender();
		ValidateRect(hWnd, nullptr);

		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);

		return 0;
	}
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool InitMainWindow(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = 0;
	wc.lpszMenuName = 0;
	wc.lpszClassName = szWndClassName;

	if (!RegisterClass(&wc))
	{
		MessageBox(0, _T("RegisterClass Failed."), 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, nClientWidth, nClientHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	hMainWnd = CreateWindow(szWndClassName, szWndCaption,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hInstance, 0);
	if (!hMainWnd)
	{
		MessageBox(0, _T("CreateWindow Failed."), 0, 0);
		return false;
	}

	ShowWindow(hMainWnd, SW_SHOW);
	UpdateWindow(hMainWnd);

	return true;
}

int RunMainLoop()
{
	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Otherwise, do animation/game stuff.
		else
		{

		}
	}

	return (int)msg.wParam;
}

int InitializeApp(HINSTANCE hInstance)
{
	HRESULT  hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(hr))
	{
		return -1;
	}

	hr = CreateDeviceIndependentResources();
	if (FAILED(hr))
	{
		return -1;
	}

	if (!InitMainWindow(hInstance))
	{
		return -1;
	}

	return 0;
}

void FinalizeApp()
{
	ReleaseCom(pD2DFactory);
	DestroyDeviceResources();

	CoUninitialize();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	int ret = InitializeApp(hInstance);
	if (ret != 0)
	{
		return ret;
	}

	ret = RunMainLoop();

	FinalizeApp();

	return ret;
}