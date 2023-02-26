#include "BoxCollisionComponent.h"

FBoxCollisionComponent::FBoxCollisionComponent()
{
    BoundingBox = new DirectX::BoundingBox();
}

void FBoxCollisionComponent::SetCenter(DirectX::XMFLOAT3 NewCenter)
{
    BoundingBox->Center = NewCenter;
}

void FBoxCollisionComponent::SetExtents(DirectX::XMFLOAT3 NewExtents)
{
    BoundingBox->Extents = NewExtents;
}
