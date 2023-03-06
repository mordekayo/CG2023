#include "Planet.h"

#include <cmath>
#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>

#include "../../GameFramework/Game.h"
#include "../../GameFramework/Components/RenderComponent.h"

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
    
    DirectX::SimpleMath::Quaternion Rotator = GetRotationQuat();
    
    Rotator *= DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(RotationAxis, SelfRotationSpeed * DeltaTime);
    SetRotationQuat(Rotator);

    if(ParentPlanet != nullptr && !XMVector3Equal(OrbitalAxis, DirectX::XMVectorZero()))
    {
        OrbitalRotation *= DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(OrbitalAxis, OrbitalRotationSpeed * DeltaTime);
        OrbitalRotation.Normalize();

        OrbitalTranslation = DirectX::SimpleMath::Matrix::CreateFromQuaternion(OrbitalRotation) * DirectX::SimpleMath::Matrix::CreateTranslation(ParentPlanet->GetTransform());
    }
    
    UpdateWorldMatrix();
}

DirectX::SimpleMath::Vector3 Planet::GetTransform() const
{
    return DirectX::SimpleMath::Vector3::Transform(GetLocalTransform(), OrbitalTranslation);
}

DirectX::SimpleMath::Vector3 Planet::GetLocalTransform() const
{
    return FGameObject::GetTransform();
}

void Planet::UpdateWorldMatrix()
{
    FGameObject::UpdateWorldMatrix();
    
    WorldView *= OrbitalTranslation;
}

void Planet::Construct()
{
    std::vector<DirectX::SimpleMath::Vector4> Vertexes;
    std::vector<int> Indices;
    
    Vertexes.push_back(DirectX::XMFLOAT4(0, Radius, 0, 1));
    Vertexes.push_back(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
    auto PI = 3.141592653589793238462643;
    auto phiStep = PI / VerticalSectors;
    auto thetaStep = 2.0f * PI / HorizontalSectors;
    
    for (int i = 1; i <= VerticalSectors - 1; i++)
    {
        
         auto phi = i * phiStep;
         for (int j = 0; j <= HorizontalSectors; j++)
         {
             auto theta = j * thetaStep;
             Vertexes.push_back(DirectX::XMFLOAT4(Radius * sin(phi) * cos(theta),
                 Radius * cos(phi),
                 Radius * sin(phi) * sin(theta),
                    1.0f));

             float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
             float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
             float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

             Vertexes.push_back({ x, y, z, 1.0f });
         }
    }
    Vertexes.push_back(DirectX::XMFLOAT4(0, -Radius, 0, 1));
    Vertexes.push_back(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

    for (int i = 1; i <= HorizontalSectors; i++)
    {
        Indices.push_back(0);
        Indices.push_back(i + 1);
        Indices.push_back(i);
    }
    
    auto baseIndex = 1;
    auto ringVertexCount = HorizontalSectors + 1;

    for (int i = 0; i < VerticalSectors - 2; i++)
    {
        for (int j = 0; j < HorizontalSectors; j++)
        {
            Indices.push_back(baseIndex + i * ringVertexCount + j);
            Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
            Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

            Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
            Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
            Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
        }
    }

    auto southPoleIndex = Indices.size() - 1;
    baseIndex = southPoleIndex - ringVertexCount;
    for (int i = 0; i < HorizontalSectors; i++) 
    {
        Indices.push_back(southPoleIndex);
        Indices.push_back(baseIndex + i);
        Indices.push_back(baseIndex + i + 1);
    }

    FRenderComponent* SphereMesh = new FRenderComponent();
    SphereMesh->SetPoints(std::move(Vertexes));
    SphereMesh->SetIndices(std::move(Indices));
    SphereMesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    AddComponent(SphereMesh);
}

void Planet::Init()
{
    //if (OrbitalAxis.Dot(GetLocalTransform()) != 0) 
    //{
    //    DirectX::SimpleMath::Vector3 Normal = OrbitalAxis.Cross(GetLocalTransform());
    //    OrbitalAxis = Normal.Cross(GetLocalTransform());
    //}

    OrbitalAxis.Normalize();
    RotationAxis.Normalize();

    FGameObject::Init();
}
