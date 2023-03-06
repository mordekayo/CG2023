#pragma once

#include "../../GameFramework/Utils/InputDevice.h"

class FGameObject;
class FCamera;
class InputDevice;

class FTargetCameraController
{
public:
    FTargetCameraController();

    void Update(float DeltaTime);

    void SetCamera(FCamera* NewCamera);

    void SetTarget(FGameObject* Object);
    
    float CameraYaw = 0;
    float CameraPitch = 0;
    bool WasProjectionKeyDown = false;

private:

    FGameObject* Target;
    float CameraRotationSpeed = 0.001f;
    FCamera* Camera = nullptr;
    InputDevice* Input;
};
