#pragma once

/**
*	Data structures and functions related to skeleton and bones.
*/

#include <string>

/**
*	Joint representation. 
*	In the article is it called NODE. 
*/
struct Joint {
	std::string name;
	float offset[3];
	//float euler[3];
	float rotation[3];		// initial pose rotation - maybe not needed?
	float rotationOrder[3];	
	// ...
	int childrenCount;
	Joint ** children;
	Joint * parent;
	float ** offsetPerFrame;
	//float ** eulerPerFrame;
	float ** rotationPerFrame;
	// scale is not used in BVH
	// ...
};

/**
*	Skeleton representation.
*	In the article is it called MOCAPSEGMENT.
*/
struct Skeleton {
	Joint * root;
	Joint ** bones;		// array of pointers to all bones in the skeleton
	// ...
};


