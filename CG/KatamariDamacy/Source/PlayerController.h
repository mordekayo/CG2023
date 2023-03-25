#pragma once
#include "GameObjects/GameObject.h"
#include "Utils/InputDevice.h"

class FCamera;
class FGameObject;

class FPlayerController : public FGameObject
{
public:

     FPlayerController();
     
     void Init() override;
     void Update(float DeltaTime) override;

     void Possess(FGameObject* NewPlayer);

     void SetCamera(FCamera* NewCamera);
     [[nodiscard]] FCamera* GetCamera() const;
     
private:
     
     virtual void MouseEventHandler(const InputDevice::MouseMoveEventArgs& mouseData, int payload);

     float CameraRotationSpeed = 0.0025f;
     float CameraYaw = 0;
     float CameraPitch = 0;
     
     FCamera* Camera = nullptr;
     float MovementSpeed = 1.0f;
     float RotationByMovement = 3.0f;
     FGameObject* Player = nullptr;
};

