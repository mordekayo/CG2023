#include "SphereCollisionComponent.h"

FSphereCollisionComponent::FSphereCollisionComponent()
{
    BoundingSphere = new DirectX::BoundingSphere();
    SetCenter({0.0f, 0.0f, 0.0f});
}

DirectX::BoundingSphere* FSphereCollisionComponent::GetCollision()
{
    return BoundingSphere;
}

void FSphereCollisionComponent::SetCenter(DirectX::XMFLOAT3 NewCenter)
{
    BoundingSphere->Center = NewCenter;
}

void FSphereCollisionComponent::SetRadius(float NewRadius)
{
    BoundingSphere->Radius = NewRadius;
}

bool FSphereCollisionComponent::IsIntersectsWithSphere(DirectX::BoundingSphere* SphereToCheck)
{
    return BoundingSphere->Intersects(*SphereToCheck);
}

bool FSphereCollisionComponent::IsIntersectsWithBox(DirectX::BoundingBox* BoxToCheck)
{
    return BoundingSphere->Intersects(*BoxToCheck);
}
