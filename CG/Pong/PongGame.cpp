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

	// COUNT
	// if (BallCollision->Center.x < -1.0f)
	// {
	// 	BallSpeed = 0.7f;
	// 	Ball->SetPosition({0.0f,0.0f,0.0f, 0.0f});
	// 	BallCollision->Center = {0.0f, 0.0f, 0.0f};
	// 	BallCollisionVisualizer->SetPosition(Ball->GetTransform());
	// 	BallDirection.x = -0.3f;
	// 	BallDirection.y = 0.1f;
	// 	BallDirection.x = 0.0f;
	// 	Player1Points++;
	// 	std::cout << "Second player WON! Count: " << Player1Points << "/" << Player2Points << std::endl;
	// }
	// if (BallCollision->Center.x > 1.0f)
	// {
	// 	BallSpeed = 0.7f;
	// 	Ball->SetPosition({0.0f,0.0f,0.0f, 0.0f});
	// 	BallCollision->Center = {0.0f, 0.0f, 0.0f};
	// 	BallCollisionVisualizer->SetPosition(Ball->GetTransform());
	// 	BallDirection.x = 0.3f;
	// 	BallDirection.y = 0.0f;
	// 	BallDirection.x = 0.0f;
	// 	Player2Points++;
	// 	std::cout << "First player WON! Count: " << Player1Points << "/" << Player2Points << std::endl;
	// }
	// 
	// if (Input->IsKeyDown(Keys::A)) { }
	// if (Input->IsKeyDown(Keys::D)) { }
	// if (Input->IsKeyDown(Keys::W)) 
	// {
	// 	LeftPlayer->AddTransform({0.0f, 0.5f * DeltaTime, 0.0f, 0.0f});
	// 	LeftPlayerRacketCollision->Center =
	// 		{
	// 			LeftPlayerRacketCollision->Center.x,
	// 			LeftPlayerRacketCollision->Center.y + 0.5f * DeltaTime,
	// 			0.0f
	// 		};
	// 	LeftPlayerRacketCollisionVisualizer->AddTransform({ 0.0f, 0.5f * DeltaTime, 0.0f, 0.0f });
	// }
	// if (Input->IsKeyDown(Keys::S)) 
	// {
	// 	LeftPlayer->AddTransform({0.0f, -0.5f * DeltaTime, 0.0f, 0.0f});
	// 	LeftPlayerRacketCollision->Center =
	// 		{
	// 		LeftPlayerRacketCollision->Center.x,
	// 		LeftPlayerRacketCollision->Center.y - 0.5f * DeltaTime,
	// 		0.0f
	// 		};
	// 	LeftPlayerRacketCollisionVisualizer->AddTransform({ 0.0f, -0.5f * DeltaTime, 0.0f, 0.0f });
	// }
	//
	// if (Input->IsKeyDown(Keys::Left))  { }
	// if (Input->IsKeyDown(Keys::Right)) { }
	// if (Input->IsKeyDown(Keys::Up))    
	// {
	// 	RightPlayer->AddTransform({0.0f, 0.5f * DeltaTime, 0.0f, 0.0f});
	// 	RightPlayerRacketCollision->Center =
	// 		{
	// 			RightPlayerRacketCollision->Center.x,
	// 			RightPlayerRacketCollision->Center.y + 0.5f * DeltaTime,
	// 			0.0f
	// 		};
	// 	RightPlayerRacketCollisionVisualizer->AddTransform({ 0.0f, +0.5f * DeltaTime, 0.0f, 0.0f });
	// }
	// if (Input->IsKeyDown(Keys::Down))  
	// {
	// 	RightPlayer->AddTransform({0.0f, -0.5f * DeltaTime, 0.0f, 0.0f});
	// 	RightPlayerRacketCollision->Center =
	// 		{
	// 		RightPlayerRacketCollision->Center.x,
	// 		RightPlayerRacketCollision->Center.y - 0.5f * DeltaTime,
	// 		0.0f
	// 		};
	// 	RightPlayerRacketCollisionVisualizer->AddTransform({ 0.0f, -0.5f * DeltaTime, 0.0f, 0.0f });
	// }
	//
	// BallDirection.Normalize();

	// Ball->AddTransform(BallDirection * BallSpeed * DeltaTime);
	// BallCollision->Center = { BallCollision->Center.x + BallDirection.x * BallSpeed * DeltaTime, BallCollision->Center.y + BallDirection.y * BallSpeed * DeltaTime,  0.0f };
	// BallCollisionVisualizer->SetPosition({BallCollision->Center.x, BallCollision->Center.y, BallCollision->Center.z, 0.0f});
	//
	// float Distance2;
	// BallDirection.Normalize();
	//  if(LeftPlayerRacketCollision->Intersects({BallCollision->Center.x, BallCollision->Center.y, BallCollision->Center.z}, BallDirection, Distance2))
	//  {
	//  	if (BallCollision->Center.x + BallDirection.x * BallSpeed * DeltaTime < LeftPlayerRacketCollision->Center.x + LeftPlayerRacketCollision->Extents.x + 0.01f)
	//  	{
	//  		Ball->SetPosition({ BallCollision->Center.x + BallDirection.x * Distance2, BallCollision->Center.y + BallDirection.y * Distance2, 0.0f, 0.0f });
	//  		BallCollisionVisualizer->SetPosition({ BallCollision->Center.x + BallDirection.x * Distance2, BallCollision->Center.y + BallDirection.y * Distance2, 0.0f, 0.0f });
	//  		BallCollision->Center           = { BallCollision->Center.x + BallDirection.x * Distance2, BallCollision->Center.y + BallDirection.y * Distance2, 0.0f };
	//  		// REFLECT
	//  		DirectX::SimpleMath::Vector3 vector0 = {
	//  			BallCollision->Center.x - (LeftPlayerRacketCollision->Center.x - LeftPlayerRacketCollision->Extents.x * 2),
	// 			 BallCollision->Center.y -  LeftPlayerRacketCollision->Center.y,
	// 			 0.0f
	// 		 };
	//  		vector0.Normalize();		
	//  		DirectX::SimpleMath::Vector3 vector = DirectX::SimpleMath::Vector3::Reflect({ BallDirection.x, BallDirection.y, BallDirection.z }, vector0);
	//  		BallDirection.x = vector.x;
	//  		BallDirection.y = vector.y;
	//  		BallDirection.z = vector.z;
	// 		
	//  		BallSpeed += 0.2f;
	//  	}
	//  }
	// BallDirection.Normalize();
	// float distance1;
	// if (RightPlayerRacketCollision->Intersects({ BallCollision->Center.x, BallCollision->Center.y, BallCollision->Center.z }, BallDirection, distance1))
	// {
	// 	if (BallCollision->Center.x + BallDirection.x * BallSpeed * DeltaTime > RightPlayerRacketCollision->Center.x - RightPlayerRacketCollision->Extents.x - 0.01f)
	// 	{
	// 		Ball->SetPosition({ BallCollision->Center.x + BallDirection.x * distance1, BallCollision->Center.y + BallDirection.y * distance1, 0.0f, 0.0f });
	// 		BallCollisionVisualizer->SetPosition({ BallCollision->Center.x + BallDirection.x * distance1, BallCollision->Center.y + BallDirection.y * distance1, 0.0f, 0.0f });
	// 		BallCollision->Center           = { BallCollision->Center.x + BallDirection.x * distance1, BallCollision->Center.y + BallDirection.y * distance1, 0.0f };
	// 		// REFLECT
	// 		DirectX::SimpleMath::Vector3 vector0 = {
	// 			BallCollision->Center.x - (RightPlayerRacketCollision->Center.x + RightPlayerRacketCollision->Extents.x * 2),
	// 			BallCollision->Center.y - RightPlayerRacketCollision->Center.y,
	// 			0.0f
	// 		};
	// 		vector0.Normalize();
	// 		DirectX::SimpleMath::Vector3 vector = DirectX::SimpleMath::Vector3::Reflect({ BallDirection.x, BallDirection.y, BallDirection.z }, vector0);
	// 		BallDirection.x = vector.x;
	// 		BallDirection.y = vector.y;
	// 		BallDirection.z = vector.z;
	// 		
	// 		BallSpeed += 0.2f;
	// 	}
	// }
	// BallDirection.Normalize();
	//
	// float distance3;
	// if (TopWall->Intersects({ BallCollision->Center.x, BallCollision->Center.y, BallCollision->Center.z }, BallDirection, distance3))
	// {
	// 	if (BallCollision->Center.y + BallDirection.y * BallSpeed * DeltaTime > TopWall->Center.y - TopWall->Extents.y - 0.01f)
	// 	{
	// 		Ball->SetPosition({BallCollision->Center.x + BallDirection.x * distance3, BallCollision->Center.y + BallDirection.y * distance3, 0.0f, 0.0f });
	// 		BallCollisionVisualizer->SetPosition({ BallCollision->Center.x + BallDirection.x * distance3, BallCollision->Center.y + BallDirection.y * distance3, 0.0f, 0.0f });
	// 		BallCollision->Center           = { BallCollision->Center.x + BallDirection.x * distance3, BallCollision->Center.y + BallDirection.y * distance3, 0.0f };
	// 		// REFLECT
	// 		DirectX::SimpleMath::Vector3 vector = DirectX::SimpleMath::Vector3::Reflect({ BallDirection.x, BallDirection.y, BallDirection.z }, { 0.0f, -1.0f, 0.0f });
	// 		BallDirection.x = vector.x;
	// 		BallDirection.y = vector.y;
	// 		BallDirection.z = vector.z;
	// 	}
	// }
	// BallDirection.Normalize();
	// float distance4;
	// if (BottomWall->Intersects({ BallCollision->Center.x, BallCollision->Center.y, BallCollision->Center.z }, BallDirection, distance4))
	// {
	// 	if (BallCollision->Center.y + BallDirection.y * BallSpeed * DeltaTime < BottomWall->Center.y + BottomWall->Extents.y + 0.01f)
	// 	{
	// 		Ball->SetPosition({ BallCollision->Center.x + BallDirection.x * distance4, BallCollision->Center.y + BallDirection.y * distance4, 0.0f, 0.0f });
	// 		BallCollisionVisualizer->SetPosition({ BallCollision->Center.x + BallDirection.x * distance4, BallCollision->Center.y + BallDirection.y * distance4, 0.0f, 0.0f });
	// 		BallCollision->Center           = { BallCollision->Center.x + BallDirection.x * distance4, BallCollision->Center.y + BallDirection.y * distance4, 0.0f };
	// 		// REFLECT
	// 		DirectX::SimpleMath::Vector3 vector = DirectX::SimpleMath::Vector3::Reflect( { BallDirection.x, BallDirection.y, BallDirection.z }, { 0.0f, 1.0f, 0.0f });
	// 		BallDirection.x = vector.x;
	// 		BallDirection.y = vector.y;
	// 		BallDirection.z = vector.z;
	// 	}
	// }
	// BallDirection.Normalize();

	Log();
}

