#include "BoxCollisionComponent.h"

FBoxCollisionComponent::FBoxCollisionComponent()
{
    BoundingBox = new DirectX::BoundingBox();
}

DirectX::BoundingBox* FBoxCollisionComponent::GetCollision()
{
    return BoundingBox;
}

bool FBoxCollisionComponent::IsIntersectsWithSphere(DirectX::BoundingSphere* SphereToCheck)
{
    return BoundingBox->Intersects(*SphereToCheck);
}

bool FBoxCollisionComponent::IsIntersectsWithBox(DirectX::BoundingBox* BoxToCheck)
{
    return BoundingBox->Intersects(*BoxToCheck);
}

void FBoxCollisionComponent::SetCenter(DirectX::XMFLOAT3 NewCenter)
{
    BoundingBox->Center = NewCenter;
}

void FBoxCollisionComponent::SetExtents(DirectX::XMFLOAT3 NewExtents)
{
    BoundingBox->Extents = NewExtents;
}
