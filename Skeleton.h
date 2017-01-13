#pragma once

/**
*	Data structures and functions related to skeleton and bones.
*/

#include <glm/glm.hpp>
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "mesh.h"
#include "obj_loader.h"
#include "shader.h"
#include "camera.h"

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
	float globalOffset[3];
	int rotationOrder[3];
	std::vector<glm::mat4> transformPerFrame;
};

struct CylinderBone {
	glm::vec3 halfTranslation;		// translation to the center of the bone; that is half way from the one joint to the another
	glm::mat4 rotation;				// rotation to adjust the bone's direction to point from first joint to the other
	glm::mat4 scale;
	Joint * parentJoint;			// a joint which translation this bone inherits
};

/**
*	Skeleton representation.
*/
struct Skeleton {
	Joint * root;
	std::vector<Joint *> joints;		// array of pointers to all joints in the skeleton
	Mesh * mesh;
	Mesh * cylindricalMesh;
	// cylinder bones
	std::vector<CylinderBone *> cylinderBones;
	WireframeModel * wireframeModel;
	std::vector<unsigned int> boneIndices;
	// constructors and methods
	Skeleton(void);
	void createWireframeModelMesh(Shader * shader);
	void createWireframeModelMesh(Shader * jointShader, Shader * boneShader);
	void createCylindricalMesh(Shader * shader);
	void drawOnlyJoints(Shader * shader, unsigned int frame, Camera & camera);
	void drawWireframeModel(Shader * jointShader, Shader * boneShader, unsigned int frame, Camera & camera);
	void drawCylindricalModel(Shader * shader, unsigned int frame, Camera & camera);
};


