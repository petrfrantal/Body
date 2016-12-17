#include <SDL2/SDL.h>
//#include <GL/glew.h>
//#include <gl\GLU.h>
#undef main
#include <iostream>
#include <stdio.h>
#include <string>
#include "BVHLoader.h"
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"

static const int DISPLAY_WIDTH = 800;
static const int DISPLAY_HEIGHT = 600;

//SDL_Window * window;
//SDL_GLContext context;
std::string applicationName = "Body Animation";

/*bool initOpenGL(void) {
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
		//std::cout << "Error while initializing OpenGL. Error string: " << gluErrorString(error) << std::endl;		// gluErrorString does not work so far
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
	window = SDL_CreateWindow(applicationName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DISPLAY_WIDTH, DISPLAY_HEIGHT, SDL_WINDOW_OPENGL);
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
}*/


int main(int argc, char* args[])
{
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, applicationName.c_str());

	Vertex vertices[] =
	{
		Vertex(glm::vec3(-0, -0, -0), glm::vec2(1, 0), glm::vec3(0, 0, -1)),
		Vertex(glm::vec3(-0, 0, -0), glm::vec2(0, 0), glm::vec3(0, 0, -1)),
		Vertex(glm::vec3(0, 0, -0), glm::vec2(0, 1), glm::vec3(0, 0, -1)),
		Vertex(glm::vec3(0, -0, -0), glm::vec2(1, 1), glm::vec3(0, 0, -1)),

		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0)),

		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0)),

		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0)),
	};

	unsigned int indices[] = { 0, 1, 2,
		0, 2, 3,

		6, 5, 4,
		7, 6, 4,

		10, 9, 8,
		11, 10, 8,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		22, 21, 20,
		23, 22, 20
	};


	// BVH animation

	BVHLoader loader;
	Animation * animation = loader.loadAnimation("BVH Files/01_01.bvh");
	//Animation * animation = loader.loadAnimation("BVH Files/basic.bvh");
	// create a wireframe shader
	Shader wireframeShader("./Shaders/WireframeShader", true);
	// we create a mesh from the loaded vertices; this has to be done after the GLEW init (which is done in Display constructor)
	animation->skeleton->createWireframeModelMesh(&wireframeShader);
	unsigned int frameCount = animation->animationInfo->frameCount;
	unsigned int frame = 0;


	// monkey / cube
	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
	//Mesh monkey("./res/monkey3.obj");
	Shader shader("./Shaders/basicShader", false);
	Texture texture("./res/bricks.jpg");
	Transform transform;



	//Camera camera(glm::vec3(0.0f, 0.0f, -5.0f), 70.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 100.0f);	// formerly for cube/monkey
	//Camera camera(glm::vec3(0.0f, 0.0f, -10.0f), 70.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 100.0f);		// for bvh basic 
	Camera camera(glm::vec3(0.0f, 0.0f, -300.0f), 70.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 1000.0f);		// for bvh 01_01
	SDL_Event e;
	bool isRunning = true;
	float counter = 0.0f;

	// for debugging
	std::vector<glm::vec3> cubePositions = mesh.positions;

	while (isRunning)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				isRunning = false;
		}

		display.Clear(0.0f, 0.0f, 0.0f, 1.0f);

		float sinCounter = sinf(counter);
		float absSinCounter = abs(sinCounter);

		//transform.GetPos()->x = sinCounter;
		transform.GetRot()->y = counter * 0.2f;
		//transform.GetRot()->z = counter * 1;
		//transform.GetRot()->x = counter * 1;
		//transform.GetScale()->x = absSinCounter;
		//transform.GetScale()->y = absSinCounter;

		// draw cube or monkey
		/*
		shader.Bind();
		texture.Bind();
		shader.Update(transform, camera);
		//monkey.draw();
		mesh.draw();*/
		
		/*
		glm::mat4 cubeModelMatrix = transform.GetModel();
		std::cout << "CUBE" << std::endl;
		for (int i = 0; i < cubePositions.size(); i++) {
			glm::vec4 cubePositionModelCoords = cubeModelMatrix * glm::vec4(cubePositions[i], 1.0f);
			std::cout << cubePositionModelCoords[0] << " " << cubePositionModelCoords[1] << " " << cubePositionModelCoords[2] << " " << cubePositionModelCoords[3] << std::endl;
		}
		std::cout << "--------------------------------------------------------------------------" << std::endl;*/

		// draw wireframeModel
		// debug
		//glm::mat4 cubeModelMatrix = transform.GetModel();
		//animation->skeleton->root->transformPerFrame[0] = cubeModelMatrix;
		animation->skeleton->drawWireframeModel(&wireframeShader, frame, camera);
		// update frame for animation of the wireframe model
		frame++;
		if (frame == frameCount) {
			frame = 0;
		}

		display.SwapBuffers();
		//SDL_Delay(animation->animationInfo->frameDuration);		// 1; but with animation must be according to framerate
		SDL_Delay(250);
		counter += 0.01f;
	}
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

	
	delete animation;
	return 0;
}