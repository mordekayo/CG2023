#include "PlayerBall.h"

#include "Components/RenderComponent.h"
#include "Components/CollisionComponents/SphereCollisionComponent.h"


PlayerBall::PlayerBall()
{
    FRenderComponent* SphereMesh =
    new FRenderComponent(L"../GameFramework/Source/Shaders/MyTexturedShader.hlsl"
                            ,L"../GameFramework/Source/Textures/core_01.png");    
    SphereMesh->AddMesh(3.0f, "../GameFramework/Source/Models/core_01.obj");
    SphereMesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    AddComponent(SphereMesh);
    SphereMesh->Init();

    FSphereCollisionComponent* BallCollision = new FSphereCollisionComponent(0.09f);
    AddComponent(BallCollision);
    BallCollision->Init();
}

void PlayerBall::Construct()
{
    FGameObject::Construct();
}
