#pragma once
#include "Components\ObjectComponent.h"

class FSphereCollisionComponent;

class FCollisionComponent : public FObjectComponent
{
public:
    
    FCollisionComponent();
    
    virtual bool IsIntersectsWithSphere(FSphereCollisionComponent* SphereToCheck) = 0;
};
