#include "Planet.h"

#include <cmath>
#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>

#include "../../GameFramework/Game.h"
#include "../../GameFramework/Components/RenderComponent.h"

Planet::Planet(std::string PlanetName, float PlanetRadius, float PlanetSelfRotationSpeed, DirectX::SimpleMath::Vector3 PlanetRotationAxis)
: Name(std::move(PlanetName)), Radius(PlanetRadius), SelfRotationSpeed(PlanetSelfRotationSpeed), RotationAxis(PlanetRotationAxis)
{
    
}

void Planet::Update(float DeltaTime)
{
    DirectX::SimpleMath::Quaternion Rotator = GetRotationQuat();

    Rotator *= DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(RotationAxis, SelfRotationSpeed * DeltaTime);

    SetRotationQuat(Rotator);

    std::cout << "x: " << GetRotationEuler().x <<
                 " y: " << GetRotationEuler().y <<
                 " z: " << GetRotationEuler().z << std::endl;

    FGameObject::Update(DeltaTime);
}

void Planet::Construct()
{
    std::vector<DirectX::SimpleMath::Vector4> Vertexes;
    std::vector<int> Indices;

    DirectX::XMFLOAT4 colorBase[] = { { 1.0f, 0.3f, 0.3f, 1.0f }, { 0.5f, 1.0f, 0.5f, 1.0f }, { 0.7f, 0.7f, 1.0f, 1.0f }, { 0.9f, 0.9f, 0.9f, 1.0f } };
    DirectX::XMFLOAT4 color;
    
    Vertexes.push_back(DirectX::XMFLOAT4(0, Radius, 0, 1));
    Vertexes.push_back(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
    auto PI = 3.141592653589793238462643;
    auto phiStep = PI / VerticalSectors;
    auto thetaStep = 2.0f * PI / HorizontalSectors;

    int g = 0;

    
    for (int i = 1; i <= VerticalSectors - 1; i++)
    {
        
         auto phi = i * phiStep;
         for (int j = 0; j <= HorizontalSectors; j++)
         {
             if (((j / 20 == g)) && (g < 4))
             {
                 color = colorBase[g++];
             }
             
             auto theta = j * thetaStep;
             Vertexes.push_back(DirectX::XMFLOAT4(Radius * sin(phi) * cos(theta),
                 Radius * cos(phi),
                 Radius * sin(phi) * sin(theta),
                    1.0f));

                 Vertexes.push_back(color);
         }
        g = 0;
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
    SphereMesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    AddComponent(SphereMesh);
}