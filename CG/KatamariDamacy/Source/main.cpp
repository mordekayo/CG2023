#include <cstdlib>

#include "KatamariGame.h"

#include <assimp/Importer.hpp> 
#include <assimp/scene.h> 
#include <assimp/postprocess.h>


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")

int main()
{
    KatamariGame* MyGame = KatamariGame::Instance();
    MyGame->Run();

    //Assimp::Importer importer;

    return 0;
}
