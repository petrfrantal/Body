#include "Skeleton.h"

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

void Skeleton::createCylindricalMesh(Shader * shader) {
	cylindricalMesh = new Mesh(shader);
}

void Skeleton::createWireframeModelMesh(Shader * jointShader, Shader * boneShader) {
	wireframeModel->boneIndices = boneIndices;
	mesh = new Mesh(wireframeModel, jointShader, boneShader);
}

void Skeleton::drawOnlyJoints(Shader * shader, unsigned int frame, Camera & camera) {
	glPointSize(5.0f);
	size_t jointCount = joints.size();
	glm::mat4 modelMatrix;
	glm::mat4 MVP;
	glm::mat4 projectionViewMatrix = camera.getViewProjection();
	glUseProgram(shader->shaderProgram);
	glBindVertexArray(mesh->vertexArrayObject);
	for (size_t jointIndex = 0; jointIndex < jointCount; jointIndex++) {
		// transform the joint
		modelMatrix = joints[jointIndex]->transformPerFrame[frame];
		MVP = projectionViewMatrix * modelMatrix;
		glUniformMatrix4fv(shader->MVPLocation, 1, GL_FALSE, &MVP[0][0]);
		glDrawArrays(GL_POINTS, (int)jointIndex, 1);						// draw the joint
	}
	glUseProgram(0);
	glBindVertexArray(0);
}

void Skeleton::drawWireframeModel(Shader * jointShader, Shader * boneShader, unsigned int frame, Camera & camera) {
	glPointSize(5.0f);
	size_t jointCount = joints.size();
	glm::mat4 modelMatrix;
	glm::mat4 MVP;
	glm::mat4 projectionViewMatrix = camera.getViewProjection();
	glUseProgram(jointShader->shaderProgram);
	glBindVertexArray(mesh->vertexArrayObject);
	for (size_t jointIndex = 0; jointIndex < jointCount; jointIndex++) {
		modelMatrix = joints[jointIndex]->transformPerFrame[frame];		// transform the joint
		//printGlmMatrixColumnsAsColumns2(modelMatrix);
		MVP = projectionViewMatrix * modelMatrix;
		glUniformMatrix4fv(jointShader->MVPLocation, 1, GL_FALSE, &MVP[0][0]);
		glDrawArrays(GL_POINTS, (int)jointIndex, 1);							// draws one joint as a point

		// testing prints


		if (jointIndex != jointCount - 1) {
			glm::vec4 v = glm::vec4(joints[jointIndex + 1]->globalOffset[0], joints[jointIndex + 1]->globalOffset[1], joints[jointIndex + 1]->globalOffset[2], 1.0f);
			//glm::vec4 a = modelMatrix * v;

			//std::cout << a.x << " " << a.y << " " << a.z << " " << a.w << std::endl;
		}

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
	
	// now draw bones as GL_LINES consisting of two joints
	
	glLineWidth(3.0f);
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
		firstMVP = projectionViewMatrix * firstModelMatrix;
		secondMVP = projectionViewMatrix * secondModelMatrix;
		glUniformMatrix4fv(boneShader->firstMVPLocation, 1, GL_FALSE, &firstMVP[0][0]);
		glUniformMatrix4fv(boneShader->secondMVPLocation, 1, GL_FALSE, &secondMVP[0][0]);
		glDrawArrays(GL_LINES, jointIndex, 2);
	}
	glBindVertexArray(0);
	glUseProgram(0);	
}

