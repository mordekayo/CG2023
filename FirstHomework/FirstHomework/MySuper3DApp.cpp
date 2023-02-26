// MySuper3DApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "FGame.h"
#include "Components/FRenderComponent.h"
#include "GameObjects/FGameObject.h"

int main()
{
    FGame* MyGame = FGame::Instance();
    FGameObject* GameObject = new FGameObject();
    GameObject->AddComponent(new FRenderComponent());
    MyGame->AddGameObject(GameObject);
    MyGame->Run();
    delete MyGame;
}
