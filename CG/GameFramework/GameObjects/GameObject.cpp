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

void FGameObject::Update() const
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
        FRenderComponent* RenderComponent = static_cast<FRenderComponent*>(Component);
        if(RenderComponent != nullptr)
        {
            RenderComponent->Draw();
        }
    }
}

DirectX::SimpleMath::Vector4 FGameObject::GetTransform() const
{
    return Position;
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

void FGameObject::AddTransform(DirectX::SimpleMath::Vector4 Transform)
{
    Position += Transform;
}

void FGameObject::SetPosition(DirectX::SimpleMath::Vector4 NewPosition)
{
    Position = NewPosition;
}
