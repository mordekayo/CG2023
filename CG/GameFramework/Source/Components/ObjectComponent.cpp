#include "ObjectComponent.h"

#include <cassert>

void FObjectComponent::Init()
{
    assert(Owner, "Init should be called only on attached components");
}

void FObjectComponent::Update()
{
    
}

void FObjectComponent::SetOwner(FGameObject* NewOwner)
{
    Owner = NewOwner;
}

DirectX::SimpleMath::Matrix FObjectComponent::GetLocalTransform() const
{
    return DirectX::SimpleMath::Matrix::CreateFromQuaternion(Rotation) * DirectX::SimpleMath::Matrix::CreateTranslation(Translation);
}

void FObjectComponent::AddComponentTranslation(DirectX::SimpleMath::Vector3 AdditionalTranslation)
{
    Translation += AdditionalTranslation;
}

void FObjectComponent::SetComponentTranslation(DirectX::SimpleMath::Vector3 NewTranslation)
{
    Translation = NewTranslation;
}
