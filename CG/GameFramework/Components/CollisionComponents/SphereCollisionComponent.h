#pragma once
#include "CollisionComponent.h"

class FSphereCollisionComponent : public FCollisionComponent
{
public:

    FSphereCollisionComponent();

    DirectX::BoundingSphere* GetCollision();
    
    bool IsIntersectsWithSphere(DirectX::BoundingSphere* SphereToCheck) override;
    bool IsIntersectsWithBox(DirectX::BoundingBox* BoxToCheck) override;
    
    void SetCenter(DirectX::XMFLOAT3 NewCenter) override;
    void SetRadius(float NewRadius);

private:
    
    DirectX::BoundingSphere* BoundingSphere = nullptr;
};
