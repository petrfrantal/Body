#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>

/**
*	Represents the SDL window of the application.
*/
class Display {
private:
	SDL_Window * m_window;			// SDL window object
	SDL_GLContext m_glContext;		// OpenGL context for the use of the SDL window
public:

	/**
	*	Constructs the display object with the given width, height and title.
	*/
	Display(int width, int height, const std::string & title);

	/**
	*	Destroys the display object.
	*/
	~Display();

	/**
	*	Clears the color buffer with the given color.
	*/
	void clear(float r, float g, float b, float a);

	/**
	*	Swaps the buffers (double buffering is being used).
	*/
	void swapBuffers();

	/**
	*	Prints the counts of the vertex and frgment shader uniforms. Auxiliary method.
	*/
	void printShaderUniformsCounts(void);
};
