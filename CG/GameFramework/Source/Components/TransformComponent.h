#pragma once
#include "ObjectComponent.h"

class FTransformComponent : public FObjectComponent
{
public:

    DirectX::SimpleMath::Vector3 localPosition;
    DirectX::SimpleMath::Quaternion localRotation;
    FTransformComponent* parent;

    DirectX::SimpleMath::Vector3 scale;

    FTransformComponent();
    FTransformComponent(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotation);

    DirectX::SimpleMath::Vector3 GetPosition() const;
    DirectX::SimpleMath::Quaternion GetRotation() const;
    void SetPosition(const DirectX::SimpleMath::Vector3& position);
    void SetRotation(const DirectX::SimpleMath::Quaternion& rotation);
    DirectX::SimpleMath::Matrix GetView() const;
    DirectX::SimpleMath::Matrix GetLocalView() const;
    DirectX::SimpleMath::Matrix GetModel() const;
    DirectX::SimpleMath::Matrix GetLocalModel() const;
    
    DirectX::SimpleMath::Vector3 GetLocalLeft() const;
    DirectX::SimpleMath::Vector3 GetLocalUp() const;
    DirectX::SimpleMath::Vector3 GetLocalForward() const;
    DirectX::SimpleMath::Vector3 GetLeft() const;
    DirectX::SimpleMath::Vector3 GetUp() const;
    DirectX::SimpleMath::Vector3 GetForward() const;
};