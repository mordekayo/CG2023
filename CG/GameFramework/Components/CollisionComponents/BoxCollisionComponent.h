#pragma once
#include "CollisionComponent.h"

class FBoxCollisionComponent : public FCollisionComponent
{
public:

    FBoxCollisionComponent();
    
    void SetCenter(DirectX::XMFLOAT3 NewCenter) override;
    void SetExtents(DirectX::XMFLOAT3 NewExtents);
    
private:
    
    DirectX::BoundingBox* BoundingBox = nullptr;
};
