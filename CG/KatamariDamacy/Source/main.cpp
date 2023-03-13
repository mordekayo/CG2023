#include <cstdlib>

#include "KatamariGame.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")


int main()
{
    KatamariGame* MyGame = KatamariGame::Instance();
    MyGame->Run();

    return 0;
}
