#include "CameraControllerComponent.h"
#include "CameraComponent.h"
#include "Components/TransformComponent.h"
#include <algorithm>

#include "Game.h"
#include "GameObjects/GameObject.h"

FCameraControllerComponent::FCameraControllerComponent()
{
    Target = nullptr;
    Yaw = 0;
    Pitch = 0;
    MinPitch = 0.0 / 180.0 * DirectX::XM_PI;
    MaxPitch = 90.0 / 180.0 * DirectX::XM_PI;
    ArmLength = 5.0f;
}

void FCameraControllerComponent::Initialize()
{
    FGame::Instance()->GetInputDevice()->MouseMove.AddRaw(this, &FCameraControllerComponent::MouseEventHandler);
    Pitch = MinPitch;
}

void FCameraControllerComponent::Update(float DeltaTime)
{
    if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::E))
    {
        std::cout << " X: " << GameObject->TransformComponent->GetLeft().x << " Y: " << GameObject->TransformComponent->GetLeft().y << " Z: " << GameObject->TransformComponent->GetLeft().z << std::endl;
    }
    const DirectX::SimpleMath::Vector3 Arm = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(0,0, - ArmLength), DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(Yaw, Pitch, 0));
    GameObject->TransformComponent->SetPosition(Target->GetPosition() + Arm);
    GameObject->TransformComponent->SetRotation(DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(Yaw, Pitch, 0));
}

void FCameraControllerComponent::MouseEventHandler(const FInputDevice::MouseMoveEventArgs& MouseData)
{
    Yaw   -= MouseData.Offset.x * 0.003f;
    Pitch += MouseData.Offset.y * 0.003f;
    
    if (Pitch > MaxPitch)
    {
        Pitch = MaxPitch;
    }
    
    if (Pitch < MinPitch)
    {
        Pitch = MinPitch;
    }
}