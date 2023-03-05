
#include <cstdlib>

#include "SolarSystemGame.h"

int main()
{
    SolarSystemGame* MyGame = SolarSystemGame::Instance();
    MyGame->Run();
    delete MyGame;
}
