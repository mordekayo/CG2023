#include "PongGame.h"

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
