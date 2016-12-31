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
#include "Cylinder.h"		// vertices and indices of the cylinder

static const int DISPLAY_WIDTH = 800;
static const int DISPLAY_HEIGHT = 600;
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

	Animation * animation = loader.loadAnimation("BVH Files/01_01.bvh");
	//Animation * animation = loader.loadAnimation("BVH Files/basic.bvh");
	//Animation * animation = loader.loadAnimation("BVH Files/basic2.bvh");
	//Animation * animation = loader.loadAnimation("BVH Files/test.bvh");

	// SHADER DEFINITIONS -----------------------------------------------------------------------------------------------------------------------------------------------

	Shader wireframeShader("./Shaders/WireframeShader");
	Shader boneShader("./Shaders/LineBoneShader", "./Shaders/WireframeShader");

	// MESH / MODEL DEFINITIONS -----------------------------------------------------------------------------------------------------------------------------------------------

	//animation->skeleton->createWireframeModelMesh(&wireframeShader);					// we create a mesh from the loaded vertices; this has to be done after the GLEW init (which is done in Display constructor)
	animation->skeleton->createWireframeModelMesh(&wireframeShader, &boneShader);
	animation->skeleton->createCylindricalMesh(&cylinderVertices[0], cylinderNVertices, &cylinderTriangles[0], cylinderNTriangles, &wireframeShader);	// create cylindrical mesh

	// OLD MESH / MODEL DEFINITIONS -----------------------------------------------------------------------------------------------------------------------------------------------

	// monkey / cube
	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
	//Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
	//Mesh monkey("./res/monkey3.obj");
	Shader shader("./Shaders/basicShader");
	Texture texture("./res/bricks.jpg");
	Transform transform;

	// OLD CAMERA DEFINITIONS - STILL FUNCTIONAL -----------------------------------------------------------------------------------------------------------------------------------------------

	//Camera camera(glm::vec3(0.0f, 0.0f, -5.0f), 70.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 100.0f);	// formerly for cube/monkey
	//Camera camera(glm::vec3(0.0f, 0.0f, -10.0f), 70.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 100.0f);		// for bvh basic 
	//Camera camera(glm::vec3(-300.0f, 0.0f, 0.0f), 70.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 1000.0f);		// for bvh 01_01
	//Camera camera(glm::vec3(0.0f, 0.0f, -30.0f), 70.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 1000.0f);		// for bvh 01_01

	// CAMERA DEFINITIONS -----------------------------------------------------------------------------------------------------------------------------------------------

	// camera in -x axis looking to the origin
	Camera camera(glm::vec3(-300.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 70.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 1000.0f);
	//Camera camera(glm::vec3(-50.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 70.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 1000.0f);

	// camera in x axis looking to the origin
	//Camera camera(glm::vec3(300.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), 70.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 1000.0f);

	// camera in y axis looking to the origin - from above - doesn't work
	//Camera camera(glm::vec3(0.0f, 500.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), 70.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 1000.0f);

	// camera in z axis looking to the origin (good for BVH "basic")
	//Camera camera(glm::vec3(0.0f, 0.0f, 50.0f), glm::vec3(0.0f, 0.0f, -1.0f), 70.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 1000.0f);

	// ------------------------------------------------------------------------------------------------------------------------------------------------------------------


	SDL_Event e;
	bool isRunning = true;
	float counter = 0.0f;

	unsigned int frameCount = animation->animationInfo->frameCount;
	unsigned int frame = 0;

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

		//transform.getPos()->x = sinCounter;
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
		//mesh.draw();*/
		
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
		//animation->skeleton->drawOnlyJoints(&wireframeShader, frame, camera);
		animation->skeleton->drawWireframeModel(&wireframeShader, &boneShader, frame, camera);
		animation->skeleton->drawCylindricalModel(&wireframeShader, frame, camera);
		// update frame for animation of the wireframe model
		frame++;
		if (frame == frameCount) {
			frame = 0;
		}

		display.SwapBuffers();
		//SDL_Delay(animation->animationInfo->frameDuration);		// 1; but with animation must be according to framerate
		SDL_Delay(20);
		counter += 0.01f;
	}
	
	delete animation;
	return 0;
}