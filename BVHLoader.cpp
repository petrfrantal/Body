/**
*	Definitions of BVHLoader methods.
*/

#include "BVHLoader.h"
#include "Math.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

void BVHLoader::loadMotion(std::istream & file, Animation * animation) {
	std::string input;
	unsigned int frameCount;
	float frameDuration;
	file >> input;		// Frames:
	file >> frameCount;
	animation->animationInfo->frameCount = frameCount;
	file >> input;
	file >> input;
	file >> frameDuration;
	animation->animationInfo->frameDuration = frameDuration;
	animation->animationInfo->framesPerSecond = 1.0f / frameDuration;
	// now the rest of the file is the motion data, each line contains one frame
	unsigned int jointCount = animation->animationInfo->jointCount;
	std::vector<Joint *> joints = animation->skeleton->joints;
	Joint * root = animation->skeleton->root;
	Joint * joint;
	float rootPositionPerFrameX;
	float rootPositionPerFrameY;
	float rootPositionPerFrameZ;
	float rootRotationPerFrame[3];
	float jointRotationPerFrame[3];
	glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::mat4 rootTransform;
	glm::mat4 rootTranslation;
	glm::mat4 rootRotation;
	glm::mat4 rootXrotation;
	glm::mat4 rootYrotation;
	glm::mat4 rootZrotation;
	glm::mat4 jointXrotation;
	glm::mat4 jointYrotation;
	glm::mat4 jointZrotation;
	glm::mat4 jointTranslation;
	glm::mat4 jointTranslation2;
	for (unsigned int frame = 0; frame < frameCount; frame++) {
		// init root related matrices that will be used later to identities
		rootTransform = glm::mat4(1.0f);
		rootTranslation = glm::mat4(1.0f);
		rootRotation = glm::mat4(1.0f);
		rootXrotation = glm::mat4(1.0f);
		rootYrotation = glm::mat4(1.0f);
		rootZrotation = glm::mat4(1.0f);
		// load root values
		// positions should always be in a x-y-z order
		file >> rootPositionPerFrameX;
		file >> rootPositionPerFrameY;
		file >> rootPositionPerFrameZ;
		// rotations, store them in correct order based on rotationOrder
		file >> rootRotationPerFrame[root->rotationOrder[0]];
		file >> rootRotationPerFrame[root->rotationOrder[1]];
		file >> rootRotationPerFrame[root->rotationOrder[2]];
		//rootTransform = glm::translate(rootTransform, glm::vec3(rootPositionPerFrameX, rootPositionPerFrameY, rootPositionPerFrameZ));		// test

		// Test matrices
		/*
		if (frame == 2) {
			glm::mat4 testRotation = glm::mat4(1.0);
			glm::mat4 testTranslation = glm::mat4(1.0);
			testRotation = glm::rotate(testRotation, degreesToRadians(rootRotationPerFrame[root->rotationOrder[2]]), yAxis);		// rotation around y axis
			testTranslation = glm::translate(testTranslation, glm::vec3(rootPositionPerFrameX, rootPositionPerFrameY, rootPositionPerFrameZ));
			std::cout << "Matrix rotated around Y axis" << std::endl;
			printGlmMatrixColumnsAsColumns(testRotation);
			std::cout << "Matrix translated in X axis" << std::endl;
			printGlmMatrixColumnsAsColumns(testTranslation);
			std::cout << "Matice slozene transformace, nejdriv rotace, pak translace" << std::endl;
			glm::mat4 testMat3 = testTranslation * testRotation;
			printGlmMatrixColumnsAsColumns(testMat3);
		}*/


		for (int i = 0; i < 3; i++) {
			switch (root->rotationOrder[i]) {
				case xRotation:
					rootXrotation = glm::rotate(glm::mat4(1.0f), degreesToRadians(rootRotationPerFrame[root->rotationOrder[i]]), xAxis);
					rootRotation = rootXrotation * rootRotation;
					break;
				case yRotation:
					rootYrotation = glm::rotate(glm::mat4(1.0f), degreesToRadians(rootRotationPerFrame[root->rotationOrder[i]]), yAxis);
					rootRotation = rootYrotation * rootRotation;
					break;
				case zRotation:
					rootZrotation = glm::rotate(glm::mat4(1.0f), degreesToRadians(rootRotationPerFrame[root->rotationOrder[i]]), zAxis);
					rootRotation = rootZrotation * rootRotation;
					break;
				}
		}
		rootTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(rootPositionPerFrameX, rootPositionPerFrameY, rootPositionPerFrameZ));
		rootTransform = rootTranslation * rootRotation;
		//printGlmMatrixColumnsAsColumns(rootTransform);
		root->transformPerFrame.push_back(rootTransform);
		// load other joint values
		for (unsigned int jointIndex = 1; jointIndex < jointCount; jointIndex++) {
			// init rotations to identities
			jointXrotation = glm::mat4(1.0f);
			jointYrotation = glm::mat4(1.0f);
			jointZrotation = glm::mat4(1.0f);
			glm::mat4 jointTransform = glm::mat4(1.0f);
			// get joint and load values
			joint = joints[jointIndex];
			file >> jointRotationPerFrame[joint->rotationOrder[0]];
			file >> jointRotationPerFrame[joint->rotationOrder[1]];
			file >> jointRotationPerFrame[joint->rotationOrder[2]];
			jointTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(-joints[jointIndex]->globalOffset[0], -joints[jointIndex]->globalOffset[1], -joints[jointIndex]->globalOffset[2]));
			jointTransform = jointTranslation * jointTransform;
			for (int i = 0; i < 3; i++) {
				switch (joint->rotationOrder[i]) {
				case xRotation:
					jointXrotation = glm::rotate(glm::mat4(1.0f), degreesToRadians(jointRotationPerFrame[joint->rotationOrder[i]]), xAxis);
					jointTransform = jointXrotation * jointTransform;
					break;
				case yRotation:
					jointYrotation = glm::rotate(glm::mat4(1.0f), degreesToRadians(jointRotationPerFrame[joint->rotationOrder[i]]), yAxis);
					jointTransform = jointYrotation * jointTransform;
					break;
				case zRotation:
					jointZrotation = glm::rotate(glm::mat4(1.0f), degreesToRadians(jointRotationPerFrame[joint->rotationOrder[i]]), zAxis);
					jointTransform = jointZrotation * jointTransform;
					break;
				}
			}
			jointTranslation2 = glm::translate(glm::mat4(1.0f), glm::vec3(joints[jointIndex]->globalOffset[0], joints[jointIndex]->globalOffset[1], joints[jointIndex]->globalOffset[2]));
			jointTransform = jointTranslation2 * jointTransform;
			//printGlmMatrixColumnsAsColumns(joint->parent->transformPerFrame[frame] * jointTransform);
			joint->transformPerFrame.push_back(joint->parent->transformPerFrame[frame] * jointTransform);
		}
	}
}

