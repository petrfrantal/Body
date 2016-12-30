#pragma once

/**
*	Data structures and functions related to skeleton and bones.
*/

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "mesh.h"
#include "obj_loader.h"
#include "shader.h"
#include "glm/gtc/type_ptr.hpp"

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

/**
*	Skeleton representation.
*/
struct Skeleton {
	Joint * root;
	std::vector<Joint *> joints;		// array of pointers to all joints in the skeleton
	Mesh * mesh;
	Mesh * cylindricalMesh;
	WireframeModel * wireframeModel;
	std::vector<unsigned int> boneIndices;
	const int BONE_COUNT = 43;
	// testing viewport matrix
	float arr [16] = {400.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 300.0f, 0.0f, 0.0f,
						0.0f, 0.0, 1.0f, 0.0f, 
						400.0f, 300.0f, 0.0f, 1.0f};
	glm::mat4 viewport = glm::make_mat4(arr);
	// constructors and methods
	Skeleton(void);
	void createWireframeModelMesh(Shader * shader);
	void createWireframeModelMesh(Shader * jointShader, Shader * boneShader);
	void createCylindricalMesh(const float * vertices, unsigned int verticesSize, const unsigned int * indices, unsigned int indicesSize, Shader * shader);
	void drawOnlyJoints(Shader * shader, unsigned int frame, Camera & camera);
	void drawWireframeModel(Shader * jointShader, Shader * boneShader, unsigned int frame, Camera & camera);
	void drawCylindricalModel(Shader * shader, unsigned int frame, Camera & camera);
};


