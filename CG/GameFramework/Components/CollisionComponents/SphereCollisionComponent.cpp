#include "SphereCollisionComponent.h"

FSphereCollisionComponent::FSphereCollisionComponent()
{
    BoundingSphere = new DirectX::BoundingSphere();
    SetCenter({0.0f, 0.0f, 0.0f});
}

void FSphereCollisionComponent::SetCenter(DirectX::XMFLOAT3 NewCenter)
{
    BoundingSphere->Center = NewCenter;
}

void FSphereCollisionComponent::SetRadius(float NewRadius)
{
    BoundingSphere->Radius = NewRadius;
}
