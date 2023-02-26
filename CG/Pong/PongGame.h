#pragma once
#include <DirectXMath.h>

#include "../GameFramework/Game.h"

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
	FGameObject* RightPlayer = nullptr;
	FGameObject* Ball = nullptr;

	DirectX::FXMVECTOR BallDirection = {1.0f, 0.5f, 0.0f, 0.0f};
	float BallSpeed = 0.5f;

};
