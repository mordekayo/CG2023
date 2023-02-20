#include "Game.h"
#include "TriangleComponent.h"

int main()
{
	Game* game = Game::CreateInstance();
	game->components.push_back(new TriangleComponent());
	game->Run();
	delete game;
}