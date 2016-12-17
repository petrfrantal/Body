#pragma once

/**
*	Data structures and functions related to skeleton and bones.
*/

#include <string>
#include <vector>
#include "Math.h"
#include <glm/glm.hpp>
#include "mesh.h"
#include "obj_loader.h"
#include "shader.h"

/**
*	Joint representation.
*/
struct Joint {
	std::string name;
	Joint * parent;
	unsigned int index;			// used for indexing into EBO
	std::vector<Joint *> children;
	int childrenCount;
	float offset[3];
	int rotationOrder[3];
	std::vector<glm::mat4> transformPerFrame;
};

/**
*	Skeleton representation.
*/
struct Skeleton {
	Joint * root;
	std::vector<Joint *> joints;		// array of pointers to all joints in the skeleton
	Mesh * mesh;
	WireframeModel * wireframeModel;
	Skeleton(void);
	void createWireframeModelMesh(Shader * shader);
	void drawSkeleton(long frame);		// OpenGL drawing method
};


