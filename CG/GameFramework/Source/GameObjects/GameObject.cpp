#include "GameObject.h"

#include "Game.h"
#include "Components/TransformComponent.h"
#include "Components/MeshComponent.h"
#include "Components/RenderComponent.h"
#include "Components/ObjectComponent.h"
#include "Components/CameraComponent.h"
#include "Components/CameraControllerComponent.h"
#include "Components/RenderShadowsComponent.h"

FGameObject::FGameObject(FGameObject* parent)
{
    TransformComponent = new FTransformComponent();
    AddComponent(TransformComponent);
}

FGameObject::~FGameObject()
{
    for (FObjectComponent* Component : Components)
    {
        delete Component;
    }
    Components.clear();
}

void FGameObject::Initialize()
{
    for (FObjectComponent* Component : Components)
    {
        Component->Initialize();
    }
}

void FGameObject::Update(float deltaTime)
{
    for (FObjectComponent* Component : Components)
    {
        Component->Update(deltaTime);
    }
}

void FGameObject::AddComponent(FObjectComponent* Component)
{
    Components.push_back(Component);
    Component->GameObject = this;
}

void FGameObject::CreatePlane(float PlaneSize, const std::string& TextureFileName)
{
    MeshComponent = new FMeshComponent(TextureFileName);
    MeshComponent->AddPlane(PlaneSize);
    AddComponent(MeshComponent);
    
    RenderShadowsComponent = new FRenderShadowsComponent(MeshComponent);
    AddComponent(RenderShadowsComponent);
    
    RenderComponent = new FRenderComponent(MeshComponent);
    AddComponent(RenderComponent);
}

void FGameObject::CreateMesh(float ScaleRate, const std::string& TextureFileName, const std::string& ObjectFileName)
{
    MeshComponent = new FMeshComponent(TextureFileName);
    MeshComponent->AddMesh(ScaleRate, ObjectFileName);
    AddComponent(MeshComponent);
    
    RenderShadowsComponent = new FRenderShadowsComponent(MeshComponent);
    AddComponent(RenderShadowsComponent);

    RenderComponent = new FRenderComponent(MeshComponent);
    AddComponent(RenderComponent);
}