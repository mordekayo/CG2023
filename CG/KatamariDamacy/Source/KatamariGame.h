#pragma once

#include "Game.h"

class KatamariGame : public FGame
{
public:
    static KatamariGame* Instance();

protected:

    void Construct() override;

    void Update(float DeltaTime) override;

private:

};

