#pragma once
#include "CollisionComponent.h"

class FSphereCollisionComponent : public FCollisionComponent
{
public:

    FSphereCollisionComponent();
    
    void SetCenter(DirectX::XMFLOAT3 NewCenter) override;
    void SetRadius(float NewRadius);
    
private:
    
    DirectX::BoundingSphere* BoundingSphere = nullptr;
};
