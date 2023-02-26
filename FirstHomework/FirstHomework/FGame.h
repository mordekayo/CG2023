#pragma once

#include <chrono>
#include <windows.h>
#include <iostream>
#include <wrl.h>
#include <d3d.h>
#include <d3d11.h>
#include <set>

class FDisplayWin32;
class FGameObject;

class FGame
{
public:
	
	FGame(const FGame&) = delete;
	void operator = (const FGame&) = delete;

	static FGame* Instance();
	void InternalUpdate();

	void Run();

	FDisplayWin32& GetDisplay();
	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() const;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() const;

	void AddGameObject(FGameObject* ObjectToAdd);
	void DeleteGameObject(FGameObject* ObjectToDelete);
	
private:

	FGame();
	static FGame* GameInstance;

	FDisplayWin32* Display;
	Microsoft::WRL::ComPtr<ID3D11Device> Device = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> BackTex = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetView = nullptr;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerState;
	D3D11_VIEWPORT Viewport;
	
	void CreateResources();
	
	void InitGameObjects() const;
	void UpdateGameObjects() const;
	void DrawGameObjects() const;

	void BeginFrame();
	void RenderFrame();
	void EndFrame();
	
	std::set<FGameObject*> GameObjects;
	
	LPCWSTR ApplicationName;
	int ScreenWidth;
	int ScreenHeight;

	std::chrono::time_point<std::chrono::steady_clock> PrevTime;
	float totalTime = 0;
	unsigned frameCount = 0;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