void PongGame::Log()
{
	// std::cout << "Ball x: %f " << Ball->GetTransform().x <<
	// 			 "Ball y: %f " << Ball->GetTransform().y <<
	// 			 "Ball z: %f " << Ball->GetTransform().z << std::endl;
	//
	// std::cout << "BallCol x: %f " << BallCollision->Center.x <<
	// 	"BallCol y: %f " << BallCollision->Center.y <<
	// 	"BallCol z: %f " << BallCollision->Center.z << std::endl;
	//
	// std::cout << "BallColV x: %f " << BallCollisionVisualizer->GetTransform().x <<
	// 	"BallColV y: %f " << BallCollisionVisualizer->GetTransform().y <<
	// 	"BallColV z: %f " << BallCollisionVisualizer->GetTransform().z << std::endl;
	//
	// std::cout << "Lracket x: %f " << LeftPlayer->GetTransform().x <<
	// 	"Lracket y: %f " << LeftPlayer->GetTransform().y <<
	// 	"Lracket z: %f " << LeftPlayer->GetTransform().z << std::endl;
	//
	// std::cout << "LracketCol x: %f " << LeftPlayerRacketCollision->Center.x <<
	// 	"LracketCol y: %f " << LeftPlayerRacketCollision->Center.y <<
	// 	"LracketCol z: %f " << LeftPlayerRacketCollision->Center.z << std::endl;
	//
	// std::cout << "LracketColV x: %f " << LeftPlayerRacketCollisionVisualizer->GetTransform().x <<
	// 	"LracketColV y: %f " << LeftPlayerRacketCollisionVisualizer->GetTransform().y <<
	// 	"LracketColV z: %f " << LeftPlayerRacketCollisionVisualizer->GetTransform().z << std::endl;
	//
	// std::cout << "Rracket x: %f " << RightPlayer->GetTransform().x <<
	// 	"Rracket y: %f " << RightPlayer->GetTransform().y <<
	// 	"Rracket z: %f " << RightPlayer->GetTransform().z << std::endl;
	//
	// std::cout << "RracketCol x: %f " << RightPlayerRacketCollision->Center.x <<
	// 	"RracketCol y: %f " << RightPlayerRacketCollision->Center.y <<
	// 	"RracketCol z: %f " << RightPlayerRacketCollision->Center.z << std::endl;
	//
	// std::cout << "RracketColV x: %f " << RightPlayerRacketCollisionVisualizer->GetTransform().x <<
	// 	"RracketColV y: %f " << RightPlayerRacketCollisionVisualizer->GetTransform().y <<
	// 	"RracketColV z: %f " << RightPlayerRacketCollisionVisualizer->GetTransform().z << std::endl;

}

