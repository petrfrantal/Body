#include "Skeleton.h"
#include <iostream>

// test
void printGlmMatrixColumnsAsColumns2(glm::mat4 & m) {
	std::cout << m[0][0] << " " << m[1][0] << " " << m[2][0] << " " << m[3][0] << " |" << std::endl;
	std::cout << m[0][1] << " " << m[1][1] << " " << m[2][1] << " " << m[3][1] << " |" << std::endl;
	std::cout << m[0][2] << " " << m[1][2] << " " << m[2][2] << " " << m[3][2] << " |" << std::endl;
	std::cout << m[0][3] << " " << m[1][3] << " " << m[2][3] << " " << m[3][3] << " V" << std::endl;
}

Skeleton::Skeleton(void) {
	wireframeModel = new WireframeModel();
}

void Skeleton::createWireframeModelMesh(Shader * shader) {
	mesh = new Mesh(wireframeModel, shader);
}

void Skeleton::createWireframeModelMesh(Shader * jointShader, Shader * boneShader) {
	wireframeModel->boneIndices = boneIndices;
	mesh = new Mesh(wireframeModel, jointShader, boneShader);
}

void Skeleton::drawOnlyJoints(Shader * shader, unsigned int frame, Camera & camera) {
	glPointSize(5.0f);		// 5.0f
	size_t jointCount = joints.size();
	// bind shader
	glUseProgram(shader->shaderProgram);
	for (size_t jointIndex = 0; jointIndex < jointCount; jointIndex++) {
		// transform the joint
		glm::mat4 modelMatrix = joints[jointIndex]->transformPerFrame[frame];
		// testing translations to get the model to the camera
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-75.0f, -60.0f, -80.0f));		// ONLY FOR TEST
		glm::mat4 projectionViewMatrix = camera.getViewProjection();
		glm::mat4 MVP = projectionViewMatrix * modelMatrix;			// multiplication from right to left
		shader->setMVPMatrix(MVP);
		// draw the joint
		mesh->drawJointAlone(jointIndex);
	}
	glUseProgram(0);
}

void Skeleton::drawWireframeModel(Shader * jointShader, Shader * boneShader, unsigned int frame, Camera & camera) {
	
	glPointSize(5.0f);		// 5.0f
	int jointCount = joints.size();
	// bind shader
	glUseProgram(jointShader->shaderProgram);
	glm::mat4 projectionViewMatrix = camera.getViewProjection();
	for (int jointIndex = 0; jointIndex < jointCount; jointIndex++) {
		// transform the joint
		glm::mat4 modelMatrix = joints[jointIndex]->transformPerFrame[frame];
		// testing translations to get the model to the camera
		//printGlmMatrixColumnsAsColumns2(modelMatrix);
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(-75.0f, -60.0f, -80.0f));
		modelMatrix = translation * modelMatrix;
		glm::mat4 MVP = projectionViewMatrix * modelMatrix;			// multiplication from right to left
		jointShader->setMVPMatrix(MVP);
		// draw the joint
		mesh->drawJointAlone(jointIndex);			// draws one joint as a point

		// test draw
		/*
		glBindVertexArray(mesh->vertexArrayObject);
		glDrawArrays(GL_POINTS, jointIndex, 1);		// draws one joint as a point
		
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-60.0f, 0.0f, 0.0f));
		MVP = projectionViewMatrix * modelMatrix;
		shader->setMVPMatrix(MVP);
		glDrawArrays(GL_POINTS, jointIndex, 1);
		glBindVertexArray(0);*/

		// debug output
		//glm::vec4 position = glm::vec4(wireframeModel->vertices[3 * jointIndex], wireframeModel->vertices[3 * jointIndex + 1], wireframeModel->vertices[3 * jointIndex + 2], 1.0f);
		// model coords
		//glm::vec4 transformed = modelMatrix * position;
		//std::cout << "Joint " << jointIndex << "(" << joints[jointIndex]->name << ") " << ": " << transformed[0] / transformed[3] << " " << transformed[1] / transformed[3] << " " << transformed[2] / transformed[3] << " " << transformed[3] / transformed[3] << std::endl;
		
		//glm::vec4 transformed = MVP * position;
		//glm::vec4 transformedWithViewport = viewport * transformed;
		//std::cout << "Joint " << jointIndex << ": " << transformed[0] << " " << transformed[1] << " " << transformed[2] << " " << transformed[3] << std::endl;
		//transformed = modelMatrix * position;
		// after projection
		//std::cout << "Joint " << jointIndex << ": " << transformed[0] / transformed[3] << " " << transformed[1] / transformed[3] << " " << transformed[2] / transformed[3] << " " << transformed[3] / transformed[3] << std::endl;
		// after viewport
		//std::cout << "Joint " << jointIndex << "(" << joints[jointIndex]->name << ") " << ": " << transformedWithViewport[0] / transformedWithViewport[3] << " " << transformedWithViewport[1] / transformedWithViewport[3] << " " << transformedWithViewport[2] / transformedWithViewport[3] << " " << transformedWithViewport[3] / transformedWithViewport[3] << std::endl;
	}
	glBindVertexArray(0);
	glUseProgram(0);
	//std::cout << "----------------------------------------------------------------------" << std::endl;
	
	// now draw bones as GL_LINEs consisting of two joints = so far doesn't work
	
	
	glLineWidth(3.0f);
	// bind shader
	glUseProgram(boneShader->shaderProgram);
	glBindVertexArray(mesh->lineVertexArrayObject);
	size_t vertexCount = boneIndices.size();
	glm::mat4 firstModelMatrix;
	glm::mat4 secondModelMatrix;
	glm::mat4 firstMVP;
	glm::mat4 secondMVP;
	for (int jointIndex = 0; jointIndex < vertexCount; jointIndex += 2) {
		firstModelMatrix = joints[boneIndices[jointIndex]]->transformPerFrame[frame];
		secondModelMatrix = joints[boneIndices[jointIndex + 1]]->transformPerFrame[frame];
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(-75.0f, -60.0f, -80.0f));
		firstModelMatrix = translation * firstModelMatrix;
		secondModelMatrix = translation * secondModelMatrix;
		firstMVP = projectionViewMatrix * firstModelMatrix;
		secondMVP = projectionViewMatrix * secondModelMatrix;
		glUniformMatrix4fv(boneShader->firstMVPLocation, 1, GL_FALSE, &firstMVP[0][0]);
		glUniformMatrix4fv(boneShader->secondMVPLocation, 1, GL_FALSE, &secondMVP[0][0]);
		//glDrawArrays(GL_LINES, jointIndex, 2);
	}
	//glDrawArrays(GL_LINES, 0, 84 * 3);		// bone indices * 3 (3 verts)	-	make some variable for the 84
	glBindVertexArray(0);
	glUseProgram(0);
	
}