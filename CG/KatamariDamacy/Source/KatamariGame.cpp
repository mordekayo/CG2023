#include "KatamariGame.h"

#include "PlayerController.h"
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

    FGameObject* Terrain = new FGameObject();
    FRenderComponent* TerrainMesh =
        new FRenderComponent(L"../GameFramework/Source/Shaders/MyTexturedShader.hlsl"
                            ,L"../GameFramework/Source/Textures/Wood.png");
    TerrainMesh->SetMeshPlane(64);
    Terrain->AddComponent(TerrainMesh);
    
    FGameObject* Player = new FGameObject();

    FRenderComponent* SphereMesh =
    new FRenderComponent(L"../GameFramework/Source/Shaders/MyTexturedShader.hlsl"
                            ,L"../GameFramework/Source/Textures/DarkGreen.png");    
    SphereMesh->AddMesh(0.5f, "../GameFramework/Source/Models/WireframeBall.obj");
    SphereMesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    Player->AddComponent(SphereMesh);

    Player->SetTranslation({0.0f,-0.04f,0.0f});
    
    AddGameObject(Player);
    AddGameObject(Terrain);

    PlayerController = new FPlayerController();
    PlayerController->Possess(Player);
    PlayerController->SetCamera(FGame::Instance()->GetCamera());

    FreeCameraController = new FFPSCameraController();
    FreeCameraController->SetCamera(FGame::Instance()->GetCamera());
}

void KatamariGame::Update(float DeltaTime)
{
    if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::F))
    {
        bFreeCamera = true;
    }
    if (FGame::Instance()->GetInputDevice()->IsKeyDown(Keys::G))
    {
        bFreeCamera = false;
    }
    
    if(bFreeCamera)
    {
        PlayerController->Update(DeltaTime);
    }
    else
    {
        FreeCameraController->Update(DeltaTime);
    }

    FGame::Update(DeltaTime);
}
