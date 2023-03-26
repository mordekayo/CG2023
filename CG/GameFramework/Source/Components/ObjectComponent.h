#pragma once
#include "Utils/SimpleMath.h"

class FGameObject;

class FObjectComponent
{

public:

    virtual void Init();
    virtual void Update();

    void SetOwner(FGameObject* NewOwner);

    DirectX::SimpleMath::Matrix GetLocalTransform() const;

    void AddComponentTranslation(DirectX::SimpleMath::Vector3 AdditionalTranslation);
    void SetComponentTranslation(DirectX::SimpleMath::Vector3 NewTranslation);
protected:

    FGameObject* Owner = nullptr;

    DirectX::SimpleMath::Matrix LocalTransform = DirectX::SimpleMath::Matrix::Identity;

    DirectX::SimpleMath::Vector3 Translation = {0.0f, 0.0f, 0.0f};
    DirectX::SimpleMath::Quaternion Rotation = DirectX::SimpleMath::Quaternion::Identity;
};
