#pragma once
#include "Sources.h"
#include "Utils/InputDevice.h"

class FGameObject;

class FObjectComponent
{
public:

    FGameObject* GameObject;

    virtual ~FObjectComponent() {}
    virtual void Initialize() {}
    virtual void Update(float DeltaTime) {}

    virtual void MouseEventHandler(const FInputDevice::MouseMoveEventArgs& MouseData) {}
};