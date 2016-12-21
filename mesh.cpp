#define _SCL_SECURE_NO_WARNINGS
#include "mesh.h"
#include "util.h"
#include "debugTimer.h"
#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "shader.h"

Mesh::Mesh(WireframeModel * wireframeModel, Shader * shader) {
	// generate VAO
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	
	// generate VBO, first copy vertices
	float * vertices = new float[wireframeModel->vertices.size()];
	std::copy(wireframeModel->vertices.begin(), wireframeModel->vertices.end(), vertices);
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// enable attributes to shaders
	glEnableVertexAttribArray(shader->positionLocation);
	glVertexAttribPointer(shader->positionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindVertexArray(0);
	delete[] vertices;
}

Mesh::Mesh(WireframeModel * wireframeModel, Shader * jointShader, Shader * lineBoneShader) {
	// generate VAO
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// generate VBO for drawing of the joints, first copy vertices
	float * vertices = new float[wireframeModel->vertices.size()];
	std::copy(wireframeModel->vertices.begin(), wireframeModel->vertices.end(), vertices);
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// enable attributes to the joint vertex shader
	glEnableVertexAttribArray(jointShader->positionLocation);
	glVertexAttribPointer(jointShader->positionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	glBindVertexArray(0);

	// generate VAO and VBO for drawing of the bones as lines
	glGenVertexArrays(1, &lineBoneArrayObject);
	glBindVertexArray(lineBoneArrayObject);

	// copy the vertices
	float * boneVertices = new float[wireframeModel->boneVertices.size()];
	std::copy(wireframeModel->boneVertices.begin(), wireframeModel->boneVertices.end(), boneVertices);

	// create the indices; used to index the right transform matrix in shader
	unsigned int jointIndicesSize = wireframeModel->boneIndices.size();
	int * jointIndices = new int [jointIndicesSize];
	for (unsigned int i = 0; i < jointIndicesSize; i += 2) {
		jointIndices[i] = 1;
		jointIndices[i + 1] = 2;
	}

	// buffer the vertices and indices
	glGenBuffers(1, &jointBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, jointBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boneVertices), boneVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(lineBoneShader->positionLocation);
	glVertexAttribPointer(lineBoneShader->positionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glGenBuffers(1, &jointIndexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, jointIndexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(jointIndices), jointIndices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(lineBoneShader->jointIndexLocation);
	glVertexAttribPointer(lineBoneShader->jointIndexLocation, 1, GL_INT, GL_FALSE, sizeof(int), (void*)0);

	glBindVertexArray(0);

	delete[] boneVertices;
	delete[] jointIndices;
	delete[] vertices;
}

Mesh::Mesh(const std::string& fileName)
{
    initMesh(OBJModel(fileName).ToIndexedModel());
}

void Mesh::initMesh(const IndexedModel& model)
{
    m_numIndices = model.indices.size();

    glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(model.texCoords[0]) * model.texCoords.size(), &model.texCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

Mesh::Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
    IndexedModel model;

	for(unsigned int i = 0; i < numVertices; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		positions.push_back(*vertices[i].GetPos());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
		model.normals.push_back(*vertices[i].GetNormal());
	}
	
	for(unsigned int i = 0; i < numIndices; i++)
        model.indices.push_back(indices[i]);

	/*for (unsigned int i = 0; i < numVertices; i++) {
		std::cout << "Vertex " << i << " " << model.positions[i].x << " " << model.positions[i].y << " " << model.positions[i].z << std::endl;
	}*/

    initMesh(model);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(1, &vertexBufferObject);
	// former buffers
	glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Mesh::draw()
{
	glBindVertexArray(m_vertexArrayObject);

	//glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
	glDrawElementsBaseVertex(GL_POINTS, m_numIndices, GL_UNSIGNED_INT, 0, 0);

	glBindVertexArray(0);
}

void Mesh::drawJointAlone(size_t jointIndex) {
	glBindVertexArray(vertexArrayObject);
	glDrawArrays(GL_POINTS, jointIndex, 1);		// draws one joint as a point
	glBindVertexArray(0);
}

void Mesh::drawJointInBone(size_t jointIndex) {
	glBindVertexArray(vertexArrayObject);
	glDrawArrays(GL_LINES, jointIndex, 1);
	glBindVertexArray(0);
}