void Skeleton::drawCylindricalModel(Shader * shader, unsigned int frame, Camera & camera) {
	glm::mat4 projectionViewMatrix = camera.getViewProjection();
	glm::mat4 modelMatrix;
	glm::mat4 translate;
	glm::mat4 rotation;
	glm::mat4 scale;
	glm::mat4 parentTransform;
	glm::mat4 MVP;
	glm::mat4 normalMatrix;

	// compute the positions of the point lights to illuminate the model from the positions of the root joint and camera
	glm::vec3 rootPosition = glm::vec3(root->offset[0], root->offset[1], root->offset[2]);
	rootPosition = glm::vec3(root->transformPerFrame[frame] * glm::vec4(rootPosition, 1.0f));
	glm::vec3 cameraPosition = camera.getCameraPosition();
	glm::vec3 cameraToRoot = cameraPosition - rootPosition;				// vector for the parametric equation
	float parameter = 0.5f;
	glm::vec3 pointLight1Position = glm::vec3(cameraPosition[0] + parameter * cameraToRoot[0],
												cameraPosition[1] + parameter * cameraToRoot[1],
												cameraPosition[2] + parameter * cameraToRoot[2]);	// compute the parametric equation to find the position of the light
	glm::vec3 pointLight2Position = pointLight1Position + glm::vec3(0.0f, 100.0f, 0.0f);

	glUseProgram(shader->shaderProgram);

	// Draw spheres as joints

	size_t jointCount = joints.size();
	Joint * joint;
	glBindVertexArray(cylindricalMesh->sphereVertexArrayObject);
	for (size_t jointIndex = 0; jointIndex < jointCount; jointIndex++) {
		joint = joints[jointIndex];
		modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.5f, 2.5f, 2.5f));
		translate = glm::translate(glm::mat4(1.0f), glm::vec3(joint->globalOffset[0], joint->globalOffset[1], joint->globalOffset[2]));
		modelMatrix = translate * modelMatrix;
		modelMatrix = joint->transformPerFrame[frame] * modelMatrix;
		normalMatrix = glm::transpose(glm::inverse(modelMatrix));
		MVP = projectionViewMatrix * modelMatrix;
		glUniformMatrix4fv(shader->MVPLocation, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(shader->modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(shader->normalMatrixLocation, 1, GL_FALSE, &normalMatrix[0][0]);
		glUniform3fv(shader->pointLight1PositionLocation, 1, glm::value_ptr(pointLight1Position));
		glUniform3fv(shader->pointLight2PositionLocation, 1, glm::value_ptr(pointLight2Position));
		glDrawElements(GL_TRIANGLES, cylindricalMesh->sphereTriangleCount * 3, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);

	// Draw cylinders as bones

	glBindVertexArray(cylindricalMesh->vertexArrayObject);
	size_t boneCount = cylinderBones.size();
	CylinderBone * bone;
	for (size_t i = 0; i < boneCount; i++) {
		bone = cylinderBones[i];

		// scale bone to make it's length aproximately from one joint to the other
		modelMatrix = bone->scale;

		// rotate bone to make it point from one joint to the other
		rotation = bone->rotation;
		//modelMatrix = glm::rotate(glm::mat4(1.0f), 1.57079633f, glm::vec3(1.0f, 0.0f, 0.0f)) * rotation * modelMatrix;		// with test rotation correction, works for BVH 01_01
		modelMatrix = rotation * modelMatrix;

		// translate the bone to the center between the two joints
		translate = glm::translate(glm::mat4(1.0f), bone->halfTranslation);
		modelMatrix = translate * modelMatrix;

		// transform the bone according to the transform obtained for example from a BVH file
		parentTransform = bone->parentJoint->transformPerFrame[frame];
		modelMatrix = parentTransform * modelMatrix;

		//modelMatrix = glm::scale(modelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));			// test scale

		normalMatrix = glm::transpose(glm::inverse(modelMatrix));
		MVP = projectionViewMatrix * modelMatrix;
		glUniformMatrix4fv(shader->MVPLocation, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(shader->modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(shader->normalMatrixLocation, 1, GL_FALSE, &normalMatrix[0][0]);
		glUniform3fv(shader->pointLight1PositionLocation, 1, glm::value_ptr(pointLight1Position));
		glUniform3fv(shader->pointLight2PositionLocation, 1, glm::value_ptr(pointLight2Position));
		glDrawElements(GL_TRIANGLES, cylindricalMesh->cylinderTriangleCount * 3, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
	glUseProgram(0);
}