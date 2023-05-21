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
	name = L"My Game Engine";
	clientWidth = 800;
	clientHeight = 800;
	totalTime = 0;
	deltaTime = 0;
	frameCount = 0;
	startTime = std::make_shared<std::chrono::time_point<std::chrono::steady_clock>>();
	prevTime = std::make_shared<std::chrono::time_point<std::chrono::steady_clock>>();
	currentCamera = nullptr;
	currentLight  = nullptr;
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
	display = std::make_shared<FDisplayWin32>(name, clientWidth, clientHeight, WndProc);
	inputDevice = std::make_shared<InputDevice>(this);
	render = std::make_shared<FRenderSystem>();
	renderShadows = std::make_shared<FShadowsRenderSystem>();
}

void FGame::Initialize()
{
	for (auto& object : gameObjects) {
		object->Initialize();
	}
}


void FGame::Run()
{
	PrepareResources();
	Initialize();

	*startTime = std::chrono::steady_clock::now();
	*prevTime = *startTime;

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
				SetWindowText(FGame::Instance()->GetDisplay()->GetHWnd(), text);
				frameCount = 0;
			}
			Update();
			Draw();
		}
	}
	DestroyResources();
}

void FGame::UpdateInternal()
{
	if (inputDevice->IsKeyDown(Keys::Escape))
	{
		PostQuitMessage(0);
	}
}

void FGame::Update()
{
	UpdateInternal();
	for (auto& object : gameObjects)
	{
		object->Update(deltaTime);
	}
}

void FGame::Draw()
{
	renderShadows->PrepareFrame();
	renderShadows->Draw();
	renderShadows->EndFrame();
	render->PrepareFrame();
	render->Draw();
	render->EndFrame();
}

void FGame::DestroyResources()
{
	for (auto& object : gameObjects)
	{
		delete object;
	}
	gameObjects.clear();
}

void FGame::AddGameObject(FGameObject* gameObject)
{
	gameObjects.push_back(gameObject);
}

std::shared_ptr<FDisplayWin32>  FGame::GetDisplay()
{
	return display;
}

std::shared_ptr<InputDevice>   FGame::GetInputDevice()
{
	return inputDevice;
}

std::shared_ptr<FRenderSystem>  FGame::GetRenderSystem()
{
	return render;
}

std::shared_ptr<FShadowsRenderSystem> FGame::GetRenderShadowsSystem()
{
	return renderShadows;
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