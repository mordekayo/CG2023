#include "PongGame.h"
#include <DirectXMath.h>
#include "../GameFramework/Components/RenderComponent.h"
#include "../GameFramework/GameObjects/GameObject.h"
#include "../GameFramework/Utils/InputDevice.h"

PongGame::PongGame()
{
	// ApplicationName = L"Pong";
	// ScreenWidth = 800;
	// ScreenHeight = 800;
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
		LeftPlayerRacketCollision->Center =
			{
			LeftPlayerRacketCollision->Center.x + BallDirection.x * BallSpeed * DeltaTime,
			LeftPlayerRacketCollision->Center.y + BallDirection.y * BallSpeed * DeltaTime,
			0.0f
			};
	}
	if (Input->IsKeyDown(Keys::S)) 
	{
		LeftPlayer->AddTransform({0.0f, -0.5f * DeltaTime, 0.0f, 0.0f});
		LeftPlayerRacketCollision->Center =
			{
			LeftPlayerRacketCollision->Center.x + BallDirection.x * BallSpeed * DeltaTime,
			LeftPlayerRacketCollision->Center.y + BallDirection.y * BallSpeed * DeltaTime,
			0.0f
			};
	}

	if (Input->IsKeyDown(Keys::Left))  { }
	if (Input->IsKeyDown(Keys::Right)) { }
	if (Input->IsKeyDown(Keys::Up))    
	{
		RightPlayer->AddTransform({0.0f, 0.5f * DeltaTime, 0.0f, 0.0f});
		RightPlayerRacketCollision->Center =
	{
			RightPlayerRacketCollision->Center.x + BallDirection.x * BallSpeed * DeltaTime,
			RightPlayerRacketCollision->Center.y + BallDirection.y * BallSpeed * DeltaTime,
			0.0f
			};
	}
	if (Input->IsKeyDown(Keys::Down))  
	{
		RightPlayer->AddTransform({0.0f, -0.5f * DeltaTime, 0.0f, 0.0f});
		RightPlayerRacketCollision->Center =
	{
			RightPlayerRacketCollision->Center.x + BallDirection.x * BallSpeed * DeltaTime,
			RightPlayerRacketCollision->Center.y + BallDirection.y * BallSpeed * DeltaTime,
			0.0f
			};
	}

	BallDirection.Normalize();
	Ball->AddTransform(DirectX::XMVectorScale( BallDirection, BallSpeed * DeltaTime));
	BallCollision->Center = { BallCollision->Center.x + BallDirection.x * BallSpeed * DeltaTime, BallCollision->Center.y + BallDirection.y * BallSpeed * DeltaTime,  0.0f };
	BallCollisionVisualizer->AddTransform(XMVectorScale( BallDirection, BallSpeed * DeltaTime));

	float Distance1;
	DirectX::FXMVECTOR BallCenter =
	{
		BallCollision->Center.x,
		BallCollision->Center.y,
		BallCollision->Center.z
	};
	BallDirection.Normalize();
	// if(LeftPlayerRacketCollision->Intersects(BallCenter, BallDirection, Distance1))
	// {
	// 	if(BallCollision->Center.x + BallDirection.x * BallSpeed * DeltaTime >
	// 		LeftPlayerRacketCollision->Center.x - LeftPlayerRacketCollision->Extents.x - 0.01f)
	// 	{
	// 		Ball->AddTransform(BallDirection * Distance1);
	// 		BallCollision->Center = { BallCollision->Center.x + BallDirection.x * Distance1, BallCollision->Center.y + BallDirection.y * Distance1,  0.0f };
	// 		
	// 		DirectX::SimpleMath::Vector3 vector0 =
	// 		{
	// 			BallCollision->Center.x - (LeftPlayerRacketCollision->Center.x + LeftPlayerRacketCollision->Extents.x * 2),
	// 			BallCollision->Center.y - LeftPlayerRacketCollision->Center.y,
	// 			0.0f
	// 		};
	//
	// 		vector0.Normalize();
	// 		const DirectX::SimpleMath::Vector3 vector = DirectX::SimpleMath::Vector3::Reflect({BallDirection.x, BallDirection.y, BallDirection.z}, vector0);
	//
	// 		BallDirection.x = vector.x;
	// 		BallDirection.y = vector.y;
	// 		BallDirection.z = vector.z;
	// 		
	// 		//BallSpeed += 0.1f;
	// 	}
	// }
	// if(RightPlayerRacketCollision->Intersects(BallCenter, BallDirection, Distance1))
	// {
	// 	if(BallCollision->Center.x + BallDirection.x * BallSpeed * DeltaTime >
	// 		RightPlayerRacketCollision->Center.x - RightPlayerRacketCollision->Extents.x - 0.01f)
	// 	{
	// 		Ball->AddTransform(BallDirection * Distance1);
	// 		BallCollision->Center = { BallCollision->Center.x + BallDirection.x * Distance1, BallCollision->Center.y + BallDirection.y * Distance1,  0.0f };
	// 		
	// 		DirectX::SimpleMath::Vector3 vector0 =
	// 		{
	// 			BallCollision->Center.x - (RightPlayerRacketCollision->Center.x + RightPlayerRacketCollision->Extents.x * 2),
	// 			BallCollision->Center.y - RightPlayerRacketCollision->Center.y,
	// 			0.0f
	// 		};
	//
	// 		vector0.Normalize();
	// 		const DirectX::SimpleMath::Vector3 vector = DirectX::SimpleMath::Vector3::Reflect({BallDirection.x, BallDirection.y, BallDirection.z}, vector0);
	//
	// 		BallDirection.x = vector.x;
	// 		BallDirection.y = vector.y;
	// 		BallDirection.z = vector.z;
	// 		
	// 		//BallSpeed += 0.1f;
	// 	}
	// }
	BallDirection.Normalize();
}

