#include "CameraComponent.h"
#include "Game.h"
#include "DisplayWin32.h"
#include "CameraControllerComponent.h"

FCameraComponent::FCameraComponent()
{
    Aspect = 1.0f;
    NearZ = 0.1f;
    FarZ = 100.0f;
}

void FCameraComponent::Initialize()
{
    Aspect = static_cast<float>(FGame::Instance()->GetDisplay()->GetScreenWidth()) /
        static_cast<float>(FGame::Instance()->GetDisplay()->GetScreenHeight());
}

DirectX::SimpleMath::Matrix FCameraComponent::GetProjection()
{
    DirectX::SimpleMath::Matrix rotation = DirectX::SimpleMath::Matrix::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, DirectX::XM_PI);
    return rotation * DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
        FovAngle,
        Aspect,
        NearZ,
        FarZ
    );
}