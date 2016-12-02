#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <gl\GLU.h>
#undef main
#include <iostream>
#include <stdio.h>
#include <string>
#include "BVHLoader.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400

SDL_Window * window;
SDL_GLContext context;
std::string applicationName = "Body Animation";

bool initOpenGL(void) {
	// set OpenGL version to 3.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// init modelview and projection matrices
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		//std::cout << "Error while initializing OpenGL. Error string: " /*<< gluErrorString(error) <<*/ std::endl;		// gluErrorString does not work so far
		return false;
	} else {
		return true;
	}
}

bool init(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	window = SDL_CreateWindow(applicationName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	if (!window) {
		std::cout << "Error while creating and SDL OpenGL window.";
		return false;
	}
	context = SDL_GL_CreateContext(window);
	if (!initOpenGL()) {
		return false;
	}
	SDL_GL_SetSwapInterval(1);
	return true;
}


int main(int argc, char* args[])
{
	/*if (!init()) {
		return 1;
	}*/

	/*
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError()); 
	}
	else
	{
		//Create window
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 400, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);

			//Fill the surface white
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

			//Update the surface
			SDL_UpdateWindowSurface(window);

			//Wait two seconds
			SDL_Delay(2000);
		}
	}
	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();
	*/

	//SDL_Delay(1000);

	// BVH parser test
	//BVHLoader loader;
	//Animation * animation = loader.loadAnimation("BVH Files/01_01.bvh");

	return 0;
}