#include <SDL.h>
#include <stdio.h>

#include "CellularAutomata.h"
#include <iostream>
#include <stdlib.h>


bool init();

bool loadMedia();

void close();

bool createRendere();

/*GLOBAL*/
SDL_Window* gWindow = NULL;

SDL_Surface* gScreenSurface = NULL;

SDL_Surface* gHelloWorld = NULL;

SDL_Renderer* gRenderer = NULL;

int screen_width, screen_height;


/**
 * Initializes SDL and creates the window
 * 
 * \return true on SDL initialization success, false if initialization failed 
 */
bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	else {
		SDL_DisplayMode dm;
		//get display details
		if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
			printf("Error getting desktop display mode: %s\n", SDL_GetError());
			return false;
		}
		screen_width = dm.w;
		screen_height = dm.h;
		std::cout << "Screen width: " << screen_width << std::endl;
		std::cout << "Screen height: " << screen_height << std::endl;

		//create window
		gWindow = SDL_CreateWindow(
			"Cellular Automata",
			SDL_WINDOWPOS_UNDEFINED, //window x position on creation
			SDL_WINDOWPOS_UNDEFINED, //window y position on creation
			screen_width, //window width
			screen_height, //window height
			SDL_WINDOW_SHOWN	//window shown upon creation
		);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
		else {
			//get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}
	return true;
}

/**
 * Loads media
 * 
 * \return true on media loaded successfully, false otherwise
 */
bool loadMedia() {
	bool success = true;	//success flag

	gHelloWorld = SDL_LoadBMP("image.bmp");
	if (gHelloWorld == NULL)
	{
		printf("Unable to load image %s! SDL_Error: %s\n", "image.bmp", SDL_GetError());
		success = false;
	}
	return success;
}

/**
 * Deallocates any memory initialized by SDL, closes up shop
 * 
 */
void close() {
	SDL_FreeSurface( gHelloWorld );
	gHelloWorld = NULL;

	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

bool createRenderer() {
	bool success = true;
	gRenderer = SDL_CreateRenderer(gWindow, -1, 0);
	if (gRenderer == NULL)
	{
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
	}
	return success;
}


int main(int argc, char* args[]) {
	
	if (!init())
	{
		printf("Failed to initialize\n");
	} 
	else {
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		if (!createRenderer())
		{

		}
	}

	//create a new Cellular Automata
	CellularAutomata* ca = new CellularAutomata(

		5,						//iterations
		800,					//initial # of points
		9,						//minimum neighbors
		screen_width/20,		//cells wide
		screen_height/20,		//cells high
		20
	
	);


	//main loop flag
	bool quit = false;

	SDL_Event e;
	/*GAME LOOP - capped FPS*/
	const int FPS = 1000;	//frames per second
	const int frameDelay = 1000 / FPS;	//time per frame
	Uint32 frameStart;	//start of loop iteration
	Uint32 timeSinceLastUpdate = 0, lastUpdate = 0;
	int frameTime;	//time it took to process event handling, updating, and rendering

	while (!quit)
	{
		frameStart = SDL_GetTicks();	//get the time at the start of the frame in ms

		//--------------------------------------------------------//
		//event handling
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
				case SDLK_r:
					ca->reset();
					break;
				case SDLK_s:
					ca->smooth();
					break;
				case SDLK_ESCAPE:
					quit = true;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}

		timeSinceLastUpdate = SDL_GetTicks() - lastUpdate;	//get the time since the last update
		//ca->update();
		lastUpdate = SDL_GetTicks();

		//rendering
		SDL_RenderClear(gRenderer);
		ca->render(gRenderer);
		SDL_RenderPresent(gRenderer);

		//refreshing
		//--------------------------------------------------------//

		frameTime = SDL_GetTicks() - frameStart;	//get the frame time

		if (frameDelay > frameTime)	//delay the game if the frame was processed quickly, so that we cap the frame rate
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}




	close();
	return 0;
	
	return 0;
}