#include <SDL2/SDL.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include "BVHLoader.h"
#include "display.h"
#include "slider.h"
#include "mesh.h"
#include "shader.h"
#include "camera.h"
#include "Timer.h"

int main(int argc, char * argv[]) {

	// initialize OpenGL, SDL and open the SDL window
	static const int DISPLAY_WIDTH = 1280;
	static const int DISPLAY_HEIGHT = 720;
	std::string applicationName = "Body Animation";
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, applicationName.c_str());

	// path to a BVH file with animation
	std::string bvhPath;

	// as a path to a BVH file use either a command line argument if there is, or a default path
	if (argc == 2) {
		bvhPath = argv[1];
	} else {
		bvhPath = "BVH Files/Female1_bvh/Female1_C19_RunToHopToWalk.bvh";
		//bvhPath = "BVH Files/Female1_bvh/Female1_B25_CrouchToWalk.bvh";
		//bvhPath = "BVH Files/Female1_A07_Crouch.bvh";
		//bvhPath = "BVH Files/testLeg.bvh";
		//bvhPath = "BVH Files/basic.bvh";
		//bvhPath = "BVH Files/basic2.bvh";
		//bvhPath = "BVH Files/test.bvh";
	}
	
	// Load the animation from a BVH file
	BVHLoader loader;
	Animation * animation = loader.loadAnimation(bvhPath);

	/*std::ifstream file("config.txt");
	std::string input;
	if (file.is_open()) {
		file >> input;
	}
	file.close();
	Animation * animation = loader.loadAnimation(input);*/

	// create shaders
	Shader wireframeShader("./Shaders/WireframeShader");
	Shader boneShader("./Shaders/LineBoneShader", "./Shaders/WireframeShader");
	Shader cylindricalModelShader("./Shaders/CylindricalModelShader");

	// create meshes and OpenGL buffers
	//animation->skeleton->createWireframeModelMesh(&wireframeShader);
	animation->skeleton->createWireframeModelMesh(&wireframeShader, &boneShader);		// create a mesh used for the line model
	animation->skeleton->createCylindricalMesh(&cylindricalModelShader);				// create cylindrical mesh

	// create a camera
	Camera camera(glm::vec3(0.0f, 150.0f, -400.0f), glm::radians(270.0f), glm::radians(0.0f), 70.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 5000.0f);

	// frame and frame rate related variables; load the frame duration value from the animation object
	float frameDurationMS = animation->animationInfo->frameDurationMS;
	unsigned int frameCount = animation->animationInfo->frameCount;
	unsigned int frame = 0;

	// create a frame slider
	Slider slider;

	// create a timer
	Timer timer;
	double elapsedMS;

	// SDL and run related variables
	SDL_Event e;
	bool appRunning = true;						// whether the whole app is running or is to be closed
	bool animationRunning = false;				// whether the animation is running in a loop or is stopped at a frame
	int modelType = 0;							// the model that is being drawn

	// mouse motion related variables
	bool leftMouseButtonPressed = false;		// buttons
	bool rightMouseButtonPressed = false;
	bool mouseWheelPressed = false;
	bool leftMouseButtonSlider = false;
	float horizontalAngleDelta = 0.0f;			// angles to rotate the camera
	float verticalAngleDelta = 0.0f; 
	float forwardBackwardMovementDelta = 0.0f;	// variables to move the camera
	float horizontalTranslationDelta = 0.0f;
	float verticalTranslationDelta = 0.0f;
	int mouseXPos;								// auxiliary variables
	int mouseYPos;
	int newMouseXPos;
	int newMouseYPos;
	int sliderClickValue = -1;
	
	// the SDL run loop
	while (appRunning) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				appRunning = false;				// exit the app
			} else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_RIGHT:
					frame++;					// move by a one frame
					break;
				case SDLK_LEFT:
					frame--;
					break;
				case SDLK_SPACE:
					animationRunning = !animationRunning; // run the animation or stop it
					break;
				case SDLK_ESCAPE:
					appRunning = false;			// exit the app
					break;
				case SDLK_m:
					if (modelType == 0) {
						modelType = 1;			// change the type of the model to be drawn
					} else {
						modelType = 0;
					}
				default:
					break;
				}
			} else if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					sliderClickValue = slider.clickSlider(e.motion.x, e.motion.y);
					if (sliderClickValue >= 0) {
						frame = sliderClickValue;
						leftMouseButtonSlider = true;
						animationRunning = false;
					}
					else {
						leftMouseButtonPressed = true;
					}
				} else if (e.button.button == SDL_BUTTON_RIGHT) {
					rightMouseButtonPressed = true;
				} else if (e.button.button == SDL_BUTTON_MIDDLE) {
					mouseWheelPressed = true;
				}
				mouseXPos = e.motion.x;
				mouseYPos = e.motion.y;
			} else if (e.type == SDL_MOUSEBUTTONUP) {
				leftMouseButtonPressed = false;
				rightMouseButtonPressed = false;
				mouseWheelPressed = false;
				leftMouseButtonSlider = false;
			} else if (e.type == SDL_MOUSEMOTION) {
				
				if (leftMouseButtonPressed) {
					newMouseXPos = e.motion.x;				// rotate the camera
					newMouseYPos = e.motion.y;
					horizontalAngleDelta = 0.005f * (newMouseXPos - mouseXPos);
					verticalAngleDelta = 0.005f * (newMouseYPos - mouseYPos);
					camera.updateCameraAngles(horizontalAngleDelta, verticalAngleDelta);
					mouseXPos = newMouseXPos;
					mouseYPos = newMouseYPos;
				} else if (rightMouseButtonPressed) {
					newMouseXPos = e.motion.x;				// move the camera forward of backward (in the camera direction)
					newMouseYPos = e.motion.y;
					forwardBackwardMovementDelta = newMouseXPos - mouseXPos;
					camera.moveCameraForwardBackward(forwardBackwardMovementDelta);
					mouseXPos = newMouseXPos;
					mouseYPos = newMouseYPos;
				} else if (mouseWheelPressed) {
					newMouseXPos = e.motion.x;				// translate the camera left, right, up or down
					newMouseYPos = e.motion.y;
					horizontalTranslationDelta = newMouseXPos - mouseXPos;
					verticalTranslationDelta = newMouseYPos - mouseYPos;
					camera.moveCameraRightLeftUpDown(horizontalTranslationDelta, verticalTranslationDelta);
					mouseXPos = newMouseXPos;
					mouseYPos = newMouseYPos;
				}
				else if (leftMouseButtonSlider) {
					sliderClickValue = slider.dragSlider(e.motion.x);
					frame = sliderClickValue;
					sliderClickValue = -1;
				}
			}
		}

		timer.getTimeElapsed();					// just set the actual time value to measure later

		display.clear(0.0f, 0.0f, 0.0f, 1.0f);

		// adjust the frame to run the animation in a loop
		if (frame == frameCount) {
			frame = 0;
		} else if (frame == -1) {
			frame = frameCount - 1;
		}

		// draw the seleted model
		if (modelType == 0) {
			animation->skeleton->drawWireframeModel(&wireframeShader, &boneShader, frame, camera);		// draw wireframeModel - points and lines
		} else {
			animation->skeleton->drawCylindricalModel(&cylindricalModelShader, frame, camera);			// draw cylindrical model - bones as cylinders
		}

		slider.drawSlider(DISPLAY_WIDTH, DISPLAY_HEIGHT, frame, frameCount);

		display.swapBuffers();

		// delay the SDL window to achieve the desired framerate
		if (animationRunning) {
			frame++;

			/*elapsedMS = timer.getTimeElapsed();
			std::cout << elapsedMS << std::endl;
			SDL_Delay(frameDurationMS - elapsedMS);*/
 			SDL_Delay(frameDurationMS);

		}
	}
	delete animation;
	return 0;
}