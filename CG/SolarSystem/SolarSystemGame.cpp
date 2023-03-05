#include "SolarSystemGame.h"

#include <ranges>

#include "GameObjects/Planet.h"

SolarSystemGame* SolarSystemGame::Instance()
{
    if (!GameInstance)
    {
        GameInstance = new SolarSystemGame();
    }
    return static_cast<SolarSystemGame*>(GameInstance);
}

void SolarSystemGame::Construct()
{
    Planet* Sun = new Planet("Sun", 0.1f, 1, {0.0f, 1.0f, 0.0f});

    Planet* Earth = new Planet("Earth", 0.05f, 5, { 1.0f, 0.0f, 0.0f });
    Earth->SetTransform({0.3f, 0.0f, 0.0f});

    Planet* Mars = new Planet("Mars", 0.1f, 5, { 1.0f, 0.0f, 1.0f });
    Mars->SetTransform({ 0.5f, -0.3f, 1.0f });
        
    AddGameObject(Sun);
    //AddGameObject(Earth);
    AddGameObject(Mars);
    
    FGame::Construct();
}
