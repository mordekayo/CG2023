#pragma once
#include "../../GameFramework/GameObjects/GameObject.h"

struct PlanetParameters
{
    float Radius;
    float SelfRotationSpeed;
        
};

class Planet : public FGameObject
{
public:
    Planet(std::string PlanetName, float PlanetRadius, float PlanetSelfRotationSpeed, DirectX::SimpleMath::Vector3 PlanetRotationAxis);

    void Construct() override;

    void Update(float DeltaTime) override;
private:

    int HorizontalSectors = 64.0f;
    int VerticalSectors = 64.0f;
    std::string Name;
    float Radius;
    float SelfRotationSpeed;
    DirectX::SimpleMath::Vector3 RotationAxis;
};
