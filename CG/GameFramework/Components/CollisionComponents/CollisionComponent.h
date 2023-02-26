#pragma once
#include <DirectXMath.h>
#include "DirectXCollision.h"
#include "../ObjectComponent.h"

class FCollisionComponent : public FObjectComponent
{
public:
    
    FCollisionComponent();
    
    virtual void SetCenter(DirectX::XMFLOAT3 NewCenter) = 0;
};
