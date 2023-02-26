#include "PongGame.h"
#include <DirectXMath.h>
#include "../GameFramework/Components/RenderComponent.h"
#include "../GameFramework/Components/CollisionComponents/BoxCollisionComponent.h"
#include "../GameFramework/Components/CollisionComponents/SphereCollisionComponent.h"
#include "../GameFramework/GameObjects/GameObject.h"
#include "../GameFramework/Utils/InputDevice.h"

PongGame::PongGame()
{

}

PongGame* PongGame::Instance()
{
	if (!GameInstance)
	{
		GameInstance = new PongGame();
	}
	return static_cast<PongGame*>(GameInstance);
}

void PongGame::Update(float DeltaTime)
{
	FGame::Update(DeltaTime);
	
	if (Input->IsKeyDown(Keys::A)) { }
	if (Input->IsKeyDown(Keys::D)) { }
	if (Input->IsKeyDown(Keys::W)) 
	{
		LeftPlayer->AddTransform({0.0f, 0.5f * DeltaTime, 0.0f, 0.0f}); 
	}
	if (Input->IsKeyDown(Keys::S)) 
	{
		LeftPlayer->AddTransform({0.0f, -0.5f * DeltaTime, 0.0f, 0.0f});  
	}

	if (Input->IsKeyDown(Keys::Left))  { }
	if (Input->IsKeyDown(Keys::Right)) { }
	if (Input->IsKeyDown(Keys::Up))    
	{
		RightPlayer->AddTransform({0.0f, 0.5f * DeltaTime, 0.0f, 0.0f});
	}
	if (Input->IsKeyDown(Keys::Down))  
	{
		RightPlayer->AddTransform({0.0f, -0.5f * DeltaTime, 0.0f, 0.0f});
	}

	Ball->AddTransform(DirectX::XMVectorScale( BallDirection, BallSpeed * DeltaTime));
}

void PongGame::Construct()
{
	LeftPlayer = new FGameObject();
	RightPlayer = new FGameObject();
	Ball = new FGameObject();
	
	FRenderComponent* LeftPlayerRacketMesh = new FRenderComponent();
	LeftPlayer->AddComponent(LeftPlayerRacketMesh);
	FBoxCollisionComponent* LeftPlayerRacketCollision = new FBoxCollisionComponent();
	LeftPlayer->AddComponent(LeftPlayerRacketCollision);
	
	FRenderComponent* RightPlayerRacketMesh = new FRenderComponent();
	RightPlayer->AddComponent(RightPlayerRacketMesh);
	FBoxCollisionComponent* RightPlayerRacketCollision = new FBoxCollisionComponent();
	RightPlayer->AddComponent(RightPlayerRacketCollision);
	
	FRenderComponent* BallMesh = new FRenderComponent();
	Ball->AddComponent(BallMesh);
	FSphereCollisionComponent* BallCollision = new FSphereCollisionComponent();
	BallCollision->SetRadius(0.053f);
	Ball->AddComponent(BallCollision);
	
	LeftPlayerRacketMesh->SetPoints(
	{
		DirectX::XMFLOAT4(-0.8f,  0.5f, 0.5f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-1.0f, -0.5f, 0.5f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-0.8f, -0.5f, 0.5f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-1.0f,  0.5f, 0.5f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
	});

	RightPlayerRacketMesh->SetPoints(
	{
		DirectX::XMFLOAT4(1.0f,  0.5f, 0.5f, 1.0f),   DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.8f, -0.5f, 0.5f, 1.0f),   DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, -0.5f, 0.5f, 1.0f),   DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.8f,  0.5f, 0.5f, 1.0f),   DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
	});

	BallMesh->SetPoints(
	{
		DirectX::XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f),    DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-0.06f, -0.1f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.0f, -0.1f, 0.5f, 1.0f),   DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-0.06f, 0.0f, 0.5f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
	});

	LeftPlayerRacketMesh->SetIndicies({ 0, 1, 2, 1, 0, 3 });
	RightPlayerRacketMesh->SetIndicies({ 0, 1, 2, 1, 0, 3 });
	BallMesh->SetIndicies({ 0, 1, 2, 1, 0, 3 });
	
	AddGameObject(LeftPlayer);
	AddGameObject(RightPlayer);
	AddGameObject(Ball);
}

void PongGame::Run()
{
	Construct();
	FGame::Run();
}
