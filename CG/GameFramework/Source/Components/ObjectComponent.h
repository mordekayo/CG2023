#pragma once
#include "Sources.h"
#include "Utils/InputDevice.h"

class FGameObject;

class FObjectComponent
{
public:

    FGameObject* gameObject;

    virtual ~FObjectComponent() {}
    virtual void Initialize() {}
    virtual void Update(float deltaTime) {}

    virtual void MouseEventHandler(const InputDevice::MouseMoveEventArgs& mouseData) {}
};