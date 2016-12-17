/**
*	Definitions of BVHLoader methods.
*/

#include "BVHLoader.h"
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
	for (unsigned int frame = 0; frame < frameCount; frame++) {
		// load root values
		// positions should always be in a x-y-z order
		file >> rootPositionPerFrameX;
		file >> rootPositionPerFrameY;
		file >> rootPositionPerFrameZ;
		// rotations, store them in correct order based on rotationOrder
		file >> rootRotationPerFrame[root->rotationOrder[0]];
		file >> rootRotationPerFrame[root->rotationOrder[1]];
		file >> rootRotationPerFrame[root->rotationOrder[2]];
		glm::mat4 rootTransform = glm::mat4(1.0f);
		for (int i = 0; i < 3; i++) {
			switch (root->rotationOrder[i]) {
				case xRotation:
					rootTransform = glm::rotate(rootTransform, rootRotationPerFrame[root->rotationOrder[i]], xAxis);
					break;
				case yRotation:
					rootTransform = glm::rotate(rootTransform, rootRotationPerFrame[root->rotationOrder[i]], yAxis);
					break;
				case zRotation:
					rootTransform = glm::rotate(rootTransform, rootRotationPerFrame[root->rotationOrder[i]], zAxis);
					break;
				}
		}
		rootTransform = glm::translate(rootTransform, glm::vec3(rootPositionPerFrameX, rootPositionPerFrameY, rootPositionPerFrameZ));
		root->transformPerFrame.push_back(rootTransform);
		// load other joint values
		for (unsigned int jointIndex = 1; jointIndex < jointCount; jointIndex++) {
			joint = joints[jointIndex];
			file >> jointRotationPerFrame[joint->rotationOrder[0]];
			file >> jointRotationPerFrame[joint->rotationOrder[1]];
			file >> jointRotationPerFrame[joint->rotationOrder[2]];
			glm::mat4 jointTransform = glm::mat4(1.0f);
			for (int i = 0; i < 3; i++) {
				switch (joint->rotationOrder[i]) {
				case xRotation:
					jointTransform = glm::rotate(jointTransform, jointRotationPerFrame[joint->rotationOrder[i]], xAxis);
					break;
				case yRotation:
					jointTransform = glm::rotate(jointTransform, jointRotationPerFrame[joint->rotationOrder[i]], yAxis);
					break;
				case zRotation:
					jointTransform = glm::rotate(jointTransform, jointRotationPerFrame[joint->rotationOrder[i]], zAxis);
					break;
				}
			}
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
	float globalOffsetX = inputXOffset;
	float globalOffsetY = inputYOffset;
	float globalOffsetZ = inputZOffset;
	if (parent != NULL) {
		globalOffsetX += parent->offset[xOffset];
		globalOffsetY += parent->offset[yOffset];
		globalOffsetZ += parent->offset[zOffset];
	}
	animation->skeleton->wireframeModel->vertices.push_back(globalOffsetX);
	animation->skeleton->wireframeModel->vertices.push_back(globalOffsetY);
	animation->skeleton->wireframeModel->vertices.push_back(globalOffsetZ);
	// also save the indices for the actual joint and it's parent - together they are bone; this does not occur when parsing the root
	if (parent != NULL) {
		animation->skeleton->boneIndices.push_back(parent->index);
		animation->skeleton->boneIndices.push_back(joint->index);
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