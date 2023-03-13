#pragma once

#include <chrono>
#include <windows.h>
#include <wrl.h>
#include <d3d.h>
#include <d3d11.h>
#include <set>

class FRenderSystem;
class FTargetCameraController;
class FFPSCameraController;
class FCamera;
class FDisplayWin32;
class FGameObject;
class InputDevice;

class FGame
{
public:
	
	FGame(const FGame&) = delete;
	void operator = (const FGame&) = delete;
	virtual ~FGame() = default;

	static FGame* Instance();
	static FRenderSystem* GetRenderSystem();

	virtual void Run();

	FDisplayWin32& GetDisplay();
	InputDevice* GetInputDevice() const;
	FCamera* GetCamera() const;

	void AddGameObject(FGameObject* ObjectToAdd);
	void AddGameObjects(std::vector<FGameObject*> ObjectsToAdd);
	void DeleteGameObject(FGameObject* ObjectToDelete);
	
	LRESULT MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

	bool bIsFPS = true;

	FTargetCameraController* GetTargetCameraController();
protected:

	inline static FGame* GameInstance = nullptr;

	FGame();

	virtual void Update(float DeltaTime);

	virtual void Construct();
	InputDevice* Input;

	LPCWSTR ApplicationName;
	int ScreenWidth;
	int ScreenHeight;

	FCamera* Camera;
	FFPSCameraController* FPSCameraController;
	FTargetCameraController* TargetCameraController;

private:

	FRenderSystem* RenderSystem;
	FDisplayWin32* Display;
	
	void CreateResources();
	
	void InitGameObjects() const;
	void UpdateGameObjects(float DeltaTime) const;
	void DrawGameObjects() const;
	
	std::set<FGameObject*> GameObjects;
	
	std::chrono::time_point<std::chrono::steady_clock> PrevTime;
	float totalTime = 0;
	unsigned frameCount = 0;

	bool isExitRequested = false;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

