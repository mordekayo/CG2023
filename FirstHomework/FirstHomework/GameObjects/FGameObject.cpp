#include "FGameObject.h"

#include "../Components/FObjectComponent.h"
#include "../Components/FRenderComponent.h"


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

void FGameObject::AddComponent(FObjectComponent* ComponentToAdd)
{
    Components.insert(ComponentToAdd);
}

void FGameObject::DeleteComponent(FObjectComponent* ComponentToDelete)
{
    Components.erase(ComponentToDelete);
}
