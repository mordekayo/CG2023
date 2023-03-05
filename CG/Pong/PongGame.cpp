#include "PongGame.h"
#include <DirectXMath.h>
#include "../GameFramework/Components/RenderComponent.h"
#include "../GameFramework/GameObjects/GameObject.h"
#include "../GameFramework/Utils/InputDevice.h"
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

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

	//COUNT
	if (BallCollision->Center.x < -1.0f)
	{
		BallSpeed = 0.3f;
		Ball->SetPosition({0.0f,0.0f,0.0f, 0.0f});
		BallCollision->Center = {0.0f, 0.0f, 0.0f};
		BallCollisionVisualizer->SetPosition(Ball->GetTransform());
		BallDirection.x = 1.0f;
		BallDirection.y = 0.5f;
		BallDirection.z = 0.0f;
		Player1Points++;
		std::cout << "Second player WON! Count: " << Player1Points << "/" << Player2Points << std::endl;
	}
	if (BallCollision->Center.x > 1.0f)
	{
		BallSpeed = 0.3f;
		Ball->SetPosition({0.0f,0.0f,0.0f, 0.0f});
		BallCollision->Center = {0.0f, 0.0f, 0.0f};
		BallCollisionVisualizer->SetPosition(Ball->GetTransform());
		BallDirection.x = -1.0f;
		BallDirection.y = 0.5f;
		BallDirection.z = 0.0f;
		Player2Points++;
		std::cout << "First player WON! Count: " << Player1Points << "/" << Player2Points << std::endl;
	}
	
	if (Input->IsKeyDown(Keys::A)) { }
	if (Input->IsKeyDown(Keys::D)) { }
	if (Input->IsKeyDown(Keys::W)) 
	{
		if (LeftPlayer->GetTransform().y < 1.0f - 0.2)
		{
			LeftPlayer->AddTransform({0.0f, RacketSpeed * DeltaTime, 0.0f, 0.0f});
			LeftPlayerRacketCollision->Center =
				{
					LeftPlayerRacketCollision->Center.x,
					LeftPlayerRacketCollision->Center.y + RacketSpeed * DeltaTime,
					0.0f
				};
			LeftPlayerRacketCollisionVisualizer->AddTransform({ 0.0f, RacketSpeed * DeltaTime, 0.0f, 0.0f });
		}
	}
	if (Input->IsKeyDown(Keys::S)) 
	{
		if (LeftPlayer->GetTransform().y > -1.0f + 0.2)
		{
			LeftPlayer->AddTransform({0.0f, - RacketSpeed * DeltaTime, 0.0f, 0.0f});
			LeftPlayerRacketCollision->Center =
				{
				LeftPlayerRacketCollision->Center.x,
				LeftPlayerRacketCollision->Center.y - RacketSpeed * DeltaTime,
				0.0f
				};
			LeftPlayerRacketCollisionVisualizer->AddTransform({ 0.0f, - RacketSpeed * DeltaTime, 0.0f, 0.0f });
		}
	}
	
	if (Input->IsKeyDown(Keys::Left))  { }
	if (Input->IsKeyDown(Keys::Right)) { }
	if (Input->IsKeyDown(Keys::Up))    
	{
		if (RightPlayer->GetTransform().y < 1.0f - 0.2)
		{
			RightPlayer->AddTransform({ 0.0f, RacketSpeed * DeltaTime, 0.0f, 0.0f });
			RightPlayerRacketCollision->Center =
			{
				RightPlayerRacketCollision->Center.x,
				RightPlayerRacketCollision->Center.y + RacketSpeed * DeltaTime,
				0.0f
			};
			RightPlayerRacketCollisionVisualizer->AddTransform({ 0.0f, RacketSpeed * DeltaTime, 0.0f, 0.0f });
		}
	}
	if (Input->IsKeyDown(Keys::Down))  
	{
		if (RightPlayer->GetTransform().y > -1.0f + 0.2)
		{
			RightPlayer->AddTransform({0.0f, -RacketSpeed * DeltaTime, 0.0f, 0.0f});
			RightPlayerRacketCollision->Center =
				{
				RightPlayerRacketCollision->Center.x,
				RightPlayerRacketCollision->Center.y - RacketSpeed * DeltaTime,
				0.0f
				};
			RightPlayerRacketCollisionVisualizer->AddTransform({ 0.0f, -RacketSpeed * DeltaTime, 0.0f, 0.0f });
		}
	}
	
	BallDirection.Normalize();

	Ball->AddTransform(BallDirection * BallSpeed * DeltaTime);
	BallCollision->Center = { BallCollision->Center.x + BallDirection.x * BallSpeed * DeltaTime, BallCollision->Center.y + BallDirection.y * BallSpeed * DeltaTime,  0.0f };
	BallCollisionVisualizer->SetPosition({BallCollision->Center.x, BallCollision->Center.y, BallCollision->Center.z, 0.0f});
	
	float distance1;
	if (RightPlayerRacketCollision->Intersects({ BallCollision->Center.x, BallCollision->Center.y, BallCollision->Center.z }, BallDirection, distance1))
	{
		if (BallCollision->Center.x + BallDirection.x * BallSpeed * DeltaTime > RightPlayerRacketCollision->Center.x - RightPlayerRacketCollision->Extents.x - 0.01f)
		{
			Ball->SetPosition({BallCollision->Center.x + BallDirection.x * distance1, BallCollision->Center.y + BallDirection.y * distance1, 0.0f, 0.0f });
			// REFLECT
			DirectX::SimpleMath::Vector3 vector0 = {
				BallCollision->Center.x - (RightPlayerRacketCollision->Center.x + RightPlayerRacketCollision->Extents.x * 2),
				BallCollision->Center.y - RightPlayerRacketCollision->Center.y,
				0.0f
			};
			vector0.Normalize();
			DirectX::SimpleMath::Vector3 vector = DirectX::SimpleMath::Vector3::Reflect({ BallDirection.x, BallDirection.y, BallDirection.z }, vector0);
			BallDirection.x = vector.x;
			BallDirection.y = vector.y;
			BallDirection.z = vector.z;

			BallSpeed += 0.1f;
		}
		else
		{
			Ball->AddTransform(DirectX::XMVectorScale(BallDirection, BallSpeed * DeltaTime));
			BallCollision->Center = { Ball->GetTransform().x,  Ball->GetTransform().y, 0.0f };
		}
	}
	else
	{
		float distance2;
		if (LeftPlayerRacketCollision->Intersects({ BallCollision->Center.x, BallCollision->Center.y, BallCollision->Center.z }, BallDirection, distance2))
		{
			if (BallCollision->Center.x + BallDirection.x * BallSpeed * DeltaTime < LeftPlayerRacketCollision->Center.x + LeftPlayerRacketCollision->Extents.x + 0.01f)
			{
				Ball->SetPosition({BallCollision->Center.x + BallDirection.x * distance2, BallCollision->Center.y + BallDirection.y * distance2, 0.0f, 0.0f });
				// REFLECT
				DirectX::SimpleMath::Vector3 vector0 = {
					BallCollision->Center.x - (LeftPlayerRacketCollision->Center.x - LeftPlayerRacketCollision->Extents.x * 2),
					BallCollision->Center.y - LeftPlayerRacketCollision->Center.y,
					0.0f
				};
				vector0.Normalize();
				DirectX::SimpleMath::Vector3 vector = DirectX::SimpleMath::Vector3::Reflect({ BallDirection.x, BallDirection.y, BallDirection.z }, vector0);
				BallDirection.x = vector.x;
				BallDirection.y = vector.y;
				BallDirection.z = vector.z;

				BallSpeed += 0.1f;
			}
			else
			{
				Ball->AddTransform(DirectX::XMVectorScale(BallDirection, BallSpeed * DeltaTime));
				BallCollision->Center = { Ball->GetTransform().x,  Ball->GetTransform().y, 0.0f };
			}
		}
		else
		{
			float distance3;
			if (TopWall->Intersects({ BallCollision->Center.x, BallCollision->Center.y, BallCollision->Center.z }, BallDirection, distance3))
			{
				if (BallCollision->Center.y + BallDirection.y * BallSpeed * DeltaTime > TopWall->Center.y - TopWall->Extents.y - 0.01f)
				{
					Ball->SetPosition({ BallCollision->Center.x + BallDirection.x * distance3, BallCollision->Center.y + BallDirection.y * distance3, 0.0f, 0.0f });
					// REFLECT
					DirectX::SimpleMath::Vector3 vector = DirectX::SimpleMath::Vector3::Reflect({ BallDirection.x, BallDirection.y, BallDirection.z }, { 0.0f, -1.0f, 0.0f });
					BallDirection = { vector.x, vector.y, vector.z };
				}
				else
				{
					Ball->AddTransform(DirectX::XMVectorScale(BallDirection, BallSpeed * DeltaTime));
					BallCollision->Center = { Ball->GetTransform().x,  Ball->GetTransform().y, 0.0f };
				}
			}
			else
			{
				float distance4;
				if (BottomWall->Intersects({ BallCollision->Center.x, BallCollision->Center.y, BallCollision->Center.z }, BallDirection, distance4))
				{
					if (BallCollision->Center.y + BallDirection.y * BallSpeed * DeltaTime < BottomWall->Center.y + BottomWall->Extents.y + 0.01f)
					{
						Ball->SetPosition({ BallCollision->Center.x + BallDirection.x * distance4, BallCollision->Center.y + BallDirection.y * distance4, 0.0f, 0.0f });
						BallCollision->Center = { BallCollision->Center.x + BallDirection.x * distance4, BallCollision->Center.y + BallDirection.y * distance4, 0.0f };
						// REFLECT
						DirectX::SimpleMath::Vector3 vector = DirectX::SimpleMath::Vector3::Reflect({ BallDirection.x, BallDirection.y, BallDirection.z }, { 0.0f, 1.0f, 0.0f });
						BallDirection = { vector.x, vector.y, vector.z };
					}
				}
				else
				{
					Ball->AddTransform(DirectX::XMVectorScale(BallDirection, BallSpeed * DeltaTime));
					BallCollision->Center = { Ball->GetTransform().x,  Ball->GetTransform().y, 0.0f };
				}
			}
		}
	}
	BallDirection.Normalize();
}

