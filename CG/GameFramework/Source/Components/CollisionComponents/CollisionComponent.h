#pragma once
#include <DirectXMath.h>
#include "DirectXCollision.h"
#include "Components\ObjectComponent.h"

class FCollisionComponent : public FObjectComponent
{
public:
    
    FCollisionComponent();
    
    virtual bool IsIntersectsWithSphere(DirectX::BoundingSphere* SphereToCheck) = 0;
    virtual bool IsIntersectsWithBox(DirectX::BoundingBox* BoxToCheck) = 0;
    
    virtual void SetCenter(DirectX::XMFLOAT3 NewCenter) = 0;
};
