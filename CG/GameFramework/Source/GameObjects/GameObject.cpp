#include "GameObject.h"

#include "Components/ObjectComponent.h"
#include "Components/RenderComponent.h"

void FGameObject::Init()
{
    for(FObjectComponent* Component : Components)
    {
        Component->Init();
    }
}

void FGameObject::Update(float DeltaTime)
{
    for(FObjectComponent* Component : Components)
    {
        Component->Update();
    }
}

void FGameObject::Draw() const
{
    for(FObjectComponent* Component : Components)
    {
        if(FRenderComponent* RenderComponent = dynamic_cast<FRenderComponent*>(Component))
        {
            RenderComponent->Draw();
        }
    }
}

DirectX::SimpleMath::Vector3 FGameObject::GetLocalTranslation() const
{
    return Translation;
}

DirectX::SimpleMath::Vector3 FGameObject::GetWorldTranslation() const
{
    return GetWorldTransform().Translation();
}

void FGameObject::AddComponent(FObjectComponent* ComponentToAdd)
{
    ComponentToAdd->SetOwner(this);
    Components.insert(ComponentToAdd);
}

void FGameObject::DeleteComponent(FObjectComponent* ComponentToDelete)
{
    Components.erase(ComponentToDelete);
}

void FGameObject::AddTranslation(DirectX::SimpleMath::Vector3 AdditionTranslation)
{
    Translation += AdditionTranslation;
}

void FGameObject::SetTranslation(DirectX::SimpleMath::Vector3 NewTranslation)
{
    Translation = NewTranslation;
}

DirectX::SimpleMath::Quaternion FGameObject::GetLocalRotationQuat() const
{
    return Rotation;
}

DirectX::SimpleMath::Vector3 FGameObject::GetLocalRotationEuler() const
{
    return Rotation.ToEuler();
}

void FGameObject::AddRotationQuat(DirectX::SimpleMath::Quaternion AdditionalRotation)
{
    Rotation *= AdditionalRotation;
    Rotation.Normalize();
}

void FGameObject::SetLocalRotationQuat(DirectX::SimpleMath::Quaternion NewRotation)
{
    Rotation = NewRotation;
    Rotation.Normalize();
}

void FGameObject::SetLocalRotationEuler(DirectX::SimpleMath::Vector3 NewRotation)
{
    Rotation = DirectX::SimpleMath::Quaternion(NewRotation);
    Rotation.Normalize();
}

DirectX::SimpleMath::Vector3 FGameObject::GetForwardVector() const
{
    return DirectX::SimpleMath::Vector3::TransformNormal(DirectX::SimpleMath::Vector3::Backward, GetWorldTransform());
}

DirectX::SimpleMath::Vector3 FGameObject::GetRightVector() const
{
    return DirectX::SimpleMath::Vector3::TransformNormal(DirectX::SimpleMath::Vector3::Left, GetWorldTransform());
}

DirectX::SimpleMath::Vector3 FGameObject::GetLeftVector() const
{
    return DirectX::SimpleMath::Vector3::TransformNormal(DirectX::SimpleMath::Vector3::Right, GetWorldTransform());
}

DirectX::SimpleMath::Vector3 FGameObject::GetBackwardVector() const
{
    return DirectX::SimpleMath::Vector3::TransformNormal(DirectX::SimpleMath::Vector3::Forward, GetWorldTransform());
}

void FGameObject::Construct()
{
    
}

void FGameObject::SetParent(FGameObject* NewParent)
{
    ParentObject = NewParent;
}

DirectX::SimpleMath::Matrix FGameObject::GetWorldTransform() const
{
    DirectX::SimpleMath::Matrix Result = DirectX::SimpleMath::Matrix::Identity;
    const FGameObject* Object = this;
    do
    {
        Result *= Object->GetLocalTransform();
        Object = Object->ParentObject; 
    }
    while(Object != nullptr);
    
    return Result;
}

DirectX::SimpleMath::Matrix FGameObject::GetLocalTransform() const
{
    return DirectX::SimpleMath::Matrix::CreateFromQuaternion(Rotation) * DirectX::SimpleMath::Matrix::CreateTranslation(Translation);
}
