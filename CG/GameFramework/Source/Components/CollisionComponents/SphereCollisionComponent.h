#pragma once
#include "CollisionComponent.h"

class FRenderComponent;

class FSphereCollisionComponent : public FCollisionComponent
{
public:

    FSphereCollisionComponent(float Radius);

    void Update() override;
    void Init() override;
    
    DirectX::BoundingSphere* GetCollision();
    
    bool IsIntersectsWithSphere(DirectX::BoundingSphere* SphereToCheck) override;
    
    void SetRadius(float NewRadius);

private:

    FRenderComponent* CollisionVisualization = nullptr;
    
    DirectX::BoundingSphere* BoundingSphere = nullptr;
};
