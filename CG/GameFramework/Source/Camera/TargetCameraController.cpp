#include "TargetCameraController.h"

#include <DirectXMath.h>

#include "Camera.h"
#include "Game.h"
#include "GameObjects/GameObject.h"

FTargetCameraController::FTargetCameraController()
{
    Input = FGame::Instance()->GetInputDevice();
}

void FTargetCameraController::Update(float DeltaTime)
{
    if(Camera == nullptr)
    {
        return;
    }
    
    if (Input->IsKeyDown(Keys::P))
    {
        if (!WasProjectionKeyDown)
        {
            WasProjectionKeyDown = true;
            Camera->TogglePerspective();
        }
    }
    else
    {
        WasProjectionKeyDown = false;
    }

    const DirectX::SimpleMath::Matrix RotationMatrix = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(0, 0, 0);

    const DirectX::SimpleMath::Vector3 CameraForward = DirectX::SimpleMath::Vector3::Transform({0.0f, 0.0f, 1.0f}, RotationMatrix);
    const DirectX::SimpleMath::Vector3 CameraRight = DirectX::SimpleMath::Vector3::Transform({1.0f , 0.0f, 0.0f}, RotationMatrix);
    const DirectX::SimpleMath::Vector3 CameraUp = DirectX::SimpleMath::Vector3::Transform({ 0.0f , 1.0f, 0.0f }, RotationMatrix);

    if (Input->IsKeyDown(Keys::A))
    {
        Camera->AddTranslation(CameraRight * DeltaTime);
    }
    if (Input->IsKeyDown(Keys::D)) 
    {
        Camera->AddTranslation(-CameraRight * DeltaTime);
    }
    if (Input->IsKeyDown(Keys::W))
    {
        Camera->AddTranslation(CameraForward * DeltaTime);
    }
    if (Input->IsKeyDown(Keys::S))
    {
        Camera->AddTranslation(-CameraForward * DeltaTime);
    }
    if (Input->IsKeyDown(Keys::E))
    {
        Camera->AddTranslation(CameraUp * DeltaTime);
    }
    if (Input->IsKeyDown(Keys::Q))
    {
        Camera->AddTranslation(-CameraUp * DeltaTime);
    }
        if (Input->IsKeyDown(Keys::A))
    {
        Camera->AddTranslation(CameraRight * DeltaTime);
    }
    if (Input->IsKeyDown(Keys::D)) 
    {
        Camera->AddTranslation(-CameraRight * DeltaTime);
    }
    if (Input->IsKeyDown(Keys::W))
    {
        Camera->AddTranslation(CameraForward * DeltaTime);
    }
    if (Input->IsKeyDown(Keys::S))
    {
        Camera->AddTranslation(-CameraForward * DeltaTime);
    }
    if (Input->IsKeyDown(Keys::E))
    {
        Camera->AddTranslation(CameraUp * DeltaTime);
    }
    if (Input->IsKeyDown(Keys::Q))
    {
        Camera->AddTranslation(-CameraUp * DeltaTime);
    }

    DirectX::SimpleMath::Vector3 NewCameraTarget = Target->GetLocalTranslation();
    Camera->SetTarget(NewCameraTarget);

    Camera->Update(DeltaTime);
}

void FTargetCameraController::SetCamera(FCamera* NewCamera)
{
    Camera = NewCamera;
}

void FTargetCameraController::SetTarget(FGameObject* Object)
{
    Target = Object;
}
