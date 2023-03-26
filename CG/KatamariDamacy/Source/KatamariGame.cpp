#include "KatamariGame.h"

#include <iostream>
#include <random>

#include "PlayerController.h"
#include "Camera/Camera.h"
#include "Components/RenderComponent.h"
#include "Components/CollisionComponents/SphereCollisionComponent.h"

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
    SphereMesh->Init();

    FSphereCollisionComponent* BallCollision = new FSphereCollisionComponent(0.09f);
    Player->AddComponent(BallCollision);
    BallCollision->Init();
    
    Player->SetTranslation({0.0f,0.1f,-1.0f});
    
    PlayerController = new FPlayerController();
    PlayerController->SetTranslation({0.0f,0.0f,-1.0f});
    PlayerController->Possess(Player);
    
    FCamera* PlayerCamera = new FCamera();
    PlayerController->SetCamera(PlayerCamera);
    SetCurrentCamera(PlayerController->GetCamera());
    
    for(int i = 0; i < 10; i++)
    {
        FGameObject* NewObject = new FGameObject();

        const float ObjectScale = ScaleDistribution(Gen);
        
        FRenderComponent* Mesh =
                new FRenderComponent(L"../GameFramework/Source/Shaders/MyTexturedShader.hlsl"
                                    ,L"../GameFramework/Source/Textures/Gift.png");
        Mesh->AddMesh(ObjectScale, "../GameFramework/Source/Models/GiftBox.obj");
        NewObject->AddComponent(Mesh);

        FSphereCollisionComponent* BallCollision = new FSphereCollisionComponent(1.7f * ObjectScale);
        BallCollision->SetComponentTranslation({0.0f, 1.0f * ObjectScale, 0.0f});
        NewObject->AddComponent(BallCollision);
        BallCollision->Init();
        
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
    FGame::Update(DeltaTime);
    
    for(std::vector<FGameObject*>::iterator GameObjectIt = Objects.begin(); GameObjectIt != Objects.end(); )
    {
        const bool bHasIntersection = (*GameObjectIt)->GetCollisionComponent()->IsIntersectsWithSphere(Player->GetCollisionComponent());
        if(bHasIntersection)
        {
            (*GameObjectIt)->SetTransform((*GameObjectIt)->GetWorldTransform() * Player->GetWorldTransform().Invert());
            (*GameObjectIt)->SetParent(Player);
            GameObjectIt = Objects.erase(GameObjectIt);
        }
        else
        {
            ++GameObjectIt;
        }
    }

}
