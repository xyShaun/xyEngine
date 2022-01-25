#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

using namespace DirectX;

const TCHAR* szWndClassName = _T("MainWnd");
const TCHAR* szWndCaption = _T("Hello, xyEngine!");
int nClientWidth = 800;
int nClientHeight = 600;

HWND hMainWnd;
IDXGISwapChain* pSwapChain = nullptr;
ID3D11Device* pDevice = nullptr;
ID3D11DeviceContext* pDeviceContext = nullptr;
ID3D11RenderTargetView* pRTV = nullptr;
ID3D11InputLayout* pInputLayout = nullptr;
ID3D11VertexShader* pVS = nullptr;
ID3D11PixelShader* pPS = nullptr;
ID3D11Buffer* pVertexBuffer = nullptr;
D3D11_VIEWPORT vp;

struct Vertex
{
	XMFLOAT3 Position;
	XMFLOAT4 Color;
};

#ifndef ReleaseCom
#define ReleaseCom(x) { if(x){ x->Release(); x = 0; } }
#endif

HRESULT CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC scd;
	scd.BufferDesc.Width = nClientWidth;
	scd.BufferDesc.Height = nClientHeight;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 2;
	scd.OutputWindow = hMainWnd;
	scd.Windowed = true;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	D3D_FEATURE_LEVEL supportedFeatureLevels;

	UINT flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		flags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		&scd,
		&pSwapChain,
		&pDevice,
		&supportedFeatureLevels,
		&pDeviceContext);

	return hr;
}

HRESULT CreateRenderTarget()
{
	HRESULT hr = S_OK;

	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRTV);
	if (FAILED(hr))
	{
		return hr;
	}

	// Reduce reference count
	pBackBuffer->Release();

	return hr;
}

void SetupViewport()
{
	vp.Width = (FLOAT)nClientWidth;
	vp.Height = (FLOAT)nClientHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
}

HRESULT CreateShaderAndInputlayout()
{
	HRESULT hr = S_OK;

	ID3DBlob* pVSBlob = nullptr;
	ID3DBlob* pPSBlob = nullptr;

	hr = D3DReadFileToBlob(L"shader.vso", &pVSBlob);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = D3DReadFileToBlob(L"shader.pso", &pPSBlob);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pVS);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &pPS);
	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = pDevice->CreateInputLayout(inputLayout, 2, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &pInputLayout);
	if (FAILED(hr))
	{
		return hr;
	}

	pVSBlob->Release();
	pPSBlob->Release();

	return hr;
}

HRESULT CreateVertexBuffer()
{
	Vertex vertices[] =
	{
		{XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{XMFLOAT3(0.45f, -0.5f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{XMFLOAT3(-0.45f, -0.5f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)}
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	HRESULT hr = pDevice->CreateBuffer(&bd, &vertexData, &pVertexBuffer);

	return hr;
}

bool CreateDeviceResources()
{
	HRESULT hr = S_OK;
	
	hr = CreateDeviceAndSwapChain();
	if (FAILED(hr))
	{
		return false;
	}

	hr = CreateRenderTarget();
	if (FAILED(hr))
	{
		return false;
	}

	SetupViewport();

	hr = CreateShaderAndInputlayout();
	if (FAILED(hr))
	{
		return false;
	}

	hr = CreateVertexBuffer();
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void DestroyDeviceResources()
{
	ReleaseCom(pVertexBuffer);
	ReleaseCom(pInputLayout);
	ReleaseCom(pVS);
	ReleaseCom(pPS);
	ReleaseCom(pRTV);
	ReleaseCom(pDevice);
	ReleaseCom(pDeviceContext);
	ReleaseCom(pSwapChain);
}

void Render()
{
	pDeviceContext->OMSetRenderTargets(1, &pRTV, nullptr);

	pDeviceContext->RSSetViewports(1, &vp);

	pDeviceContext->ClearRenderTargetView(pRTV, Colors::MidnightBlue);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pDeviceContext->IASetInputLayout(pInputLayout);
	pDeviceContext->VSSetShader(pVS, nullptr, 0);
	pDeviceContext->PSSetShader(pPS, nullptr, 0);

	pDeviceContext->Draw(3, 0);
	
	pSwapChain->Present(0, 0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
	{
		nClientWidth = LOWORD(lParam);
		nClientHeight = HIWORD(lParam);

		if (pDevice != nullptr)
		{
			DestroyDeviceResources();
			CreateDeviceResources();
		}

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
			Render();
		}
	}

	return (int)msg.wParam;
}

int InitializeApp(HINSTANCE hInstance)
{
	if (!InitMainWindow(hInstance))
	{
		return -1;
	}

	if (!CreateDeviceResources())
	{
		return -1;
	}

	return 0;
}

void FinalizeApp()
{
	DestroyDeviceResources();
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