void PongGame::Construct()
{
	// // LEFT WALL
	// // COLLISION
	// LeftWall = new DirectX::BoundingBox();
	// LeftWall->Center  = { -1.5f, 0.0f, 0.0f };
	// LeftWall->Extents = { 0.5f, 1.0f, 0.0f };
	//
	// // RIGHT WALL
	// // COLLISION
	// RightWall = new DirectX::BoundingBox();
	// RightWall->Center  = { 1.5f, 0.0f, 0.0f };
	// RightWall->Extents = { 0.5f, 1.0f, 0.0f };
	
	LeftPlayer = new FGameObject();
	RightPlayer = new FGameObject();
	Ball = new FGameObject();

	//Left 
	const auto LeftPlayerRacketMesh = new FRenderComponent();

	LeftPlayerRacketMesh->SetPoints(
	{
		DirectX::XMFLOAT4(-0.1f,  0.5f, 0.0f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.1f, 0.5f, 0.0f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.1f, -0.5f, 0.0f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-0.1f,  -0.5f, 0.0f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
	});
	LeftPlayerRacketMesh->SetIndicies({ 0, 1, 2, 3, 0 });
	LeftPlayerRacketMesh->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
	
	LeftPlayer->AddTransform({0.8f, 0.0f, 0.0f, 1.0f});
	LeftPlayer->AddComponent(LeftPlayerRacketMesh);
	
	// LeftPlayerRacketCollision = new DirectX::BoundingBox();
	// LeftPlayerRacketCollision->Center = { LeftPlayer->GetTransform().x, LeftPlayer->GetTransform().y ,LeftPlayer->GetTransform().z };
	// LeftPlayerRacketCollision->Extents = {0.11f, 0.51f, 0.0f };
	//
	// LeftPlayerRacketCollisionVisualizer = new FGameObject();
	// LeftPlayerRacketCollisionVisualizer->AddTransform(LeftPlayer->GetTransform());
	// const auto LeftPlayerRacketCollisionVisualizerMesh = new FRenderComponent();
	//
	// LeftPlayerRacketCollisionVisualizerMesh->SetPoints(
	// 	{
	// 		DirectX::XMFLOAT4(- LeftPlayerRacketCollision->Extents.x, LeftPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
	// 		DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
	// 		DirectX::XMFLOAT4(LeftPlayerRacketCollision->Extents.x,  LeftPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
	// 		DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
	// 		DirectX::XMFLOAT4(LeftPlayerRacketCollision->Extents.x,  - LeftPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
	// 		DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
	// 		DirectX::XMFLOAT4(- LeftPlayerRacketCollision->Extents.x,  - LeftPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
	// 		DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
	// 	});
	// LeftPlayerRacketCollisionVisualizerMesh->SetIndicies({ 0, 1, 2, 3, 0 });
	// LeftPlayerRacketCollisionVisualizerMesh->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
	//
	// LeftPlayerRacketCollisionVisualizer->AddComponent(LeftPlayerRacketCollisionVisualizerMesh);

	// //Right
	// const auto RightPlayerRacketMesh = new FRenderComponent();
	//
	// RightPlayerRacketMesh->SetPoints(
	// {
	// 	DirectX::XMFLOAT4(-0.1f,  0.5f, 0.0f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	// 	DirectX::XMFLOAT4(0.1f, 0.5f, 0.0f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	// 	DirectX::XMFLOAT4(0.1f, -0.5f, 0.0f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	// 	DirectX::XMFLOAT4(-0.1f,  -0.5f, 0.0f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
	// });
	// RightPlayerRacketMesh->SetIndicies({ 0, 1, 2, 3, 0 });
	// RightPlayerRacketMesh->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
	//
	// RightPlayer->AddTransform({0.8f, 0.0f, 0.0f, 0.0f});
	// RightPlayer->AddComponent(RightPlayerRacketMesh);
	
	// RightPlayerRacketCollision = new DirectX::BoundingBox();
	// RightPlayerRacketCollision->Center = { RightPlayer->GetTransform().x, RightPlayer->GetTransform().y ,RightPlayer->GetTransform().z };
	// RightPlayerRacketCollision->Extents = {0.11f, 0.51f, 0.0f };
	//
	// RightPlayerRacketCollisionVisualizer = new FGameObject();
	// RightPlayerRacketCollisionVisualizer->AddTransform(RightPlayer->GetTransform());
	// const auto RightPlayerRacketCollisionVisualizerMesh = new FRenderComponent();
	//
	// RightPlayerRacketCollisionVisualizerMesh->SetPoints(
	// 	{
	// 		DirectX::XMFLOAT4(- RightPlayerRacketCollision->Extents.x, RightPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
	// 		DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
	// 		DirectX::XMFLOAT4(RightPlayerRacketCollision->Extents.x,  RightPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
	// 		DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
	// 		DirectX::XMFLOAT4(RightPlayerRacketCollision->Extents.x,  - RightPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
	// 		DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
	// 		DirectX::XMFLOAT4(- RightPlayerRacketCollision->Extents.x,  - RightPlayerRacketCollision->Extents.y, 0.0f, 1.0f),
	// 		DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
	// 	});
	// RightPlayerRacketCollisionVisualizerMesh->SetIndicies({ 0, 1, 2, 3, 0 });
	// RightPlayerRacketCollisionVisualizerMesh->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
	//
	// RightPlayerRacketCollisionVisualizer->AddComponent(RightPlayerRacketCollisionVisualizerMesh);
	
	// // Ball
	//
	// const auto BallMesh = new FRenderComponent();
	// std::vector<DirectX::XMFLOAT4> BallPoints;
	// std::vector<int> BallIndicies;
	// int BallSides = 32;
	// float BallRadius = 0.06f;
	// float Angle = 0.0f;
	// float AngleStep = (2 * M_PI) / BallSides;
	//
	// for (int i = 0; i < BallSides * 2; ++i)
	// {
	// 	BallIndicies.push_back(0);
	// 	
	// 	Angle = AngleStep * i;
	// 	BallPoints.push_back({ cos(Angle) * BallRadius, sin(Angle) * BallRadius, 0.0f, 1.0f });
	// 	BallPoints.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });
	// 	BallIndicies.push_back(i + 1);
	//
	// 	Angle = AngleStep * (i + 1);
	// 	BallPoints.push_back({ cos(Angle) * BallRadius, sin(Angle) * BallRadius, 0.0f, 1.0f });
	// 	BallPoints.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });
	// 	BallIndicies.push_back(i + 2);
	// }
	//
	// BallMesh->SetPoints(std::move(BallPoints));
	// BallMesh->SetIndicies(std::move(BallIndicies));
	// BallMesh->SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//
	// Ball->AddComponent(BallMesh);
	//
	// BallCollision = new DirectX::BoundingSphere();
	// BallCollision->Radius = BallRadius + 0.02f;
	// BallCollision->Center = {0.0f, 0.0f, 0.0f};
	//
	// BallCollisionVisualizer = new FGameObject();
	// BallCollisionVisualizer->AddTransform({ BallCollision->Center.x, BallCollision->Center.y, BallCollision->Center.z, 0.0f});
	//
	// const auto BallCollisionVisualizerMesh = new FRenderComponent();
	//
	// std::vector<DirectX::XMFLOAT4> BallCollisionVisualizerPoints;
	// std::vector<int> BallCollisionVisualizerIndicies;
	//
	// for (int i = 0; i < BallSides * 2; ++i)
	// {
	// 	BallCollisionVisualizerIndicies.push_back(0);
	//
	// 	Angle = AngleStep * i;
	// 	BallCollisionVisualizerPoints.push_back({ cos(Angle) * BallCollision->Radius, sin(Angle) * BallCollision->Radius, 0.0f, 1.0f });
	// 	BallCollisionVisualizerPoints.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });
	// 	BallCollisionVisualizerIndicies.push_back(i + 1);
	//
	// 	Angle = AngleStep * (i + 1);
	// 	BallCollisionVisualizerPoints.push_back({ cos(Angle) * BallCollision->Radius, sin(Angle) * BallCollision->Radius, 0.0f, 1.0f });
	// 	BallCollisionVisualizerPoints.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });
	// 	BallCollisionVisualizerIndicies.push_back(i + 2);
	// }
	//
	// BallCollisionVisualizerMesh->SetPoints(std::move(BallCollisionVisualizerPoints));
	// BallCollisionVisualizerMesh->SetIndicies(std::move(BallCollisionVisualizerIndicies));
	// BallCollisionVisualizerMesh->SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//
	// BallCollisionVisualizer->AddComponent(BallCollisionVisualizerMesh);

// 	// TOP WALL
// 	// COLLISION
// 	TopWall = new DirectX::BoundingBox();
// 	TopWall->Center  = { 0.0f, 1.5f, 0.0f };
// 	TopWall->Extents = { 1.0f + 0.06f, 0.5f +  0.06f, 0.0f };
// 	FRenderComponent* RCTopWall = new FRenderComponent();
// 	RCTopWall->SetPoints(
// 	{
// 		DirectX::XMFLOAT4(TopWall->Center.x - TopWall->Extents.x,   TopWall->Center.y + TopWall->Extents.y,  0.0f,  1.0f),  DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
// 		DirectX::XMFLOAT4(TopWall->Center.x + TopWall->Extents.x,   TopWall->Center.y + TopWall->Extents.y,  0.0f,  1.0f),  DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
// 		DirectX::XMFLOAT4(TopWall->Center.x - TopWall->Extents.x,   TopWall->Center.y - TopWall->Extents.y,  0.0f,  1.0f),  DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
// 		DirectX::XMFLOAT4(TopWall->Center.x + TopWall->Extents.x,   TopWall->Center.y - TopWall->Extents.y,  0.0f,  1.0f),  DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f)
// 	});
// 	//RCTopWall->indeces = { 0, 1, 1, 3, 3, 2, 2, 0, 0, 3, 1, 2 };
// 	RCTopWall->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
//
// // BOTTOM WALL
// 	// COLLISION
// 	BottomWall = new DirectX::BoundingBox();
// 	BottomWall->Center = { 0.0f, -1.5f, 0.0f };
// 	BottomWall->Extents = { 1.0f +  0.06f, 0.5f +  0.06f, 0.0f };
// 	FRenderComponent* RCBottomWall = new FRenderComponent();
// 	RCBottomWall->SetPoints(
// 	{
// 		DirectX::XMFLOAT4(BottomWall->Center.x - BottomWall->Extents.x,   BottomWall->Center.y + BottomWall->Extents.y,  0.0f,  1.0f),  DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
// 		DirectX::XMFLOAT4(BottomWall->Center.x + BottomWall->Extents.x,   BottomWall->Center.y + BottomWall->Extents.y,  0.0f,  1.0f),  DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
// 		DirectX::XMFLOAT4(BottomWall->Center.x - BottomWall->Extents.x,   BottomWall->Center.y - BottomWall->Extents.y,  0.0f,  1.0f),  DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f),
// 		DirectX::XMFLOAT4(BottomWall->Center.x + BottomWall->Extents.x,   BottomWall->Center.y - BottomWall->Extents.y,  0.0f,  1.0f),  DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f)
// 	});
// 	//RCBottomWall->indeces = { 0, 1, 1, 3, 3, 2, 2, 0, 0, 3, 1, 2 };
// 	RCBottomWall->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	
	AddGameObject(LeftPlayer);
	//AddGameObject(LeftPlayerRacketCollisionVisualizer);

	//AddGameObject(RightPlayer);
	//AddGameObject(RightPlayerRacketCollisionVisualizer);
	
	//AddGameObject(Ball);
	//AddGameObject(BallCollisionVisualizer);
}

void PongGame::Run()
{
	Construct();
	FGame::Run();
}
