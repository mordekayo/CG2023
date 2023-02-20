#include "Game.h"

Game* Game::instance = nullptr;
Game::Game(LPCWSTR applicationName, int screenWidth, int screenHeight)
{
	this->applicationName = applicationName;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	frameCount = 0;
	isExitRequested = false;
	totalTime = 0;
}
Game* Game::CreateInstance(LPCWSTR applicationName, int screenWidth, int screenHeight)
{
	if (!instance)
	{
		instance = new Game(applicationName, screenWidth, screenHeight);
	}
	return instance;
}
Game* Game::GetInstance()
{
	if (!instance)
	{
		CreateInstance();
	}
	return instance;
}

void Game::PrepareResources()
{
	D3D_FEATURE_LEVEL featureLevel[1] = { D3D_FEATURE_LEVEL_11_1 };

	swapDesc = std::make_shared<DXGI_SWAP_CHAIN_DESC>();

	*(swapDesc.get()) = { 0 };
	swapDesc.get()->BufferCount = 2;
	swapDesc.get()->BufferDesc.Width = display->GetScreenWidth();
	swapDesc.get()->BufferDesc.Height = display->GetScreenHeight();
	swapDesc.get()->BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.get()->BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.get()->BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.get()->BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.get()->BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.get()->BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.get()->OutputWindow = display->GetHWnd();
	swapDesc.get()->Windowed = true;
	swapDesc.get()->SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.get()->Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.get()->SampleDesc.Count = 1;
	swapDesc.get()->SampleDesc.Quality = 0;

	res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		swapDesc.get(),
		&swapChain,
		&device,
		nullptr,
		&context
	);

	if (FAILED(res)) { /* Well, that was unexpected */ }

	res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backTex);
	res = device->CreateRenderTargetView(backTex.Get(), nullptr, &rtv);
}

void Game::Initialize()
{
	for (auto component : components)
	{
		component->Initialize();
	}
}

void Game::Run()
{
	display = std::make_shared<DisplayWin32>(applicationName, screenWidth, screenHeight, WndProc);

	PrepareResources();

	Initialize();

	startTime = std::make_shared<std::chrono::time_point<std::chrono::steady_clock>>();
	prevTime = std::make_shared<std::chrono::time_point<std::chrono::steady_clock>>();
	*startTime = std::chrono::steady_clock::now();
	*prevTime = *startTime;

	MSG msg = {};
	//isExitRequested = false;
	while (!isExitRequested)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))  // ����������� ��������� ���������
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			isExitRequested = true;
		}
		Update();
		UpdateInternal();
	}
}

void Game::Update()
{
	for (auto component : components)
	{
		component->Update();
	}
}

void Game::UpdateInternal()
{
	auto curTime = std::chrono::steady_clock::now();
	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - *prevTime).count() / 1000000.0f;
	*prevTime = curTime;
	totalTime += deltaTime;
	frameCount++;
	if (totalTime > 1.0f)
	{
		float fps = frameCount / totalTime;
		totalTime -= 1.0f;
		WCHAR text[256];
		swprintf_s(text, TEXT("FPS: %f"), fps);
		SetWindowText(display->GetHWnd(), text);
		frameCount = 0;
	}
	PrepareFrame();
	Draw();
	EndFrame();
}

void Game::PrepareFrame()
{
	context->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);
	float color[] = { Game::GetInstance()->GetTotalTime(), 0.1f, 0.1f, 1.0f };
	context->ClearRenderTargetView(rtv.Get(), color);
}

void Game::Draw()
{
	for (auto component : components)
	{
		component->Draw();
	}
}

void Game::EndFrame()
{
	context->OMSetRenderTargets(0, nullptr, nullptr);
	swapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
}

void Game::DestroyResources()
{
	// there're nothing to release in Game class
	for (auto component : components)
	{
		component->DestroyResources();
	}
}

//

std::shared_ptr<DisplayWin32> Game::GetDisplay() { return display; }
HRESULT Game::GetRes() { return res; }
Microsoft::WRL::ComPtr<ID3D11Device> Game::GetDevice() { return device; }
Microsoft::WRL::ComPtr<ID3D11RenderTargetView> Game::GetRTV() { return rtv; }
Microsoft::WRL::ComPtr<IDXGISwapChain> Game::GetSwapChain() { return swapChain; }
Microsoft::WRL::ComPtr<ID3D11DeviceContext> Game::GetContext() { return context; }

DirectX::SimpleMath::Vector4* Game::GetOffset() { return &offset; }

void Game::SetRes(HRESULT res) { this->res = res; }

float Game::GetTotalTime() { return totalTime; }
void Game::SetTotalTime(float totalTime) { this->totalTime = totalTime; }
unsigned int Game::GetFrameCount() { return frameCount; }
void Game::SetFrameCount(unsigned int frameCount) { this->frameCount = frameCount; }

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return Game::GetInstance()->MessageHandler(hwnd, umessage, wparam, lparam);
}
LRESULT Game::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_KEYDOWN:
	{
		std::cout << "Key: " << static_cast<unsigned int>(wparam) << std::endl;
		auto key = static_cast<unsigned int>(wparam);
		if (key == 27) { PostQuitMessage(0); }
		if (key == 37) { offset -= { 1.0f * deltaTime, 0.0f, 0.0f, 0.0f }; }
		if (key == 39) { offset += { 1.0f * deltaTime, 0.0f, 0.0f, 0.0f }; }
		if (key == 40) { offset -= { 0.0f, 1.0f * deltaTime, 0.0f, 0.0f }; }
		if (key == 38) { offset += { 0.0f, 1.0f * deltaTime, 0.0f, 0.0f }; }
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