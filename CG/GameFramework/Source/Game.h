#pragma once
#include <chrono>

#include "Sources.h"

class FInputDevice;
class FDisplayWin32;
class FRenderSystem;
class FShadowsRenderSystem;
class FGameObject;
class FCameraComponent;
class FDirectionalLightComponent;

class FGame
{
public:

	FGame();
	FGame(const FGame&) = delete;
	void operator = (const FGame&) = delete;
	virtual ~FGame() = default;

	static FGame* Instance();
	static FGame* GameInstance;
	
	LPCWSTR	Name;
	int ClientWidth;
	int ClientHeight;
	
	float TotalTime;
	float DeltaTime;
	unsigned int FrameCount;
	std::chrono::time_point<std::chrono::steady_clock> StartTime;
	std::chrono::time_point<std::chrono::steady_clock> PrevTime;
	
	FDisplayWin32*  Display;
	FInputDevice*  InputDevice;
	FRenderSystem* RenderSystem;
	FShadowsRenderSystem* RenderShadows;

	FCameraComponent* CurrentCamera;
	FDirectionalLightComponent* CurrentLight;
	std::vector<FGameObject*> GameObjects;

	void Run();
	virtual void PrepareResources();
	virtual void Initialize();
	virtual void Update();
	virtual void UpdateInternal();
	virtual void Draw();
	virtual void DestroyResources();

	void AddGameObject(FGameObject* gameObject);

	FDisplayWin32*  GetDisplay();
	FInputDevice*   GetInputDevice();
	FRenderSystem*  GetRenderSystem();
	FShadowsRenderSystem* GetRenderShadowsSystem();

	LRESULT MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);