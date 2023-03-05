#include "GameObject.h"

#include "../Components/ObjectComponent.h"
#include "../Components/RenderComponent.h"

void FGameObject::Init() const
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

DirectX::SimpleMath::Vector3 FGameObject::GetTransform() const
{
    return Translation;
}

DirectX::SimpleMath::Matrix FGameObject::GetWorldView() const
{
    return WorldView;
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

void FGameObject::AddTransform(DirectX::SimpleMath::Vector3 Transform)
{
    Translation += Transform;
    UpdateWorldMatrix();
}

void FGameObject::SetTransform(DirectX::SimpleMath::Vector3 NewPosition)
{
    Translation = NewPosition;
    UpdateWorldMatrix();
}

DirectX::SimpleMath::Quaternion FGameObject::GetRotationQuat() const
{
    return Rotation;
}

DirectX::SimpleMath::Vector3 FGameObject::GetRotationEuler() const
{
    return Rotation.ToEuler();
}

void FGameObject::SetRotationQuat(DirectX::SimpleMath::Quaternion NewRotation)
{
    Rotation = NewRotation;
    Rotation.Normalize();
    UpdateWorldMatrix();
}

void FGameObject::SetRotationEuler(DirectX::SimpleMath::Vector3 NewRotation)
{
    Rotation = DirectX::SimpleMath::Quaternion(NewRotation);
    Rotation.Normalize();
    UpdateWorldMatrix();
}

void FGameObject::UpdateWorldMatrix()
{
    WorldView = DirectX::SimpleMath::Matrix::CreateFromQuaternion(Rotation) * DirectX::SimpleMath::Matrix::CreateTranslation(Translation);
}

void FGameObject::Construct()
{
    
}
