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
#include <glm/gtx/vector_angle.hpp>

static const int DISPLAY_WIDTH = 1280;
static const int DISPLAY_HEIGHT = 720;
std::string applicationName = "Body Animation";

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

	// BVH DEFINITIONS -----------------------------------------------------------------------------------------------------------------------------------------------

	//Animation * animation = loader.loadAnimation("BVH Files/Female1_bvh/Female1_B25_CrouchToWalk.bvh");
	//Animation * animation = loader.loadAnimation("BVH Files/Female1_bvh/Female1_C19_RunToHopToWalk.bvh");
	Animation * animation = loader.loadAnimation("BVH Files/Female1_bvh/Female1_A13_Skipping.bvh");


	//Animation * animation = loader.loadAnimation("BVH Files/01_01.bvh");
	//Animation * animation = loader.loadAnimation("BVH Files/Female1_A07_Crouch.bvh");
	//Animation * animation = loader.loadAnimation("BVH Files/testLeg.bvh");
	//Animation * animation = loader.loadAnimation("BVH Files/testBothLegs.bvh");
	//Animation * animation = loader.loadAnimation("BVH Files/basic.bvh");
	//Animation * animation = loader.loadAnimation("BVH Files/basic2.bvh");
	//Animation * animation = loader.loadAnimation("BVH Files/test.bvh");

	// SHADER DEFINITIONS -----------------------------------------------------------------------------------------------------------------------------------------------

	Shader wireframeShader("./Shaders/WireframeShader");
	Shader boneShader("./Shaders/LineBoneShader", "./Shaders/WireframeShader");
	Shader cylindricalModelShader("./Shaders/CylindricalModelShader");

	// MESH / MODEL DEFINITIONS -----------------------------------------------------------------------------------------------------------------------------------------------

	//animation->skeleton->createWireframeModelMesh(&wireframeShader);					// we create a mesh from the loaded vertices; this has to be done after the GLEW init (which is done in Display constructor)
	animation->skeleton->createWireframeModelMesh(&wireframeShader, &boneShader);
	animation->skeleton->createCylindricalMesh(&cylindricalModelShader);				// create cylindrical mesh

	// OLD MESH / MODEL DEFINITIONS -----------------------------------------------------------------------------------------------------------------------------------------------

	// monkey / cube
	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
	//Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
	//Mesh monkey("./res/monkey3.obj");
	Shader shader("./Shaders/basicShader");
	Texture texture("./res/bricks.jpg");
	Transform transform;

	// CAMERA DEFINITIONS -----------------------------------------------------------------------------------------------------------------------------------------------

	// camera in x axis looking to the origin
	//Camera camera(glm::vec3(400.0f, 150.0f, 0.0f), glm::radians(180.0f), glm::radians(0.0f), 70.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 1000.0f);

	// camera in -z axis looking to the origin
	Camera camera(glm::vec3(0.0f, 150.0f, -400.0f), glm::radians(270.0f), glm::radians(0.0f), 70.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 1000.0f);

	// FRAME DEFINITIONS ------------------------------------------------------------------------------------------------------------------------------------------------

	unsigned int frameCount = animation->animationInfo->frameCount;
	unsigned int frame = 0;

	// ------------------------------------------------------------------------------------------------------------------------------------------------------------------
  
	SDL_Event e;
	bool isRunning = true;
	float counter = 0.0f;
	bool play = false;

	// mouse motion related variables
	bool leftMouseButtonPressed = false;
	bool rightMouseButtonPressed = false;
	bool mouseWheelPressed = false;


	while (isRunning)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				isRunning = false;
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_RIGHT:
					frame++;
					break;
				case SDLK_LEFT:
					frame--;
					break;
				case SDLK_SPACE:
					play = !play;
					break;
				default:
					break;
				}
			} else if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (e.button.button = SDL_BUTTON_LEFT) {
					leftMouseButtonPressed = true;
				} else if (e.button.button = SDL_BUTTON_RIGHT) {
					rightMouseButtonPressed = true;
				} else if (e.button.button = SDL_BUTTON_MIDDLE) {
					mouseWheelPressed = true;
				}
				
			} else if (e.type == SDL_MOUSEBUTTONUP) {
				// vyhodnotit pohyb

				leftMouseButtonPressed = false;
				rightMouseButtonPressed = false;
				mouseWheelPressed = false;
			} else if (e.type == SDL_MOUSEMOTION) {
				if (leftMouseButtonPressed) {
					
				} else if (rightMouseButtonPressed) {
					
				} else if (mouseWheelPressed) {
					
				}
			}
		}

		display.Clear(0.0f, 0.0f, 0.0f, 1.0f);

		if (frame == frameCount) {
			frame = 0;
		}
		else if (frame == -1) {
			frame = frameCount - 1;
		}

		//animation->skeleton->drawWireframeModel(&wireframeShader, &boneShader, frame, camera);		// draw wireframeModel - points and lines
		animation->skeleton->drawCylindricalModel(&cylindricalModelShader, frame, camera);			// draw cylindrical model - bones as cylinders

		display.SwapBuffers();
		if (play) {
			frame++;
			SDL_Delay(20);
		}
		counter += 0.033f;
	}
	delete animation;
	return 0;
}