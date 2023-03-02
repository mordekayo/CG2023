#pragma once

#include "../GameFramework/Game.h"
#include "../GameFramework/Utils/SimpleMath.h"
#include "DirectXCollision.h"

class PongGame : public FGame
{

public:

	static PongGame* Instance();

	void Construct();
	virtual void Run() override;
	
protected:

	PongGame();

	void Update(float DeltaTime) override;
	
private:
	
	FGameObject* LeftPlayer = nullptr;
	DirectX::BoundingBox* LeftPlayerRacketCollision = nullptr;
	FGameObject* LeftPlayerRacketCollisionVisualizer = nullptr;
	
	FGameObject* RightPlayer = nullptr;
	DirectX::BoundingBox* RightPlayerRacketCollision = nullptr;
	FGameObject* RightPlayerRacketCollisionVisualizer = nullptr;
	
	FGameObject* Ball = nullptr;
	DirectX::BoundingSphere* BallCollision = nullptr;
	FGameObject* BallCollisionVisualizer = nullptr;

	DirectX::BoundingBox* TopWall = nullptr;
	DirectX::BoundingBox* BottomWall = nullptr;

	DirectX::BoundingBox* LeftWall = nullptr;
	DirectX::BoundingBox* RightWall = nullptr;

	int Player1Points = 0;
	int Player2Points = 0;
	
	DirectX::SimpleMath::Vector4 BallDirection = {-0.3f, 0.0f, 0.0f, 0.0f};
	float BallSpeed = 0.5f;
	float RacketSpeed = 1.0f;

};