void PongGame::Construct()
{
	// LEFT WALL
	// COLLISION
	LeftWall = new DirectX::BoundingBox();
	LeftWall->Center  = { -1.5f, 0.0f, 0.0f };
	LeftWall->Extents = { 0.5f, 1.0f, 1.0f };
	
	// RIGHT WALL
	// COLLISION
	RightWall = new DirectX::BoundingBox();
	RightWall->Center  = { 1.5f, 0.0f, 0.0f };
	RightWall->Extents = { 0.5f, 1.0f, 1.0f };
	
	LeftPlayer = new FGameObject();
	RightPlayer = new FGameObject();
	Ball = new FGameObject();

	//Left 
	FRenderComponent* LeftPlayerRacketMesh = new FRenderComponent();

	LeftPlayerRacketMesh->SetPoints(
	{
		DirectX::XMFLOAT4(-0.05f,  0.2f, 0.0f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.05f, 0.2f, 0.0f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.05f, -0.2f, 0.0f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-0.05f,  -0.2f, 0.0f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
	});
	
	LeftPlayerRacketMesh->SetIndices({ 0, 1, 2, 3, 0 });
	LeftPlayerRacketMesh->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
	
	LeftPlayer->AddTransform({-0.9f, 0.0f, 0.0f, 0.0f});
	LeftPlayer->AddComponent(LeftPlayerRacketMesh);
	
	LeftPlayerRacketCollision = new DirectX::BoundingBox();
	LeftPlayerRacketCollision->Center = { LeftPlayer->GetTransform().x, LeftPlayer->GetTransform().y ,LeftPlayer->GetTransform().z };
	LeftPlayerRacketCollision->Extents = {0.05f, 0.25f, 1.0f };
	
	LeftPlayerRacketCollisionVisualizer = new FGameObject();
	LeftPlayerRacketCollisionVisualizer->AddTransform(LeftPlayer->GetTransform());
	const auto LeftPlayerRacketCollisionVisualizerMesh = new FRenderComponent();
	
	LeftPlayerRacketCollisionVisualizerMesh->SetPoints(
		{
			DirectX::XMFLOAT4(- LeftPlayerRacketCollision->Extents.x, LeftPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
			DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
			DirectX::XMFLOAT4(LeftPlayerRacketCollision->Extents.x,  LeftPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
			DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
			DirectX::XMFLOAT4(LeftPlayerRacketCollision->Extents.x,  - LeftPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
			DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
			DirectX::XMFLOAT4(- LeftPlayerRacketCollision->Extents.x,  - LeftPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
			DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
		});
	LeftPlayerRacketCollisionVisualizerMesh->SetIndices({ 0, 1, 2, 3, 0 });
	LeftPlayerRacketCollisionVisualizerMesh->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
	
	LeftPlayerRacketCollisionVisualizer->AddComponent(LeftPlayerRacketCollisionVisualizerMesh);

	//Right
	const auto RightPlayerRacketMesh = new FRenderComponent();
	
	RightPlayerRacketMesh->SetPoints(
	{
		DirectX::XMFLOAT4(-0.05f,  0.2f, 0.0f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.05f, 0.2f, 0.0f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.05f, -0.2f, 0.0f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-0.05f,  -0.2f, 0.0f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
	});
	RightPlayerRacketMesh->SetIndices({ 0, 1, 2, 3, 0 });
	RightPlayerRacketMesh->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
	
	RightPlayer->AddTransform({0.9f, 0.0f, 0.0f, 0.0f});
	RightPlayer->AddComponent(RightPlayerRacketMesh);
	
	RightPlayerRacketCollision = new DirectX::BoundingBox();
	RightPlayerRacketCollision->Center = { RightPlayer->GetTransform().x, RightPlayer->GetTransform().y ,RightPlayer->GetTransform().z };
	RightPlayerRacketCollision->Extents = {0.05f, 0.25f, 1.0f };
	
	RightPlayerRacketCollisionVisualizer = new FGameObject();
	RightPlayerRacketCollisionVisualizer->AddTransform(RightPlayer->GetTransform());
	const auto RightPlayerRacketCollisionVisualizerMesh = new FRenderComponent();
	
	RightPlayerRacketCollisionVisualizerMesh->SetPoints(
		{
			DirectX::XMFLOAT4(- RightPlayerRacketCollision->Extents.x, RightPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
			DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
			DirectX::XMFLOAT4(RightPlayerRacketCollision->Extents.x,  RightPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
			DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
			DirectX::XMFLOAT4(RightPlayerRacketCollision->Extents.x,  - RightPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
			DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
			DirectX::XMFLOAT4(- RightPlayerRacketCollision->Extents.x,  - RightPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
			DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
		});
	RightPlayerRacketCollisionVisualizerMesh->SetIndices({ 0, 1, 2, 3, 0 });
	RightPlayerRacketCollisionVisualizerMesh->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
	
	RightPlayerRacketCollisionVisualizer->AddComponent(RightPlayerRacketCollisionVisualizerMesh);
	
	// Ball
	
	const auto BallMesh = new FRenderComponent();
	std::vector<DirectX::SimpleMath::Vector4> BallPoints;
	std::vector<int> BallIndicies;
	int BallSides = 32;
	float BallRadius = 0.03f;
	float Angle = 0.0f;
	float AngleStep = (2 * M_PI) / BallSides;
	
	for (int i = 0; i < BallSides * 2; ++i)
	{
		BallIndicies.push_back(0);
		
		Angle = AngleStep * i;
		BallPoints.push_back({ cos(Angle) * BallRadius, sin(Angle) * BallRadius, 0.0f, 1.0f });
		BallPoints.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });
		BallIndicies.push_back(i + 1);
	
		Angle = AngleStep * (i + 1);
		BallPoints.push_back({ cos(Angle) * BallRadius, sin(Angle) * BallRadius, 0.0f, 1.0f });
		BallPoints.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });
		BallIndicies.push_back(i + 2);
	}
	
	BallMesh->SetPoints(std::move(BallPoints));
	BallMesh->SetIndices(std::move(BallIndicies));
	BallMesh->SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	Ball->AddComponent(BallMesh);
	
	BallCollision = new DirectX::BoundingSphere();
	BallCollision->Radius = BallRadius + 0.01f;
	BallCollision->Center = {0.0f, 0.0f, 0.0f};
	
	BallCollisionVisualizer = new FGameObject();
	BallCollisionVisualizer->AddTransform({ BallCollision->Center.x, BallCollision->Center.y, BallCollision->Center.z, 0.0f});
	
	const auto BallCollisionVisualizerMesh = new FRenderComponent();
	
	std::vector<DirectX::SimpleMath::Vector4> BallCollisionVisualizerPoints;
	std::vector<int> BallCollisionVisualizerIndicies;
	
	for (int i = 0; i < BallSides * 2; ++i)
	 {
	 	Angle = AngleStep * i;
	 	BallCollisionVisualizerPoints.push_back({ cos(Angle) * BallCollision->Radius + 0.01f, sin(Angle) * BallCollision->Radius + 0.01f, 0.0f, 1.0f });
	 	BallCollisionVisualizerPoints.push_back({ 1.0f,0.7f, 1.0f, 1.0f });
	 	BallCollisionVisualizerIndicies.push_back(i + 1);
	
	 	Angle = AngleStep * (i + 1);
	 	BallCollisionVisualizerPoints.push_back({ cos(Angle) * BallCollision->Radius + 0.01f, sin(Angle) * BallCollision->Radius + 0.01f, 0.0f, 1.0f });
	 	BallCollisionVisualizerPoints.push_back({ 1.0f, 0.7f, 1.0f, 1.0f });
	 	BallCollisionVisualizerIndicies.push_back(i + 2);
	}
	
	BallCollisionVisualizerMesh->SetPoints(std::move(BallCollisionVisualizerPoints));
	BallCollisionVisualizerMesh->SetIndices(std::move(BallCollisionVisualizerIndicies));
	BallCollisionVisualizerMesh->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
	
	BallCollisionVisualizer->AddComponent(BallCollisionVisualizerMesh);

 	// TOP WALL
 	//COLLISION
 	TopWall = new DirectX::BoundingBox();
 	TopWall->Center  = { 0.0f, 1.5f, 0.0f };
 	TopWall->Extents = { 1.0f + BallRadius, 0.5f + BallRadius, 0.0f };
 	FRenderComponent* RCTopWall = new FRenderComponent();
 	RCTopWall->SetPoints(
 	{
 		DirectX::XMFLOAT4(TopWall->Center.x - TopWall->Extents.x,   TopWall->Center.y + TopWall->Extents.y,  0.0f,  1.0f),  DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
 		DirectX::XMFLOAT4(TopWall->Center.x + TopWall->Extents.x,   TopWall->Center.y + TopWall->Extents.y,  0.0f,  1.0f),  DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
 		DirectX::XMFLOAT4(TopWall->Center.x - TopWall->Extents.x,   TopWall->Center.y - TopWall->Extents.y,  0.0f,  1.0f),  DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
 		DirectX::XMFLOAT4(TopWall->Center.x + TopWall->Extents.x,   TopWall->Center.y - TopWall->Extents.y,  0.0f,  1.0f),  DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f)
 	});
 	RCTopWall->SetIndices({ 0, 1, 1, 3, 3, 2, 2, 0, 0, 3, 1, 2 });
 	RCTopWall->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	// BOTTOM WALL
 	// COLLISION
 	BottomWall = new DirectX::BoundingBox();
 	BottomWall->Center = { 0.0f, -1.5f, 0.0f };
 	BottomWall->Extents = { 1.0f + BallRadius, 0.5f + BallRadius, 0.0f };
 	FRenderComponent* RCBottomWall = new FRenderComponent();
 	RCBottomWall->SetPoints(
 	{
 		DirectX::XMFLOAT4(BottomWall->Center.x - BottomWall->Extents.x,   BottomWall->Center.y + BottomWall->Extents.y,  0.0f,  1.0f),  DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
 		DirectX::XMFLOAT4(BottomWall->Center.x + BottomWall->Extents.x,   BottomWall->Center.y + BottomWall->Extents.y,  0.0f,  1.0f),  DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
 		DirectX::XMFLOAT4(BottomWall->Center.x - BottomWall->Extents.x,   BottomWall->Center.y - BottomWall->Extents.y,  0.0f,  1.0f),  DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
 		DirectX::XMFLOAT4(BottomWall->Center.x + BottomWall->Extents.x,   BottomWall->Center.y - BottomWall->Extents.y,  0.0f,  1.0f),  DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f)
 	});
 	RCBottomWall->SetIndices({ 0, 1, 1, 3, 3, 2, 2, 0, 0, 3, 1, 2 });
	RCBottomWall->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	
	AddGameObject(LeftPlayer);
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
