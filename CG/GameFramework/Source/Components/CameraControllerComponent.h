#pragma once
#include "Sources.h"
#include "Utils/InputDevice.h"
#include "ObjectComponent.h"

class FTransformComponent;

class FCameraControllerComponent : public FObjectComponent
{
public:

    FTransformComponent* aim;
    float yaw;
    float pitch;
    float minPitch;
    float maxPitch;
    float armLength;

    FCameraControllerComponent();
    virtual void Update(float deltaTime) override;
    virtual void Initialize() override;
    virtual void MouseEventHandler(const InputDevice::MouseMoveEventArgs& mouseData) override;
};
