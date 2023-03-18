#pragma once
#include "GameObjects/GameObject.h"

class FCamera;
class FGameObject;

class FPlayerController : public FGameObject
{
public:
     void Init() override;
     void Update(float DeltaTime) override;

     void Possess(FGameObject* NewPlayer);

     void SetCamera(FCamera* NewCamera);
     
private:
     
     FCamera* Camera = nullptr;
     float MovementSpeed = 1.0f;
     FGameObject* Player = nullptr;
};

