#pragma once

#include "../../GameFramework/Utils/InputDevice.h"

class FCamera;
class InputDevice;

class FFPSCameraController
{
public:
    FFPSCameraController();

    void Update(float DeltaTime);
    virtual void MouseEventHandler(const InputDevice::MouseMoveEventArgs& mouseData, int payload);

    void SetCamera(FCamera* NewCamera);
    
    float CameraYaw = 0;
    float CameraPitch = 0;
    bool WasProjectionKeyDown = false;

private:
    float CameraRotationSpeed = 0.005f;
    FCamera* Camera = nullptr;
    InputDevice* Input;
};
