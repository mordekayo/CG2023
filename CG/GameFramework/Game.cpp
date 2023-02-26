#include "Game.h"

#include "DisplayWin32.h"
#include "GameObjects/GameObject.h"
#include "Sources.h"
#include <chrono>

FGame::FGame()
{
	ApplicationName = L"My FGame engine";
	ScreenWidth = 800;
	ScreenHeight = 800;
}

void FGame::CreateResources()
{
	Display = new FDisplayWin32(ApplicationName, ScreenWidth, ScreenHeight, WndProc);
	
	constexpr D3D_FEATURE_LEVEL FeatureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
	SwapChainDesc.BufferCount = 2;
	SwapChainDesc.BufferDesc.Width = ScreenWidth;
	SwapChainDesc.BufferDesc.Height = ScreenHeight;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.OutputWindow = Display->GetHWnd();
	SwapChainDesc.Windowed = true;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	
	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		FeatureLevel,
		1,
		D3D11_SDK_VERSION,
		&SwapChainDesc,
		&SwapChain,
		&Device,
		nullptr,
		&Context);
	
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackTex);	// __uuidof(ID3D11Texture2D)
	Device->CreateRenderTargetView(BackTex.Get(), nullptr, &RenderTargetView);

	CD3D11_RASTERIZER_DESC RastDesc = {};
	RastDesc.CullMode = D3D11_CULL_NONE;
	RastDesc.FillMode = D3D11_FILL_SOLID;
	
	Device->CreateRasterizerState(&RastDesc, RasterizerState.GetAddressOf());

	Context->RSSetState(RasterizerState.Get());
}

void FGame::InitGameObjects() const
{
	for (FGameObject* Object : GameObjects)
	{
		Object->Init();
	}
}

void FGame::UpdateGameObjects() const
{
	for (FGameObject* Object : GameObjects)
	{
		Object->Update();
	}
}

void FGame::DrawGameObjects() const
{
	for (FGameObject* Object : GameObjects)
	{
		Object->Draw();
	}
}

void FGame::BeginFrame()
{
	Context->OMSetRenderTargets(1, RenderTargetView.GetAddressOf(), nullptr);
	const float Color[] = { 1.0f, 0.1f, 0.1f, 1.0f };
	Context->ClearRenderTargetView(RenderTargetView.Get(), Color);
}

void FGame::RenderFrame()
{
	
}

void FGame::EndFrame()
{
	Context->OMSetRenderTargets(0, nullptr, nullptr);

	SwapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
}

FGame* FGame::Instance()
{
	if(!GameInstance)
	{
		GameInstance = new FGame();
	}
	return GameInstance;
}

void FGame::InternalUpdate()
{
	auto curTime = std::chrono::steady_clock::now();
	const float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() / 1000000.0f;
	PrevTime = curTime;

	totalTime += deltaTime;
	frameCount++;

	if (totalTime > 1.0f)
	{
		float fps = frameCount / totalTime;

		totalTime -= 1.0f;

		WCHAR text[256];
		swprintf_s(text, TEXT("FPS: %f"), fps);
		SetWindowText(Display->GetHWnd(), text);

		frameCount = 0;
	}

	GetContext()->ClearState();

	GetContext()->RSSetState(RasterizerState.Get());

	Viewport.Width = static_cast<float>(FGame::Instance()->GetDisplay().GetScreenWidth());
	Viewport.Height = static_cast<float>(FGame::Instance()->GetDisplay().GetScreenHeight());
	Viewport.MinDepth = 0;
	Viewport.MaxDepth = 1.0f;
	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;

	FGame::Instance()->GetContext()->RSSetViewports(1, &Viewport);
	
	BeginFrame();
	DrawGameObjects();
	EndFrame();
}

void FGame::Run()
{
	CreateResources();
	
	InitGameObjects();
	
	PrevTime = std::chrono::steady_clock::now();
	float totalTime = 0;
	unsigned int frameCount = 0;


	MSG msg = {};
	bool isExitRequested = false;
	while (!isExitRequested)
	{
	
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		if (msg.message == WM_QUIT)
		{
			isExitRequested = true;
		}
		
		InternalUpdate();
	}
}

FDisplayWin32& FGame::GetDisplay()
{
	return *Display;
}

Microsoft::WRL::ComPtr<ID3D11Device> FGame::GetDevice() const
{
	return Device;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> FGame::GetContext() const
{
	return Context;
}

void FGame::AddGameObject(FGameObject* ObjectToAdd)
{
	GameObjects.insert(ObjectToAdd);
}

void FGame::DeleteGameObject(FGameObject* ObjectToDelete)
{
	GameObjects.erase(ObjectToDelete);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			std::cout << "Key: " << static_cast<unsigned int>(wparam) << std::endl;
			
			return 0;
		}
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
	default:
		{
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
	}
}
