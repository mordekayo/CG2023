#include "Game.h"

#include "DisplayWin32.h"
#include "GameObjects/GameObject.h"
#include "Utils/InputDevice.h"
#include "Sources.h"
#include <chrono>

#include "Camera/Camera.h"
#include "Camera/FPSCameraController.h"
#include "Camera/TargetCameraController.h"

FGame::FGame()
{
	ApplicationName = L"My FGame engine";
	ScreenWidth = 800;
	ScreenHeight = 800;
}

void FGame::CreateResources()
{
	Display = new FDisplayWin32(ApplicationName, ScreenWidth, ScreenHeight, WndProc);
	Input = new InputDevice(this);
	Camera = new FCamera();

	FPSCameraController = new FFPSCameraController();
	FPSCameraController->SetCamera(Camera);
	
	TargetCameraController = new FTargetCameraController();
	TargetCameraController->SetCamera(Camera);

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

void FGame::UpdateGameObjects(float DeltaTime) const
{
	for (FGameObject* Object : GameObjects)
	{
		Object->Update(DeltaTime);
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
	const float Color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
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

void FGame::Update(float DeltaTime)
{
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

	UpdateGameObjects(DeltaTime);
	
	BeginFrame();
	DrawGameObjects();
	EndFrame();
	
	if (bIsFPS)
	{
		FPSCameraController->Update(DeltaTime);
	}
	else
	{
		TargetCameraController->Update(DeltaTime);
	}
}

void FGame::Construct()
{
	for(const auto GameObject : GameObjects)
	{
		GameObject->Construct();
	}
}

void FGame::Run()
{
	CreateResources();
	
	Construct();
	InitGameObjects();
	
	
	PrevTime = std::chrono::steady_clock::now();
	float totalTime = 0;
	unsigned int frameCount = 0;


	MSG msg = {};
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
		
		const auto CurTime = std::chrono::steady_clock::now();
		const float DeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(CurTime - PrevTime).count() / 1000000.0f;
		PrevTime = CurTime;
		totalTime += DeltaTime;
		frameCount++;
		
		Update(DeltaTime);
	}
}

FDisplayWin32& FGame::GetDisplay()
{
	return *Display;
}

InputDevice* FGame::GetInputDevice() const
{
	return Input;
}

Microsoft::WRL::ComPtr<ID3D11Device> FGame::GetDevice() const
{
	return Device;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> FGame::GetContext() const
{
	return Context;
}

FCamera* FGame::GetCamera() const
{
	return Camera;
}

void FGame::AddGameObject(FGameObject* ObjectToAdd)
{
	GameObjects.insert(ObjectToAdd);
}

void FGame::AddGameObjects(std::vector<FGameObject*> ObjectsToAdd)
{
	GameObjects.insert(ObjectsToAdd.cbegin(), ObjectsToAdd.cend());
}

void FGame::DeleteGameObject(FGameObject* ObjectToDelete)
{
	GameObjects.erase(ObjectToDelete);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return FGame::Instance()->MessageHandler(hwnd, umessage, wparam, lparam);
}

LRESULT FGame::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	switch (umessage)
	{
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			isExitRequested = true;
			return 0;
		}
	case WM_INPUT:
	{
		UINT dwSize = 0;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];

		if (lpb == nullptr)
		{
			return 0;
		}

		if (GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
		{
			OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));
		}

		RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb);

		if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{
			//printf(" Kbd: make=%04i Flags:%04i Reserved:%04i ExtraInformation:%08i, msg=%04i VK=%i \n",
			//	raw->data.keyboard.MakeCode,
			//	raw->data.keyboard.Flags,
			//	raw->data.keyboard.Reserved,
			//	raw->data.keyboard.ExtraInformation,
			//	raw->data.keyboard.Message,
			//	raw->data.keyboard.VKey);

			Input->OnKeyDown({
				raw->data.keyboard.MakeCode,
				raw->data.keyboard.Flags,
				raw->data.keyboard.VKey,
				raw->data.keyboard.Message
				});
		}
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			//printf(" Mouse: X=%04d Y:%04d \n", raw->data.mouse.lLastX, raw->data.mouse.lLastY);
			Input->OnMouseMove({
				raw->data.mouse.usFlags,
				raw->data.mouse.usButtonFlags,
				static_cast<int>(raw->data.mouse.ulExtraInformation),
				static_cast<int>(raw->data.mouse.ulRawButtons),
				static_cast<short>(raw->data.mouse.usButtonData),
				raw->data.mouse.lLastX,
				raw->data.mouse.lLastY
				});
		}
		delete[] lpb;
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	}
}

FTargetCameraController* FGame::GetTargetCameraController()
{
	return TargetCameraController;
}
