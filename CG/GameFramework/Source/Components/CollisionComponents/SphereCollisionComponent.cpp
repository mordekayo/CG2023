#include "SphereCollisionComponent.h"

#include "Components/RenderComponent.h"
#include "GameObjects/GameObject.h"
#include "Utils/SimpleMath.h"

FSphereCollisionComponent::FSphereCollisionComponent(float Radius)
{
    BoundingSphere = new DirectX::BoundingSphere();
    BoundingSphere->Radius = Radius;
    
    CollisionVisualization = new FRenderComponent(L"../GameFramework/Source/Shaders/MyVeryFirstShader.hlsl");
    CollisionVisualization->AddSphere(Radius, 64.0f, 64.0f, {0.97f, 0.84f, 0.1f, 1.0f});
    CollisionVisualization->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void FSphereCollisionComponent::Update()
{
    FCollisionComponent::Update();

    BoundingSphere->Center = Owner->GetWorldTranslation() + GetLocalTransform().Translation();
    CollisionVisualization->SetComponentTranslation(GetLocalTransform().Translation());
}

void FSphereCollisionComponent::Init()
{
    FCollisionComponent::Init();
    
    Owner->AddComponent(CollisionVisualization);
}

DirectX::BoundingSphere* FSphereCollisionComponent::GetCollision()
{
    return BoundingSphere;
}

void FSphereCollisionComponent::SetRadius(float NewRadius)
{
    BoundingSphere->Radius = NewRadius;
}

bool FSphereCollisionComponent::IsIntersectsWithSphere(DirectX::BoundingSphere* SphereToCheck)
{
    return BoundingSphere->Intersects(*SphereToCheck);
}

