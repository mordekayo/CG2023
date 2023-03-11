#include <cstdlib>

#include "KatamariGame.h"

int main()
{
    KatamariGame* MyGame = KatamariGame::Instance();
    MyGame->Run();

    return 0;
}
