// MySuper3DApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Game.h"
#include "GameObjects/Triangle.h"

int main()
{
    Game* MyGame = Game::Instance();
    Triangle* NewTriangle = new Triangle();
    MyGame->AddGameObject(NewTriangle);
    MyGame->Run();
    delete MyGame;
}
