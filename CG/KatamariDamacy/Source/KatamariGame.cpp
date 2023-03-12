#include "KatamariGame.h"

#include "Components/RenderComponent.h"

KatamariGame* KatamariGame::Instance()
{
    if (!GameInstance)
    {
        GameInstance = new KatamariGame();
    }
    return static_cast<KatamariGame*>(GameInstance);
}

void KatamariGame::Construct()
{
    FGame::Construct();
}

void KatamariGame::Update(float DeltaTime)
{
    FGame::Update(DeltaTime);
}
