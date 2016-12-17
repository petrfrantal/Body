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

	glm::vec3* GetPos() { return &pos; }
	glm::vec2* GetTexCoord() { return &texCoord; }
	glm::vec3* GetNormal() { return &normal; }

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
	GLuint vertexArrayObject;		// VAO
	GLuint vertexBufferObject;		// VBO
	unsigned int m_numIndices;
	void initMesh(const IndexedModel& model);
public:
    Mesh(const std::string& fileName);
	Mesh(WireframeModel * wireframeModel, Shader * shader);
	Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	void draw();
	void drawJointAlone(size_t jointIndex);
	void drawJointInBone(size_t jointIndex);
	~Mesh();

	std::vector<glm::vec3> positions;
};
