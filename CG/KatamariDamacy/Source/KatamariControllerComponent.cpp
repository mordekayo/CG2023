#include "KatamariControllerComponent.h"

#include "Game.h"
#include "GameObjects/GameObject.h"
#include "Components/TransformComponent.h"

KatamariControllerComponent::KatamariControllerComponent()
{
    this->katamariSpeed = 0.0f;
    this->cameraTransform = nullptr;
}

void KatamariControllerComponent::Update(float deltaTime)
{
    DirectX::SimpleMath::Vector3 left = cameraTransform->GetLeft();
    DirectX::SimpleMath::Vector3 forward = left.Cross(DirectX::SimpleMath::Vector3::UnitY);
    if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::A))
    {
        gameObject->TransformComponent->SetRotation(gameObject->TransformComponent->GetRotation() *
            DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(forward, - katamariSpeed * deltaTime / 1.0f));
        gameObject->TransformComponent->SetPosition(gameObject->TransformComponent->GetPosition() + katamariSpeed * deltaTime * left);
    }
    if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::D))
    {
        gameObject->TransformComponent->SetRotation(gameObject->TransformComponent->GetRotation() *
            DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(forward, katamariSpeed * deltaTime / 1.0f));
        gameObject->TransformComponent->SetPosition(gameObject->TransformComponent->GetPosition() - katamariSpeed * deltaTime * left);
    }
    if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::W))
    {
        gameObject->TransformComponent->SetRotation(gameObject->TransformComponent->GetRotation() *
            DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(left, katamariSpeed * deltaTime / 1.0f));
        gameObject->TransformComponent->SetPosition(gameObject->TransformComponent->GetPosition() + katamariSpeed * deltaTime * forward);
    }
    if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::S))
    {
        gameObject->TransformComponent->SetRotation(gameObject->TransformComponent->GetRotation() *
            DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(left, - katamariSpeed * deltaTime / 1.0f));
        gameObject->TransformComponent->SetPosition(gameObject->TransformComponent->GetPosition() - katamariSpeed * deltaTime * forward);
    }
}
