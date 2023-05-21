#include "CameraControllerComponent.h"
#include "CameraComponent.h"
#include "Components/TransformComponent.h"
#include <algorithm>

#include "Game.h"
#include "GameObjects/GameObject.h"

FCameraControllerComponent::FCameraControllerComponent()
{
    this->aim = nullptr;
    this->yaw = 0;
    this->pitch = 0;
    this->minPitch = 0.0 / 180.0 * DirectX::XM_PI;
    this->maxPitch = 90.0 / 180.0 * DirectX::XM_PI;
    this->armLength = 5.0f;
}

void FCameraControllerComponent::Initialize()
{
    FGame::Instance()->GetInputDevice()->MouseMove.AddRaw(this, &FCameraControllerComponent::MouseEventHandler);
    this->pitch = minPitch;
}

void FCameraControllerComponent::Update(float deltaTime)
{
    if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::E))
    {
        std::cout << " X: " << gameObject->TransformComponent->GetLeft().x << " Y: " << gameObject->TransformComponent->GetLeft().y << " Z: " << gameObject->TransformComponent->GetLeft().z << std::endl;
    }
    DirectX::SimpleMath::Vector3 arm = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(0,0, - armLength), DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(yaw, pitch, 0));
    gameObject->TransformComponent->SetPosition(aim->GetPosition() + arm);
    gameObject->TransformComponent->SetRotation(DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(yaw, pitch, 0));
}

void FCameraControllerComponent::MouseEventHandler(const InputDevice::MouseMoveEventArgs& mouseData)
{
    yaw   -= mouseData.Offset.x * 0.003f;
    pitch += mouseData.Offset.y * 0.003f;
    if (pitch > maxPitch) { pitch = maxPitch; }
    if (pitch < minPitch) { pitch = minPitch; }
}