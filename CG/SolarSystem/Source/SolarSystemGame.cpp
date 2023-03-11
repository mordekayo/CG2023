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
    MoonParameters->OrbitalAxis = DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
    MoonParameters->OrbitalRotationSpeed = 10.0f;
    MoonParameters->ParentPlanet = Earth;
    
    Moon = new Planet(MoonParameters);
    Moon->SetTransform({0.0f, 0.2f, 0.0f});

    PlanetParameters* VenusParameters = new PlanetParameters();
    VenusParameters->Name = "Venus";
    VenusParameters->Radius = 0.1f;
    VenusParameters->RotationAxis = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f);
    VenusParameters->SelfRotationSpeed = 1.0f;
    VenusParameters->OrbitalAxis = DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
    VenusParameters->OrbitalRotationSpeed = 0.5f;
    VenusParameters->ParentPlanet = Sun;
    Venus = new Planet(VenusParameters);
    Venus->SetTransform({ 0.0f, 0.5f, 0.0f });

    PlanetParameters* Venus2Parameters = new PlanetParameters();
    Venus2Parameters->Name = "Venus2";
    Venus2Parameters->Radius = 0.03f;
    Venus2Parameters->RotationAxis = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
    Venus2Parameters->SelfRotationSpeed = 1.0f;
    Venus2Parameters->OrbitalAxis = DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
    Venus2Parameters->OrbitalRotationSpeed = 0.0f;
    Venus2Parameters->ParentPlanet = Venus;
    Venus2 = new Planet(Venus2Parameters);
    Venus2->SetTransform({ 0.0f, 0.0f, 0.2f });
    
    AddGameObject(Sun);
    AddGameObject(Earth);
    AddGameObject(Moon);
    AddGameObject(Venus);
    AddGameObject(Venus2);
    
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
    if (Input->IsKeyDown(Keys::D4))
    {
        bIsFPS = false;
        GetTargetCameraController()->SetTarget(Venus);
    }
    if (Input->IsKeyDown(Keys::D5))
    {
        bIsFPS = false;
        GetTargetCameraController()->SetTarget(Venus2);
    }
}