#include "Skeleton.h"
#include <iostream>

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
	// first draw joints as GL_POINTs
	glPointSize(5.0f);		// 5.0f
	size_t jointCount = joints.size();
	//std::cout << "SKELETON" << std::endl;
	for (size_t jointIndex = 0; jointIndex < jointCount; jointIndex++) {
		// bind shader
		glUseProgram(jointShader->shaderProgram);
		// transform the joint
		glm::mat4 modelMatrix = joints[jointIndex]->transformPerFrame[frame];
		// testing translations to get the model to the camera
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-75.0f, -60.0f, -80.0f));

		glm::mat4 projectionViewMatrix = camera.getViewProjection();
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

		glUseProgram(0);

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
	//std::cout << "----------------------------------------------------------------------" << std::endl;
	
	// now draw bones as GL_LINEs consisting of two joints = so far doesn't work
	
	glLineWidth(3.0f);
	// bind shader
	glUseProgram(boneShader->shaderProgram);
	// transform the bone's joint
	glm::mat4 modelMatrix;
	glm::mat4 projectionViewMatrix = camera.getViewProjection();
	glm::mat4 MVP;
	for (int i = 0; i < BONE_COUNT; i++) {
		modelMatrix = joints[i]->transformPerFrame[frame];
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-75.0f, -60.0f, -80.0f));
		MVP = projectionViewMatrix * modelMatrix;
		glUniformMatrix4fv(boneShader->MVPsLocations[i], 1, GL_FALSE, &MVP[0][0]);
	}
	// draw the bone's joint
	glDrawArrays(GL_LINES, 0, 84 * 3);		// bone indices * 3 (3 verts)	-	make some variable for the 84
	glUseProgram(0);
	
}