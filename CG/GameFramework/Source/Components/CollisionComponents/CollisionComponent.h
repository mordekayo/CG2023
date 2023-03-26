#pragma once
#include <DirectXMath.h>
#include "DirectXCollision.h"
#include "Components\ObjectComponent.h"
#include "Utils/SimpleMath.h"

class FCollisionComponent : public FObjectComponent
{
public:
    
    FCollisionComponent();
    
    virtual bool IsIntersectsWithSphere(DirectX::BoundingSphere* SphereToCheck) = 0;
};
