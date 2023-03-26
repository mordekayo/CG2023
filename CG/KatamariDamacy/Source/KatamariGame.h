#pragma once

#include "Game.h"
#include "GameObjects/PlayerBall.h"

class FPlayerController;
class FFPSCameraController;

class KatamariGame : public FGame
{
public:
    static KatamariGame* Instance();

protected:

    void Construct() override;

    void Update(float DeltaTime) override;

private:

    bool bFreeCamera = false;
    FFPSCameraController* FreeCameraController = nullptr;

    FPlayerController* PlayerController = nullptr;
    PlayerBall* Player = nullptr;
    std::vector<FGameObject*> Objects;
};

