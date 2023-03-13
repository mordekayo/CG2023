#include "Planet.h"

#include <cmath>
#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>

#include "Game.h"
#include "Components/RenderComponent.h"

Planet::Planet(PlanetParameters* InitialParameters)
{
    Name = InitialParameters->Name;
    Radius = InitialParameters->Radius;
    SelfRotationSpeed = InitialParameters->SelfRotationSpeed;
    RotationAxis = InitialParameters->RotationAxis;  
    OrbitalRotationSpeed = InitialParameters->OrbitalRotationSpeed;
    OrbitalAxis = InitialParameters->OrbitalAxis;
    ParentPlanet = InitialParameters->ParentPlanet;
}

void Planet::Update(float DeltaTime)
{
    FGameObject::Update(DeltaTime);
    
    DirectX::SimpleMath::Quaternion Rotator = GetLocalRotationQuat();
    
    Rotator *= DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(RotationAxis, SelfRotationSpeed * DeltaTime);
    SetLocalRotationQuat(Rotator);

    if(ParentPlanet != nullptr && !XMVector3Equal(OrbitalAxis, DirectX::XMVectorZero()))
    {
        OrbitalRotation *= DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(OrbitalAxis, OrbitalRotationSpeed * DeltaTime);
        OrbitalRotation.Normalize();

        OrbitalTranslation = DirectX::SimpleMath::Matrix::CreateFromQuaternion(OrbitalRotation) * DirectX::SimpleMath::Matrix::CreateTranslation(ParentPlanet->GetLocalTranslation());
    }
}

DirectX::SimpleMath::Vector3 Planet::GetLocalTranslation2() const
{
    return DirectX::SimpleMath::Vector3::Transform(GetLocalTranslation(), OrbitalTranslation);
}

void Planet::Construct()
{

}

void Planet::Init()
{
    OrbitalAxis.Normalize();
    RotationAxis.Normalize();

    FGameObject::Init();
}
