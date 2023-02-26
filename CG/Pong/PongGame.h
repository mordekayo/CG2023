#pragma once
#include "..\GameFramework\Game.h"

class PongGame : public FGame
{

public:

	FGameObject* LeftPlayer;
	FGameObject* RightPlayer;
	FGameObject* Ball;

	static PongGame* Instance();

protected:

	PongGame();
};