#pragma once

/**
*	Data structures and functions related to skeleton and bones.
*/

#include <string>
#include <vector>
#include "Math.h"
#include <glm/glm.hpp>

/**
*	Joint representation.
*/
struct Joint {
	std::string name;
	Joint * parent;
	std::vector<Joint *> children;
	int childrenCount;
	float offset[3];
	int rotationOrder[3];	
	std::vector<glm::vec3> positionPerFrame;	// used only in the root joint, only which has the position channels defined in BVH file
	std::vector<glm::vec3> rotationPerFrame;
};

/**
*	Skeleton representation.
*	In the article is it called MOCAPSEGMENT.
*/
struct Skeleton {
	Joint * root;
	std::vector<Joint *> joints;		// array of pointers to all joints in the skeleton
	void drawSkeleton(long frame);		// OpenGL drawing method
};


