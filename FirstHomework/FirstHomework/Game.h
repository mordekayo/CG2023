#pragma once

#include "Resources.h"

class DisplayWin32;
class GameComponent;

class Game
{

private:

	LPCWSTR applicationName;
	int screenWidth;
	int screenHeight;
	Game(LPCWSTR applicationName, int screenWidth, int screenHeight);
	static Game* instance;

	std::shared_ptr<DisplayWin32> display;
	bool isExitRequested;
	float deltaTime;

	std::shared_ptr<DXGI_SWAP_CHAIN_DESC> swapDesc;
	HRESULT res = NULL;
	Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backTex = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;

	DirectX::SimpleMath::Vector4 offset = {};

public:

	Game(const Game&) = delete;
	void operator = (const Game&) = delete;
	static Game* CreateInstance(LPCWSTR applicationName = L" My First Game Engine ", int screenWidth = 800, int screenHeight = 800);
	static Game* GetInstance();

	std::vector<GameComponent*> components;

	std::shared_ptr<DisplayWin32> GetDisplay();
	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext();
	Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain();
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetRTV();
	HRESULT GetRes();
	void SetRes(HRESULT res);

	DirectX::SimpleMath::Vector4* GetOffset();

	//

	void Run();
	void PrepareResources();
	void Initialize();
	void UpdateInternal();
	void PrepareFrame();
	void Update();
	void Draw();
	void EndFrame();
	void DestroyResources();

	//

	std::shared_ptr<std::chrono::time_point<std::chrono::steady_clock>> startTime;
	std::shared_ptr<std::chrono::time_point<std::chrono::steady_clock>> prevTime;
	float totalTime;
	unsigned int frameCount;

	float GetTotalTime();
	void SetTotalTime(float totalTime);
	unsigned int GetFrameCount();
	void SetFrameCount(unsigned int frameCount);

	LRESULT MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
};
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


