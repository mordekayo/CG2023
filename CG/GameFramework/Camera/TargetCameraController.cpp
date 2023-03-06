#include "TargetCameraController.h"

#include <DirectXMath.h>

#include "Camera.h"
#include "../../GameFramework/Game.h"
#include "../GameObjects/GameObject.h"

FTargetCameraController::FTargetCameraController()
{
    FGame::Instance()->GetInputDevice()->MouseMove.AddRaw(this, &FTargetCameraController::MouseEventHandler, 10);

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
        Camera->AddTransform(CameraRight * DeltaTime);
    }
    if (Input->IsKeyDown(Keys::D)) 
    {
        Camera->AddTransform(-CameraRight * DeltaTime);
    }
    if (Input->IsKeyDown(Keys::W))
    {
        Camera->AddTransform(CameraForward * DeltaTime);
    }
    if (Input->IsKeyDown(Keys::S))
    {
        Camera->AddTransform(-CameraForward * DeltaTime);
    }
    if (Input->IsKeyDown(Keys::E))
    {
        Camera->AddTransform(CameraUp * DeltaTime);
    }
    if (Input->IsKeyDown(Keys::Q))
    {
        Camera->AddTransform(-CameraUp * DeltaTime);
    }
        if (Input->IsKeyDown(Keys::A))
    {
        Camera->AddTransform(CameraRight * DeltaTime);
    }
    if (Input->IsKeyDown(Keys::D)) 
    {
        Camera->AddTransform(-CameraRight * DeltaTime);
    }
    if (Input->IsKeyDown(Keys::W))
    {
        Camera->AddTransform(CameraForward * DeltaTime);
    }
    if (Input->IsKeyDown(Keys::S))
    {
        Camera->AddTransform(-CameraForward * DeltaTime);
    }
    if (Input->IsKeyDown(Keys::E))
    {
        Camera->AddTransform(CameraUp * DeltaTime);
    }
    if (Input->IsKeyDown(Keys::Q))
    {
        Camera->AddTransform(-CameraUp * DeltaTime);
    }
}

void FTargetCameraController::MouseEventHandler(const InputDevice::MouseMoveEventArgs& mouseData, int payload)
{
    CameraYaw += -mouseData.Offset.x * CameraRotationSpeed;
    CameraPitch += mouseData.Offset.y * CameraRotationSpeed;

    if (CameraPitch > DirectX::XM_PIDIV2 - CameraRotationSpeed)
    {
        CameraPitch = DirectX::XM_PIDIV2 - CameraRotationSpeed;
    }
    if (CameraPitch < -DirectX::XM_PIDIV2 + CameraRotationSpeed)
    {
        CameraPitch = -DirectX::XM_PIDIV2 + CameraRotationSpeed;
    }
}

void FTargetCameraController::SetCamera(FCamera* NewCamera)
{
    Camera = NewCamera;
}

void FTargetCameraController::SetTarget(FGameObject* Object)
{
    Target = Object;
}
