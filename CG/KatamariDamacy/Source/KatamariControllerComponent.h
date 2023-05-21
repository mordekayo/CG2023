#pragma once
#include "Components/ObjectComponent.h"

class FTransformComponent;

class KatamariControllerComponent : public FObjectComponent
{
public:

    float katamariSpeed;

    FTransformComponent* cameraTransform;

    KatamariControllerComponent();
    virtual void Update(float deltaTime) override;
};

