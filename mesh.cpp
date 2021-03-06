#include "mesh.h"

Mesh::Mesh(WireframeModel * wireframeModel, Shader * shader) {
	// generate VAO
	glGenVertexArrays(1, &jointVertexArrayObject);
	glBindVertexArray(jointVertexArrayObject);
	
	// generate VBO, first copy vertices
	float * vertices = new float[wireframeModel->vertices.size()];
	std::copy(wireframeModel->vertices.begin(), wireframeModel->vertices.end(), vertices);
	glGenBuffers(1, &jointVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, jointVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// enable attributes to shaders
	glEnableVertexAttribArray(shader->positionLocation);
	glVertexAttribPointer(shader->positionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindVertexArray(0);
	delete[] vertices;
}

Mesh::Mesh(WireframeModel * wireframeModel, Shader * jointShader, Shader * lineBoneShader) {
	// generate VAO
	glGenVertexArrays(1, &jointVertexArrayObject);
	glBindVertexArray(jointVertexArrayObject);

	// generate VBO for drawing of the joints, first copy vertices
	int verticesSize = wireframeModel->vertices.size();
	float * vertices = new float[verticesSize];
	std::copy(wireframeModel->vertices.begin(), wireframeModel->vertices.end(), vertices);
	glGenBuffers(1, &jointVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, jointVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesSize, vertices, GL_STATIC_DRAW);

	// enable attributes to the joint vertex shader
	glEnableVertexAttribArray(jointShader->positionLocation);
	glVertexAttribPointer(jointShader->positionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	glBindVertexArray(0);

	// generate VAO and VBO for drawing of the bones as lines
	glGenVertexArrays(1, &lineVertexArrayObject);
	glBindVertexArray(lineVertexArrayObject);

	// copy the vertices
	float * boneVertices = new float[wireframeModel->boneVertices.size()];
	std::copy(wireframeModel->boneVertices.begin(), wireframeModel->boneVertices.end(), boneVertices);

	// create the indices; used to index the right transform matrix in shader
	int jointIndicesSize = wireframeModel->boneIndices.size();
	int * jointIndices = new int [jointIndicesSize];
	for (int i = 0; i < jointIndicesSize; i += 2) {
		jointIndices[i] = 1;
		jointIndices[i + 1] = 2;
	}	

	// buffer the vertices and indices
	int boneVerticesSize = wireframeModel->boneVertices.size();
	glGenBuffers(1, &lineVertexAttrBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, lineVertexAttrBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * boneVerticesSize, boneVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(lineBoneShader->positionLocation);
	glVertexAttribPointer(lineBoneShader->positionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glGenBuffers(1, &lineIndexAttrBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, lineIndexAttrBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * jointIndicesSize, jointIndices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(lineBoneShader->jointIndexLocation);
	glVertexAttribIPointer(lineBoneShader->jointIndexLocation, 1, GL_INT, sizeof(int), (void*)0);

	glBindVertexArray(0);

	delete[] boneVertices;
	delete[] jointIndices;
	delete[] vertices;
}

// Used for the cylindrical model
Mesh::Mesh(Shader * shader) {

	// Cylinder model

	// generate VAO
	glGenVertexArrays(1, &cylinderVertexArrayObject);
	glBindVertexArray(cylinderVertexArrayObject);

	// vertices - VBO
	glGenBuffers(1, &cylinderVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cylinderVertices), cylinderVertices, GL_STATIC_DRAW);

	// indices - EBO
	glGenBuffers(1, &cylinderElementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderElementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cylinderTriangles), cylinderTriangles, GL_STATIC_DRAW);

	glEnableVertexAttribArray(shader->positionLocation);
	glVertexAttribPointer(shader->positionLocation, 3, GL_FLOAT, GL_FALSE, cylinderNAttribsPerVertex * sizeof(float), (void*)0);
	glEnableVertexAttribArray(shader->normalLocation);
	glVertexAttribPointer(shader->normalLocation, 3, GL_FLOAT, GL_FALSE, cylinderNAttribsPerVertex * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);

	// Sphere model

	// generate VAO
	glGenVertexArrays(1, &sphereVertexArrayObject);
	glBindVertexArray(sphereVertexArrayObject);

	// vertices - VBO
	glGenBuffers(1, &sphereVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphereVertices), sphereVertices, GL_STATIC_DRAW);

	// indices - EBO
	glGenBuffers(1, &sphereElementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereElementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphereTriangles), sphereTriangles, GL_STATIC_DRAW);

	glEnableVertexAttribArray(shader->positionLocation);
	glVertexAttribPointer(shader->positionLocation, 3, GL_FLOAT, GL_FALSE, sphereNAttribsPerVertex * sizeof(float), (void*)0);
	glEnableVertexAttribArray(shader->normalLocation);
	glVertexAttribPointer(shader->normalLocation, 3, GL_FLOAT, GL_FALSE, sphereNAttribsPerVertex * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);

	cylinderTriangleCount = cylinderNTriangles;
	sphereTriangleCount = sphereNTriangles;
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &cylinderVertexArrayObject);
	glDeleteBuffers(1, &cylinderVertexBufferObject);
	glDeleteBuffers(1, &cylinderElementBufferObject);
	glDeleteVertexArrays(1, &sphereVertexArrayObject);
	glDeleteBuffers(1, &sphereVertexBufferObject);
	glDeleteBuffers(1, &sphereElementBufferObject);
	glDeleteVertexArrays(1, &lineVertexArrayObject);
	glDeleteBuffers(1, &lineVertexAttrBufferObject);
	glDeleteBuffers(1, &lineIndexAttrBufferObject);
	glDeleteVertexArrays(1, &jointVertexArrayObject);
	glDeleteBuffers(1, &jointVertexBufferObject);
}

GLuint Mesh::getCylinderVAO(void) {
	return cylinderVertexArrayObject;
}

GLuint Mesh::getSphereVAO(void) {
	return sphereVertexArrayObject;
}

GLuint Mesh::getLineBoneVAO(void) {
	return lineVertexArrayObject;
}

GLuint Mesh::getPointJointVAO(void) {
	return jointVertexArrayObject;
}

unsigned int Mesh::getCylinderTrinagleCount(void) {
	return cylinderTriangleCount;
}

unsigned int Mesh::getSphereTriangleCount(void) {
	return sphereTriangleCount;
}