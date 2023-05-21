#pragma once
#include "Game.h"

class KatamariGame : public FGame
{
public:
    static KatamariGame* Instance();
    
    KatamariGame();

    void Initialize();
};
