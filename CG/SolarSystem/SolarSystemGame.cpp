#include "SolarSystemGame.h"

SolarSystemGame* SolarSystemGame::Instance()
{
    if (!GameInstance)
    {
        GameInstance = new SolarSystemGame();
    }
    return static_cast<SolarSystemGame*>(GameInstance);
}