#include "Skeleton.h"
#include <iostream>

Skeleton::Skeleton(void) {
	wireframeModel = new WireframeModel();
}

void Skeleton::createWireframeModelMesh(Shader * shader) {
	mesh = new Mesh(wireframeModel, shader);
}

void Skeleton::drawWireframeModel(Shader * shader, unsigned int frame, Camera & camera) {
	// first draw joints as GL_POINTs
	glPointSize(5.0f);		// 5.0f
	size_t jointCount = joints.size();
	//std::cout << "SKELETON" << std::endl;
	for (size_t jointIndex = 0; jointIndex < jointCount; jointIndex++) {
		// bind shader
		glUseProgram(shader->shaderProgram);
		// transform the joint
		glm::mat4 modelMatrix = joints[jointIndex]->transformPerFrame[frame];
		glm::mat4 projectionViewMatrix = camera.getViewProjection();
		glm::mat4 MVP = projectionViewMatrix * modelMatrix;			// multiplication from right to left
		shader->setMVPMatrix(MVP);
		// draw the joint
		mesh->drawJointAlone(jointIndex);
		glUseProgram(0);

		// debug output
		glm::vec4 position = glm::vec4(wireframeModel->vertices[3 * jointIndex], wireframeModel->vertices[3 * jointIndex + 1], wireframeModel->vertices[3 * jointIndex + 2], 1.0f);
		glm::vec4 transformed = MVP * position;
		//std::cout << "Joint " << jointIndex << ": " << transformed[0] << " " << transformed[1] << " " << transformed[2] << " " << transformed[3] << std::endl;
		//transformed = modelMatrix * position;
		std::cout << "Joint " << jointIndex << ": " << transformed[0] / transformed[3] << " " << transformed[1] / transformed[3] << " " << transformed[2] / transformed[3] << " " << transformed[3] / transformed[3] << std::endl;
	}
	//std::cout << "----------------------------------------------------------------------" << std::endl;
	
	// now draw bones as GL_LINEs consisting of two joints = so far doesn't work
	/*
	glLineWidth(3.0f);
	size_t boneIndicesCount = boneIndices.size();
	unsigned int jointIndex;
	for (size_t index = 0; index < boneIndicesCount; index++) {
		// bind shader
		glUseProgram(shader->shaderProgram);
		// transform the bone's joint
		jointIndex = boneIndices[index];
		glm::mat4 modelMatrix = joints[jointIndex]->transformPerFrame[frame];
		glm::mat4 projectionViewMatrix = camera.getViewProjection();
		glm::mat4 MVP = projectionViewMatrix * modelMatrix;
		shader->setMVPMatrix(MVP);
		// draw the bone's joint
		mesh->drawJointInBone(jointIndex);
		glUseProgram(0);
	}*/
	
}