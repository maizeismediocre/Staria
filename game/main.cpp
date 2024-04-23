#include "stdafx.h"
#include "MyGame.h"

CMyGame game;
CGameApp app;

int main(int argc, char* argv[])
{
	app.OpenConsole();
	app.OpenWindow(800, 600, "Starian");
	//app.OpenFullScreen(1080, 1920, 32);
	app.SetClearColor(CColor::Black());
	app.Run(&game);
	return(0);
}
