#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "obj_loader.h"
#include "shader.h"

struct Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& normal)
	{
		this->pos = pos;
		this->texCoord = texCoord;
		this->normal = normal;
	}

	glm::vec3* getPos() { return &pos; }
	glm::vec2* getTexCoord() { return &texCoord; }
	glm::vec3* getNormal() { return &normal; }

private:
	glm::vec3 pos;
	glm::vec2 texCoord;
	glm::vec3 normal;
};

enum MeshBufferPositions
{
	POSITION_VB,
	TEXCOORD_VB,
	NORMAL_VB,
	INDEX_VB
};

class Mesh
{
private:
	static const unsigned int NUM_BUFFERS = 4;
	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	//GLuint vertexArrayObject;		// VAO
	GLuint vertexBufferObject;		// VBO
	GLuint elementBufferObject;		// EBO

	// VBOs for bones as lines
	GLuint lineVertexAttrBufferObject;		// vertices
	GLuint lineIndexAttrBufferObject;	// indices 1 2, 1 2, ...

	unsigned int m_numIndices;
	void initMesh(const IndexedModel& model);
public:
	GLuint vertexArrayObject;		// VAO
	GLuint lineVertexArrayObject;		// VAO for bones as lines

	unsigned int cylinderTriangleCount;

    Mesh(const std::string& fileName);
	Mesh(WireframeModel * wireframeModel, Shader * shader);
	Mesh(WireframeModel * wireframeModel, Shader * jointShader, Shader * lineBoneShader);
	Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	Mesh(const float * vertices, unsigned int verticesSize, const unsigned int * indices, unsigned int indicesSize, Shader * shader);
	void draw();
	void drawJointAlone(int jointIndex);
	void drawJointInBone(size_t jointIndex);
	~Mesh();

	std::vector<glm::vec3> positions;
};
