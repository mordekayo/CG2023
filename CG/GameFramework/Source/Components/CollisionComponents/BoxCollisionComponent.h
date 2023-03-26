#pragma once
#include "CollisionComponent.h"

class FBoxCollisionComponent : public FCollisionComponent
{
public:

    FBoxCollisionComponent();

    DirectX::BoundingBox* GetCollision();
    
    bool IsIntersectsWithSphere(DirectX::BoundingSphere* SphereToCheck) override;
    void SetExtents(DirectX::XMFLOAT3 NewExtents);
    
private:
    
    DirectX::BoundingBox* BoundingBox = nullptr;
};
