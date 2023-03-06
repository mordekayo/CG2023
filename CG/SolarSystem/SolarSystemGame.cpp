#include "SolarSystemGame.h"

#include <iostream>

#include "../GameFramework/Camera/TargetCameraController.h"
#include "GameObjects/Planet.h"
#include "../GameFramework/Utils/InputDevice.h"

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
    SunParameters->Radius = 0.1f;
    SunParameters->RotationAxis = DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
    SunParameters->SelfRotationSpeed = 1.0f;
    SunParameters->ParentPlanet = nullptr;
    Sun = new Planet(SunParameters);
    Sun->SetTransform({0.0f, 0.0f, 0.0f});
    
    PlanetParameters* EarthParameters = new PlanetParameters();
    EarthParameters->Name = "Earth";
    EarthParameters->Radius = 0.1f;
    EarthParameters->RotationAxis = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f);
    EarthParameters->SelfRotationSpeed = 1.0f;
    EarthParameters->OrbitalAxis = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
    EarthParameters->OrbitalRotationSpeed = 1.0f;
    EarthParameters->ParentPlanet = Sun;
    
    Earth = new Planet(EarthParameters);
    Earth->SetTransform({0.7f, 0.0f, 0.0f});

    PlanetParameters* MoonParameters = new PlanetParameters();
    MoonParameters->Name = "Moon";
    MoonParameters->Radius = 0.05f;
    MoonParameters->RotationAxis = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
    MoonParameters->SelfRotationSpeed = 3.0f;
    MoonParameters->OrbitalAxis = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
    MoonParameters->OrbitalRotationSpeed = 10.0f;
    MoonParameters->ParentPlanet = Earth;
    
    Moon = new Planet(MoonParameters);
    Moon->SetTransform({0.2f, 0.0f, 0.0f});
    
    AddGameObject(Sun);
    AddGameObject(Earth);
    AddGameObject(Moon);
    
    FGame::Construct();
}

void SolarSystemGame::Update(float DeltaTime)
{
    FGame::Update(DeltaTime);

    if (Input->IsKeyDown(Keys::D0))
    {
        bIsFPS = true;
    }
    if (Input->IsKeyDown(Keys::D1))
    {
        bIsFPS = false;
        GetTargetCameraController()->SetTarget(Sun);
    }
    if (Input->IsKeyDown(Keys::D2))
    {
        bIsFPS = false;
        GetTargetCameraController()->SetTarget(Earth);
    }
    if (Input->IsKeyDown(Keys::D3))
    {
        bIsFPS = false;
        GetTargetCameraController()->SetTarget(Moon);
    }
}