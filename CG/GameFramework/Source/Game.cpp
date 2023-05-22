#include "Game.h"

#include "DisplayWin32.h"
#include "Utils/InputDevice.h"
#include "Render/RenderSystem.h"
#include "Render/ShadowsRenderSystem.h"
#include "GameObjects/GameObject.h"
#include "Components/CameraComponent.h"
#include "Components/Light/DirectionalLightComponent.h"

FGame* FGame::GameInstance = nullptr;

FGame::FGame()
{
	Name = L"My Game Engine";
	ClientWidth = 800;
	ClientHeight = 800;
	TotalTime = 0;
	DeltaTime = 0;
	FrameCount = 0;
	StartTime = std::chrono::time_point<std::chrono::steady_clock>();
	PrevTime = std::chrono::time_point<std::chrono::steady_clock>();
	CurrentCamera = nullptr;
	CurrentLight  = nullptr;
}

FGame* FGame::Instance()
{
	if (!GameInstance)
	{
		GameInstance = new FGame();
	}
	return GameInstance;
}

void FGame::PrepareResources()
{
	Display = new FDisplayWin32(Name, ClientWidth, ClientHeight, WndProc);
	InputDevice = new FInputDevice(this);
	RenderSystem =  new FRenderSystem();
	RenderShadows = new FShadowsRenderSystem();
}

void FGame::Initialize()
{
	for (const auto& Object : GameObjects)
	{
		Object->Initialize();
	}
}

void FGame::Run()
{
	PrepareResources();
	Initialize();

	StartTime = std::chrono::steady_clock::now();
	PrevTime = StartTime;

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			auto curTime = std::chrono::steady_clock::now();
			DeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() / 1000000.0f;
			PrevTime = curTime;
			TotalTime += DeltaTime;
			FrameCount++;
			if (TotalTime > 1.0f)
			{
				float fps = FrameCount / TotalTime;
				TotalTime -= 1.0f;
				WCHAR text[256];
				swprintf_s(text, TEXT("FPS: %f"), fps);
				SetWindowText(FGame::Instance()->GetDisplay()->GetHWnd(), text);
				FrameCount = 0;
			}
			Update();
			Draw();
		}
	}
	DestroyResources();
}

void FGame::UpdateInternal()
{
	if (InputDevice->IsKeyDown(Keys::Escape))
	{
		PostQuitMessage(0);
	}
}

void FGame::Update()
{
	UpdateInternal();
	for (const auto& Object : GameObjects)
	{
		Object->Update(DeltaTime);
	}
}

void FGame::Draw()
{
	RenderShadows->PrepareFrame();
	RenderShadows->Draw();
	RenderShadows->EndFrame();
	
	RenderSystem->PrepareFrame();
	RenderSystem->Draw();
	RenderSystem->EndFrame();
}

void FGame::DestroyResources()
{
	for (const FGameObject* object : GameObjects)
	{
		delete object;
	}
	GameObjects.clear();
}

void FGame::AddGameObject(FGameObject* gameObject)
{
	GameObjects.push_back(gameObject);
}

FDisplayWin32*  FGame::GetDisplay()
{
	return Display;
}

FInputDevice*   FGame::GetInputDevice()
{
	return InputDevice;
}

FRenderSystem*  FGame::GetRenderSystem()
{
	return RenderSystem;
}

FShadowsRenderSystem* FGame::GetRenderShadowsSystem()
{
	return RenderShadows;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return FGame::Instance()->MessageHandler(hwnd, umessage, wparam, lparam);
}

LRESULT FGame::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	{
		switch (umessage)
		{
			case WM_CLOSE:
			{
				PostQuitMessage(0);
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
					FGame::Instance()->GetInputDevice()->OnKeyDown({
						raw->data.keyboard.MakeCode,
						raw->data.keyboard.Flags,
						raw->data.keyboard.VKey,
						raw->data.keyboard.Message
						});
				}
				else if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					FGame::Instance()->GetInputDevice()->OnMouseMove({
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
}