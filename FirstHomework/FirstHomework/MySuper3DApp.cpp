// MySuper3DApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include "Game.h"
#include "GameObjects/Triangle.h"

int main()
{
    Game* MyGame = Game::Instance();
    MyGame->AddGameObject(new Triangle());
    MyGame->Run();
    delete MyGame;
}