Joint * BVHLoader::loadJoint(std::istream & file, Animation * animation, Joint * parent) {
	Joint * joint = new Joint();
	joint->parent = parent;
	file >> joint->name;
	std::string input;
	file >> input;		// read the '{' which has no further use
	// then there is always the OFFSET keyword
	file >> input;
	float inputXOffset;
	float inputYOffset;
	float inputZOffset;
	file >> inputXOffset;
	file >> inputYOffset;
	file >> inputZOffset;
	joint->offset[xOffset] = inputXOffset;
	joint->offset[yOffset] = inputYOffset;
	joint->offset[zOffset] = inputZOffset;
	joint->globalOffset[xOffset] = inputXOffset;
	joint->globalOffset[yOffset] = inputYOffset;
	joint->globalOffset[zOffset] = inputZOffset;
	// then there are always the channels
	file >> input;
	int channelCount;
	file >> channelCount;
	if (channelCount == 6) {		// root joint, where xPosition, yPosition and zPosition are added. Jump over them, we expect them always in this order.
		file >> input;
		file >> input;
		file >> input;
	}
	// rotation channels
	for (int i = 0; i < 3; i++) {
		file >> input;
		if (input[0] == 'X') {
			joint->rotationOrder[i] = xRotation;
		} else if (input[0] == 'Y') {
			joint->rotationOrder[i] = yRotation;
		} else if (input[0] == 'Z') {
			joint->rotationOrder[i] = zRotation;
		} else {
			std::cout << "ERROR: occured while parsing the channels of a joint in part HIERARCHY." << std::endl;
			return NULL;
		}
	}
	// save this joint into the skeleton
	animation->skeleton->joints.push_back(joint);
	joint->index = animation->skeleton->joints.size() - 1;
	// save the joint's vertex (tuple of three floats) into Wireframe's vertices; the offset read from BVH file line must be added to the joint's parent's offset
	if (parent != NULL) {
		joint->globalOffset[xOffset] += parent->globalOffset[xOffset];
		joint->globalOffset[yOffset] += parent->globalOffset[yOffset];
		joint->globalOffset[zOffset] += parent->globalOffset[zOffset];
	}
	animation->skeleton->wireframeModel->vertices.push_back(joint->globalOffset[xOffset]);
	animation->skeleton->wireframeModel->vertices.push_back(joint->globalOffset[yOffset]);
	animation->skeleton->wireframeModel->vertices.push_back(joint->globalOffset[zOffset]);
	// also save the indices for the actual joint and it's parent - together they are bone; this does not occur when parsing the root
	if (parent != NULL) {
		animation->skeleton->boneIndices.push_back(parent->index);
		animation->skeleton->boneIndices.push_back(joint->index);
		// and push also offsets of the joint and it's parent to the array for creating bones as lines
		animation->skeleton->wireframeModel->boneVertices.push_back(parent->offset[xOffset]);
		animation->skeleton->wireframeModel->boneVertices.push_back(parent->offset[yOffset]);
		animation->skeleton->wireframeModel->boneVertices.push_back(parent->offset[zOffset]);
		animation->skeleton->wireframeModel->boneVertices.push_back(joint->offset[xOffset]);
		animation->skeleton->wireframeModel->boneVertices.push_back(joint->offset[yOffset]);
		animation->skeleton->wireframeModel->boneVertices.push_back(joint->offset[zOffset]);
	}
	// now only keywords JOINT, End Site or '}' can occur
	while (file.good()) {
		file >> input;
		if (input == "JOINT") {
			Joint * child = loadJoint(file, animation, joint);
			joint->children.push_back(child);
		} else if (input == "End") {
			file >> input;		// Site
			file >> input;		// {
			file >> input;		// OFFSET
			// TODO: determine length of the last bone based on the end site offset, so far we do not save the length based on offsets
			file >> input;		// x
			file >> input;		// y
			file >> input;		// z
			file >> input;		// }
		} else if (input == "}") {
			return joint;
		} else {
			std::cout << "ERROR: occured after the parsing of the channels of a joint in part HIERARCHY. Expected 'Joint' or 'End site' of '}' keywords but didn't get them." << std::endl;
			return NULL;
		}
	}
}

void BVHLoader::loadHierarchy(std::istream & file, Animation * animation) {
	std::string input;
	while (file.good()) {
		file >> input;
		if (input == "ROOT") {
			animation->skeleton->root = loadJoint(file, animation);
			animation->animationInfo->jointCount = animation->skeleton->joints.size();
		} else if (input == "MOTION") {
			loadMotion(file, animation);
			return;
		}
	} 
}

Animation * BVHLoader::loadAnimation(const std::string & fileName) {
	std::ifstream file(fileName);
	if (file.is_open()) {
		std::string input;
		file >> input;
		if (input == "HIERARCHY") {  // test whether the file starts with HIERARCHY keyword
			Animation * animation = new Animation();
			loadHierarchy(file, animation);
			file.close();
			return animation;
		} else {
			std::cout << "ERROR: Selected BVH file does not contain the keyword HIERARCHY in the beginning" << std::endl;
			file.close();
			return NULL;
		}
	}
}