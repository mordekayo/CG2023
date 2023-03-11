#pragma once
#include "GameObjects/GameObject.h"
#include <string>
class Planet;

struct PlanetParameters
{
    std::string Name;
    float Radius;
    float SelfRotationSpeed;
    DirectX::SimpleMath::Vector3 RotationAxis;
    float OrbitalRotationSpeed;
    DirectX::SimpleMath::Vector3 OrbitalAxis;
    Planet* ParentPlanet;
};

class Planet : public FGameObject
{
public:
    Planet(PlanetParameters* InitialParameters);

    void Construct() override;

    void Init() override;
    void Update(float DeltaTime) override;

    DirectX::SimpleMath::Vector3 GetTransform() const override;

    DirectX::SimpleMath::Vector3 GetLocalTransform() const;
protected:

    void UpdateWorldMatrix();
    
private:

    DirectX::SimpleMath::Matrix OrbitalTranslation = DirectX::SimpleMath::Matrix::Identity;
    
    int HorizontalSectors = 16.0f;
    int VerticalSectors = 16.0f;

    std::string Name;
    float Radius;
    float SelfRotationSpeed;
    float OrbitalRotationSpeed;
    DirectX::SimpleMath::Vector3 RotationAxis;
    DirectX::SimpleMath::Vector3 OrbitalAxis;
    Planet* ParentPlanet;

    DirectX::SimpleMath::Quaternion OrbitalRotation = DirectX::SimpleMath::Quaternion::Identity;
};
