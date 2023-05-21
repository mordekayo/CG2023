#include "CameraComponent.h"
#include "Game.h"
#include "DisplayWin32.h"
#include "CameraControllerComponent.h"

FCameraComponent::FCameraComponent()
{
    aspect = 1.0f;
    nearZ = 0.1f;
    farZ = 100.0f;
}

void FCameraComponent::Initialize()
{
    aspect = static_cast<float>(FGame::Instance()->GetDisplay()->GetScreenWidth()) /
        static_cast<float>(FGame::Instance()->GetDisplay()->GetScreenHeight());
}

DirectX::SimpleMath::Matrix FCameraComponent::GetProjection()
{
    DirectX::SimpleMath::Matrix rotation = DirectX::SimpleMath::Matrix::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, DirectX::XM_PI);
    return rotation * DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
        fovAngle,
        aspect,
        nearZ,
        farZ
    );
}