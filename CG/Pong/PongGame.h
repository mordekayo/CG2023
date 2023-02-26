#pragma once
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

};
