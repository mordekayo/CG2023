#include "KatamariGame.h"

#include <iostream>
#include <random>

#include "PlayerController.h"
#include "Camera/Camera.h"
#include "Camera/FPSCameraController.h"
#include "Components/RenderComponent.h"
#include "Utils/DebugObject.h"

KatamariGame* KatamariGame::Instance()
{
    if (!GameInstance)
    {
        GameInstance = new KatamariGame();
    }
    return static_cast<KatamariGame*>(GameInstance);
}

void KatamariGame::Construct()
{
    FGame::Construct();

    std::random_device RandomDevice;  // Will be used to obtain a seed for the random number engine
    std::mt19937 Gen(RandomDevice()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> PlacementDistribution(-1, 1);
    std::uniform_real_distribution<> ScaleDistribution(0.01, 0.1);
    
    FGameObject* Terrain = new FGameObject();
    FRenderComponent* TerrainMesh =
        new FRenderComponent(L"../GameFramework/Source/Shaders/MyTexturedShader.hlsl"
                            ,L"../GameFramework/Source/Textures/moon.jpg");
    TerrainMesh->SetMeshPlane(4);
    Terrain->AddComponent(TerrainMesh);
    
    Player = new PlayerBall();

    FRenderComponent* SphereMesh =
    new FRenderComponent(L"../GameFramework/Source/Shaders/MyTexturedShader.hlsl"
                            ,L"../GameFramework/Source/Textures/core_01.png");    
    SphereMesh->AddMesh(3.0f, "../GameFramework/Source/Models/core_01.obj");
    SphereMesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    Player->AddComponent(SphereMesh);

    Player->SetTranslation({0.0f,0.1f,0.0f});
    
    PlayerController = new FPlayerController();
    PlayerController->Possess(Player);
    
    FCamera* PlayerCamera = new FCamera();
    PlayerController->SetCamera(PlayerCamera);
    SetCurrentCamera(PlayerController->GetCamera());

    std::vector<FGameObject*> Objects;
    for(int i = 0; i < 10; i++)
    {
        FGameObject* NewObject = new FGameObject();

        FRenderComponent* Mesh =
                new FRenderComponent(L"../GameFramework/Source/Shaders/MyTexturedShader.hlsl"
                                    ,L"../GameFramework/Source/Textures/Gift.png");
        Mesh->AddMesh(ScaleDistribution(Gen), "../GameFramework/Source/Models/GiftBox.obj");
        NewObject->AddComponent(Mesh);

        
        NewObject->SetTranslation({ static_cast<float>(PlacementDistribution(Gen)), 0.0f, static_cast<float>(PlacementDistribution(Gen))});
        Objects.push_back(NewObject);
    }
    
    AddGameObject(Player);
    AddGameObject(Terrain);
    AddGameObject(PlayerController);
    AddGameObjects(Objects);
}

void KatamariGame::Update(float DeltaTime)
{
    // if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::F))
    // {
    //     bFreeCamera = true;
    // }
    // if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::G))
    // {
    //     bFreeCamera = false;
    // }
    //
    // if(bFreeCamera)
    // {
    //     SetCurrentCamera(PlayerController->GetCamera());
    //     PlayerController->Update(DeltaTime);
    // }
    // else
    // {
    //     SetCurrentCamera(FreeCameraController->GetCamera());
    //     FreeCameraController->Update(DeltaTime);
    // }
    
    FGame::Update(DeltaTime);
}
