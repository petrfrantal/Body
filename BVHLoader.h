#pragma once

/**
*	BVH file loader data structure.
*	It's purpose is to load motion capture data from a given file.
*/

#include <fstream>
#include <string>
#include <algorithm>	// std::remove
#include <iostream>
#include "Animation.h"
#include "Skeleton.h"

struct BVHLoader {
	Animation * loadAnimation(const std::string & fileName);
	void loadHierarchy(std::istream & file, Animation * animation);
	void loadMotion(std::istream & file, Animation * animation);
	Joint * loadJoint(std::istream & file, Animation * animation, Joint * parent = NULL);	// maybe to split it in two methods and not use the implicit parameter NULL
};

enum rotations {
	xRotation,
	yRotation,
	zRotation
};

enum offset {
	xOffset,
	yOffset,
	zOffset
};
