#include "PlayerController.h"

#include "Game.h"
#include "Camera/Camera.h"
#include "GameObjects/GameObject.h"
#include "Utils/InputDevice.h"
#include "Utils/SimpleMath.h"

void FPlayerController::Init()
{
    FGameObject::Init();

    Camera->SetTranslation({0.0f, 1.0f, -1.0f});
}

void FPlayerController::Update(float DeltaTime)
{
    if(Player == nullptr)
    {
        return;
    }
    if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::A))
    {
        Player->AddRotationQuat(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(GetForwardVector(), + MovementSpeed * DeltaTime));
        Player->AddTranslation(MovementSpeed * DeltaTime * GetLeftVector());
        Camera->AddTranslation(MovementSpeed * DeltaTime * GetLeftVector());
    }
    if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::D))
    {
        Player->AddRotationQuat(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(GetForwardVector(), - MovementSpeed * DeltaTime));
        Player->AddTranslation(MovementSpeed * DeltaTime * GetRightVector());
        Camera->AddTranslation(MovementSpeed * DeltaTime * GetRightVector());
    }
    if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::W))
    {
        Player->AddRotationQuat(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(GetRightVector(), + MovementSpeed * DeltaTime));
        Player->AddTranslation(MovementSpeed * DeltaTime * GetForwardVector());
        Camera->AddTranslation(MovementSpeed * DeltaTime * GetForwardVector());
    }
    if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::S))
    {
        Player->AddRotationQuat(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(GetRightVector(), - MovementSpeed * DeltaTime));
        Player->AddTranslation(MovementSpeed * DeltaTime * GetBackwardVector());
        Camera->AddTranslation(MovementSpeed * DeltaTime * GetBackwardVector());
    }

    DirectX::SimpleMath::Vector3 NewCameraTarget = GetWorldTranslation() + DirectX::SimpleMath::Vector3::Transform({ 0.0f, 50.0f, 50.0f }, DirectX::SimpleMath::Matrix::Identity);
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
}

