//#if _DEBUG
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
//#endif

#include "Game.h"

int main(int argc, char *argv []) {
	B00289996::Game game;

	while(game.IsRunning()) {
		game.Update();
		game.Render();
	}
	return 0;
}
