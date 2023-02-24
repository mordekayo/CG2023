#pragma once

#include <chrono>
#include <windows.h>
#include <iostream>
#include <wrl.h>
#include <d3d.h>
#include <d3d11.h>
#include <set>

class DisplayWin32;
class GameObject;

class Game
{
public:
	
	Game(const Game&) = delete;
	void operator = (const Game&) = delete;

	static Game* Instance();
	void InternalUpdate();

	void Run();

	DisplayWin32& GetDisplay();
	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() const;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() const;

	void AddGameObject(GameObject* ObjectToAdd);
	void DeleteGameObject(GameObject* ObjectToDelete);
	
private:

	Game();
	static Game* GameInstance;

	DisplayWin32* Display;
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
	
	std::set<GameObject*> GameObjects;
	
	LPCWSTR ApplicationName;
	int ScreenWidth;
	int ScreenHeight;

	std::chrono::time_point<std::chrono::steady_clock> PrevTime;
	float totalTime = 0;
	unsigned frameCount = 0;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