void PongGame::Construct()
{
	LeftPlayer = new FGameObject();
	RightPlayer = new FGameObject();
	Ball = new FGameObject();

	const auto LeftPlayerRacketMesh = new FRenderComponent();

	LeftPlayerRacketMesh->SetPoints(
	{
		DirectX::XMFLOAT4(-0.8f,  0.5f, 0.5f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-1.0f, -0.5f, 0.5f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-0.8f, -0.5f, 0.5f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-1.0f,  0.5f, 0.5f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
	});
	LeftPlayerRacketMesh->SetIndicies({ 0, 1, 2, 1, 0, 3 });
	
	LeftPlayer->AddComponent(LeftPlayerRacketMesh);
	
	LeftPlayerRacketCollision = new DirectX::BoundingBox();
	LeftPlayerRacketCollision->Center = {-0.8f, 0.0f, 0.0f };
	LeftPlayerRacketCollision->Extents = {0.11f, 0.51f, 0.0f };

	LeftPlayerRacketCollisionVisualizer = new FGameObject();
	LeftPlayerRacketCollisionVisualizer->AddTransform({ LeftPlayerRacketCollision->Center.x, LeftPlayerRacketCollision->Center.y, LeftPlayerRacketCollision->Center.z, 0.0f});
	const auto LeftPlayerRacketCollisionVisualizerMesh = new FRenderComponent();

	LeftPlayerRacketCollisionVisualizerMesh->SetPoints(
		{
			DirectX::XMFLOAT4(LeftPlayerRacketCollision->Center.x - LeftPlayerRacketCollision->Extents.x,  LeftPlayerRacketCollision->Center.y + LeftPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
			DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
			DirectX::XMFLOAT4(LeftPlayerRacketCollision->Center.x + LeftPlayerRacketCollision->Extents.x,  LeftPlayerRacketCollision->Center.y + LeftPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
			DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
			DirectX::XMFLOAT4(LeftPlayerRacketCollision->Center.x - LeftPlayerRacketCollision->Extents.x,  LeftPlayerRacketCollision->Center.y - LeftPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
			DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
			DirectX::XMFLOAT4(LeftPlayerRacketCollision->Center.x + LeftPlayerRacketCollision->Extents.x,  LeftPlayerRacketCollision->Center.y - LeftPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
			DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
		});
	LeftPlayerRacketCollisionVisualizerMesh->SetIndicies({ 0, 1, 1, 3, 3, 2, 2, 0, 0, 3, 1, 2 });
	LeftPlayerRacketCollisionVisualizerMesh->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	LeftPlayerRacketCollisionVisualizer->AddComponent(LeftPlayerRacketCollisionVisualizerMesh);
	
	const auto RightPlayerRacketMesh = new FRenderComponent();

	RightPlayerRacketMesh->SetPoints(
	{
		DirectX::XMFLOAT4(1.0f,  0.5f, 0.5f, 1.0f),   DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.8f, -0.5f, 0.5f, 1.0f),   DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, -0.5f, 0.5f, 1.0f),   DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.8f,  0.5f, 0.5f, 1.0f),   DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
	});

	RightPlayerRacketMesh->SetIndicies({ 0, 1, 2, 1, 0, 3 });

	RightPlayer->AddComponent(RightPlayerRacketMesh);
	
	RightPlayerRacketCollision = new DirectX::BoundingBox();
	RightPlayerRacketCollision->Center = {0.9f, 0.0f, 0.0f };
	RightPlayerRacketCollision->Extents = {0.11f, 0.51f, 0.0f };

	RightPlayerRacketCollisionVisualizer = new FGameObject();
	RightPlayerRacketCollisionVisualizer->AddTransform({ RightPlayerRacketCollision->Center.x, RightPlayerRacketCollision->Center.y, RightPlayerRacketCollision->Center.z, 0.0f});
	const auto RightPlayerRacketCollisionVisualizerMesh = new FRenderComponent();

	RightPlayerRacketCollisionVisualizerMesh->SetPoints(
	{
		DirectX::XMFLOAT4(RightPlayerRacketCollision->Center.x - RightPlayerRacketCollision->Extents.x,  RightPlayerRacketCollision->Center.y + RightPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(RightPlayerRacketCollision->Center.x + RightPlayerRacketCollision->Extents.x,  RightPlayerRacketCollision->Center.y + RightPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(RightPlayerRacketCollision->Center.x - RightPlayerRacketCollision->Extents.x,  RightPlayerRacketCollision->Center.y - RightPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(RightPlayerRacketCollision->Center.x + RightPlayerRacketCollision->Extents.x,  RightPlayerRacketCollision->Center.y - RightPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
	});
	RightPlayerRacketCollisionVisualizerMesh->SetIndicies({ 0, 1, 1, 3, 3, 2, 2, 0, 0, 3, 1, 2 });
	RightPlayerRacketCollisionVisualizerMesh->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	
	RightPlayerRacketCollisionVisualizer->AddComponent(RightPlayerRacketCollisionVisualizerMesh);
	
	const auto BallMesh = new FRenderComponent();

	BallMesh->SetPoints(
{
	DirectX::XMFLOAT4(-0.05f, 0.05f, 0.0f, 1.0f),    DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT4(0.05f, 0.05f, 0.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT4(0.05f, -0.05f, 0.0f, 1.0f),   DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT4(-0.05f, -0.05f, 0.0f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
});
	BallMesh->SetIndicies({ 0, 1, 2, 0, 2, 3 });
	
	Ball->AddComponent(BallMesh);
	
	BallCollision = new DirectX::BoundingSphere();
	BallCollision->Radius = 0.06f;
	
	BallCollisionVisualizer = new FGameObject();
	BallCollisionVisualizer->AddTransform({ BallCollision->Center.x, BallCollision->Center.y, BallCollision->Center.z, 0.0f});
	const auto BallCollisionVisualizerMesh = new FRenderComponent();

	BallCollisionVisualizerMesh->SetPoints(
	{
		DirectX::XMFLOAT4(BallCollision->Center.x - BallCollision->Radius,  BallCollision->Center.y + BallCollision->Radius, 0.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(BallCollision->Center.x + BallCollision->Radius,  BallCollision->Center.y + BallCollision->Radius, 0.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(BallCollision->Center.x - BallCollision->Radius,  BallCollision->Center.y - BallCollision->Radius, 0.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(BallCollision->Center.x + BallCollision->Radius,  BallCollision->Center.y - BallCollision->Radius, 0.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
	});
	BallCollisionVisualizerMesh->SetIndicies({ 0, 1, 1, 3, 3, 2, 2, 0, 0, 3, 1, 2 });
	BallCollisionVisualizerMesh->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	
	BallCollisionVisualizer->AddComponent(BallCollisionVisualizerMesh);
	
	//AddGameObject(LeftPlayer);
	AddGameObject(LeftPlayerRacketCollisionVisualizer);
	
	AddGameObject(RightPlayer);
	AddGameObject(RightPlayerRacketCollisionVisualizer);
	
	AddGameObject(Ball);
	AddGameObject(BallCollisionVisualizer);
}

void PongGame::Run()
{
	Construct();
	FGame::Run();
}
