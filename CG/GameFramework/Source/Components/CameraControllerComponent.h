#pragma once
#include "Sources.h"
#include "Utils/InputDevice.h"
#include "ObjectComponent.h"

class FTransformComponent;

class FCameraControllerComponent : public FObjectComponent
{
public:

    FTransformComponent* Target;
    float Yaw;
    float Pitch;
    float MinPitch;
    float MaxPitch;
    float ArmLength;

    FCameraControllerComponent();
    virtual void Update(float DeltaTime) override;
    virtual void Initialize() override;
    virtual void MouseEventHandler(const FInputDevice::MouseMoveEventArgs& MouseData) override;
};
