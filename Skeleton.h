#pragma once

/**
*	Data structures and functions related to skeleton and bones.
*/

#include <string>
#include <vector>
#include "Math.h"

/**
*	Joint representation. 
*	In the article is it called NODE. 
*/
struct Joint {
	std::string name;
	float offset[3];
	//float euler[3];
	float position[3];		// position of a root joint
	float rotation[3];		// initial pose rotation - maybe not needed?
	int rotationOrder[3];	
	int childrenCount;
	std::vector<Joint *> children;
	Joint * parent;
	std::vector<Vector3f> offsetPerFrame;	// used only in the root joint, only which has the position channels defined in BVH file
	//float ** eulerPerFrame;
	std::vector<Vector3f> rotationPerFrame;
	// ...
};

/**
*	Skeleton representation.
*	In the article is it called MOCAPSEGMENT.
*/
struct Skeleton {
	Joint * root;
	std::vector<Joint *> joints;		// array of pointers to all joints in the skeleton
	// ...

	void drawSkeleton(long frame);		// OpenGL drawing method
};


