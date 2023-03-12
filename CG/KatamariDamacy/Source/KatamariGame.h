#pragma once

#include "Game.h"
#include "GameObjects/PlayerBall.h"

class KatamariGame : public FGame
{
public:
    static KatamariGame* Instance();

protected:

    void Construct() override;

    void Update(float DeltaTime) override;

private:

    PlayerBall* Player;
};

