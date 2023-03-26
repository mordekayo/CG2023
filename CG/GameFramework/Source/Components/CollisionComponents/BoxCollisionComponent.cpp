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


void FBoxCollisionComponent::SetExtents(DirectX::XMFLOAT3 NewExtents)
{
    BoundingBox->Extents = NewExtents;
}
