#pragma once
#include <chrono>

#include "Sources.h"

class FDisplayWin32;
class InputDevice;
class FRenderSystem;
class FShadowsRenderSystem;
class FGameObject;
class FCameraComponent;
class FDirectionalLightComponent;

class FGame
{
public:

	LPCWSTR	name;
	int clientWidth;
	int clientHeight;
	static FGame* GameInstance;
	FGame();
	FGame(const FGame&) = delete;
	void operator = (const FGame&) = delete;
	virtual ~FGame() = default;
	static FGame* Instance();
	float totalTime;
	float deltaTime;
	unsigned int frameCount;
	std::shared_ptr<std::chrono::time_point<std::chrono::steady_clock>> startTime;
	std::shared_ptr<std::chrono::time_point<std::chrono::steady_clock>> prevTime;
	std::shared_ptr<FDisplayWin32>  display;
	std::shared_ptr<InputDevice>   inputDevice;
	std::shared_ptr<FRenderSystem>  render;
	std::shared_ptr<FShadowsRenderSystem> renderShadows;

	FCameraComponent* currentCamera;
	FDirectionalLightComponent* currentLight;
	std::vector<FGameObject*> gameObjects;

	void Run();
	virtual void PrepareResources();
	virtual void Initialize();
	virtual void Update();
	virtual void UpdateInternal();
	virtual void Draw();
	virtual void DestroyResources();

	void AddGameObject(FGameObject* gameObject);

	std::shared_ptr<FDisplayWin32>  GetDisplay();
	std::shared_ptr<InputDevice>   GetInputDevice();
	std::shared_ptr<FRenderSystem>  GetRenderSystem();
	std::shared_ptr<FShadowsRenderSystem> GetRenderShadowsSystem();

	LRESULT MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);