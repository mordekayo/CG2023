#include "PongGame.h"
#include "../GameFramework/Game.h"
#include "../GameFramework/GameObjects/GameObject.h"
#include "../GameFramework/Components/RenderComponent.h"

int main()
{
    PongGame* MyGame = PongGame::Instance();
    FGameObject* GameObject = new FGameObject();
    GameObject->AddComponent(new FRenderComponent());
    MyGame->AddGameObject(GameObject);
    MyGame->Run();
    delete MyGame;
}
