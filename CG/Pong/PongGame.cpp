#include "PongGame.h"

PongGame::PongGame()
{

}

PongGame* PongGame::Instance()
{
	return static_cast<PongGame*>(FGame::Instance());
}
