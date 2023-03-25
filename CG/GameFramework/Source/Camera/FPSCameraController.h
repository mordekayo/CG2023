#pragma once

#include "Utils/InputDevice.h"

class FCamera;
class InputDevice;

class FFPSCameraController
{
public:
    FFPSCameraController();

    void Update(float DeltaTime);
    virtual void MouseEventHandler(const InputDevice::MouseMoveEventArgs& mouseData, int payload);

    void SetCamera(FCamera* NewCamera);
    FCamera* GetCamera() const;
    float CameraYaw = 0;
    float CameraPitch = 0;
    bool WasProjectionKeyDown = false;

private:

    float CameraTranslationSpeed = 0.5;
    float CameraRotationSpeed = 0.0025f;
    FCamera* Camera = nullptr;
    InputDevice* Input;
};
