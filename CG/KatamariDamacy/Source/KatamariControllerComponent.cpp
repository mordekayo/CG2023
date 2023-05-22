#include "KatamariControllerComponent.h"

#include "Game.h"
#include "GameObjects/GameObject.h"
#include "Components/TransformComponent.h"

KatamariControllerComponent::KatamariControllerComponent()
{
    KatamariSpeed = 0.0f;
    CameraTransform = nullptr;
}

void KatamariControllerComponent::Update(float DeltaTime)
{
    const DirectX::SimpleMath::Vector3 Left = CameraTransform->GetLeft();
    const DirectX::SimpleMath::Vector3 Forward = Left.Cross(DirectX::SimpleMath::Vector3::UnitY);
    if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::A))
    {
        GameObject->TransformComponent->SetRotation(GameObject->TransformComponent->GetRotation() *
            DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(Forward, - KatamariSpeed * DeltaTime / 1.0f));
        GameObject->TransformComponent->SetPosition(GameObject->TransformComponent->GetPosition() + KatamariSpeed * DeltaTime * Left);
    }
    if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::D))
    {
        GameObject->TransformComponent->SetRotation(GameObject->TransformComponent->GetRotation() *
            DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(Forward, KatamariSpeed * DeltaTime / 1.0f));
        GameObject->TransformComponent->SetPosition(GameObject->TransformComponent->GetPosition() - KatamariSpeed * DeltaTime * Left);
    }
    if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::W))
    {
        GameObject->TransformComponent->SetRotation(GameObject->TransformComponent->GetRotation() *
            DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(Left, KatamariSpeed * DeltaTime / 1.0f));
        GameObject->TransformComponent->SetPosition(GameObject->TransformComponent->GetPosition() + KatamariSpeed * DeltaTime * Forward);
    }
    if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::S))
    {
        GameObject->TransformComponent->SetRotation(GameObject->TransformComponent->GetRotation() *
            DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(Left, - KatamariSpeed * DeltaTime / 1.0f));
        GameObject->TransformComponent->SetPosition(GameObject->TransformComponent->GetPosition() - KatamariSpeed * DeltaTime * Forward);
    }
}
