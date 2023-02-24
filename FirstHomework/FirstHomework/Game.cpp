#include "Game.h"

#include "DisplayWin32.h"
#include "GameObjects/GameObject.h"

#include <chrono>

Game::Game()
{
	ApplicationName = L"My game engine";
	ScreenWidth = 800;
	ScreenHeight = 800;
}

void Game::CreateResources()
{
	Display = new DisplayWin32(ApplicationName, ScreenWidth, ScreenHeight, WndProc);
	
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
}

void Game::InitGameObjects() const
{
	for (GameObject* Object : GameObjects)
	{
		Object->Init();
	}
}

void Game::UpdateGameObjects() const
{
	for (GameObject* Object : GameObjects)
	{
		Object->Update();
	}
}

void Game::DrawGameObjects() const
{
	for (GameObject* Object : GameObjects)
	{
		Object->Draw();
	}
}

void Game::BeginFrame()
{
	Context->OMSetRenderTargets(1, RenderTargetView.GetAddressOf(), nullptr);
	const float Color[] = { 1.0f, 0.1f, 0.1f, 1.0f };
	Context->ClearRenderTargetView(RenderTargetView.Get(), Color);
}

void Game::RenderFrame()
{
	
}

void Game::EndFrame()
{
	Context->OMSetRenderTargets(0, nullptr, nullptr);

	SwapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
}

Game* Game::Instance()
{
	if(!GameInstance)
	{
		GameInstance = new Game();
	}
	return GameInstance;
}

void Game::Run()
{
	CreateResources();
	
	InitGameObjects();
	
	std::chrono::time_point<std::chrono::steady_clock> PrevTime = std::chrono::steady_clock::now();
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

		Context->ClearState();

		Context->RSSetState(rastState);

		D3D11_VIEWPORT viewport = {};
		viewport.Width = static_cast<float>(ScreenWidth);
		viewport.Height = static_cast<float>(ScreenHeight);
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1.0f;

		Context->RSSetViewports(1, &viewport);

		Context->IASetInputLayout(layout);
		Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
		Context->IASetVertexBuffers(0, 1, &vb, strides, offsets);
		Context->VSSetShader(vertexShader, nullptr, 0);
		Context->PSSetShader(pixelShader, nullptr, 0);


		auto	curTime = std::chrono::steady_clock::now();
		float	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() / 1000000.0f;
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

		BeginFrame();
		DrawGameObjects();
		EndFrame();
	}
}

DisplayWin32& Game::GetDisplay()
{
	return *Display;
}

Microsoft::WRL::ComPtr<ID3D11Device> Game::GetDevice() const
{
	return Device;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> Game::GetContext() const
{
	return Context;
}

void Game::AddGameObject(GameObject* ObjectToAdd)
{
	GameObjects.insert(ObjectToAdd);
}

void Game::DeleteGameObject(GameObject* ObjectToDelete)
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

			if (static_cast<unsigned int>(wparam) == 27) PostQuitMessage(0);
			return 0;
		}
	default:
		{
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
	}
}
