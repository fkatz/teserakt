/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include "Application.h"



int main(int argc, char* args[])
{

	Application* application = new Application();
	application->run();
	SDL_Quit();

	return 0;
}