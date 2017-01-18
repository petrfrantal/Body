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
#include "shader.h"
#include "camera.h"

/**
*	Joint representation.
*/
struct Joint {
	std::string name;
	Joint * parent;								// parent joint
	unsigned int index;							// index of the joint in the Skeleton joint array
	std::vector<Joint *> children;				// children of the joint
	int childrenCount = 0;						// count of the direct children of the joint (not containing their children)
	float offset[3];							// offset from the parent joint
	float globalOffset[3];						// offset from the root joint
	int rotationOrder[3];						// order of the rotation (around x, y and z axes) used for the joint. It may differ for every joint.
	std::vector<glm::mat4> transformPerFrame;	// array of the transform matrices. Each matrix describes the transform in a different animation frame.
};

/**
*	Representation of a bone of the cylindrical model.
*/
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
	std::vector<Joint *> joints;					// array of pointers to all joints in the skeleton
	Mesh * pointLineMesh;							// mesh of the line model (wireframe model)
	Mesh * cylindricalMesh;							// mesh of the cylindrical model
	std::vector<CylinderBone *> cylinderBones;		// cylinder bones
	WireframeModel * wireframeModel;				// wireframe model containing the vertices of the joints and bones used for the wireframe (line) model
	std::vector<unsigned int> boneIndices;
	// constructors and methods
	Skeleton(void);

	/**
	*	Creates the wireframe model mesh with given shaders.
	*/
	void createWireframeModelMesh(Shader * jointShader, Shader * boneShader);

	/**
	*	Creates the mesh of a cylindrical model using the given shader.
	*/
	void createCylindricalMesh(Shader * shader);

	/**
	*	Draws the wireframe model. Joints of the model are displayed as points and it's bones as lines.
	*/
	void drawWireframeModel(Shader * jointShader, Shader * boneShader, unsigned int frame, Camera & camera);

	/**
	*	Draws the cylindrical model where joints are drawn as spheres and bones as cylinders.
	*/
	void drawCylindricalModel(Shader * shader, unsigned int frame, Camera & camera);
};


