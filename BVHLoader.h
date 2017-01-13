#pragma once

/**
*	BVH file loader data structure.
*	It's purpose is to load the animation data from a file.
*/

#include <fstream>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <math.h>
#include "Animation.h"
#include "Skeleton.h"

/**
*	The class of the BVH loader.
*/
struct BVHLoader {
	
	/**
	*	Main method for creating and loading the animation.
	*/
	Animation * loadAnimation(const std::string & fileName);

	/**
	*	Loads the hierarchy part of a BVH file.
	*/
	void loadHierarchy(std::istream & file, Animation * animation);

	/**
	*	Loads the motion part of a BVH file.
	*/
	void loadMotion(std::istream & file, Animation * animation);

	/**
	*	Loads a joint's information, most importatly it's transformation matrices.
	*/
	Joint * loadJoint(std::istream & file, Animation * animation, Joint * parent = NULL);
};

/**
*	Enum for an easier indexing of rotations in joints.
*/
enum rotations {
	xRotation,
	yRotation,
	zRotation
};

/**
*	Enum for an easier indexing of offsets in joints.
*/
enum offset {
	xOffset,
	yOffset,
	zOffset
};
