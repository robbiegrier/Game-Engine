#include "Game.h"
#include "PCSTree.h"
#include "PCSNode.h"
#include "PCSTreeForwardIterator.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	static_cast<void>(prevInstance);
	static_cast<void>(cmdLine);

	int output = Azul::Game::Launch(hInstance, cmdShow);
	Azul::Game::Teardown();
	return output;
}