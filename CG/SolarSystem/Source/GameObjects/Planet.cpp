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
    // std::vector<DirectX::SimpleMath::Vector4> Vertexes;
    // std::vector<int> Indices;
    //
    // Vertexes.push_back(DirectX::XMFLOAT4(0, Radius, 0, 1));
    // Vertexes.push_back(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
    // auto PI = 3.141592653589793238462643;
    // auto phiStep = PI / VerticalSectors;
    // auto thetaStep = 2.0f * PI / HorizontalSectors;
    //
    // for (int i = 1; i <= VerticalSectors - 1; i++)
    // {
    //     
    //      auto phi = i * phiStep;
    //      for (int j = 0; j <= HorizontalSectors; j++)
    //      {
    //          auto theta = j * thetaStep;
    //          Vertexes.push_back(DirectX::XMFLOAT4(Radius * sin(phi) * cos(theta),
    //              Radius * cos(phi),
    //              Radius * sin(phi) * sin(theta),
    //                 1.0f));
    //
    //          float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    //          float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    //          float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    //
    //          Vertexes.push_back({ x, y, z, 1.0f });
    //      }
    // }
    // Vertexes.push_back(DirectX::XMFLOAT4(0, -Radius, 0, 1));
    // Vertexes.push_back(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
    //
    // for (int i = 1; i <= HorizontalSectors; i++)
    // {
    //     Indices.push_back(0);
    //     Indices.push_back(i + 1);
    //     Indices.push_back(i);
    // }
    //
    // auto baseIndex = 1;
    // auto ringVertexCount = HorizontalSectors + 1;
    //
    // for (int i = 0; i < VerticalSectors - 2; i++)
    // {
    //     for (int j = 0; j < HorizontalSectors; j++)
    //     {
    //         Indices.push_back(baseIndex + i * ringVertexCount + j);
    //         Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
    //         Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
    //
    //         Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
    //         Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
    //         Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
    //     }
    // }
    //
    // auto southPoleIndex = Indices.size() - 1;
    // baseIndex = southPoleIndex - ringVertexCount;
    // for (int i = 0; i < HorizontalSectors; i++) 
    // {
    //     Indices.push_back(southPoleIndex);
    //     Indices.push_back(baseIndex + i);
    //     Indices.push_back(baseIndex + i + 1);
    // }

    FRenderComponent* SphereMesh =
        new FRenderComponent(L"../GameFramework/Source/Shaders/MyTexturedShader.hlsl"
                                ,L"../GameFramework/Source/Textures/Wood.png");
    SphereMesh->AddMesh(0.02f, "../GameFramework/Source/Models/13517_Beach_Ball_v2_L3.obj");
    SphereMesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    AddComponent(SphereMesh);
}

void Planet::Init()
{
    OrbitalAxis.Normalize();
    RotationAxis.Normalize();

    FGameObject::Init();
}
