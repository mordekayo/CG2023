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
    for (auto component : components)
    {
        delete component;
    }
    components.clear();
}

void FGameObject::Initialize()
{
    for (const auto& component : components)
    {
        component->Initialize();
    }
}

void FGameObject::Update(float deltaTime)
{
    for (const auto& component : components)
    {
        component->Update(deltaTime);
    }
}

void FGameObject::AddComponent(FObjectComponent* component)
{
    components.push_back(component);
    component->gameObject = this;
}

void FGameObject::CreatePlane(float planeSize, std::string textureFileName)
{
    MeshComponent = new FMeshComponent(textureFileName);
    MeshComponent->AddPlane(planeSize);
    AddComponent(MeshComponent);
	
    //SHADOWS
    RenderShadowsComponent = new FRenderShadowsComponent(MeshComponent);
    AddComponent(RenderShadowsComponent);

    //MAIN FRAME
    RenderComponent = new FRenderComponent(MeshComponent);
    AddComponent(RenderComponent);
}

void FGameObject::CreateMesh(float scaleRate, std::string textureFileName, std::string objectFileName)
{
    MeshComponent = new FMeshComponent(textureFileName);
    MeshComponent->AddMesh(scaleRate, objectFileName);
    AddComponent(MeshComponent);
	
    //SHADOWS
    RenderShadowsComponent = new FRenderShadowsComponent(MeshComponent);
    AddComponent(RenderShadowsComponent);

    //MAIN FRAME
    RenderComponent = new FRenderComponent(MeshComponent);
    AddComponent(RenderComponent);
}