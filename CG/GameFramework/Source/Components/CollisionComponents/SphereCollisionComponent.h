#pragma once
#include "CollisionComponent.h"
#include "Utils/SimpleMath.h"

class FRenderComponent;

class FSphereCollisionComponent : public FCollisionComponent
{
public:

    FSphereCollisionComponent(float Radius);

    void Update() override;
    void Init() override;
    
    DirectX::BoundingSphere* GetCollision();
    
    bool IsIntersectsWithSphere(FSphereCollisionComponent* SphereToCheck) override;
    
    void SetRadius(float NewRadius);
    
private:

    bool bShowDebug = false;
    FRenderComponent* CollisionVisualization = nullptr;
    
    DirectX::BoundingSphere* BoundingSphere = nullptr;
};
