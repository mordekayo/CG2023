#include "PongGame.h"

int main()
{
    PongGame* MyGame = PongGame::Instance();
    MyGame->Run();

    return 0;
}
