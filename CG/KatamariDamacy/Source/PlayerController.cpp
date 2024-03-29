#include "PlayerController.h"

#include "Game.h"
#include "Camera/Camera.h"
#include "GameObjects/GameObject.h"
#include "Utils/InputDevice.h"
#include "Utils/SimpleMath.h"

FPlayerController::FPlayerController()
{
    FGame::Instance()->GetInputDevice()->MouseMove.AddRaw(this, &FPlayerController::MouseEventHandler, 10);
}

void FPlayerController::Init()
{
    FGameObject::Init();

    
}

void FPlayerController::Update(float DeltaTime)
{
    if(Player == nullptr)
    {
        return;
    }
    
    const DirectX::SimpleMath::Matrix RotationMatrix = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(CameraYaw, CameraPitch, 0);

    if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::W))
    {
        Player->AddRotationQuat(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(GetLeftVector(), MovementSpeed * RotationByMovement * DeltaTime));
        AddTranslation(MovementSpeed * DeltaTime * GetForwardVector());
        Player->AddTranslation(MovementSpeed * DeltaTime * GetForwardVector());
    }
    if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::A))
    {
        Player->AddRotationQuat(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(GetForwardVector(), - MovementSpeed * RotationByMovement * DeltaTime));
        AddTranslation(MovementSpeed * DeltaTime * GetLeftVector());
        Player->AddTranslation(MovementSpeed * DeltaTime * GetLeftVector());
    }
    if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::S))
    {
        Player->AddRotationQuat(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(GetLeftVector(), -MovementSpeed * RotationByMovement * DeltaTime));
        AddTranslation(MovementSpeed * DeltaTime * GetBackwardVector());
        Player->AddTranslation(MovementSpeed * DeltaTime * GetBackwardVector());
    }
    if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::D))
    {
        Player->AddRotationQuat(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(GetForwardVector(), + MovementSpeed * RotationByMovement * DeltaTime));
        AddTranslation(MovementSpeed * DeltaTime * GetRightVector());
        Player->AddTranslation(MovementSpeed * DeltaTime * GetRightVector());
    }
    
    SetLocalRotationQuat(DirectX::SimpleMath::Quaternion::CreateFromRotationMatrix(RotationMatrix));
    
    const DirectX::SimpleMath::Vector3 NewCameraTarget = GetWorldTranslation();
    Camera->SetTarget(NewCameraTarget);

    Camera->Update(DeltaTime);
    
    FGameObject::Update(DeltaTime);
}

void FPlayerController::Possess(FGameObject* NewPlayer)
{
    Player = NewPlayer;
}

void FPlayerController::SetCamera(FCamera* NewCamera)
{
    Camera = NewCamera;
    Camera->SetTranslation({0.0f, 0.3f, -0.3f});
    Camera->SetParent(this);
}

FCamera* FPlayerController::GetCamera() const
{
    return Camera;
}

void FPlayerController::MouseEventHandler(const InputDevice::MouseMoveEventArgs& mouseData, int payload)
{
    CameraYaw += -mouseData.Offset.x * CameraRotationSpeed;
}

