#pragma once
#include "Components/ObjectComponent.h"

class FTransformComponent;

class KatamariControllerComponent : public FObjectComponent
{
public:

    float KatamariSpeed;

    FTransformComponent* CameraTransform;

    KatamariControllerComponent();
    virtual void Update(float DeltaTime) override;
};

