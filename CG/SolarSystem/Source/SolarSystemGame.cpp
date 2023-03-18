#include "SolarSystemGame.h"

#include <iostream>

#include "Camera/TargetCameraController.h"
#include "GameObjects/Planet.h"
#include "Utils/InputDevice.h"
#include "Camera/Camera.h"

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
    PlanetParameters* SunParameters = new PlanetParameters();
    SunParameters->Name = "Sun";
    SunParameters->Radius = 0.2f;
    SunParameters->RotationAxis = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
    SunParameters->SelfRotationSpeed = 0.0f;
    SunParameters->ParentPlanet = nullptr;
    Sun = new Planet(SunParameters);
    Sun->SetTranslation({0.0f, 0.0f, 5.0f});

    AddGameObject(Sun);
    
    FGame::Construct();
}

void SolarSystemGame::Update(float DeltaTime)
{
    FGame::Update(DeltaTime);
}